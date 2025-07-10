#ifndef MESSAGEPARSER_H
#define MESSAGEPARSER_H

#include <vector>
#include <cstdint>
enum BusIdentifier {
    BODY_CAN = 0x01,
    EV_CAN = 0x02,
    CH_CAN = 0x03
};
class MessageParser {
public:
    static void parseDataPacket(const std::vector<uint8_t>& packet, std::vector<uint8_t>& data, uint8_t& command, uint8_t& responseFlag);
    std::vector<uint8_t> constructCANMessage(BusIdentifier busIdentifier, const std::vector<uint8_t>& messageIdentifier, const std::vector<uint8_t>& dataContent);
    void parseCANMessage(const std::vector<uint8_t>& message, BusIdentifier& busIdentifier, std::vector<uint8_t>& messageIdentifier, std::vector<uint8_t>& dataContent);

private:
    static bool validateChecksum(const std::vector<uint8_t>& packet);
};

#endif // MESSAGEPARSER_H

