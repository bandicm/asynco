#include "../lib/asynco.hpp"
using namespace marcelb::asynco;

#include <iostream>
using namespace std;

Asynco asynco;

void notLambdaFunction() {
    cout << "Call to not lambda function" << endl;
}


class clm {
    public:
    void classMethode() {
        cout << "Call class method" << endl;
    }
};

void sleep_to (int _time) {
    promise<void> _promise;
    Timer t = asynco.delayed( [&]() {
        _promise.set_value();
    }, _time);

    return asynco.await(_promise.get_future());
}

int main() {

    asynco.run(2);

    /**
    * Run an lambda function asynchronously
    */

    asynco.async ( []() {
        cout << "async " << endl;
    });


    /**
     * Run not lambda function
    */

    asynco.async (notLambdaFunction);

    /**
     * Run class method
    */

    clm classes;
    asynco.async ( [&classes] () {
        classes.classMethode();
    });

    //------------------AWAIT----------------------

    auto a = asynco.async ( []() {
        sleep_to(1000);   //only for simulating long duration function
        return 5;
    });

    cout << asynco.await(a) << endl;

    /**
    * await async function call and use i cout
    */

    cout << asynco.await(asynco.async ( [] () {
        sleep_to(1000);
        cout << "await_ end" << endl;
        return 4;
    })) << endl;


    asynco.await ([]()  {       // run in runtime and await now
        cout << "Hello" << endl;
    });

    asynco.join();
    return 0;
}