#ifndef REMOTESEATHEATCTL_H
#define REMOTESEATHEATCTL_H

#include <condition_variable>
#include <mutex>
#include "GB32960MessageParser.h"
#include "vehicle_controller.h"
#include <android/hardware/automotive/vehicle/2.0/IVehicle.h>
using namespace android::hardware::automotive::vehicle::V2_0;

class RemoteSeatHeatCtl {
  public:
    RemoteSeatHeatCtl(VehicleController& vehicle);
    void setRemoteSeatHeatCtl(SeatHeatCtlMessage &havcCtl);
    void notifyState();
    void waitingForState();
    float getSOCBatteryLevel();
    int getBCMPowerSt();
    int getMaiDrvrSeatSt();
    int getSteerWhlHeatSts();
    int getACPowerFb();
    void seatHeatCMDThread();
    void startRemoteSeatHeatCtlCMD();
  public:
    std::vector<std::thread> mThreads;
    VehicleController mVehicle;
    SeatHeatCtlMessage mSeatHeatCtl;
    uint8_t mRemotePowerCtrlReq = 0x00;
    uint8_t mSeatHeatReq = 0x00;
  private:
    std::condition_variable cv;
    std::mutex mutex;
    std::mutex mutexCmdRefresh;
};
#endif