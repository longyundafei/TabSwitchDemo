#include "VehicleStatusMessage.h"
#define PROPERTY_VALUE_MAX_LEN 250

int VehicleStatusMessage::getvehicle_anti_theft_statusValue() {
    return getValue(toInt(VehicleProperty::GECKO_BCM_VEHALRMSYSST));
}
int VehicleStatusMessage::gethigh_pressure_statusValue() {
    return getValue(toInt(VehicleProperty::GECKO_VCU_HVPOWER));
}
int VehicleStatusMessage::getprogram_version_control_statusValue() {
    return getValue(toInt(VehicleProperty::GECKO_MCU_CCM_VERSION));
}
int VehicleStatusMessage::getlock_vehicle_control_flagValue() {
    //return getValue(toInt(VehicleProperty::GECKO_CCM_VERSION));
    //return (int)mReqSpeedLimitLevel;
    int level = 0;
    char ret[PROPERTY_VALUE_MAX_LEN] = "\0";
    if(property_get("persist.tbox.speedlimitlevel", ret, nullptr)!=0) {
        level = std::stoi(ret);
        //LOG(INFO) << "getlock_vehicle_control_flagValue 1:" << level;
        if(level ==1){
            memset(ret, 0, sizeof(ret));
            if(property_get("persist.tbox.chargingcondition", ret, nullptr)!=0){
                int charging = std::stoi(ret);
                //LOG(INFO) << "getlock_vehicle_control_flagValue 2:" << charging;
                if(charging == 1){
                    level = 6;
                }
            }
        }
    }
    //LOG(INFO) << "getlock_vehicle_control_flagValue:" << level;
    return level;
}
int VehicleStatusMessage::getlock_vehicle_control_statusValue() {
    int level = getValue(toInt(VehicleProperty::GECKO_MCU_SPEED_LIMIT_LEVEL));
    if(level == 1){
        int charging = getValue(toInt(VehicleProperty::GECKO_MCU_CHARGING_CONDIONS));
        if(charging == 1){
            level = 6;
        }
    }
    //LOG(INFO) << "getlock_vehicle_control_statusValue:" << level;
    return level;
}
int VehicleStatusMessage::getmaintenance_reminderValue() {
    int ret = getValue(toInt(VehicleProperty::GECKO_VEHICLE_REMIND_WARNING_ST));
    if(ret != 0) {
        ret = 1;
    }
    return ret;
}
int VehicleStatusMessage::getpayment_reminderValue() {
    int ret = getValue(toInt(VehicleProperty::GECKO_VEHICLE_PAY_WARNING_ST));
    if(ret != 0) {
        ret = 1;
    }
    return ret;
}
int VehicleStatusMessage::getannual_review_reminderValue() {
    int ret = getValue(toInt(VehicleProperty::GECKO_VEHICLE_ANNUAL_REVIEW_WARNING_ST));
    if(ret != 0) {
        ret = 1;
    }
    return ret;
}
int VehicleStatusMessage::getbattery_maintenance_flagValue() {
    return getValue(toInt(VehicleProperty::GECKO_CCM_BATTMAINTAIN_CTR_CMD));
}
int VehicleStatusMessage::getbattery_maintenance_statusValue() {
    return getValue(toInt(VehicleProperty::GECKO_CCM_BATTMAINTAIN_STS));
}
int VehicleStatusMessage::getremaining_rangeValue() {
    return getValue(toInt(VehicleProperty::GECKO_MCU_DISPLAY_RANGE));
}
int VehicleStatusMessage::getremaining_SOCValue() {
    return getValue(toInt(VehicleProperty::EV_BATTERY_LEVEL));
}
int VehicleStatusMessage::getcharge_discharge_gun_connection_statusValue() {
    return getValue(toInt(VehicleProperty::GECKO_VCU_CHRGCNCTRSTS));
}
int VehicleStatusMessage::getcharging_statusValue() {
    return getRawValue(toInt(VehicleProperty::GECKO_VCU_CHRG_STS));
}
int VehicleStatusMessage::getcharging_target_SOCValue() {
    return getValue(toInt(VehicleProperty::GECKO_VCU_CHRG_SOC_TAR_SP));
}
int VehicleStatusMessage::getremaining_charging_timeValue() {
    return getValue(toInt(VehicleProperty::GECKO_EV_CHARGE_TIME_REMAINING));
}
int VehicleStatusMessage::getcharging_currentValue() {
    int current = getValue(toInt(VehicleProperty::GECKO_EV_BATTERY_CURRENT)) - 10000;
    current = (int)fabs(current)*0.1;
    return current;
}
int VehicleStatusMessage::getcharging_voltageValue() {
    //return getValue(toInt(VehicleProperty::GECKO_EV_BATTERY_VOLTAGE));
    return getRawPropValue(toInt(VehicleProperty::GECKO_EV_BATTERY_VOLTAGE));
}
int VehicleStatusMessage::gettire_pressureValue() {
    /*
    left_front_tire_pressure = getRawValue(toInt(VehicleProperty::TIRE_PRESSURE),1);
    right_front_tire_pressure = getRawValue(toInt(VehicleProperty::TIRE_PRESSURE),2);
    left_rear_tire_pressure = getRawValue(toInt(VehicleProperty::TIRE_PRESSURE),4);
    right_rear_tire_pressure = getRawValue(toInt(VehicleProperty::TIRE_PRESSURE),8);
    */
    left_front_tire_pressure = getRawPropFloatToIntValue(toInt(VehicleProperty::TIRE_PRESSURE),1);
    right_front_tire_pressure = getRawPropFloatToIntValue(toInt(VehicleProperty::TIRE_PRESSURE),2);
    left_rear_tire_pressure = getRawPropFloatToIntValue(toInt(VehicleProperty::TIRE_PRESSURE),4);
    right_rear_tire_pressure = getRawPropFloatToIntValue(toInt(VehicleProperty::TIRE_PRESSURE),8);
    return 0;
}

/*void VehicleStatusMessage::setSpeedLimitLevel(uint8_t level) {
    switch(level){
        case 0x55:
            mReqSpeedLimitLevel = 0x01;
            break;
        case 0x56:
            mReqSpeedLimitLevel = 0x02;
            break;
        case 0x57:
            mReqSpeedLimitLevel = 0x03;
            break;
        case 0x58:
            mReqSpeedLimitLevel = 0x04;
            break;
        case 0x59:
            mReqSpeedLimitLevel = 0x05;
            break;
        case 0x60:
            mReqSpeedLimitLevel = 0x06;
			break;
        case 0xaa:
            mReqSpeedLimitLevel = 0x00;
            break;
        default:
            LOG(INFO) << "VehicleStatusMessage cannot handle SpeedLimitLevel msg, do nothing ";
            break;
    }
    LOG(INFO) << "VehicleStatusMessage setSpeedLimitLevel = " << mReqSpeedLimitLevel;
}*/

std::vector<uint8_t> VehicleStatusMessage::generateDataPacket() {
    std::vector<uint8_t> dataPacket;
    dataPacket.push_back(VehicleStatusMessageId);
    uint16_t packlength = 0;
    dataPacket.push_back((packlength >> 8) & 0xFF);
    dataPacket.push_back(packlength & 0xFF);
    packlength = 0;

    vehicle_anti_theft_status = getvehicle_anti_theft_statusValue();
    dataPacket.push_back(vehicle_anti_theft_status);
    packlength = packlength + 1;
    high_pressure_status = gethigh_pressure_statusValue();
    dataPacket.push_back(high_pressure_status);
    packlength = packlength + 1;
    program_version_control_status = getprogram_version_control_statusValue();
    dataPacket.push_back(program_version_control_status);
    packlength = packlength + 1;
    lock_vehicle_control_flag = getlock_vehicle_control_flagValue();
    dataPacket.push_back(lock_vehicle_control_flag);
    packlength = packlength + 1;
    lock_vehicle_control_status = getlock_vehicle_control_statusValue();
    dataPacket.push_back(lock_vehicle_control_status);
    packlength = packlength + 1;
    //当前未实现，需要补充
    maintenance_reminder = getmaintenance_reminderValue();
    dataPacket.push_back(maintenance_reminder);
    packlength = packlength + 1;
    payment_reminder = getpayment_reminderValue();
    dataPacket.push_back(payment_reminder);
    packlength = packlength + 1;
    annual_inspection_reminder = getannual_review_reminderValue();
    dataPacket.push_back(annual_inspection_reminder);
    packlength = packlength + 1;
    battery_maintenance_flag = getbattery_maintenance_flagValue();
    battery_maintenance_flag = 0xff;
    dataPacket.push_back(battery_maintenance_flag);
    packlength = packlength + 1;
    battery_maintenance_status = getbattery_maintenance_statusValue();
    battery_maintenance_status = 0xff;
    dataPacket.push_back(battery_maintenance_status);
    packlength = packlength + 1;
    remaining_range = getremaining_rangeValue();
    dataPacket.push_back((remaining_range >> 8) & 0xFF);
    dataPacket.push_back(remaining_range & 0xFF);
    packlength = packlength + 2;
    remaining_SOC = getremaining_SOCValue();
    dataPacket.push_back(remaining_SOC);
    packlength = packlength + 1;
    charge_discharge_gun_connection_status = getcharge_discharge_gun_connection_statusValue();
    dataPacket.push_back(charge_discharge_gun_connection_status);
    packlength = packlength + 1;
    charging_status = getcharging_statusValue();
    dataPacket.push_back(charging_status);
    packlength = packlength + 1;
    if(charging_status == 0x02 || charging_status == 0x03){
        charging_target_SOC = getcharging_target_SOCValue();
        remaining_charging_time = getremaining_charging_timeValue();
        charging_current = getcharging_currentValue();
        charging_voltage = getcharging_voltageValue();
    } else {
        charging_target_SOC = 0xFF;
        remaining_charging_time = 0xFFFF;
        charging_current = 0xFF;
        charging_voltage = 0xFFFF;
    }
    dataPacket.push_back(charging_target_SOC);
    packlength = packlength + 1;
    dataPacket.push_back((remaining_charging_time >> 8) & 0xFF);
    dataPacket.push_back(remaining_charging_time & 0xFF);
    packlength = packlength + 2;
    dataPacket.push_back(charging_current);
    packlength = packlength + 1;
    dataPacket.push_back((charging_voltage >> 8) & 0xFF);
    dataPacket.push_back(charging_voltage & 0xFF);
    packlength = packlength + 2;
    gettire_pressureValue();
    dataPacket.push_back(left_front_tire_pressure);
    packlength = packlength + 1;
    dataPacket.push_back(right_front_tire_pressure);
    packlength = packlength + 1;
    dataPacket.push_back(left_rear_tire_pressure);
    packlength = packlength + 1;
    dataPacket.push_back(right_rear_tire_pressure);
    packlength = packlength + 1;
    uint8_t temp = 0xff;
    dataPacket.push_back(temp);
    dataPacket.push_back(temp);
    dataPacket.push_back(temp);
    dataPacket.push_back(temp);
    packlength = packlength + 4;
    dataPacket[1] = (packlength >> 8) & 0xFF;
    dataPacket[2] = packlength & 0xFF;
    return dataPacket;
}