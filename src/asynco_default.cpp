
#include "../lib/asynco_default.hpp"

namespace marcelb::asynco {

Asynco Asynco_Default_Runtime;

// Asynco& Asynco_Default_Runtime() {
//     static Asynco _default; // ili koliko već treba
//     return _default;
// }

Timer delayed(function<void()> callback, uint64_t time) {
    return Timer(Asynco_Default_Runtime.io_ctx, callback, time, TimerType::Delayed);
}

Timer periodic(function<void()> callback, uint64_t time) {
    return Timer(Asynco_Default_Runtime.io_ctx, callback, time, TimerType::Periodic);
}

};
