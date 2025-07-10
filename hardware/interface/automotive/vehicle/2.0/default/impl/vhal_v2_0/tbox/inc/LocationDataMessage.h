#ifndef LOCATIONDATAMESSAGE_H
#define LOCATIONDATAMESSAGE_H

#include <cstdint>
#include <vector>
#include <android-base/logging.h>
#include <cutils/properties.h>

class LocationDataMessage {
  public:
    LocationDataMessage(uint8_t locationStatus, double longitude, double latitude);
    LocationDataMessage();

    std::vector<uint8_t> generateDataPacket();
    void printMessage();

  private:
    const uint8_t locationdataid = 0x05;
    uint8_t locationStatus;
    double longitude;
    double latitude;
};

#endif  // LOCATIONDATAMESSAGE_H
