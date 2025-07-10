#include "HavcStatus.h"

int HavcStatus::getair_conditioning_switchValue() {
    uint8_t val1 = 0xff;
    uint8_t value = 0xff;
    val1 = getValue(toInt(VehicleProperty::HVAC_POWER_ON));
    switch(val1){
        case 0:
        value = OFF;
        break;
        case 1:
        value = ON;
        break;
        default:
        value = INVALID;
        break;
    }
    return value;
}
int HavcStatus::getcirculation_modeValue() {
    return getValue(toInt(VehicleProperty::HVAC_RECIRC_ON));
}
int HavcStatus::getair_outlet_modeValue() {
    return getValue(toInt(VehicleProperty::HVAC_FAN_DIRECTION));
}
int HavcStatus::getcooling_switchValue() {
    return getValue(toInt(VehicleProperty::HVAC_AC_ON));
}
int HavcStatus::getheating_switchValue() {
    return getValue(toInt(VehicleProperty::GECKO_HVAC_PTC_HEAT_ON));
}
int HavcStatus::getdefrosting_switchValue() {
    return getValue(toInt(VehicleProperty::HVAC_DEFROSTER));
}
int HavcStatus::getfan_speed_adjustmentValue() {
    return getValue(toInt(VehicleProperty::HVAC_FAN_SPEED));
}
int HavcStatus::gettemperature_adjustmentValue() {
    return getValue(toInt(VehicleProperty::HVAC_TEMPERATURE_SET));
}

std::vector<uint8_t> HavcStatus::generateDataPacket() {
    std::vector<uint8_t> dataPacket;
    dataPacket.push_back(HavcStatusId);
    uint16_t packlength = 0;
    dataPacket.push_back((packlength >> 8) & 0xFF);
    dataPacket.push_back(packlength & 0xFF);
    packlength = 0;
    air_conditioning_switch = getair_conditioning_switchValue();
    dataPacket.push_back(air_conditioning_switch);
    packlength = packlength + 1;
    circulation_mode = getcirculation_modeValue();
    dataPacket.push_back(circulation_mode);
    packlength = packlength + 1;
    air_outlet_mode = getair_outlet_modeValue();
    dataPacket.push_back(air_outlet_mode);
    packlength = packlength + 1;
    cooling_switch = getcooling_switchValue();
    dataPacket.push_back(cooling_switch);
    packlength = packlength + 1;
    heating_switch = getheating_switchValue();
    dataPacket.push_back(heating_switch);
    packlength = packlength + 1;
    //当前有两个域，需要明确清楚到底是117，还是1,2
    defrosting_switch = getdefrosting_switchValue();
    dataPacket.push_back(defrosting_switch);
    packlength = packlength + 1;
    fan_speed_adjustment = getfan_speed_adjustmentValue();
    dataPacket.push_back(fan_speed_adjustment);
    packlength = packlength + 1;
    temperature_adjustment = gettemperature_adjustmentValue();
    dataPacket.push_back(temperature_adjustment);
    packlength = packlength + 1;
    uint8_t temp = INVALID;
    dataPacket.push_back(temp);
    packlength = packlength + 1;

    dataPacket[1] = (packlength >> 8) & 0xFF;
    dataPacket[2] = packlength & 0xFF;
    return dataPacket;
}