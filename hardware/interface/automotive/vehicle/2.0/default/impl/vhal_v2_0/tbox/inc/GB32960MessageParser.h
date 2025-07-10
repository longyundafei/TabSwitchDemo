#ifndef GB32960_MESSAGE_PARSER_H
#define GB32960_MESSAGE_PARSER_H

#include <string>
#include <vector>
#include "GB32960MessageData.h"
#include "parm_report_message.h"
#include "vehicle_controller.h"

struct msg80pack {
    std::vector<uint8_t> time;
    uint8_t parmNum;
    std::vector<uint8_t> parm;
};
struct msg81 {
    uint8_t id;
    std::vector<uint8_t> value;
};
struct msg81pack {
    std::vector<uint8_t> time;
    uint8_t parmNum;
    std::vector<msg81> parm;
};
struct msg82 {
    uint8_t id;
    std::vector<uint8_t> value;
};
struct msg82pack {
    std::vector<uint8_t> time;
    uint8_t commandId;
    std::vector<msg82> commandparm;
};

struct LockVehicleMessage {
    uint8_t chargingConditions;
    uint8_t speedLimitLevel;
};

struct VersionSwitchMessage {
    uint8_t requestVersion;
};

struct HavcCtlMessage {
    std::vector<uint8_t> time;
    uint8_t air_conditioning_switch;
    uint8_t working_time;
    uint8_t circulation_mode;
    uint8_t air_outlet_mode;
    uint8_t cooling_switch;
    uint8_t heating_switch;
    uint8_t defrosting_switch;
    uint8_t fan_speed_adjustment;
    uint8_t temperature_adjustment;
};

struct SeatHeatCtlMessage {
    uint8_t seatHeatSwitch;
    uint8_t heatLevel;
};

class GB32960MessageParser {
  public:
    uint8_t command;
    uint8_t responseFlag;
    std::string uniqueID;
    uint8_t encryptionMethod;
    uint16_t dataLength;
    std::vector<uint8_t> data;
    uint8_t checksum;
    msg80pack mMsg80pack;
    msg81pack mMsg81pack;
    msg82pack mMsg82pack;
    LockVehicleMessage mMsgLockVehicle;
    VersionSwitchMessage mMsgVersionSwitch;
    HavcCtlMessage mMsgHavcCtl;
    uint8_t mMsgVerSwitch;
    SeatHeatCtlMessage mMsgSeatHeatCtl;

  public:
    VehicleController pVehicleCtl;

  public:
    bool parseMessage(const std::vector<uint8_t>& message);
    bool parse80Message(const std::vector<uint8_t>& message);
    bool parse81Message(const std::vector<uint8_t>& message, ParmReportMessage& parm);
    bool parse82Message(const std::vector<uint8_t>& message);
    bool parseMsgLockVehicle(const std::vector<uint8_t>& message,LockVehicleMessage &lockVehicle);
    bool parseMsgVersionSwitch(const std::vector<uint8_t>& message,VersionSwitchMessage &version);
    bool parseMsgHavcCtl(const std::vector<uint8_t>& message,HavcCtlMessage &hacvCtl);
    bool parseMsgSeatHeatCtl(const std::vector<uint8_t>& message,SeatHeatCtlMessage &seatHeatCtl);
    void hvacDataHandle(std::vector<uint8_t> hvacData);
    int hvacHandleresponse(std::vector<uint8_t> hvacData);
    void closehvacHandle();
    int doorLockHandle(uint8_t doorLock);
    void serviceControlDataHandle(uint8_t type, uint8_t data);
    std::vector<uint8_t> powerRepHandle(std::vector<uint8_t> powerRep);
    std::vector<uint8_t> queryPowerRepHandle();
    std::vector<uint8_t> reChargeHandle(std::vector<uint8_t> reCharge);
    std::vector<uint8_t> queryReChargeHandle();
    std::vector<uint8_t> diagAuthHandle(std::vector<uint8_t> remoteDiagAuth);
    std::vector<uint8_t> diagTransHandle(std::vector<uint8_t> remoteDiagTrans);
    int validateRemoteControleCMD(std::string vin, const std::vector<uint8_t>& message);
    int validateVinForRemoteControleCMD(std::string vin, const std::vector<uint8_t>& message);

  private:
    uint8_t calculateChecksum(const std::vector<uint8_t>& message);

    std::vector<VehicleProperty> hvacProperties = {
            VehicleProperty::HVAC_POWER_ON,       VehicleProperty::INVALID,
            VehicleProperty::HVAC_RECIRC_ON,      VehicleProperty::HVAC_FAN_DIRECTION,
            VehicleProperty::HVAC_AC_ON,          VehicleProperty::GECKO_HVAC_PTC_HEAT_ON,
            VehicleProperty::HVAC_DEFROSTER,      VehicleProperty::HVAC_FAN_SPEED,
            VehicleProperty::HVAC_TEMPERATURE_SET};
};

#endif  // GB32960_MESSAGE_PARSER_H
