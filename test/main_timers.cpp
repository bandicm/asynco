#include "../lib/asynco.hpp"
using namespace marcelb::asynco;

#include <iostream>
using namespace std;

int main() {
    Asynco asynco;
    asynco.run(2);

    Timer inter1 = asynco.periodic ([]() {
     cout << "Interval 1" << endl;
    }, 1000);

    // stop periodic
    inter1.stop();

    // how many times it has expired
    int ti = inter1.ticks();

    // is it stopped
    bool stoped_i = inter1.stoped();

    // start delayed
    Timer time1 = asynco.delayed ( [] () {
        cout << "Timeout 1 " << endl;
    }, 10000);

    // stop delayed
    time1.stop();

    // is it expired
    int tt = time1.expired();

    // is it stopped
    bool stoped_t = time1.stoped();


    asynco.join();
    return 0;
}