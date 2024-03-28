#ifndef _ROTOR_
#define _ROTOT_

#include "runner.hpp"
#include "chrono"
#include <memory>

#include "iostream"

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

int64_t rtime_us() {
    return chrono::duration_cast<chrono::microseconds>(chrono::system_clock::now()
        .time_since_epoch())
        .count();
}

namespace {

/**
 * Intern class for timer async loop
*/
class timer_core {
    public:
    mutex hangon;
    condition_variable cv;
    function<void()> callback;
    int64_t time;
    int64_t next;
    bool repeat;
    bool stop;

    /**
     * Timer constructor, receives a callback function and time
    */
    timer_core( function<void()> _callback, int64_t _time, bool _repeat):
        callback(_callback), time(_time*1000), repeat(_repeat), stop(false) {
            next = rtime_us() + time;
    }

    /**
     * Stop timer
    */
    void clear() {
        // lock_guard<mutex> hang(hangon);
        stop = true;
        cv.notify_one();
    }

    /**
     * Destruktor of timer, call stop
    */
    ~timer_core() {
        clear();
    }
};

/**
 * Event loop for time events
*/

class rotor {
    vector<shared_ptr<timer_core>> tcores;
    mutex te_m;
    bool rotating = true;
    int64_t sampling;

    condition_variable te_cv;

    /**
     * Loop method, started by the constructor in a separate runner
     * It checks the events on the stack and sends the expired ones to the runner
    */
    void loop() {
        while (rotating) {
            vector<shared_ptr<timer_core>>::iterator next_tc;
            shared_ptr<timer_core> next_ptr;

            {
            unique_lock<mutex> te_l(te_m);
            te_cv.wait(te_l, [this]{ return !tcores.empty() || rotating; });
            if (!rotating) {
                break;
            }

            next_tc = min_element( tcores.begin(), tcores.end(),
                [](shared_ptr<timer_core> a, shared_ptr<timer_core> b ) {
                    return a->next < b->next;
                }
            );

            next_ptr = *next_tc;
            }

            unique_lock<mutex> next_l(next_ptr->hangon);
            next_ptr->cv.wait_for(next_l, chrono::microseconds(next_ptr->next - rtime_us()),  [&next_ptr] () {
                return next_ptr->stop;
            });

            if (next_ptr->stop) {
                remove(next_tc);
            } else {
                _asyncon.put_task(next_ptr->callback);
                if (next_ptr->repeat) {
                    next_ptr->next += next_ptr->time;
                }
                else {
                    remove(next_tc);
                }
            }

        } 
    }
    
    /**
     * The method deletes a non-repeating or stopped event from the stack
    */
    void remove(vector<shared_ptr<timer_core>>::iterator it) {
        lock_guard<mutex> lock(te_m);
        tcores.erase(it);
        // te_cv.notify_one();
    }

    public:

    /**
     * Constructor for the rotor, starts the given loop by occupying one runner
    */
    rotor() {
        _asyncon.put_task( [&] () {
            loop();
        });
    };

    /**
     * Adds a time event to the stack
    */
    void insert(shared_ptr<timer_core> tcore) {
        lock_guard<mutex> lock(te_m);
        tcores.push_back(tcore);
        te_cv.notify_one();
    };

    /**
     * Returns the number of active events
    */
    int active() {
        return tcores.size();
    }

    /**
     * Stops all active events and stops the rotor
    */
    ~rotor() {
        for (int i=0; i<tcores.size(); i++) {
            tcores[i]->clear();
        }
        rotating = false;
    }

};

}


/**
 * It is intended that there is only one global declaration
*/
static rotor _rotor;

/**
 * Core class for pure async timer functions
*/

class _timer_intern {
    shared_ptr<timer_core> tcore;
    public:

    _timer_intern(function<void()> _callback, int64_t _time, bool repeat) {
        tcore = make_shared<timer_core>(_callback, _time, repeat);
        _rotor.insert(tcore);
    }
    
    /**
     * Stop interval
    */
    void clear() {
        tcore->clear();
    }
};

/**
 * Class interval for periodic execution of the callback in time in ms
*/
class interval : public _timer_intern {
    public:

    /**
     * The constructor receives a callback function and an interval time
    */
    interval( function<void()> _callback, int64_t _time):
        _timer_intern(_callback, _time, true) {
    }
};

/**
 * Class interval for delayed callback execution in ms
*/
class timeout : public _timer_intern {
    public:

    /**
     * The constructor receives a callback function and a delay time
    */
    timeout( function<void()> _callback, int64_t delay):
        _timer_intern(_callback, delay, false) {
    }

};

}
}

#endif
