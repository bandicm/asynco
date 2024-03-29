#ifndef _ROTOR_
#define _ROTOT_

#include "asynco.hpp"
#include <chrono>

using namespace std;
using namespace marcelb;
using namespace asynco;

namespace marcelb {
namespace asynco {

/**
 * Get the time in ms from the epoch
*/

int64_t rtime_ms() {
    return chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now()
        .time_since_epoch())
        .count();
}

/**
 * Get the time in us from the epoch
*/

int64_t rtime_us() {
    return chrono::duration_cast<chrono::microseconds>(chrono::system_clock::now()
        .time_since_epoch())
        .count();
}

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
    void init() {
        st.async_wait( [this] (const boost::system::error_code&) {
            if (!_stop) {
                callback();
                if (repeate) {
                    st = boost::asio::steady_timer(_asynco_engine.io_context, boost::asio::chrono::milliseconds(time));
                    init();
                }
                _ticks++;
            }
        });
    }
   
    public:

    /**
     * The constructor creates the steady_timer and accompanying variables and runs a method to initialize the timer
    */
    timer (function<void()> _callback, uint64_t _time, bool _repeate) :
        st(_asynco_engine.io_context, boost::asio::chrono::milliseconds(_time)),
        _stop(false),
        repeate(_repeate),
        callback(_callback),
        time(_time) {
            
        init();
    }

    /**
     * Stop timer
     * The stop flag is set and timer remove it from the queue
    */
    void stop() {
        _stop = true;
        st.cancel();
    }
    
    /**
     * Run callback now
     * Forces the callback function to run independently of the timer
    */
    void now() {
        st.cancel();
    }

    /**
     * Get the number of times the timer callback was runned
    */
    uint64_t ticks() {
        return _ticks;
    }

    /**
     * The logic status of the timer stop state
    */
    bool stoped() {
        return _stop;
    }

    /**
     * The destructor stops the timer
    */
    ~timer() {
        stop();
    }
};

/**
 * Class interval for periodic execution of the callback in time in ms
*/
class interval {
    shared_ptr<timer> _timer;

    public:
    /**
     * Constructor initializes a shared pointer of type timer
    */
    interval(function<void()> callback, uint64_t time) : 
        _timer(make_shared<timer> (callback, time, true)) {
    }

    /**
     * Stop interval
     * The stop flag is set and interval remove it from the queue
    */
    void stop() {
        _timer->stop();
    }

    /**
     * Run callback now
     * Forces the callback function to run independently of the interval
    */
    void now() {
        _timer->now();
    }
    
    /**
     * Get the number of times the interval callback was runned
    */
    uint64_t ticks() {
        return _timer->ticks();
    }

    /**
     * The logic status of the interval stop state
    */
    bool stoped() {
        return _timer->stoped();
    }
    
    /**
     * The destructor stops the interval
    */
    ~interval() {
        stop();
    }
};

/**
 * Class timeout for delayed callback execution in ms
*/
class timeout {
    shared_ptr<timer> _timer;

    public:
    /**
     * Constructor initializes a shared pointer of type timer
    */
    timeout(function<void()> callback, uint64_t time) : 
        _timer(make_shared<timer> (callback, time, false)) {
    }
    
    /**
     * Stop timeout
     * The stop flag is set and timeout remove it from the queue
    */
    void stop() {
        _timer->stop();
    }

    /**
     * Run callback now
     * Forces the callback function to run independently of the timeout
    */
    void now() {
        _timer->now();
    }

    /**
     * Get the number of times the timeout callback was runned
    */
    bool expired() {
        return bool(_timer->ticks());
    }

    /**
     * The logic status of the timeout stop state
    */
    bool stoped() {
        return _timer->stoped();
    }

    /**
     * The destructor stops the timeout
    */
    ~timeout() {
        stop();
    }

};

}
}

#endif
