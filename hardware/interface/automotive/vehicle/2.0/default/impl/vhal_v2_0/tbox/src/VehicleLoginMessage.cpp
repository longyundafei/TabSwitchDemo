#include "VehicleLoginMessage.h"
#include <ctime>
#include <iostream>
#include <cutils/properties.h>
#define PROPERTY_VALUE_MAX_LEN 250
#define LOGIN_SN_FILE "/data/vendor/tbox/loginSN.txt"
#define LOGIN_SN_FILE_SAMPLE "/data/vendor/tbox/loginSNSample.txt"

VehicleLoginMessage::VehicleLoginMessage(
        const std::string& iccId, uint8_t rechargeableEnergySubsystemCount,
        uint8_t rechargeableEnergySystemCodeLength,
        const std::vector<uint8_t>& rechargeableEnergySystemCodes)
    : iccId(iccId),
      rechargeableEnergySubsystemCount(rechargeableEnergySubsystemCount),
      rechargeableEnergySystemCodeLength(rechargeableEnergySystemCodeLength),
      rechargeableEnergySystemCodes(rechargeableEnergySystemCodes) {

    // 生成登入流水号（循环累加，最大值为65531）
     /*static uint16_t loginSerialNumber = 0;
    loginSerialNumber = (loginSerialNumber + 1) % 65532;
    serialNumber = loginSerialNumber;*/
}
std::vector<uint8_t> VehicleLoginMessage::stringToBinary(const std::string& str) {
    std::vector<uint8_t> binary;
    for (char c : str) {
        std::bitset<8> bits(c);
        uint8_t byte = static_cast<uint8_t>(bits.to_ulong());
        binary.push_back(byte);
    }
    return binary;
}

std::vector<uint8_t> VehicleLoginMessage::generateDataPacket() {
    std::vector<uint8_t> dataPacket;

    // 获取当前时间
    std::time_t now = std::time(nullptr);
    std::tm* timeinfo = std::localtime(&now);

    // 设置数据采集时间字段
    year = static_cast<uint8_t>(timeinfo->tm_year % 100);  // 年份取后两位
    month = static_cast<uint8_t>(timeinfo->tm_mon + 1);    // 月份从1开始
    day = static_cast<uint8_t>(timeinfo->tm_mday);
    hour = static_cast<uint8_t>(timeinfo->tm_hour);
    minute = static_cast<uint8_t>(timeinfo->tm_min);
    second = static_cast<uint8_t>(timeinfo->tm_sec);

    // 添加数据采集时间
    dataPacket.push_back(year);
    dataPacket.push_back(month);
    dataPacket.push_back(day);
    dataPacket.push_back(hour);
    dataPacket.push_back(minute);
    dataPacket.push_back(second);

    // 添加登入流水号
    serialNumber = generateSerialNumber();
    saveLoginSN();
    dataPacket.push_back((serialNumber >> 8) & 0xFF);
    dataPacket.push_back(serialNumber & 0xFF);

    // 添加ICC ID
    char propertyValue[PROPERTY_VALUE_MAX_LEN] = "\0";
    if(property_get("persist.iccid", propertyValue, nullptr)!=0){
        iccId = propertyValue;
    } else {
        LOG(INFO) << "ERROR : can not get iccid!!!";
    }
    std::vector<uint8_t> packetid = stringToBinary(iccId);
    dataPacket.insert(dataPacket.end(), packetid.begin(), packetid.end());

    // 添加可充电储能子系统数
    dataPacket.push_back(rechargeableEnergySubsystemCount);

    // 添加可充电储能系统编码长度
    dataPacket.push_back(rechargeableEnergySystemCodeLength);

    // 添加可充电储能系统编码
    for (const auto& code : rechargeableEnergySystemCodes) {
        dataPacket.push_back(code);
/*       if (code.length() <= rechargeableEnergySystemCodeLength) {
            dataPacket.insert(dataPacket.end(), code.begin(), code.end());
        } else {
            dataPacket.insert(dataPacket.end(), code.begin(),
                              code.begin() + rechargeableEnergySystemCodeLength);
        }*/
    }

    return dataPacket;
}

uint16_t VehicleLoginMessage::generateSerialNumber(){
    LOG(INFO) << __func__;
    uint16_t sn = 0;
    uint8_t y = 0;
    uint8_t m = 0;
    uint8_t d = 0;
    std::string yearProp;
    std::string monProp;
    std::string dayProp;
    std::string serialProp;
    std::string value;
    char propValue[PROPERTY_VALUE_MAX_LEN] = "\0";
    if(property_get("persist.tbox.loginsn", propValue, nullptr)!=0){
        value =  propValue;
        yearProp = value.substr(0,2);
        monProp = value.substr(2,2);
        dayProp = value.substr(4,2);
        serialProp = value.substr(6,5);
        LOG(INFO) << __func__ << yearProp;
        LOG(INFO) << __func__ << monProp;
        LOG(INFO) << __func__ << dayProp;
        LOG(INFO) << __func__ << serialProp;
        y = static_cast<uint8_t>(std::stoi(yearProp));
        m = static_cast<uint8_t>(std::stoi(monProp));
        d = static_cast<uint8_t>(std::stoi(dayProp));
        sn = static_cast<uint16_t>(std::stoi(serialProp));
    }

    if(day!= d||month!=m||year!=y||sn >= 65531){
        sn = 1;
    } else {
        sn++;
    }
    LOG(INFO) << __func__ << "sn:" <<static_cast<int>(sn);
    return sn;
}

void VehicleLoginMessage::saveLoginSN() {
    std::string y = std::to_string(year);
    if(y.size()<2){
        y = "0"+y;
    }
    std::string m = std::to_string(month);
    if(m.size()<2){
        m = "0"+m;
    }
    std::string d = std::to_string(day);
    if(d.size()<2){
        d = "0"+d;
    }
    std::string s = std::to_string(serialNumber);
    while(s.size()<5){
        s = "0"+s;
    }
    std::string value = y+m+d+s;
    property_set("persist.tbox.loginsn", value.c_str());
}

void VehicleLoginMessage::printMessage() {
/*    std::cout << "数据采集时间: " << static_cast<int>(year) << "-" << static_cast<int>(month) << "-"
              << static_cast<int>(day) << " " << static_cast<int>(hour) << ":"
              << static_cast<int>(minute) << ":" << static_cast<int>(second) << std::endl;
    std::cout << "登入流水号: " << serialNumber << std::endl;
    std::cout << "ICC ID: " << iccId << std::endl;
    std::cout << "可充电储能子系统数: " << static_cast<int>(rechargeableEnergySubsystemCount)
              << std::endl;
    std::cout << "可充电储能系统编码长度: " << static_cast<int>(rechargeableEnergySystemCodeLength)
              << std::endl;
    std::cout << "可充电储能系统编码: ";
    for (const auto& code : rechargeableEnergySystemCodes) {
        std::cout << code << " ";
    }
    std::cout << std::endl;*/

    LOG(INFO) << "数据采集时间: " << static_cast<int>(year) << "-" << static_cast<int>(month) << "-"
              << static_cast<int>(day) << " " << static_cast<int>(hour) << ":"
              << static_cast<int>(minute) << ":" << static_cast<int>(second);
    LOG(INFO) << "登入流水号: " << serialNumber;
    LOG(INFO) << "可充电储能子系统数: " << static_cast<int>(rechargeableEnergySubsystemCount);
    LOG(INFO) << "可充电储能系统编码长度: " << static_cast<int>(rechargeableEnergySystemCodeLength);
    LOG(INFO) << "可充电储能系统编码: ";
    for (const auto& code : rechargeableEnergySystemCodes) {
        LOG(INFO) << code << " ";
    }
}

VehicleLogoutMessage::VehicleLogoutMessage(uint16_t logoutSerialNumber)
    : logoutSerialNumber(logoutSerialNumber) {

}

std::vector<uint8_t> VehicleLogoutMessage::generateDataPacket() {
    std::vector<uint8_t> dataPacket;

    std::time_t now = std::time(nullptr);
    std::tm* timeinfo = std::localtime(&now);

    // 设置数据采集时间字段
    year = static_cast<uint8_t>(timeinfo->tm_year % 100);  // 年份取后两位
    month = static_cast<uint8_t>(timeinfo->tm_mon + 1);    // 月份从1开始
    day = static_cast<uint8_t>(timeinfo->tm_mday);
    hour = static_cast<uint8_t>(timeinfo->tm_hour);
    minute = static_cast<uint8_t>(timeinfo->tm_min);
    second = static_cast<uint8_t>(timeinfo->tm_sec);

    // 添加登出时间
    dataPacket.push_back(year);
    dataPacket.push_back(month);
    dataPacket.push_back(day);
    dataPacket.push_back(hour);
    dataPacket.push_back(minute);
    dataPacket.push_back(second);

    // 添加登出流水号
    dataPacket.push_back((logoutSerialNumber >> 8) & 0xFF);
    dataPacket.push_back(logoutSerialNumber & 0xFF);

    return dataPacket;
}

remoteCtrlRespMessage::remoteCtrlRespMessage() {
    std::time_t now = std::time(nullptr);
    std::tm* timeinfo = std::localtime(&now);

    year = static_cast<uint8_t>(timeinfo->tm_year % 100);
    month = static_cast<uint8_t>(timeinfo->tm_mon + 1);
    day = static_cast<uint8_t>(timeinfo->tm_mday);
    hour = static_cast<uint8_t>(timeinfo->tm_hour);
    minute = static_cast<uint8_t>(timeinfo->tm_min);
    second = static_cast<uint8_t>(timeinfo->tm_sec);
}

std::vector<uint8_t> remoteCtrlRespMessage::generateDataPacket(std::vector<uint8_t> response) {
    std::vector<uint8_t> dataPacket;

    dataPacket.push_back(year);
    dataPacket.push_back(month);
    dataPacket.push_back(day);
    dataPacket.push_back(hour);
    dataPacket.push_back(minute);
    dataPacket.push_back(second);
    dataPacket.insert(dataPacket.end(), response.begin(), response.end());

    return dataPacket;
}
