
#include "../lib/asynco_default.hpp"

namespace marcelb::asynco {

Asynco Asynco_Default_Runtime;

Timer delayed(function<void()> callback, uint64_t time) {
    return Timer(Asynco_Default_Runtime.io_ctx, callback, time, TimerType::Delayed);
}

Timer periodic(function<void()> callback, uint64_t time) {
    return Timer(Asynco_Default_Runtime.io_ctx, callback, time, TimerType::Periodic);
}

void sleep(int _time) {
    return Asynco_Default_Runtime.sleep(_time);
}

SleepHandle sleep2(int _time) {
    return Asynco_Default_Runtime.sleep2(_time);
}


Asynco& asynco_default_runtime() {
    return Asynco_Default_Runtime;
}

void asynco_default_run() {
#ifdef ASYNCO_THREADS_POOL_SIZE
    Asynco_Default_Runtime.run(ASYNCO_THREADS_POOL_SIZE);
#else
    Asynco_Default_Runtime.run();
#endif
}

void asynco_default_run_on_this() {
    Asynco_Default_Runtime.run_on_this();
}

void asynco_default_join() {
    Asynco_Default_Runtime.join();
}

io_context& asynco_default_io_context() {
    return Asynco_Default_Runtime.io_ctx;
}

};
