#ifndef _ASYNCO_
#define _ASYNCO_

#include "engine.hpp"
#include <iostream>
using namespace std;

#if __cplusplus >= 202002L
#include <boost/asio/awaitable.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/use_awaitable.hpp>
#endif

namespace marcelb {
namespace asynco {

/**
 * Run the function asynchronously
*/
template<class F, class... Args>
auto async_(F&& f, Args&&... args) -> future<typename result_of<F(Args...)>::type> {
    using return_type = typename result_of<F(Args...)>::type;
    future<return_type> res = _asynco_engine.io_context.post(boost::asio::use_future(bind(forward<F>(f), forward<Args>(args)...)));
    return res;
}

#if __cplusplus >= 202002L
/**
 * Run the coroutine
*/
template <typename T>
std::future<T> async_(boost::asio::awaitable<T> _coroutine) {
    std::promise<T> promise;
    auto future = promise.get_future();

    co_spawn(_asynco_engine.io_context, [_coroutine = std::move(_coroutine), promise = std::move(promise)]() mutable -> boost::asio::awaitable<void> {
        try {
            if constexpr (!std::is_void_v<T>) {
                T result = co_await std::move(_coroutine);
                promise.set_value(std::move(result));
            } else {
                co_await std::move(_coroutine);
                promise.set_value(); // Za void ne postavljamo rezultat
            }
        } catch (...) {
            promise.set_exception(std::current_exception()); // Postavljamo izuzetak
        }
    }, boost::asio::detached);

    return future;
}

#endif

/**
 * Block until the multiple asynchronous call completes
 * Use only on no-void calls
 */

template<typename... F>
auto await_(F&&... f) -> std::tuple<typename std::decay<decltype(f.get())>::type...> {
    return std::make_tuple(move(f).get()...);
}

/**
 * Block until the multiple asynchronous call completes
 * Use only on no-void calls
 */

template<typename... F>
auto await_(F&... f) -> std::tuple<typename std::decay<decltype(f.get())>::type...> {
    return std::make_tuple(f.get()...);
}

/**
 * Block until the asynchronous call completes - dont block asynco engine loop
*/
template<typename T>
T await_(future<T>& r, uint16_t time_us = 10) {
    while (r.wait_for(std::chrono::microseconds(time_us)) != std::future_status::ready) {
        _asynco_engine.io_context.poll_one();
    }
    return r.get(); 
}

/**
 * Block until the asynchronous call completes - dont block asynco engine loop
*/
template<typename T>
T await_(future<T>&& r, uint16_t time_us = 10) {
    while (r.wait_for(std::chrono::microseconds(time_us)) != std::future_status::ready) {
        _asynco_engine.io_context.poll_one();
    }
    return move(r).get();
}

/**
 * Run the function asynchronously an block until completes
*/
template<class F, class... Args>
auto await_(F&& f, Args&&... args) -> typename result_of<F(Args...)>::type {
    return await_(
        async_(f, args...)
    );
}


#if __cplusplus >= 202002L

/**
 * Run the coruotine and wait
 */
template <typename T>
T await_(boost::asio::awaitable<T> _coroutine) {
    return await_(
        async_(
            move(_coroutine)
        ));
}

#endif

}
}

#endif