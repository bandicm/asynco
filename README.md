
# Asynco

A C++ library for event-driven asynchronous multi-threaded programming.

## Motivation

The original concept was to create an interface capable of asynchronously calling any function. It has since evolved into a library that incorporates a thread pool, each with its own event loop, event-driven programming, and functions inherently designed for asynchronous operation (including periodic and delayed functions).

The asynchronous filesystem is provided solely to guide users on how to wrap any time- or IO-intensive function for asynchronous execution.

## Features

- Object oriented
- Small and easy to integrate
- Header only
- Asynchronous programming
- Multithread
- Asynchronous timer functions: periodic, delayed (like setInterval and setTimeout from JS)
- Typed events (on, tick, off) (like EventEmitter from JS: on, emit, etc)
- Event loops
- Multiple parallel execution loops
- Asynchronous file IO
- Based on ASIO (Boost Asio)
## Installation

Just download the latest release and unzip it into your project. 

```c++
#define NUM_OF_RUNNERS 8                // To change the number of threads used by asynco, without this it runs according to the number of cores

#include "asynco/lib/asynco.hpp"        // asynco(), wait()
#include "asynco/lib/triggers.hpp"      // trigger (event emitter)
#include "asynco/lib/timers.hpp"        // periodic, delayed (like setInterval and setTimeout from JS)
#include "asynco/lib/filesystem.hpp"    // for async read and write files

using namespace marcelb;
using namespace asynco;
using namespace triggers;

// At the end of the main function, always set
_asynco_engine.run();
return 0;

```

## Usage

Time asynchronous functions

```c++
// start periodic
periodic inter1 ([]() {
     cout << "Interval 1" << endl;
}, 1000);

// stop periodic
inter1.stop();

// how many times it has expired
int t = inter1.ticks();

// is it stopped
bool stoped = inter1.stoped();

// start delayed
delayed time1 ( [] () {
    cout << "Timeout 1 " << endl;
}, 10000);

// stop delayed
time1.stop();

// is it expired
int t = time1.expired();

// is it stopped
bool stoped = time1.stoped();

```
Make functions asynchronous

```c++
/**
* Run an lambda function asynchronously
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

cout << wait(a) << endl;

/**
* Wait async function call and use i cout
*/

cout << wait(asynco( [] () {
    sleep_for(chrono::seconds(1)); // only for simulating long duration function
    cout << "wait end" << endl;
    return 4;
})) << endl;

/**
* Sleep with delayed sleep implement
*/

void sleep_to (int _time) {
    promise<void> _promise;
    delayed t( [&]() {
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
    delayed t( [&]() {
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

trigger<int, int> ev2int;
trigger<int, string> evintString;
trigger<> evoid;

ev2int.on("sum", [](int a, int b) {
    cout << "Sum " << a+b << endl;
});

evintString.on("substract", [](int a, string b) {
    cout << "Substract " << a-stoi(b) << endl;
});

evoid.on("void", []() {
    cout << "Void emited" << endl;
});

// multiple listeners

string emited2 = "2";

evoid.on("void", [&]() {
    cout << "Void emited " << emited2 << endl;
});

sleep(1);

/**
* Emit
*/

ev2int.tick("sum", 5, 8);

sleep(1);
evintString.tick("substract", 3, to_string(2));

sleep(1);
evoid.tick("void");

// Turn off the event listener

evoid.off("void");
evoid.tick("void"); // nothing is happening

```
Extend own class whit events

```c++
class myOwnClass : public trigger<int> {
    public:
    myOwnClass() : trigger() {};
};

myOwnClass myclass;

delayed t( [&] {
    myclass.tick("constructed", 1);
}, 200);

myclass.on("constructed", [] (int i) {
    cout << "Constructed " << i  << endl;
});

```

Asynchronous file IO

```c++
string data_;

fs::read("test.txt", [&data_] (string data, exception* error) {
    if (error) {
        cout << "Error " << error->what() << endl;
    } else {
        cout << "Data " << endl << data << endl;
        data_ = data;
        cout << "Data_" << data_ << endl;
    }
});

fs::write("test1.txt", "Hello world", [] (exception* error) {
    if (error) {
        cout << "Error " << error->what() << endl;
    } else {
        cout << "Write successfuly" << endl;
    }
});

auto future_data = fs::read("test.txt");

try {
    string data = wait(future_data);
} catch (exception& err) {
    cout << err.what() << endl;
}

auto future_status = fs::write("test.txt", "Hello world");

try {
    wait(future_status);
} catch (exception& err) {
    cout << err.what() << endl;
}

```

## License

[APACHE 2.0](http://www.apache.org/licenses/LICENSE-2.0/)


## Support & Feedback

For support and any feedback, contact the address: marcelb96@yahoo.com.

## Contributing

Contributions are always welcome!

Feel free to fork and start working with or without a later pull request. Or contact for suggest and request an option.

