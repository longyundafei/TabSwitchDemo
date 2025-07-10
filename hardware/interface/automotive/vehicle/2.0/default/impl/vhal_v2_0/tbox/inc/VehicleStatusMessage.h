#ifndef VEHICLESTATUSMESSAGE_H
#define VEHICLESTATUSMESSAGE_H

#include "RealTimeMessageBase.h"
#include <cmath>
#include <cutils/properties.h>

class VehicleStatusMessage : public RealTimeMessageBase{
  public:
    using RealTimeMessageBase::RealTimeMessageBase;

    std::vector<uint8_t> generateDataPacket();
    void printMessage();
    int getvehicle_anti_theft_statusValue();
    int gethigh_pressure_statusValue();
    int getprogram_version_control_statusValue();
    int getlock_vehicle_control_flagValue();
    int getlock_vehicle_control_statusValue();
    int getmaintenance_reminderValue();
    int getpayment_reminderValue();
    int getannual_review_reminderValue();
    int getbattery_maintenance_flagValue();
    int getbattery_maintenance_statusValue();
    int getremaining_rangeValue();
    int getremaining_SOCValue();
    int getcharge_discharge_gun_connection_statusValue();
    int getcharging_statusValue();
    int getcharging_target_SOCValue();
    int getremaining_charging_timeValue();
    int getcharging_currentValue();
    int getcharging_voltageValue();
    int gettire_pressureValue();
    //void setSpeedLimitLevel(uint8_t level);

  private:
    uint8_t VehicleStatusMessageId = 0x90;
    uint8_t vehicle_anti_theft_status;                  //整车防盗状态
    uint8_t high_pressure_status;                       //高压状态
    uint8_t program_version_control_status;             //程序版本切换控制状态
    uint8_t lock_vehicle_control_flag;                  //锁车控制标志
    uint8_t lock_vehicle_control_status;                //锁车控制状态
    uint8_t maintenance_reminder;                       //保养提示
    uint8_t payment_reminder;                           //缴费提示
    uint8_t annual_inspection_reminder;                 //年审提示
    uint8_t battery_maintenance_flag;                   //动力电池保养维护标志
    uint8_t battery_maintenance_status;                 //动力电池保养维护状态
    uint16_t remaining_range;                            //剩余续航里程
    uint8_t remaining_SOC;                              //剩余电量SOC
    uint8_t charge_discharge_gun_connection_status;     //充/放电枪连接状态
    uint8_t charging_status;                            //充电状态
    uint8_t charging_target_SOC;                        //充电目标电量
    uint16_t remaining_charging_time;                    //剩余充电时间
    uint8_t charging_current;                           //充电电流
    uint16_t charging_voltage;                           //充电电压
    uint8_t left_front_tire_pressure;                   //左前轮胎气压
    uint8_t right_front_tire_pressure;                  //右前轮胎气压
    uint8_t left_rear_tire_pressure;                    //左后轮胎气压
    uint8_t right_rear_tire_pressure;                   //右后轮胎气压
    //uint8_t mReqSpeedLimitLevel = 0x0;
};
#endif