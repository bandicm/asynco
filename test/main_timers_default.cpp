#include "../lib/asynco_default.hpp"
using namespace marcelb::asynco;

#include <iostream>
using namespace std;


int main() {
    asynco_default_run();

    Timer inter1 = periodic ([]() {
     cout << "Interval 1" << endl;
    }, 1000);

    // stop periodic
    inter1.stop();

    // how many times it has expired
    int ti = inter1.ticks();

    // is it stopped
    bool stoped_i = inter1.stoped();

    // start delayed
    Timer time1 = delayed ( [] () {
        cout << "Timeout 1 " << endl;
    }, 10000);

    // stop delayed
    time1.stop();

    // is it expired
    int tt = time1.expired();

    // is it stopped
    bool stoped_t = time1.stoped();

    asynco_default_join();
    return 0;
}