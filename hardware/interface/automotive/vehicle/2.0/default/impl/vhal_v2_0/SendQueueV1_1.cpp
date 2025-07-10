#include "SendQueueV1_1.h"
#include <log/log.h>

#include <android-base/logging.h>

void SendQueueV1_1::enqueue(const std::vector<uint8_t>& message) {
    std::lock_guard<std::mutex> lock(mutex_);
    queue_.push(message);
    cv_.notify_one();
}
int SendQueueV1_1::getsize() {
    return queue_.size();
}
int SendQueueV1_1::getfrontsize() {
    if (!queue_.empty())
    {
        return queue_.front().size();
    }
    return 0;
}
std::vector<uint8_t> SendQueueV1_1::dequeue() {
    std::unique_lock<std::mutex> lock(mutex_);
    cv_.wait(lock, [this] { return !queue_.empty(); });
    std::vector<uint8_t> message = queue_.front();
    queue_.pop();
    return message;
}
std::vector<uint8_t> SendQueueV1_1::dequeueTimeouta(int timeout) {
    std::unique_lock<std::mutex> lock(mutex_);
    int timeoutMilliseconds = timeout;
    // 设置超时时间为500毫秒
    std::vector<uint8_t> message;
    auto timeout_time = std::chrono::steady_clock::now() + std::chrono::milliseconds(timeoutMilliseconds);
    if(cv_.wait_until(lock, timeout_time ,[this] { return !queue_.empty(); })){
        message = queue_.front();
        queue_.pop();
        return message;
    }else{
        return message;
    }
}
std::vector<uint8_t> SendQueueV1_1::dequeueTimeoutb() {
    std::unique_lock<std::mutex> lock(mutex_);
//    int timeoutMilliseconds = 1000;
    // 设置超时时间为500毫秒
    std::vector<uint8_t> message;
    auto timeout_time = std::chrono::steady_clock::now() + std::chrono::seconds(1);
    if(cv_.wait_until(lock, timeout_time ,[this] { return !queue_.empty(); })){
        message = queue_.front();
        queue_.pop();
        return message;
    }else{
        return message;
    }
}
bool SendQueueV1_1::dequeueTimeout() {
    std::unique_lock<std::mutex> lock(mutex_);
    int timeoutMilliseconds = 300;
    // 设置超时时间为500毫秒
    auto timeout_time = std::chrono::steady_clock::now() + std::chrono::milliseconds(timeoutMilliseconds);
    if(cv_.wait_until(lock, timeout_time ,[this] { return !queue_.empty(); })){
        queue_.pop();
        return true;
    }
    else
        return false;
}

