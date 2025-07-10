#ifndef VEHICLEDATAMESSAGE_H
#define VEHICLEDATAMESSAGE_H

#include "RealTimeMessageBase.h"

class VehicleDataMessage : public RealTimeMessageBase{
  public:
    using RealTimeMessageBase::RealTimeMessageBase;

    std::vector<uint8_t> generateDataPacket();
    void printMessage();

    uint32_t getvehicleStatusValue();
    uint32_t getchargingStatusValue();
    uint32_t getvehicleSpeedValue();
    uint32_t getaccumulatedMileageValue();
    uint32_t gettotalVoltageValue();
    uint32_t gettotalCurrentValue();
    uint32_t getstateOfChargeValue();
    uint32_t getdcDcStatusValue();
    uint32_t getgearvalueValue();
    uint32_t getparkvalueValue();
    uint32_t getinsulationResistanceValue();
    uint32_t getacceleratorPedalPositionValue();
    uint32_t getbrakePedalStatusValue();
    uint32_t getbrakePedalPosValue();
    uint32_t getbrakePedalVldValue();



  private:
    uint8_t vehicleid = 0x01;


  public:
    uint8_t vehicleStatus;
    uint8_t chargingStatus;
    uint8_t runningMode;
    uint16_t vehicleSpeed;
    uint32_t accumulatedMileage;
    uint16_t totalVoltage = 0;
    uint16_t totalCurrent = 0;
    uint8_t stateOfCharge;
    uint8_t dcDcStatus;
    uint8_t gear;
    uint16_t insulationResistance = 0;
    uint8_t acceleratorPedalPosition;
    uint8_t brakePedalStatus;
};

#endif  // VEHICLEDATAMESSAGE_H
