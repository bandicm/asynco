#include "../lib/asynco_default.hpp"
using namespace marcelb::asynco;

#include <iostream>
using namespace std;

int main() {
    asynco_default_run();


    async_ ([](){
        loop {
            marcelb::asynco::sleep(1000).await();
            cout << "Loop 1" << endl;
        }
    });


    async_ ([](){
        loop {
            marcelb::asynco::sleep(2000).await();
            cout << "Loop 2" << endl;
        }
    });


    async_ ([](){
        loop {
            marcelb::asynco::sleep(2500).await();
            cout << "Loop 25" << endl;
        }
    });


    async_ ([](){
        loop {
            marcelb::asynco::sleep(3000).await();
            cout << "Loop 3" << endl;
        }
    });


    async_ ([](){
        loop {
            marcelb::asynco::sleep(3500).await();
            cout << "Loop 35" << endl;
        }
    });


    async_ ([](){
        loop {
            marcelb::asynco::sleep(4000).await();
            cout << "Loop 4" << endl;
        }
    });


    async_ ([](){
        loop {
            marcelb::asynco::sleep(4500).await();
            cout << "Loop 45" << endl;
        }
    });


    async_ ([](){
        loop {
            marcelb::asynco::sleep(5000).await();
            cout << "Loop 5" << endl;
        }
    });


    async_ ([](){
        loop {
            marcelb::asynco::sleep(5500).await();
            cout << "Loop 55" << endl;
        }
    });

    async_ ([](){
        loop {
            marcelb::asynco::sleep(6000).await();
            cout << "Loop 6" << endl;
        }
    });

    async_ ([](){
        loop {
            marcelb::asynco::sleep(6500).await();
            cout << "Loop 65" << endl;
        }
    });

    async_ ([](){
        loop {
            marcelb::asynco::sleep(7000).await();
            cout << "Loop 7" << endl;
        }
    });

    loop {  // blokira trenutnu
        marcelb::asynco::sleep(1500).await();
        cout << "Loop 15" << endl;
    }

    asynco_default_join();
    return 0;
}