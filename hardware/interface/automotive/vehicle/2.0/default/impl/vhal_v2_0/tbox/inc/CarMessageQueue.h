#ifndef CARMESSAGEQUEUE_H
#define CARMESSAGEQUEUE_H

#include <android/hardware/automotive/vehicle/2.0/IVehicle.h>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <vector>
#include "TboxDataDeclaration.h"
#include "LockMap.h"

using namespace android;
using namespace android::hardware;
using namespace android::hardware::automotive::vehicle::V2_0;
using ::android::sp;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using MutexGuard = std::lock_guard<std::mutex>;
using HidlVecOfValues = hidl_vec<VehiclePropValue>;

class CarMessageQueue {
  public:
    CarMessageQueue();
    // 入队列操作
    void enqueue(HidlVecOfValues value);

    // 出队列操作
    HidlVecOfValues dequeue();
    // 启动线程进行出队列操作
    void start();
    void settboxdecl(LockMap<int32_t, TboxDataDeclaration>* mp);

  public:
    std::map<int, VehiclePropValue> mOutput;  // 引用到外部的map

    // private:
  public:
    std::mutex mLock;
    std::condition_variable mEventCond;
    std::vector<HidlVecOfValues> mReceivedEvents;
    std::thread mThread;
    LockMap<int32_t, TboxDataDeclaration>* mpTboxdecl;
};

#endif  // CARMESSAGEQUEUE_H
