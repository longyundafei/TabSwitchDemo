#include "RealTimeDataMessage.h"
#include <ctime>

// 实时上报数据消息类的构造函数
RealTimeDataMessage::RealTimeDataMessage() {
    std::time_t now = std::time(nullptr);
    std::tm* timeinfo = std::localtime(&now);

    // 设置数据采集时间字段
    year = static_cast<uint8_t>(timeinfo->tm_year % 100);  // 年份取后两位
    month = static_cast<uint8_t>(timeinfo->tm_mon + 1);    // 月份从1开始
    day = static_cast<uint8_t>(timeinfo->tm_mday);
    hour = static_cast<uint8_t>(timeinfo->tm_hour);
    minute = static_cast<uint8_t>(timeinfo->tm_min);
    second = static_cast<uint8_t>(timeinfo->tm_sec);
}

// 添加二进制信息包
void RealTimeDataMessage::addInfoPacket(const std::vector<uint8_t>& infoPacket) {
    infoList.insert(infoList.end(), infoPacket.begin(), infoPacket.end());
}

// 生成数据包
std::vector<uint8_t> RealTimeDataMessage::generateDataPacket() {
    std::vector<uint8_t> dataPacket;
    // 添加数据采集时间
    dataPacket.push_back(year);
    dataPacket.push_back(month);
    dataPacket.push_back(day);
    dataPacket.push_back(hour);
    dataPacket.push_back(minute);
    dataPacket.push_back(second);
    dataPacket.insert(dataPacket.end(), infoList.begin(), infoList.end());
    return dataPacket;
}

// 打印消息内容
void RealTimeDataMessage::printMessage() {
/*    std::cout << "数据采集时间: " << static_cast<int>(year) << "-" << static_cast<int>(month) << "-"
              << static_cast<int>(day) << " " << static_cast<int>(hour) << ":"
              << static_cast<int>(minute) << ":" << static_cast<int>(second) << std::endl;

    std::cout << "二进制信息列表：";
    for (uint8_t byte : infoList) {
        std::cout << static_cast<int>(byte) << " ";
    }
    std::cout << std::endl;*/

    LOG(INFO) << "数据采集时间: " << static_cast<int>(year) << "-" << static_cast<int>(month) << "-"
              << static_cast<int>(day) << " " << static_cast<int>(hour) << ":"
              << static_cast<int>(minute) << ":" << static_cast<int>(second);

    LOG(INFO) << "二进制信息列表：";
    for (uint8_t byte : infoList) {
        LOG(INFO) << static_cast<int>(byte) << " ";
    }
}
