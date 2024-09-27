#ifndef _ASYNCO_
#define _ASYNCO_

#include "engine.hpp"
#include <iostream>

using namespace std;

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

/**
 * Block until the asynchronous call completes
*/
template<typename T>
T await_(future<T>& r) {
    return r.get();
}

/**
 * Block until the asynchronous call completes
*/
template<typename T>
T await_(future<T>&& r) {
    return move(r).get();
}

}
}

#endif