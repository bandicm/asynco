
// #include "../lib/loop.hpp"
// #include "../lib/asynco.hpp"
// #include "../example/asynco.hpp"
#include "../lib/event.hpp"
#include <iostream>
#include <unistd.h>

using namespace std;
using namespace marcelb;
using namespace this_thread;

#ifndef ON_ASYNC
#define ON_ASYNC
AsyncLoop on_async(8);
#endif

int main () {

    // auto start = chrono::high_resolution_clock::now();

    // interval inter1 ([&]() {
    //     cout << "interval prvi " << chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - start).count() << endl;
    // }, 1000);

    // interval inter2 ([&]() {
    //     cout << "interval drugi " << chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - start).count() << endl;
    // }, 2000);

    // interval inter3 ([&]() {
    //     cout << "interval treći " << chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - start).count() << endl;
    // }, 3000);

    // timeout time1 ( [&] () {
    //     cout << "Close interval 1 i 2 " << chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - start).count() << endl;
    //     inter1.clear();
    //     inter2.clear();
    // }, 10000);

    // timeout time2 ([&] () {
    //     cout << "Close interval 3 " << chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - start).count() << endl;
    //     inter3.clear();
    // }, 2000);

    // cout << "zadataka: " << on_async.count_tasks() << " niti: " << on_async.count_threads() << endl;


    //  for (int i = 0; i < 8; ++i) {
    //     pool.put_task( [&] (int id) {
    //         this_thread::sleep_for(chrono::seconds(1));
    //         cout << a*i << endl;
    //     }, i);
    // }

    // auto res1 = pool.put_task( [] () {
    //     cout << "Jebiga " <<endl;
    //     throw string ("jebiga!!");
    // });

    // try {
    //     res1.get();
    // } catch (const string except) {
    //     cout << except << endl;
    // }

    // auto res = pool.put_task( []() {
    //     this_thread::sleep_for(chrono::seconds(1));
    //     return 4;
    // });

    // cout << wait(asynco( [] () {
    //     sleep_for(chrono::seconds(1));
    //     cout << "RETURN" << endl;
    //     return 4;
    // })) << endl;

    // asynco( []() {
    //     sleep_for(2s);
    //     cout << "RETURN 2" << endl;
    //     return 5;
    // });

    // cout << wait(pool.put_task( []() {
    //     this_thread::sleep_for(chrono::seconds(1));
    //     return 7;
    // })); 


    // cout << wait(run1) << endl;

    // event<int, int> ev2int;
    // event<int, string> evintString;

    // ev2int.on("sum", [](int a, int b) {
    //     cout << "Sum " << a+b << endl;
    // });

    // evintString.on("substract", [](int a, string b) {
    //     cout << "Substract " << a-stoi(b) << endl;
    // });

    // sleep(5);

    // ev2int.emit("sum", 5, 8);

    // sleep(2);
    // evintString.emit("substract", 3, to_string(2));


    sleep(1000);

    return 0;
}

