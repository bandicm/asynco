#ifndef _RUNNER_
#define _RUNNER_

#include <thread>
#include <vector>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <future>

using namespace std;

namespace marcelb {
namespace asynco {

#define HW_CONCURRENCY_MINIMAL 4

/**
 * The runner class implements multithread, task stack and event loop for asynchronous execution of tasks
*/
class runner {
    private:
    vector<thread> runners;
    queue<function<void()>> tasks;
    mutex q_io;
    condition_variable cv;
    bool stop;

    public:

    /**
     * The constructor starts as many threads as the system has cores, 
     * and runs an event loop inside each one. 
     * Each event loop waits for tasks from the stack and executes them.
    */
    runner(unsigned int _num_of_runners = 0) : stop(false) {
        unsigned int num_of_runners = _num_of_runners;

        if (num_of_runners == 0) {
            #ifdef NUM_OF_RUNNERS
                num_of_runners = NUM_OF_RUNNERS;
            #else
                num_of_runners = thread::hardware_concurrency();
                if (num_of_runners < HW_CONCURRENCY_MINIMAL) {
                    num_of_runners = HW_CONCURRENCY_MINIMAL;
                }
            #endif
        }

        for (size_t i = 0; i < num_of_runners; ++i) {
            runners.emplace_back( thread([&] {
                while (!stop) {
                    function<void()> task;
                    {
                        unique_lock<mutex> lock(q_io);
                        cv.wait(lock, [this] { return stop || !tasks.empty(); });
                        // if (stop && tasks.empty())
                        if (stop)
                            return;
                        task = move(tasks.front());
                        tasks.pop();
                    }
                    task();
                }
            }));
        }
    }

    

    /**
     * With the method, we send the callback function and its arguments to the task stack
    */
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

    /**
     * Returns the number of tasks the runner has to perform
    */
    unsigned int count_tasks() {
        return tasks.size();
    }

    /**
     * Returns the number of threads used by the runner
    */
    unsigned int count_threads() {
        return runners.size();
    }

    /**
     * The destructor stops all loops and stops all threads
    */
    ~runner() {
        {
            unique_lock<mutex> lock(q_io);
            stop = true;
        }
        cv.notify_all();
        for (thread& runner : runners) {
            runner.join();
        }
        runners.clear();
    }

};


/**
 * Internal global library variable
*/
static runner _asyncon;

}
}

#endif