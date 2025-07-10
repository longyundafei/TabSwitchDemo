#include "RealNameAuthMessage.h"
RealNameAuthMessage::RealNameAuthMessage() {

}

std::vector<uint8_t> RealNameAuthMessage::generateDataPacket() {
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

	dataPacket.push_back(0xFF);
	dataPacket.push_back(0xFF);
	dataPacket.push_back(0xFF);
    return dataPacket;
}