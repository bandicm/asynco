#include "../lib/asynco.hpp"
using namespace marcelb::asynco;

#include <iostream>
using namespace std;

awaitable<int> c2(int a) {
    co_return a * 2;
}


int main() {

    Asynco asynco;  // or global
    asynco.run(2);

    asynco.async(c2(4));

    asynco.async([]() -> awaitable<void> {
        std::cout << "Hello" << std::endl;
        co_await c2(4);
        co_return;
    }());


    int r = asynco.await(
        asynco.async(
            c2(10)
    ));


    auto a =  asynco.await( c2(3));
    cout << a << endl;

    asynco.await([]() -> awaitable<void> {
        cout << "Hello" << endl;
        co_return;
    }());


    asynco.join();
    return 0;
}