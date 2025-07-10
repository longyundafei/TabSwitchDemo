#ifndef SENDQUEUEV1_1_H
#define SENDQUEUEV1_1_H

#include <queue>
#include <vector>
#include <mutex>
#include <condition_variable>

class SendQueueV1_1 {
private:
    std::queue<std::vector<uint8_t>> queue_;
    std::mutex mutex_;
    std::condition_variable cv_;
public:
    void enqueue(const std::vector<uint8_t>& message);
    std::vector<uint8_t> dequeue();
    bool dequeueTimeout();
    std::vector<uint8_t> dequeueTimeouta(int timeout);
    std::vector<uint8_t> dequeueTimeoutb();
    int getsize();
    int getfrontsize();
};

#endif  // SENDQUEUE_H
