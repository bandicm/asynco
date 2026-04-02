#include "../lib/asynco_default.hpp"
using namespace marcelb::asynco;

#include <iostream>
using namespace std;

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

int main() {
    asynco_default_run();

    /**
    * initialization of typed events
    */

    // Trigger<int, int> ev2int = trigger<int, int>();
    // Trigger<int, string> evintString = trigger<int, string>();
    // Trigger<> evoid = trigger<>();

    // ev2int.on("sum", [](int a, int b) {
    //     cout << "Sum " << a+b << endl;
    // });

    // evintString.on("substract", [](int a, string b) {
    //     cout << "Substract " << a-stoi(b) << endl;
    // });

    // evoid.on("void", []() {
    //     cout << "Void emited" << endl;
    // });

    // // multiple listeners

    // string emited2 = "2";

    // evoid.on("void", [&]() {
    //     cout << "Void emited " << emited2 << endl;
    // });

    // sleep(1);

    // /**
    // * Emit
    // */

    // ev2int.tick("sum", 5, 8);

    // sleep(1);
    // evintString.tick("substract", 3, to_string(2));

    // sleep(1);
    // evoid.tick("void");

    // Turn off the event listener

    // evoid.off("void");
    // evoid.tick("void"); // nothing is happening

    // class myOwnClass : public Trigger<int> {
    //     public:
    //     myOwnClass() : Trigger(asynco_default_runtime()) {};
    // };

    // myOwnClass myclass;

    // Timer t = delayed( [&] {
    //     myclass.tick("constructed", 1);
    // }, 200);

    // myclass.on("constructed", [] (int i) {
    //     cout << "Constructed " << i  << endl;
    // });

    // ClassWithTriggers mt;

    // mt.on<int>("int", function<void(int)>([&](int i) {
    //     cout << "Emit int " << i << endl;
    // }));

    // mt.on<string>("string", function<void(string)>([&](string s) {
    //     cout << "Emit string " << s << endl;
    // }));

    // mt.tick("int", 5);
    // mt.tick("string", string("Hello world"));

    asynco_default_join();
    return 0;
}