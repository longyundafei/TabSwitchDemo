#include "FaultDataMessage.h"

int FaultDataMessage::getmaxAlarmLevelValue() {
    int pcuFailSt = getValue(toInt(VehicleProperty::GECKO_VCU_PCU_FAIL_STA));
    if (pcuFailSt == 0x01){
        return getValue(toInt(VehicleProperty::GECKO_VCU_SYSFAILSTA));
    } else {
        return 0x00;
    }
}
int FaultDataMessage::gettemperatureDifferenceAlarmValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_DELTATEMP));
}
int FaultDataMessage::getbatteryHighTemperatureAlarmValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_OVERTEMP));
}
int FaultDataMessage::getvehicleEnergyStorageDeviceOvervoltageAlarmValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_PACKOVERVOLT));
}
int FaultDataMessage::getvehicleEnergyStorageDeviceUndervoltageAlarmValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_PACKUDVOLT));
}
int FaultDataMessage::getsocLowAlarmValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_PACKLOWSOC));
}
int FaultDataMessage::getsingleCellBatteryOvervoltageAlarmValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_CELLOVERVOLT));
}
int FaultDataMessage::getsingleCellBatteryUndervoltageAlarmValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_CELLUDVOLT));
}
int FaultDataMessage::getsocHighAlarmValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_PACKHIGHSOC));
}
int FaultDataMessage::getsocJumpAlarmValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_PACKFASTCHGSOC));
}
int FaultDataMessage::getrechargeableEnergyStorageSystemMismatchedAlarmValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_BATSYSNOTMTC));
}
int FaultDataMessage::getbatterySingleConsistencyPoorAlarmValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_UCELLINCON));
}
int FaultDataMessage::getinsulationAlarmValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_INSULATION_WRNING));
}
int FaultDataMessage::getdcDcTemperatureAlarmValue() {
    return getValue(toInt(VehicleProperty::GECKO_IPU_OVER_TEMPERATURE_FAIL));
}
int FaultDataMessage::getbrakeSystemAlarmValue() {
    //return getValue(toInt(VehicleProperty::GECKO_VCU_BRAKSYSWARN));
    int failureSts = getValue(toInt(VehicleProperty::GECKO_EHB_FAILURESTS));
    if (failureSts == 0x1){
        return 1;
    }
    int epbFlSts = getValue(toInt(VehicleProperty::GECKO_EHB_EPB_FLTSTS));
    if (epbFlSts == 0x1 || epbFlSts == 0x2){
        return 1;
    }
    int esCorTcsFailed = getValue(toInt(VehicleProperty::GECKO_ESC_ES_COR_TCS_FAILED));
    if (esCorTcsFailed == 0x1){
        return 1;
    }
    return 0;
}
int FaultDataMessage::getldcDcStatusAlarmValue() {
    int value = 0;
    int val = 0;
    value =  getRawValue(toInt(VehicleProperty::GECKO_IPU_DCC_SYSFAILSTA));
    if(value==1||value==2||value==3||value==4){
        val = 1;
    }else{
        val = 0;
    }
    return val;
}
int FaultDataMessage::getmotorControllerTemperatureAlarmValue() {
    return getValue(toInt(VehicleProperty::GECKO_MCU_IGBT_OVERHEAT_WARNING));
}
int FaultDataMessage::gethighVoltageInterlockStateAlarmValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_HVILFAULT));
}
int FaultDataMessage::getmotorTemperatureAlarmValue() {
    return getValue(toInt(VehicleProperty::GECKO_MCU_MOTOR_OVERHEAT_WARNING));
}
int FaultDataMessage::getvehicleEnergyStorageDeviceOverchargeAlarmValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_PACKOVERCHARGE));
}
#ifdef EV48
int FaultDataMessage::getrechargeableFaultCountValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_BATTFAULTTOTALNUM));
}
int FaultDataMessage::getrechargeableFaultCodesValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_FAULTCODE));
}
int FaultDataMessage::getdriveMotorFaultCountValue() {
    return getValue(toInt(VehicleProperty::GECKO_MCU_MOTORFAULTTOTALNUM));
}
int FaultDataMessage::getdriveMotorFaultCodesValue() {
    return getValue(toInt(VehicleProperty::GECKO_MCU_ERR_CODE));
}
int FaultDataMessage::getOtherFaultCodesValue() {
    return getValue(toInt(VehicleProperty::GECKO_VCU_FAULT_CODE));
}
#endif
void FaultDataMessage::createWarnData(uint32_t& data, int flag, int bitaddr) {
    uint32_t bit = 0x00000001;
    bit = bit << bitaddr;
    if(flag==1){
        data = data | bit;
    }
}
std::vector<uint8_t> FaultDataMessage::generateDataPacket() {
    std::vector<uint8_t> dataPacket;
    dataPacket.push_back(faultDataID);
    // 添加最高报警等级
    maxAlarmLevel = getmaxAlarmLevelValue();
    dataPacket.push_back(maxAlarmLevel);

    generalAlarmFlags = 0;
    int temperatureDifferenceAlarm; // 温度差异报警
    temperatureDifferenceAlarm = gettemperatureDifferenceAlarmValue();
    createWarnData(generalAlarmFlags,temperatureDifferenceAlarm,0);
    int batteryHighTemperatureAlarm; // 电池高温报警
    batteryHighTemperatureAlarm = getbatteryHighTemperatureAlarmValue();
    createWarnData(generalAlarmFlags,batteryHighTemperatureAlarm,1);
    int vehicleEnergyStorageDeviceOvervoltageAlarm; // 车载储能装置类受过压报警
    vehicleEnergyStorageDeviceOvervoltageAlarm = getvehicleEnergyStorageDeviceOvervoltageAlarmValue();
    createWarnData(generalAlarmFlags,vehicleEnergyStorageDeviceOvervoltageAlarm,2);
    int vehicleEnergyStorageDeviceUndervoltageAlarm; // 车载储能装置类型欠压报警
    vehicleEnergyStorageDeviceUndervoltageAlarm = getvehicleEnergyStorageDeviceUndervoltageAlarmValue();
    createWarnData(generalAlarmFlags,vehicleEnergyStorageDeviceUndervoltageAlarm,3);
    int socLowAlarm; // SOC低报警
    socLowAlarm = getsocLowAlarmValue();
    createWarnData(generalAlarmFlags,socLowAlarm,4);
    int singleCellBatteryOvervoltageAlarm; // 单体电池过压报警
    singleCellBatteryOvervoltageAlarm = getsingleCellBatteryOvervoltageAlarmValue();
    createWarnData(generalAlarmFlags,singleCellBatteryOvervoltageAlarm,5);
    int singleCellBatteryUndervoltageAlarm; // 单体电池欠压报警
    singleCellBatteryUndervoltageAlarm = getsingleCellBatteryUndervoltageAlarmValue();
    createWarnData(generalAlarmFlags,singleCellBatteryUndervoltageAlarm,6);
    int socHighAlarm; // SOC过高报警
    socHighAlarm = getsocHighAlarmValue();
    createWarnData(generalAlarmFlags,socHighAlarm,7);
    int socJumpAlarm; // SOC跳变报警
    socJumpAlarm = getsocJumpAlarmValue();
    createWarnData(generalAlarmFlags,socJumpAlarm,8);
    int rechargeableEnergyStorageSystemMismatchedAlarm; // 可充电储能系统不匹配报警
    rechargeableEnergyStorageSystemMismatchedAlarm = getrechargeableEnergyStorageSystemMismatchedAlarmValue();
    createWarnData(generalAlarmFlags,rechargeableEnergyStorageSystemMismatchedAlarm,9);
    int batterySingleConsistencyPoorAlarm; // 电池单体一致性差报警
    batterySingleConsistencyPoorAlarm = getbatterySingleConsistencyPoorAlarmValue();
    createWarnData(generalAlarmFlags,batterySingleConsistencyPoorAlarm,10);
    int insulationAlarm; // 绝缘报警
    insulationAlarm = getinsulationAlarmValue();
    createWarnData(generalAlarmFlags,insulationAlarm,11);
    int dcDcTemperatureAlarm; // DC DC温度报警
    dcDcTemperatureAlarm = getdcDcTemperatureAlarmValue();
    createWarnData(generalAlarmFlags,dcDcTemperatureAlarm,12);
    int brakeSystemAlarm; // 制动系统报警
    brakeSystemAlarm = getbrakeSystemAlarmValue();
    createWarnData(generalAlarmFlags,brakeSystemAlarm,13);
    int ldcDcStatusAlarm; // LDC-DC状态报警
    ldcDcStatusAlarm = getldcDcStatusAlarmValue();
    createWarnData(generalAlarmFlags,ldcDcStatusAlarm,14);
    int motorControllerTemperatureAlarm; // 骆动电机控制器温度报警
    motorControllerTemperatureAlarm = getmotorControllerTemperatureAlarmValue();
    createWarnData(generalAlarmFlags,motorControllerTemperatureAlarm,15);
    int highVoltageInterlockStateAlarm; // 高压互锁状态报警
    highVoltageInterlockStateAlarm = gethighVoltageInterlockStateAlarmValue();
    createWarnData(generalAlarmFlags,highVoltageInterlockStateAlarm,16);
    int motorTemperatureAlarm; // 骡动电机温度报警
    motorTemperatureAlarm = getmotorTemperatureAlarmValue();
    createWarnData(generalAlarmFlags,motorTemperatureAlarm,17);
    int vehicleEnergyStorageDeviceOverchargeAlarm; // 车载储能装置类型过充报警
    vehicleEnergyStorageDeviceOverchargeAlarm = getvehicleEnergyStorageDeviceOverchargeAlarmValue();
    createWarnData(generalAlarmFlags,vehicleEnergyStorageDeviceOverchargeAlarm,18);

    //LOG(INFO) << __func__ << __LINE__ << ": " << generalAlarmFlags;

    // 添加通用报警标志
    dataPacket.push_back((generalAlarmFlags >> 24) & 0xFF);
    dataPacket.push_back((generalAlarmFlags >> 16) & 0xFF);
    dataPacket.push_back((generalAlarmFlags >> 8) & 0xFF);
    dataPacket.push_back(generalAlarmFlags & 0xFF);

    LOG(INFO) << "rechargeableFaultCode, size : " << rechargeableFaultCodes.size();
    LOG(INFO) << "driveMotorFaultCode, size : " << driveMotorFaultCodes.size();
    LOG(INFO) << "otherFaultCode, size : " << otherFaultCodes.size();
    // 添加可充电储能装置故障总数
    //rechargeableFaultCount = getrechargeableFaultCountValue();
    rechargeableFaultCount = rechargeableFaultCodes.size();
    dataPacket.push_back(rechargeableFaultCount);

    // 添加可充电储能装置故障代码列表
    for(auto i = 0; i<rechargeableFaultCount; i++){
        dataPacket.push_back((rechargeableFaultCodes[i] >> 24) & 0xFF);
        dataPacket.push_back((rechargeableFaultCodes[i] >> 16) & 0xFF);
        dataPacket.push_back((rechargeableFaultCodes[i] >> 8) & 0xFF);
        dataPacket.push_back(rechargeableFaultCodes[i] & 0xFF);
     }

    // 添加驱动电机故障总数
    driveMotorFaultCount = driveMotorFaultCodes.size();
    dataPacket.push_back(driveMotorFaultCount);

    // 添加驱动电机故障代码列表
    for(auto i = 0; i<driveMotorFaultCount;i++){
        dataPacket.push_back((driveMotorFaultCodes[i] >> 24) & 0xFF);
        dataPacket.push_back((driveMotorFaultCodes[i] >> 16) & 0xFF);
        dataPacket.push_back((driveMotorFaultCodes[i] >> 8) & 0xFF);
        dataPacket.push_back(driveMotorFaultCodes[i] & 0xFF);
    }

    // 添加发动机故障总数，设置为0，不添加
    engineFaultCount = 0x00;
    dataPacket.push_back(engineFaultCount);

    engineFaultCodes = {0x00000001};
    // 添加发动机故障列表
    for(auto i = 0; i<engineFaultCount;i++){
        dataPacket.push_back((engineFaultCodes[0] >> 24) & 0xFF);
        dataPacket.push_back((engineFaultCodes[0] >> 16) & 0xFF);
        dataPacket.push_back((engineFaultCodes[0] >> 8) & 0xFF);
        dataPacket.push_back(engineFaultCodes[0] & 0xFF);
    }

    // 添加其他故障总数
    otherFaultCount = otherFaultCodes.size();
    dataPacket.push_back(otherFaultCount);

    // 添加其他故障代码列表
    for(auto i = 0; i<otherFaultCount;i++){
        dataPacket.push_back((otherFaultCodes[i] >> 24) & 0xFF);
        dataPacket.push_back((otherFaultCodes[i] >> 16) & 0xFF);
        dataPacket.push_back((otherFaultCodes[i] >> 8) & 0xFF);
        dataPacket.push_back(otherFaultCodes[i] & 0xFF);
    }

    return dataPacket;
}

void FaultDataMessage::printMessage() {
    /*std::cout << "最高报警等级: " << static_cast<int>(maxAlarmLevel) << std::endl;
    std::cout << "通用报警标志: " << std::hex << generalAlarmFlags << std::dec << std::endl;
    std::cout << "可充电储能装置故障总数: " << static_cast<int>(rechargeableFaultCount)
              << std::endl;
    std::cout << "可充电储能装置故障代码列表: ";
    for (const auto& code : rechargeableFaultCodes) {
        std::cout << std::hex << code << " ";
    }
    std::cout << std::dec << std::endl;*/
    LOG(INFO) << "最高报警等级: " << static_cast<int>(maxAlarmLevel);
    LOG(INFO) << "通用报警标志: " << std::hex << generalAlarmFlags << std::dec;
    LOG(INFO) << "可充电储能装置故障总数: " << static_cast<int>(rechargeableFaultCount);
    LOG(INFO) << "可充电储能装置故障代码列表: ";
    for (const auto& code : rechargeableFaultCodes) {
        LOG(INFO) << std::hex << code << " ";
    }
    LOG(INFO) << std::dec;
    // 打印其他字段...
}
