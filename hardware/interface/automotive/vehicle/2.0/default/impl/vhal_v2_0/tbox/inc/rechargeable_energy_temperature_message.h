#ifndef RECHARGEABLE_ENERGY_TEMPERATURE_MESSAGE_H
#define RECHARGEABLE_ENERGY_TEMPERATURE_MESSAGE_H

#include "RealTimeMessageBase.h"

class ProbeTemperatureMessage : public RealTimeMessageBase{
  public:
    using RealTimeMessageBase::RealTimeMessageBase;

    std::vector<uint8_t> generateDataPacket();
    void printMessage();
    int getprobeCountsValue();
    int gettemperatureFrameNo();
    int gettemperature1Value();
    int gettemperature2Value();
    int gettemperature3Value();
    int gettemperature4Value();
    int gettemperature5Value();
    int gettemperature6Value();
#ifdef EV48
    int gettemperature7Value();
#endif
  public:
    std::vector<uint8_t> probeTemperatures;

  private:
    uint8_t probeTemid = 0x09;
    uint8_t systemCount;
    uint8_t systemNumbers;
    uint16_t probeCounts;
};

#endif  // RECHARGEABLE_ENERGY_TEMPERATURE_MESSAGE_H
