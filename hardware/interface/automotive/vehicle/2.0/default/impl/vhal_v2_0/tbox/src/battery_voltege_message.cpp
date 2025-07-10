#include <iostream>
#include "battery_voltage_message.h"

int BatteryVoltageMessage::getsystemVoltagesValue() {
    return getValue(toInt(VehicleProperty::GECKO_EV_BATTERY_VOLTAGE));
}
int BatteryVoltageMessage::getsystemCurrentsValue() {
    return getValue(toInt(VehicleProperty::GECKO_EV_BATTERY_CURRENT));
}
int BatteryVoltageMessage::getframeCellCountValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_PACKTOTALCELLNUM));
}

int BatteryVoltageMessage::getvoltage1Value() {
    return getValue(toInt(VehicleProperty::GECKO_B2V_V_CELL_VOLTN1));
}

int BatteryVoltageMessage::getvoltage2Value() {
    return getValue(toInt(VehicleProperty::GECKO_B2V_V_CELL_VOLTN2));
}

int BatteryVoltageMessage::getvoltage3Value() {
    return getValue(toInt(VehicleProperty::GECKO_B2V_V_CELL_VOLTN3));
}
std::vector<uint8_t> BatteryVoltageMessage::generateDataPacket() {
    std::vector<uint8_t> dataPacket;
    // 添加类型标志08
    dataPacket.push_back(batteryVoltageid);
    // 添加可充电储能系统个数
    systemCount = 0x01;
    dataPacket.push_back(systemCount);

    // 添加可充电储能子系统电压信息列表

    // 添加可充电储能子系统号
    systemNumbers = 0x01;
    dataPacket.push_back(systemNumbers);

    int sysVolt = getsystemVoltagesValue();
    if((sysVolt >= 0) && (sysVolt <= 10000)) {
        systemVoltages = sysVolt;
    }
    // 添加可充电储能装置电压
    dataPacket.push_back((systemVoltages >> 8) & 0xFF);
    dataPacket.push_back(systemVoltages & 0xFF);

    int current = getsystemCurrentsValue();
    if((current >= 0) && (current <= 20000)){
        systemCurrents = current;
    }
    // 添加可充电储能装置电流
    dataPacket.push_back((systemCurrents >> 8) & 0xFF);
    dataPacket.push_back(systemCurrents & 0xFF);
    //set total cell count
    totalCellCount = static_cast<uint16_t>(mCellVoltages.size());
    // 添加单体电池总数
    dataPacket.push_back((totalCellCount >> 8) & 0xFF);
    dataPacket.push_back(totalCellCount & 0xFF);

    startCellIndex = 0x0001;
    // 添加起始电池序号
    dataPacket.push_back((startCellIndex >> 8) & 0xFF);
    dataPacket.push_back(startCellIndex & 0xFF);

    //frameCellCount = getframeCellCountValue();
    //TODO: remove and get the value from prop
    frameCellCount = static_cast<uint8_t>(mCellVoltages.size());;
    // 添加本帧单体电池总数
    dataPacket.push_back(frameCellCount);

    // 添加单体电池电压
     for(auto i = 0;i<frameCellCount;i++){
        dataPacket.push_back((mCellVoltages[i] >> 8) & 0xFF);
        dataPacket.push_back(mCellVoltages[i] & 0xFF);
    }

    return dataPacket;
}

void BatteryVoltageMessage::printMessage() {
    /*std::cout << "系统个数: " << static_cast<int>(systemCount) << std::endl;
    std::cout << "可充电储能子系统号: " << static_cast<int>(systemNumbers) << std::endl;
    std::cout << "可充电储能装置电压: " << static_cast<double>(systemVoltages) / 10.0 << " V"
              << std::endl;
    std::cout << "可充电储能装置电流: " << static_cast<double>(systemCurrents - 1000) / 10.0 << " A"
              << std::endl;
    std::cout << "单体电池总数: " << totalCellCount << std::endl;
    std::cout << "起始电池序号: " << startCellIndex << std::endl;
    std::cout << "本帧单体电池总数: " << static_cast<int>(frameCellCount) << std::endl;
    std::cout << "单体电池电压: ";*/

    LOG(INFO) << "系统个数: " << static_cast<int>(systemCount);
    LOG(INFO) << "可充电储能子系统号: " << static_cast<int>(systemNumbers);
    LOG(INFO) << "可充电储能装置电压: " << static_cast<double>(systemVoltages) / 10.0 << " V";
    LOG(INFO) << "可充电储能装置电流: " << static_cast<double>(systemCurrents - 1000) / 10.0 << " A";
    LOG(INFO) << "单体电池总数: " << totalCellCount;
    LOG(INFO) << "起始电池序号: " << startCellIndex;
    LOG(INFO) << "本帧单体电池总数: " << static_cast<int>(frameCellCount);
    LOG(INFO) << "单体电池电压: ";
    for (const auto& voltage : mCellVoltages) {
        //std::cout << static_cast<double>(voltage) / 1000.0 << " V ";
        LOG(INFO) << static_cast<double>(voltage) / 1000.0 << " V ";
    }
    //std::cout << std::endl;
}
