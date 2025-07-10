#include "RemoteSteerWheelHeatCtl.h"

RemoteSteerWheelHeatCtl::RemoteSteerWheelHeatCtl(VehicleController& vehicle) {
    mVehicle = vehicle;
}
void RemoteSteerWheelHeatCtl::setRemoteSteerWheelHeatCtl(uint8_t steerWheelHeatCtl){
    mSteerWheelHeatCtl = (int)steerWheelHeatCtl;
}

void RemoteSteerWheelHeatCtl::notifyState() {
    //std::cout << __func__ << std::endl;
    LOG(INFO) << "RemoteSteerWheelHeatCtl" << __func__;
    std::unique_lock<std::mutex> lock(mutex);
    cv.notify_one();
}
void RemoteSteerWheelHeatCtl::waitingForState() {
    //std::cout << __func__ << std::endl;
    LOG(INFO) << "RemoteSteerWheelHeatCtl" << __func__;
    std::unique_lock<std::mutex> lock(mutex);
    cv.wait(lock);
}
int RemoteSteerWheelHeatCtl::getBCMPowerSt(){
    StatusCode refStatus;
    VehiclePropValue refValue;
    mVehicle.getPropertybyPropId((int)VehicleProperty::GECKO_BCM_SYSPOWERSTS, 0, refValue, refStatus);
    int powerSt = refValue.value.int32Values[0];
    LOG(INFO) << "RemoteSteerWheelHeatCtl" << __func__ << powerSt;
    return powerSt;
}
int RemoteSteerWheelHeatCtl::getMaiDrvrSeatSt(){
    StatusCode refStatus;
    VehiclePropValue refValue;
    mVehicle.getPropertybyPropId((int)VehicleProperty::GECKO_BCM_MAIDRVARSEATSTS, 0, refValue, refStatus);
    int seatSt = refValue.value.int32Values[0];
    LOG(INFO) << "RemoteSteerWheelHeatCtl" << __func__ << seatSt;
    return seatSt;
}
int RemoteSteerWheelHeatCtl::getSteerWhlHeatSts(){
    StatusCode refStatus;
    VehiclePropValue refValue;
    mVehicle.getPropertybyPropId((int)VehicleProperty::GECKO_BCM_STEERWHLHEATSTS, 0, refValue, refStatus);
    int steerWhilHeatSt = refValue.value.int32Values[0];
    LOG(INFO) << "RemoteSteerWheelHeatCtl" << __func__ << steerWhilHeatSt;
    return steerWhilHeatSt;
}
int RemoteSteerWheelHeatCtl::getACPowerFb(){
    StatusCode refStatus;
    VehiclePropValue refValue;
    mVehicle.getPropertybyPropId((int)VehicleProperty::HVAC_POWER_ON, 117, refValue, refStatus);
    int ACPowerSt = refValue.value.int32Values[0];
    LOG(INFO) << "RemoteSteerWheelHeatCtl" << __func__ << ACPowerSt;
    return ACPowerSt;
}

void RemoteSteerWheelHeatCtl::steerWheelHeatCMDThread(){
    LOG(INFO) <<  "RemoteSteerWheelHeatCtl " <<__func__ << " start";
    int steerWheelHeatSwitch = mSteerWheelHeatCtl;
    VehiclePropValue tbox_remt_steer_wheel_heat_req;
    tbox_remt_steer_wheel_heat_req.prop = (int)VehicleProperty::GECKO_CCU_REMTSTEERWHLHEATFB;
    tbox_remt_steer_wheel_heat_req.areaId = 0;
    tbox_remt_steer_wheel_heat_req.value.int32Values.resize(1);
    VehiclePropValue tbox_remt_power_ctl;
    tbox_remt_power_ctl.prop = (int)VehicleProperty::GECKO_CCU_REMTPOWERCTRLFB;
    tbox_remt_power_ctl.areaId = 0;
    tbox_remt_power_ctl.value.int32Values.resize(1);
    if(steerWheelHeatSwitch == 0x00){
        //steer wheel heat off
        LOG(INFO) <<  "RemoteSteerWheelHeatCtl " <<__func__ << " start to off the steer wheel heat";
        tbox_remt_steer_wheel_heat_req.value.int32Values[0] = 0x02;
        mVehicle.setProperty(tbox_remt_steer_wheel_heat_req);
        notifyState();
        mSteerWheelHeatReq = 0x02;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        //BCM_MaiDrvrSeatSts=0x0&&BCM_SteerWhlHeatSts=0x0&&AC_ACPowerFb=0x0 下高压
        int mainDriveSeatSt = getMaiDrvrSeatSt();
        int steerWhlHeatSt = getSteerWhlHeatSts();
        int ACPower = getACPowerFb();
        LOG(INFO) <<  "RemoteSteerWheelHeatCtl " <<__func__ << " start to off the steer wheel heat " << "mainDriveSeatSt:" << mainDriveSeatSt << ", steerWhlHeatSt" << steerWhlHeatSt << ", ACPower" << ACPower;
        if(mainDriveSeatSt == 0x00 && steerWhlHeatSt == 0x00 && ACPower == 0x00){
            //下高压
            tbox_remt_power_ctl.value.int32Values[0] = 0x01;
            mVehicle.setProperty(tbox_remt_power_ctl);
            mRemotePowerCtrlReq = 0x01;
            LOG(INFO) <<  "RemoteSteerWheelHeatCtl " <<__func__ << " set power ctl 1";
        }
    } else {
        int powerSt = static_cast<int>(getBCMPowerSt());
        LOG(INFO) <<  "RemoteSteerWheelHeatCtl " <<__func__ << " start to turn on the steer wheel heat: powerSt = " << powerSt;
        if(powerSt == 0x00 || powerSt == 0x02){
            if(powerSt == 0x00){
                //上高压
                int result = 0;
                tbox_remt_power_ctl.value.int32Values[0] = 0x02;
                mVehicle.setProperty(tbox_remt_power_ctl);
                mRemotePowerCtrlReq = 0x02;
                LOG(INFO) <<  "RemoteSteerWheelHeatCtl " <<__func__ << " set power ctl";
                StatusCode refStatus;
                VehiclePropValue refValue;
                for(auto i = 0; i<60; i++){
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                    mVehicle.getPropertybyPropId((int)VehicleProperty::GECKO_VCU_PT_ST, 0, refValue, refStatus);
                    result = refValue.value.int32Values[0];
                    if(result == 0x01){
                        LOG(INFO) << "RemoteSteerWheelHeatCtl " << "TBOX_RemtPowerCtrlReq success";
                        break;
                    }
                }
                if(result != 0x01){
                    LOG(INFO) << "RemoteSteerWheelHeatCtl " << "TBOX_RemtPowerCtrlReq fail";
                    notifyState();
                    mThreads.pop_back();
                    LOG(INFO) << "RemoteSteerWheelHeatCtl " << "seatHeatCMDThread exit";
                    return;//fail
                }
            }
            tbox_remt_steer_wheel_heat_req.value.int32Values[0] = 0x01;
            mVehicle.setProperty(tbox_remt_steer_wheel_heat_req);
            mSteerWheelHeatReq = 0x01;
            LOG(INFO) <<  "RemoteSteerWheelHeatCtl " <<__func__ << " set steer wheel heat " << steerWheelHeatSwitch;
        }
        notifyState();
    }
    mThreads.pop_back();
    LOG(INFO) << "RemoteSteerWheelHeatCtl steerWheelHeatCMDThread exit";
}

void RemoteSteerWheelHeatCtl::startRemoteSteerWheelHeatCtlCMD(){
    if(mThreads.size() == 0){
        LOG(INFO) << "RemoteSteerWheelHeatCtl mThreads.size() == 0";
        mThreads.emplace_back(std::bind(&RemoteSteerWheelHeatCtl::steerWheelHeatCMDThread, this));
        for (auto& thread : mThreads) {
            thread.detach();
        }
    } else {
        LOG(INFO) << "RemoteSteerWheelHeatCtl mThreads.size() != 0, need refresh cmd";
        mutexCmdRefresh.lock();
        //mNeedRefresh = 1;
        mutexCmdRefresh.unlock();
    }
}