#include "../lib/asynco_default.hpp"
using namespace marcelb::asynco;

#include <iostream>
using namespace std;


int main() {
    asynco_default_run();


    async_ ([](){
        loop {
            cout << "Loop 1" << endl;
            sleep(1000);
        }
    });


    async_ ([](){
        loop {
            cout << "Loop 2" << endl;
            sleep(2000);
        }
    });


    async_ ([](){
        loop {
            cout << "Loop 25" << endl;
            sleep(2500);
        }
    });


    async_ ([](){
        loop {
            cout << "Loop 3" << endl;
            sleep(3000);
        }
    });


    async_ ([](){
        loop {
            cout << "Loop 35" << endl;
            sleep(3500);
        }
    });


    async_ ([](){
        loop {
            cout << "Loop 4" << endl;
            sleep(4000);
        }
    });


    async_ ([](){
        loop {
            cout << "Loop 45" << endl;
            sleep(4500);
        }
    });


    async_ ([](){
        loop {
            cout << "Loop 5" << endl;
            sleep(5000);
        }
    });


    async_ ([](){
        loop {
            cout << "Loop 55" << endl;
            sleep(5500);
        }
    });

    async_ ([](){
        loop {
            cout << "Loop 6" << endl;
            sleep(6000);
        }
    });

    async_ ([](){
        loop {
            cout << "Loop 65" << endl;
            sleep(6500);
        }
    });

    async_ ([](){
        loop {
            cout << "Loop 7" << endl;
            sleep(7000);
        }
    });

    loop {  // blokira trenutnu
        cout << "Loop 15" << endl;
        sleep(1500);
    }

    asynco_default_join();
    return 0;
}