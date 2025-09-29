#include "../lib/asynco.hpp"

namespace marcelb::asynco {


void Asynco::init_loops_in_threads(uint8_t threads) {
    for (int i=0; i<threads; i++) {
        _runners.push_back(thread ( [this] () {
            io_ctx.run();
        }));
    }
}

void Asynco::run(uint8_t threads) {
    _work = make_unique<io_service::work>(io_ctx);
    init_loops_in_threads(threads);
}

void Asynco::run_on_this() {
    if (!_work) {
        _work = make_unique<io_service::work>(io_ctx);
    }
    io_ctx.run();
}

void Asynco::join() {
    for (auto& runner : _runners) {
        runner.join();
    }
}

Timer Asynco::delayed(function<void()> callback, uint64_t time) {
    return Timer(io_ctx, callback, time, TimerType::Delayed);
}

Timer Asynco::periodic(function<void()> callback, uint64_t time) {
    return Timer(io_ctx, callback, time, TimerType::Periodic);
}

void Asynco::sleep(int _time) {
    promise<void> _promise;
    Timer t = delayed( [&]() {
        _promise.set_value();
    }, _time);

    return await(_promise.get_future());
}

};
