#ifndef _ASYNCO_
#define _ASYNCO_

#include "loop.hpp"

using namespace std;

namespace marcelb {

#ifndef ON_ASYNC
#define ON_ASYNC
AsyncLoop on_async;
#endif

class interval;
class timeout;

class loop_core {
   static vector<interval> intervals;
   static vector<timeout> timeouts;
    time_t sampling;
    mutex i_m, t_m;

    loop_core() {
        on_async.put_task( [this] () {
            loop();
        });
    }

    void run(interval& _interval)  {
        lock_guard<mutex> lock(i_m);
        intervals.push_back(_interval);
        update_sampling();
    }

    void run(timeout& _timeout)  {
        lock_guard<mutex> lock(t_m);
        timeouts.push_back(_timeout);
        update_sampling();
    }

    void loop() {
        while (true) {
            for (auto& _interval : intervals) {
                int64_t now = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
                if (now - _interval.execute >= _interval._duration) {
                    _interval.callback();
                    _interval.execute = now;
                }
            }

            for (int i=0; i<timeouts.size(); i++) {
                int64_t now = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
                if (now - timeouts[i]._construct >= timeouts[i]._duration) {
                    auto& _timeout = timeouts[i];
                    {
                        lock_guard<mutex> lock(t_m);
                        timeouts.erase(timeouts.begin() + i);
                    }
                    _timeout.callback();
                }
            }
            sleep_for(chrono::milliseconds(sampling));
        }
    }

    void update_sampling() {
        sampling = 0;
        for (auto& _interval : intervals)  {
            sampling += _interval._duration;
        }
        for (auto& _timeout : timeouts)  {
            sampling += _timeout._duration;
        }
        sampling /= (intervals.size() + timeouts.size())*2;
    }


};

loop_core co_loop;




class interval {
    public:
    bool run = true;
    function<void()> callback;
    const time_t _duration;
    time_t execute = 0;
    
    // public:
    interval(function<void()> func, const time_t duration): callback(func), _duration(duration) {
        #ifndef ON_ASYNC
            throw string("Not on_async defined!");
        #endif

        auto task = [&] () {
            while (run) {
                // this_thread::sleep_for(chrono::milliseconds(_duration));
                // if (run) {
                    callback();
                // }
            }
        };

        // on_async.put_task(task);
        co_loop.run(this*);
    }

    void clear() {
        run = false;
    }

    ~interval() {
        clear();
    }
};

class timeout {
    public:
    bool run = true;
    function<void()> callback;
    const time_t _duration;
    int64_t _construct =
        chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch())
        .count();
    
    // public:
    timeout(function<void()> f, const time_t duration): callback(f), _duration(duration) {
        #ifndef ON_ASYNC
            throw string("Not on_async defined!");
        #endif

        auto task = [&] () {
            // int64_t _start = 
            //     chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch())
            //     .count();
            // this_thread::sleep_for(chrono::milliseconds(_duration - (_start - _construct)));
            if (run) {
                callback();
            }
        };

        // on_async.put_task(task);
        co_loop.run(this*);
    }

    void clear() {
        run = false;
    }

    ~timeout() {
        clear();
    }
};


template<class F, class... Args>
auto asynco(F&& f, Args&&... args) -> future<typename result_of<F(Args...)>::type> {
    using return_type = typename result_of<F(Args...)>::type;

    future<return_type> res = on_async.put_task(bind(forward<F>(f), forward<Args>(args)...));
    return res;
}

template<typename T>
T wait(future<T> r) {
    return r.get();
}

}

#endif