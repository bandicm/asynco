#include "../lib/asynco.hpp"
using namespace marcelb::asynco;

#include <iostream>
using namespace std;


int main() {
    Asynco asynco;
    asynco.run(4);

    // asynco.async ([&](){
    //     loop {
    //         cout << "Loop 1" << endl;
    //         asynco.sleep(1000);
    //     }
    // });


    // asynco.async ([&](){
    //     loop {
    //         cout << "Loop 2" << endl;
    //         asynco.sleep(2000);
    //     }
    // });


    // asynco.async ([&](){
    //     loop {
    //         cout << "Loop 25" << endl;
    //         asynco.sleep(2500);
    //     }
    // });


    // asynco.async ([&](){
    //     loop {
    //         cout << "Loop 3" << endl;
    //         asynco.sleep(3000);
    //     }
    // });


    // asynco.async ([&](){
    //     loop {
    //         cout << "Loop 35" << endl;
    //         asynco.sleep(3500);
    //     }
    // });


    // asynco.async ([&](){
    //     loop {
    //         cout << "Loop 4" << endl;
    //         asynco.sleep(4000);
    //     }
    // });


    // asynco.async ([&](){
    //     loop {
    //         cout << "Loop 45" << endl;
    //         asynco.sleep(4500);
    //     }
    // });


    // asynco.async ([&](){
    //     loop {
    //         cout << "Loop 5" << endl;
    //         asynco.sleep(5000);
    //     }
    // });


    // asynco.async ([&](){
    //     loop {
    //         cout << "Loop 55" << endl;
    //         asynco.sleep(5500);
    //     }
    // });

    // asynco.async ([&](){
    //     loop {
    //         cout << "Loop 6" << endl;
    //         asynco.sleep(6000);
    //     }
    // });

    // asynco.async ([&](){
    //     loop {
    //         cout << "Loop 65" << endl;
    //         asynco.sleep(6500);
    //     }
    // });

    // asynco.async ([&](){
    //     loop {
    //         cout << "Loop 7" << endl;
    //         asynco.sleep(7000);
    //     }
    // });

    // loop {  // blokira trenutnu
    //     cout << "Loop 15" << endl;
    //     asynco.sleep(1500);
    // }

    asynco.join();
    return 0;
}