#ifndef _TIMERS_
#define _TIMERS_

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
 * Class periodic for periodic execution of the callback in time in ms
*/
class periodic {
    shared_ptr<timer> _timer;

    public:
    /**
     * Constructor initializes a shared pointer of type timer
    */
    periodic(function<void()> callback, uint64_t time) : 
        _timer(make_shared<timer> (callback, time, true)) {
    }

    /**
     * Stop periodic
     * The stop flag is set and periodic remove it from the queue
    */
    void stop() {
        _timer->stop();
    }

    /**
     * Run callback now
     * Forces the callback function to run independently of the periodic
    */
    void now() {
        _timer->now();
    }
    
    /**
     * Get the number of times the periodic callback was runned
    */
    uint64_t ticks() {
        return _timer->ticks();
    }

    /**
     * The logic status of the periodic stop state
    */
    bool stoped() {
        return _timer->stoped();
    }
    
    /**
     * The destructor stops the periodic
    */
    ~periodic() {
        stop();
    }
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
    delayed(function<void()> callback, uint64_t time) : 
        _timer(make_shared<timer> (callback, time, false)) {
    }
    
    /**
     * Stop delayed
     * The stop flag is set and delayed remove it from the queue
    */
    void stop() {
        _timer->stop();
    }

    /**
     * Run callback now
     * Forces the callback function to run independently of the delayed
    */
    void now() {
        _timer->now();
    }

    /**
     * Get is the delayed callback runned
    */
    bool expired() {
        return bool(_timer->ticks());
    }

    /**
     * The logic status of the delayed stop state
    */
    bool stoped() {
        return _timer->stoped();
    }

    /**
     * The destructor stops the delayed
    */
    ~delayed() {
        stop();
    }

};

delayed giveup(function<void()> callback, uint64_t time)  {
    delayed timeout ( []() {
        cout << "Timeout " << endl;
        throw runtime_error("Time expired");
    }, time);
    _asynco_engine.io_context.post([&] () {
        cout << "execute " << endl;

        callback();
        cout << "clear " << endl;
        
        timeout.stop();
    });

    return timeout;
}

}
}

#endif
