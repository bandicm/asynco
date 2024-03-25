#ifndef _ASYNCO_
#define _ASYNCO_

#include "runner.hpp"

using namespace std;

namespace marcelb {
namespace asynco {

/**
 * Run the function asynchronously
*/
template<class F, class... Args>
auto atask(F&& f, Args&&... args) -> future<typename result_of<F(Args...)>::type> {
    using return_type = typename result_of<F(Args...)>::type;

    future<return_type> res = _asyncon.put_task(bind(forward<F>(f), forward<Args>(args)...));
    return res;
}

/**
 * Block until the asynchronous call completes
*/
template<typename T>
T wait(future<T>& r) {
    return r.get();
}

/**
 * Block until the asynchronous call completes
*/
template<typename T>
T wait(future<T>&& r) {
    return move(r).get();
}

}
}

#endif