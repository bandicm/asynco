#ifndef _ASYNCO_DEFAULT_
#define _ASYNCO_DEFAULT_

#include "asynco.hpp"

namespace marcelb {
namespace asynco {


extern Asynco Asynco_Default_Runtime;
// Asynco& Asynco_Default_Runtime();

/**
 * Run the function asynchronously
*/
template<class F, class... Args>
auto async_(F&& f, Args&&... args) -> future<invoke_result_t<F, Args...>> {
    cout << "async_default" << endl;

    return Asynco_Default_Runtime.async(bind(forward<F>(f), forward<Args>(args)...));
}

#if __cplusplus >= 202002L
/**
 * Run the coroutine
*/
template <typename T>
std::future<T> async_(boost::asio::awaitable<T> _coroutine) {
    return Asynco_Default_Runtime.async(move(_coroutine));
}
#endif

/**
 * Block until the asynchronous call completes - dont block asynco engine loop
*/
template<typename T>
T await_(future<T>& r, uint16_t time_us = 10) {
    return Asynco_Default_Runtime.await(r, time_us);
}

/**
 * Block until the asynchronous call completes - dont block asynco engine loop
*/
template<typename T>
T await_(future<T>&& r, uint16_t time_us = 10) {
    return Asynco_Default_Runtime.await(r, time_us);
}

/**
 * Run the function asynchronously an block until completes
*/
template<class F, class... Args>
auto await_(F&& f, Args&&... args) -> invoke_result_t<F, Args...> {
    return Asynco_Default_Runtime.await(bind(forward<F>(f), forward<Args>(args)...));
}


#if __cplusplus >= 202002L
/**
 * Run the coruotine and wait
 */
template <typename T>
T await_(boost::asio::awaitable<T> _coroutine) {
    return Asynco_Default_Runtime.await(move(_coroutine));
}
#endif

/**
 * Block until the multiple asynchronous call completes
 * Use only on no-void calls
 */

template<typename... F>
auto await_(F&&... f) -> std::tuple<typename std::decay<decltype(Asynco_Default_Runtime.await(f))>::type...> {
    return Asynco_Default_Runtime.await(move(f)...);
}

/**
 * Block until the multiple asynchronous call completes
 * Use only on no-void calls
 */

template<typename... F>
auto await_(F&... f) -> std::tuple<typename std::decay<decltype(Asynco_Default_Runtime.await(f))>::type...> {
    return Asynco_Default_Runtime.await(f...);;
}

Timer delayed(function<void()> callback, uint64_t time);

Timer periodic(function<void()> callback, uint64_t time);

template<typename... T>
Trigger<T...> trigger() {
    return Trigger<T...>(Asynco_Default_Runtime);
}

/**
 * Alternative names of functions - mostly for the sake of more beautiful coloring of the code
 */
#define async_ marcelb::asynco::async_
#define await_ marcelb::asynco::await_

#if __cplusplus >= 202002L
#define asyncable boost::asio::awaitable
#endif

}
}



#endif