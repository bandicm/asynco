#include "../lib/timers.hpp"

namespace marcelb::asynco {

int64_t rtime_ms() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()
        .time_since_epoch())
        .count();
}

int64_t rtime_us() {
    return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now()
        .time_since_epoch())
        .count();
}

void Timer::init() {
    st.async_wait( [this] (const boost::system::error_code&) {
        if (!_stop) {
            callback();
            if (type == TimerType::Periodic) {
                st = steady_timer(io_ctx, boost::asio::chrono::milliseconds(time));
                init();
            }
            _ticks++;
        }
    });
}

Timer::Timer (io_context& _io_ctx, function<void()> _callback, uint64_t _time, TimerType _type):
    io_ctx(_io_ctx),
    st(io_ctx, boost::asio::chrono::milliseconds(_time)),
    _stop(false),
    type(_type),
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

bool Timer::expired() {
    return bool(_ticks);
}

bool Timer::stoped() {
    return _stop;
}

Timer::~Timer() {
    stop();
}

};
