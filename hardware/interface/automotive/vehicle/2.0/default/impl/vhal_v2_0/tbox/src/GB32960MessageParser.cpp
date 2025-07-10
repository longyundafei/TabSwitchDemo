#include "GB32960MessageParser.h"
#include <android-base/logging.h>
#include <chrono>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <thread>
#include "tinyxml2.h"
#include "xml_config.h"

bool GB32960MessageParser::parseMessage(const std::vector<uint8_t>& message) {
    // Check the start marker
    if (message[0] != 0x23 || message[1] != 0x23) {
        //std::cout << "Invalid start marker." << std::endl;
        LOG(INFO) << "Invalid start marker.";
        return false;
    }

    // Parse the fields
    command = message[2];
    responseFlag = message[3];
    uniqueID = std::string(message.begin() + 4, message.begin() + 21);
    encryptionMethod = message[21];
    // dataLength = (message[23] << 8) | message[22];
    dataLength = (message[22] << 8) | message[23];
    data = std::vector<uint8_t>(message.begin() + 24, message.end() - 1);
    checksum = message.back();

    if (command != 7) {
        // Print the parsed fields
        LOG(INFO) << "VehicleTask: " << std::hex << static_cast<int>(command);
        LOG(INFO) << "Command: " << std::hex << static_cast<int>(command);
        LOG(INFO) << "Response Flag: " << std::hex << static_cast<int>(responseFlag);
        LOG(INFO) << "Unique ID: " << uniqueID;
        LOG(INFO) << "Encryption Method: " << std::hex << static_cast<int>(encryptionMethod);
        LOG(INFO) << "Data Length: " << dataLength;
        std::ostringstream ss;
        for (const auto& byte : data) {
            ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte) << " ";
        }
        LOG(INFO) << "Data: " << ss.str();
        LOG(INFO) << "Checksum: " << std::hex << static_cast<int>(checksum);
    }

    // Verify the checksum
    uint8_t calculatedChecksum = calculateChecksum(message);
    if (checksum != calculatedChecksum) {
        LOG(INFO) << "Invalid checksum.";
        return false;
    }

    return true;
}
bool GB32960MessageParser::parse80Message(const std::vector<uint8_t>& message) {
    mMsg80pack.time.clear();
    mMsg80pack.time.insert(mMsg80pack.time.end(), message.begin(), message.begin() + 6);
    memcpy(&mMsg80pack.parmNum, message.data() + 6, 1);
    mMsg80pack.parm.clear();
    mMsg80pack.parm.insert(mMsg80pack.parm.end(), message.begin() + 7,
                           message.begin() + 7 + mMsg80pack.parmNum);
    for (const auto& byte : mMsg80pack.time) {
        //std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte) << " ";
        LOG(INFO) << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte) << " ";
    }
    //std::cout << mMsg80pack.parmNum << "  ";
    LOG(INFO) << mMsg80pack.parmNum << "  ";
    for (const auto& byte : mMsg80pack.parm) {
        //std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte) << " ";
        LOG(INFO) << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte) << " ";
    }

    return true;
}
bool GB32960MessageParser::parse81Message(const std::vector<uint8_t>& message,
                                          ParmReportMessage& parm) {
    mMsg81pack.time.clear();
    mMsg81pack.time.insert(mMsg81pack.time.end(), message.begin(), message.begin() + 6);
    memcpy(&mMsg81pack.parmNum, message.data() + 6, 1);
    size_t i = 7;
    size_t length = 0;
    size_t lengthtemp = 0;
    mMsg81pack.parmNum = 0;
    mMsg81pack.parm.resize(0);
    // mMsg81pack.parmNum = message[6];
    while (i < message.size()) {
        uint8_t param = message[i];
        i += 1;

        msg81 new_msg;
        new_msg.id = param;

        length = 0;
        //std::cout << "param: " << (int)param << "\n";
        LOG(INFO) << "param: " << (int)param;
        switch (param) {
            case 0x01:
                parm.mlocalStorageCycle = (message[i] << 8) | message[i + 1];
                length = 2;
                break;
            case 0x02:
                parm.mnormalReportCycle = (message[i] << 8) | message[i + 1];
                length = 2;
                break;
            case 0x03:
                parm.malarmReportCycle = (message[i] << 8) | message[i + 1];
                length = 2;
                break;
            case 0x06:
                parm.mport = (message[i] << 8) | message[i + 1];
                length = 2;
                break;
            case 0x0A:
                parm.mterminalRespTimeout = (message[i] << 8) | message[i + 1];
                length = 2;
                break;
            case 0x0B:
                parm.mplatformRespTimeout = (message[i] << 8) | message[i + 1];
                length = 2;
                break;
            case 0x0F:
                parm.mpublicPort = (message[i] << 8) | message[i + 1];
                length = 2;
                break;
            case 0x04:
                parm.mdomainNameLen = message[i];
                LOG(INFO) << "04" << std::hex << std::setw(2) << std::setfill('0')
                          << static_cast<int>(message[i]);
                if (i >= message.size()) return false;
                length = 1;  // the length is given in the next byte
                lengthtemp = message[i];
                // std::cout << "lengthtemp: "<< (int)lengthtemp <<"\n";
                break;
            case 0x0D:
                parm.mpublicDomainLen = message[i];
                LOG(INFO) << "0d" << std::hex << std::setw(2) << std::setfill('0')
                          << static_cast<int>(message[i]);
                if (i >= message.size()) return false;
                length = 1;  // the length is given in the next byte
                lengthtemp = message[i];
                // std::cout << "lengthtemp: "<< (int)lengthtemp <<"\n";
                break;
            case 0x07: {
                std::string tempstr(message.begin() + i, message.begin() + i + 5);
                parm.mhardwareVer = tempstr;
                length = 5;
                break;
            }
            case 0x08: {
                std::string tempstr(message.begin() + i, message.begin() + i + 5);
                parm.mfirmwareVer = tempstr;
                length = 5;
                break;
            }
            case 0x09:
                parm.mheartbeatCycle = message[i];
                length = 1;
                break;
            case 0x0C:
                parm.mloginRetryInterval = message[i];
                length = 1;
                break;
            case 0x10:
                parm.misSampling = message[i];
                length = 1;
                break;
            case 0x05: {
                if (i >= message.size()) return false;
                length = lengthtemp * sizeof(uint8_t);  // M or N times BYTE
                std::string tempstr(message.begin() + i, message.begin() + i + length);
                parm.mdomainName = tempstr;
                break;
            }
            case 0x0E: {
                if (i >= message.size()) return false;
                length = lengthtemp * sizeof(uint8_t);  // M or N times BYTE
                std::string tempstr(message.begin() + i, message.begin() + i + length);
                parm.mpublicDomain = tempstr;
                break;
            }
            default:
                // std::cout << "Invalid parameter encountered: "<< (int)param <<"\n";
                return false;
        }

        if (i + length > message.size()) return false;  // Out of data

        //std::cout << "length: " << (int)length << "\n";
        LOG(INFO) << "length: " << (int)length;
        for (size_t j = 0; j < length; ++j) {
            new_msg.value.push_back(message[i + j]);
        }

        i += length;

        mMsg81pack.parm.push_back(new_msg);
        mMsg81pack.parmNum++;
    }
    LOG(INFO) << "mlocalStorageCycle:" << parm.mlocalStorageCycle;
    LOG(INFO) << "mnormalReportCycle:" << parm.mnormalReportCycle;
    LOG(INFO) << "malarmReportCycle:" << parm.malarmReportCycle;
    LOG(INFO) << "mdomainNameLen:" << parm.mdomainNameLen;
    LOG(INFO) << "mdomainName:" << parm.mdomainName;
    LOG(INFO) << "mport:" << parm.mport;
    LOG(INFO) << "mhardwareVer:" << parm.mhardwareVer;
    LOG(INFO) << "mfirmwareVer:" << parm.mfirmwareVer;
    LOG(INFO) << "mheartbeatCycle:" << parm.mheartbeatCycle;
    LOG(INFO) << "mterminalRespTimeout:" << parm.mterminalRespTimeout;
    LOG(INFO) << "mplatformRespTimeout:" << parm.mplatformRespTimeout;
    LOG(INFO) << "mloginRetryInterval:" << parm.mloginRetryInterval;
    LOG(INFO) << "mpublicDomainLen:" << parm.mpublicDomainLen;
    LOG(INFO) << "mpublicDomain:" << parm.mpublicDomain;
    LOG(INFO) << "mpublicPort:" << parm.mpublicPort;
    LOG(INFO) << "misSampling:" << parm.misSampling;

    return true;
}
bool GB32960MessageParser::parse82Message(const std::vector<uint8_t>& message) {
    std::ostringstream ss;
    mMsg82pack.time.clear();
    mMsg82pack.time.insert(mMsg82pack.time.end(), message.begin(), message.begin() + 6);
    mMsg82pack.commandId = message.back();
    return true;
}

bool GB32960MessageParser::parseMsgLockVehicle(const std::vector<uint8_t>& message,LockVehicleMessage &lockVehicle){
     lockVehicle.chargingConditions = message[0];
     lockVehicle.speedLimitLevel = message[1];
     return true;
}

bool GB32960MessageParser::parseMsgVersionSwitch(const std::vector<uint8_t>& message,VersionSwitchMessage &version){
     version.requestVersion = message[0];
     return true;
}


bool GB32960MessageParser::parseMsgHavcCtl(const std::vector<uint8_t>& message,HavcCtlMessage &hacvCtl) {
    hacvCtl.time.clear();
    hacvCtl.time.insert(hacvCtl.time.end(), message.begin(), message.begin() + 6);
    hacvCtl.air_conditioning_switch = message[6];
    hacvCtl.working_time = message[7];
    hacvCtl.circulation_mode = message[8];
    hacvCtl.air_outlet_mode = message[9];
    hacvCtl.cooling_switch = message[10];
    hacvCtl.heating_switch = message[11];
    hacvCtl.defrosting_switch = message[12];
    hacvCtl.fan_speed_adjustment = message[13];
    hacvCtl.temperature_adjustment = message[14];
    //add by lijing for debug 20231122
    LOG(INFO) << "air_conditioning_switch: " << std::hex << static_cast<int>(hacvCtl.air_conditioning_switch)
              << "," << std::endl << "working_time: " << std::hex << static_cast<int>(hacvCtl.working_time)
              << "," << std::endl << "circulation_mode: " << std::hex << static_cast<int>(hacvCtl.circulation_mode)
              << "," << std::endl << "air_outlet_mode: " << std::hex << static_cast<int>(hacvCtl.air_outlet_mode)
              << "," << std::endl << "cooling_switch: " << std::hex << static_cast<int>(hacvCtl.cooling_switch)
              << "," << std::endl << "heating_switch: " << std::hex << static_cast<int>(hacvCtl.heating_switch)
              << "," << std::endl << "defrosting_switch: " << std::hex << static_cast<int>(hacvCtl.defrosting_switch)
              << "," << std::endl << "fan_speed_adjustment: " << std::hex << static_cast<int>(hacvCtl.fan_speed_adjustment)
              << "," << std::endl << "temperature_adjustment: " << std::hex << static_cast<int>(hacvCtl.temperature_adjustment);
    return true;
}

bool GB32960MessageParser::parseMsgSeatHeatCtl(const std::vector<uint8_t>& message,SeatHeatCtlMessage &seatHeatCtl)  {
    seatHeatCtl.seatHeatSwitch = message[6];
    seatHeatCtl.heatLevel = message[7];
    return true;
}

void GB32960MessageParser::hvacDataHandle(std::vector<uint8_t> hvacData) {
    VehiclePropValue vehicleSethvac;
    for (uint8_t i = 0; i < hvacProperties.size(); i++) {
        int sethvacvalue = pVehicleCtl.getRightValueByPropId((int)hvacProperties[i]);
        if (sethvacvalue == -1) {
            continue;
        }
        if ((sethvacvalue != hvacData[i]) && (i != 1) && (i != 8)) {
            vehicleSethvac.areaId = pVehicleCtl.getareaIdByPropId((int)hvacProperties[i]);
            vehicleSethvac.value.int32Values.resize(1);
            vehicleSethvac.prop = (int)hvacProperties[i];
            if (hvacData[i] == 0xff) {
                vehicleSethvac.value.int32Values[0] = 0;
            } else {
                vehicleSethvac.value.int32Values[0] = hvacData[i];
            }
            pVehicleCtl.setProperty(vehicleSethvac);
        } else if ((sethvacvalue != hvacData[i]) && (i == 8)) {
            vehicleSethvac.areaId = pVehicleCtl.getareaIdByPropId((int)hvacProperties[i]);
            vehicleSethvac.value.floatValues.resize(1);
            vehicleSethvac.prop = (int)hvacProperties[i];
            vehicleSethvac.value.floatValues[0] = hvacData[i];
            pVehicleCtl.setProperty(vehicleSethvac);
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

int GB32960MessageParser::hvacHandleresponse(std::vector<uint8_t> hvacData) {
    for (uint8_t i = 0; i < hvacProperties.size(); i++) {
        if (i != 1 && i != 2 && i != 3) {
            int sethvacvalue = pVehicleCtl.getRightValueByPropId((int)hvacProperties[i]);
            if ((sethvacvalue != -1) && (sethvacvalue != hvacData[i]) && (i != 1) && (i != 8) &&
                (i != 7)) {
                if (sethvacvalue != 0 || hvacData[i] != 0xff) {
                    return 0;
                }
            }
        }
    }
    return 1;
}

void GB32960MessageParser::closehvacHandle() {
    VehiclePropValue vehicleSethvac;
    vehicleSethvac.areaId = 117;
    vehicleSethvac.prop = 354419984;
    vehicleSethvac.value.int32Values.resize(1);
    vehicleSethvac.value.int32Values[0] = 0;
    pVehicleCtl.setProperty(vehicleSethvac);
}

int GB32960MessageParser::doorLockHandle(uint8_t doorLock) {
    VehiclePropValue vehicleSetDoor;
    switch (doorLock) {
        case 0:  // open all door
            vehicleSetDoor.prop = (int)VehicleProperty::GECKO_CCM_REMOTE_LOCK;
            vehicleSetDoor.value.int32Values.resize(1);
            vehicleSetDoor.value.int32Values[0] = 2;
            pVehicleCtl.setProperty(vehicleSetDoor);
            return 1;
        case 1:  // close all door
            vehicleSetDoor.prop = (int)VehicleProperty::GECKO_CCM_REMOTE_LOCK;
            vehicleSetDoor.value.int32Values.resize(1);
            vehicleSetDoor.value.int32Values[0] = 1;
            pVehicleCtl.setProperty(vehicleSetDoor);
            return 1;
        case 2:  // ignore,do not handle, close back door
            return 0;
        case 3:  // open back door
            vehicleSetDoor.prop = (int)VehicleProperty::GECKO_CCM_REMOTE_LOCK;
            vehicleSetDoor.value.int32Values.resize(1);
            vehicleSetDoor.value.int32Values[0] = 3;
            pVehicleCtl.setProperty(vehicleSetDoor);
            return 1;
        case 4:  // ignore,do not handle, close side sliding door
            return 0;
        case 5:  // ignore,do not handle, open side sliding door
            return 0;
        case 6:  // ignore,do not handle, close direver door
            return 0;
        case 7:  // ignore,do not handle, open direver door
            return 0;
        case 8:  // ignore,do not handle, close direver&side sliding door
            return 0;
        case 9:  // ignore,do not handle, open direver&side sliding door
            return 0;
        default:
            break;
    }
    return 0;
}
void GB32960MessageParser::serviceControlDataHandle(uint8_t type, uint8_t data) {
    const char* xmlPath = "/vendor/etc/gecko-tbox-config.xml";
    tinyxml2::XMLDocument doc;
    doc.LoadFile(xmlPath);
    tinyxml2::XMLElement* config = doc.FirstChildElement("config");
    tinyxml2::XMLElement* setServiceElement = config->FirstChildElement("serviceControl");
    std::string setServiceValue = setServiceElement->GetText();
    // std::string setServiceValue = pVehicleCtl.getPropertybyPropId(557916168);

    VehiclePropValue vehicleSetService;
    VehiclePropValue maintenanceProp;
    std::stringstream ss;
    ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(data);
    std::string hexdata = ss.str();
    switch (type) {
        case 136:  // 0x88
            setServiceValue[0] = hexdata[0];
            setServiceValue[1] = hexdata[1];
            maintenanceProp.prop = static_cast<int>(VehicleProperty::GECKO_VEHICLE_REMIND_WARNING_ST);
            break;
        case 137:  // 0x89
            setServiceValue[2] = hexdata[0];
            setServiceValue[3] = hexdata[1];
            maintenanceProp.prop = static_cast<int>(VehicleProperty::GECKO_VEHICLE_PAY_WARNING_ST);
            break;
        case 138:  // 0x8a
            setServiceValue[4] = hexdata[0];
            setServiceValue[5] = hexdata[1];
            maintenanceProp.prop = static_cast<int>(VehicleProperty::GECKO_VEHICLE_ANNUAL_REVIEW_WARNING_ST);
            break;
        case 139:  // 0x8b
            setServiceValue[6] = hexdata[0];
            setServiceValue[7] = hexdata[1];
            break;
        default:
            break;
    }
    vehicleSetService.prop = static_cast<int>(VehicleProperty::GECKO_GLOBAL_ANNOUNCEMENT);
    vehicleSetService.areaId = 0;
    vehicleSetService.value.int32Values.resize(4);
    for (size_t i = 0; i < vehicleSetService.value.int32Values.size(); i++) {
        std::string temp = setServiceValue.substr(i * 2, 2);
        vehicleSetService.value.int32Values[i] = static_cast<uint8_t>(std::stoi(temp, nullptr, 16));
    }
    pVehicleCtl.setProperty(vehicleSetService);
    maintenanceProp.areaId = 0;
    maintenanceProp.value.int32Values.resize(1);
    maintenanceProp.value.int32Values[0] = static_cast<int>(data);
    pVehicleCtl.setProperty(maintenanceProp);
    setServiceElement->SetText(setServiceValue.c_str());
    doc.SaveFile(xmlPath);
    return;
}

std::vector<uint8_t> GB32960MessageParser::powerRepHandle(std::vector<uint8_t> powerRep) {
    const char* xmlPath = "/vendor/etc/gecko-tbox-config.xml";
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(xmlPath) != tinyxml2::XML_SUCCESS) {
        //std::cout << "failed load gecko-tbox-power-replenishment" << std::endl;
        LOG(INFO) << "failed load gecko-tbox-power-replenishment";
        std::vector<uint8_t> handleResponse = powerRep;
        handleResponse.push_back(0x00);
        return handleResponse;
    }
    tinyxml2::XMLElement* config = doc.FirstChildElement("config");

    std::string replenishmentCycle;
    for (int i = 0; i < 2; i++) {
        replenishmentCycle += toString(powerRep[i]);
    }
    std::string startVoltage = toString(powerRep[2]);
    std::string stopVoltage = toString(powerRep[3]);

    tinyxml2::XMLElement* replenishmentCycleElement =
            config->FirstChildElement("replenishmentCycle");
    replenishmentCycleElement->SetText(replenishmentCycle.c_str());
    tinyxml2::XMLElement* startVoltageElement = config->FirstChildElement("startvoltage");
    startVoltageElement->SetText(startVoltage.c_str());
    tinyxml2::XMLElement* stopVoltageElement = config->FirstChildElement("stopvoltage");
    stopVoltageElement->SetText(stopVoltage.c_str());

    doc.SaveFile(xmlPath);

    std::vector<uint8_t> handleResponse = powerRep;
    handleResponse.push_back(0x01);
    return handleResponse;
}

std::vector<uint8_t> GB32960MessageParser::queryPowerRepHandle() {
    const char* xmlPath = "/vendor/etc/gecko-tbox-config.xml";
    tinyxml2::XMLDocument doc;
    doc.LoadFile(xmlPath);
    tinyxml2::XMLElement* config = doc.FirstChildElement("config");

    uint16_t replenishmentCycle = static_cast<uint16_t>(
            std::stoi(config->FirstChildElement("replenishmentCycle")->GetText()));
    uint8_t startvoltage =
            static_cast<uint8_t>(std::stoi(config->FirstChildElement("startvoltage")->GetText()));
    uint8_t stopvoltage =
            static_cast<uint8_t>(std::stoi(config->FirstChildElement("stopvoltage")->GetText()));

    std::vector<uint8_t> powerRep;
    powerRep.push_back((replenishmentCycle >> 8) & 0xFF);
    powerRep.push_back(replenishmentCycle & 0xFF);
    powerRep.push_back(startvoltage);
    powerRep.push_back(stopvoltage);
    powerRep.push_back(0xFF);
    return powerRep;
}

std::vector<uint8_t> GB32960MessageParser::reChargeHandle(std::vector<uint8_t> reCharge) {
    const char* xmlPath = "/vendor/etc/gecko-tbox-config.xml";
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(xmlPath) != tinyxml2::XML_SUCCESS) {
        //std::cout << "failed load gecko-tbox-power-replenishment" << std::endl;
        LOG(INFO) << "failed load gecko-tbox-power-replenishment";
        std::vector<uint8_t> handleResponse = reCharge;
        handleResponse.push_back(0x00);
        return handleResponse;
    }
    tinyxml2::XMLElement* config = doc.FirstChildElement("config");

    std::string chargeMode = toString(reCharge[0]);
    std::string reservationMode = toString(reCharge[1]);
    std::string startHour = toString(reCharge[2]);
    std::string startMinute = toString(reCharge[3]);
    std::string stopHour = toString(reCharge[4]);
    std::string stopMinute = toString(reCharge[5]);
    std::string loopEnable;
    for (int i = 6; i < 13; i++) {
        loopEnable += toString(reCharge[i]);
    }
    std::string targetPower = toString(reCharge[13]);

    tinyxml2::XMLElement* chargeModeElement = config->FirstChildElement("chargeMode");
    chargeModeElement->SetText(chargeMode.c_str());
    tinyxml2::XMLElement* reservationModeElement = config->FirstChildElement("reservationMode");
    reservationModeElement->SetText(reservationMode.c_str());
    tinyxml2::XMLElement* startHourElement = config->FirstChildElement("startHour");
    startHourElement->SetText(startHour.c_str());
    tinyxml2::XMLElement* startMinuteElement = config->FirstChildElement("startMinute");
    startMinuteElement->SetText(startMinute.c_str());
    tinyxml2::XMLElement* stopHourElement = config->FirstChildElement("stopHour");
    stopHourElement->SetText(stopHour.c_str());
    tinyxml2::XMLElement* stopMinuteElement = config->FirstChildElement("stopMinute");
    stopMinuteElement->SetText(stopMinute.c_str());
    tinyxml2::XMLElement* loopEnableElement = config->FirstChildElement("loopEnable");
    loopEnableElement->SetText(loopEnable.c_str());
    tinyxml2::XMLElement* targetPowerElement = config->FirstChildElement("targetPower");
    targetPowerElement->SetText(targetPower.c_str());

    doc.SaveFile(xmlPath);

    std::vector<uint8_t> handleResponse = reCharge;
    handleResponse.push_back(0x01);
    return handleResponse;
}

std::vector<uint8_t> GB32960MessageParser::queryReChargeHandle() {
    const char* xmlPath = "/vendor/etc/gecko-tbox-config.xml";
    tinyxml2::XMLDocument doc;
    doc.LoadFile(xmlPath);
    tinyxml2::XMLElement* config = doc.FirstChildElement("config");

    uint8_t chargeMode =
            static_cast<uint16_t>(std::stoi(config->FirstChildElement("chargeMode")->GetText()));
    uint8_t reservationMode = static_cast<uint8_t>(
            std::stoi(config->FirstChildElement("reservationMode")->GetText()));
    uint8_t startHour =
            static_cast<uint8_t>(std::stoi(config->FirstChildElement("startHour")->GetText()));
    uint8_t startMinute =
            static_cast<uint16_t>(std::stoi(config->FirstChildElement("startMinute")->GetText()));
    uint8_t stopHour =
            static_cast<uint8_t>(std::stoi(config->FirstChildElement("stopHour")->GetText()));
    uint8_t stopMinute =
            static_cast<uint8_t>(std::stoi(config->FirstChildElement("stopMinute")->GetText()));
    std::string loopEnable = config->FirstChildElement("loopEnable")->GetText();
    std::vector<uint8_t> vecLoopEnable(loopEnable.begin(), loopEnable.end());
    uint8_t targetPower =
            static_cast<uint8_t>(std::stoi(config->FirstChildElement("targetPower")->GetText()));

    std::vector<uint8_t> powerRep;
    powerRep.push_back(chargeMode);
    powerRep.push_back(reservationMode);
    powerRep.push_back(startHour);
    powerRep.push_back(startMinute);
    powerRep.push_back(stopHour);
    powerRep.push_back(stopMinute);
    powerRep.insert(powerRep.end(), vecLoopEnable.begin(), vecLoopEnable.end());
    powerRep.push_back(targetPower);
    powerRep.push_back(0xFF);
    return powerRep;
}

std::vector<uint8_t> GB32960MessageParser::diagAuthHandle(std::vector<uint8_t> remoteDiagAuth) {
    std::vector<uint8_t> key;
    std::vector<uint8_t> response;
    key = std::vector<uint8_t>(remoteDiagAuth.begin() + 1, remoteDiagAuth.begin() + 4);
    response.push_back(0);
    response.insert(response.end(), key.begin(), key.end());
    return response;
}

std::vector<uint8_t> GB32960MessageParser::diagTransHandle(std::vector<uint8_t> remoteDiagTrans) {
    std::vector<uint8_t> response;
    response.push_back(0);
    response.push_back(0);
    response.push_back(0x72);
    response.insert(response.end(), remoteDiagTrans.begin() + 4, remoteDiagTrans.end());
    return response;
}

uint8_t GB32960MessageParser::calculateChecksum(const std::vector<uint8_t>& message) {
    uint8_t checksum = 0;
    for (size_t i = 2; i < message.size() - 1; i++) {
        checksum ^= message[i];
    }
    return checksum;
}

int GB32960MessageParser::validateRemoteControleCMD(std::string vin, const std::vector<uint8_t>& message) {
    /*std::string msgVIN = std::string(message.begin() + 4, message.begin() + 21);
    int ret = 0;
    ret = msgVIN.compare(vin);
    LOG(INFO) << "validateRemoteControleCMD vin:" << vin;
    LOG(INFO) << "validateRemoteControleCMD msgVIN:" << msgVIN;
    LOG(INFO) << "validateRemoteControleCMD ret:" << ret;*/
    int ret = validateVinForRemoteControleCMD(vin, message);
    if(ret == 1){
        LOG(INFO) << "validateRemoteControleCMD vin validate fail";
        return ret;
    }
    int msgYear = message[24];
    int msgMonth = message[25];
    int msgDay = message[26];
    int msgHour = message[27];
    int msgMin = message[28];
    LOG(INFO) << "validateRemoteControleCMD msgyear:" << msgYear << ", msgMonth:" << msgMonth << ", msgDay:" << msgDay << ", msgHour:" << msgHour << ", msgmin:" << msgMin;

    std::time_t now = std::time(nullptr);
    std::tm* timeinfo = std::localtime(&now);

    int year = timeinfo->tm_year % 100;
    int month = timeinfo->tm_mon + 1;
    int day = timeinfo->tm_mday;
    int hour = timeinfo->tm_hour;
    int minute = timeinfo->tm_min;
    //int second = timeinfo->tm_sec;
    LOG(INFO) << "validateRemoteControleCMD year:" << year << ", month:" << month << ", day:" << day << ", hour:" << hour << ", min:"<< minute;
    if ((year != msgYear) || (month != msgMonth) || (day != msgDay) || (msgHour != hour)){
        ret = 1;
    } else if (abs(minute-msgMin) > 3){
        ret = 1;
    }
    LOG(INFO) << "validateRemoteControleCMD ret = " << ret;
    return ret;
}

int GB32960MessageParser::validateVinForRemoteControleCMD(std::string vin, const std::vector<uint8_t>& message){
    std::string msgVIN = std::string(message.begin() + 4, message.begin() + 21);
    int ret = 0;
    ret = msgVIN.compare(vin);
    LOG(INFO) << "validateRemoteControleCMD vin:" << vin;
    LOG(INFO) << "validateRemoteControleCMD msgVIN:" << msgVIN;
    LOG(INFO) << "validateRemoteControleCMD ret:" << ret;
    return ret;
}