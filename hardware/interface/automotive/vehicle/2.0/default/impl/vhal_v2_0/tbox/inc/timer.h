#ifndef _TIMER_H_
#define _TIMER_H_

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <thread>

class Timer {
  public:
    Timer() : expired(true), try_to_expire(false) {}

    Timer(const Timer& timer) {
        expired = timer.expired.load();
        try_to_expire = timer.try_to_expire.load();
    }

    ~Timer() { stop(); }
    std::function<void()> timeoutCallback;
    void start(int delay /*, std::function<void()> task*/) {
        // is started, do not start again
        if (expired == false) {
            stop();
        }
        // start async timer, launch thread and wait in that thread
        expired = false;
        std::thread([this, delay /*, task*/]() {
            while (!try_to_expire) {
                // sleep every delay and do the task again and again until times up
                std::this_thread::sleep_for(std::chrono::seconds(delay));
                // task();
                if (timeoutCallback) {
                    timeoutCallback();
                    break;
                }
            }

            {
                // timer be stopped, update the condition variable expired and wake main thread
                std::lock_guard<std::mutex> locker(_mutex);
                expired = true;
                expired_cond.notify_one();
            }
        }).detach();
    }

    void startOnce(int delay /*, std::function<void()> task*/) {
        std::thread([delay /*, task*/]() {
            std::this_thread::sleep_for(std::chrono::seconds(delay));
            // task();
        }).detach();
    }

    void stop() {
        // do not stop again
        if (expired) return;

        if (try_to_expire) return;

        // wait until timer
        try_to_expire = true;  // change this bool value to make timer while loop stop
        {
            std::unique_lock<std::mutex> locker(_mutex);
            expired_cond.wait(locker, [this] { return expired == true; });

            // reset the timer
            if (expired == true) try_to_expire = false;
        }
    }

  private:
    std::atomic<bool> expired;        // timer stopped status
    std::atomic<bool> try_to_expire;  // timer is in stop process
    std::mutex _mutex;
    std::condition_variable expired_cond;
};

#endif