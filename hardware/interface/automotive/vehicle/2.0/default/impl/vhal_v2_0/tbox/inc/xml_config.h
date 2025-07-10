#ifndef XML_CONFIG_H
#define XML_CONFIG_H
#include <iostream>
#include <string>
#include <cutils/properties.h>
#include "parm_report_message.h"
struct NetParmSave{
    std::string ip;
    int iplen;
    int port;
    std::string pip;
    int piplen;
    int pport;
};
class XMLConfig {
  public:
    struct strVehicleConfig {
        int tboxservicestartwait;
        int localStorageCycle;
        int normalReportCycle;
        int alarmReportCycle;
        int domainNameLen;
        std::string domainName;
        int port;
        std::string hardwareVer;
        std::string firmwareVer;
        int heartbeatCycle;
        int terminalRespTimeout;
        int platformRespTimeout;
        int loginRetryInterval;
        int publicDomainLen;
        std::string publicDomain;
        int publicPort;
        int isSampling;
    };
    strVehicleConfig vehicleConfig;
    int ChangeNetParm;
    NetParmSave ChangeNetParmSave;

  public:
    XMLConfig();
    int vehicleConfigInit();
    void updateConfig(const std::string& key, const std::string& value);
    std::string getConfig(const std::string& key);
    void LoadParmConfig(ParmReportMessage& parm);
    void UpdateParmConfig(ParmReportMessage& parm);

  private:
    const char* mXmlPath = "/data/vendor/tbox/gecko-tbox-config.xml";
};

#endif