#include "BinaryQueue.h"
#include <log/log.h>

BinaryQueue::BinaryQueue(size_t capacity) : capacity(capacity), frontIndex(0), rearIndex(0) {
    queueData = new unsigned char[capacity];
}

BinaryQueue::~BinaryQueue() {
    delete[] queueData;
}

void BinaryQueue::enqueue(const unsigned char* data, size_t length) {
    for (size_t i = 0; i < length; i++) {
        queueData[rearIndex] = data[i];
        rearIndex = (rearIndex + 1) % capacity;
    }
}

void BinaryQueue::dequeue(unsigned char* data, size_t length) {
    for (size_t i = 0; i < length; i++) {
        data[i] = queueData[frontIndex];
        frontIndex = (frontIndex + 1) % capacity;
    }
    // ALOGI("dequeue frontIndex:%zu ", frontIndex);
}

size_t BinaryQueue::length() const {
    if (rearIndex >= frontIndex) {
        return rearIndex - frontIndex;
    } else {
        return (capacity - frontIndex) + rearIndex;
    }
}
