#ifndef VEHICLE_LOGIN_MESSAGE_H
#define VEHICLE_LOGIN_MESSAGE_H

#include <bitset>
#include <iomanip>
#include <string>
#include <vector>
#include <android-base/logging.h>
#include <fstream>
#include <sstream>
#include <sys/stat.h>

class VehicleLoginMessage {
  private:
    uint8_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    uint16_t serialNumber;
    std::string iccId;
    uint8_t rechargeableEnergySubsystemCount;
    uint8_t rechargeableEnergySystemCodeLength;
    std::vector<uint8_t> rechargeableEnergySystemCodes;

  public:
    VehicleLoginMessage(const std::string& iccId, uint8_t rechargeableEnergySubsystemCount,
                        uint8_t rechargeableEnergySystemCodeLength,
                        const std::vector<uint8_t>& rechargeableEnergySystemCodes);

    std::vector<uint8_t> generateDataPacket();
    void printMessage();
    uint16_t getserialNumber() { return serialNumber; };
    uint16_t generateSerialNumber();
    void saveLoginSN();

  private:
    std::vector<uint8_t> stringToBinary(const std::string& str);
};

class VehicleLogoutMessage {
  private:
    uint8_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    uint16_t logoutSerialNumber;

  public:
    VehicleLogoutMessage(uint16_t logoutSerialNumber);
    std::vector<uint8_t> generateDataPacket();
};

class remoteCtrlRespMessage {
  private:
    uint8_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;

  public:
    remoteCtrlRespMessage();
    std::vector<uint8_t> generateDataPacket(std::vector<uint8_t> response);
};

#endif  // VEHICLE_LOGIN_MESSAGE_H
