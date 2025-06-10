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

void Timer::init() {
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

Timer::Timer (function<void()> _callback, uint64_t _time, bool _repeate) :
    st(_asynco_engine.io_context, boost::asio::chrono::milliseconds(_time)),
    _stop(false),
    repeate(_repeate),
    callback(_callback),
    time(_time) {
        
    init();
}

void Timer::stop() {
    _stop = true;
    st.cancel();
}

void Timer::now() {
    st.cancel();
}

uint64_t Timer::ticks() {
    return _ticks;
}

bool Timer::stoped() {
    return _stop;
}

Timer::~Timer() {
    stop();
}

Periodic::Periodic(function<void()> callback, uint64_t time) : 
    _timer(make_shared<Timer> (callback, time, true)) {
}

void Periodic::stop() {
    _timer->stop();
}

void Periodic::now() {
    _timer->now();
}

uint64_t Periodic::ticks() {
    return _timer->ticks();
}

bool Periodic::stoped() {
    return _timer->stoped();
}

Periodic::~Periodic() {
    stop();
}

Delayed::Delayed(function<void()> callback, uint64_t time) : 
    _timer(make_shared<Timer> (callback, time, false)) {
}

void Delayed::stop() {
    _timer->stop();
}

void Delayed::now() {
    _timer->now();
}

bool Delayed::expired() {
    return bool(_timer->ticks());
}

bool Delayed::stoped() {
    return _timer->stoped();
}

Delayed::~Delayed() {
    stop();
}


};
