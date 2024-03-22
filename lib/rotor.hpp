#ifndef _ROTOR_
#define _ROTOT_

#include "runner.hpp"
#include "chrono"
#include <memory>

#include "iostream"

using namespace std;
using namespace marcelb;

namespace marcelb {

/**
 * Get the time in ms from the epoch
*/

int64_t rtime_ms() {
    return chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now()
        .time_since_epoch())
        .count();
}

/**
 * Intern class for timer async loop
*/
class timer_core {
    public:
    mutex hangon;
    function<void()> callback;
    int64_t init;
    int64_t time;
    bool repeat;
    bool stop;

    /**
     * Timer constructor, receives a callback function and time
    */
    timer_core( function<void()> _callback, int64_t _time, bool _repeat):
        callback(_callback), init(rtime_ms()), time(_time), repeat(_repeat), stop(false) {
    }

    /**
     * Stop timer
    */
    void clear() {
        lock_guard<mutex> hang(hangon);
        stop = true;
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

    /**
     * Loop method, started by the constructor in a separate runner
     * It checks the events on the stack and sends the expired ones to the runner
    */
    void loop() {
        while (rotating) {
            for (int i=0; i<tcores.size(); i++) {

                if (tcores[i]->stop) {
                    remove(i);
                    i--;
                }

                else if (expired(tcores[i])) {
                    _asyncon.put_task(tcores[i]->callback);
                    if (tcores[i]->repeat) {
                        tcores[i]->init = rtime_ms();
                    }
                    else {
                        remove(i);
                        i--;
                    }
                }
            }
            this_thread::sleep_for(chrono::milliseconds(sampling));
        } 
    }

    /**
     * The method checks whether the time event has expired
    */
    bool expired(shared_ptr<timer_core> tcore) {
        return rtime_ms() - tcore->init >= tcore->time;
    }

    /**
     * The method deletes a non-repeating or stopped event from the stack
    */
    void remove(const int& position) {
        lock_guard<mutex> lock(te_m);
        tcores.erase(tcores.begin()+position);
        update_sampling();
    }

    /**
     * Updates the idle time of the loop, according to twice the frequency of available events
    */
    void update_sampling() {
        if (tcores.empty()) {
            sampling = 100;
            return;
        }
        sampling = tcores[0]->time;
        for (int i=0; i<tcores.size(); i++) {
            if (sampling > tcores[i]->time) {
                sampling = tcores[i]->time;
            }
        }
        sampling /= tcores.size()*2;
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
        update_sampling();
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

#endif
