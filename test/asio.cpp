#include <iostream>
#include <vector>
#include <thread>
#include <functional>
#include <boost/asio.hpp>

using namespace std;

int64_t rtime_ms() {
    return chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now()
        .time_since_epoch())
        .count();
}


// globals
int64_t start = rtime_ms();;
// timer global
boost::asio::io_context io_context;
unique_ptr<boost::asio::io_service::work> work(new boost::asio::io_service::work(io_context));


class timer {
    boost::asio::steady_timer st;
    bool stop = false;
    bool repeate;
    function<void()> callback;
    uint64_t time;

    void init() {
        st.async_wait( [this] (const boost::system::error_code&) {
            if (!stop) {
                callback();
                if (repeate) {
                    reinit();           
                }
            }
        });
    }

    void reinit() {
        st = boost::asio::steady_timer(io_context, boost::asio::chrono::milliseconds(time));
        init();
    }
    
    public:
    timer (function<void()> _callback, uint64_t _time, bool _repeate)
        : st(io_context, boost::asio::chrono::milliseconds(_time)),
        stop(false),
        repeate(_repeate),
        callback(_callback),
        time(_time) {
            
        init();
    }

    void clear() {
        stop = true;
        st.cancel();
    }

    ~timer() {
        clear();
    }
};

vector<shared_ptr<timer>> timer_container;

timer setInterval(int interval, std::function<void()> func) {
    cout << "set interval construction call" << endl;
    // return make_shared<timer> (func, interval, true);
    return timer (func, interval, true);
};

class interval {
    shared_ptr<timer> _timer;

    public:
    interval(function<void()> callback, uint64_t time) : _timer(make_shared<timer> (callback, time, true)) {
        timer_container.push_back(_timer);
    }

};


timer setTimeout(int interval, std::function<void()> func) {
    cout << "set timeout construction call" << endl;
    // return make_shared<timer> (func, interval, false);
    return timer(func, interval, false);
};

// void clearTimer(shared_ptr<timer> _timer) {
//     _timer->stop = true;
//     _timer->st.cancel();
// }

int main() {

    vector<thread> loops;

    for (int i =0; i<8; i++)
    loops.push_back( thread([&]() {
         while (true) {io_context.run();}
    }));
 

    timer timer1 = setInterval(1000, []() {
    //    cout << "Interval end " << rtime_ms() - start << endl;
        cout << "Interval 1 " << this_thread::get_id()  << endl; 
    });

    interval([]() {
    //    cout << "Interval end " << rtime_ms() - start << endl;
        cout << "Interval 2 " << this_thread::get_id()  << endl; 

    }, 1500);

    // timer timer2 = setTimeout(5000, [&]() {
    //    cout << "Timeout end " << rtime_ms() - start << endl;
    //    timer1.clear();
    // });

    for (int i =0; i<4; i++)
        loops[i].join();


    return 0;
}

