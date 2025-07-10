#ifndef VEHICLE_CONTROLLER_H
#define VEHICLE_CONTROLLER_H

#include <android-base/logging.h>
#include <android/hardware/automotive/vehicle/2.0/IVehicle.h>
#include <iostream>
#include "CarMessageQueue.h"
#include "TboxDataDeclaration.h"
#include "LockMap.h"

using namespace android;
using namespace android::hardware;
using namespace android::hardware::automotive::vehicle::V2_0;
using ::android::sp;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;

// #define VEHICLE_OPTIONS_NUM 200

struct Item {
    std::string right_name;
    int32_t right_areaId;
    std::string right_value;
    std::string right_value_type;
    SubscribeOptions opt;
    int left_tranMode;
};
class VehicleCallback : public IVehicleCallback {
  private:
  public:
    Return<void> onPropertyEvent(const hidl_vec<VehiclePropValue>& values) override {
        {
            mReceivedEvents.push_back(values);
        }
        LOG(INFO) << "TBOXVehicleCallback onPropertyEvent";
        std::unique_lock<std::mutex> lock(mLock);
        mEventCond.notify_one();
        return Return<void>();
    }
    Return<void> onPropertySet(const VehiclePropValue& /* value */) override {
        LOG(INFO) << "TBOXVehicleCallback onPropertySet";
        //std::unique_lock<std::mutex> lock(mLock);
        //mEventCond.notify_one();
        return Return<void>();
    }
    Return<void> onPropertySetError(StatusCode /* errorCode */, int32_t /* propId */,
                                    int32_t /* areaId */) override {
        LOG(INFO) << "TBOXVehicleCallback onPropertySetError";
        return Return<void>();
    }
    void reset() {
        mReceivedEvents.clear();
    }

    const std::vector<HidlVecOfValues>& getReceivedEvents() {
        return mReceivedEvents;
    }

    void getUpdateSOCPowerState(){
        std::unique_lock<std::mutex> lock(mLock);
        LOG(INFO) << __func__;
        mEventCond.wait(lock);
        LOG(INFO) << __func__<< " EXIT";
    }
private:
    std::mutex mLock;
    std::condition_variable mEventCond;
    std::vector<HidlVecOfValues> mReceivedEvents;
};

class VehicleController {
  public:
    VehicleController();
    VehicleController(LockMap<int32_t, TboxDataDeclaration>& tboxdecl);
    void Start();
    void Start(LockMap<int32_t, TboxDataDeclaration>& tboxdecl);

    void subscribeToProperties(const std::vector<int>& properties);
    void subscribeToProperty(int propID);

    void unsubscribeFromProperties(const std::vector<int>& properties);

    VehiclePropValue getProperty(int propId);

    void setProperty(const VehiclePropValue& value);

    std::string getPropertybyPropId(int propId);
    void getPropertybyPropId(int propId,int areaId,VehiclePropValue& refValue,StatusCode& refStatus);

    std::string left_to_right(std::string& value_type, VehiclePropValue& refvalue);

    int getRightValueByPropId(int propId);

    int32_t getareaIdByPropId(int propId);

  public:
    static std::vector<Item> mItemList;
    sp<IVehicle> mVehicle;
    sp<VehicleCallback> mCallback;
};

#endif  // VEHICLE_CONTROLLER_H