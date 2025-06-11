#include "../lib/asynco_default.hpp"
using namespace marcelb::asynco;

#include <iostream>
using namespace std;

int main() {
    asynco_default_run();

    asyncable<int> c2(int a) {
        co_return a * 2;
    }

    async_(c2(4));

    async_([]() -> asyncable<void> {
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

    await_ ([]() -> asyncable<void> {
        cout << "Hello" << endl;
        co_return;
    }());


    asynco_default_join();
    return 0;
}