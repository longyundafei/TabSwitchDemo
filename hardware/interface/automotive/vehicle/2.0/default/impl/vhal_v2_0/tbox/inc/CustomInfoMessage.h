#ifndef CUSTOMINFOMESSAGE_H
#define CUSTOMINFOMESSAGE_H

#include <cstdint>
#include <vector>
#include "vehicle_controller.h"

class CustomInfoMessage {
  public:
    CustomInfoMessage();
    void updateCustomInfoMessage();
    std::vector<uint8_t> getHvacValue();
    std::vector<uint8_t> getVehicleDoorData();
    std::vector<uint8_t> getVehicleStatusData();
    std::vector<uint8_t> getDataCollectionData();
    std::vector<uint8_t> getCustomInfoData();

  public:
    VehicleController mVehicleCtl;

  private:
    std::vector<uint8_t> hvacValue;
    std::vector<uint8_t> carDoorValue;
    std::vector<uint8_t> carStatusValue;
    std::vector<uint8_t> dataCollectionValue;
    std::vector<uint8_t> customInfoValue;
    enum customInfoId {
        customInfoFlagsId = 0x8A,      //  �Զ�����Ϣ��־
        dataCollectionFlagId = 0x8B,   //  ���ݲɼ���־
        customVehicleStatusId = 0x90,  //  �Զ�������״̬
        customCarDoorId = 0x91,        //  �Զ�������״̬
        customHvacId = 0x92,           //  �Զ���յ�״̬
    };
};

class CustomVehicleStatusMessage {
  public:
    CustomVehicleStatusMessage(
            uint16_t headerData, uint8_t antiTheft, uint8_t highPressure,
            uint8_t programVersionSwitch, uint8_t lockCarCtrlFlag, uint8_t lockCarCtrlStatus,
            uint8_t maintenanceTips, uint8_t paymentTips, uint8_t annualReviewTips,
            uint8_t powerBatteryMaintenanceFlag, uint8_t powerBatteryMaintenancestatus,
            uint16_t remainingCruisingRange, uint8_t remainingPower, uint8_t chargeGunConnectStatus,
            uint8_t chargeStatus, uint8_t chargingTargetPower, uint16_t remainingChargeTime,
            uint8_t chargingCurrent, uint16_t chargingVoltage, uint8_t leftFrontTirePressure,
            uint8_t rightFrontTirePressure, uint8_t leftRearTirePressure,
            uint8_t rightRearTirePressure, uint32_t protocolReserv);

    std::vector<uint8_t> generateDataPacket();
    void printMessage();

  private:
    uint8_t customVehicleStatusId = 0x90;
    uint16_t headerData;
    uint8_t antiTheft;
    uint8_t highPressure;
    uint8_t programVersionSwitch;
    uint8_t lockCarCtrlFlag;
    uint8_t lockCarCtrlStatus;
    uint8_t maintenanceTips;
    uint8_t paymentTips;
    uint8_t annualReviewTips;
    uint8_t powerBatteryMaintenanceFlag;
    uint8_t powerBatteryMaintenancestatus;
    uint16_t remainingCruisingRange;
    uint8_t remainingPower;
    uint8_t chargeGunConnectStatus;
    uint8_t chargeStatus;
    uint8_t chargingTargetPower;
    uint16_t remainingChargeTime;
    uint8_t chargingCurrent;
    uint16_t chargingVoltage;
    uint8_t leftFrontTirePressure;
    uint8_t rightFrontTirePressure;
    uint8_t leftRearTirePressure;
    uint8_t rightRearTirePressure;
    uint32_t protocolReserv;
};

class CustomDataCollectionFlags {
  public:
    CustomDataCollectionFlags(
            uint16_t headerData, uint16_t RealtimeDischargePower, uint8_t bmsSelftestStatus,
            uint8_t bmsOperationMode, uint16_t powerBatteryRealtimePower,
            uint16_t RealtimeChargingPower, uint16_t SOH, uint8_t bmsPositiveContactorStatus,
            uint8_t bmsNegativeContactorStatus, uint8_t chargingMode,
            uint16_t RealtimeChargingCurrentRequest, uint16_t RealtimeChargingVoltageRequest,
            uint16_t RealtimeDCChargingVoltage, uint16_t RealtimeDCChargingCurrent,
            uint8_t fastChargeContactorCtrlCmd, uint8_t fastChargeNegaTouchpointTemp,
            uint8_t fastChargeposiTouchpointTemp, uint16_t dcdcOutputVoltage,
            uint16_t dcdcOutputCurrent, uint8_t dcdcOperatingTemp, uint8_t hcmWorkState,
            uint8_t fastChargeContactorStatus, uint8_t highVoltageLoopInterlockState,
            uint8_t mcuStatus, uint8_t cpDutyCycle, uint16_t obcInputChargingCurrent,
            uint16_t obcInputChargingVoltage, uint16_t obcOutputChargingCurrent,
            uint16_t obcOutputChargingVoltage, uint16_t lowVoltageBatteryVoltageValue,
            uint8_t vcuCoolingSystemSwitch, uint16_t vcuRequestTorqueToMcu,
            uint8_t indicatorPowerSystem, uint64_t vcuHardwareVersionNumber,
            uint64_t vcuSoftwareVersionNumber, uint64_t bmsSoftwareVersionNumber,
            uint64_t obcHardwareVersionNumber, uint64_t obcSoftwareVersionNumber,
            uint8_t aircondRefrigerationPower, uint8_t vacuumPumpSystemFaultFlag);

    std::vector<uint8_t> generateDataPacket();
    void printMessage();

  private:
    uint8_t customDataCollectionFlagsId = 0x8B;
    uint16_t headerData;
    uint16_t RealtimeDischargePower;
    uint8_t bmsSelftestStatus;
    uint8_t bmsOperationMode;
    uint16_t powerBatteryRealtimePower;
    uint16_t RealtimeChargingPower;
    uint16_t SOH;
    uint8_t bmsPositiveContactorStatus;
    uint8_t bmsNegativeContactorStatus;
    uint8_t chargingMode;
    uint16_t RealtimeChargingCurrentRequest;
    uint16_t RealtimeChargingVoltageRequest;
    uint16_t RealtimeDCChargingVoltage;
    uint16_t RealtimeDCChargingCurrent;
    uint8_t fastChargeContactorCtrlCmd;
    uint8_t fastChargeNegaTouchpointTemp;
    uint8_t fastChargeposiTouchpointTemp;
    uint16_t dcdcOutputVoltage;
    uint16_t dcdcOutputCurrent;
    uint8_t dcdcOperatingTemp;
    uint8_t hcmWorkState;
    uint8_t fastChargeContactorStatus;
    uint8_t highVoltageLoopInterlockState;
    uint8_t mcuStatus;
    uint8_t cpDutyCycle;
    uint16_t obcInputChargingCurrent;
    uint16_t obcInputChargingVoltage;
    uint16_t obcOutputChargingCurrent;
    uint16_t obcOutputChargingVoltage;
    uint16_t lowVoltageBatteryVoltageValue;
    uint8_t vcuCoolingSystemSwitch;
    uint16_t vcuRequestTorqueToMcu;
    uint8_t indicatorPowerSystem;
    uint64_t vcuHardwareVersionNumber;
    uint64_t vcuSoftwareVersionNumber;
    uint64_t bmsSoftwareVersionNumber;
    uint64_t obcHardwareVersionNumber;
    uint64_t obcSoftwareVersionNumber;
    uint8_t aircondRefrigerationPower;
    uint8_t vacuumPumpSystemFaultFlag;
};



#endif
