#ifndef _ASYNCO_ENGINE_
#define _ASYNCO_ENGINE_

#include <vector>
#include <memory>
using namespace std;

#include <boost/asio.hpp>

namespace marcelb {
namespace asynco {

#define HW_CONCURRENCY_MINIMAL 4

/**
 * Internal anonymous class for initializing the ASIO context and thread pool
 * !!! It is anonymous to protect against use in the initialization of other objects of the same type !!!
*/
class Engine {
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

};


extern Engine _asynco_engine;



}
}



#endif



