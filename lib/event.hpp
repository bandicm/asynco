#ifndef _EVENT_
#define _EVENT_

#include <iostream>
#include <map>
#include <string>
#include <functional>
#include "runner.hpp"

using namespace std;

namespace marcelb {

#ifndef ON_RUNNER
#define ON_RUNNER
runner on_async;
#endif

/**
 * Event class, for event-driven programming.
 * These events are typed according to the arguments of the callback function
*/
template<typename... T>
class event {
    private:
    unordered_map<string, function<void(T...)>> events;

    public:

    /**
     * Defines event by key, and callback function
    */
    void on(const string& key, function<void(T...)> callback) {
        events[key] = callback;
    }

    /**
     * It emits an event and sends a callback function saved according to the key with the passed parameters
    */
    template<typename... Args>
    void emit(const string& key, Args... args) {
        auto it = events.find(key);
        if (it != events.end()) {
            auto callback = bind(it->second, forward<Args>(args)...);
            on_async.put_task(callback);
        }
    }


};

}

#endif