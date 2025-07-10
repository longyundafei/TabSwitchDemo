#ifndef REAL_NAME_AUTH_MESSAGE_H
#define REAL_NAME_AUTH_MESSAGE_H

#include <bitset>
#include <iomanip>
#include <string>
#include <vector>
#include <android-base/logging.h>


class RealNameAuthMessage {
  private:
    uint8_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;

  public:
    RealNameAuthMessage();
    std::vector<uint8_t> generateDataPacket();

};

#endif  // REAL_NAME_AUTH_MESSAGE_H