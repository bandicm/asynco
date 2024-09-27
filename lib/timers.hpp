#ifndef _ASYNCO_TIMERS_
#define _ASYNCO_TIMERS_

#include <chrono>
using namespace std;

#include "asynco.hpp"

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

/**
 * Core timer class for construct time async functions
*/
class timer {
    boost::asio::steady_timer st;
    bool _stop = false;
    bool repeate;
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
    timer (function<void()> _callback, uint64_t _time, bool _repeate);

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
     * The logic status of the timer stop state
    */
    bool stoped();
    /**
     * The destructor stops the timer
    */
    ~timer();
};

/**
 * Class periodic for periodic execution of the callback in time in ms
*/
class periodic {
    shared_ptr<timer> _timer;

    public:

    /**
     * Constructor initializes a shared pointer of type timer
    */
    periodic(function<void()> callback, uint64_t time);

    /**
     * Stop periodic
     * The stop flag is set and periodic remove it from the queue
    */
    void stop();

    /**
     * Run callback now
     * Forces the callback function to run independently of the periodic
    */
    void now();
    
    /**
     * Get the number of times the periodic callback was runned
    */
    uint64_t ticks();
    /**
     * The logic status of the periodic stop state
    */
    bool stoped();
    
    /**
     * The destructor stops the periodic
    */
    ~periodic();
};

/**
 * Class delayed for delayed callback execution in ms
*/
class delayed {
    shared_ptr<timer> _timer;

    public:

    /**
     * Constructor initializes a shared pointer of type timer
    */
    delayed(function<void()> callback, uint64_t time);
    
    /**
     * Stop delayed
     * The stop flag is set and delayed remove it from the queue
    */
    void stop();

    /**
     * Run callback now
     * Forces the callback function to run independently of the delayed
    */
    void now();

    /**
     * Get is the delayed callback runned
    */
    bool expired();
    /**
     * The logic status of the delayed stop state
    */
    bool stoped();
    /**
     * The destructor stops the delayed
    */
    ~delayed();

};

shared_ptr<periodic> Periodic(function<void()> callback, uint64_t time);
shared_ptr<delayed> Delayed(function<void()> callback, uint64_t time);


}
}

#endif
