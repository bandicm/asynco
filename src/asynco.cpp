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


Sleep Asynco::sleep(uint64_t time) {
    return Sleep(time, *this);
}

// Sleep

Sleep::Sleep(uint64_t _time, Asynco &runtime): runtime(runtime) {
    future = promise.get_future();

    timer = std::make_shared<Timer>(runtime.io_ctx, [this](){
        this->promise.set_value();
    }, _time, TimerType::Delayed);
}


// #ifdef _ASYNCO_DEFAULT_
// Sleep::Sleep(uint64_t _time): runtime(asynco_default_runtime()) {
//     future = promise.get_future();

//     timer = std::make_shared<Timer>(runtime.io_ctx, [this](){
//         this->promise.set_value();
//     }, _time, TimerType::Delayed);
// }
// #endif


void Sleep::await() {
#ifdef _ASYNCO_DEFAULT_
    await_(future);
#else
    runtime.await(future);
#endif
}

};
