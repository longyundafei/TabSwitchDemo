#include "ChargingCtl.h"
#include <android-base/properties.h>

ChargingCtl::ChargingCtl(VehicleController& vehicle) {
    mVehicle = vehicle;
}

void ChargingCtl::notify() {
    LOG(INFO) << "ChargingCtl notify";
    std::unique_lock<std::mutex> lock(mutex);
    cv.notify_one();
}
void ChargingCtl::getNotification() {
    LOG(INFO) << "ChargingCtl getNotification";
    std::unique_lock<std::mutex> lock(mutex);
    cv.wait(lock);
}

void ChargingCtl::setReservedChargingConfig(std::vector<uint8_t> reCharge){
    LOG(INFO) << "ChargingCtl setReservedChargingConfig";

    VehiclePropValue reservationMode;
    reservationMode.prop = (int)VehicleProperty::GECKO_CCM_APPOINTMENT_MODE;
    reservationMode.areaId = 0;
    reservationMode.value.int32Values.resize(1);
    reservationMode.value.int32Values[0] = reCharge[1];
    LOG(INFO) << "ChargingCtl setReservedChargingConfig reservationMode : " << toString(reservationMode.value.int32Values[0]);
    mVehicle.setProperty(reservationMode);

    VehiclePropValue startHour;
    startHour.prop = (int)VehicleProperty::GECKO_CCM_CHRAGING_BEGIN_TIME_HOUR;
    startHour.areaId = 0;
    startHour.value.int32Values.resize(1);
    startHour.value.int32Values[0] = reCharge[2];
    LOG(INFO) << "ChargingCtl setReservedChargingConfig startHour : " << toString(startHour.value.int32Values[0]);
    mVehicle.setProperty(startHour);

    VehiclePropValue startMinute;
    startMinute.prop = (int)VehicleProperty::GECKO_CCM_CHRAGING_BEGIN_TIME_MINUTE;
    startMinute.areaId = 0;
    startMinute.value.int32Values.resize(1);
    startMinute.value.int32Values[0] = reCharge[3];
    LOG(INFO) << "ChargingCtl setReservedChargingConfig startMinute : " << toString(startMinute.value.int32Values[0]);
    mVehicle.setProperty(startMinute);

    VehiclePropValue stopHour;
    stopHour.prop = (int)VehicleProperty::GECKO_CCM_CHRAGING_END_TIME_HOUR;
    stopHour.areaId = 0;
    stopHour.value.int32Values.resize(1);
    stopHour.value.int32Values[0] = reCharge[4];
    LOG(INFO) << "ChargingCtl setReservedChargingConfig stopHour : " << toString(stopHour.value.int32Values[0]);
    mVehicle.setProperty(stopHour);

    VehiclePropValue stopMinute;
    stopMinute.prop = (int)VehicleProperty::GECKO_CCM_CHRAGING_END_TIME_MINUTE;
    stopMinute.areaId = 0;
    stopMinute.value.int32Values.resize(1);
    stopMinute.value.int32Values[0] = reCharge[5];
    LOG(INFO) << "ChargingCtl setReservedChargingConfig stopMinute : " << toString(stopMinute.value.int32Values[0]);
    mVehicle.setProperty(stopMinute);

    //MCU格式为 周一、周二、周三、周四、周五、周六、周日
    VehiclePropValue loopEnable;
    loopEnable.prop = (int)VehicleProperty::GECKO_CCM_CHRAGING_LOOP;
    loopEnable.areaId = 0;
    loopEnable.value.int32Values.resize(7);
    int i;
    for(i=6; i<12; i++){
        loopEnable.value.int32Values[i-6] = reCharge[i+1];
        LOG(INFO) << "ChargingCtl setReservedChargingConfig loopEnable : " << toString(loopEnable.value.int32Values[i-6]);
    }
    loopEnable.value.int32Values[6] = reCharge[6];
    LOG(INFO) << "ChargingCtl setReservedChargingConfig loopEnable : " << toString(loopEnable.value.int32Values[6]);
    mVehicle.setProperty(loopEnable);
    LOG(INFO) << "ChargingCtl setReservedChargingConfig targetPower reCharge[13]: " << static_cast<int>(reCharge[13]);
    if(reCharge[13] != 0xFF){
        VehiclePropValue targetPower;
        targetPower.prop = (int)VehicleProperty::GECKO_CCM_CHRAGING_TARGET_SOC;
        targetPower.areaId = 0;
        targetPower.value.int32Values.resize(1);
        targetPower.value.int32Values[0] = reCharge[13];
        LOG(INFO) << "ChargingCtl setReservedChargingConfig targetPower : " << toString(targetPower.value.int32Values[0]);
        mVehicle.setProperty(targetPower);
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    VehiclePropValue chargeMode;
    chargeMode.prop = (int)VehicleProperty::GECKO_CHRAGINGMODESP;
    chargeMode.areaId = 0;
    chargeMode.value.int32Values.resize(1);
    chargeMode.value.int32Values[0] = reCharge[0];
    LOG(INFO) << "ChargingCtl setReservedChargingConfig chargeMode : " << toString(chargeMode.value.int32Values[0]);
    mVehicle.setProperty(chargeMode);
}

uint8_t ChargingCtl::checkReservedChargingConfigSetResult(std::vector<uint8_t> reCharge) {
    uint8_t ret = 0;
    StatusCode rS_ChargeMode;
    VehiclePropValue rV_ChargeMode;
    mVehicle.getPropertybyPropId((int)VehicleProperty::GECKO_CHRAGINGMODESP, 0, rV_ChargeMode, rS_ChargeMode);
    uint8_t value = static_cast<uint8_t>(rV_ChargeMode.value.int32Values[0]);
    LOG(INFO) << "ChargingCtl checkReservedChargingConfigSetResult get VCUChargeMode value = " << rV_ChargeMode.value.int32Values[0];
    uint8_t VCUChargeMode = 0xFF;
    switch(value){
        case 0x01:
        case 0x02:
        VCUChargeMode = value-1;
        break;
        case 0x03:
        VCUChargeMode = 1;
        break;
        default:
        VCUChargeMode = 0xFF;
        break;
    }
    if (VCUChargeMode != (reCharge[0])){
        LOG(INFO) << "ChargingCtl checkReservedChargingConfigSetResult VCUChargeMode = " << toString(VCUChargeMode) << ", reCharge[0] = " << toString(reCharge[0]);
        ret = 1;
    }
    /*StatusCode rS_TargetPower;
    VehiclePropValue rV_TargetPower;
    mVehicle.getPropertybyPropId((int)VehicleProperty::GECKO_CCM_CHRAGING_TARGET_SOC, 0, rV_TargetPower, rS_TargetPower);
    uint8_t targetPower = static_cast<uint8_t>(rV_TargetPower.value.int32Values[0]);
    if(targetPower != reCharge[13]){
        LOG(INFO) << "ChargingCtl checkReservedChargingConfigSetResult targetPower = " << toString(targetPower) << ", reCharge[13] = " << toString(reCharge[13]);
        ret = 1;
    }*/
    return ret;
}

std::vector<uint8_t> ChargingCtl::getReservedChargingConfig(){
    LOG(INFO) << "ChargingCtl getReservedChargingConfig";
    StatusCode rS_ChargeMode;
    VehiclePropValue rV_ChargeMode;
    mVehicle.getPropertybyPropId((int)VehicleProperty::GECKO_CHRAGINGMODESP, 0, rV_ChargeMode, rS_ChargeMode);
    uint8_t value = static_cast<uint8_t>(rV_ChargeMode.value.int32Values[0]);
    uint8_t chargeMode = 0xFF;
    switch(value){
        case 0x01:
        case 0x02:
        chargeMode = value-1;
        break;
        case 0x03:
        chargeMode = 1;
        break;
        default:
        chargeMode = 0xFF;
        break;
    }
    LOG(INFO) << "ChargingCtl getReservedChargingConfig chargeMode : " << toString(chargeMode);

    StatusCode rS_ReservedMode;
    VehiclePropValue rV_ReservedMode;
    mVehicle.getPropertybyPropId((int)VehicleProperty::GECKO_CCM_APPOINTMENT_MODE, 0, rV_ReservedMode, rS_ReservedMode);
    uint8_t reservedMode = static_cast<uint8_t>(rV_ReservedMode.value.int32Values[0]);
    LOG(INFO) << "ChargingCtl getReservedChargingConfig reservedMode : " << toString(reservedMode);

    StatusCode rS_StartHour;
    VehiclePropValue rV_StartHour;
    mVehicle.getPropertybyPropId((int)VehicleProperty::GECKO_CCM_CHRAGING_BEGIN_TIME_HOUR, 0, rV_StartHour, rS_StartHour);
    uint8_t startHour = static_cast<uint8_t>(rV_StartHour.value.int32Values[0]);
    LOG(INFO) << "ChargingCtl getReservedChargingConfig startHour : " << toString(startHour);

    StatusCode rS_StartMin;
    VehiclePropValue rV_StartMin;
    mVehicle.getPropertybyPropId((int)VehicleProperty::GECKO_CCM_CHRAGING_BEGIN_TIME_MINUTE, 0, rV_StartMin, rS_StartMin);
    uint8_t startMinute = static_cast<uint8_t>(rV_StartMin.value.int32Values[0]);
    LOG(INFO) << "ChargingCtl getReservedChargingConfig startMinute : " << toString(startMinute);

    StatusCode rS_StopHour;
    VehiclePropValue rV_StopHour;
    mVehicle.getPropertybyPropId((int)VehicleProperty::GECKO_CCM_CHRAGING_END_TIME_HOUR, 0, rV_StopHour, rS_StopHour);
    uint8_t stopHour = static_cast<uint8_t>(rV_StopHour.value.int32Values[0]);
    LOG(INFO) << "ChargingCtl getReservedChargingConfig stopHour : " << toString(stopHour);

    StatusCode rS_StopMin;
    VehiclePropValue rV_StopMin;
    mVehicle.getPropertybyPropId((int)VehicleProperty::GECKO_CCM_CHRAGING_END_TIME_MINUTE, 0, rV_StopMin, rS_StopMin);
    uint8_t stopMinute = static_cast<uint8_t>(rV_StopMin.value.int32Values[0]);
    LOG(INFO) << "ChargingCtl getReservedChargingConfig stopMinute : " << toString(stopMinute);

    StatusCode rS_LoopEnable;
    VehiclePropValue rV_LoopEnable;
    mVehicle.getPropertybyPropId((int)VehicleProperty::GECKO_CCM_CHRAGING_LOOP, 0, rV_LoopEnable, rS_LoopEnable);
    //MCU格式为 周一、周二、周三、周四、周五、周六、周日
    std::vector<uint8_t> loopEnable;
    loopEnable.push_back(static_cast<uint8_t>(rV_LoopEnable.value.int32Values[6]));
    LOG(INFO) << "ChargingCtl getReservedChargingConfig loopEnable : " << toString(rV_LoopEnable.value.int32Values[6]);
    int i;
    for (i = 0; i < 6; i++){
        loopEnable.push_back(static_cast<uint8_t>(rV_LoopEnable.value.int32Values[i]));
        LOG(INFO) << "ChargingCtl getReservedChargingConfig loopEnable : " << toString(rV_LoopEnable.value.int32Values[i]);
    }

    StatusCode rS_TargetPower;
    VehiclePropValue rV_TargetPower;
    mVehicle.getPropertybyPropId((int)VehicleProperty::GECKO_CCM_CHRAGING_TARGET_SOC, 0, rV_TargetPower, rS_TargetPower);
    uint8_t targetPower = static_cast<uint8_t>(rV_TargetPower.value.int32Values[0]);
    LOG(INFO) << "ChargingCtl getReservedChargingConfig targetPower : " << toString(targetPower);


    std::vector<uint8_t> powerRep;
    powerRep.push_back(chargeMode);
    powerRep.push_back(reservedMode);
    powerRep.push_back(startHour);
    powerRep.push_back(startMinute);
    powerRep.push_back(stopHour);
    powerRep.push_back(stopMinute);
    for(i=0; i<7; i++){
        powerRep.push_back(loopEnable[i]);
    }
    powerRep.push_back(targetPower);
    powerRep.push_back(0xFF);
    return powerRep;
}

/*查询智能补电配置*/
std::vector<uint8_t> ChargingCtl::getSmartDCConfig() {
    LOG(INFO) << "getSmartDCConfig";
    VehiclePropValue dcQuery;
    dcQuery.prop = (int)VehicleProperty::GECKO_SMART_DC_QUERY;
    dcQuery.areaId = 0;
    dcQuery.value.int32Values.resize(1);
    dcQuery.value.int32Values[0] = 1;
    mVehicle.setProperty(dcQuery);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    //StatusCode rS_time;
    VehiclePropValue rV_time;
    //mVehicle.getPropertybyPropId((int)VehicleProperty::GECKO_MCU_RECHARGE_TIME, 0, rV_time, rS_time);
    rV_time.value.int32Values[0] = 0;
    LOG(INFO) << "getSmartDCConfig " << "time" << toString(rV_time.value.int32Values[0]);
    //StatusCode rS_startVolt;
    VehiclePropValue rV_startVolt;
    //mVehicle.getPropertybyPropId((int)VehicleProperty::GECKO_MCU_RECHARGE_START_VOLT, 0, rV_startVolt, rS_startVolt);
    rV_startVolt.value.floatValues[0] = 0;
    LOG(INFO) << "getSmartDCConfig " << "startg volt" << toString(rV_startVolt.value.floatValues[0]);
    //StatusCode rS_endVolt;
    VehiclePropValue rV_endVolt;
    //mVehicle.getPropertybyPropId((int)VehicleProperty::GECKO_MCU_RECHARGE_END_VOLT, 0, rV_endVolt, rS_endVolt);
    rV_endVolt.value.floatValues[0] = 0;
    LOG(INFO) << "getSmartDCConfig " << "end volt" << toString(rV_endVolt.value.floatValues[0]);

    std::vector<uint8_t> powerRep;
    powerRep.push_back(static_cast<uint8_t>((rV_time.value.int32Values[0] >> 8) & 0xFF));
    powerRep.push_back(static_cast<uint8_t>(rV_time.value.int32Values[0] & 0xFF));
    powerRep.push_back(static_cast<uint8_t>(rV_startVolt.value.floatValues[0]*10));
    powerRep.push_back(static_cast<uint8_t>(rV_endVolt.value.floatValues[0]*10));
    powerRep.push_back(0xFF);
    return powerRep;
}

void ChargingCtl::waitingThread() {
    LOG(INFO) << "ChargingCtl waiting for 300ms";
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    notify();
}

/*下发智能补电配置*/
std::vector<uint8_t> ChargingCtl::setSmartDCConfig(std::vector<uint8_t> powerRep) {
    LOG(INFO) << "setSmartDCConfig";
    VehiclePropValue powerRepConfig;
    powerRepConfig.prop = (int)VehicleProperty::GECKO_VCU_SMART_DC_ST;
    powerRepConfig.areaId = 0;
    powerRepConfig.value.int32Values.resize(3);
    powerRepConfig.value.int32Values[0] = powerRep[2];
    LOG(INFO) << "setSmartDCConfig start vol:" << powerRepConfig.value.int32Values[0];
    powerRepConfig.value.int32Values[1] = powerRep[3];
    LOG(INFO) << "setSmartDCConfig end vol:" << powerRepConfig.value.int32Values[1];
    powerRepConfig.value.int32Values[2] = (powerRep[0]<<8) | powerRep[1];
    LOG(INFO) << "setSmartDCConfig time:" << powerRepConfig.value.int32Values[2];
    mVehicle.setProperty(powerRepConfig);
    mThreads.emplace_back(std::bind(&ChargingCtl::waitingThread, this));
    getNotification();
    return getSmartDCConfig();
}