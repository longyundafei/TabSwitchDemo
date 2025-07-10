#include "parm_report_message.h"
#include <ctime>
std::vector<uint8_t> ParmReportMessage::generateMessage(int idCount,
                                                        const std::vector<uint8_t>& idList) {
    std::vector<uint8_t> data;
    // 获取当前时间
    std::time_t now = std::time(nullptr);
    std::tm* timeinfo = std::localtime(&now);

    uint8_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    // 设置数据采集时间字段
    year = static_cast<uint8_t>(timeinfo->tm_year % 100);  // 年份取后两位
    month = static_cast<uint8_t>(timeinfo->tm_mon + 1);    // 月份从1开始
    day = static_cast<uint8_t>(timeinfo->tm_mday);
    hour = static_cast<uint8_t>(timeinfo->tm_hour);
    minute = static_cast<uint8_t>(timeinfo->tm_min);
    second = static_cast<uint8_t>(timeinfo->tm_sec);

    // 添加数据采集时间
    data.push_back(year);
    data.push_back(month);
    data.push_back(day);
    data.push_back(hour);
    data.push_back(minute);
    data.push_back(second);
    data.push_back((uint8_t)idCount);

    for (int i = 0; i < idCount; i++) {
        int id = idList[i];
        switch (id) {
            case 0x01:
                // 添加本地存储周期
                addWord(data, 0x01, mlocalStorageCycle);
                break;
            case 0x02:
                // 0x02 正常上报周期
                addWord(data, 0x02, mnormalReportCycle);
                break;
            case 0x03:
                // 0x03 报警上报周期
                addWord(data, 0x03, malarmReportCycle);
                break;
            case 0x04:
                // 0x04 域名长度
                data.push_back(0x04);
                // 添加数据长度
                data.push_back(static_cast<uint8_t>(mdomainNameLen & 0xFF));  // 低字节
                break;
            case 0x05:
                // 0x05 域名
                addString(data, 0x05, mdomainName);
                break;
            case 0x06:
                // 0x06 端口号
                addWord(data, 0x06, mport);
                break;
            case 0x07:
                // 0x07 硬件版本
                addString(data, 0x07, mhardwareVer);
                break;
            case 0x08:
                // 0x08 固件版本
                addString(data, 0x08, mfirmwareVer);
                break;
            case 0x09:
                // 0x09 心跳周期
                data.push_back(0x09);
                data.push_back(mheartbeatCycle);
                break;
            case 0x0A:
                // 0x0A 终端应答超时时间
                addWord(data, 0x0A, mterminalRespTimeout);
                break;
            case 0x0B:
                // 0x0B 平台应答超时时间
                addWord(data, 0x0B, mplatformRespTimeout);
                break;
            case 0x0C:
                // 0x0C 登入重试间隔
                data.push_back(0x0C);
                data.push_back(mloginRetryInterval);
                break;
            case 0x0D:
                // 0x0D 公共平台域名长度
                data.push_back(0x0D);
                // 添加数据长度
                data.push_back(static_cast<uint8_t>(mpublicDomainLen & 0xFF));  // 低字节
                break;
            case 0x0E:
                // 0x0E 公共平台域名
                addString(data, 0x0E, mpublicDomain);
                break;
            case 0x0F:
                // 0x0F 公共平台端口
                addWord(data, 0x0F, mpublicPort);
                break;
            case 0x10:
                // 0x10 是否处于抽样监测
                data.push_back(0x10);
                data.push_back(misSampling);
                break;
            // ...其他id处理
            default:
                break;
        }
    }
    return data;
}

void ParmReportMessage::addWord(std::vector<uint8_t>& data, uint8_t id, uint16_t value) {
    data.push_back(id);
    data.push_back((value >> 8) & 0xFF);
    data.push_back(value & 0xFF);
}
void ParmReportMessage::addString(std::vector<uint8_t>& data, uint8_t id,
                                  const std::string& value) {
    data.push_back(id);
    for (char c : value) {
        data.push_back(c);
    }
}
bool ParmReportMessage::compare(uint8_t a, uint8_t b) {
    return a < b;
}
// 其他函数实现