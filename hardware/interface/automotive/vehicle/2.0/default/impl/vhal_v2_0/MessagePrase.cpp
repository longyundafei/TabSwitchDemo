#include "MessageParser.h"
#include <iostream>
#include <iomanip>
#include <bitset>
#include <log/log.h>

#include "FunctionMap.h"
#include "RecordData.h"

namespace android::hardware::automotive::vehicle::V2_0::impl {
int MessageParser::parseDataPacket(const std::vector<uint8_t>& packet, std::vector<uint8_t>& data, uint8_t& command, uint8_t& responseFlag) {
    // 检查报文长度是否符合要求
    if (packet.size() < 6) {
        std::cerr << "Invalid data packet. Packet size is less than expected." << std::endl;
        ALOGE("Invalid data packet. Packet size is less than expected.");
        return -1;
    }

    // 检查起始符
    if (packet[0] != 0x02) {
//        std::cerr << "Invalid data packet. Start marker is incorrect." << std::endl;
//        ALOGE("Invalid data packet. Start marker is incorrect.");
        return -1;
    }

    // 检查校验和
    if (!validateChecksum(packet)) {
        std::cerr << "Invalid data packet. Checksum is incorrect." << std::endl;
        ALOGE("Invalid data packet. Checksum is incorrect.");
        return -1;
    }

    // 获取命令单元和应答标志
    command = packet[1];
    responseFlag = packet[2];

    // 获取数据单元长度
    uint16_t dataLength = (packet[4] << 8) | packet[3];

    // 检查数据单元长度是否符合要求
    if (packet.size() < 6 + dataLength) {
        std::cerr << "Invalid data packet. Data length does not match the packet size." << std::endl;
        ALOGE("Invalid data packet. Data length does not match the packet size.");
        return -1;
    }

    // 提取数据单元
    data.assign(packet.begin() + 5, packet.begin() + 5 + dataLength);
    return 0;
}

void MessageParser::constructDataPacket(std::vector<uint8_t>& packet, const std::vector<uint8_t> data, uint8_t command, uint8_t responseFlag)
{
    //检查报文长度
    if (data.size() < 8){
        std::cerr << "Invalid data packet. Packet size is less than expected." << std::endl;
        return;
    }

    //添加起始符
    packet.push_back(0x02);

    //添加command
    packet.push_back(command);

    //添加responseFlag
    packet.push_back(responseFlag);

    uint16_t data_len = data.size();
    uint8_t len[2] = { 0 };
    len[0] = data_len & 0xff;
    len[1] = data_len >> 8;
    //添加长度
    packet.push_back(len[0]);
    packet.push_back(len[1]);

    //添加报文数据
    packet.insert(packet.end(), data.begin(), data.end());

    //添加校验和
    uint8_t checksum = validateGetsum(packet);
    packet.push_back(checksum);
}

//bool MessageParser::validateChecksum(const std::vector<uint8_t>& packet) {
//    uint8_t checksum = 0;
//
//    // 异或校验
//    for (size_t i = 0; i < packet.size() - 1; i++) {
//        checksum ^= packet[i];
//    }
//
//    // 检查校验和是否正确
//    return (checksum == packet.back());
//}

bool MessageParser::validateChecksum(const std::vector<uint8_t>& packet) {
    uint8_t checksum = 0;

    // 异或校验
    for (size_t i = 0; i < packet.size() - 1; i++) {
        checksum ^= packet[i];
    }
    
    // 打印校验过程
    std::cout << "Checksum Validation: ";
    for (size_t i = 0; i < packet.size() - 1; i++) {
        std::cout << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(packet[i]) << " ^ ";
    }
    std::cout << " = " << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(checksum);
    std::cout << " (Expected: " << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(packet.back()) << ")";
    std::cout << std::endl;

    // 检查校验和是否正确
    return (checksum == packet.back());
}

uint8_t MessageParser::validateGetsum(const std::vector<uint8_t> packet) {
    uint8_t checksum = 0;

    // 异或校验
    for (size_t i = 0; i < packet.size(); i++) {
        checksum ^= packet[i];
    }

    // 打印校验过程
    std::cout << "Checksum Validation: ";
    for (size_t i = 0; i < packet.size(); i++) {
        std::cout << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(packet[i]) << " ^ ";
    }
    std::cout << " = " << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(checksum);
    std::cout << " (Expected: " << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(packet.back()) << ")";
    std::cout << std::endl;

    return checksum;
}

std::vector<uint8_t> MessageParser::constructCANMessage(BusIdentifier busIdentifier, uint16_t messageID, const std::vector<uint8_t> dataContent) {
    std::vector<uint8_t> message;

    message.push_back(static_cast<uint8_t>(busIdentifier));
    uint8_t messageIdentifier[2] = { 0 };
    messageIdentifier[0] = messageID & 0xff;
    messageIdentifier[1] = messageID >> 8;
    message.push_back(messageIdentifier[0]);
    message.push_back(messageIdentifier[1]);
    message.insert(message.end(), dataContent.begin(), dataContent.end());
    return message;
}

void MessageParser::parseCANMessage(const std::vector<uint8_t>& message, BusIdentifier& busIdentifier, std::vector<uint8_t>& messageIdentifier, std::vector<uint8_t>& dataContent) {
    if (message.size() >= 3) {
        busIdentifier = static_cast<BusIdentifier>(message[0]);
        messageIdentifier = {message[1], message[2]};
        if (message.size() > 3) {
            dataContent.assign(message.begin() + 3, message.end());
        }
    }
}
std::vector<uint8_t> MessageParser::makePack(std::vector<uint8_t> msg,uint8_t command, uint8_t responseFlag)
{
    //合成包头
    std::vector<uint8_t> packet;
    constructDataPacket(packet,msg,command,responseFlag);
//    std::string buf;
//    int size = packet.size();
//            for (int i = 0; i < size; i++) {
//                std::ostringstream ss;
//                ss.clear();
//                int data = packet[i];
//                ss << std::hex << data;
//                buf += "0x";
//                buf += ss.str();
//                buf += ", ";
//            }
//    ALOGI("---- %s", buf.c_str());
    return packet;
}

std::vector<uint8_t> MessageParser::makePack(uint16_t messageID, void* canData, BusIdentifier busIdentifier, uint8_t command, uint8_t responseFlag)
{
    //合成报文

    std::vector<uint8_t> data;
    if (busIdentifier == UDS_CAN ||busIdentifier == SOC_SET || busIdentifier == SOC_GET ) {
        data.assign((uint8_t*)canData, (uint8_t*)canData+8);
    }else {
        data = functionMap.packcallFunction(messageID, static_cast<void*>(canData), busIdentifier);
    }
    std::cout << "data:   ";
    for (size_t i = 0; i < data.size(); i++) {
        std::cout << std::hex << "0x" << static_cast<int>(data[i]) << " ";
    }
    std::cout << std::endl;

    //合成包头
    std::vector<uint8_t> packet;
    std::vector<uint8_t> canMessage;
    if(busIdentifier == SOC_SET || busIdentifier == SOC_GET) {
       canMessage.insert(canMessage.end(), data.begin(), data.end());
    } else {
       canMessage = constructCANMessage(busIdentifier, messageID, data);
    }
    constructDataPacket(packet, canMessage, command, responseFlag);
    //std::vector<uint8_t> data = {0x03,0x01,0x02,0x00,0x0B,0x02,0x50,0x02,0x08,0x00,0x3A,0x98,0x00,0x00,0x00,0x00,0xf0};

    //packet.push_back(0x0a);
    std::cout << "message:   ";
    for (size_t i = 0; i < packet.size(); i++) {
        std::cout << std::hex << static_cast<int>(packet[i]) << " ";
    }
    std::cout << std::endl;

    recordData(busIdentifier, packet);
    return packet;
}
std::vector<uint8_t> MessageParser::makeTRANPack(uint16_t messageID, void* canData, BusIdentifier busIdentifier, uint8_t command, uint8_t responseFlag)
{
    //合成报文
    std::vector<uint8_t> data;
    data.assign((uint8_t*)canData, (uint8_t*)canData+8);

    //合成包头
    std::vector<uint8_t> packet;
    std::vector<uint8_t> canMessage;
    canMessage = constructCANMessage(busIdentifier, messageID, data);
    constructDataPacket(packet, canMessage, command, responseFlag);
    //std::vector<uint8_t> data = {0x03,0x01,0x02,0x00,0x0B,0x02,0x50,0x02,0x08,0x00,0x3A,0x98,0x00,0x00,0x00,0x00,0xf0};

    //packet.push_back(0x0a);

    return packet;
}

void MessageParser::recordData(BusIdentifier busIdentifier, const std::vector<uint8_t> packet)
{
    switch(busIdentifier){
        case BusIdentifier::BODY_CAN:
            RecordData::getInstance()->recordBody_CAN(packet);
            break;
        case BusIdentifier::EV_CAN:
            RecordData::getInstance()->recordEVCAN(packet);
            break;
        case BusIdentifier::CH_CAN:
            RecordData::getInstance()->recordChassis_CAN(packet);
            break;
        case BusIdentifier::MCU:
            RecordData::getInstance()->recordMCU(packet);
            break;
        default:  ALOGE("recordData err, Wrong  BUS id:%d", static_cast<int>(busIdentifier));;
    }
}

}
