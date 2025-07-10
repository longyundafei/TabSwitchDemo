#ifndef SENDQUEUE_H
#define SENDQUEUE_H

#include <queue>
#include <vector>
#include <mutex>
#include <condition_variable>

class SendQueue {
private:
    std::queue<std::vector<uint8_t>> queue_;
    std::mutex mutex_;
    std::condition_variable cv_;

public:
    void enqueue(const std::vector<uint8_t>& message);
    std::vector<uint8_t> dequeue();
};

#endif  // SENDQUEUE_H

