#ifndef VEHICLE_INFO_MESSAGE_H
#define VEHICLE_INFO_MESSAGE_H

#include <bitset>
#include <iomanip>
#include <string>
#include <vector>
#include <android-base/logging.h>
#include <cutils/properties.h>
#include <cmath>
#include "vehicle_controller.h"

class VehicleInfoMessage {
  public:
    uint8_t year = 0;
    uint8_t month = 0;
    uint8_t day = 0;
    uint8_t hour = 0;
    uint8_t minute = 0;
    uint8_t second = 0;
    std::string iccid;
    std::string imei;
    std::string imsi;
    std::string simn;
    double longitude = 0;
    double latitude = 0;

  public:
    VehicleInfoMessage(VehicleController& vehicle);

    std::vector<uint8_t> generateDataPacket(const std::string& uniqueID);

  private:
    std::vector<uint8_t> stringToBinary(const std::string& str);
    VehicleController mVehicle;
};

#endif  // VEHICLE_INFO_MESSAGE_H
