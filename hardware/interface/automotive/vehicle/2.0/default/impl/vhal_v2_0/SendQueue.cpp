#include "SendQueue.h"
#include <log/log.h>

#include <android-base/logging.h>

SendQueue* SendQueue::instance = new (std::nothrow) SendQueue();

SendQueue* SendQueue::getInstance()
{
    return instance;
}

void SendQueue::deleteInstance()
{
    if (instance)
    {
        delete instance;
        instance = nullptr;
    }
}

void SendQueue::enqueue(const std::vector<uint8_t>& message) {
    std::lock_guard<std::mutex> lock(mutex_);
    queue_.push(message);
    if (msg_num<65534) {
        msg_num++;
    }
    cv_.notify_one();
}

std::vector<uint8_t> SendQueue::dequeue() {
    std::unique_lock<std::mutex> lock(mutex_);
    cv_.wait(lock, [this] { return !queue_.empty(); });
    std::vector<uint8_t> message = queue_.front();
    queue_.pop();
    if (msg_num>0) {
        msg_num--;
    }
    return message;
}

std::vector<uint8_t> SendQueue::dequeue_to_spi(uint32_t num) {
    std::vector<uint8_t> msg;
	std::vector<uint8_t> msg_it;
    std::vector<uint8_t> temp_msg = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	uint32_t i = 0;
    std::unique_lock<std::mutex> lock(mutex_);
    for (i = 0; i< 60; i++) {
        if (i < num) {
            msg_it = queue_.front();
            queue_.pop();
            msg.insert(msg.end(), msg_it.begin(), msg_it.end());
        }
        else {
            msg.insert(msg.end(), temp_msg.begin(), temp_msg.end());
        }
    }
	//ALOGI("lihy dequeue_to_spi msg.size %lu", msg.size());
    if (num < 60) {
        msg_num = 0;
    } else {
        msg_num -= 60;
    }
    return msg;
}
