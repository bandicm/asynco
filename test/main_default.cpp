#include "../lib/asynco_default.hpp"
using namespace marcelb::asynco;

#include <iostream>
using namespace std;

int main() {
    asynco_default_run();

    // code

    asynco_default_join();
    return 0;
}