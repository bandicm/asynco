#ifndef _EVENT_
#define _EVENT_

#include <iostream>
#include <map>
#include <string>
#include <functional>
#include "loop.hpp"

using namespace std;

namespace marcelb {

#ifndef ON_ASYNC
#define ON_ASYNC
AsyncLoop on_async;
#endif


template<typename... T>
class event {
    private:
    unordered_map<string, function<void(T...)>> events;

    public:
    void on(const string& key, function<void(T...)> callback) {
        events[key] = callback;
    }

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