#define NUM_OF_RUNNERS 2

#include "../lib/asynco.hpp"
#include "../lib/event.hpp"
#include "../lib/rotor.hpp"
// #include "../lib/filesystem.hpp"

#include <iostream>
#include <unistd.h>

using namespace std;
using namespace marcelb::asynco;
using namespace events;
// using namespace asynco;
using namespace this_thread;

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

void notLambdaFunction() {
    cout << "Call to not lambda function" << endl;
}

class clm {
    public:
    void classMethode() {
        cout << "Call class method" << endl;
    }
};

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

    // interval( [&] () {
    //     cout << "interval 1: "  << rtime_ms() - start << endl;
    // }, 50);

    // interval( [&] () {
    //     cout << "interval 1: "  << rtime_ms() - start << endl;
    // }, 100);

    // interval( [&] () {
    //     cout << "interval 2: " << rtime_ms() - start << endl;
    // }, 200);

    // interval( [&] () {
    //     cout << "interval 3: " << rtime_ms() - start << endl;
    // }, 300);


    // interval( [&] () {
    //     cout << "interval 4: "  << rtime_ms() - start << endl;
    // }, 400);

    interval inter1 ([&]() {
        cout << "interval prvi " << rtime_ms() - start << endl;
    }, 1000);

    interval inter2 ([&]() {
        cout << "interval drugi " << rtime_ms() - start << endl;
    }, 2000);

    interval inter3 ([&]() {
        cout << "interval treći " << rtime_ms() - start << endl;
    }, 3000);

    // interval inter4 ([&]() {
    //     cout << "interval cetvrti " << rtime_ms() - start << endl;
    // }, 1000);

    // interval inter5 ([&]() {
    //     cout << "interval peti " << rtime_ms() - start << endl;
    // }, 2000);

    // interval inter6 ([&]() {
    //     cout << "interval sesti " << rtime_ms() - start << endl;
    // }, 3000);

    timeout time1 ( [&] () {
        cout << "Close interval 1 i 2 " << rtime_ms() - start << endl;
        // inter1.clear();
        // cout << "inter1.stop " << inter1.stop << endl;
        // inter2.clear();
        // cout << "inter2.stop " << inter2.stop << endl;
    }, 5000);


    // timeout time2 ([&] () {
    //     cout << "Close interval 3 " << rtime_ms() - start << endl;
    //     // inter3.clear();
    //     time1.clear();
    // }, 2000);

    // // ------------------------ MAKE FUNCTIONS ASYNCHRONOUS -------------------------

    /**
     * Run an function asyncronic
    */

    // atask( []() {
    //     sleep_for(2s);   // only for simulate log duration function
    //     cout << "atask 1" << endl;
    //     return 5;
    // });

    // /**
    //  * Call not lambda function
    // */

    // atask (notLambdaFunction);


    // wait (
    //     atask (
    //         notLambdaFunction
    //     )
    // );

    // /**
    //  * Call class method
    // */

    // clm classes;
    // atask( [&classes] () {
    //     classes.classMethode();
    // });

    // sleep(5);

    // /**
    //  * Wait after runned as async
    //  */

    // auto a = atask( []() {
    //     sleep_for(2s);   // only for simulate log duration function
    //     cout << "atask 2" << endl;
    //     return 5;
    // });

    // cout << wait(a) << endl;
    // cout << "print after atask 2" << endl;

    // /**
    //  * Wait async function call and use i cout
    // */

    // cout << wait(atask( [] () {
    //     sleep_for(chrono::seconds(1)); // only for simulate log duration function
    //     cout << "wait end" << endl;
    //     return 4;
    // })) << endl;

    // /**
    //  * Sleep with timeout sleep implement
    // */

    // sleep_to(3000);
    // cout << "sleep_to " << rtime_ms() - start << endl;

    // /**
    //  * Catch promise reject
    // */

    // try {
    //     promise_reject(3000);
    // } catch (runtime_error err) {
    //     cout<< err.what() << endl;
    // }

    // cout << "promise_reject " << rtime_ms() - start << endl;


    // /**
    //  * Nested asynchronous invocation
    // */


    // atask( [] {
    //     cout << "idemo ..." << endl;
    //     atask( [] {
    //         cout << "ugdnježdena async funkcija " << endl;
    //     });
    // });

    // // --------------- EVENTS -------------------

    // /**
    //  * initialization of typed events
    // */

    // event<int, int> ev2int;
    // event<int, string> evintString;
    // event<> evoid;

    // ev2int.on("sum", [](int a, int b) {
    //     cout << "Sum " << a+b << endl;
    // });

    // ev2int.on("sum", [](int a, int b) {
    //     cout << "Sum done" << endl;
    // });

    // evintString.on("substract", [](int a, string b) {
    //     cout << "Substract " << a-stoi(b) << endl;
    // });

    // evoid.on("void", []() {
    //     cout << "Void emited" << endl;
    // });

    // string emited2 = "2";

    // evoid.on("void", [&]() {
    //     cout << "Void emited " << emited2 << endl;
    // });

    // evoid.emit("void");
    // sleep(1);

    // /**
    //  * Emit
    // */

    // ev2int.emit("sum", 5, 8);
    

    // sleep(1);
    // evintString.emit("substract", 3, to_string(2));

    // sleep(1);
    // evoid.off("void");
    // evoid.emit("void");

    // /**
    //  * Own class 
    // */

    // myOwnClass myclass;

    // timeout t( [&] {
    //     myclass.emit("constructed", 1);
    // }, 200);

    // myclass.on("constructed", [] (int i) {
    //     cout << "Constructed " << i  << endl;
    // });



    // auto status = fs::read("test1.txt");


    // try {
    //     auto data = wait(status);
    //     cout << data;
    // } catch (exception& err) {
    //     cout << err.what() << endl;
    // }

    cout << "Sleep" << endl;
    sleep(100000); // only for testing

    return 0;
}

