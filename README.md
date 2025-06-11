
# Asynco

A C++ library for event-driven asynchronous multi-threaded programming that serves as a runtime for asynchronous operations. It acts as a wrapper around the Boost.Asio library, providing a cleaner way to write asynchronous, concurrent, and parallel code utilizing a set of threads and an event loops. It offers features for event-driven programming, timers, and coroutine support.

## Motivation

The initial goal was to create an interface that makes it easy and clean to asynchronously invoke any function in C++ without resorting to complex calls. Initially, the library was built around a custom implementation of a scheduling loop for queuing functions. However, this part was later replaced with Boost.Asio, mainly for its timer functionality. As the library evolved, it expanded to include a thread pool, each with its own event loop, and adopted event-driven programming. This enhancement also introduced functions specifically designed for asynchronous operations, including periodic and delayed execution.

The asynchronous filesystem was included solely to demonstrate how users can wrap any time- or I/O-intensive functions for asynchronous execution.

## Features

- Object oriented
- Small and easy to integrate
- Asynchronous programming
- Multithread
- Asynchronous timer functions: Periodic, Delayed (like setInterval and setTimeout from JS)
- Typed events (on, tick, off) (like EventEmitter from JS: on, emit, etc)
- Event loops
- Multiple parallel execution loops
- Based on ASIO (Boost Asio)
- On C++20 support Boost.Asio coroutines
## Installation

Just download the latest release and unzip it into your project. 

```c++
// for default global runtime

#include "asynco/lib/asynco_default.hpp"


using namespace marcelb;
using namespace asynco;

int main() {
    asynco_default_run();

    // code

    asynco_default_join()
    return 0;
}

// own instace of runtime

#include "asynco/lib/asynco.hpp"

using namespace marcelb;
using namespace asynco;

int main() {
    Asynco asynco;
    asynco.run(2);

    // code

    asynco.join();
    return 0;
}

```

## Usage

In the following sections, we will explore timers, function execution via the runtime, asynchronous invocation, and waiting for results. We will cover essential use cases involving triggers, and coroutines.

### Timers

We have one timer classes, int two mode Periodic (which runs a callback function periodically), and Delayed (delayed runs a callback function only once).

```c++
// start periodic
Timer inter1 = periodic ([]() {
     cout << "Interval 1" << endl;
}, 1000);

// or usint own instance runtime
/**
 *  Asynco asynco;
 *  asynco.run(2);
 *  Timer inter1 = asynco.periodic ([]() {
 *       cout << "Interval 1" << endl;
 *   }, 1000);
*/
// stop periodic
inter1.stop();

// how many times it has expired
int t = inter1.ticks();

// is it stopped
bool stoped = inter1.stoped();

// start delayed
Timer time1 = delayed( [] () {
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
    sleep(2);   // only for simulating long duration function
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

To wait for the result (blocking the flow) use `await_` (This does not block the event loop in principle. If the result is not ready for a short time, it starts another job in place while it waits.)

```c++

auto a = async_ ( []() {
    sleep(2);   // only for simulating long duration function
    cout << "nonsync " << endl;
    return 5;
});

cout << await_(a) << endl;

/**
* await_ async function call and use i cout
*/

cout << await_(async_ ( [] () {
    sleep(1); // only for simulating long duration function
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
Here too you can use your own runtime instance, only the methods are `.async()` and `.await()`

### Triggers

The library implements Triggers, which are basically typed Events.

```c++
/**
* initialization of typed events
*/

Trigger<int, int> ev2int = trigger<int, int>();
Trigger<int, string> evintString = trigger<int, string>();
Trigger<> evoid = trigger<>();

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
    myOwnClass() : Trigger(asynco_default_runtime()) {};
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
    ClassWithTriggers(): emitter1(asynco_default_runtime()), emitter2(asynco_default_runtime()) {}

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

## Coroutine

If `define.hpp` is included, you can initialize coroutines with `boost::asio::awaitable<T>`.

```c++

awaitable<int> c2(int a) {
    co_return a * 2;
}

```
To run the coroutine at runtime, simply call:
```c++

async_(c2(4));

```
Or using a lambda expression:

```c++

async_([]() -> awaitable<void> {
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

await_ ([]() -> awaitable<void> {
    cout << "Hello" << endl;
    co_return;
}());

```

If you need a result, you can also retrieve it with `await_`.
Here too you can use your own runtime instance, only the methods are `.async()` and `.await()`

## License

[APACHE 2.0](http://www.apache.org/licenses/LICENSE-2.0/)


## Support & Feedback

For support and any feedback, contact the address: marcelb96@yahoo.com.

## Contributing

Contributions are always welcome!

Feel free to fork and start working with or without a later pull request. Or contact for suggest and request an option.

