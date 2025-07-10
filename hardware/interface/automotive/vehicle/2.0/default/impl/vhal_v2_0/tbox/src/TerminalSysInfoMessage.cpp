#include "TerminalSysInfoMessage.h"
int TerminalSysInfoMessage::getCountryCodeValue() {
    return 0xFF;//getValue(toInt(VehicleProperty::GECKO_HU_NAVICOUNTRYCODE));
}
int TerminalSysInfoMessage::getCountryCodeIndValue() {
    return getValue(toInt(VehicleProperty::GECKO_HU_NAV_COUNTRYID));
}
/*int TerminalSysInfoMessage::getRemotePowerCtrlReqValue() {
    return getValue(toInt(VehicleProperty::GECKO_CCU_REMTPOWERCTRLFB));
}
int TerminalSysInfoMessage::getRemoteSteerWhlHeatReqValue() {
    return getValue(toInt(VehicleProperty::GECKO_CCU_REMTSTEERWHLHEATFB));
}
int TerminalSysInfoMessage::getRemoteMainDrSeatHeatReqValue() {
    return getValue(toInt(VehicleProperty::GECKO_CCU_REMTMAIDRSEATHEATFB));
}*/
int TerminalSysInfoMessage::getACPowerCtrlValue() {
    return getValue(toInt(VehicleProperty::HVAC_POWER_ON));
}
int TerminalSysInfoMessage::getECOModeSetValue() {
    return getValue(toInt(VehicleProperty::GECKO_HVAC_HYBIRD_ECO_MODE));
}
int TerminalSysInfoMessage::getTempSetValue() {
    return getValue(toInt(VehicleProperty::HVAC_TEMPERATURE_SET));
}
int TerminalSysInfoMessage::getAIRInletModeSetValue() {
    return getValue(toInt(VehicleProperty::HVAC_RECIRC_ON));
}
int TerminalSysInfoMessage::getFanSpdSetValue() {
    return getValue(toInt(VehicleProperty::HVAC_FAN_SPEED));
}
int TerminalSysInfoMessage::getDefrostCtrlValue() {
    return getValue(toInt(VehicleProperty::HVAC_DEFROSTER));
}
int TerminalSysInfoMessage::getBlowerModeSetValue() {
    return getValue(toInt(VehicleProperty::HVAC_FAN_DIRECTION));
}
int TerminalSysInfoMessage::getACAutoCtlValue() {
    return getValue(toInt(VehicleProperty::HVAC_AC_ON));
}
int TerminalSysInfoMessage::getVentilationValue() {
    return getValue(toInt(VehicleProperty::GECKO_CCU_OPENVENTILATIONFB));
}
int TerminalSysInfoMessage::getACRemoteCtlFlagValue() {
    return getValue(toInt(VehicleProperty::GECKO_CCM_REMT_CTR_FLAG));
}
int TerminalSysInfoMessage::getACTempSetValue() {
    return getValue(toInt(VehicleProperty::HVAC_TEMPERATURE_SET));
}
int TerminalSysInfoMessage::getRemoteDefrostCtlValue() {
    return getValue(toInt(VehicleProperty::GECKO_AC_REMOTEDFROSTCTLFB));
}
std::vector<uint8_t> TerminalSysInfoMessage::generateDataPacket() {
    std::vector<uint8_t> dataPacket;
    dataPacket.push_back(TerminalSysInfoId);
    uint16_t packlength = 0;
    dataPacket.push_back((packlength >> 8) & 0xFF);
    dataPacket.push_back(packlength & 0xFF);
    packlength = 0;
    //当前国家
    country_code = getCountryCodeValue();
    dataPacket.push_back(country_code);
    packlength = packlength + 1;
    //指示当前所在国家
    country_id = getCountryCodeIndValue();
    dataPacket.push_back(country_id);
    packlength = packlength + 1;
    //远程上下电请求
    //remt_power_ctrl_req = getRemotePowerCtrlReqValue();
    dataPacket.push_back(remt_power_ctrl_req);
    packlength = packlength + 1;
    //远程开关方向盘加热请求
    //remt_steer_whl_heat_req = getRemoteSteerWhlHeatReqValue();
    dataPacket.push_back(remt_steer_whl_heat_req);
    packlength = packlength + 1;
    //远程开关座椅加热加热请求
    //remt_mai_dr_seat_heat_req = getRemoteMainDrSeatHeatReqValue();
    dataPacket.push_back(remt_mai_dr_seat_heat_req);
    packlength = packlength + 1;
    //HU空调开/关控制信号
    ac_power_ctl = getACPowerCtrlValue();
    dataPacket.push_back(ac_power_ctl);
    packlength = packlength + 1;
    //HU软开关空调运行模式设置信号
    hybrid_eco_mode_set = getECOModeSetValue();
    dataPacket.push_back(hybrid_eco_mode_set);
    packlength = packlength + 1;
    //HU软开关空调温度设置信号
    left_temp_set = getTempSetValue();
    dataPacket.push_back(left_temp_set);
    packlength = packlength + 1;
    //空调内/外循环调节开关设置信号
    air_inlet_mode_set = getAIRInletModeSetValue();
    dataPacket.push_back(air_inlet_mode_set);
    packlength = packlength + 1;
    //HU空调风量挡位调节信号
    front_fan_spd_set = getFanSpdSetValue();
    dataPacket.push_back(front_fan_spd_set);
    packlength = packlength + 1;
    //HU强制除霜开关
    manual_defrost_ctl = getDefrostCtrlValue();
    dataPacket.push_back(manual_defrost_ctl);
    packlength = packlength + 1;
    //HU空调出风模式设置信号
    blower_mode_set = getBlowerModeSetValue();
    dataPacket.push_back(blower_mode_set);
    packlength = packlength + 1;
    //HU空调AUTO开关
    ac_auto_ctl = getACAutoCtlValue();
    dataPacket.push_back(ac_auto_ctl);
    packlength = packlength + 1;
    //远程通风
    open_ventilation = getVentilationValue();
    dataPacket.push_back(open_ventilation);
    packlength = packlength + 1;
    //远程空调
    ac_remt_ctrl_flag = getACRemoteCtlFlagValue();
    dataPacket.push_back(ac_remt_ctrl_flag);
    packlength = packlength + 1;
    //远程空调温度设置
    ac_temp_set =getACTempSetValue();
    dataPacket.push_back(ac_temp_set);
    packlength = packlength + 1;
    //远程强制除霜
    remote_defrost_ctl = getRemoteDefrostCtlValue();
    dataPacket.push_back(remote_defrost_ctl);
    packlength = packlength + 1;
    //车主选择的续航里程persist.gecko.rangemode
    char propValue[PROPERTY_VALUE_MAX_LEN] = "\0";
    if(property_get("persist.gecko.rangemode", propValue, nullptr)!=0){
        std::string mode = propValue;
        if(mode == "1"){
            drive_range_mode = 1;
        } else {
            drive_range_mode = 0;
        }
    }
    LOG(INFO) << "drive_range_mode = " << static_cast<int>(drive_range_mode);
    dataPacket.push_back(drive_range_mode);
    packlength = packlength + 1;

    dataPacket[1] = (packlength >> 8) & 0xFF;
    dataPacket[2] = packlength & 0xFF;
    return dataPacket;
}