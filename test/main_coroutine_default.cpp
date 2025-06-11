#include "../lib/asynco_default.hpp"
using namespace marcelb::asynco;

#include <iostream>
using namespace std;

awaitable<int> c2(int a) {
    co_return a * 2;
}

int main() {
    asynco_default_run();


    async_(c2(4));

    async_([]() -> awaitable<void> {
        std::cout << "Hello" << std::endl;
        co_await c2(4);
        co_return;
    }());


    int r = await_(
        async_(
            c2(10)
    ));


    auto a =  await_ ( c2(3));
    cout << a << endl;

    await_ ([]() -> awaitable<void> {
        cout << "Hello" << endl;
        co_return;
    }());


    asynco_default_join();
    return 0;
}