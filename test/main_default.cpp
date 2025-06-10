#include "../lib/asynco_default.hpp"
using namespace marcelb::asynco;

#include <iostream>
using namespace std;

int main() {
    Asynco_Default_Runtime.run();
    cout << "main" << endl;

    async_([](){
        cout << "idemo" << endl;
    });

    auto interval = periodic([&](){
        cout << "idemo" << endl;
    }, 1000);

    
    Asynco_Default_Runtime.join();
    return 0;
}