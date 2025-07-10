#ifndef GB32960_MESSAGE_DATA_H
#define GB32960_MESSAGE_DATA_H

#include <string>
#include <vector>
#ifdef EV48
#include "CustomInfoMessage.h"
#include "CustomInfoFlags.h"
#include "DataCollectionFlags.h"
#include "DoorLockStatus.h"
#include "HavcStatus.h"
#endif

#include "DriveMotorDataMessage.h"
#include "ExtremeDataMessage.h"
#include "FaultDataMessage.h"
#include "LocationDataMessage.h"
#include "RealTimeDataMessage.h"
#include "VehicleDataMessage.h"
#include "VehicleLoginMessage.h"
#include "battery_voltage_message.h"
#include "parm_report_message.h"
#include "rechargeable_energy_temperature_message.h"
#include "VehicleStatusMessage.h"
#include "VehicleInfoMessage.h"
#include "RealNameAuthMessage.h"
#include "TboxDataDeclaration.h"
#include "vehicle_controller.h"
#include "FrontCamInfoMessage.h"
#include "IntegratedSwitchInfoMessage.h"
#include "TerminalSysInfoMessage.h"
#include "RadioSysInfoMessage.h"
#include "ReverseAssistantSysInfoMessage.h"
#include "ChassisSysInfoMessage.h"
#include "PowerSysInfoMessage.h"
#include "BatterySysInfoMessage.h"
#include "BatteryChargingInfoMessage.h"
#include "ACSysInfoMessage.h"
#include "VehicleBodySysInfoMessage.h"
#include "VehicleControllerInfoMessage.h"

class GB32960MessageData {
  private:
    std::string startMarker;
    uint8_t command;
    uint8_t responseFlag;
    std::string uniqueID;
    uint8_t encryptionMethod;
    uint16_t dataLength;
    std::vector<uint8_t> data;
    uint8_t checksum;
    VehicleController mVehicle;
    LockMap<int32_t, TboxDataDeclaration>* mTboxdecl;

  public:
    std::string mUniqueID;
    std::string mIccid;
    ParmReportMessage mParmMsg;
#ifdef EV48
    CustomInfoMessage mCustomInfoMessage;
#endif
    ParmReportMessage mParmWMsg;
    std::vector<uint8_t> mRechargeableEnergySystemCodes = {0x01};
    VehicleLoginMessage mVehicleloginMessage{mIccid, 1, 1, mRechargeableEnergySystemCodes};
    VehicleLoginMessage mVehicleloginMessageSample{mIccid, 1, 1, mRechargeableEnergySystemCodes};
    VehicleInfoMessage mVehicleInfoMessage{mVehicle};
    RealNameAuthMessage mRealNameAuthMessage{};
    std::vector<uint8_t> mAddtionMessage;

  public:
    GB32960MessageData(LockMap<int32_t, TboxDataDeclaration>& tboxdecl, VehicleController& vehicle);
    GB32960MessageData(uint8_t command, uint8_t responseFlag, const std::string& uniqueID,
                       uint8_t encryptionMethod, const std::vector<uint8_t>& data);
    void calculateChecksum();
    void printMessage();
    std::vector<uint8_t> generateDataPacket();
    std::vector<uint8_t> generateLoginMessage();
    std::vector<uint8_t> generateLoginMessageSample();
    std::vector<uint8_t> generateLogoutMessage();
    std::vector<uint8_t> generateLogoutMessageSample();
    std::vector<uint8_t> generateHeartbeatMessage();
    std::vector<uint8_t> generatetimeSynchronizationMessage();
#ifdef EV48
    std::vector<uint8_t> generateRealtimeMessage(VehicleDataMessage& vehiclemessage,
                                                 DriveMotorDataMessage& drivemotormessage,
                                                 LocationDataMessage& locationdatamessage,
                                                 ExtremeDataMessage& extremedatamessage,
                                                 FaultDataMessage& faultdatamessage,
                                                 BatteryVoltageMessage& batteryVoltageMessage,
                                                 ProbeTemperatureMessage& probeTemperatureMessage,
                                                 CustomInfoFlags& customInfoFlags,
                                                 DataCollectionFlags& dataCollectionflags,
                                                 VehicleStatusMessage& vehicleStatusMessage,
                                                 DoorLockStatus& doorLockStatus,
                                                 HavcStatus& havcStatus);
#endif
    std::vector<uint8_t> generateME11RealtimeMessage(VehicleDataMessage& vehiclemessage,
                                                 DriveMotorDataMessage& drivemotormessage,
                                                 LocationDataMessage& locationdatamessage,
                                                 ExtremeDataMessage& extremedatamessage,
                                                 FaultDataMessage& faultdatamessage,
                                                 BatteryVoltageMessage& batteryVoltageMessage,
                                                 ProbeTemperatureMessage& probeTemperatureMessage,
                                                 FrontCamInfoMessage& frontCamInfoMessage,
                                                 IntegratedSwitchInfoMessage& intgSwitchInfoMessage,
                                                 TerminalSysInfoMessage& terminalSysInfoMessage,
                                                 RadioSysInfoMessage& radioSysInfoMessage,
                                                 ReverseAssistantSysInfoMessage& reverseAssistantSysInfoMessage,
                                                 ChassisSysInfoMessage& chassisSysInfoMessage,
                                                 PowerSysInfoMessage& powerSysInfoMessage,
                                                 BatterySysInfoMessage& batterySysInfoMessage,
                                                 BatteryChargingInfoMessage& batteryChargingInfoMessage,
                                                 ACSysInfoMessage& acSysInfoMessage,
                                                 VehicleBodySysInfoMessage& vehicleBodySysInfoMessage,
                                                 VehicleControllerInfoMessage& vehicleControllerInfoMessage);

    std::vector<uint8_t> generate80ReportMessage(int idCount, const std::vector<uint8_t>& idList);
    std::vector<uint8_t> controlMessageResponse(uint8_t type, std::vector<uint8_t> serviceCtrlResp);
    std::vector<uint8_t> generate81ReportMessage(int idCount, const std::vector<uint8_t>& idList);
    std::vector<uint8_t> generate81ReportMessage(const std::vector<uint8_t>& msg);
    std::vector<uint8_t> generate82ReportMessage();
    std::vector<uint8_t> changeMessageID(uint8_t MSGID, const std::vector<uint8_t>& message);
    std::vector<uint8_t> generateVehicleInfoMessage();
    std::vector<uint8_t> generateRealNameAuthMessage();
    //void saveLoginSN();

  private:
    std::vector<uint8_t> stringToBinary(const std::string& str);
};

#endif  // GB32960_MESSAGE_DATA_H
