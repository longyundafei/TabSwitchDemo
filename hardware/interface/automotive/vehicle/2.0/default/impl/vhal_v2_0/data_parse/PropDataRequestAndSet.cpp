/*
 * Copyright (C) 2016 The Android Open Source Project
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

#define LOG_TAG "automotive.vehicle@2.0-service"
#include <android/log.h>
#include <hidl/HidlTransportSupport.h>
#include <iostream>
#include <vhal_v2_0/EmulatedUserHal.h>
#include <vhal_v2_0/EmulatedVehicleConnector.h>
#include <vhal_v2_0/EmulatedVehicleHal.h>
#include <vhal_v2_0/VehicleHalManager.h>
#include <android/hardware/automotive/vehicle/2.0/IVehicle.h>
#include <utils/Log.h>
#include <unordered_set>
#include <hidl/ServiceManagement.h>

using namespace android;
using namespace android::hardware;
using namespace android::hardware::automotive::vehicle::V2_0;
using ::android::sp;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;

constexpr auto kTimeout = std::chrono::milliseconds(500);
//constexpr auto kInvalidProp = 0x31600207;

class VtsVehicleCallback : public IVehicleCallback {
  private:
    using MutexGuard = std::lock_guard<std::mutex>;
    using HidlVecOfValues = hidl_vec<VehiclePropValue>;
    std::mutex mLock;
    std::condition_variable mEventCond;
    std::vector<HidlVecOfValues> mReceivedEvents;

  public:
    Return<void> onPropertyEvent(const hidl_vec<VehiclePropValue>& values) override {
        {
            MutexGuard guard(mLock);
            mReceivedEvents.push_back(values);
            ALOGI("--------------dxy---------------mReceivedEvents.push_back...");
        }
        mEventCond.notify_one();
        ALOGI("--------------dxy---------------onPropertyEvent...");
        return Return<void>();
    }

    Return<void> onPropertySet(const VehiclePropValue& /* value */) override {
        ALOGI("--------------dxy---------------onPropertySet...");
        return Return<void>();
    }
    Return<void> onPropertySetError(StatusCode /* errorCode */, int32_t /* propId */,
                                    int32_t /* areaId */) override {
        ALOGI("--------------dxy---------------onPropertySetError...");                                        
        return Return<void>();
    }

    bool waitForExpectedEvents(size_t expectedEvents) {
        std::unique_lock<std::mutex> g(mLock);
        ALOGI("--------------dxy---------------waitForExpectedEvents...");
        if (expectedEvents == 0 && mReceivedEvents.size() == 0) {
            return mEventCond.wait_for(g, kTimeout) == std::cv_status::timeout;
        }

        while (expectedEvents != mReceivedEvents.size()) {
            if (mEventCond.wait_for(g, kTimeout) == std::cv_status::timeout) {
                return false;
            }
        }
        return true;
    }

    void reset() { 
        ALOGI("--------------dxy---------------reset()");
        mReceivedEvents.clear(); 
    }
};

class VehicleHalHidlTest {
  public:
    virtual void SetUp(){
        ALOGI("--------------dxy---------------reset()");
        mVehicle = IVehicle::getService("default");
    }
    virtual void TearDown(){
        ALOGI("--------------dxy---------------TearDown()");
    }

    sp<IVehicle> mVehicle;
    // 添加 VehicleHalManager 成员变量
    VehicleHalManager* mHalManager;

    void getpropAllPropConfigs(){
        ALOGD("--------------dxy-------------- getpropAllPropConfigs()");
        bool isCalled = false;
        hidl_vec<VehiclePropConfig> propConfigs;
        if(nullptr != mVehicle){
            mVehicle->getAllPropConfigs([&isCalled, &propConfigs](const hidl_vec<VehiclePropConfig>& cfgs) {
            propConfigs = cfgs;
            ALOGD("--------------dxy-------------- propConfigs.size() = %zu",propConfigs.size());
            isCalled = true;
            });
        }
    }

    bool isBooleanGlobalProp(int32_t property) {
        ALOGI("--------------dxy---------------isBooleanGlobalProp()");
        return (property & (int)VehiclePropertyType::MASK) == (int)VehiclePropertyType::BOOLEAN &&
               (property & (int)VehicleArea::MASK) == (int)VehicleArea::GLOBAL;
    }

    virtual ~VehicleHalHidlTest() {
        ALOGI("--------------dxy---------------~VehicleHalHidlTest()");
    }

    void invokeGet(int32_t property, int32_t areaId) {
        ALOGI("--------------dxy---------------invokeGet");
        VehiclePropValue requestedValue{};
        requestedValue.prop = property;
        requestedValue.areaId = areaId;
        invokeGet(requestedValue);
    }

    void invokeGet(const VehiclePropValue& requestedPropValue) {
        ALOGI("invokeGet(const VehiclePropValue& requestedPropValue)");
        mActualValue = VehiclePropValue{};  // reset previous values
        StatusCode refStatus;
        VehiclePropValue refValue;
        bool isCalled = false;
        mVehicle->get(requestedPropValue,
                      [&refStatus, &refValue, &isCalled](StatusCode status,const VehiclePropValue& value) {
                          refStatus = status;
                          refValue = value;
                          isCalled = true;
                      });
        mActualValue = refValue;
        mActualStatusCode = refStatus;
    }

    VehiclePropValue mActualValue;
    StatusCode mActualStatusCode;
};

void threadFunction(){
    ALOGI("threadFounction()");
    auto  store = std::make_unique<VehiclePropertyStore>();
   // 创建 VehicleHalHidlTest 实例
    VehicleHalHidlTest testInstance;

    // 调用 SetUp() 方法进行初始化
    testInstance.SetUp();
    
    // 在这里可以调用 VehicleHalHidlTest 中的其他成员函数
    // 例如：testInstance.invokeGet(...);

    // 调用 TearDown() 方法进行清理
    testInstance.TearDown();
    while(true){
        ALOGI("--------------dxy---------------threadFounction...");
        std::this_thread::sleep_for(std::chrono::milliseconds(5000));
        testInstance.getpropAllPropConfigs();
    }
}