#include "MessageHandler.h"
#include "ev_can_ccm_4_0.h"
#include "MessageParser.h"
#include "FunctionMap.h"

enum class MessageType : uint8_t {
    CAN_REPORT = 0x01,         // CAN报文上报，上行
    CAN_DOWNLINK = 0x02,       // SOC下发CAN报文，下发
    MCU_UPGRADE_ENTER = 0x03,  // MCU升级进入，下行
    MCU_UPGRADE_DATA = 0x04,   // MCU升级数据包，下行
    MCU_UPGRADE_FINISH = 0x05  // MCU升级结束，下行
};
MessageHandler:: MessageHandler(){
    functionMap.addparam();
}
FunctionMap& MessageHandler::getfunctionMap(){
    return functionMap;
}
void MessageHandler::handleMessage(const std::vector<uint8_t>& message) {
    // 处理接收到的二进制信息
    std::vector<uint8_t> data;
    uint8_t command, responseFlag;
    BusIdentifier busIdentifier;
    std::vector<uint8_t> messageIdentifier;
    std::vector<uint8_t> dataContent;
    uint16_t hexValue = 0;
    MessageParser mprase;
//    propmanager.importPropertiesFromFile("properties.txt");
    mprase.parseDataPacket(message, data, command, responseFlag);
    MessageType messageType = static_cast<MessageType>(command);
    switch(messageType){
        case MessageType::CAN_REPORT:
            std::cout << " MessageType::CAN_REPORT " << std::endl;
            mprase.parseCANMessage(data,busIdentifier,messageIdentifier,dataContent);
            if (messageIdentifier.size() == 2) {
//                hexValue = (messageIdentifier[0] << 8) | messageIdentifier[1];
                hexValue = (messageIdentifier[1] << 8) | messageIdentifier[0];
            }
            std::cout << " can id is: " << hexValue << std::endl;
            functionMap.callFunction(hexValue,dataContent);
            break;
        case MessageType::CAN_DOWNLINK:
            break;
        case MessageType::MCU_UPGRADE_DATA:
            break;
        case MessageType::MCU_UPGRADE_FINISH:
            break;
        default:
            break;
    }
    // 调用MessageProducer的方法示例
//    mp.sendMessage("Processed message");
}

