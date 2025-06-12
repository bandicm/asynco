#ifndef _ASYNCO_TIMERS_
#define _ASYNCO_TIMERS_

#include <chrono>
#include <iostream>
using namespace std;

#include <boost/asio.hpp>
using namespace boost::asio;

namespace marcelb {
namespace asynco {

/**
 * Get the time in ms from the epoch
*/

int64_t rtime_ms();

/**
 * Get the time in us from the epoch
*/

int64_t rtime_us();

enum TimerType {
    Delayed,
    Periodic
};

/**
 * Core timer class for construct time async functions
*/
class Timer {
    io_context& io_ctx;
    steady_timer st;
    bool _stop = false;
    TimerType type;
    function<void()> callback;
    uint64_t time;
    uint64_t _ticks = 0;

    /**
     * A method to assign a callback wrapper and a reinitialization algorithm
    */
    void init();   
    public:

    /**
     * The constructor creates the steady_timer and accompanying variables and runs a method to initialize the timer
    */
    Timer (io_context& io_ctx, function<void()> _callback, uint64_t _time, TimerType _type = TimerType::Delayed);

    /**
     * Stop timer
     * The stop flag is set and timer remove it from the queue
    */
    void stop();
    
    /**
     * Run callback now
     * Forces the callback function to run independently of the timer
    */
    void now();

    /**
     * Get the number of times the timer callback was runned
    */
    uint64_t ticks();

    /**
     * Get is the delayed callback runned
    */
    bool expired();

    /**
     * The logic status of the timer stop state
    */
    bool stoped();
    
    /**
     * The destructor stops the timer
    */
    ~Timer();
};


}
}

#endif
