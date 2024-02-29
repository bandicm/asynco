#ifndef _LOOP_
#define _LOOP_

#include <thread>
#include <vector>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <future>

using namespace std;

namespace marcelb {

#ifdef ON_ASYNC
extern AsyncLoop on_async;
#endif

class AsyncLoop {
    private:
    vector<thread> workers;
    queue<function<void()>> tasks;
    mutex q_io;
    condition_variable cv;
    bool stop;

    public:
    AsyncLoop(size_t pool_size = thread::hardware_concurrency()) : stop(false) {
        for (size_t i = 0; i < pool_size; ++i) {
            workers.emplace_back([this] {
                while (true) {
                    function<void()> task;
                    {
                        unique_lock<mutex> lock(q_io);
                        cv.wait(lock, [this] { return stop || !tasks.empty(); });
                        if (stop && tasks.empty())
                            return;
                        task = move(tasks.front());
                        tasks.pop();
                    }
                    task();
                }
            });
        }
    }

    template<class F, class... Args>
    auto put_task(F&& f, Args&&... args)
        -> future<typename result_of<F(Args...)>::type> {
        using return_type = typename result_of<F(Args...)>::type;

        auto task = make_shared<packaged_task<return_type()>>(bind(forward<F>(f), forward<Args>(args)...));
        future<return_type> res = task->get_future();
        {
            unique_lock<mutex> lock(q_io);

            if (stop) {
                throw runtime_error("Pool is stoped!");
            }

            tasks.emplace([task]() { (*task)(); });
        }

        cv.notify_one();
        return res;
    }

    unsigned int count_tasks() {
        return tasks.size();
    }

    unsigned int count_threads() {
        return workers.size();
    }

    ~AsyncLoop() {
        {
            unique_lock<mutex> lock(q_io);
            stop = true;
        }
        cv.notify_all();
        for (thread& worker : workers) {
            worker.join();
        }
    }

};

}

#endif