
# Asynco

A C++ library for event-driven asynchronous multi-threaded programming that serves as a runtime for asynchronous operations. It acts as a wrapper around the Boost.Asio library, providing a cleaner way to write asynchronous, concurrent, and parallel code utilizing a set of threads and an event loops. It offers features for event-driven programming, timers, and coroutine support.

## Motivation

The initial goal was to create an interface that makes it easy and clean to asynchronously invoke any function in C++ without resorting to complex calls. Initially, the library was built around a custom implementation of a scheduling loop for queuing functions. However, this part was later replaced with Boost.Asio, mainly for its timer functionality. As the library evolved, it expanded to include a thread pool, each with its own event loop, and adopted event-driven programming. This enhancement also introduced functions specifically designed for asynchronous operations, including periodic and delayed execution.

The asynchronous filesystem was included solely to demonstrate how users can wrap any time- or I/O-intensive functions for asynchronous execution.

## Features

- Object oriented
- Small and easy to integrate
- Header only
- Asynchronous programming
- Multithread
- Asynchronous timer functions: Periodic, Delayed (like setInterval and setTimeout from JS)
- Typed events (on, tick, off) (like EventEmitter from JS: on, emit, etc)
- Event loops
- Multiple parallel execution loops
- Asynchronous file IO
- Based on ASIO (Boost Asio)
- On C++20 support Boost.Asio coroutines
## Installation

Just download the latest release and unzip it into your project. 

```c++
#define NUM_OF_RUNNERS 8                // To change the number of threads used by asynco, without this it runs according to the number of cores

#include "asynco/lib/asynco.hpp"        // async_ (), await_()
#include "asynco/lib/triggers.hpp"      // Trigger (event emitter)
#include "asynco/lib/timers.hpp"        // Periodic, Delayed (like setInterval and setTimeout from JS)
#include "asynco/lib/filesystem.hpp"    // for async read and write files
#include "asynco/lib/define.hpp"        // async_, await_, asyncable_ defines

using namespace marcelb;
using namespace asynco;

// At the end of the main function, always set
_asynco_engine.run();
return 0;

```

## Usage

In the following sections, we will explore timers, function execution via the runtime, asynchronous invocation, and waiting for results. We will cover essential use cases involving triggers, file handling, and coroutines.

### Timers

We have two timer classes, Periodic (which runs a callback function periodically), and Delayed (delayed runs a callback function only once).

```c++
// start periodic
Periodic inter1 ([]() {
     cout << "Interval 1" << endl;
}, 1000);

// stop periodic
inter1.stop();

// how many times it has expired
int t = inter1.ticks();

// is it stopped
bool stoped = inter1.stoped();

// start delayed
Delayed time1 ( [] () {
    cout << "Timeout 1 " << endl;
}, 10000);

// stop delayed
time1.stop();

// is it expired
int t = time1.expired();

// is it stopped
bool stoped = time1.stoped();

```
### Make functions asynchronous

Running functions at runtime, asynchronous execution, uses the `async_` call and its return type is `std::future<T>`

```c++
/**
* Run an lambda function asynchronously
*/

async_ ( []() {
    sleep_for(2s);   // only for simulating long duration function
    cout << "nonsync " << endl;
    return 5;
});


/**
 * Run not lambda function
*/

void notLambdaFunction() {
    cout << "Call to not lambda function" << endl;
}

async_ (notLambdaFunction);

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
async_ ( [&classes] () {
    classes.classMethode();
});
```

To wait for the result (blocking the flow) use `await_` (basically nothing more than a `.get()` call on a future object)

```c++

auto a = async_ ( []() {
    sleep_for(2s);   // only for simulating long duration function
    cout << "nonsync " << endl;
    return 5;
});

cout << await_(a) << endl;

/**
* await_ async function call and use i cout
*/

cout << await_(async_ ( [] () {
    sleep_for(chrono::seconds(1)); // only for simulating long duration function
    cout << "await_ end" << endl;
    return 4;
})) << endl;

```

If you want to run asynchronously but need the result immediately, you can use a shorter notation

```c++

await_ ([]()  {
    cout << "Hello" << endl;
});

```
If multiple function calls do not depend on each other, you can call them and wait for the results later, better concurrency.

```c++

auto a = async_ ( []() {
    cout << "A" << endl;
    return 3;
});

auto b = async_ ( []() {
    cout << "B" << endl;
    throw runtime_error("Test exception");
    return;
});

auto c = async_ ( []() {
    cout << "C" << endl;
    return "Hello";
});

int a_;
string c_;

auto await_all = [&] () {
    a_ = await_(a);
    await_(b);
    c_ = await_(c);
};

try {
    await_all();
    cout << "a_ " << a_ << " c_ " << c_ << endl;
} catch (const exception& exc) {
    cout << exc.what() << endl;
}

// //  same type 

vector<future<void>> fut_vec;
for (int i=0; i<5; i++) {
    fut_vec.push_back(
        async_ ( [i]() {
            cout << "Async_ " << i << endl;
        })
    );
}

auto await_all = [&] () {
    for (int i=0; i<fut_vec.size(); i++) {
        await_ (fut_vec[i]);
    }
};

```
Just an example:

```c++

/**
 * Sleep with delayed sleep implement
 **/ 

void sleep_to (int _time) {
    promise<void> _promise;
    Delayed t( [&]() {
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
    Delayed t( [&]() {
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

### Triggers

The library implements Triggers, which are basically typed Events.

```c++
/**
* initialization of typed events
*/

Trigger<int, int> ev2int;
Trigger<int, string> evintString;
Trigger<> evoid;

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
class myOwnClass : public Trigger<int> {
    public:
    myOwnClass() : Trigger() {};
};

myOwnClass myclass;

Delayed t( [&] {
    myclass.tick("constructed", 1);
}, 200);

myclass.on("constructed", [] (int i) {
    cout << "Constructed " << i  << endl;
});

```

Implementing a class with multiple triggers of different types

```c++

class ClassWithTriggers {
    Trigger<int> emitter1;
    Trigger<string> emitter2;

public:
    template<typename... T>
    void on(const string& key, function<void(T...)> callback) {
        if constexpr (sizeof...(T) == 1 && is_same_v<tuple_element_t<0, tuple<T...>>, int>) {
            emitter1.on(key, callback);
        }
        else if constexpr (sizeof...(T) == 1 && is_same_v<tuple_element_t<0, tuple<T...>>, string>) {
            emitter2.on(key, callback);
        }
    }

    template <typename... Args>
    void tick(const string& key, Args&&... args) {
        if constexpr (sizeof...(Args) == 1 && is_same_v<tuple_element_t<0, tuple<Args...>>, int>) {
            emitter1.tick(key, forward<Args>(args)...);
        }
        else if constexpr (sizeof...(Args) == 1 && is_same_v<tuple_element_t<0, tuple<Args...>>, string>) {
            emitter2.tick(key, forward<Args>(args)...);
        }
        else {
            static_assert(sizeof...(Args) == 0, "Unsupported number or types of arguments");
        }
    }
};

ClassWithTriggers mt;

mt.on<int>("int", function<void(int)>([&](int i) {
    cout << "Emit int " << i << endl;
}));

mt.on<string>("string", function<void(string)>([&](string s) {
    cout << "Emit string " << s << endl;
}));

mt.tick("int", 5);
mt.tick("string", string("Hello world"));

```

Another example:
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
    string data = await_(future_data);
} catch (exception& err) {
    cout << err.what() << endl;
}

auto future_status = fs::write("test.txt", "Hello world");

try {
    await_(future_status);
} catch (exception& err) {
    cout << err.what() << endl;
}

```

## Coroutine

If `define.hpp` is included, you can initialize coroutines using `asyncable<T>`; if not, just use `boost::asio::awaitable<T>`.

```c++

asyncable<int> c2(int a) {
    co_return a * 2;
}

```
To run the coroutine at runtime, simply call:
```c++

async_(c2(4));

```
Or using a lambda expression:

```c++

async_([]() -> asyncable<void> {
    std::cout << "Hello" << std::endl;
    co_await c2(4);
    co_return;
}());

```
To retrieve results from coroutines, you can do so as you would from classical functions by calling `await_`:
```c++

int r = await_(
    async_(
        c2(10)
));

```

If you need the result immediately, you can use a shorter notation

```c++

auto a =  await_ ( c2(3));
cout << a << endl;

await_ ([]() -> asyncable<void> {
    cout << "Hello" << endl;
    co_return;
}());

```

Timers and triggers work the same with coroutines; it is important to call the coroutine with `async_` in the callback, and to call `async_`, wrap it with a lambda expression:

```c++

Periodic p([]() {
    async_(c2(34));
}, 2000);

```
If you need a result, you can also retrieve it with `await_`.


## License

[APACHE 2.0](http://www.apache.org/licenses/LICENSE-2.0/)


## Support & Feedback

For support and any feedback, contact the address: marcelb96@yahoo.com.

## Contributing

Contributions are always welcome!

Feel free to fork and start working with or without a later pull request. Or contact for suggest and request an option.

