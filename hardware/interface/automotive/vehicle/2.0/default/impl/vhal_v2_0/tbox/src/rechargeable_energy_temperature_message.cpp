#include "rechargeable_energy_temperature_message.h"
#include <iostream>

int ProbeTemperatureMessage::getprobeCountsValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_PACKTOTALTEMPNUM));
}
int ProbeTemperatureMessage::gettemperatureFrameNo() {
    return getValue(toInt(VehicleProperty::GECKO_B2V_T_FRAMENO));
}
int ProbeTemperatureMessage::gettemperature1Value() {
    return getValue(toInt(VehicleProperty::GECKO_B2V_T_CELL_TEMPN1));
}
int ProbeTemperatureMessage::gettemperature2Value() {
    return getValue(toInt(VehicleProperty::GECKO_B2V_T_CELL_TEMPN2));
}
int ProbeTemperatureMessage::gettemperature3Value() {
    return getValue(toInt(VehicleProperty::GECKO_B2V_T_CELL_TEMPN3));
}
int ProbeTemperatureMessage::gettemperature4Value() {
    return getValue(toInt(VehicleProperty::GECKO_B2V_T_CELL_TEMPN4));
}
int ProbeTemperatureMessage::gettemperature5Value() {
    return getValue(toInt(VehicleProperty::GECKO_B2V_T_CELL_TEMPN5));
}
int ProbeTemperatureMessage::gettemperature6Value() {
    return getValue(toInt(VehicleProperty::GECKO_B2V_T_CELL_TEMPN6));
}
#ifdef EV48
int ProbeTemperatureMessage::gettemperature7Value() {
    return getValue(toInt(VehicleProperty::GECKO_B2V_T_CELL_TEMPN7));
}
#endif
std::vector<uint8_t> ProbeTemperatureMessage::generateDataPacket() {
    std::vector<uint8_t> dataPacket;
    // 添加类型标志09
    dataPacket.push_back(probeTemid);

    // 添加可充电储能子系统个数
    systemCount = 0x01;
    dataPacket.push_back(systemCount);

    // 添加可充电储能子系统温度信息列表

    // 添加可充电储能子系统号
    systemNumbers = 0x01;
    dataPacket.push_back(systemNumbers);

    //probeCounts = getprobeCountsValue();
    probeCounts = static_cast<uint16_t>(probeTemperatures.size());

    // 添加可充电储能温度探针个数
    dataPacket.push_back((probeCounts >> 8) & 0xFF);
    dataPacket.push_back(probeCounts & 0xFF);
    //probeTemperatures.resize(probeCounts);

    // 添加单体电池电压
    // 添加可充电储能子系统各温度探针检测到的温度值



 /*  if(probeCounts<=7){
        for(auto i = 0; i<probeCounts;i++){
            probeTemperatures[i] = temperatures[i];
        }
    }else{
        for(auto i = 0; i<7;i++){
            probeTemperatures[i] = temperatures[i];
        }
    }*/


    for(auto i=0;i<probeCounts;i++){
        dataPacket.push_back(probeTemperatures[i]);
    }

    return dataPacket;
}

void ProbeTemperatureMessage::printMessage() {
    /*std::cout << "可充电储能子系统个数: " << static_cast<int>(systemCount) << std::endl;

    std::cout << "可充电储能子系统号: " << static_cast<int>(systemNumbers) << std::endl;
    std::cout << "可充电储能温度探针个数: " << probeCounts << std::endl;

    std::cout << "可充电储能子系统各温度探针检测到的温度值: ";
    for (const auto& temperature : probeTemperatures) {
        std::cout << static_cast<int>(temperature) + 40 << " °C ";
    }
    std::cout << std::endl;*/

    LOG(INFO) << "可充电储能子系统个数: " << static_cast<int>(systemCount);
    LOG(INFO) << "可充电储能子系统号: " << static_cast<int>(systemNumbers);
    LOG(INFO) << "可充电储能温度探针个数: " << probeCounts;
    LOG(INFO) << "可充电储能子系统各温度探针检测到的温度值: ";
    for (const auto& temperature : probeTemperatures) {
        LOG(INFO) << static_cast<int>(temperature) + 40 << " °C ";
    }
}
