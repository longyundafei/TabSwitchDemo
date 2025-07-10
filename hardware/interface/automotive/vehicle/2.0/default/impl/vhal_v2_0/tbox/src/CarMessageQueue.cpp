#include "CarMessageQueue.h"
#include <android-base/logging.h>

CarMessageQueue::CarMessageQueue() {
//    start();
}
void CarMessageQueue::settboxdecl(LockMap<int32_t, TboxDataDeclaration>* mp){
    mpTboxdecl = mp;
}
// 入队列操作
void CarMessageQueue::enqueue(HidlVecOfValues value) {
    std::lock_guard<std::mutex> lock(mLock);
    mReceivedEvents.push_back(value);
    mEventCond.notify_one();  // 唤醒等待新事件的线程
}

// 出队列操作
HidlVecOfValues CarMessageQueue::dequeue() {
    std::unique_lock<std::mutex> lock(mLock);
    mEventCond.wait(lock, [this] { return !mReceivedEvents.empty(); });  // 等待直到有新的事件
    HidlVecOfValues value = mReceivedEvents.front();
    mReceivedEvents.erase(mReceivedEvents.begin());
    // 这里我们简单地将队列中的第一个元素的值作为键，你可能需要修改这个逻辑以适应你的需求。
    for (auto it : value) {
//        mOutput[it.prop] = it;
        (*mpTboxdecl)[it.prop].value = it;
//        LOG(INFO) << __func__ << __LINE__ << (*mpTboxdecl)[it.prop].value.prop;
    }
    return value;
}

// 启动线程进行出队列操作
void CarMessageQueue::start() {
    mThread = std::thread([this] {
        while (true) {
            this->dequeue();
        }
    });
}