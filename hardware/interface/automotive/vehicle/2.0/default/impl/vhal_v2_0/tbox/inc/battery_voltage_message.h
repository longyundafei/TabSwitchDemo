#ifndef BATTERY_VOLTAGE_MESSAGE_H
#define BATTERY_VOLTAGE_MESSAGE_H

#include "RealTimeMessageBase.h"

class BatteryVoltageMessage : public RealTimeMessageBase{
  public:
    using RealTimeMessageBase::RealTimeMessageBase;

    std::vector<uint8_t> generateDataPacket();
    void printMessage();
    int getsystemVoltagesValue();
    int getsystemCurrentsValue();
    int getframeCellCountValue();
    int getvoltage1Value();
    int getvoltage2Value();
    int getvoltage3Value();
    int gettotalCurrentValue(int propId);
  public:
    std::vector<uint16_t> mCellVoltages;

  private:
    uint8_t batteryVoltageid = 0x08;
    uint8_t systemCount;
    uint8_t systemNumbers;
    uint16_t systemVoltages = 0x00;
    uint16_t systemCurrents;
    uint16_t totalCellCount;
    uint16_t startCellIndex;
    uint8_t frameCellCount;
};

#endif  // BATTERY_VOLTAGE_MESSAGE_H
