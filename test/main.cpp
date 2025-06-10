#define NUM_OF_RUNNERS 4

#include "asynco.hpp"
#include "trigger.hpp"
#include "filesystem.hpp"
#include "timers.hpp"
#include "define.hpp"

using namespace marcelb::asynco;

#include <iostream>
#include <unistd.h>
#include <thread>
#include <future>
#include <vector>

using namespace std;
using namespace this_thread;

// asyncable<int> c2 (int a)  {
//     co_return a*2;
// } 

// asyncable<void> sleep_co (int a)  {
//     sleep(a);
//     cout << "Gotov" << endl;
//     co_return;
// } 


// asyncable<void> c ()  {
//     cout << "Ispisi" << endl;
//     co_await c2(0);
//     co_return;
// } 



// void sleep_to (int _time) {
//     promise<void> _promise;
//     Delayed t( [&]() {
//         _promise.set_value();
//     }, _time);

//     return _promise.get_future().get();
// }

// void sleep_to (int _time) {
//     promise<void> _promise;
//     Delayed t( [&]() {
//         _promise.set_value();
//     }, _time);
//     await_ (_promise.get_future(), 100);
// }

// future<void> sleep_to (int _time) {
//     promise<void> _promise;
//     future<void> _future = _promise.get_future();
    
//     Delayed t( [&]() {
//         _promise.set_value();
//     }, _time);

//     return _future;
// }

// void promise_reject (int _time) {
//     promise<void> _promise;
//     Delayed t( [&]() {
//         try {
//             // simulate except
//             throw runtime_error("Error simulation");
//             _promise.set_value();
//         } catch (...) {
//             _promise.set_exception(current_exception());
//         }
//     }, _time);

//     return _promise.get_future().get();
// }

// void notLambdaFunction() {
//     cout << "Call to not lambda function" << endl;
// }

// class clm {
//     public:
//     void classMethode() {
//         cout << "Call class method" << endl;
//     }
// };

// // // ------------------ EXTEND OWN CLASS WITH EVENTS -------------------

// class myOwnClass : public Trigger<int> {
//     public:
//     myOwnClass() : Trigger() {};
// };

// // ----------------- MULTIPLE TRIGGERS IN ONE CLASS ------------------

// class ClassWithTriggers {
//     Trigger<int> emitter1;
//     Trigger<string> emitter2;

// public:
//     template<typename... T>
//     void on(const string& key, function<void(T...)> callback) {
//         if constexpr (sizeof...(T) == 1 && is_same_v<tuple_element_t<0, tuple<T...>>, int>) {
//             emitter1.on(key, callback);
//         }
//         else if constexpr (sizeof...(T) == 1 && is_same_v<tuple_element_t<0, tuple<T...>>, string>) {
//             emitter2.on(key, callback);
//         }
//     }

//     template <typename... Args>
//     void tick(const string& key, Args&&... args) {
//         if constexpr (sizeof...(Args) == 1 && is_same_v<tuple_element_t<0, tuple<Args...>>, int>) {
//             emitter1.tick(key, forward<Args>(args)...);
//         }
//         else if constexpr (sizeof...(Args) == 1 && is_same_v<tuple_element_t<0, tuple<Args...>>, string>) {
//             emitter2.tick(key, forward<Args>(args)...);
//         }
//         else {
//             static_assert(sizeof...(Args) == 0, "Unsupported number or types of arguments");
//         }
//     }
// };


int main () {

    auto start = rtime_ms();

    // --------------- TIME ASYNCHRONOUS FUNCTIONS --------------

    /**
     * Init Periodic and delayed; clear Periodic and delayed
    */

    // Periodic inter1 ([&]() {
    //     cout << "Periodic prvi " << rtime_ms() - start << endl;
    // }, 1000);

    // Periodic inter2 ([&]() {
    //     cout << "Periodic drugi " << rtime_ms() - start << endl;
    // }, 2000);

    // Periodic inter3 ([&]() {
    //     cout << "Periodic treći " << rtime_ms() - start << endl;
    // }, 1000);

    // Periodic inter4 ([&]() {
    //     // cout << "Periodic cetvrti " << rtime_ms() - start << endl;
    //     cout << "Ticks " << inter3.ticks() << endl;
    // }, 500);

    // Periodic inter5 ([&]() {
    //     cout << "Periodic peti " << rtime_ms() - start << endl;
    // }, 2000);

    // Periodic inter6 ([&]() {
    //     cout << "Periodic sesti " << rtime_ms() - start << endl;
    // }, 3000);

    // Delayed time1 ( [&] () {
    //     cout << "Close Periodic 1 i 2 " << rtime_ms() - start << endl;
    //     inter1.stop();
    //     cout << "inter1.stop " << endl;
    //     inter2.stop();
    //     cout << "inter2.stop " << endl;
    // }, 8000);


    // Delayed time2 ([&] () {
    //     cout << "Close Periodic 3 " << rtime_ms() - start << endl;
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
    //  * Sleep with Delayed sleep implement
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

    auto a = async_ ( []() {
        cout << "A" << endl;
        return 3;
    });

    auto b = async_ ( []() {
        cout << "B" << endl;
        // throw runtime_error("Test exception");
        return;
    });

    auto c = async_ ( []() {
        cout << "C" << endl;
        return "Hello";
    });

    int a_;
    string c_;

    // auto all = await_(a, c);
    // cout << get<0>(all) << get<1>(all) << endl;

    // ili

    tie(a_, c_) = await_(a, c);
    cout << a_ << c_ << endl;

    int d_;
    float e_;
    tie(d_, e_) = await_( async_ ( []() {return 1;}), async_ ([](){ return 4.3;}));

    cout << d_ << e_ << endl;
    

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

    // Trigger<int, int> ev2int;
    // Trigger<int, string> evintString;
    // Trigger<> evoid;

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

    // Delayed t( [&] {
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

    // ClassWithTriggers mt;

    // mt.on<int>("int", function<void(int)>([&](int i) {
    //     cout << "Emit int " << i << endl;
    // }));

    // mt.on<string>("string", function<void(string)>([&](string s) {
    //     cout << "Emit string " << s << endl;
    // }));

    // mt.tick("int", 5);
    // mt.tick("string", string("Hello world"));


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


    // ----------------------------------------------------------------------------------------------------


    // auto i = async_ ( []() -> asyncable<int> {
    //     cout << "aaaa" << endl;
    //     co_return 5;
    // });

    // auto i = async_ (retint());

    // auto i_ = await_(i);

    // cout << i_ << endl;


    // Periodic a( []() -> asyncable<void> {
    //     cout << "corutina" << endl;
    //     // co_await retint();
    // }, 2000);


    // Periodic b_( []()  {
    //     cout << "funckija" << endl;
    // }, 2000);


    // Trigger<int, int> ev2int;
    // Trigger<int, string> evintString;
    // Trigger<> evoid;

    // ev2int.on("sum", [](int a, int b) -> asyncable<void> {
    //     cout << "Sum " << a+b << endl;
    // });

    // ev2int.on("sum", [](int a, int b) -> asyncable<void> {
    //     cout << "Sum done" << endl;
    // });

    // evintString.on("substract", [](int a, string b) -> asyncable<void> {
    //     cout << "Substract " << a-stoi(b) << endl;
    // });

    // evoid.on("void", []() {
    //     auto a = await_ (async_ (c2(34)));
    //     cout << "A " << a << endl;
    // });


    // auto c1 = []() -> asyncable<void> {
    //     cout << "Roge " << endl;
    //     co_return;

    // };

    // auto a =  await_ ( c2(3));
    // cout << a << endl;


    // await_ ([]() -> asyncable<void> {
    //     cout << "Hello" << endl;
    //     co_await c2(4);
    //     co_return;
    // }());

    // async_ ([]() -> asyncable<void> {
    //     cout << "1" << endl;
    //     co_await sleep_co(1);
    //     co_return;
    // }());

    // async_ ([]() -> asyncable<void> {
    //     cout << "2" << endl;
    //     co_await sleep_co(1);
    //     co_return;
    // }());

    // async_ ([]() -> asyncable<void> {
    //     cout << "3" << endl;
    //     co_await sleep_co(1);
    //     co_return;
    // }());

    // async_ ([]() -> asyncable<void> {
    //     cout << "4" << endl;
    //     co_await sleep_co(1);
    //     co_return;
    // }());

    // async_ ([]() -> asyncable<void> {
    //     cout << "5" << endl;
    //     co_await sleep_co(1);
    //     co_return;
    // }());


    // await_ ([]()  {
    //     cout << "Hello" << endl;
    // });

    // Periodic p( []() {
    //     async_ (
    //         c2(34)
    //     );
    // }, 2000);

    // await_( async_ ( [c1 = move(c1)]() -> asyncable<void> {
    //     cout << "Baba roga" << endl;
    //     co_await c1();
    // }));

    // string emited2 = "2";

    // evoid.on("void", [&]() -> asyncable<void> {
    //     cout << "Void emited " << emited2 << endl;
    // });

    // evoid.tick("void");




    // vector<future<void>> futures;

    // for (int i=0; i<20; i++) {
    //     futures.push_back(
    //         async_([a = i](){
    //             for (int i=0; i<1000; i++) {
    //                 cout << a << " " << i << endl;
    //                 // sleep_to(i);
    //             }
    //         })
    //     );
    // }

    // for (int i=0; i<20; i++) {
    //     await_(futures[i]);
    //     // await_(futures[i]);
    // }





    cout << "-------------end main------------- " << rtime_ms() - start << endl;
    _asynco_engine.run();
    return 0;
}

