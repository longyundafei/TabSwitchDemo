#include "RemoteSeatHeatCtl.h"

RemoteSeatHeatCtl::RemoteSeatHeatCtl(VehicleController& vehicle) {
    mVehicle = vehicle;
}

void RemoteSeatHeatCtl::setRemoteSeatHeatCtl(SeatHeatCtlMessage &seatHeatCtl){
    mSeatHeatCtl = seatHeatCtl;
}

void RemoteSeatHeatCtl::notifyState() {
    //std::cout << __func__ << std::endl;
    LOG(INFO) << "RemoteSeatHeatCtl " <<__func__;
    std::unique_lock<std::mutex> lock(mutex);
    cv.notify_one();
}
void RemoteSeatHeatCtl::waitingForState() {
    //std::cout << __func__ << std::endl;
    LOG(INFO) << "RemoteSeatHeatCtl " <<__func__;
    std::unique_lock<std::mutex> lock(mutex);
    cv.wait(lock);
}

float RemoteSeatHeatCtl::getSOCBatteryLevel(){
    StatusCode refStatus;
    VehiclePropValue refValue;
    mVehicle.getPropertybyPropId((int)VehicleProperty::EV_BATTERY_LEVEL, 0, refValue, refStatus);
    LOG(INFO) <<  "RemoteSeatHeatCtl " <<__func__ << refValue.value.floatValues[0];
    return refValue.value.floatValues[0];
}
int RemoteSeatHeatCtl::getBCMPowerSt(){
    StatusCode refStatus;
    VehiclePropValue refValue;
    mVehicle.getPropertybyPropId((int)VehicleProperty::GECKO_BCM_SYSPOWERSTS, 0, refValue, refStatus);
    int powerSt = refValue.value.int32Values[0];
    LOG(INFO) <<  "RemoteSeatHeatCtl " <<__func__ << powerSt;
    return powerSt;
}
int RemoteSeatHeatCtl::getMaiDrvrSeatSt(){
    StatusCode refStatus;
    VehiclePropValue refValue;
    mVehicle.getPropertybyPropId((int)VehicleProperty::GECKO_BCM_MAIDRVARSEATSTS, 0, refValue, refStatus);
    int seatSt = refValue.value.int32Values[0];
    LOG(INFO) <<  "RemoteSeatHeatCtl " <<__func__ << seatSt;
    return seatSt;
}
int RemoteSeatHeatCtl::getSteerWhlHeatSts(){
    StatusCode refStatus;
    VehiclePropValue refValue;
    mVehicle.getPropertybyPropId((int)VehicleProperty::GECKO_BCM_STEERWHLHEATSTS, 0, refValue, refStatus);
    int steerWhilHeatSt = refValue.value.int32Values[0];
    LOG(INFO) <<  "RemoteSeatHeatCtl " <<__func__ << steerWhilHeatSt;
    return steerWhilHeatSt;
}
int RemoteSeatHeatCtl::getACPowerFb(){
    StatusCode refStatus;
    VehiclePropValue refValue;
    mVehicle.getPropertybyPropId((int)VehicleProperty::HVAC_POWER_ON, 117, refValue, refStatus);
    int ACPowerSt = refValue.value.int32Values[0];
    LOG(INFO) <<  "RemoteSeatHeatCtl " <<__func__ << ACPowerSt;
    return ACPowerSt;
}
void RemoteSeatHeatCtl::seatHeatCMDThread(){
    LOG(INFO) <<  "RemoteSeatHeatCtl " <<__func__ << " start";
    int seatHeatSwitch = mSeatHeatCtl.seatHeatSwitch;
    int heatLevel = mSeatHeatCtl.heatLevel;
    VehiclePropValue tbox_remt_seat_heat_req;
    tbox_remt_seat_heat_req.prop = (int)VehicleProperty::GECKO_CCU_REMTMAIDRSEATHEATFB;
    tbox_remt_seat_heat_req.areaId = 0;
    tbox_remt_seat_heat_req.value.int32Values.resize(1);
    VehiclePropValue tbox_remt_power_ctl;
    tbox_remt_power_ctl.prop = (int)VehicleProperty::GECKO_CCU_REMTPOWERCTRLFB;
    tbox_remt_power_ctl.areaId = 0;
    tbox_remt_power_ctl.value.int32Values.resize(1);
    if(seatHeatSwitch == 0x00){
        //seat heat off
        LOG(INFO) <<  "RemoteSeatHeatCtl " <<__func__ << " start to off the seat heat";
        tbox_remt_seat_heat_req.value.int32Values[0] = 0x04;
        mVehicle.setProperty(tbox_remt_seat_heat_req);
        notifyState();
        mSeatHeatReq = 0x04;
        //BCM_MaiDrvrSeatSts=0x0&&BCM_SteerWhlHeatSts=0x0&&AC_ACPowerFb=0x0 下高压
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        int mainDriveSeatSt = getMaiDrvrSeatSt();
        int steerWhlHeatSt = getSteerWhlHeatSts();
        int ACPower = getACPowerFb();
        LOG(INFO) <<  "RemoteSeatHeatCtl " <<__func__ << " start to turn off the seat heat: mainDriveSeatSt = " << mainDriveSeatSt << ", steerWhlHeatSt = " << steerWhlHeatSt << ", ACPower = " << ACPower;
        if(mainDriveSeatSt == 0x00 && steerWhlHeatSt == 0x00 && ACPower == 0x00){
            //下高压
            mRemotePowerCtrlReq = 0x01;
            tbox_remt_power_ctl.value.int32Values[0] = 0x01;
            mVehicle.setProperty(tbox_remt_power_ctl);
            LOG(INFO) <<  "RemoteSeatHeatCtl " <<__func__ << " set power ctl 1";
        }
    } else {
        float batteryLevel = getSOCBatteryLevel();
        int powerSt = getBCMPowerSt();
        LOG(INFO) <<  "RemoteSeatHeatCtl " <<__func__ << " start to turn on the seat heat: battery level = " << batteryLevel << ", powerSt = " << powerSt;
        if(batteryLevel >= 20 && (powerSt == 0x00 || powerSt == 0x02)){
            if(powerSt == 0x00){
                //上高压
                int result = 0;
                tbox_remt_power_ctl.value.int32Values[0] = 0x02;
                mVehicle.setProperty(tbox_remt_power_ctl);
                mRemotePowerCtrlReq = 0x02;
                LOG(INFO) <<  "RemoteSeatHeatCtl " <<__func__ << " set power ctl 2";
                StatusCode refStatus;
                VehiclePropValue refValue;
                for(auto i = 0; i<60; i++){
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                    mVehicle.getPropertybyPropId((int)VehicleProperty::GECKO_VCU_PT_ST, 0, refValue, refStatus);
                    result = refValue.value.int32Values[0];
                    if(result == 0x01){
                        LOG(INFO) << "RemoteSeatHeatCtl " << "TBOX_RemtPowerCtrlReq Up success";
                        break;
                    }
                }
                if(result != 0x01){
                    LOG(INFO) << "RemoteSeatHeatCtl " << "TBOX_RemtPowerCtrlReq Up fail";
                    notifyState();
                    mThreads.pop_back();
                    LOG(INFO) << "RemoteSeatHeatCtl " << "seatHeatCMDThread exit";
                    return;//fail
                }
            }
            tbox_remt_seat_heat_req.value.int32Values[0] = heatLevel;
            mVehicle.setProperty(tbox_remt_seat_heat_req);
            mSeatHeatReq = heatLevel;
            LOG(INFO) <<  "RemoteSeatHeatCtl " <<__func__ << " set seat heat " << heatLevel;
        }
        notifyState();
    }
    mThreads.pop_back();
    LOG(INFO) << "RemoteSeatHeatCtl seatHeatCMDThread exit";
}

void RemoteSeatHeatCtl::startRemoteSeatHeatCtlCMD(){
        if(mThreads.size() == 0){
            LOG(INFO) <<  "RemoteSeatHeatCtl " <<__func__ <<"mThreads.size() == 0";
            mThreads.emplace_back(std::bind(&RemoteSeatHeatCtl::seatHeatCMDThread, this));
            for (auto& thread : mThreads) {
                thread.detach();
            }
        } else {
            LOG(INFO) << "RemoteSeatHeatCtl " <<__func__ <<"mThreads.size() != 0, need refresh cmd";
            mutexCmdRefresh.lock();
            //mNeedRefresh = 1;
            mutexCmdRefresh.unlock();
        }
}