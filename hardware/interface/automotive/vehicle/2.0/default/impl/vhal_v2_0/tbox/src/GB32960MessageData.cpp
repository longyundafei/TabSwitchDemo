#include "GB32960MessageData.h"
#include <android-base/logging.h>
#include <bitset>
#include <iomanip>
#include <iostream>
#define PROPERTY_VALUE_MAX_LEN 250

GB32960MessageData::GB32960MessageData(LockMap<int32_t, TboxDataDeclaration>& tboxdecl,
                                       VehicleController& vehicle) {
    mTboxdecl = &tboxdecl;
    mVehicle = vehicle;
}

GB32960MessageData::GB32960MessageData(uint8_t command, uint8_t responseFlag,
                                       const std::string& uniqueID, uint8_t encryptionMethod,
                                       const std::vector<uint8_t>& data)
    : startMarker("##"),
      command(command),
      responseFlag(responseFlag),
      uniqueID(uniqueID),
      encryptionMethod(encryptionMethod),
      dataLength(data.size()),
      data(data),
      checksum(0) {
    calculateChecksum();
}

void GB32960MessageData::calculateChecksum() {
    checksum = 0;
    checksum ^= command;
    checksum ^= responseFlag;
    for (const auto& byte : uniqueID) {
        checksum ^= byte;
    }
    checksum ^= encryptionMethod;
    checksum ^= dataLength & 0xFF;
    checksum ^= (dataLength >> 8) & 0xFF;
    for (const auto& byte : data) {
        checksum ^= byte;
    }
}

void GB32960MessageData::printMessage() {
    /*std::cout << "起始符: " << startMarker << std::endl;
    std::cout << "命令单元: " << static_cast<int>(command) << std::endl;
    std::cout << "应答标志: " << static_cast<int>(responseFlag) << std::endl;
    std::cout << "唯一识别码: " << uniqueID << std::endl;
    std::cout << "数据单元加密方式: " << static_cast<int>(encryptionMethod) << std::endl;
    std::cout << "数据单元长度: " << dataLength << std::endl;
    std::cout << "数据单元: ";
    for (const auto& byte : data) {
        std::cout << static_cast<int>(byte) << " ";
    }
    std::cout << std::endl;
    std::cout << "校验码: " << static_cast<int>(checksum) << std::endl;*/
    LOG(INFO) << "起始符: " << startMarker;
    LOG(INFO) << "命令单元: " << static_cast<int>(command);
    LOG(INFO) << "应答标志: " << static_cast<int>(responseFlag);
    LOG(INFO) << "唯一识别码: " << uniqueID;
    LOG(INFO) << "数据单元加密方式: " << static_cast<int>(encryptionMethod);
    LOG(INFO) << "数据单元长度: " << dataLength;
    LOG(INFO) << "数据单元: ";
    for (const auto& byte : data) {
        LOG(INFO) << static_cast<int>(byte) << " ";
    }
    LOG(INFO) << "校验码: " << static_cast<int>(checksum);
}
std::vector<uint8_t> GB32960MessageData::stringToBinary(const std::string& str) {
    std::vector<uint8_t> binary;
    for (char c : str) {
        std::bitset<8> bits(c);
        uint8_t byte = static_cast<uint8_t>(bits.to_ulong());
        binary.push_back(byte);
    }
    return binary;
}

std::vector<uint8_t> GB32960MessageData::generateDataPacket() {
    std::vector<uint8_t> packet;

    // 添加起始标志
    packet.insert(packet.end(), startMarker.begin(), startMarker.end());

    // 添加命令
    packet.push_back(command);

    // 添加响应标志
    packet.push_back(responseFlag);

    // 添加唯一ID
    std::vector<uint8_t> packetid = stringToBinary(uniqueID);
    packet.insert(packet.end(), packetid.begin(), packetid.end());

    // 添加加密方法
    packet.push_back(encryptionMethod);

    // 添加数据长度
    packet.push_back(static_cast<uint8_t>(dataLength >> 8));    // 高字节
    packet.push_back(static_cast<uint8_t>(dataLength & 0xFF));  // 低字节

    // 添加数据
    packet.insert(packet.end(), data.begin(), data.end());

    // 计算并添加校验和
    // checksum = 0xaa;
    packet.push_back(checksum);
    if (command != 7) {
        // 打印二进制报文
        std::ostringstream ss;
        for (const auto& byte : packet) {
            ss << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(byte) << " ";
        }
        //LOG(INFO) << "Generated Data Packet: ";
        //LOG(INFO) << ss.str();
    }
    //    std::cout << std::dec << std::endl;
    return packet;
}
std::vector<uint8_t> GB32960MessageData::generateLoginMessage() {
    // 生成车辆登入的二进制信息
//    std::vector<std::string> rechargeableEnergySystemCodes = {"A"};
    // 创建车辆登入消息对象
//    VehicleLoginMessage vehicleloginMessage(mIccid, 1, 1, rechargeableEnergySystemCodes);
    // 生成数据包并打印消息内容
    std::vector<uint8_t> vehicleloginMessagedataPacket = mVehicleloginMessage.generateDataPacket();
    GB32960MessageData vehicleloginGBmessage(0x01, 0xFE, mUniqueID, 0x01,
                                             vehicleloginMessagedataPacket);
    std::vector<uint8_t> message = vehicleloginGBmessage.generateDataPacket();
    // uint16_t MachineLoginserialNumber = vehicleloginMessage.getserialNumber();
    uint16_t sn = mVehicleloginMessage.getserialNumber();
    //std::cout << "generateLoginMessage" << std::endl;
    LOG(INFO) << "generateLoginMessage sn : " << sn;
    return message;
}
std::vector<uint8_t> GB32960MessageData::generateLoginMessageSample() {
    // 生成车辆登入的二进制信息
    // 生成数据包并打印消息内容
    std::vector<uint8_t> vehicleloginMessagedataPacket = mVehicleloginMessageSample.generateDataPacket();
    GB32960MessageData vehicleloginGBmessage(0x01, 0xFE, mUniqueID, 0x01,
                                             vehicleloginMessagedataPacket);
    std::vector<uint8_t> message = vehicleloginGBmessage.generateDataPacket();
    // uint16_t MachineLoginserialNumber = vehicleloginMessage.getserialNumber();
    uint16_t sn = mVehicleloginMessageSample.getserialNumber();
    //std::cout << "generateLoginMessage" << std::endl;
    LOG(INFO) << "generateLoginMessageSample sn : " << sn;
    return message;
}
std::vector<uint8_t> GB32960MessageData::generateLogoutMessage() {
    // 创建车辆登出消息对象
    uint16_t sn = mVehicleloginMessage.getserialNumber();
    VehicleLogoutMessage vehiclelogoutMessage(sn);
    // 生成数据包并打印消息内容
    std::vector<uint8_t> vehiclelogoutMessagedataPacket = vehiclelogoutMessage.generateDataPacket();
    GB32960MessageData vehiclelogoutGBmessage(0x04, 0xFE, mUniqueID, 0x01,
                                             vehiclelogoutMessagedataPacket);
    std::vector<uint8_t> message = vehiclelogoutGBmessage.generateDataPacket();
    //std::cout << "generateLoginMessage" << std::endl;
    LOG(INFO) << "generateLogoutMessage sn :" << sn;
    return message;
}

std::vector<uint8_t> GB32960MessageData::generateLogoutMessageSample() {
    // 创建车辆登出消息对象
    uint16_t sn = mVehicleloginMessageSample.getserialNumber();
    VehicleLogoutMessage vehiclelogoutMessageSample(sn);
    // 生成数据包并打印消息内容
    std::vector<uint8_t> vehiclelogoutMessageSampledataPacket = vehiclelogoutMessageSample.generateDataPacket();
    GB32960MessageData vehiclelogoutGBmessageSample(0x04, 0xFE, mUniqueID, 0x01,
                                             vehiclelogoutMessageSampledataPacket);
    std::vector<uint8_t> message = vehiclelogoutGBmessageSample.generateDataPacket();
    //std::cout << "generateLoginMessage" << std::endl;
    LOG(INFO) << "generateLogoutMessageSample sn :" << sn;
    return message;
}

std::vector<uint8_t> GB32960MessageData::generateHeartbeatMessage() {
    // 生成车辆心跳的二进制信息
    std::vector<uint8_t> vehicleHeartbeatMessagedataPacket = {};
    GB32960MessageData vehicleHeartbeatGBmessage(0x07, 0xFE, mUniqueID, 0x01,
                                                 vehicleHeartbeatMessagedataPacket);
    std::vector<uint8_t> message = vehicleHeartbeatGBmessage.generateDataPacket();

    //std::cout << "generateHeartbeatMessage" << std::endl;
    LOG(INFO) << "generateHeartbeatMessage";
    return message;
}
std::vector<uint8_t> GB32960MessageData::generatetimeSynchronizationMessage() {
    // 生成tbox校正时间的二进制信息
    std::vector<uint8_t> vehicleHeartbeatMessagedataPacket = {};
    GB32960MessageData vehicleHeartbeatGBmessage(0x08, 0xFE, mUniqueID, 0x01,
                                                 vehicleHeartbeatMessagedataPacket);
    std::vector<uint8_t> message = vehicleHeartbeatGBmessage.generateDataPacket();

    LOG(INFO) << __func__;
    return message;
}

std::vector<uint8_t> GB32960MessageData::generate80ReportMessage(
        int idCount, const std::vector<uint8_t>& idList) {
    // 生成车辆心跳的二进制信息
    std::vector<uint8_t> msg;
    //    ParmReportMessage parmMsg;
    msg = mParmMsg.generateMessage(idCount, idList);
    GB32960MessageData vehiclemessage(0x80, 0x01, mUniqueID, 0x01, msg);
    std::vector<uint8_t> message = vehiclemessage.generateDataPacket();

    //std::cout << __func__ << std::endl;
    LOG(INFO) << __func__;
    return message;
}

std::vector<uint8_t> GB32960MessageData::generate81ReportMessage(
        int idCount, const std::vector<uint8_t>& idList) {
    // 生成车辆心跳的二进制信息
    std::vector<uint8_t> msg;
    msg = mParmMsg.generateMessage(idCount, idList);
    GB32960MessageData vehiclemessage(0x81, 0x01, mUniqueID, 0x01, msg);
    std::vector<uint8_t> message = vehiclemessage.generateDataPacket();
    return message;
}

std::vector<uint8_t> GB32960MessageData::generate81ReportMessage(const std::vector<uint8_t>& msg) {
    GB32960MessageData vehiclemessage(0x81, 0x01, mUniqueID, 0x01, msg);
    std::vector<uint8_t> message = vehiclemessage.generateDataPacket();
    return message;
}

std::vector<uint8_t> GB32960MessageData::generate82ReportMessage() {
    // 生成车辆心跳的二进制信息
    std::vector<uint8_t> msg = {0x00};
    remoteCtrlRespMessage remoteCtrlRespMessage;
    std::vector<uint8_t> remoteCtrlRespDataPacket = remoteCtrlRespMessage.generateDataPacket(msg);
    GB32960MessageData vehiclemessage(0x82, 0x01, mUniqueID, 0x01, remoteCtrlRespDataPacket);
    std::vector<uint8_t> message = vehiclemessage.generateDataPacket();
    return message;
}
std::vector<uint8_t> GB32960MessageData::controlMessageResponse(
        uint8_t type, std::vector<uint8_t> serviceCtrlResp) {
    remoteCtrlRespMessage remoteCtrlRespMessage;
    std::vector<uint8_t> remoteCtrlRespDataPacket =
            remoteCtrlRespMessage.generateDataPacket(serviceCtrlResp);
    GB32960MessageData ctrlRespMessage(type, 0x01, mUniqueID, 0x01, remoteCtrlRespDataPacket);
    std::vector<uint8_t> message = ctrlRespMessage.generateDataPacket();
    return message;
}
#ifdef EV48
std::vector<uint8_t> GB32960MessageData::generateRealtimeMessage(
        VehicleDataMessage& vehiclemessage, DriveMotorDataMessage& drivemotormessage,
        LocationDataMessage& locationdatamessage, ExtremeDataMessage& extremedatamessage,
        FaultDataMessage& faultdatamessage, BatteryVoltageMessage& batteryVoltageMessage,
        ProbeTemperatureMessage& probeTemperatureMessage, CustomInfoFlags& customInfoFlags,
        DataCollectionFlags& dataCollectionflags, VehicleStatusMessage& vehicleStatusMessage,
        DoorLockStatus& doorLockStatus, HavcStatus& havcStatus) {
    LOG(INFO) << "generateRealtimeMessage";
    RealTimeDataMessage realtimemessage;
    std::vector<uint8_t> vehiclemessagepack = vehiclemessage.generateDataPacket();
    std::ostringstream ss;
    for (const auto& byte : vehiclemessagepack) {
        ss << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(byte) << " ";
    }
    //LOG(INFO) << "Generated vehiclemessagepack Packet: ";
    //LOG(INFO) << ss.str();
    std::vector<uint8_t> drivemotormessagepack = drivemotormessage.generateDataPacket();
    ss.str("");
    for (const auto& byte : drivemotormessagepack) {
        ss << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(byte) << " ";
    }
    //LOG(INFO) << "Generated drivemotormessagepack Packet: ";
    //LOG(INFO) << ss.str();
    //LocationDataMessage locationdatamessage(0x00, 0x0000f006, 0x00006102);
    std::vector<uint8_t> locationdatamessagepack = locationdatamessage.generateDataPacket();
    std::vector<uint8_t> extremedatamessagepack = extremedatamessage.generateDataPacket();
    ss.str("");
    for (const auto& byte : extremedatamessagepack) {
        ss << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(byte) << " ";
    }
    //LOG(INFO) << "Generated extremedatamessagepack Packet: ";
    //LOG(INFO) << ss.str();
    std::vector<uint8_t> faultdatamessagepack = faultdatamessage.generateDataPacket();
    ss.str("");
    for (const auto& byte : faultdatamessagepack) {
        ss << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(byte) << " ";
    }
    //LOG(INFO) << "Generated faultdatamessagepack Packet: ";
    //LOG(INFO) << ss.str();
    std::vector<uint8_t> batteryVoltagemessagepack = batteryVoltageMessage.generateDataPacket();
    ss.str("");
    for (const auto& byte : batteryVoltagemessagepack) {
        ss << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(byte) << " ";
    }
    //LOG(INFO) << "Generated batteryVoltagemessagepack Packet: ";
    //LOG(INFO) << ss.str();
    std::vector<uint8_t> batteryTempmessagepack = probeTemperatureMessage.generateDataPacket();
    ss.str("");
    for (const auto& byte : batteryTempmessagepack) {
        ss << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(byte) << " ";
    }
    //LOG(INFO) << "Generated batteryTempmessagepack Packet: ";
    //LOG(INFO) << ss.str();
    std::vector<uint8_t> customInfoFlagsmessagepack = customInfoFlags.generateDataPacket();
    ss.str("");
    for (const auto& byte : customInfoFlagsmessagepack) {
        ss << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(byte) << " ";
    }
    //LOG(INFO) << "Generated customInfoFlagsmessagepack Packet: ";
    //LOG(INFO) << ss.str();
    std::vector<uint8_t> dataCollectionflagsmessagepack = dataCollectionflags.generateDataPacket();
    ss.str("");
    for (const auto& byte : dataCollectionflagsmessagepack) {
        ss << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(byte) << " ";
    }
    //LOG(INFO) << "Generated dataCollectionflagsmessagepack Packet: ";
    //LOG(INFO) << ss.str();
    std::vector<uint8_t> vehicleStatusMessagepack = vehicleStatusMessage.generateDataPacket();
    ss.str("");
    for (const auto& byte : vehicleStatusMessagepack) {
        ss << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(byte) << " ";
    }
    //LOG(INFO) << "Generated vehicleStatusMessagepack Packet: ";
    //LOG(INFO) << ss.str();
    std::vector<uint8_t> doorLockStatusMessagepack = doorLockStatus.generateDataPacket();
    ss.str("");
    for (const auto& byte : doorLockStatusMessagepack) {
        ss << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(byte) << " ";
    }
    //LOG(INFO) << "Generated doorLockStatusMessagepack Packet: ";
    //LOG(INFO) << ss.str();
    std::vector<uint8_t> havcStatusMessagepack = havcStatus.generateDataPacket();
    ss.str("");
    for (const auto& byte : havcStatusMessagepack) {
        ss << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(byte) << " ";
    }
    //LOG(INFO) << "Generated havcStatusMessagepack Packet: ";
    //LOG(INFO) << ss.str();

    // 生成实时数据
    realtimemessage.addInfoPacket(vehiclemessagepack);
    realtimemessage.addInfoPacket(drivemotormessagepack);
    realtimemessage.addInfoPacket(locationdatamessagepack);
    realtimemessage.addInfoPacket(extremedatamessagepack);
    realtimemessage.addInfoPacket(faultdatamessagepack);
    realtimemessage.addInfoPacket(batteryVoltagemessagepack);
    realtimemessage.addInfoPacket(batteryTempmessagepack);
    //GB32960.3 TEST start
    char propValue[PROPERTY_VALUE_MAX_LEN] = "\0";
    std::string testMode;
    if(property_get("persist.gecko.tbox.testmode", propValue, nullptr)!=0){
        testMode = propValue;
        if(testMode != "1"){
            realtimemessage.addInfoPacket(customInfoFlagsmessagepack);
            realtimemessage.addInfoPacket(dataCollectionflagsmessagepack);
            realtimemessage.addInfoPacket(vehicleStatusMessagepack);
            realtimemessage.addInfoPacket(doorLockStatusMessagepack);
            realtimemessage.addInfoPacket(havcStatusMessagepack);
            LOG(INFO) << "not GB32960_3_TEST ";
        } else {
            LOG(INFO) << "GB32960_3_TEST ";
        }
    } else {
        realtimemessage.addInfoPacket(customInfoFlagsmessagepack);
        realtimemessage.addInfoPacket(dataCollectionflagsmessagepack);
        realtimemessage.addInfoPacket(vehicleStatusMessagepack);
        realtimemessage.addInfoPacket(doorLockStatusMessagepack);
        realtimemessage.addInfoPacket(havcStatusMessagepack);
        LOG(INFO) << "not GB32960_3_TEST 2";
    }
    //GB32960.3 TEST end
    std::vector<uint8_t> realtimemessagepack = realtimemessage.generateDataPacket();

    // 生成GB报文
    GB32960MessageData realtimeGBmessage(0x02, 0xFE, mUniqueID, 0x01, realtimemessagepack);
    std::vector<uint8_t> message = realtimeGBmessage.generateDataPacket();

    GB32960MessageData additionRealtimeGBmessage(0x03, 0xFE, mUniqueID, 0x01, realtimemessagepack);
    std::vector<uint8_t> addtionMessage = additionRealtimeGBmessage.generateDataPacket();
    mAddtionMessage.resize(0);
    mAddtionMessage.assign(addtionMessage.begin(), addtionMessage.end());
    return message;
}
#endif
std::vector<uint8_t> GB32960MessageData::generateME11RealtimeMessage(
        VehicleDataMessage& vehiclemessage, DriveMotorDataMessage& drivemotormessage,
        LocationDataMessage& locationdatamessage, ExtremeDataMessage& extremedatamessage,
        FaultDataMessage& faultdatamessage, BatteryVoltageMessage& batteryVoltageMessage,
        ProbeTemperatureMessage& probeTemperatureMessage, FrontCamInfoMessage& frontCamInfoMessage,
        IntegratedSwitchInfoMessage& intgSwitchInfoMessage, TerminalSysInfoMessage& terminalSysInfoMessage,
        RadioSysInfoMessage& radioSysInfoMessage, ReverseAssistantSysInfoMessage& reverseAssistantSysInfoMessage,
        ChassisSysInfoMessage& chassisSysInfoMessage, PowerSysInfoMessage& powerSysInfoMessage,
        BatterySysInfoMessage& batterySysInfoMessage, BatteryChargingInfoMessage& batteryChargingInfoMessage,
        ACSysInfoMessage& acSysInfoMessage, VehicleBodySysInfoMessage& vehicleBodySysInfoMessage,
        VehicleControllerInfoMessage& vehicleControllerInfoMessage) {
    LOG(INFO) << "generateME11RealtimeMessage";
    RealTimeDataMessage realtimemessage;
    std::vector<uint8_t> vehiclemessagepack = vehiclemessage.generateDataPacket();
    std::ostringstream ss;
    for (const auto& byte : vehiclemessagepack) {
        ss << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(byte) << " ";
    }
    //LOG(INFO) << "Generated vehiclemessagepack Packet: ";
    //LOG(INFO) << ss.str();
    std::vector<uint8_t> drivemotormessagepack = drivemotormessage.generateDataPacket();
    ss.str("");
    for (const auto& byte : drivemotormessagepack) {
        ss << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(byte) << " ";
    }
    //LOG(INFO) << "Generated drivemotormessagepack Packet: ";
    //LOG(INFO) << ss.str();
    //LocationDataMessage locationdatamessage(0x00, 0x0000f006, 0x00006102);
    std::vector<uint8_t> locationdatamessagepack = locationdatamessage.generateDataPacket();
    std::vector<uint8_t> extremedatamessagepack = extremedatamessage.generateDataPacket();
    ss.str("");
    for (const auto& byte : extremedatamessagepack) {
        ss << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(byte) << " ";
    }
    //LOG(INFO) << "Generated extremedatamessagepack Packet: ";
    //LOG(INFO) << ss.str();
    std::vector<uint8_t> faultdatamessagepack = faultdatamessage.generateDataPacket();
    ss.str("");
    for (const auto& byte : faultdatamessagepack) {
        ss << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(byte) << " ";
    }
    //LOG(INFO) << "Generated faultdatamessagepack Packet: ";
    //LOG(INFO) << ss.str();
    std::vector<uint8_t> batteryVoltagemessagepack = batteryVoltageMessage.generateDataPacket();
    ss.str("");
    for (const auto& byte : batteryVoltagemessagepack) {
        ss << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(byte) << " ";
    }
    //LOG(INFO) << "Generated batteryVoltagemessagepack Packet: ";
    //LOG(INFO) << ss.str();
    std::vector<uint8_t> batteryTempmessagepack = probeTemperatureMessage.generateDataPacket();
    ss.str("");
    for (const auto& byte : batteryTempmessagepack) {
        ss << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(byte) << " ";
    }
    //LOG(INFO) << "Generated batteryTempmessagepack Packet: ";
    //LOG(INFO) << ss.str();
    std::vector<uint8_t> frontCamInfomessagepack = frontCamInfoMessage.generateDataPacket();
    ss.str("");
    for (const auto& byte : frontCamInfomessagepack) {
        ss << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(byte) << " ";
    }
    LOG(INFO) << "Generated frontCamInfomessagepack Packet: ";
    LOG(INFO) << ss.str();

    std::vector<uint8_t> intgSwitchInfomessagepack = intgSwitchInfoMessage.generateDataPacket();
    ss.str("");
    for (const auto& byte : intgSwitchInfomessagepack) {
        ss << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(byte) << " ";
    }

    std::vector<uint8_t> terminalSysInfomessagepack = terminalSysInfoMessage.generateDataPacket();
    ss.str("");
    for (const auto& byte : terminalSysInfomessagepack) {
        ss << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(byte) << " ";
    }

    std::vector<uint8_t> radioSysInfomessagepack = radioSysInfoMessage.generateDataPacket();
    ss.str("");
    for (const auto& byte : radioSysInfomessagepack) {
        ss << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(byte) << " ";
    }

    std::vector<uint8_t> reverseAssistantSysInfomessagepack = reverseAssistantSysInfoMessage.generateDataPacket();
    ss.str("");
    for (const auto& byte : reverseAssistantSysInfomessagepack) {
        ss << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(byte) << " ";
    }

    std::vector<uint8_t> chassisSysInfomessagepack = chassisSysInfoMessage.generateDataPacket();
    ss.str("");
    for (const auto& byte : chassisSysInfomessagepack) {
        ss << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(byte) << " ";
    }

    std::vector<uint8_t> powerSysInfomessagepack = powerSysInfoMessage.generateDataPacket();
    ss.str("");
    for (const auto& byte : powerSysInfomessagepack) {
        ss << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(byte) << " ";
    }

    std::vector<uint8_t> batterySysInfomessagepack = batterySysInfoMessage.generateDataPacket();
    ss.str("");
    for (const auto& byte : batterySysInfomessagepack) {
        ss << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(byte) << " ";
    }

    std::vector<uint8_t> batteryChargingInfomessagepack = batteryChargingInfoMessage.generateDataPacket();
    ss.str("");
    for (const auto& byte : batteryChargingInfomessagepack) {
        ss << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(byte) << " ";
    }

    std::vector<uint8_t> acSysInfomessagepack = acSysInfoMessage.generateDataPacket();
    ss.str("");
    for (const auto& byte : acSysInfomessagepack) {
        ss << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(byte) << " ";
    }

    std::vector<uint8_t> vehicleBodySysInfomessagepack = vehicleBodySysInfoMessage.generateDataPacket();
    ss.str("");
    for (const auto& byte : vehicleBodySysInfomessagepack) {
        ss << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(byte) << " ";
    }

    std::vector<uint8_t> vehicleControllerInfomessagepack = vehicleControllerInfoMessage.generateDataPacket();
    ss.str("");
    for (const auto& byte : vehicleControllerInfomessagepack) {
        ss << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(byte) << " ";
    }

    // 生成实时数据
    realtimemessage.addInfoPacket(vehiclemessagepack);
    realtimemessage.addInfoPacket(drivemotormessagepack);
    realtimemessage.addInfoPacket(locationdatamessagepack);
    realtimemessage.addInfoPacket(extremedatamessagepack);
    realtimemessage.addInfoPacket(faultdatamessagepack);
    realtimemessage.addInfoPacket(batteryVoltagemessagepack);
    realtimemessage.addInfoPacket(batteryTempmessagepack);
    //GB32960.3 TEST start
    char propValue[PROPERTY_VALUE_MAX_LEN] = "\0";
    std::string testMode;
    if(property_get("persist.gecko.tbox.testmode", propValue, nullptr)!=0){
        testMode = propValue;
        if(testMode != "1"){
            realtimemessage.addInfoPacket(frontCamInfomessagepack);
            realtimemessage.addInfoPacket(intgSwitchInfomessagepack);
            realtimemessage.addInfoPacket(terminalSysInfomessagepack);
            realtimemessage.addInfoPacket(radioSysInfomessagepack);
            realtimemessage.addInfoPacket(reverseAssistantSysInfomessagepack);
            realtimemessage.addInfoPacket(chassisSysInfomessagepack);
            realtimemessage.addInfoPacket(powerSysInfomessagepack);
            realtimemessage.addInfoPacket(batterySysInfomessagepack);
            realtimemessage.addInfoPacket(batteryChargingInfomessagepack);
            realtimemessage.addInfoPacket(acSysInfomessagepack);
            realtimemessage.addInfoPacket(vehicleBodySysInfomessagepack);
            realtimemessage.addInfoPacket(vehicleControllerInfomessagepack);
            LOG(INFO) << "not GB32960_3_TEST ";
        } else {
            LOG(INFO) << "GB32960_3_TEST ";
        }
    } else {
        realtimemessage.addInfoPacket(frontCamInfomessagepack);
        realtimemessage.addInfoPacket(intgSwitchInfomessagepack);
        realtimemessage.addInfoPacket(terminalSysInfomessagepack);
        realtimemessage.addInfoPacket(radioSysInfomessagepack);
        realtimemessage.addInfoPacket(reverseAssistantSysInfomessagepack);
        realtimemessage.addInfoPacket(chassisSysInfomessagepack);
        realtimemessage.addInfoPacket(powerSysInfomessagepack);
        realtimemessage.addInfoPacket(batterySysInfomessagepack);
        realtimemessage.addInfoPacket(batteryChargingInfomessagepack);
        realtimemessage.addInfoPacket(acSysInfomessagepack);
        realtimemessage.addInfoPacket(vehicleBodySysInfomessagepack);
        realtimemessage.addInfoPacket(vehicleControllerInfomessagepack);
        LOG(INFO) << "not GB32960_3_TEST 2";
    }
    //GB32960.3 TEST end
    std::vector<uint8_t> realtimemessagepack = realtimemessage.generateDataPacket();

    // 生成GB报文
    GB32960MessageData realtimeGBmessage(0x02, 0xFE, mUniqueID, 0x01, realtimemessagepack);
    std::vector<uint8_t> message = realtimeGBmessage.generateDataPacket();

    GB32960MessageData additionRealtimeGBmessage(0x03, 0xFE, mUniqueID, 0x01, realtimemessagepack);
    std::vector<uint8_t> addtionMessage = additionRealtimeGBmessage.generateDataPacket();
    mAddtionMessage.resize(0);
    mAddtionMessage.assign(addtionMessage.begin(), addtionMessage.end());
    return message;
}

std::vector<uint8_t> GB32960MessageData::changeMessageID(uint8_t MSGID, const std::vector<uint8_t>& message){
    std::vector<uint8_t> desMessage;
    desMessage.resize(0);
    uint8_t command = MSGID;
    uint8_t responseFlag = message[3];
    uint8_t encryptionMethod = 0x01;
    dataLength = (message[22] << 8) | message[23];
    if(message.size() != (25+dataLength)) {
        LOG(INFO) << __func__ << " modify Msg ID failed, data length mismatched";
        return desMessage;
    }
    std::vector<uint8_t> msgPackage;
    msgPackage.assign(message.begin()+24, message.end()-1);
    GB32960MessageData realtimeGBmessage(command, responseFlag, mUniqueID, encryptionMethod, msgPackage);
    desMessage = realtimeGBmessage.generateDataPacket();
    return desMessage;
}

/*void GB32960MessageData::saveLoginSN(){
    mVehicleloginMessage.saveLoginSN();
}*/

std::vector<uint8_t> GB32960MessageData::generateVehicleInfoMessage() {
    std::vector<uint8_t> vehicleInfoMessagedataPacket = mVehicleInfoMessage.generateDataPacket(mUniqueID);
    GB32960MessageData vehicleInfoGBmessage(0x1B, 0xFE, mUniqueID, 0x01,
                                             vehicleInfoMessagedataPacket);
    std::vector<uint8_t> message = vehicleInfoGBmessage.generateDataPacket();
    LOG(INFO) << "generatevehicleInfoMessage";
    return message;
}

std::vector<uint8_t> GB32960MessageData::generateRealNameAuthMessage() {
    std::vector<uint8_t> realNameAuthMessagedataPacket = mRealNameAuthMessage.generateDataPacket();
    GB32960MessageData realNameAuthGBmessage(0x21, 0xFE, mUniqueID, 0x01,
                                                 realNameAuthMessagedataPacket);
    std::vector<uint8_t> message = realNameAuthGBmessage.generateDataPacket();
    LOG(INFO) << "generateRealNameAuthMessage";
    return message;
}