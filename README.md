
# Asynco

A C++ library for event-driven asynchronous multi-threaded programming.

## Features

- Object oriented
- Small and easy to integrate
- Header only
- Asynchronous programming
- Multithread
- Asynchronous timer functions: interval, timeout
- Typed events (on, emit)
- Event loops
- Parallel execution loops
## Installation

Just download the latest release and unzip it into your project. 

```c++
#include "asynco/lib/asynco.hpp"    // asynco(), wait()
#include "asynco/lib/event.hpp"     // event
#include "asynco/lib/rotor.hpp"     // interval, timeout
#include "asynco/lib/runner.hpp"    // on_async
using namespace marcelb;

#ifndef ON_RUNNER
#define ON_RUNNER
runner on_async;
#endif
```

## Usage

Time asynchronous functions

```c++
// start interval
interval inter1 ([]() {
     cout << "Interval 1" << endl;
}, 1000);

// stop interval
inter1.clear();

// start timeout
timeout time1 ( [] () {
    cout << "Timeout 1 " << endl;
}, 10000);

// stop timeout
time1.clear();
```
Make functions asynchronous

```c++
/**
* Put task directly and get returned value - it is not recommended to use it
*/

auto res1 = on_async.put_task( [] () {
    cout << "Not except " <<endl;
    throw string ("Is except!");
});

try {
    res1.get();
} catch (const string except) {
    cout << except << endl;
}

/**
* Run an lambda function asyncronic
*/

asynco( []() {
    sleep_for(2s);   // only for simulating long duration function
    cout << "asynco" << endl;
    return 5;
});


/**
 * Run not lambda function
*/

void notLambdaFunction() {
    cout << "Call to not lambda function" << endl;
}

asynco (notLambdaFunction);

/**
 * Run class method
*/

class clm {
    public:
    void classMethode() {
        cout << "Call class method" << endl;
    }
};

clm classes;
asynco( [&classes] () {
    classes.classMethode();
});



/**
* Wait after runned as async
*/

auto a = asynco( []() {
    sleep_for(2s);   // only for simulating long duration function
    cout << "asynco" << endl;
    return 5;
});

cout << wait(move(a)) << endl;

/**
* Wait async function call and use i cout
*/

cout << wait(asynco( [] () {
    sleep_for(chrono::seconds(1)); // only for simulating long duration function
    cout << "wait end" << endl;
    return 4;
})) << endl;

/**
* Sleep with timeout sleep implement
*/

void sleep_to (int _time) {
    promise<void> _promise;
    timeout t( [&]() {
        _promise.set_value();
    }, _time);

    return _promise.get_future().get();
}

sleep_to(3000);

/**
* Catch promise reject
*/

void promise_reject (int _time) {
    promise<void> _promise;
    timeout t( [&]() {
        try {
            // simulate except
            throw runtime_error("Error simulation");
            _promise.set_value();
        } catch (...) {
            _promise.set_exception(current_exception());
        }
    }, _time);

    return _promise.get_future().get();
}

try {
    promise_reject(3000);
} catch (runtime_error err) {
    cout<< err.what() << endl;
}
```
Events

```c++
/**
* initialization of typed events
*/

event<int, int> ev2int;
event<int, string> evintString;
event<> evoid;

ev2int.on("sum", [](int a, int b) {
    cout << "Sum " << a+b << endl;
});

evintString.on("substract", [](int a, string b) {
    cout << "Substract " << a-stoi(b) << endl;
});

evoid.on("void", []() {
    cout << "Void emited" << endl;
});

sleep(1);

/**
* Emit
*/

ev2int.emit("sum", 5, 8);

sleep(1);
evintString.emit("substract", 3, to_string(2));

sleep(1);
evoid.emit("void");
```
Extend own class whit events

```c++
class myOwnClass : public event<int> {
    public:
    myOwnClass() : event() {};
};

myOwnClass myclass;

timeout t( [&] {
    myclass.emit("constructed", 1);
}, 200);

myclass.on("constructed", [] (int i) {
    cout << "Constructed " << i  << endl;
});

```
## License

[APACHE 2.0](http://www.apache.org/licenses/LICENSE-2.0/)


## Support & Feedback

For support and any feedback, contact the address: marcelb96@yahoo.com.

## Contributing

Contributions are always welcome!

Feel free to fork and start working with or without a later pull request. Or contact for suggest and request an option.

