#include "DataSaver.h"
#include <fstream>
#include <iomanip>
#include <stdexcept>
#include "MessageDefine.h"

void DataSaver::saveToFile(BusIdentifier& busIdentifier, const std::vector<uint8_t>& data) {
    const std::string& filename;
    switch(busIdentifier){
        case BODY_CAN:
            filename = "/data/data/Body_CAN.txt";
        break;
        case EV_CAN:
            filename = "/data/data/EVCAN.txt";
        break;
        case CH_CAN:
            filename = "/data/data/Chassis_CAN.txt";
        break;
        case MCU:
        break;
        default:
        break;

    }
    if(filename=="")
        return;
    std::ofstream file(filename, std::ios::app);
    if (!file) {
        throw std::runtime_error("Cannot open file for writing: " + filename);
    }

    for (const auto& byte : data) {
        file << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
    }
    file << "\n";
}
