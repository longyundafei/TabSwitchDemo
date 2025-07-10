#ifndef REMOTEVERSIONSWITCH_H
#define REMOTEVERSIONSWITCH_H

#include <condition_variable>
#include "GB32960MessageParser.h"
#include "vehicle_controller.h"
#include <android/hardware/automotive/vehicle/2.0/IVehicle.h>
using namespace android::hardware::automotive::vehicle::V2_0;

class RemoteVersionSwitch {
  public:
    RemoteVersionSwitch(VehicleController& vehicle);
    int setVersion(int version);
    void notifyVCUWorkDone();
    void getVCURespose();
    int checkVCUVersion();
    void setVersionSwitchMsg(VersionSwitchMessage &msg);
    void waitingForNotify();
    int getVCUVersion();

  public:
    std::vector<std::thread> mThreads;
    VehicleController mVehicle;
    VersionSwitchMessage mVersionSwitchMsg;
    int mRequestVersion;

  private:
    void VersionSwitchThread();

  private:
    std::condition_variable cv;
    std::mutex mutex;
};
#endif