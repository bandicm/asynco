#include "../lib/asynco.hpp"
using namespace marcelb::asynco;

#include <iostream>
using namespace std;

int main() {

    Asynco asynco;
    asynco.run(2);

    // code

    asynco.join();
    return 0;
}