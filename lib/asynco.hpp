#ifndef _ASYNCO_
#define _ASYNCO_

#include "runner.hpp"

using namespace std;

namespace marcelb {

#ifndef ON_RUNNER
#define ON_RUNNER
runner on_async;
#endif

/**
 * Run the function asynchronously
*/
template<class F, class... Args>
auto asynco(F&& f, Args&&... args) -> future<typename result_of<F(Args...)>::type> {
    using return_type = typename result_of<F(Args...)>::type;

    future<return_type> res = on_async.put_task(bind(forward<F>(f), forward<Args>(args)...));
    return res;
}

/**
 * Block until the asynchronous call completes
*/
template<typename T>
T wait(future<T> r) {
    return r.get();
}

}

#endif