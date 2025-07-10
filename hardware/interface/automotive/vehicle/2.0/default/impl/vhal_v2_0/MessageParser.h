#ifndef MESSAGEPARSER_H
#define MESSAGEPARSER_H

#include <vector>
#include <cstdint>

#include "MessageDefine.h"
#include "FunctionMap.h"
namespace android::hardware::automotive::vehicle::V2_0::impl {
class FunctionMap;
class MessageParser {
public:
    int parseDataPacket(const std::vector<uint8_t>& packet, std::vector<uint8_t>& data, uint8_t& command, uint8_t& responseFlag);
    void constructDataPacket(std::vector<uint8_t>& packet, const std::vector<uint8_t> data, uint8_t command, uint8_t responseFlag);
    std::vector<uint8_t> constructCANMessage(BusIdentifier busIdentifier, uint16_t messageID, const std::vector<uint8_t> dataContent);
    void parseCANMessage(const std::vector<uint8_t>& message, BusIdentifier& busIdentifier, std::vector<uint8_t>& messageIdentifier, std::vector<uint8_t>& dataContent);
    std::vector<uint8_t> makePack(uint16_t messageID, void* canData, BusIdentifier busIdentifier, uint8_t command, uint8_t responseFlag);
    std::vector<uint8_t> makePack(std::vector<uint8_t> msg,uint8_t command, uint8_t responseFlag);
    std::vector<uint8_t> makeTRANPack(uint16_t messageID, void* canData, BusIdentifier busIdentifier, uint8_t command, uint8_t responseFlag);

private:
    static bool validateChecksum(const std::vector<uint8_t>& packet);
    static uint8_t validateGetsum(const std::vector<uint8_t> packet);
    void recordData(BusIdentifier busIdentifier, const std::vector<uint8_t> packet);
    FunctionMap functionMap;
};
}
#endif // MESSAGEPARSER_H

