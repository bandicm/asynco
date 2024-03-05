#ifndef _ROTOR_
#define _ROTOT_

#include "runner.hpp"
#include "chrono"

#include "iostream"

using namespace std;
using namespace marcelb;

#ifndef ON_RUNNER
#define ON_RUNNER
runner on_async;
#endif

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
 * Structure for time events
*/

struct time_event {
    function<void()> callback;
    int64_t init;
    int64_t time;
    bool repeat;
    bool stop;
};

/**
 * Event loop for time events
*/

class rotor {
    vector<struct time_event *> tevents;
    mutex te_m;
    bool rotating = true;
    int64_t sampling;

    /**
     * Loop method, started by the constructor in a separate runner
     * It checks the events on the stack and sends the expired ones to the runner
    */
    void loop() {
        while (rotating) {
            for (int i=0; i<tevents.size(); i++) {

                if (tevents[i]->stop) {
                    remove(i);
                    i--;
                }

                else if (expired(tevents[i])) {
                    on_async.put_task(tevents[i]->callback);
                    if (tevents[i]->repeat) {
                        tevents[i]->init = rtime_ms();
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
    bool expired(struct time_event *tevent) {
        return rtime_ms() - tevent->init >= tevent->time;
    }

    /**
     * The method deletes a non-repeating or stopped event from the stack
    */
    void remove(const int& position) {
        lock_guard<mutex> lock(te_m);
        tevents.erase(tevents.begin()+position);
        update_sampling();
    }

    /**
     * Updates the idle time of the loop, according to twice the frequency of available events
    */
    void update_sampling() {
        if (tevents.empty()) {
            sampling = 100;
            return;
        }
        sampling = tevents[0]->time;
        for (int i=0; i<tevents.size(); i++) {
            if (sampling > tevents[i]->time) {
                sampling = tevents[i]->time;
            }
        }
        sampling /= tevents.size()*2;
    }

    public:

    /**
     * Constructor for the rotor, starts the given loop by occupying one runner
    */
    rotor() {
        on_async.put_task( [&] () {
            loop();
        });
    };

    /**
     * Adds a time event to the stack
    */
    void insert(struct time_event *tevent) {
        lock_guard<mutex> lock(te_m);
        tevents.push_back(tevent);
        update_sampling();
    };

    /**
     * Returns the number of active events
    */
    int active() {
        return tevents.size();
    }

    /**
     * Stops all active events and stops the rotor
    */
    ~rotor() {
        for (int i=0; i<tevents.size(); i++) {
            tevents[i]->stop = true;
        }
        rotating = false;
    }

};

/**
 * It is intended that there is only one global declaration
*/
rotor _rotor;

/**
 * A class for all timer functions
*/
class timer_core {
    public:
    struct time_event t_event;

    /**
     * Timer constructor, receives a callback function and time
    */
    timer_core( function<void()> _callback, int64_t _time):
        t_event({ _callback, rtime_ms(), _time, false, false }) {

    }
    /**
     * Stop timer
    */
    void clear() {
        t_event.stop = true;
    }

    /**
     * Destruktor of timer, call stop
    */
    ~timer_core() {
        clear();
    }
};

/**
 * Class interval for periodic execution of the callback in time in ms
*/
class interval : public timer_core {
    public:
    
    /**
     * The constructor receives a callback function and an interval time
    */
    interval( function<void()> _callback, int64_t _time): timer_core(_callback, _time) {
        t_event.repeat = true;
        _rotor.insert(&t_event);
    }
};

/**
 * Class interval for delayed callback execution in ms
*/
class timeout : public timer_core {
    public:

    /**
     * The constructor receives a callback function and a delay time
    */
    timeout( function<void()> _callback, int64_t delay): timer_core(_callback, delay) {
        t_event.repeat = false;
        _rotor.insert(&t_event);
    }
};

}

#endif
