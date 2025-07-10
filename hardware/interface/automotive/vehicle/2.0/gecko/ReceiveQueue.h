#ifndef RECEIVEQUEUE_H
#define RECEIVEQUEUE_H

#include <queue>
#include <vector>
#include <mutex>
#include <condition_variable>
#define MAX_BUFFER_SIZE 1024

class ReceiveQueue {
private:
    std::queue<std::vector<uint8_t>> queue_;
    std::mutex mutex_;
    std::condition_variable cv_;

public:
    void enqueue(const std::vector<uint8_t>& message);
    std::vector<uint8_t> dequeue();
};

#endif  // RECEIVEQUEUE_H

