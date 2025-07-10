#include "xml_config.h"
#include <android-base/logging.h>
#include "tinyxml2.h"
#define PROPERTY_VALUE_MAX_LEN 250

XMLConfig::XMLConfig() {}

int XMLConfig::vehicleConfigInit() {
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(mXmlPath) != tinyxml2::XML_SUCCESS) {
        //std::cout << "load gecko-tbox-config.xml failed" << std::endl;
        LOG(INFO) << "load gecko-tbox-config.xml failed";
        return 0;
    }

    tinyxml2::XMLElement* config = doc.FirstChildElement("config");
    if (config == nullptr) {
        //std::cout << "cannot find config node" << std::endl;
        LOG(INFO) << "cannot find config node";
        return 0;
    }
    //std::cout << "load gecko-tbox-config.xml success" << std::endl;
    LOG(INFO) << "load gecko-tbox-config.xml success";
    vehicleConfig.tboxservicestartwait =
            static_cast<int>(std::stoi(config->FirstChildElement("tboxservicestartwait")->GetText()));
    vehicleConfig.localStorageCycle =
            static_cast<int>(std::stoi(config->FirstChildElement("localStorageCycle")->GetText()));
    vehicleConfig.normalReportCycle =
            static_cast<int>(std::stoi(config->FirstChildElement("normalReportCycle")->GetText()));
    vehicleConfig.alarmReportCycle =
            static_cast<int>(std::stoi(config->FirstChildElement("alarmReportCycle")->GetText()));
    vehicleConfig.domainNameLen =
            static_cast<int>(std::stoi(config->FirstChildElement("domainNameLen")->GetText()));
    vehicleConfig.domainName = config->FirstChildElement("domainName")->GetText();
    vehicleConfig.port = static_cast<int>(std::stoi(config->FirstChildElement("port")->GetText()));
    vehicleConfig.hardwareVer = config->FirstChildElement("hardwareVer")->GetText();
    vehicleConfig.firmwareVer = config->FirstChildElement("firmwareVer")->GetText();
    vehicleConfig.heartbeatCycle =
            static_cast<int>(std::stoi(config->FirstChildElement("heartbeatCycle")->GetText()));
    vehicleConfig.terminalRespTimeout = static_cast<int>(
            std::stoi(config->FirstChildElement("terminalRespTimeout")->GetText()));
    vehicleConfig.platformRespTimeout = static_cast<int>(
            std::stoi(config->FirstChildElement("platformRespTimeout")->GetText()));
    vehicleConfig.loginRetryInterval =
            static_cast<int>(std::stoi(config->FirstChildElement("loginRetryInterval")->GetText()));
    vehicleConfig.publicDomainLen =
            static_cast<int>(std::stoi(config->FirstChildElement("publicDomainLen")->GetText()));
    vehicleConfig.publicDomain = config->FirstChildElement("publicDomain")->GetText();
    vehicleConfig.publicPort =
            static_cast<int>(std::stoi(config->FirstChildElement("publicPort")->GetText()));
    vehicleConfig.isSampling =
            static_cast<int>(std::stoi(config->FirstChildElement("isSampling")->GetText()));

    return 1;
}

void XMLConfig::LoadParmConfig(ParmReportMessage& parm) {
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(mXmlPath) != tinyxml2::XML_SUCCESS) {
        //std::cout << "load gecko-tbox-config.xml failed" << std::endl;
        LOG(INFO) << "load gecko-tbox-config.xml failed";
        return;
    }

    tinyxml2::XMLElement* config = doc.FirstChildElement("config");
    if (config == nullptr) {
        //std::cout << "cannot find config node" << std::endl;
        LOG(INFO) << "cannot find config node";
        return;
    }
    LOG(INFO) << "load gecko-tbox-config.xml success";

    parm.tboxservicestartwait =
            static_cast<int>(std::stoi(config->FirstChildElement("tboxservicestartwait")->GetText()));
    parm.mlocalStorageCycle =
            static_cast<int>(std::stoi(config->FirstChildElement("localStorageCycle")->GetText()));
    parm.mnormalReportCycle =
            static_cast<int>(std::stoi(config->FirstChildElement("normalReportCycle")->GetText()));
    parm.malarmReportCycle =
            static_cast<int>(std::stoi(config->FirstChildElement("alarmReportCycle")->GetText()));
    parm.mdomainNameLen =
            static_cast<int>(std::stoi(config->FirstChildElement("domainNameLen")->GetText()));
    parm.mdomainName = config->FirstChildElement("domainName")->GetText();
    parm.mport = static_cast<int>(std::stoi(config->FirstChildElement("port")->GetText()));
    parm.mhardwareVer = config->FirstChildElement("hardwareVer")->GetText();
    char propertyValue[PROPERTY_VALUE_MAX_LEN] = "\0";
    if(property_get("gecko.soc.hardware.version", propertyValue, nullptr)!=0){
        std::string HWVer = propertyValue;
        if(HWVer.length() > 5){
            LOG(INFO) << "HWVer = " << HWVer << ", length = " << HWVer.length();
            HWVer.erase(std::remove(HWVer.begin(), HWVer.end(), ' '), HWVer.end());
            parm.mhardwareVer = HWVer.substr(HWVer.length()-5,5);
            LOG(INFO) << "1parm.mhardwareVer = " << parm.mhardwareVer;
        }
    }
    LOG(INFO) << "parm.mhardwareVer = " << parm.mhardwareVer;
    parm.mfirmwareVer = config->FirstChildElement("firmwareVer")->GetText();
    if(property_get("ro.build.id", propertyValue, nullptr)!=0){
        std::string SWVer = propertyValue;
        parm.mfirmwareVer = SWVer.substr(3,5);
    }
    LOG(INFO) << "parm.mfirmwareVer = " << parm.mfirmwareVer;
    parm.mheartbeatCycle =
            static_cast<int>(std::stoi(config->FirstChildElement("heartbeatCycle")->GetText()));
    parm.mterminalRespTimeout = static_cast<int>(
            std::stoi(config->FirstChildElement("terminalRespTimeout")->GetText()));
    parm.mplatformRespTimeout = static_cast<int>(
            std::stoi(config->FirstChildElement("platformRespTimeout")->GetText()));
    parm.mloginRetryInterval =
            static_cast<int>(std::stoi(config->FirstChildElement("loginRetryInterval")->GetText()));
    parm.mpublicDomainLen =
            static_cast<int>(std::stoi(config->FirstChildElement("publicDomainLen")->GetText()));
    parm.mpublicDomain = config->FirstChildElement("publicDomain")->GetText();
    parm.mpublicPort =
            static_cast<int>(std::stoi(config->FirstChildElement("publicPort")->GetText()));
    parm.misSampling =
            static_cast<int>(std::stoi(config->FirstChildElement("isSampling")->GetText()));
}
void XMLConfig::UpdateParmConfig(ParmReportMessage& parm) {
    updateConfig("tboxservicestartwait", std::to_string(parm.tboxservicestartwait));
    updateConfig("localStorageCycle", std::to_string(parm.mlocalStorageCycle));
    updateConfig("normalReportCycle", std::to_string(parm.mnormalReportCycle));
    updateConfig("alarmReportCycle", std::to_string(parm.malarmReportCycle));
    updateConfig("domainNameLen", std::to_string(parm.mdomainNameLen));
    updateConfig("domainName", parm.mdomainName);
    updateConfig("port", std::to_string(parm.mport));
    updateConfig("hardwareVer", parm.mhardwareVer);
    updateConfig("firmwareVer", parm.mfirmwareVer);
    updateConfig("heartbeatCycle", std::to_string(parm.mheartbeatCycle));
    updateConfig("terminalRespTimeout", std::to_string(parm.mterminalRespTimeout));
    updateConfig("platformRespTimeout", std::to_string(parm.mplatformRespTimeout));
    updateConfig("loginRetryInterval", std::to_string(parm.mloginRetryInterval));
    updateConfig("publicDomainLen", std::to_string(parm.mpublicDomainLen));
    updateConfig("publicDomain", parm.mpublicDomain);
    updateConfig("publicPort", std::to_string(parm.mpublicPort));
    updateConfig("isSampling", std::to_string(parm.misSampling));
}
void XMLConfig::updateConfig(const std::string& key, const std::string& value) {
    // 实现更新配置
    tinyxml2::XMLDocument doc;
    LOG(INFO) << __func__ << "-" << mXmlPath << "-" << key << "-" << value;
//    doc.LoadFile(mXmlPath);
    if (doc.LoadFile(mXmlPath) != tinyxml2::XML_SUCCESS) {
        //std::cout << "load gecko-tbox-config.xml failed" << std::endl;
        LOG(INFO) << "load gecko-tbox-config.xml failed";
        LOG(INFO) << __func__ << __LINE__ << "-" << mXmlPath << "-" << key << "-" << value;
        return;
    }

    tinyxml2::XMLElement* config = doc.FirstChildElement("config");
    if (config == nullptr) {
        LOG(INFO) << __func__ << __LINE__ << "-" << mXmlPath << "-" << key << "-" << value;
        return;
    }

    tinyxml2::XMLElement* elem = config->FirstChildElement(key.c_str());
    if (elem) {
        elem->SetText(value.c_str());
    }

    LOG(INFO) << __func__ << __LINE__ << "-" << mXmlPath << "-" << key << "-" << value;
    doc.SaveFile(mXmlPath);
}

std::string XMLConfig::getConfig(const std::string& key) {
    // 实现获取配置
    tinyxml2::XMLDocument doc;
    doc.LoadFile(mXmlPath);

    tinyxml2::XMLElement* config = doc.FirstChildElement("config");
    if (config == nullptr) {
        return "";
    }

    tinyxml2::XMLElement* elem = config->FirstChildElement(key.c_str());
    if (elem) {
        return elem->GetText();
    } else {
        return "";
    }
}