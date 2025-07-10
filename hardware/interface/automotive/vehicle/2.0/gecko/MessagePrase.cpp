#include "MessageParser.h"
#include <iostream>
#include <iomanip>
#include <bitset>

void MessageParser::parseDataPacket(const std::vector<uint8_t>& packet, std::vector<uint8_t>& data, uint8_t& command, uint8_t& responseFlag) {
    // 检查报文长度是否符合要求
    if (packet.size() < 6) {
        std::cerr << "Invalid data packet. Packet size is less than expected." << std::endl;
        return;
    }

    // 检查起始符
    if (packet[0] != 0x02) {
        std::cerr << "Invalid data packet. Start marker is incorrect." << std::endl;
        return;
    }

    // 检查校验和
    if (!validateChecksum(packet)) {
        std::cerr << "Invalid data packet. Checksum is incorrect." << std::endl;
        return;
    }

    // 获取命令单元和应答标志
    command = packet[1];
    responseFlag = packet[2];

    // 获取数据单元长度
    uint16_t dataLength = (packet[3] << 8) | packet[4];

    // 检查数据单元长度是否符合要求
    if (packet.size() < 6 + dataLength) {
        std::cerr << "Invalid data packet. Data length does not match the packet size." << std::endl;
        return;
    }

    // 提取数据单元
    data.assign(packet.begin() + 5, packet.begin() + 5 + dataLength);
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


std::vector<uint8_t> MessageParser::constructCANMessage(BusIdentifier busIdentifier, const std::vector<uint8_t>& messageIdentifier, const std::vector<uint8_t>& dataContent) {
    std::vector<uint8_t> message;
    message.push_back(static_cast<uint8_t>(busIdentifier));
    message.insert(message.end(), messageIdentifier.begin(), messageIdentifier.end());
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
