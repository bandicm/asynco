#ifndef _ASYNCO_
#define _ASYNCO_

#include <vector>
#include <memory>
#include <type_traits>
#include <thread>
#include <future>
#include <functional>
#include <iostream>
using namespace std;

#include <boost/asio.hpp>
#if __cplusplus >= 202002L
#include <boost/asio/awaitable.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/use_awaitable.hpp>
#endif
using namespace boost::asio;

#include "timers.hpp"
#include "trigger.hpp"

namespace marcelb {
namespace asynco {

/**
 *  Asynco runtime 
 *  Used for all asynchronous capabilities of this wrapper
 *  Initializes threads and boost::asio::io_context
*/
class Asynco {
    vector<thread> _runners;
    unique_ptr<io_service::work> _work;

    void init_loops_in_threads(uint8_t threads);

public:
    io_context io_ctx;

    /**
     * It starts the thread initialization and the Boost::Asio event loop in each of them
     */

    void run(uint8_t threads = thread::hardware_concurrency());

    /**
     * Starts Boost::Asio event loop in the current thread
     */
    
    void run_on_this();

    /**
     * Waits until all threads have finished working
     */

    void join();

    /**
     * Run the function asynchronously in runtime
    */
    template<class F, class... Args>
    auto async(F&& f, Args&&... args) -> future<invoke_result_t<F, Args...>> {
        using return_type = invoke_result_t<F, Args...>;
        future<return_type> res = io_ctx.post(boost::asio::use_future(bind(forward<F>(f), forward<Args>(args)...)));
        return res;
    }

#if __cplusplus >= 202002L
    /**
     * Run the coroutine in runtime
    */
    template <typename T>
    future<T> async(boost::asio::awaitable<T> _coroutine) {
        promise<T> promise;
        auto future = promise.get_future();

        co_spawn(io_ctx, [_coroutine = move(_coroutine), promise = move(promise)]() mutable -> boost::asio::awaitable<void> {
            try {
                if constexpr (!is_void_v<T>) {
                    T result = co_await move(_coroutine);
                    promise.set_value(move(result));
                } else {
                    co_await move(_coroutine);
                    promise.set_value(); // Za void ne postavljamo rezultat
                }
            } catch (...) {
                promise.set_exception(current_exception()); // Postavljamo izuzetak
            }
        }, boost::asio::detached);

        return future;
    }
#endif

    /**
     * Wait until the asynchronous call completes
    */
    template<typename T>
    T await(future<T>& r, uint16_t time_us = 10) {
        while (r.wait_for(std::chrono::microseconds(time_us)) != future_status::ready) {
            io_ctx.poll_one();
        }
        return r.get(); 
    }

    /**
     * Wait until the asynchronous call completes
    */
    template<typename T>
    T await(future<T>&& r, uint16_t time_us = 10) {
        while (r.wait_for(std::chrono::microseconds(time_us)) != future_status::ready) {
            io_ctx.poll_one();
        }
        return move(r).get();
    }

    /**
     * Run the function asynchronously an wait until completes
    */
    template<class F, class... Args>
    auto await(F&& f, Args&&... args) -> invoke_result_t<F, Args...> {
        return await(
            async(f, args...)
        );
    }

#if __cplusplus >= 202002L
    /**
     * Run the coruotine and wait
     */
    template <typename T>
    T await(boost::asio::awaitable<T> _coroutine) {
        return await(
            async(
                move(_coroutine)
            ));
    }
#endif

    /**
     * Wait until the multiple asynchronous call completes
     * Use only on no-void calls
     */

    template<typename... F>
    auto await(F&&... f) -> tuple<typename decay<decltype(await(f))>::type...> {
        return make_tuple(await(f)...);
    }

    /**
     * Wait until the multiple asynchronous call completes
     * Use only on no-void calls
     */

    template<typename... F>
    auto await(F&... f) -> tuple<typename decay<decltype(await(f))>::type...> {
        return make_tuple(await(f)...);
    }

    /**
     * Initialize the delayed timer
     */

    Timer delayed(function<void()> callback, uint64_t time);

    /**
     * Initialize the periodic timer
     */

    Timer periodic(function<void()> callback, uint64_t time);

    /**
     * Initialize trigger (typed event)
     */

    template<typename... T>
    Trigger<T...> trigger() {
        return Trigger<T...>(*this);
    }


};


}
}

#endif