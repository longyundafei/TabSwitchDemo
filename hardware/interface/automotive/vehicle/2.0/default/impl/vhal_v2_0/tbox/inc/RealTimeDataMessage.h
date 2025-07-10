#ifndef REAL_TIME_DATA_MESSAGE_H
#define REAL_TIME_DATA_MESSAGE_H

#include <iostream>
#include <vector>
#include <android-base/logging.h>

class RealTimeDataMessage {
  public:
    RealTimeDataMessage();
    void addInfoPacket(const std::vector<uint8_t>& infoPacket);
    std::vector<uint8_t> generateDataPacket();
    void printMessage();

  private:
    uint8_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    //    std::vector<uint8_t> timestamp;
    std::vector<uint8_t> infoList;
};

#endif  // REAL_TIME_DATA_MESSAGE_H
