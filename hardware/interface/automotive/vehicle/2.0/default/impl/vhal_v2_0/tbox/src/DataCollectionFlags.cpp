#include "DataCollectionFlags.h"

int DataCollectionFlags::getpower_battery_discharge_powerValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_REALTIMEALOWDISCHAPOWER));
}
int DataCollectionFlags::getBMS_self_test_statusValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_INITSTATUS));
}
int DataCollectionFlags::getBMS_running_modeValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_HVBATTST));
}
int DataCollectionFlags::getpower_battery_current_capacityValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_SOE));
}
int DataCollectionFlags::getcurrent_charging_capacityValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_SINGLECHRGENERGY));
}
int DataCollectionFlags::getSOH_battery_healthValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_BATTERYSOH));
}
int DataCollectionFlags::getBMS_positive_relay_statusValue() {
    return getValue(toInt(VehicleProperty::GECKO_IPU_MAINRELAYPOSISTATUS));
}
int DataCollectionFlags::getBMS_negative_relay_statusValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_MAINRELAYNEGA_STATUS));
}
int DataCollectionFlags::getcharging_modeValue() {
    return getRawValue(toInt(VehicleProperty::GECKO_VCU_CHRG_STS));
}
int DataCollectionFlags::getreal_time_charging_current_requestValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_BATTINFO4_DCCHARGREQCURR));
}
int DataCollectionFlags::getreal_time_charging_voltage_requestValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_BATTINFO4_DCCHARGREQVOL));
}
int DataCollectionFlags::getreal_time_DC_charging_voltageValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_BATTINFO4_MACHPUTVOLT));
}
int DataCollectionFlags::getreal_time_DC_charging_currentValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_BATTINFO4_MACHPUTCURR));
}
int DataCollectionFlags::getfast_charge_relay_control_commandValue() {
    return getRawValue(toInt(VehicleProperty::GECKO_VCU_DCCHRGRLYSTS));
}
int DataCollectionFlags::getfast_charge_negative_contact_tempValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_DEG_DCCHARSOCKETTEMP2));
}
int DataCollectionFlags::getfast_charge_positive_contact_tempValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_DEG_DCCHARSOCKETTEMP1));
}
int DataCollectionFlags::getDCDC_output_voltageValue() {
    return getValue(toInt(VehicleProperty::GECKO_IPU_DCDCREALTIMEOPVOLT));
}
int DataCollectionFlags::getDCDC_output_currentValue() {
    return getValue(toInt(VehicleProperty::GECKO_IPU_DCDCREALTIMEOPCURR));
}
int DataCollectionFlags::getDCDC_working_temperatureValue() {
    return getValue(toInt(VehicleProperty::GECKO_IPU_DCDCBODYTEMP));
}
int DataCollectionFlags::getHCM_working_enable_statusValue() {
    return getValue(toInt(VehicleProperty::GECKO_IPU_WORKSTATUS));
}
int DataCollectionFlags::getfast_charge_relay_statusValue() {
    return getRawValue(toInt(VehicleProperty::GECKO_VCU_DCCHRGRLYSTS));
}
int DataCollectionFlags::gethigh_voltage_interlock_statusValue() {
    return getValue(toInt(VehicleProperty::GECKO_IPU_IPUHVILSTA));
}
int DataCollectionFlags::getMCU_statusValue() {
    return getValue(toInt(VehicleProperty::GECKO_MCU_MOTOROPERMODE));
}
int DataCollectionFlags::getCP_duty_cycleValue() {
    return getValue(toInt(VehicleProperty::GECKO_IPU_CPDUTYCYCLE));
}
int DataCollectionFlags::getOBC_input_charging_currentValue() {
    return getValue(toInt(VehicleProperty::GECKO_IPU_OBCINPUTCURRENT));
}
int DataCollectionFlags::getOBC_input_charging_voltageValue() {
    return getValue(toInt(VehicleProperty::GECKO_IPU_OBCINPUTVOLTAGE));
}
int DataCollectionFlags::getOBC_output_charging_currentValue() {
    return getValue(toInt(VehicleProperty::GECKO_IPU_OBC_OUTPUT_CURRENT));
}
int DataCollectionFlags::getOBC_output_charging_voltageValue() {
    return getValue(toInt(VehicleProperty::GECKO_IPU_OBC_OUTPUT_VOLTAGE));
}
int DataCollectionFlags::getlow_voltage_battery_voltageValue() {
    return getValue(toInt(VehicleProperty::GECKO_VCU_POWERSUPPLYVOLTAG));
}
int DataCollectionFlags::getVCU_cooling_system_switch_enableValue() {
/*    int val1,val2;
    val1 = getValue(toInt(VehicleProperty::GECKO_VCU_WATERPUMPPWM));
    val2 = getValue(toInt(VehicleProperty::GECKO_VCU_FANRELAY_PINSTS));
    int value = val2 | val1;
    return value;
*/
    int ret = getRawValue(toInt(VehicleProperty::GECKO_VCU_MOTWATERPUMPSTATUS));
    if (((0x00 <= ret)&&(ret <= 0x0a)) || (ret == 0xff)) {
        return ret;
    } else {
        return 0xfe;
    }
}
int DataCollectionFlags::getVCU_request_torque_to_MCUValue() {
    return getValue(toInt(VehicleProperty::GECKO_VCU_TQREQ));
}
int DataCollectionFlags::getpower_system_READY_statusValue() {
    return getRawValue(toInt(VehicleProperty::GECKO_VCU_PT_ST));
}
int DataCollectionFlags::getVCU_hardware_versionValue() {
    return getValue(toInt(VehicleProperty::GECKO_VCU_HARDWARE_NUM));
}
int DataCollectionFlags::getVCU_software_versionValue() {
    return getValue(toInt(VehicleProperty::GECKO_VCU_SOFEWARE_NUM));
}
int DataCollectionFlags::getBMS_software_versionValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_SOFTWAREVERSION));
}
int DataCollectionFlags::getOBC_hardware_versionValue() {
    return getValue(toInt(VehicleProperty::GECKO_IPU_HARDWARE_NUM));
}
int DataCollectionFlags::getOBC_software_versionValue() {
    return getValue(toInt(VehicleProperty::GECKO_IPU_SOFEWARE_NUM));
}
int DataCollectionFlags::getair_conditioning_cooling_powerValue() {
    return getValue(toInt(VehicleProperty::GECKO_EAS_CRTPWR));
}
int DataCollectionFlags::getvacuum_pump_system_fault_flagValue() {
    return getValue(toInt(VehicleProperty::GECKO_VCU_VACUMPUMPFAULT));
}

std::vector<uint8_t> DataCollectionFlags::generateDataPacket() {
    std::vector<uint8_t> dataPacket;
    dataPacket.push_back(DataCollectionFlagsId);
    uint16_t packlength = 0;
    dataPacket.push_back((packlength >> 8) & 0xFF);
    dataPacket.push_back(packlength & 0xFF);
    packlength = 0;

    power_battery_discharge_power = getpower_battery_discharge_powerValue();
    dataPacket.push_back((power_battery_discharge_power >> 8) & 0xFF);
    dataPacket.push_back(power_battery_discharge_power & 0xFF);
    packlength = packlength + 2; //2
    BMS_self_test_status = getBMS_self_test_statusValue();
    dataPacket.push_back(BMS_self_test_status);
    packlength = packlength + 1; //3
    BMS_running_mode = getBMS_running_modeValue();
    dataPacket.push_back(BMS_running_mode);
    packlength = packlength + 1; //4
    power_battery_current_capacity = getpower_battery_current_capacityValue();
    dataPacket.push_back((power_battery_current_capacity >> 8) & 0xFF);
    dataPacket.push_back(power_battery_current_capacity & 0xFF);
    packlength = packlength + 2; //6
    current_charging_capacity = getcurrent_charging_capacityValue();
    dataPacket.push_back((current_charging_capacity >> 8) & 0xFF);
    dataPacket.push_back(current_charging_capacity & 0xFF);
    packlength = packlength + 2; //8
    SOH_battery_health = getSOH_battery_healthValue();
    dataPacket.push_back((SOH_battery_health >> 8) & 0xFF);
    dataPacket.push_back(SOH_battery_health & 0xFF);
    packlength = packlength + 2; //10
    BMS_positive_relay_status = getBMS_positive_relay_statusValue();
    dataPacket.push_back(BMS_positive_relay_status);
    packlength = packlength + 1; //11
    BMS_negative_relay_status = getBMS_negative_relay_statusValue();
    dataPacket.push_back(BMS_negative_relay_status);
    packlength = packlength + 1; //12
    charging_mode = getcharging_modeValue();
    dataPacket.push_back(charging_mode);
    packlength = packlength + 1; //13
    real_time_charging_current_request = getreal_time_charging_current_requestValue();
    dataPacket.push_back((real_time_charging_current_request >> 8) & 0xFF);
    dataPacket.push_back(real_time_charging_current_request & 0xFF);
    packlength = packlength + 2; //15
    real_time_charging_voltage_request = getreal_time_charging_voltage_requestValue();
    dataPacket.push_back((real_time_charging_voltage_request >> 8) & 0xFF);
    dataPacket.push_back(real_time_charging_voltage_request & 0xFF);
    packlength = packlength + 2; //17
    real_time_DC_charging_voltage = getreal_time_DC_charging_voltageValue();
    dataPacket.push_back((real_time_DC_charging_voltage >> 8) & 0xFF);
    dataPacket.push_back(real_time_DC_charging_voltage & 0xFF);
    packlength = packlength + 2; //19
    real_time_DC_charging_current = getreal_time_DC_charging_currentValue();
    dataPacket.push_back((real_time_DC_charging_current >> 8) & 0xFF);
    dataPacket.push_back(real_time_DC_charging_current & 0xFF);
    packlength = packlength + 2; //21
    fast_charge_relay_control_command = getfast_charge_relay_control_commandValue();
    dataPacket.push_back(fast_charge_relay_control_command);
    packlength = packlength + 1; //22
    fast_charge_negative_contact_temp = getfast_charge_negative_contact_tempValue();
    dataPacket.push_back(fast_charge_negative_contact_temp);
    packlength = packlength + 1; //23
    fast_charge_positive_contact_temp = getfast_charge_positive_contact_tempValue();
    dataPacket.push_back(fast_charge_positive_contact_temp);
    packlength = packlength + 1; //24
    DCDC_output_voltage = getDCDC_output_voltageValue();
    dataPacket.push_back((DCDC_output_voltage >> 8) & 0xFF);
    dataPacket.push_back(DCDC_output_voltage & 0xFF);
    packlength = packlength + 2; //26
    DCDC_output_current = getDCDC_output_currentValue();
    dataPacket.push_back((DCDC_output_current >> 8) & 0xFF);
    dataPacket.push_back(DCDC_output_current & 0xFF);
    packlength = packlength + 2; //28
    DCDC_working_temperature = getDCDC_working_temperatureValue();
    dataPacket.push_back(DCDC_working_temperature);
    packlength = packlength + 1; //29
    HCM_working_enable_status = getHCM_working_enable_statusValue();
    dataPacket.push_back(HCM_working_enable_status);
    packlength = packlength + 1; //30
    fast_charge_relay_status = getfast_charge_relay_statusValue();
    dataPacket.push_back(fast_charge_relay_status);
    packlength = packlength + 1; //31
    high_voltage_interlock_status = gethigh_voltage_interlock_statusValue();
    dataPacket.push_back(high_voltage_interlock_status);
    packlength = packlength + 1; //32
    //需要明确
    MCU_status = getMCU_statusValue();
    dataPacket.push_back(MCU_status);
    packlength = packlength + 1; //33
    CP_duty_cycle = getCP_duty_cycleValue();
    dataPacket.push_back(CP_duty_cycle);
    packlength = packlength + 1; //34
    OBC_input_charging_current = getOBC_input_charging_currentValue();
    dataPacket.push_back((OBC_input_charging_current >> 8) & 0xFF);
    dataPacket.push_back(OBC_input_charging_current & 0xFF);
    packlength = packlength + 2; //36
    OBC_input_charging_voltage = getOBC_input_charging_voltageValue();
    dataPacket.push_back((OBC_input_charging_voltage >> 8) & 0xFF);
    dataPacket.push_back(OBC_input_charging_voltage & 0xFF);
    packlength = packlength + 2; //38
    OBC_output_charging_current = getOBC_output_charging_currentValue();
    dataPacket.push_back((OBC_output_charging_current >> 8) & 0xFF);
    dataPacket.push_back(OBC_output_charging_current & 0xFF);
    packlength = packlength + 2; //40
    OBC_output_charging_voltage = getOBC_output_charging_voltageValue();
    dataPacket.push_back((OBC_output_charging_voltage >> 8) & 0xFF);
    dataPacket.push_back(OBC_output_charging_voltage & 0xFF);
    packlength = packlength + 2; //42
    low_voltage_battery_voltage = getlow_voltage_battery_voltageValue();
    dataPacket.push_back((low_voltage_battery_voltage >> 8) & 0xFF);
    dataPacket.push_back(low_voltage_battery_voltage & 0xFF);
    packlength = packlength + 2; //44
    VCU_cooling_system_switch_enable = getVCU_cooling_system_switch_enableValue();
    dataPacket.push_back(VCU_cooling_system_switch_enable);
    packlength = packlength + 1; //45
    VCU_request_torque_to_MCU = getVCU_request_torque_to_MCUValue();
    dataPacket.push_back((VCU_request_torque_to_MCU >> 8) & 0xFF);
    dataPacket.push_back(VCU_request_torque_to_MCU & 0xFF);
    packlength = packlength + 2; //47
    power_system_READY_status = getpower_system_READY_statusValue();
    dataPacket.push_back(power_system_READY_status);
    packlength = packlength + 1; //48
    memset(VCU_hardware_version,0,sizeof(VCU_hardware_version));
    uint16_t VCU_hardware_version_v = getVCU_hardware_versionValue();
    VCU_hardware_version[1] = (VCU_hardware_version_v >> 8) & 0xFF;
    VCU_hardware_version[0] = VCU_hardware_version_v & 0xFF;
    for(auto i = 0; i<7;i++){
        dataPacket.push_back(VCU_hardware_version[6-i]);
    }
    packlength = packlength + 7; //55
    memset(VCU_software_version,0,sizeof(VCU_software_version));
    uint16_t VCU_software_version_v = getVCU_software_versionValue();
    VCU_software_version[1] = (VCU_software_version_v >> 8) & 0xFF;
    VCU_software_version[0] = VCU_software_version_v & 0xFF;
    for(auto i = 0; i<7;i++){
        dataPacket.push_back(VCU_software_version[6-i]);
    }
    packlength = packlength + 7; //62
    memset(BMS_software_version,0,sizeof(BMS_software_version));
    uint16_t BMS_software_version_v = getBMS_software_versionValue();
    BMS_software_version[3] = (BMS_software_version_v >> 24) & 0xFF;
    BMS_software_version[2] = (BMS_software_version_v >> 16) & 0xFF;
    BMS_software_version[1] = (BMS_software_version_v >> 8) & 0xFF;
    BMS_software_version[0] = BMS_software_version_v & 0xFF;
    for(auto i = 0; i<7;i++){
        dataPacket.push_back(BMS_software_version[6-i]);
    }
    packlength = packlength + 7; //69
    memset(OBC_hardware_version,0,sizeof(OBC_hardware_version));
    uint16_t OBC_hardware_version_v = getOBC_hardware_versionValue();
    OBC_hardware_version[1] = (OBC_hardware_version_v >> 8) & 0xFF;
    OBC_hardware_version[0] = OBC_hardware_version_v & 0xFF;
    for(auto i = 0; i<7;i++){
        dataPacket.push_back(OBC_hardware_version[6-i]);
    }
    packlength = packlength + 7; //76
    memset(OBC_software_version,0,sizeof(OBC_software_version));
    uint16_t OBC_software_version_v = getOBC_software_versionValue();
    OBC_software_version[1] = (OBC_software_version_v >> 8) & 0xFF;
    OBC_software_version[0] = OBC_software_version_v & 0xFF;
    for(auto i = 0; i<7;i++){
        dataPacket.push_back(OBC_software_version[6-i]);
    }
    packlength = packlength + 7; //83
    air_conditioning_cooling_power = getair_conditioning_cooling_powerValue();
    dataPacket.push_back(air_conditioning_cooling_power);
    packlength = packlength + 1; //84
    vacuum_pump_system_fault_flag = getvacuum_pump_system_fault_flagValue();
    dataPacket.push_back(vacuum_pump_system_fault_flag);
    packlength = packlength + 1; //85
	
//    uint8_t air_conditioning_cooling_power;    // 空调制冷功率
//    uint8_t vacuum_pump_system_fault_flag;       // 真空泵系统故障标志

    dataPacket[1] = (packlength >> 8) & 0xFF;
    dataPacket[2] = packlength & 0xFF;
    return dataPacket;
}