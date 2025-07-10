#ifndef PARM_REPORT_MESSAGE_H
#define PARM_REPORT_MESSAGE_H

#include <string>
#include <vector>

class ParmReportMessage {
  public:
    std::vector<uint8_t> generateMessage(int idCount, const std::vector<uint8_t>& idList);

  private:
    void addWord(std::vector<uint8_t>& data, uint8_t id, uint16_t value);
    void addString(std::vector<uint8_t>& data, uint8_t id, const std::string& value);
    bool compare(uint8_t a, uint8_t b);

  public:
    // 车载终端本地存储时间周期	Local storage time cycle of Vehicle terminal
    // local_storage_timecycle_of_vehicleterminal = 3600*24*7 正常时信息上报时间周期	Information
    // reporting time cycle at Normal		normal_information_reporting_time_cycle = 30
    // 出现报警时信息上报时间周期	Information reporting time cycle at Appears Alarm
    // alarm_information_reporting_time_cycle = 1 远程服务与管理平台域名长度	Domain name length
    // of Remote Service and Management Platform		domain_name_length_of_remote_service = 100
    // 远程服务与管理平台域名	Domain name of Remote Service and Management Platform
    // domain_name_of_remote_service = "http://58.19.38.70/" 远程服务与管理平台端口	Ports of Remote
    // Service and Management Platform		port_of_remote_service = "6638" 硬件版本	Hardware
    // version		hardware_version = "1.2.1" 固件版本	Firmware version		firmware_version =
    // "1.2.1" 车载终端心跳发送周期	Heartbeat send cycle of Vehicle terminal
    // heartbeat_send_cycle_vehicleterminal = 60 终端应答超时时间	Terminal response timeout
    // terminal_response_timeout = 60 平台应答超时时间	Platform response timeout
    // platform_response_timeout = 120 连续三次登入失败后,到下一次登入的时间间隔	The login time
    // interval after Three consecutive login Fail		logintime_interval_after_loginfail = 60*30
    // 公共平台域名长度N	Domain name length of Public Platform
    // domain_name_length_of_public_platform = 100 公共平台域名	Domain name of Public Platform
    // domain_name_of_public_platform =  "http://58.19.38.70/" 公共平台端口	Ports of Public Platform
    // port_of_public_platform = "6638" 是否处于抽样检测中	Is it in sampling testing
    // sampling_testing_state = 0
    uint16_t tboxservicestartwait = 20;
    uint16_t mlocalStorageCycle = 1000;  // 3600*24*7;
    uint16_t mnormalReportCycle = 30;
    uint16_t malarmReportCycle = 1;
    uint8_t mdomainNameLen = 17;
    std::string mdomainName = "dQ.flybees.com.cn";
    uint16_t mport = 6648;
    std::string mhardwareVer = "1.2.1";
    std::string mfirmwareVer = "1.2.1";
    uint8_t mheartbeatCycle = 60;
    uint16_t mterminalRespTimeout = 76;
    uint16_t mplatformRespTimeout = 60;
    uint8_t mloginRetryInterval = 45;
    uint8_t mpublicDomainLen = 17;
    std::string mpublicDomain = "dQ.flybees.com.cn";
    uint16_t mpublicPort = 0;
    uint8_t misSampling = 1;
};

#endif