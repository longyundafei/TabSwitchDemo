#ifndef DRIVEMOTORDATAMESSAGE_H
#define DRIVEMOTORDATAMESSAGE_H

#include "RealTimeMessageBase.h"

class DriveMotorDataMessage : public RealTimeMessageBase{
  public:
    using RealTimeMessageBase::RealTimeMessageBase;
    DriveMotorDataMessage(LockMap<int32_t, TboxDataDeclaration>& tboxdecl,VehicleController& vehicle);


    std::vector<uint8_t> generateDataPacket();
    void printMessage();

    uint32_t getmotorStatusValue();
    uint32_t getcontrollerTemperatureValue();
    uint32_t getmotorSpeedValue();
    uint32_t getmotorTorqueValue();
    uint32_t getmotorTemperatureValue();
    uint32_t getcontrollerInputVoltageValue();
    uint32_t getcontrollerBusCurrentValue();

  private:
    const uint8_t drivemotorid = 0x02;
    struct MotorData {
        uint8_t motorNumber;
        uint8_t motorStatus;
        uint8_t controllerTemperature = 0;
        uint16_t motorSpeed;
        uint16_t motorTorque;
        uint8_t motorTemperature;
        uint16_t controllerInputVoltage;
        uint16_t controllerBusCurrent;
    };

    uint8_t motorCount = 0x01;
    std::vector<MotorData> motorDataList;
};

#endif  // DRIVEMOTORDATAMESSAGE_H
