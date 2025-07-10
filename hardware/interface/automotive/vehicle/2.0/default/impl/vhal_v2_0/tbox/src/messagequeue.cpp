#include "messagequeue.h"

void MessageQueue::enqueue(const std::vector<uint8_t>& message) {
    std::lock_guard<std::mutex> lock(mutex_);
    queue_.push(message);
    cv_.notify_one();
}
void MessageQueue::nocvenqueue(const std::vector<uint8_t>& message) {
    std::lock_guard<std::mutex> lock(mutex_);
    queue_.push(message);
}
void MessageQueue::notifydequeue() {
    decv_.notify_one();
}
int MessageQueue::cvdequeue(int timeoutMs) {
    std::unique_lock<std::mutex> lock(mutex_);
    if (!decv_.wait_for(lock, std::chrono::milliseconds(timeoutMs), [] { return 1; })) return 1;
    queue_.pop();
    return 0;
}
std::vector<uint8_t> MessageQueue::dequeue() {
    std::unique_lock<std::mutex> lock(mutex_);
    cv_.wait(lock, [this] { return !queue_.empty(); });
    std::vector<uint8_t> message = queue_.front();
    queue_.pop();
    return message;
}

std::vector<uint8_t> MessageQueue::dequeue(int timeoutMs) {
    std::unique_lock<std::mutex> lock(mutex_);

    if (queue_.empty()) {
        // 等待超时或者队列有新的消息入队
        if (!cv_.wait_for(lock, std::chrono::milliseconds(timeoutMs),
                          [this] { return !queue_.empty(); })) {
            // 超时，返回空向量
            return std::vector<uint8_t>();
        }
    }

    std::vector<uint8_t> message = queue_.front();
    queue_.pop();
    return message;
}

int MessageQueue::WaitQueue(int timeoutMs) {
    std::unique_lock<std::mutex> lock(mutex_);
    if (queue_.empty()) {
        // 等待超时或者队列有新的消息入队
        if (!cv_.wait_for(lock, std::chrono::milliseconds(timeoutMs),
                          [this] { return !queue_.empty(); })) {
            // 超时，返回空向量
            return 1;
        }
    }
    return 0;
}
std::vector<uint8_t> MessageQueue::getMsgNoPop() {
    std::unique_lock<std::mutex> lock(mutex_);
    std::vector<uint8_t> message = queue_.front();
    return message;
}
std::vector<uint8_t> MessageQueue::noCheckdequeue() {
    std::unique_lock<std::mutex> lock(mutex_);
    std::vector<uint8_t> message = queue_.front();
    queue_.pop();
    return message;
}
size_t MessageQueue::getQueueSize() {
    return queue_.size();
}
void MessageQueue::clear() {
    while (!queue_.empty()) {
        queue_.pop();
    }
}