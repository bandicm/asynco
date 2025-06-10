#include "../lib/asynco.hpp"
using namespace marcelb::asynco;

#include <iostream>
using namespace std;

int main() {

    Asynco asynco;
    asynco.run(2);

    auto interval = asynco.periodic([](){
        cout << "idemo" << endl;
    }, 1000);


    asynco.join();
    return 0;
}