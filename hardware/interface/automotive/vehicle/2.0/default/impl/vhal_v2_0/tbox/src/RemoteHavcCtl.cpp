#include "RemoteHavcCtl.h"
#include <android-base/properties.h>
#define PROPERTY_VALUE_MAX_LEN 250

RemoteHavcCtl::RemoteHavcCtl(VehicleController& vehicle) {
    mVehicle = vehicle;
}
void RemoteHavcCtl::setRemoteHavcCtl(HavcCtlMessage &havcCtl){
    mHavcCtl = havcCtl;
}
void RemoteHavcCtl::setHavcWork() {
    //std::cout << __func__ << std::endl;
    LOG(INFO) << "remoteHVACControl" << __func__;
    std::unique_lock<std::mutex> lock(mutex);
    cv.notify_one();
}
void RemoteHavcCtl::getHaveWorkState() {
    //std::cout << __func__ << std::endl;
    LOG(INFO) << "remoteHVACControl" << __func__;
    std::unique_lock<std::mutex> lock(mutex);
    cv.wait(lock);
}

int RemoteHavcCtl::PreCondition() {
    //std::cout << __func__ << std::endl;
    LOG(INFO) << "remoteHVACControl" << __func__;
    StatusCode refStatus;
    VehiclePropValue refValue;

    mVehicle.getPropertybyPropId((int)VehicleProperty::GECKO_BCM_DOORLOCKSTA_FL, 0, refValue, refStatus);
    int driveLockSt = refValue.value.int32Values[0];

    mVehicle.getPropertybyPropId((int)VehicleProperty::GECKO_BCM_BODYWARNSTS, 0, refValue, refStatus);
    int vehAlrmSysSt = refValue.value.int32Values[0];

    mVehicle.getPropertybyPropId((int)VehicleProperty::EV_BATTERY_LEVEL, 0, refValue, refStatus);
    float batteryLevel = refValue.value.floatValues[0];

    //诊断模式判断
    //0：OBD Tester offline  1：OBD Tester online
    mVehicle.getPropertybyPropId((int)VehicleProperty::GECKO_CCU_DIAGTESTERONLINE, 0, refValue, refStatus);
    int diagMode = refValue.value.int32Values[0];

    //OTA模式判断
    char propValue[PROPERTY_VALUE_MAX_LEN] = "\0";
    std::string otaMode = "0";
    if(property_get("gecko.mcu.updateing", propValue, nullptr)!=0){
        otaMode = propValue;
    }
    //mVehicle.getPropertybyPropId((int)VehicleProperty::GECKO_OTA_MODEL, 0, refValue, refStatus);
    //int ccmOtaMode = refValue.value.int32Values[0];
    int ccmOtaMode = 0;
   /* if(otaMode!="0"){
        return -1;
    }*/
    //电池CCM电压判断
    mVehicle.getPropertybyPropId((int)VehicleProperty::GECKO_SIBS_KL30_VOLTAGE, 0, refValue, refStatus);
    float ccmVol = refValue.value.int32Values[0];
    //BCM_SysPowerSts
    int bcmSysPowerSt = getBCMPowerSt();

    LOG(INFO) << "remoteHVACControl "
              << "driveLockSt = " << driveLockSt
              << ", vehAlrmSysSt = " << vehAlrmSysSt
              << ", batteryLevel = " << batteryLevel
              << ", diagnosis = " << diagMode
              << ", otaMode = " << otaMode
              << ", ccmOtaMode = " << ccmOtaMode
              << ", ccmVol = " << ccmVol
              << ", bcmSysPowerSt = " << bcmSysPowerSt;
    if ((driveLockSt!=1) || (vehAlrmSysSt!=1) || (batteryLevel<20) || (diagMode== 1) || (otaMode!="0") || (ccmOtaMode==1) || (ccmVol < 95) || (bcmSysPowerSt!=0x00 && bcmSysPowerSt!=0x02)){
        LOG(INFO) << "remoteHVACControl " <<"PreCondition match fail, return -1;";
        return -1;
    }
    return 0;
}
void RemoteHavcCtl::WakeUpVehicle() {

}

/*
   @param wakeup  1:wakeup 有工作需求, 0:no need wakeup无工作需求
 */
void RemoteHavcCtl::vehicleKeepAwake(int wakeup) {
    //0x0:No Node hasn’t woke up network(Passvie wakeup); 0x1:Node has woke up network(Active wakeup)
    VehiclePropValue strWorkReq;
    strWorkReq.prop = static_cast<int>(VehicleProperty::GECKO_CDC_ACTIVEWAKEUP);
    strWorkReq.areaId = 0;
    strWorkReq.value.int32Values.resize(1);
    strWorkReq.value.int32Values[0] = wakeup;
    //0x0:No Keep Awake Reasons; 0x1:Keep Awake Reasons Occurred
    VehiclePropValue keepAwakeSt;
    keepAwakeSt.prop = static_cast<int>(VehicleProperty::GECKO_CDC_KEEPAWAKESTA);
    keepAwakeSt.areaId = 0;
    keepAwakeSt.value.int32Values.resize(1);
    keepAwakeSt.value.int32Values[0] = wakeup;
    //0x0:No Module Remote Req; 0x1:Module Remote Req
    VehiclePropValue keepAwakeReasRemReq;
    keepAwakeReasRemReq.prop = static_cast<int>(VehicleProperty::GECKO_CDC_KEEPAWAKEREASREMREQ);
    keepAwakeReasRemReq.areaId = 0;
    keepAwakeReasRemReq.value.int32Values.resize(1);
    keepAwakeReasRemReq.value.int32Values[0] = wakeup;
    if(wakeup){
        mutexWakeUpRef.lock();
        wakeUpRef++;
        mutexWakeUpRef.unlock();
        if(wakeUpRef == 1){
            mVehicle.setProperty(strWorkReq);
            mVehicle.setProperty(keepAwakeSt);
            mVehicle.setProperty(keepAwakeReasRemReq);
            LOG(INFO) << "vehicleKeepAwake setSTRWorkReq req: 1";
        }
    } else {
        if(wakeUpRef > 0){
            mutexWakeUpRef.lock();
            wakeUpRef--;
            mutexWakeUpRef.unlock();
        }
        if(wakeUpRef == 0){
            mVehicle.setProperty(strWorkReq);
            mVehicle.setProperty(keepAwakeSt);
            mVehicle.setProperty(keepAwakeReasRemReq);
            LOG(INFO) << "vehicleKeepAwake setSTRWorkReq req: 0";
        }
    }
    LOG(INFO) << "vehicleKeepAwake setSTRWorkReq req: " << wakeup << ", wakeUpRef :" << wakeUpRef;
}

void RemoteHavcCtl::setSettingsToAC(){
    LOG(INFO) << "remoteHVACControl " << __func__;
    VehiclePropValue temperature_adjustment;
    VehiclePropValue air_conditioning_switch;
    air_conditioning_switch.prop = (int)VehicleProperty::GECKO_CCM_REMT_CTR_FLAG;
    air_conditioning_switch.areaId = 0;
    if (mHavcCtl.air_conditioning_switch != 0xff
    && mHavcCtl.defrosting_switch == 0xff){//开关空调
        //set default Temperature when open AC
        //donot close AC here, to avoid duplicated close AC
        if(mHavcCtl.air_conditioning_switch == 0x01){
            air_conditioning_switch.value.int32Values.resize(1);
            air_conditioning_switch.value.int32Values[0] = mHavcCtl.air_conditioning_switch;
            mVehicle.setProperty(air_conditioning_switch);
            LOG(INFO) << "remoteHVACControl " << __func__ << "set HVAC_POWER_ON " << air_conditioning_switch.value.int32Values[0];
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
            temperature_adjustment.prop = (int)VehicleProperty::HVAC_TEMPERATURE_SET;
            temperature_adjustment.areaId = 49;
            temperature_adjustment.value.floatValues.resize(1);
            if(mHavcCtl.temperature_adjustment == 0xff){
                temperature_adjustment.value.floatValues[0] = static_cast<float>(17 + 100);
                LOG(INFO) << "remoteHVACControl " << __func__ << "set default HVAC_TEMPERATURE_SET";
            } else {
                temperature_adjustment.value.floatValues[0] = static_cast<float>(mHavcCtl.temperature_adjustment + 69);
            }
            LOG(INFO) << "remoteHVACControl " << __func__ << "set HVAC_TEMPERATURE_SET = " << temperature_adjustment.value.floatValues[0];
            mVehicle.setProperty(temperature_adjustment);
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
        }
    }
    if(mHavcCtl.defrosting_switch != 0xff){
        // defrosting_switch;除霜开关
        if(mHavcCtl.defrosting_switch == 0x01){
            air_conditioning_switch.value.int32Values.resize(1);
            air_conditioning_switch.value.int32Values[0] = 1;
            mVehicle.setProperty(air_conditioning_switch);
            LOG(INFO) << "remoteHVACControl " << __func__ << "set HVAC_POWER_ON for defrost" << air_conditioning_switch.value.int32Values[0];
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
        }
        VehiclePropValue defrosting_switch;
        defrosting_switch.prop = (int)VehicleProperty::GECKO_AC_REMOTEDFROSTCTLFB;
        defrosting_switch.areaId = 0;
        defrosting_switch.value.int32Values.resize(1);
        defrosting_switch.value.int32Values[0] = mHavcCtl.defrosting_switch + 1;
        mVehicle.setProperty(defrosting_switch);
        LOG(INFO) << "remoteHVACControl " << __func__ << "set GECKO_AC_REMOTEDFROSTCTLFB = " << defrosting_switch.value.int32Values[0];
        if(mHavcCtl.defrosting_switch == 0x01 && mHavcCtl.temperature_adjustment != 0xff){
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
            temperature_adjustment.prop = (int)VehicleProperty::HVAC_TEMPERATURE_SET;
            temperature_adjustment.areaId = 49;
            temperature_adjustment.value.floatValues.resize(1);
            temperature_adjustment.value.floatValues[0] = static_cast<float>(mHavcCtl.temperature_adjustment + 69);
            LOG(INFO) << "remoteHVACControl " << __func__ << "set Defrosting HVAC_TEMPERATURE_SET = " << temperature_adjustment.value.floatValues[0];
            mVehicle.setProperty(temperature_adjustment);
        }
    }
}

void RemoteHavcCtl::HavcThread() {
    LOG(INFO) << "remoteHVACControl " << "HavcThread()";
    StatusCode refStatus;
    VehiclePropValue refValue;
    VehiclePropValue tbox_remt_power_ctl;
    tbox_remt_power_ctl.prop = (int)VehicleProperty::GECKO_CCU_REMTPOWERCTRLFB;
    tbox_remt_power_ctl.areaId = 0;
    tbox_remt_power_ctl.value.int32Values.resize(1);
    int bcmSysPowerSt = getBCMPowerSt();
    if(bcmSysPowerSt == 0x00){
        //上高压
        tbox_remt_power_ctl.value.int32Values[0] = 0x02;
        mVehicle.setProperty(tbox_remt_power_ctl);
        mRemotePowerCtrlReq = 0x02;
        LOG(INFO) << "remoteHVACControl " << "TBOX_RemtPowerCtrlReq up 0x02";
        //6s内读上高压结果
        int result = 0;
        for(auto i = 0; i<60; i++){
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            mVehicle.getPropertybyPropId((int)VehicleProperty::GECKO_VCU_PT_ST, 0, refValue, refStatus);
            result = refValue.value.int32Values[0];
            if(result == 0x01){
                LOG(INFO) << "remoteHVACControl " << "TBOX_RemtPowerCtrlReq success";
                break;
            }
        }
        if(result != 0x01){
            LOG(INFO) << "remoteHVACControl " << "TBOX_RemtPowerCtrlReq fail";
            setHavcWork();
            mThreads.pop_back();
            LOG(INFO) << "remoteHVACControl " << "HavcThread exit";
            return;//fail
        }
    }
    setSettingsToAC();
    //waiting for AC following
    std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    // working_time;
    uint32_t counter = mHavcCtl.working_time * 60 * 2;
    //发送空调响应状态给服务端
    setHavcWork();
    //启动定时器
    do{
        if(mNeedRefresh == 1){
            setSettingsToAC();
            //waiting for AC following
            std::this_thread::sleep_for(std::chrono::milliseconds(1500));
            // working_time;
            if(mHavcCtl.working_time != 0xff){
                counter = mHavcCtl.working_time * 60 * 2;
            }
            //发送空调响应状态给服务端
            if(mHavcCtl.air_conditioning_switch != 0x00){
                setHavcWork();
            }
            mutexCmdRefresh.lock();
            mNeedRefresh = 0;
            mutexCmdRefresh.unlock();
            //release keep awake flag
            vehicleKeepAwake(0);
        }
        mVehicle.getPropertybyPropId((int)VehicleProperty::GECKO_BCM_DOORLOCKSTA_FL, 0, refValue, refStatus);
        int drivelock = refValue.value.int32Values[0];
        mVehicle.getPropertybyPropId((int)VehicleProperty::GECKO_FL_DOOR_STATE, 0, refValue, refStatus);
        int fldoorajar = refValue.value.int32Values[0];
        mVehicle.getPropertybyPropId((int)VehicleProperty::GECKO_FR_DOOR_STATE, 0, refValue, refStatus);
        int frdoorajar = refValue.value.int32Values[0];
        if((drivelock==0)&&((fldoorajar==1)||(frdoorajar==1))){
            LOG(INFO) << "remoteHVACControl " << "drivelock=0, fldoorajar="<< fldoorajar << ", frdoorajar=" << frdoorajar <<", need to close the AC!";
            break;
        }
        mVehicle.getPropertybyPropId((int)VehicleProperty::EV_BATTERY_LEVEL, 0, refValue, refStatus);
        float batteryLevel = refValue.value.floatValues[0];
        if(batteryLevel<20){
            LOG(INFO) << "remoteHVACControl " << "batteryLevel="<<batteryLevel<<", need to close the AC!";
            break;
        }
        if(mRemoteHavcReq == 0){
            LOG(INFO) << "remoteHVACControl " << "receive remote CMD, need to close the AC!";
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        counter--;
    }while(counter!=0);
    LOG(INFO) << "remoteHVACControl " << "close the AC!";
    mutexCmdRefresh.lock();
    mNeedRefresh = 0;
    mutexCmdRefresh.unlock();
    VehiclePropValue air_conditioning_switch;
    air_conditioning_switch.prop = (int)VehicleProperty::GECKO_CCM_REMT_CTR_FLAG;
    air_conditioning_switch.areaId = 0;
    air_conditioning_switch.value.int32Values.resize(1);
    air_conditioning_switch.value.int32Values[0] = 0x00;
    mVehicle.setProperty(air_conditioning_switch);
    setHavcWork();
    LOG(INFO) << "remoteHVACControl " << __func__ << " close AC set HVAC_POWER_ON at the end:" << air_conditioning_switch.value.int32Values[0];
    //下高压
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    int mainDriveSeatSt = getMaiDrvrSeatSt();
    int steerWhlHeatSt = getSteerWhlHeatSts();
    int ACPower = getACPowerSt();
    LOG(INFO) << "remoteHVACControl " <<__func__ << " start to set HV: mainDriveSeatSt = " << mainDriveSeatSt << ", steerWhlHeatSt = " << steerWhlHeatSt << ", ACPower = " << ACPower;
    if(mainDriveSeatSt == 0x00 && steerWhlHeatSt == 0x00 && ACPower == 0x00){
        tbox_remt_power_ctl.value.int32Values[0] = 0x01;
        mVehicle.setProperty(tbox_remt_power_ctl);
        mRemotePowerCtrlReq = 0x01;
        LOG(INFO) << "remoteHVACControl " << "TBOX_RemtPowerCtrlReq down 0x01";
    } else {
        LOG(INFO) << "remoteHVACControl " << "TBOX_RemtPowerCtrlReq do not down";
    }
    //release keep awake flag
    vehicleKeepAwake(0);
    mThreads.pop_back();
    LOG(INFO) << "remoteHVACControl " << "HavcThread exit";
}

int RemoteHavcCtl::RemoteHavcOpen() {
    LOG(INFO) << "remoteHVACControl " << __func__ ;
    //唤醒整车
    WakeUpVehicle();
    //判断前置条件
    if(PreCondition()==0){
        //条件满足启动空调执行线程
        LOG(INFO) << "remoteHVACControl " << "PreCondition()==0";
        mRemoteHavcReq = 1;
        if(mThreads.size() == 0){
            LOG(INFO) << "remoteHVACControl " << "mThreads.size() == 0";
            if(mHavcCtl.air_conditioning_switch == 0x01 && mHavcCtl.defrosting_switch == 0x00){
                LOG(INFO) << "remoteHVACControl " << "close defrosting switch when the AC is OFF, return";
                return 2;
            }
            mThreads.emplace_back(std::bind(&RemoteHavcCtl::HavcThread, this));
            for (auto& thread : mThreads) {
                thread.detach();
            }
        } else {
            LOG(INFO) << "remoteHVACControl " << "mThreads.size() != 0, need refresh cmd";
            mutexCmdRefresh.lock();
            mNeedRefresh = 1;
            mutexCmdRefresh.unlock();
        }
    }else{
        //条件不满足返回结果
        LOG(INFO) << "remoteHVACControl " << "PreCondition()!=0";
        return 1;
    }
    return 0;
}

int RemoteHavcCtl::RemoteHavcClose() {
    //std::cout << __func__ << std::endl;
    LOG(INFO) << "remoteHVACControl " << __func__;
    mRemoteHavcReq = 0;
    if(mThreads.size() == 0){
        LOG(INFO) << "remoteHVACControl " << "no open cmd exacuating, return directly";
        return 1;
    }
    LOG(INFO) << "remoteHVACControl " << "mThreads.size() != 0, need refresh cmd, close the AC";
    getHaveWorkState();
    return 0;
    /*if(PreCondition()==0){
        mRemoteHavcReq = 0;
        if(mThreads.size() == 0){
            LOG(INFO) << "remoteHVACControl " << "no open cmd exacuating, return directly";
            return 1;
        } else {
            LOG(INFO) << "remoteHVACControl " << "mThreads.size() != 0, need refresh cmd, close the AC";
            mutexCmdRefresh.lock();
            mNeedRefresh = 1;
            mutexCmdRefresh.unlock();
            getHaveWorkState();
        }
        return 0;
    } else {
        //条件不满足返回结果
        LOG(INFO) << "remoteHVACControl " << "close AC PreCondition()!=0";
        return 0;
    }*/
}
int RemoteHavcCtl::getBCMPowerSt(){
    StatusCode refStatus;
    VehiclePropValue refValue;
    mVehicle.getPropertybyPropId((int)VehicleProperty::GECKO_BCM_SYSPOWERSTS, 0, refValue, refStatus);
    int powerSt = refValue.value.int32Values[0];
    LOG(INFO) <<  "remoteHVACControl " <<__func__ << powerSt;
    return powerSt;
}
int RemoteHavcCtl::getMaiDrvrSeatSt(){
    StatusCode refStatus;
    VehiclePropValue refValue;
    mVehicle.getPropertybyPropId((int)VehicleProperty::GECKO_BCM_MAIDRVARSEATSTS, 0, refValue, refStatus);
    int seatSt = refValue.value.int32Values[0];
    LOG(INFO) <<  "remoteHVACControl " <<__func__ << seatSt;
    return seatSt;
}
int RemoteHavcCtl::getSteerWhlHeatSts(){
    StatusCode refStatus;
    VehiclePropValue refValue;
    mVehicle.getPropertybyPropId((int)VehicleProperty::GECKO_BCM_STEERWHLHEATSTS, 0, refValue, refStatus);
    int steerWhilHeatSt = refValue.value.int32Values[0];
    LOG(INFO) <<  "remoteHVACControl " <<__func__ << steerWhilHeatSt;
    return steerWhilHeatSt;
}
int RemoteHavcCtl::getACPowerSt(){
    StatusCode refStatus;
    VehiclePropValue refValue;
    mVehicle.getPropertybyPropId((int)VehicleProperty::HVAC_POWER_ON, 117, refValue, refStatus);
    int ACPowerSt = refValue.value.int32Values[0];
    LOG(INFO) << "remoteHVACControl " <<__func__ << ACPowerSt;
    return ACPowerSt;
}
