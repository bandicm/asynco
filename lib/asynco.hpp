#ifndef _ASYNCO_
#define _ASYNCO_

#include <boost/asio.hpp>
#include <iostream>

using namespace std;

namespace marcelb {
namespace asynco {

#define HW_CONCURRENCY_MINIMAL 4


/**
 * Internal anonymous class for initializing the ASIO context and thread pool
 * !!! It is anonymous to protect against use in the initialization of other objects of the same type !!!
*/
class {
    public:
    boost::asio::io_context io_context;

    void run() {
        for (auto& runner : runners) {
            runner.join();
        }
    }
    
    private:

    unique_ptr<boost::asio::io_service::work> work { [&] () {
        return new boost::asio::io_service::work(io_context);
    } ()};

    vector<thread> runners { [&] () {
        vector<thread> _runs;
        unsigned int num_of_runners;
        #ifdef NUM_OF_RUNNERS
            num_of_runners = NUM_OF_RUNNERS;
        #else
            num_of_runners = thread::hardware_concurrency();
            if (num_of_runners < HW_CONCURRENCY_MINIMAL) {
                num_of_runners = HW_CONCURRENCY_MINIMAL;
            }
        #endif

        for (int i=0; i<num_of_runners; i++) {
            _runs.push_back(thread ( [this] () {
                io_context.run();
            }));
        }

        return _runs;
    } ()};

} _asynco_engine;

/**
 * Run the function asynchronously
*/
template<class F, class... Args>
auto nonsync(F&& f, Args&&... args) -> future<typename result_of<F(Args...)>::type> {
    using return_type = typename result_of<F(Args...)>::type;
    future<return_type> res = _asynco_engine.io_context.post(boost::asio::use_future(bind(forward<F>(f), forward<Args>(args)...)));
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