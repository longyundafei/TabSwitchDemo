#ifndef TBOX_H
#define TBOX_H
#include "BinaryMessage.h"
#include "CarMessageQueue.h"
#include "GB32960MessageParser.h"
#include "Machine.h"
#include "messageproductstate.h"
#include "messagequeue.h"
#include "networkcomm.h"
#include "parm_report_message.h"
#include "timer.h"
#include "vehicle_controller.h"
#include "xml_config.h"
#include "TboxDataDeclaration.h"
#include "config_parser.h"
#include "LockMap.h"
#include "GB32960MessageData.h"
#include "VehicleDataMessage.h"
#include "DriveMotorDataMessage.h"
#ifdef EV48
#include "VehicleStatusMessage.h"
#include "DoorLockStatus.h"
#include "HavcStatus.h"
#endif
#include "ChargingCtl.h"
#include "RemoteHavcCtl.h"
#include "RemoteSeatHeatCtl.h"
#include "RemoteSteerWheelHeatCtl.h"
#include "RemoteVersionSwitch.h"
#include "LockVehicle.h"
#include "VehicleStatusInfo.h"
#include <cutils/properties.h>



class Tbox {
  public:
    Tbox();  // 构造函数
    void init();
    void updateVin();
    void HandleMessage(const std::vector<uint8_t>& message);
    void HandleMessageSample(const std::vector<uint8_t>& message);
    void RemoteCtl(msg82pack& pack);
    void shutdown();
    void reboot();
    int ResetConfigfile();
    int getSOCPowerState();
    void sendLogoutMessage();
    void saveSpeedLimitLevelReq(uint8_t chargingCondition, uint8_t level);
    void setSTRWorkReq(int workReq);
    void revertIPConfig();
    uint32_t validateBCMPowerSt();
    uint8_t getBCMPowerSt();
    int getSOCBatteryLevel();
    void waitingForVehicleAwake();

  public:
    MachineState mState;  // 当前状态
    MachineState mStateSample;  // 当前状态
    GB32960MessageParser mMessageParser;
    MessageQueue mMsgQueue;
    MessageQueue mMsgQueueSample;
    MessageQueue mSaveQueue;
    MessageQueue mSaveQueueSample;
    MessageQueue mSave7DayQueue;
    MessageQueue mMsg30SecondsQueue;
    MessageQueue mMsg30SecondsSampleQueue;
    BinaryMessage mBinaryMessage;
    std::vector<std::vector<uint8_t>> mCacheResult;
    std::vector<std::vector<uint8_t>> mCacheResultSample;
    sp<IVehicle> mVehicle;
    ParmReportMessage mParmReportMsg;
    MessageProductState mMsgProductState = MessageProductState::Normal;
    LockMap<int32_t, TboxDataDeclaration> mTboxDecl;
    ConfigParser mConfigParser{"/vendor/etc/gecko-tbox-vehicle.json",mTboxDecl};
    VehicleController mVehicleCtl{mTboxDecl};
    GB32960MessageData mMessageData{mTboxDecl,mVehicleCtl};
    VehicleDataMessage mVehiclemessage{mTboxDecl,mVehicleCtl};
    DriveMotorDataMessage mDrivemotormessage{mTboxDecl,mVehicleCtl};
    LocationDataMessage mLocationDataMessage{};
    ExtremeDataMessage mExtremedatamessage{mTboxDecl,mVehicleCtl};
    FaultDataMessage mFaultdatamessage{mTboxDecl,mVehicleCtl};
    BatteryVoltageMessage mBatteryVoltageMessage{mTboxDecl,mVehicleCtl};
    ProbeTemperatureMessage mProbeTemperatureMessage{mTboxDecl,mVehicleCtl};
#ifdef EV48
    CustomInfoFlags mCustomInfoFlags{mTboxDecl,mVehicleCtl};
    DataCollectionFlags mDataCollectionFlags{mTboxDecl,mVehicleCtl};
    VehicleStatusMessage mVehicleStatusMessage{mTboxDecl,mVehicleCtl};
    DoorLockStatus mDoorLockStatus{mTboxDecl,mVehicleCtl};
    HavcStatus mHavcStatus{mTboxDecl,mVehicleCtl};
#endif
    FrontCamInfoMessage mFrontCamInfoMessage{mTboxDecl,mVehicleCtl};    //add for ME11
    IntegratedSwitchInfoMessage mIntgSwitchInfoMessage{mTboxDecl,mVehicleCtl};    //add for ME11
    TerminalSysInfoMessage mTerminalSysInfoMessage{mTboxDecl,mVehicleCtl};    //add for ME11
    RadioSysInfoMessage mRadioSysInfoMessage{mTboxDecl,mVehicleCtl};    //add for ME11
    ReverseAssistantSysInfoMessage mReverseAssistantSysInfoMessage{mTboxDecl,mVehicleCtl};    //add for ME11
    ChassisSysInfoMessage mChassisSysInfoMessage{mTboxDecl,mVehicleCtl};    //add for ME11
    PowerSysInfoMessage mPowerSysInfoMessage{mTboxDecl,mVehicleCtl};    //add for ME11
    BatterySysInfoMessage mBatterySysInfoMessage{mTboxDecl,mVehicleCtl};    //add for ME11
    BatteryChargingInfoMessage mBatteryChargingInfoMessage{mTboxDecl,mVehicleCtl};    //add for ME11
    ACSysInfoMessage mACSysInfoMessage{mTboxDecl,mVehicleCtl};    //add for ME11
    VehicleBodySysInfoMessage mVehicleBodySysInfoMessage{mTboxDecl,mVehicleCtl};    //add for ME11
    VehicleControllerInfoMessage mVehicleControllerInfoMessage{mTboxDecl,mVehicleCtl};    //add for ME11
    NetworkComm mNetworkComm{&mMessageData};
    NetworkComm mNetworkCommSample{&mMessageData};
    ChargingCtl mChargingCtl{mVehicleCtl};
    RemoteHavcCtl mRemoteHaveCtl{mVehicleCtl};
    RemoteSeatHeatCtl mRemoteSeatHeatCtl{mVehicleCtl};
    RemoteSteerWheelHeatCtl mRemoteSteerWheelHeatCtl{mVehicleCtl};
    RemoteVersionSwitch mRemoteVersionSwitch{mVehicleCtl};
    LockVehicle mLockVehicle{mVehicleCtl};
    VehicleStatusInfo mVehicleStatusInfo{mVehicleCtl};
    XMLConfig mXmlConfig;
    Timer mTimer;
    int mLastAlarmLevel = 0x00;
    int network = 0x00;
    int mRealNameAuthState;
    //int mLogoutReason = 0;//1:进入str
    //int mWakeup = 1;
    int mSTR = 0;

  private:
    enum tboxcmd {
      vehicleLogin = 1,
      realtimeInformationReporting = 2,
      resendInformationReporting = 3,
      vehicleLogout = 4,
      heartbeat = 7,
      timeSynchronization = 8,
    };

    enum geckoResponse {
      gecko_response_ok = 0,
      gecko_response_fail = 1,
    };

    enum geckoTboxWorkReq {
      //gecko_tbox_work_req_default = 0,
      gecko_tbox_no_work_req = 0,
      gecko_tbox_work_req = 1,
    };

    enum remoteCtrl {
        responce_nok = 0,
        responce_ok,
        eLockVehicle = 0x84,         // 锁车控制
        eProgramVersion = 0x85,      // 程序版本切换控制
        eMaintenance = 0x88,         // 保养提示
        ePayMent = 0x89,             // 缴费提示
        eAnnualVerif = 0x8A,         // 年审提示
        eBatteryMaintenance = 0x8B,  // 动力电池保养维护
        eQueryReCharge = 0xA0,       // 预约充电配置查询
        eQueryBatteryMaint = 0xA1,   // 电池维护配置查询
        eQueryPowerRepl = 0xA2,      // 智能补电配置查询
        eReCharge = 0xA3,            // 预约充电配置
        eBatteryMaintconfig = 0xA4,  // 电池维护配置
        ePowerReplenishment = 0xA5,  // 智能补电配置
        eVehicleDoorLock = 0xA6,     // 门锁控制
        eVehicleSearch = 0xA7,       // 寻车控制
        eHVACControl = 0xA8,         // 空调控制
        eRemoteDiagAuth = 0xA9,      // 远程诊断授权
        eRemoteDiagTrans = 0xAA,     // 远程诊断透传
        eRealNameAuthState = 0xAC,   // 实名认证回复
        eRemtMaiDrSeatHeat = 0xB0,   // 远程座椅加热
        eRemtSteerWhlHeat = 0xB1,    // 远程方向盘加热
        eRemotePowerLock = 0xB2,     // 禁止上电
    };
    enum cmdRemoteCtl {
        REMOTE_UPGRADE = 0x01,
        SHUTDOWN,
        RESET,
        FACTORY_RESET,
        DISCONNECT,
        ALARM,
        START_SAMPLING,
        OFFLINE_MODE = 0x80,
        REMOTE_CAN_COLLECTION = 0x90,
    };
};

#endif  // MACHINE_H
