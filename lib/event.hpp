#ifndef _EVENT_
#define _EVENT_

#include <iostream>
#include <map>
#include <string>
#include <functional>

using namespace std;

namespace marcelb {

class event {
    map<string, function<void(const tuple<>&)>> events;
public:

    template <typename... Args>
    void on(const string& key, function<void(Args...)> f) {
        // events[key] = [f](Args... args) {
        //     f(args...);
        // };
    }

    template <typename... Args>
    void emit(const string& key, Args&&... args) {
        if (events.find(key) == events.end()) {
            cout << "No defined listener for event: " << key << endl;
            return;
        }
        else {
            for (auto& func : events[key]) {
                func(forward<Args>(args)...);
            }
        }
    }
};

}

#endif