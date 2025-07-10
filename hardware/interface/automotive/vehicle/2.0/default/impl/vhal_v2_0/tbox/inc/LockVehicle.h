#ifndef LOCKVEHICLE_H
#define LOCKVEHICLE_H
#include <condition_variable>
#include "GB32960MessageParser.h"
#include "vehicle_controller.h"
#include <android/hardware/automotive/vehicle/2.0/IVehicle.h>
using namespace android::hardware::automotive::vehicle::V2_0;

class LockVehicle {
  public:
    LockVehicle(VehicleController& vehicle);
    uint8_t setSpeedLimitLevel(uint8_t chargingConditions, uint8_t level);
    void setLockVehicleMsg(LockVehicleMessage &msg);

  public:
    std::vector<std::thread> mThreads;
    VehicleController mVehicle;
	  int mSpeedLimitLevel;
    LockVehicleMessage mLockVehicleMsg;

  private:

};
#endif