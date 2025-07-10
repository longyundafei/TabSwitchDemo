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
    static SendQueue* instance;
    SendQueue(){}
    SendQueue(const SendQueue&) = delete;
    SendQueue& operator=(const SendQueue&) = delete;
public:
    static SendQueue* getInstance();
    static void deleteInstance();
    uint32_t msg_num;
    void enqueue(const std::vector<uint8_t>& message);
    std::vector<uint8_t> dequeue();
    std::vector<uint8_t> dequeue_to_spi(uint32_t num);
};

#endif  // SENDQUEUE_H
