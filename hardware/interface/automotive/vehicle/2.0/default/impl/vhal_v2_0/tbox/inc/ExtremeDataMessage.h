#ifndef EXTREMEDATAMESSAGE_H
#define EXTREMEDATAMESSAGE_H

#include "RealTimeMessageBase.h"

class ExtremeDataMessage : public RealTimeMessageBase{
  public:
    using RealTimeMessageBase::RealTimeMessageBase;

    std::vector<uint8_t> generateDataPacket();
    void printMessage();
#ifdef EV48
    int getmaxVoltageBatterySystemValue();
    int getMinVoltageBatterySystemValue();
    int getMaxTemperatureSubsystemValue();
    int getMinTemperatureSubsystemValue();
#endif
    int getmaxVoltageBatteryModuleValue();
    int getmaxVoltageValue();
    int getMinVoltageBatteryModuleValue();
    int getMinVoltageValue();
    int getMaxTemperatureProbeValue();
    int getMaxTemperatureValue();
    int getMinTemperatureProbeValue();
    int getMinTemperatureValue();


  private:
    const uint8_t extremedataid = 0x06;
    uint8_t maxVoltageBatterySystem = 0x01;
    uint8_t maxVoltageBatteryModule = 0x01;
    double maxVoltage;
    uint8_t minVoltageBatterySystem = 0x01;
    uint8_t minVoltageBatteryModule = 0x01;
    double minVoltage;
    uint8_t maxTemperatureSubsystem = 0x01;
    uint8_t maxTemperatureProbe = 0x01;
    uint8_t maxTemperature;
    uint8_t minTemperatureSubsystem = 0x01;
    uint8_t minTemperatureProbe = 0x01;
    uint8_t minTemperature;
};

#endif  // EXTREMEDATAMESSAGE_H
