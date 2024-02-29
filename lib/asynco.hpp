#ifndef _ASYNCO_
#define _ASYNCO_

#include "loop.hpp"

using namespace std;

namespace marcelb {

#ifndef ON_ASYNC
#define ON_ASYNC
AsyncLoop on_async;
#endif

class interval {
    bool run = true;
    function<void()> callback;
    const time_t _duration;
    
    public:
    interval(function<void()> func, const time_t duration): callback(func), _duration(duration) {
        #ifndef ON_ASYNC
        throw string("Not on_async defined!");
        #endif

        auto task = [&] () {
            while (run) {
                this_thread::sleep_for(chrono::milliseconds(_duration));
                if (run) {
                    callback();
                }
            }
        };

        on_async.put_task(task);
    }

    void clear() {
        run = false;
    }

    ~interval() {
        clear();
    }
};

class timeout {
    bool run = true;
    function<void()> callback;
    const time_t _duration;
    
    public:
    timeout(function<void()> f, const time_t duration): callback(f), _duration(duration) {
        #ifndef ON_ASYNC
        throw string("Not on_async defined!");
        #endif

        auto task = [&] () {
            this_thread::sleep_for(chrono::milliseconds(_duration));
            if (run) {
                callback();
            }
        };

        on_async.put_task(task);
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