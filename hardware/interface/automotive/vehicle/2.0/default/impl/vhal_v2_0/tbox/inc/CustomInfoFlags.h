#ifndef CUSTOMINFOFLAGS_H
#define CUSTOMINFOFLAGS_H

#include "RealTimeMessageBase.h"
#include <cutils/properties.h>

class CustomInfoFlags : public RealTimeMessageBase{
  public:
    using RealTimeMessageBase::RealTimeMessageBase;

    std::vector<uint8_t> generateDataPacket();
    void printMessage();
    int getprogramVersionSwitchControlStatusValue();
    int getlockControlStatusValue();
    int getchargingQuantityValue();
    int getvehicleChargeFlagValue();
    int getlockControlFlagValue();
    int getvehicleOnSignalValue();
    int getvehicleCanWakeupState();
    int getvehicleCanState();
    int getvehicleSmallBatteryVoltage();
    int getmaintenance_reminderValue();
    int getpayment_reminderValue();
    int getannual_review_reminderValue();
    int getvehicleChargeSignalValue();
    int getvcuVersionNumberValue();
    int getpumpFaultValue();
    int getptcWorkingStatusValue();
    int getptcRelayActualStatusValue();
    int getpowerBatteryHeatingRelayStatusValue();
    int getquickChargeRelayAdhesionFaultValue();
    int getairConditioningWorkingStatusValue();
    int gethandbrakeStatusValue();
    int getpowerBatteryMaintenanceFlagValue();
    int getpowerBatteryMaintenanceStatusValue();
    int getVCUVersion();
    //void setSpeedLimitLevel(uint8_t level);
    void updateConfig(int Sampling);

  private:
    uint8_t customInfoFlagsId = 0x8A;
    uint16_t headerData;
    const std::vector<uint8_t> infodata;

    uint8_t deviceModelNumber; // 设备型号编号
    uint8_t powerSupplyIdentification; // 设备供电标识
    uint8_t firmwareVersion[5]; // 车载终端固件版本号
    uint8_t relayStatus; // 继电器状态
    uint8_t terminalBindingStatus; // 终端绑定状态
    uint8_t programVersionSwitchControlStatus; // 程序版本切换控制状态
    uint8_t lockControlStatus; // 锁车控制状态
    uint16_t chargingQuantity; // 本次充电电量
    uint8_t vcuVersionNumber[8]; // VCU版本号
    uint8_t vehicleChargeFlag; // 车辆快慢充标志
    uint8_t lockControlFlag; // 锁车控制标志
    uint8_t faultCount; // 故障数
    uint16_t vehicleTypeIdentification = 0x0071; // 车型识别
    uint8_t terminalWorkingStatus; // 车载终端工作状态
    uint8_t vehicleOnSignal; // 车辆ON信号
    uint8_t vehicleChargeSignal; // 车辆CHARGE信号
    uint8_t canWakeUpStatus; // 车载终端CAN唤醒状态
    uint8_t canState; // 车载终端CAN0~3状态
    uint16_t vehicleSmallBatteryVoltage; // 车辆小电瓶电压
    uint16_t internalBatteryVoltage; // 车载终端内部电池电压
    uint8_t LinkIpLength; // 双联路IP长度
    std::vector<uint8_t> LinkIpPort; // 双联路IP端口
    uint8_t LinkStatus; // 双链路状态
    uint8_t maintenanceReminder; // 保养提示
    uint8_t paymentReminder; // 缴费提示
    uint8_t annualReviewReminder; // 年审提示
    uint8_t networkSignal; // 车载终端网络信号
    uint8_t gpsWorkingStatus; // 车载终端GPS工作状态
    uint8_t emmcWorkingStatus; // 车载终端EMMC工作状态
    uint16_t airConditioningFeedbackCurrent; // 空调反馈电流
    uint8_t pumpFault; // 水泵故障
    uint8_t vacuumPumpStatus; // 真空泵状态
    uint8_t vacuumDegree; // 真空度
    uint8_t ptcWorkingStatus; // PTC工作状态
    uint8_t ptcRelayActualStatus; // PTC继电器实际状态
    uint8_t powerBatteryHeatingRelayStatus; // 动力电池加热继电器状态
    uint8_t quickChargeRelayAdhesionFault; // 快充继电器粘连故障
    uint8_t airConditioningWorkingStatus; // 空调工作状态
    uint8_t handbrakeStatus; // 手刹状态
    uint8_t powerBatteryMaintenanceFlag; // 动力电池保养维护标志
    uint8_t powerBatteryMaintenanceStatus; // 动力电池保养维护状态
    uint8_t voiceAlarmModuleOnlineStatus; // 语音报警模块在线状态
    //uint8_t mReqSpeedLimitLevel = 0x0;
};
#endif