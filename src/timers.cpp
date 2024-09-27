#include "../lib/timers.hpp"

namespace marcelb::asynco {

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

void timer::init() {
    st.async_wait( [this] (const boost::system::error_code&) {
        if (!_stop) {
            callback();
            if (repeate) {
                st = boost::asio::steady_timer(_asynco_engine.io_context, boost::asio::chrono::milliseconds(time));
                init();
            }
            _ticks++;
        }
    });
}

timer::timer (function<void()> _callback, uint64_t _time, bool _repeate) :
    st(_asynco_engine.io_context, boost::asio::chrono::milliseconds(_time)),
    _stop(false),
    repeate(_repeate),
    callback(_callback),
    time(_time) {
        
    init();
}

void timer::stop() {
    _stop = true;
    st.cancel();
}

void timer::now() {
    st.cancel();
}

uint64_t timer::ticks() {
    return _ticks;
}

bool timer::stoped() {
    return _stop;
}

timer::~timer() {
    stop();
}

periodic::periodic(function<void()> callback, uint64_t time) : 
    _timer(make_shared<timer> (callback, time, true)) {
}

void periodic::stop() {
    _timer->stop();
}

void periodic::now() {
    _timer->now();
}

uint64_t periodic::ticks() {
    return _timer->ticks();
}

bool periodic::stoped() {
    return _timer->stoped();
}

periodic::~periodic() {
    stop();
}

delayed::delayed(function<void()> callback, uint64_t time) : 
    _timer(make_shared<timer> (callback, time, false)) {
}

void delayed::stop() {
    _timer->stop();
}

void delayed::now() {
    _timer->now();
}

bool delayed::expired() {
    return bool(_timer->ticks());
}

bool delayed::stoped() {
    return _timer->stoped();
}

delayed::~delayed() {
    stop();
}

mutex p_io, d_io;
vector<shared_ptr<periodic>> periodic_calls_container;
vector<shared_ptr<delayed>> delayed_calls_container;

shared_ptr<periodic> Periodic(function<void()> callback, uint64_t time) {
    shared_ptr<periodic> periodic_ptr(make_shared<periodic>(callback, time));
    async_ ( [&, periodic_ptr](){
        lock_guard<mutex> lock(p_io);
        periodic_calls_container.push_back(periodic_ptr);
        for (uint32_t i=0; i<periodic_calls_container.size(); i++) {
            if (periodic_calls_container[i]->stoped()) {
                periodic_calls_container.erase(periodic_calls_container.begin()+i);
                i--;
            }
        }
    });
    return periodic_ptr;
}

shared_ptr<delayed> Delayed(function<void()> callback, uint64_t time) {
    shared_ptr<delayed> delayed_ptr(make_shared<delayed>(callback, time));
    async_ ( [&, delayed_ptr](){
        lock_guard<mutex> lock(p_io);
        delayed_calls_container.push_back(delayed_ptr);
        for (uint32_t i=0; i<delayed_calls_container.size(); i++) {
            if (delayed_calls_container[i]->stoped() || delayed_calls_container[i]->expired()) {
                delayed_calls_container.erase(delayed_calls_container.begin()+i);
                i--;
            }
        }
    });
    return delayed_ptr;
}



};
