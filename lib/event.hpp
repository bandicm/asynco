#ifndef _EVENT_
#define _EVENT_

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <functional>
#include "runner.hpp"

using namespace std;

namespace marcelb {
namespace asynco {
namespace events {

/**
 * Event class, for event-driven programming.
 * These events are typed according to the arguments of the callback function
*/
template<typename... T>
class event {
    private:
    mutex m_eve;
    unordered_map<string, vector<function<void(T...)>>> events;

    public:

    /**
     * Defines event by key, and callback function
    */
    void on(const string& key, function<void(T...)> callback) {
        lock_guard _off(m_eve);
        events[key].push_back(callback);
    }

    /**
     * It emits an event and sends a callback function saved according to the key with the passed parameters
    */
    template<typename... Args>
    void emit(const string& key, Args... args) {
        auto it_eve = events.find(key);
        if (it_eve != events.end()) {
            for (uint i =0; i<it_eve->second.size(); i++) {
                auto callback = bind(it_eve->second[i], forward<Args>(args)...); 
                _asyncon.put_task(callback);
            }
        }
    }

    /**
     * Remove an event listener from an event
    */
    void off(const string& key) {
        lock_guard _off(m_eve);
        events.erase(key);
    }


};

}
}
}

#endif