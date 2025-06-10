#include "../lib/asynco.hpp"

namespace marcelb::asynco {


void Asynco::init_loops_in_threads(uint8_t threads) {
    for (int i=0; i<threads; i++) {
        cout << "loops init " << endl;

        _runners.push_back(thread ( [this] () {
            io_ctx.run();
        }));
    }
}

// Asynco::Asynco(uint8_t threads){//:
//     // _work(io_service::work(io_ctx)) {
//     run(threads);
// }

void Asynco::run(uint8_t threads) {
    _work = make_unique<io_service::work>(io_ctx);
    cout << "Asynco" << endl;
    init_loops_in_threads(threads);
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


};
