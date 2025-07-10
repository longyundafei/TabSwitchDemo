#include "RemoteVersionSwitch.h"
#include <android-base/properties.h>

RemoteVersionSwitch::RemoteVersionSwitch(VehicleController& vehicle) {
    mVehicle = vehicle;
    mRequestVersion = 0x00;
}
void RemoteVersionSwitch::setVersionSwitchMsg(VersionSwitchMessage &msg){
    mVersionSwitchMsg = msg;
}

void RemoteVersionSwitch::notifyVCUWorkDone() {
    LOG(INFO) << "notifyVCUWorkDone";
    std::unique_lock<std::mutex> lock(mutex);
    cv.notify_one();
}
void RemoteVersionSwitch::waitingForNotify() {
    LOG(INFO) << "waitingFonNotify";
    std::unique_lock<std::mutex> lock(mutex);
    cv.wait(lock);
}

void RemoteVersionSwitch::getVCURespose() {
    LOG(INFO) << "getVCURespose";
    waitingForNotify();
    mThreads.pop_back();
}


void RemoteVersionSwitch::VersionSwitchThread(){
    VehiclePropValue ccm_version_requested;
    ccm_version_requested.prop = (int)VehicleProperty::GECKO_CCM_VCU_VERSION;
    ccm_version_requested.areaId = 0;
    ccm_version_requested.value.int32Values.resize(1);
    ccm_version_requested.value.int32Values[0] = mRequestVersion;
    LOG(INFO) << "set GECKO_CCM_VCU_VERSION = " << mRequestVersion;
    mVehicle.setProperty(ccm_version_requested);
    uint32_t counter = 10 * 2;
    LOG(INFO) << "waiting for 10s";
    do{
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        counter--;
		//mVehicle.getPropertybyPropId((int)VehicleProperty::GECKO_CCM_VERSION, 0, refValue, refStatus);
        //int ccmVersion = refValue.value.int32Values[0];
        if (mRequestVersion != ccm_version_requested.value.int32Values[0]){
            LOG(INFO) << "receive new version switch request, need to re-executive!";
            ccm_version_requested.prop = (int)VehicleProperty::GECKO_CCM_VCU_VERSION;
            ccm_version_requested.areaId = 0;
            ccm_version_requested.value.int32Values.resize(1);
            ccm_version_requested.value.int32Values[0] = mRequestVersion;
            mVehicle.setProperty(ccm_version_requested);
            counter = 10 * 2;
        }
        LOG(INFO) << "VersionSwitchThread counter = " << counter;
    }while(counter!=0);
    notifyVCUWorkDone();
}

int RemoteVersionSwitch::setVersion(int version){
    LOG(INFO) << __func__ << ": " <<std::hex << version;
    mRequestVersion = version;
    /*if(mThreads.size() == 0){
        LOG(INFO) << "mThreads.size() == 0, and create RemoteVersionSwitch::VersionSwitchThread;";
        mThreads.emplace_back(std::bind(&RemoteVersionSwitch::VersionSwitchThread, this));
        for (auto& thread : mThreads) {
            thread.detach();
        }
		return 1;
    }else{
        LOG(INFO) << "mThreads.size() != 0, and the new version request is " << std::hex << mRequestVersion << std::endl;
		return 0;
    }*/
    VehiclePropValue ccm_version_requested;
    ccm_version_requested.prop = (int)VehicleProperty::GECKO_CCM_VCU_VERSION;
    ccm_version_requested.areaId = 0;
    ccm_version_requested.value.int32Values.resize(1);
    ccm_version_requested.value.int32Values[0] = mRequestVersion;
    LOG(INFO) << "RemoteVersionSwitch set GECKO_CCM_VCU_VERSION = " << mRequestVersion;
    mVehicle.setProperty(ccm_version_requested);
    return 1;
}

int RemoteVersionSwitch::getVCUVersion(){
    StatusCode refStatus;
    VehiclePropValue refValue;
    //mVehicle.getPropertybyPropId((int)VehicleProperty::GECKO_MCU_CCM_VERSION, 0, refValue, refStatus);
    mVehicle.getPropertybyPropId((int)VehicleProperty::GECKO_VCU_VERSIONFB, 0, refValue, refStatus);
    return refValue.value.int32Values[0];
}

int RemoteVersionSwitch::checkVCUVersion(){
    /*StatusCode refStatus;
    VehiclePropValue refValue;
    mVehicle.getPropertybyPropId((int)VehicleProperty::GECKO_MCU_CCM_VERSION, 0, refValue, refStatus);
    int vcmVersion = refValue.value.int32Values[0];
    if(mRequestVersion == vcmVersion){
        return 0;
    } else {
        return 1;
    }*/
    StatusCode refStatus;
    VehiclePropValue refValue;
    int resp = 1;
    for(auto i = 0; i<15; i++){
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        mVehicle.getPropertybyPropId((int)VehicleProperty::GECKO_VCU_VERSIONSETFB, 0, refValue, refStatus);
        if(refValue.value.int32Values[0] == 0x01){
            resp = 0;
            LOG(INFO) << "RemoteVersionSwitch: got response from CCU_RemoteLockFb : " << static_cast<int>(resp);
            break;
        }
    }
    return resp;
}