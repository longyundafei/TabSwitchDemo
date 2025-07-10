#ifndef REMOTEHAVCCTL_H
#define REMOTEHAVCCTL_H

#include <condition_variable>
#include <mutex>
#include "GB32960MessageParser.h"
#include "vehicle_controller.h"
#include <android/hardware/automotive/vehicle/2.0/IVehicle.h>
using namespace android::hardware::automotive::vehicle::V2_0;

class RemoteHavcCtl {
  public:
    RemoteHavcCtl(VehicleController& vehicle);
    int RemoteHavcOpen();
    int RemoteHavcClose();
    int PreCondition();
    void WakeUpVehicle();
    void vehicleKeepAwake(int wakeup);
    void HavcThread();
    void setRemoteHavcCtl(HavcCtlMessage &havcCtl);
    void setHavcWork();
    void getHaveWorkState();
    void setSettingsToAC();
    int getBCMPowerSt();
    int getMaiDrvrSeatSt();
    int getSteerWhlHeatSts();
    int getACPowerSt();
  public:
    std::vector<std::thread> mThreads;
    VehicleController mVehicle;
    HavcCtlMessage mHavcCtl;
    uint8_t mRemotePowerCtrlReq = 0x00;
    int mRemoteHavcReq = 0;//0, off; 1, on
    int wakeUpRef = 0;
    std::mutex mutexWakeUpRef;
  private:
    int mNeedRefresh = 0;
    std::condition_variable cv;
    std::mutex mutex;
    std::mutex mutexCmdRefresh;
};
#endif