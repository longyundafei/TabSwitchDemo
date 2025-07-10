#ifndef DATACOLLECTIONFLAGS_H
#define DATACOLLECTIONFLAGS_H

#include "RealTimeMessageBase.h"

class DataCollectionFlags : public RealTimeMessageBase{
  public:
    using RealTimeMessageBase::RealTimeMessageBase;

    std::vector<uint8_t> generateDataPacket();
    void printMessage();
    int getpower_battery_discharge_powerValue();
    int getBMS_self_test_statusValue();
    int getBMS_running_modeValue();
    int getpower_battery_current_capacityValue();
    int getcurrent_charging_capacityValue();
    int getSOH_battery_healthValue();
    int getBMS_positive_relay_statusValue();
    int getBMS_negative_relay_statusValue();
    int getcharging_modeValue();
    int getreal_time_charging_current_requestValue();
    int getreal_time_charging_voltage_requestValue();
    int getreal_time_DC_charging_voltageValue();
    int getreal_time_DC_charging_currentValue();
    int getfast_charge_relay_control_commandValue();
    int getfast_charge_negative_contact_tempValue();
    int getfast_charge_positive_contact_tempValue();
    int getDCDC_output_voltageValue();
    int getDCDC_output_currentValue();
    int getDCDC_working_temperatureValue();
    int getHCM_working_enable_statusValue();
    int getfast_charge_relay_statusValue();
    int gethigh_voltage_interlock_statusValue();
    int getMCU_statusValue();
    int getCP_duty_cycleValue();
    int getOBC_input_charging_currentValue();
    int getOBC_input_charging_voltageValue();
    int getOBC_output_charging_currentValue();
    int getOBC_output_charging_voltageValue();
    int getlow_voltage_battery_voltageValue();
    int getVCU_cooling_system_switch_enableValue();
    int getVCU_request_torque_to_MCUValue();
    int getpower_system_READY_statusValue();
    int getVCU_hardware_versionValue();
    int getVCU_software_versionValue();
    int getBMS_software_versionValue();
    int getOBC_hardware_versionValue();
    int getOBC_software_versionValue();
    int getair_conditioning_cooling_powerValue();
    int getvacuum_pump_system_fault_flagValue();

  private:
    uint8_t DataCollectionFlagsId = 0x8B;

    uint16_t power_battery_discharge_power;     // 动力电池实时许用放电功率
    uint8_t BMS_self_test_status;                 // BMS自检状态
    uint8_t BMS_running_mode;                     // BMS运行模式
    uint16_t power_battery_current_capacity;    // 动力电池当前电量kWh
    uint16_t current_charging_capacity;         // 本次（实时）充电电量kWh
    uint16_t SOH_battery_health;               // SOH（电池健康度）
    uint8_t BMS_positive_relay_status;            // BMS正极接触器状态
    uint8_t BMS_negative_relay_status;            // BMS负极接触器状态
    uint8_t charging_mode;                        // 充电模式
    uint16_t real_time_charging_current_request;// 实时充电电流请求值
    uint16_t real_time_charging_voltage_request;// 实时充电电压请求值
    uint16_t real_time_DC_charging_voltage;     // 实时直流充电电压
    uint16_t real_time_DC_charging_current;     // 实时直流充电电流
    uint8_t fast_charge_relay_control_command;    // 快充接触器控制命令
    uint8_t fast_charge_negative_contact_temp; // 快充口负极触点温度
    uint8_t fast_charge_positive_contact_temp; // 快充口正极触点温度
    uint16_t DCDC_output_voltage;               // DCDC输出电压
    uint16_t DCDC_output_current;               // DCDC输出电流
    uint8_t DCDC_working_temperature;          // DCDC工作温度
    uint8_t HCM_working_enable_status;            // HCM工作使能状态
    uint8_t fast_charge_relay_status;             // 快充接触器状态
    uint8_t high_voltage_interlock_status;        // 高压环路互锁状态
    uint8_t MCU_status;                           // MCU状态
    uint8_t CP_duty_cycle;                     // CP占空比
    uint16_t OBC_input_charging_current;        // OBC输入充电电流
    uint16_t OBC_input_charging_voltage;        // OBC输入充电电压
    uint16_t OBC_output_charging_current;       // OBC输出充电电流
    uint16_t OBC_output_charging_voltage;       // OBC输出充电电压
    uint16_t low_voltage_battery_voltage;       // 低压蓄电池电压值
    uint8_t VCU_cooling_system_switch_enable;     // VCU对冷却系统开关使能
    uint16_t VCU_request_torque_to_MCU;         // VCU对MCU请求转矩
    uint8_t power_system_READY_status;            // 指示动力系统READY状态
    uint8_t VCU_hardware_version[7];         // VCU硬件版本号
    uint8_t VCU_software_version[7];         // VCU软件版本号
    uint8_t BMS_software_version[7];         // BMS软件版本号
    uint8_t OBC_hardware_version[7];         // OBC硬件版本号
    uint8_t OBC_software_version[7];         // OBC软件版本号
    uint8_t air_conditioning_cooling_power;    // 空调制冷功率
    uint8_t vacuum_pump_system_fault_flag;       // 真空泵系统故障标志
};
#endif