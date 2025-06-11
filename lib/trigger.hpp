#ifndef _ASYNCO_TRIGGER_
#define _ASYNCO_TRIGGER_

#include <map>
#include <vector>
#include <string>
#include <functional>

using namespace std;

#include "asynco.hpp"
namespace marcelb {
namespace asynco {

class Asynco;

/**
 * Trigger class, for event-driven programming.
 * These events are typed according to the arguments of the callback function
*/
template<typename... T>
class Trigger {
    private:
    Asynco& engine;
    mutex m_eve;
    unordered_map<string, vector<function<void(T...)>>> triggers;

    public:
    Trigger(Asynco& _engine)
    : engine(_engine) {}

    /**
     * Defines event by key, and callback function
    */
    void on(const string& key, function<void(T...)> callback) {
        lock_guard _off(m_eve);
        triggers[key].push_back(callback);
    }


    /**
     * It emits an event and sends a callback function saved according to the key with the passed parameters
    */
    template<typename... Args>
    void tick(const string& key, Args... args) {
        auto it_eve = triggers.find(key);
        if (it_eve != triggers.end()) {
            for (uint i =0; i<it_eve->second.size(); i++) {
                auto callback = bind(it_eve->second[i], forward<Args>(args)...); 
                engine.async(callback);
            }
        }
    }

    /**
     * Remove an Trigger listener from an event
    */
    void off(const string& key)  {
        lock_guard _off(m_eve);
        triggers.erase(key);
    }

    /**
     * Remove all Trigger listener
    */
    void off() {
        lock_guard _off(m_eve);
        triggers.clear();
    }

    /**
     * Get num of listeners by an Trigger key
    */
    unsigned int listeners(const string& key) {
        return triggers[key].size();
    }

    /**
     * Get num of all listeners
    */
    unsigned int listeners() {
        unsigned int listeners = 0;
        for (auto& ev : triggers) {
            listeners += ev.second.size();
        }
        return listeners;
    }


};

}
}

#endif