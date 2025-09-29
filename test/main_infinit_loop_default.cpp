#include "../lib/asynco_default.hpp"
using namespace marcelb::asynco;

#include <iostream>
using namespace std;


int main() {
    asynco_default_run();


    async_ ([](){
        loop {
            cout << "Loop 1" << endl;
            await_(sleep2(1000).future);
        }
    });


    async_ ([](){
        loop {
            cout << "Loop 2" << endl;
            await_(sleep2(2000).future);
        }
    });


    async_ ([](){
        loop {
            cout << "Loop 25" << endl;
            await_(sleep2(2500).future);
        }
    });


    async_ ([](){
        loop {
            cout << "Loop 3" << endl;
            await_(sleep2(3000).future);
        }
    });


    async_ ([](){
        loop {
            cout << "Loop 35" << endl;
            await_(sleep2(3500).future);
        }
    });


    async_ ([](){
        loop {
            cout << "Loop 4" << endl;
            await_(sleep2(4000).future);
        }
    });


    async_ ([](){
        loop {
            cout << "Loop 45" << endl;
            await_(sleep2(4500).future);
        }
    });


    async_ ([](){
        loop {
            cout << "Loop 5" << endl;
            await_(sleep2(5000).future);
        }
    });


    async_ ([](){
        loop {
            cout << "Loop 55" << endl;
            await_(sleep2(5500).future);
        }
    });

    async_ ([](){
        loop {
            cout << "Loop 6" << endl;
            await_(sleep2(6000).future);
        }
    });

    async_ ([](){
        loop {
            cout << "Loop 65" << endl;
            await_(sleep2(6500).future);
        }
    });

    async_ ([](){
        loop {
            cout << "Loop 7" << endl;
            await_(sleep2(7000).future);
        }
    });

    loop {  // blokira trenutnu
        cout << "Loop 15" << endl;
        sleep2(1500).future.get();
    }

    asynco_default_join();
    return 0;
}