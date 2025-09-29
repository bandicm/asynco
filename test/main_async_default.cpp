#include "../lib/asynco_default.hpp"
using namespace marcelb::asynco;

#include <iostream>
using namespace std;

void notLambdaFunction() {
    cout << "Call to not lambda function" << endl;
}


class clm {
    public:
    int i = 7;
    void classMethode() {
        cout << "Call class method" << i << endl;
    }

    future<void> asyncMethode() {
        return async_([&]() {
            cout << "Async class method" << i << endl;
        });
    }
};

void sleep_to (int _time) {
    promise<void> _promise;
    Timer t = delayed( [&]() {
        _promise.set_value();
    }, _time);

    return await_(_promise.get_future());
}

int main() {

    asynco_default_run();

    /**
    * Run an lambda function asynchronously
    */

    async_ ( []() {
        cout << "async " << endl;
    });


    /**
     * Run not lambda function
    */

    async_ (notLambdaFunction);

    /**
     * Run class method
    */

    clm classes;
    async_ ( [&classes] () {
        classes.classMethode();
    });

    await_(classes.asyncMethode());

    //------------------AWAIT----------------------

    auto a = async_ ( []() {
        sleep_to(1000);   //only for simulating long duration function
        return 5;
    });

    cout << await_(a) << endl;

    /**
    * await async function call and use i cout
    */

    cout << await_(async_ ( [] () {
        sleep_to(1000);
        cout << "await_ end" << endl;
        return 4;
    })) << endl;


    await_ ([]()  {       // run in runtime and await now
        cout << "Hello" << endl;
    });

    asynco_default_join();
    return 0;
}