
#include "../lib/runner.hpp"
#include "../lib/asynco.hpp"
#include "../lib/event.hpp"
#include "../lib/rotor.hpp"

#include <iostream>
#include <unistd.h>

using namespace std;
using namespace marcelb;
using namespace this_thread;

#ifndef ON_RUNNER
#define ON_RUNNER
runner on_async;
#endif


void sleep_to (int _time) {
    promise<void> _promise;
    timeout t( [&]() {
        _promise.set_value();
    }, _time);

    return _promise.get_future().get();
}

void promise_reject (int _time) {
    promise<void> _promise;
    timeout t( [&]() {
        try {
            // simulate except
            throw runtime_error("Error simulation");
            _promise.set_value();
        } catch (...) {
            _promise.set_exception(current_exception());
        }
    }, _time);

    return _promise.get_future().get();
}

// ------------------ EXTEND OWN CLASS WITH EVENTS -------------------

class myOwnClass : public event<int> {
    public:
    myOwnClass() : event() {};
};


int main () {

    auto start = rtime_ms();

    // --------------- TIME ASYNCHRONOUS FUNCTIONS --------------

    /**
     * Init interval and timeout; clear interval and timeout
    */

    // interval inter1 ([&]() {
    //     cout << "interval prvi " << rtime_ms() - start << endl;
    // }, 1000);

    // interval inter2 ([&]() {
    //     cout << "interval drugi " << rtime_ms() - start << endl;
    // }, 2000);

    // interval inter3 ([&]() {
    //     cout << "interval treći " << rtime_ms() - start << endl;
    // }, 3000);

    // timeout time1 ( [&] () {
    //     cout << "Close interval 1 i 2 " << rtime_ms() - start << endl;
    //     inter1.clear();
    //     inter2.clear();
    // }, 10000);

    // timeout time2 ([&] () {
    //     cout << "Close interval 3 " << rtime_ms() - start << endl;
    //     inter3.clear();
    //     time1.clear();
    // }, 2000);

    // ------------------------ MAKE FUNCTIONS ASYNCHRONOUS -------------------------

    /**
     * Put task directly and get returned value - it is not recommended to use it
    */

    // auto res1 = on_async.put_task( [] () {
    //     cout << "Jebiga " <<endl;
    //     throw string ("jebiga!!");
    // });

    // try {
    //     res1.get();
    // } catch (const string except) {
    //     cout << except << endl;
    // }

    /**
     * Run an function asyncronic
    */

    // asynco( []() {
    //     sleep_for(2s);   // only for simulate log duration function
    //     cout << "asynco" << endl;
    //     return 5;
    // });


    /**
     * Wait after runned as async
     */

    // auto a = asynco( []() {
    //     sleep_for(2s);   // only for simulate log duration function
    //     cout << "asynco" << endl;
    //     return 5;
    // });

    // cout << wait(move(a)) << endl;

    /**
     * Wait async function call and use i cout
    */

    // cout << wait(asynco( [] () {
    //     sleep_for(chrono::seconds(1)); // only for simulate log duration function
    //     cout << "wait end" << endl;
    //     return 4;
    // })) << endl;

    /**
     * Sleep with timeout sleep implement
    */

    // sleep_to(3000);
    // cout << "sleep_to " << rtime_ms() - start << endl;

    /**
     * Catch promise reject
    */

    // try {
    //     promise_reject(3000);
    // } catch (runtime_error err) {
    //     cout<< err.what() << endl;
    // }

    // cout << "promise_reject " << rtime_ms() - start << endl;


    /**
     * Nested asynchronous invocation
    */


    // asynco( [] {
    //     cout << "idemo ..." << endl;
    //     asynco( [] {
    //         cout << "ugdnježdena async funkcija " << endl;
    //     });
    // });

    // --------------- EVENTS -------------------

    /**
     * initialization of typed events
    */

    // event<int, int> ev2int;
    // event<int, string> evintString;
    // event<> evoid;

    // ev2int.on("sum", [](int a, int b) {
    //     cout << "Sum " << a+b << endl;
    // });

    // evintString.on("substract", [](int a, string b) {
    //     cout << "Substract " << a-stoi(b) << endl;
    // });

    // evoid.on("void", []() {
    //     cout << "Void emited" << endl;
    // });

    // sleep(1);

    /**
     * Emit
    */

    // ev2int.emit("sum", 5, 8);

    // sleep(1);
    // evintString.emit("substract", 3, to_string(2));

    // sleep(1);
    // evoid.emit("void");

    /**
     * Own class 
    */

    // myOwnClass myclass;

    // timeout t( [&] {
    //     myclass.emit("constructed", 1);
    // }, 200);

    // myclass.on("constructed", [] (int i) {
    //     cout << "Constructed " << i  << endl;
    // });

    sleep(10000); // only for testing

    return 0;
}

