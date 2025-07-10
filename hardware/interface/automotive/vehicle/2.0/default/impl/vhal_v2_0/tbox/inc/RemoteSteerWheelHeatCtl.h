#ifndef REMOTESTEERWHEELHEATCTL_H
#define REMOTESTEERWHEELHEATCTL_H

#include <condition_variable>
#include <mutex>
#include "GB32960MessageParser.h"
#include "vehicle_controller.h"
#include <android/hardware/automotive/vehicle/2.0/IVehicle.h>
using namespace android::hardware::automotive::vehicle::V2_0;

class RemoteSteerWheelHeatCtl {
  public:
    RemoteSteerWheelHeatCtl(VehicleController& vehicle);
    void setRemoteSteerWheelHeatCtl(uint8_t steerWheelHeatCtl);
    void notifyState();
    void waitingForState();
    int getBCMPowerSt();
    int getMaiDrvrSeatSt();
    int getSteerWhlHeatSts();
    int getACPowerFb();
    void steerWheelHeatCMDThread();
    void startRemoteSteerWheelHeatCtlCMD();
  public:
    std::vector<std::thread> mThreads;
    VehicleController mVehicle;
    int mSteerWheelHeatCtl;
    uint8_t mRemotePowerCtrlReq = 0x00;
    uint8_t mSteerWheelHeatReq = 0x00;
  private:
    std::condition_variable cv;
    std::mutex mutex;
    std::mutex mutexCmdRefresh;
};
#endif