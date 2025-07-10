#ifndef CHARGINGCTL_H
#define CHARGINGCTL_H

#include <condition_variable>
#include "GB32960MessageParser.h"
#include "vehicle_controller.h"
#include <android/hardware/automotive/vehicle/2.0/IVehicle.h>
using namespace android::hardware::automotive::vehicle::V2_0;

class ChargingCtl {
  public:
    ChargingCtl(VehicleController& vehicle);
    void setReservedChargingConfig(std::vector<uint8_t> reCharge);
    uint8_t checkReservedChargingConfigSetResult(std::vector<uint8_t> reCharge);
    std::vector<uint8_t> getReservedChargingConfig();
    std::vector<uint8_t> setSmartDCConfig(std::vector<uint8_t> powerRep);
    std::vector<uint8_t> getSmartDCConfig();

  public:
    VehicleController mVehicle;

  private:
    void waitingThread();
    void notify();
    void getNotification();

  private:
    std::vector<std::thread> mThreads;
    std::condition_variable cv;
    std::mutex mutex;
};
#endif