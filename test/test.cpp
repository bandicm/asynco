// // #define NUM_OF_RUNNERS 2

#include "../lib/asynco.hpp"
#include "../lib/trigger.hpp"
#include "../lib/filesystem.hpp"
#include "../lib/timers.hpp"
#include "../lib/define.hpp"

using namespace marcelb::asynco;
using namespace triggers;

#include <iostream>
#include <unistd.h>
#include <thread>
#include <future>
#include <vector>

using namespace std;
using namespace this_thread;



void sleep_to (int _time) {
    promise<void> _promise;
    delayed t( [&]() {
        _promise.set_value();
    }, _time);

    return _promise.get_future().get();
}

void promise_reject (int _time) {
    promise<void> _promise;
    delayed t( [&]() {
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

class myOwnClass : public trigger<int> {
    public:
    myOwnClass() : trigger() {};
};

// ----------------- MULTIPLE TRIGGERS IN ONE CLASS ------------------

class ClassWithTriggers {
    trigger<int> emitter1;
    trigger<string> emitter2;

public:
    template<typename... T>
    void on(const string& key, function<void(T...)> callback) {
        if constexpr (sizeof...(T) == 1 && is_same_v<tuple_element_t<0, tuple<T...>>, int>) {
            emitter1.on(key, callback);
        }
        else if constexpr (sizeof...(T) == 1 && is_same_v<tuple_element_t<0, tuple<T...>>, string>) {
            emitter2.on(key, callback);
        }
    }

    template <typename... Args>
    void tick(const string& key, Args&&... args) {
        if constexpr (sizeof...(Args) == 1 && is_same_v<tuple_element_t<0, tuple<Args...>>, int>) {
            emitter1.tick(key, forward<Args>(args)...);
        }
        else if constexpr (sizeof...(Args) == 1 && is_same_v<tuple_element_t<0, tuple<Args...>>, string>) {
            emitter2.tick(key, forward<Args>(args)...);
        }
        else {
            static_assert(sizeof...(Args) == 0, "Unsupported number or types of arguments");
        }
    }
};


int main () {

    auto start = rtime_ms();

    // --------------- TIME ASYNCHRONOUS FUNCTIONS --------------

    /**
     * Init periodic and delayed; clear periodic and delayed
    */

    // periodic inter1 ([&]() {
    //     cout << "periodic prvi " << rtime_ms() - start << endl;
    // }, 1000);

    // periodic inter2 ([&]() {
    //     cout << "periodic drugi " << rtime_ms() - start << endl;
    // }, 2000);

    // periodic inter3 ([&]() {
    //     cout << "periodic treći " << rtime_ms() - start << endl;
    // }, 1000);

    // periodic inter4 ([&]() {
    //     // cout << "periodic cetvrti " << rtime_ms() - start << endl;
    //     cout << "Ticks " << inter3.ticks() << endl;
    // }, 500);

    // periodic inter5 ([&]() {
    //     cout << "periodic peti " << rtime_ms() - start << endl;
    // }, 2000);

    // periodic inter6 ([&]() {
    //     cout << "periodic sesti " << rtime_ms() - start << endl;
    // }, 3000);

    // delayed time1 ( [&] () {
    //     cout << "Close periodic 1 i 2 " << rtime_ms() - start << endl;
    //     inter1.stop();
    //     cout << "inter1.stop " << endl;
    //     inter2.stop();
    //     cout << "inter2.stop " << endl;
    // }, 8000);


    // delayed time2 ([&] () {
    //     cout << "Close periodic 3 " << rtime_ms() - start << endl;
    //     inter3.stop();
    //     cout << "Stoped " << inter3.stoped() << endl;
    //     // time1.stop();
    // }, 5000);


    // if (time2.expired()) {
    //     cout << "isteko " << endl;
    // } else {
    //     cout << "nije isteko " << endl;
    // }

    // // sleep(6);

    // if (time2.expired()) {
    //     cout << "isteko " << endl;
    // } else {
    //     cout << "nije isteko " << endl;
    // }

    // auto d = Delayed( [](){
    //     cout << "Delayed" << endl;
    // }, 2000);

    // auto p = Periodic( [](){
    //     cout << "Periodic" << endl;
    // }, 700);

    // Periodic( [&] (){
    //     cout << "Delayed expire " << d->expired() << endl;
    //     cout << "Periodic ticks " << p->ticks() << endl;
    //     cout << "Delayed stoped " << d->stoped() << endl;
    //     cout << "Periodic stoped " << p->stoped() << endl;
    // }, 1000);

    // Delayed( [&](){
    //     p->stop();
    // }, 10000);

    // // // // ------------------------ MAKE FUNCTIONS ASYNCHRONOUS -------------------------

    // // /**
    // //  * Run an function asyncronic
    // // */

    // async_ ( []() {
    //     sleep_for(2s);   // only for simulate log duration function
    //     cout << "asynco 1" << endl;
    //     return 5;
    // });

    // /**
    //  * Call not lambda function
    // */

    // async_ (notLambdaFunction);


    // await_ (
    //     async_ (
    //         notLambdaFunction
    //     )
    // );


    // // async(launch::async, [] () {
    // //     cout << "Another thread in async style!"  << endl;
    // // });

    // // /**
    // //  * Call class method
    // // */

    // clm classes;
    // async_ ( [&classes] () {
    //     classes.classMethode();
    // });

    // sleep(5);

    // // /**
    // //  * await_ after runned as async
    // //  */

    // auto aa = async_ ( []() {
    //     sleep_for(2s);   // only for simulate log duration function
    //     cout << "async_  2" << endl;
    //     return 5;
    // });

    // cout << await_(aa) << endl;
    // cout << "print after async_  2" << endl;

    // /**
    //  * await_ async function call and use i cout
    // */

    // cout << await_(async_ ( [] () {
    //     sleep_for(chrono::seconds(1)); // only for simulate log duration function
    //     cout << "await_ end" << endl;
    //     return 4;
    // })) << endl;

    // /**
    //  * Sleep with delayed sleep implement
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


    // async_ ( [] {
    //     cout << "idemo ..." << endl;
    //     async_ ( [] {
    //         cout << "ugdnježdena async funkcija " << endl;
    //     });
    // });


    // // -------------------------- AWAIT ALL ----------------------------------

    // auto a = async_ ( []() {
    //     cout << "A" << endl;
    //     return 3;
    // });

    // auto b = async_ ( []() {
    //     cout << "B" << endl;
    //     throw runtime_error("Test exception");
    //     return;
    // });

    // auto c = async_ ( []() {
    //     cout << "C" << endl;
    //     return "Hello";
    // });

    // int a_;
    // string c_;

    // auto await_all = [&] () {
    //     a_ = await_(a);
    //     await_(b);
    //     c_ = await_(c);
    // };

    // try {
    //     await_all();
    //     cout << "a_ " << a_ << " c_ " << c_ << endl;
    // } catch (const exception& exc) {
    //     cout << exc.what() << endl;
    // }

    // // //  same type 

    // vector<future<void>> fut_vec;
    // for (int i=0; i<5; i++) {
    //     fut_vec.push_back(
    //         async_ ( [i]() {
    //             cout << "Async_ " << i << endl;
    //         })
    //     );
    // }

    // auto await_all2 = [&] () {
    //     for (int i=0; i<fut_vec.size(); i++) {
    //         await_ (fut_vec[i]);
    //     }
    // };

    // await_all2();

    // // --------------- EVENTS -------------------

    // /**
    //  * initialization of typed events
    // */

    // trigger<int, int> ev2int;
    // trigger<int, string> evintString;
    // trigger<> evoid;

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

    // evoid.tick("void");
    // sleep(1);

    // /**
    //  * Emit
    // */

    // ev2int.tick("sum", 5, 8);
    

    // sleep(1);
    // evintString.tick("substract", 3, to_string(2));

    // sleep(1);
    // evoid.off("void");
    // evoid.tick("void");


    // cout << "Ukupno 2 int " <<  ev2int.listeners() << endl;
    // cout << "Ukupno evintString " <<  evintString.listeners() << endl;
    // cout << "Ukupno evoid " <<  evoid.listeners() << endl;
    // cout << "Ukupno 2 int " <<  ev2int.listeners("sum") << endl;

    // /**
    //  * Own class 
    // */

    // myOwnClass myclass;

    // delayed t( [&] {
    //     myclass.tick("constructed", 1);
    // }, 200);

    // myclass.on("constructed", [] (int i) {
    //     cout << "Constructed " << i  << endl;
    // });

    // /**
    //  * 
    //  * Use class with multiple triggers
    //  * 
    //  */

    ClassWithTriggers mt;

    mt.on<int>("int", function<void(int)>([&](int i) {
        cout << "Emit int " << i << endl;
    }));

    mt.on<string>("string", function<void(string)>([&](string s) {
        cout << "Emit string " << s << endl;
    }));

    mt.tick("int", 5);
    mt.tick("string", string("Hello world"));


    // auto status = fs::read("test1.txt");


    // try {
    //     auto data = await_(status);
    //     cout << data;
    // } catch (exception& err) {
    //     cout << err.what() << endl;
    // }


    // string data_;
    // auto start_read = rtime_us();

    // fs::read("test1.txt", [&data_, &start_read] (string data, exception* error) {
    //     if (error) {
    //         cout << "Error " << error->what() << endl;
    //     } else {
    //         // cout << "Data " << endl << data << endl;
    //         // data_ = data;
    //         // cout << "Data_" << data_ << endl;
    //         cout << "read " << rtime_us() - start_read << endl;
    //     }
    // });


    // // ----------------------------------------------------------------------------------------------------

    cout << "Run" << endl;
    _asynco_engine.run();

    return 0;
}

