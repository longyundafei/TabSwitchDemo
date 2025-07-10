/*
 * Copyright (C) 2020 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "GarageModeServerSideHandler.h"
#include <chrono>
#include <condition_variable>
#include <fstream>
#include <thread>
#include <errno.h>
#include <sys/inotify.h>
#include <android-base/logging.h>
#include <utils/SystemClock.h>
#include "vhal_v2_0/VehicleUtils.h"
#include "MessageHandler.h"
#include "PropertyManager.h"

namespace android::hardware::automotive::vehicle::V2_0::impl {

using std::chrono::duration_cast;
using std::chrono::steady_clock;
using std::literals::chrono_literals::operator""s;

class GarageModeServerSideHandlerImpl : public GarageModeServerSideHandler {
  public:
    GarageModeServerSideHandlerImpl(
       IVehicleServer* vehicleServer,
       VehiclePropValuePool* vehicleObjectPool,
       CommunicationThread* pthread)
       : mVehicleServer(vehicleServer),
         mValueObjectPool(vehicleObjectPool),
         mCommThread(pthread) {
        mThreads.emplace_back(
                std::bind(&GarageModeServerSideHandlerImpl::HeartbeatTimeoutWatcher, this));
        mThreads.emplace_back(std::bind(&GarageModeServerSideHandlerImpl::SpeedWatcher, this));
    }

    ~GarageModeServerSideHandlerImpl() {
        mShuttingDownFlag.store(true);
        mHeartbeatCV.notify_all();
        for (auto& thread : mThreads) {
            if (thread.joinable()) {
                thread.join();
            }
        }
    }

    void HandleHeartbeat() override;

  private:
    void HeartbeatTimeoutWatcher();
    void SpeedWatcher();
  private:

    recyclable_ptr<VehiclePropValue> CreatespeedReq(float speed);

    IVehicleServer* const mVehicleServer;
    VehiclePropValuePool* const mValueObjectPool;
    CommunicationThread* const mCommThread;

    // TODO(chenhaosjtuacm): use std::filesystem when toolchain >= gcc8 is available
    const std::string mPowerStateMarkerPath;

    std::atomic<bool> mSystemShuttingDownPrepareFlag{false};
    std::atomic<bool> mShuttingDownFlag{false};
    std::atomic<steady_clock::time_point> mLastHeartbeatTime{};
    std::vector<std::thread> mThreads;
    std::condition_variable mHeartbeatCV;
    std::mutex mHeartbeatMutex;
    const std::string mSpeedFilePath = "./speed.txt";  // 车速文件路径

  // 读取速度值函数  
  float read_speed_from_file(const std::string& path) {
    static float speed = 0.0;
    speed = speed + 2.2;
    // 读取速度文件的实现   
    return speed;
  }

  // 构造浮点属性函数
  VehiclePropValue create_float_prop(int32_t prop, float value) {
    VehiclePropValue v;
    v.prop = prop;
    v.value.floatValues.resize(1);
    v.value.floatValues[0] = value;

    return v;
  }
  bool is_modify_event(const struct inotify_event& event) {
    return event.mask & IN_MODIFY; 
  }
};

void GarageModeServerSideHandlerImpl::HandleHeartbeat() {
    LOG(DEBUG) << __func__ << ": received heartbeat from the client";
    mLastHeartbeatTime.store(steady_clock::now());
}

void GarageModeServerSideHandlerImpl::HeartbeatTimeoutWatcher() {
    constexpr auto kHeartbeatTimeout = duration_cast<steady_clock::duration>(5s);
    constexpr auto kHeartbeatCheckPeriod = 1s;
    while (!mShuttingDownFlag.load()) {
        if (!mSystemShuttingDownPrepareFlag.load()) {
            std::unique_lock<std::mutex> heartbeatLock(mHeartbeatMutex);
            mHeartbeatCV.wait(heartbeatLock, [this]() {
                return mSystemShuttingDownPrepareFlag.load() || mShuttingDownFlag.load();
            });

            // Reset mLastHeartbeatTime everytime after entering shutdown state
            HandleHeartbeat();
        }
        auto timeSinceLastHeartbeat = steady_clock::now() - mLastHeartbeatTime.load();
        if (timeSinceLastHeartbeat > kHeartbeatTimeout) {
            LOG(ERROR) << __func__ << ": heartbeat timeout!";
            // TODO(chenhaosjtuacm): Shutdown AGL
            break;
        }
        std::this_thread::sleep_for(kHeartbeatCheckPeriod);
    }
}

//此函数实现车速的监视，当车速发生变化时会调用onPropertyValueFromCar上报车速信息
void GarageModeServerSideHandlerImpl::SpeedWatcher() {
//  MessageHandler* handler = mCommThread->getHandler();
//  handler->getfunctionMap();
  if(mCommThread->getHandler()->getfunctionMap().propmanager.isUpdated("ec_vcu_vehicle_spd")){
    float speed = 0.0;
    speed = std::stof(mCommThread->getHandler()->getfunctionMap().propmanager.findProperty("ec_vcu_vehicle_spd")->value);
    mVehicleServer->onPropertyValueFromCar(*CreatespeedReq(speed),true);;
  }

//  int inotifyFd = inotify_init();
//  alignas(alignof(struct inotify_event)) char buffer[4096];
//  while (!mShuttingDownFlag.load()) {
//    inotify_add_watch(inotifyFd, mSpeedFilePath.c_str(), IN_MODIFY);
//     int len = read(inotifyFd, buffer, sizeof(buffer));
//     if(len<0){
//    LOG(ERROR) << "read err1";
//    }
//    else if(len < static_cast<int>(sizeof(struct inotify_event))){
//      LOG(ERROR)  << "read err2";
//    }else{
//      // 从buffer取出事件
//      struct inotify_event* event = 
//      reinterpret_cast<struct inotify_event*>(buffer);
//      if (is_modify_event(*event)) {
//        float speed = read_speed_from_file(mSpeedFilePath);
//        mVehicleServer->onPropertyValueFromCar(*CreatespeedReq(speed),true);
//      }
//    }
//  }
}

recyclable_ptr<VehiclePropValue> GarageModeServerSideHandlerImpl::CreatespeedReq(float speed) {
    auto req = mValueObjectPool->obtainFloat(speed);
    req->prop = toInt(VehicleProperty::PERF_VEHICLE_SPEED);
    req->areaId = 0;
    req->timestamp = elapsedRealtimeNano();
    req->status = VehiclePropertyStatus::AVAILABLE;
    return req;
}

std::unique_ptr<GarageModeServerSideHandler> makeGarageModeServerSideHandler(
        IVehicleServer* vehicleServer, VehiclePropValuePool* valueObjectPool,CommunicationThread *pthread) {
    return std::make_unique<GarageModeServerSideHandlerImpl>(vehicleServer, valueObjectPool, pthread);
}

}  // namespace android::hardware::automotive::vehicle::V2_0::impl
