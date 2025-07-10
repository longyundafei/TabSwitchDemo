#ifndef BINARY_QUEUE_H
#define BINARY_QUEUE_H
#include <cstddef>

class BinaryQueue {
private:
    unsigned char* queueData;
    size_t capacity;
    size_t frontIndex;
    size_t rearIndex;

public:
    BinaryQueue(size_t capacity);
    ~BinaryQueue();
    void enqueue(const unsigned char* data, size_t length);
    void dequeue(unsigned char* data, size_t length);
    size_t length() const;
};

#endif  // BINARY_QUEUE_H

