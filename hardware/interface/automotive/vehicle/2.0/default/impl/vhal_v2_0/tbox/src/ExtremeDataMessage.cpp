#include "ExtremeDataMessage.h"
#include <bitset>
#include <cmath>
#include <iostream>
#include <vector>
#ifdef EV48
int ExtremeDataMessage::getmaxVoltageBatterySystemValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_MAXVOLTCELLPACKNUM));
}
int ExtremeDataMessage::getMinVoltageBatterySystemValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_MINVOLTCELLPACKNUM));
}
int ExtremeDataMessage::getMaxTemperatureSubsystemValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_MAXTEMPCELLPACKNUM));
}
int ExtremeDataMessage::getMinTemperatureSubsystemValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_MINTEMPCELLPACKNUM));
}
#endif
int ExtremeDataMessage::getmaxVoltageBatteryModuleValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_MAXVOLTMONOMERCELL));
}
int ExtremeDataMessage::getmaxVoltageValue() {
    return getValue(toInt(VehicleProperty::GECKO_MOMER_MAX_VOLTCELL));
}
int ExtremeDataMessage::getMinVoltageBatteryModuleValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_MINVOLTMONOMERCELL));
}
int ExtremeDataMessage::getMinVoltageValue() {
    return getValue(toInt(VehicleProperty::GECKO_MOMER_MIN_VOLTCELL));
}

int ExtremeDataMessage::getMaxTemperatureProbeValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_MAXTEMPMONOMERCELL));
}
int ExtremeDataMessage::getMaxTemperatureValue() {
    return getValue(toInt(VehicleProperty::GECKO_MOMER_MAX_TEMPCELL));
}
int ExtremeDataMessage::getMinTemperatureProbeValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_MINTEMPMONOMERCELL));
}
int ExtremeDataMessage::getMinTemperatureValue() {
    return getValue(toInt(VehicleProperty::GECKO_MOMER_MIN_TEMPCELL));
}
std::vector<uint8_t> ExtremeDataMessage::generateDataPacket() {
    std::vector<uint8_t> dataPacket;

    dataPacket.push_back(extremedataid);
    /*int maxVoltBatSys = getmaxVoltageBatterySystemValue();
    if((maxVoltBatSys <= 31) && (maxVoltBatSys >= 1)){
        maxVoltageBatterySystem = maxVoltBatSys;
    } else {
        LOG(INFO) << "warning, cannot get maxVoltageBatterySystem, reporting last value :" << static_cast<int>(maxVoltageBatterySystem);
    }*/
    // 添加最高电压电池系统号
    dataPacket.push_back(maxVoltageBatterySystem);

    int maxVoltBatModule = getmaxVoltageBatteryModuleValue();
    if((maxVoltBatModule <= 250) && (maxVoltBatModule >= 1)){
        maxVoltageBatteryModule = maxVoltBatModule;
    } else {
        LOG(INFO) << "warning, cannot get maxVoltageBatteryModule, reporting last value :" << static_cast<int>(maxVoltageBatteryModule);
    }

    // 添加最高电压电池单体代号
    dataPacket.push_back(maxVoltageBatteryModule);

    // 添加电池单体电压最高值
    int16_t maxVoltageValue;
    maxVoltageValue = getmaxVoltageValue();
    dataPacket.push_back((maxVoltageValue >> 8) & 0xFF);
    dataPacket.push_back(maxVoltageValue & 0xFF);

    /*int minVoltSysNum = getMinVoltageBatterySystemValue();
    if((minVoltSysNum <= 31) && (minVoltSysNum >= 1)) {
        minVoltageBatterySystem = minVoltSysNum;
    } else {
        LOG(INFO) << "warning, cannot get minVoltageBatterySystem, reporting last value :" << static_cast<int>(minVoltageBatterySystem);
    }*/
    // 添加最低电压电池系统号
    dataPacket.push_back(minVoltageBatterySystem);

    int minVoltModuleNum = getMinVoltageBatteryModuleValue();
    if((minVoltModuleNum <= 250) && (minVoltModuleNum >= 1)){
        minVoltageBatteryModule = minVoltModuleNum;
    } else {
        LOG(INFO) << "warning, cannot get minVoltageBatteryModule, reporting last value :" << static_cast<int>(minVoltageBatteryModule);
    }

    // 添加最低电压电池单体代号
    dataPacket.push_back(minVoltageBatteryModule);

    // 添加电池单体电压最低值
    int16_t minVoltageValue;
    minVoltageValue = getMinVoltageValue();
    dataPacket.push_back((minVoltageValue >> 8) & 0xFF);
    dataPacket.push_back(minVoltageValue & 0xFF);

    /*int maxTempSubsys =  getMaxTemperatureSubsystemValue();
    if((maxTempSubsys <= 31) && (maxTempSubsys >= 1)){
        maxTemperatureSubsystem = maxTempSubsys;
    } else {
        LOG(INFO) << "warning, cannot get maxTemperatureSubsystem, reporting last value :" << static_cast<int>(maxTemperatureSubsystem);
    }*/
    // 添加最高温度子系统号
    dataPacket.push_back(maxTemperatureSubsystem);

    int maxTempProbe = getMaxTemperatureProbeValue();
    if((maxTempProbe <= 250) && (maxTempProbe >= 1)){
        maxTemperatureProbe = maxTempProbe;
    } else {
        LOG(INFO) << "warning, cannot get maxTemperatureProbe, reporting last value :" << static_cast<int>(maxTemperatureProbe);
    }
    // 添加最高温度探针序号
    dataPacket.push_back(maxTemperatureProbe);

    int maxTemp = getMaxTemperatureValue();
    if(maxTemp > 0){
        maxTemperature = maxTemp;
    } else {
        LOG(INFO) << "warning, cannot get maxTemperature, reporting last value :" << static_cast<int>(maxTemperature);
    }
    // 添加最高温度值
    dataPacket.push_back(maxTemperature);

    /*int minTempSubsystem =  getMinTemperatureSubsystemValue();
    if((minTempSubsystem <= 31) && (minTempSubsystem >= 1)){
        minTemperatureSubsystem = minTempSubsystem;
    } else {
        LOG(INFO) << "warning, cannot get minTemperatureSubsystem, reporting last value :" << static_cast<int>(minTemperatureSubsystem);
    }*/
    // 添加最低温度子系统号
    dataPacket.push_back(minTemperatureSubsystem);

    int minTempProbe =  getMinTemperatureProbeValue();
    if((minTempProbe <= 250) && (minTempProbe >= 1)){
        minTemperatureProbe = minTempProbe;
    } else {
        LOG(INFO) << "warning, cannot get minTemperatureProbe, reporting last value :" << static_cast<int>(minTemperatureProbe);
    }
    // 添加最低温度探针序号
    dataPacket.push_back(minTemperatureProbe);

    int minTemp = getMinTemperatureValue();
    if (minTemp > 0){
        minTemperature =  minTemp;
    } else {
        LOG(INFO) << "warning, cannot get minTemperature, reporting last value :" << static_cast<int>(minTemperature);
    }
    // 添加最低温度值
    dataPacket.push_back(minTemperature);
    return dataPacket;
}

void ExtremeDataMessage::printMessage() {
    /*std::cout << "信息ID: " << static_cast<int>(extremedataid) << std::endl;
    std::cout << "最高电压电池系统号: " << static_cast<int>(maxVoltageBatterySystem) << std::endl;
    std::cout << "最高电压电池单体代号: " << static_cast<int>(maxVoltageBatteryModule) << std::endl;
    std::cout << "电池单体电压最高值: " << maxVoltage << " V" << std::endl;
    std::cout << "最低电压电池系统号: " << static_cast<int>(minVoltageBatterySystem) << std::endl;
    std::cout << "最低电压电池单体代号: " << static_cast<int>(minVoltageBatteryModule) << std::endl;
    std::cout << "电池单体电压最低值: " << minVoltage << " V" << std::endl;
    std::cout << "最高温度子系统号: " << static_cast<int>(maxTemperatureSubsystem) << std::endl;
    std::cout << "最高温度探针序号: " << static_cast<int>(maxTemperatureProbe) << std::endl;
    std::cout << "最高温度值: " << static_cast<int>(maxTemperature) << " °C" << std::endl;
    std::cout << "最低温度子系统号: " << static_cast<int>(minTemperatureSubsystem) << std::endl;
    std::cout << "最低温度探针序号: " << static_cast<int>(minTemperatureProbe) << std::endl;
    std::cout << "最低温度值: " << static_cast<int>(minTemperature) << " °C" << std::endl;*/
    LOG(INFO) << "信息ID: " << static_cast<int>(extremedataid);
    LOG(INFO) << "最高电压电池系统号: " << static_cast<int>(maxVoltageBatterySystem);
    LOG(INFO) << "最高电压电池单体代号: " << static_cast<int>(maxVoltageBatteryModule);
    LOG(INFO) << "电池单体电压最高值: " << maxVoltage << " V";
    LOG(INFO) << "最低电压电池系统号: " << static_cast<int>(minVoltageBatterySystem);
    LOG(INFO) << "最低电压电池单体代号: " << static_cast<int>(minVoltageBatteryModule);
    LOG(INFO) << "电池单体电压最低值: " << minVoltage << " V";
    LOG(INFO) << "最高温度子系统号: " << static_cast<int>(maxTemperatureSubsystem);
    LOG(INFO) << "最高温度探针序号: " << static_cast<int>(maxTemperatureProbe);
    LOG(INFO) << "最高温度值: " << static_cast<int>(maxTemperature) << " °C";
    LOG(INFO) << "最低温度子系统号: " << static_cast<int>(minTemperatureSubsystem);
    LOG(INFO) << "最低温度探针序号: " << static_cast<int>(minTemperatureProbe);
    LOG(INFO) << "最低温度值: " << static_cast<int>(minTemperature) << " °C";
}
