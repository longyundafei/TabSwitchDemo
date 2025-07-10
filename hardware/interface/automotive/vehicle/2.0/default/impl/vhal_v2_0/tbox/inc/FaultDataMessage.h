#ifndef FAULTDATAMESSAGE_H
#define FAULTDATAMESSAGE_H

#include "RealTimeMessageBase.h"

class FaultDataMessage  : public RealTimeMessageBase{
  public:
    using RealTimeMessageBase::RealTimeMessageBase;

    std::vector<uint8_t> generateDataPacket();
    void printMessage();
    int getmaxAlarmLevelValue();
    int gettemperatureDifferenceAlarmValue();
    int getbatteryHighTemperatureAlarmValue();
    int getvehicleEnergyStorageDeviceOvervoltageAlarmValue();
    int getvehicleEnergyStorageDeviceUndervoltageAlarmValue();
    int getsocLowAlarmValue();
    int getsingleCellBatteryOvervoltageAlarmValue();
    int getsingleCellBatteryUndervoltageAlarmValue();
    int getsocHighAlarmValue();
    int getsocJumpAlarmValue();
    int getrechargeableEnergyStorageSystemMismatchedAlarmValue();
    int getbatterySingleConsistencyPoorAlarmValue();
    int getinsulationAlarmValue();
    int getdcDcTemperatureAlarmValue();
    int getbrakeSystemAlarmValue();
    int getldcDcStatusAlarmValue();
    int getmotorControllerTemperatureAlarmValue();
    int gethighVoltageInterlockStateAlarmValue();
    int getmotorTemperatureAlarmValue();
    int getvehicleEnergyStorageDeviceOverchargeAlarmValue();
    int getrechargeableFaultCountValue();
    int getrechargeableFaultCodesValue();
    int getdriveMotorFaultCountValue();
    int getdriveMotorFaultCodesValue();
#ifdef EV48
    int getOtherFaultCodesValue();
#endif
    void createWarnData(uint32_t& data, int flag, int bitaddr);
  public:
    uint8_t maxAlarmLevel;
    uint8_t rechargeableFaultCount;
    std::vector<uint32_t> rechargeableFaultCodes;
    uint8_t driveMotorFaultCount;
    std::vector<uint32_t> driveMotorFaultCodes;
    uint8_t engineFaultCount;
    std::vector<uint32_t> engineFaultCodes;
    uint8_t otherFaultCount;
    std::vector<uint32_t> otherFaultCodes;

  private:
    uint8_t faultDataID = 0x07;
    uint32_t generalAlarmFlags;

};

#endif  // FAULTDATAMESSAGE_H
