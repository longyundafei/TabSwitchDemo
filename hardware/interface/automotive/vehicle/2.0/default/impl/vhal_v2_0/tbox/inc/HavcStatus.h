#ifndef HAVCSTATUS_H
#define HAVCSTATUS_H

#include "RealTimeMessageBase.h"

class HavcStatus : public RealTimeMessageBase{
  public:
    using RealTimeMessageBase::RealTimeMessageBase;

    std::vector<uint8_t> generateDataPacket();
    int getair_conditioning_switchValue();
    int getcirculation_modeValue();
    int getair_outlet_modeValue();
    int getcooling_switchValue();
    int getheating_switchValue();
    int getdefrosting_switchValue();
    int getfan_speed_adjustmentValue();
    int gettemperature_adjustmentValue();
    void printMessage();

  private:
    uint8_t HavcStatusId = 0x92;
    uint8_t air_conditioning_switch;   //空调开关
    uint8_t circulation_mode;          //循环模式
    uint8_t air_outlet_mode;           //出风模式
    uint8_t cooling_switch;            //制冷开关
    uint8_t heating_switch;            //制热开关
    uint8_t defrosting_switch;         //除霜开关
    uint8_t fan_speed_adjustment;      //风量调节
    uint8_t temperature_adjustment;    //温度调节
    const uint8_t ON = 2;
    const uint8_t OFF = 1;
    const uint8_t INVALID = 0xff;
};
#endif