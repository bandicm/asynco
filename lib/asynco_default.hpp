#ifndef _ASYNCO_DEFAULT_
#define _ASYNCO_DEFAULT_

#include "asynco.hpp"

namespace marcelb {
namespace asynco {

/**
 * Default runtime
 */

extern Asynco Asynco_Default_Runtime;

/**
 * Run the function asynchronously in default runtime
*/
template<class F, class... Args>
auto async_(F&& f, Args&&... args) -> future<invoke_result_t<F, Args...>> {
    return Asynco_Default_Runtime.async(bind(forward<F>(f), forward<Args>(args)...));
}

#if __cplusplus >= 202002L
/**
 * Run the coroutine in default runtime
*/
template <typename T>
std::future<T> async_(boost::asio::awaitable<T> _coroutine) {
    return Asynco_Default_Runtime.async(move(_coroutine));
}
#endif

/**
 * Wait until the asynchronous call completes
*/
template<typename T>
T await_(future<T>& r, uint16_t time_us = 10) {
    return Asynco_Default_Runtime.await(r, time_us);
}

/**
 * Wait until the asynchronous call completes
*/
template<typename T>
T await_(future<T>&& r, uint16_t time_us = 10) {
    return Asynco_Default_Runtime.await(r, time_us);
}

/**
 * Run the function asynchronously an wait until completes
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
 * Wait until the multiple asynchronous call completes
 * Use only on no-void calls
 */

template<typename... F>
auto await_(F&&... f) -> std::tuple<typename std::decay<decltype(Asynco_Default_Runtime.await(f))>::type...> {
    return Asynco_Default_Runtime.await(move(f)...);
}

/**
 * Wait until the multiple asynchronous call completes
 * Use only on no-void calls
 */

template<typename... F>
auto await_(F&... f) -> std::tuple<typename std::decay<decltype(Asynco_Default_Runtime.await(f))>::type...> {
    return Asynco_Default_Runtime.await(f...);;
}

/**
 * Initialize the delayed timer
 */

Timer delayed(function<void()> callback, uint64_t time);

/**
 * Initialize the periodic timer
 */

Timer periodic(function<void()> callback, uint64_t time);

Sleep sleep(uint64_t time);

/**
 * Initialize trigger (typed event)
 */

template<typename... T>
Trigger<T...> trigger() {
    return Trigger<T...>(Asynco_Default_Runtime);
}

/**
 * Get reference of default runtime
 */

Asynco& asynco_default_runtime();

/**
 * Run default runtime
 */

void asynco_default_run();

/**
 * Run default runtime in this thread
 */

void asynco_default_run_on_this();

/**
 * Waits until all threads have finished working
 */

void asynco_default_join();

/**
 * Get reference of boost::asio::io_context
 */

io_context& asynco_default_io_context();

/**
 * Run the function asynchronously in default runtime
*/
#define async_ marcelb::asynco::async_

/**
 * Wait until the asynchronous call completes
*/
#define await_ marcelb::asynco::await_


}
}



#endif