#include "SendQueue.h"

void SendQueue::enqueue(const std::vector<uint8_t>& message) {
    std::lock_guard<std::mutex> lock(mutex_);
    queue_.push(message);
    cv_.notify_one();
}

std::vector<uint8_t> SendQueue::dequeue() {
    std::unique_lock<std::mutex> lock(mutex_);
    cv_.wait(lock, [this] { return !queue_.empty(); });
    std::vector<uint8_t> message = queue_.front();
    queue_.pop();
    return message;
}

