#include "ACSysInfoMessage.h"
int ACSysInfoMessage::getLeftTempValue() {
    return getValue(toInt(VehicleProperty::HVAC_TEMPERATURE_SET));
}
int ACSysInfoMessage::getManualDefrostFBValue() {
    return getValue(toInt(VehicleProperty::HVAC_DEFROSTER));
}
int ACSysInfoMessage::getACAutoFBValue() {
    return getValue(toInt(VehicleProperty::HVAC_AC_ON));
}
int ACSysInfoMessage::getBlowerModeValue() {
    return getValue(toInt(VehicleProperty::HVAC_FAN_DIRECTION));
}
int ACSysInfoMessage::getFanSpdValue() {
    return getValue(toInt(VehicleProperty::HVAC_FAN_SPEED));
}
int ACSysInfoMessage::getACPowerFBValue() {
    return getValue(toInt(VehicleProperty::HVAC_POWER_ON));
}
int ACSysInfoMessage::getAutoBlowerModeValue() {
    return getValue(toInt(VehicleProperty::GECKO_AC_AUTOBLOWERMODESTA));
}
int ACSysInfoMessage::getHybridECOModeValue() {
    return getValue(toInt(VehicleProperty::GECKO_HVAC_HYBIRD_ECO_MODE));
}
int ACSysInfoMessage::getAirInletModeValue() {
    return getValue(toInt(VehicleProperty::HVAC_RECIRC_ON));
}
int ACSysInfoMessage::getRearViewMirrorHeatingStValue() {
    return getValue(toInt(VehicleProperty::GECKO_BCM_REAR_VIEW_MIRR_HEATING_STS));
}
int ACSysInfoMessage::getExtendedDefrostActiveReqValue() {
    return getValue(toInt(VehicleProperty::GECKO_AC_EXTDDEFROSTACTVREQ));
}
int ACSysInfoMessage::getEnvirTempValValue() {
    return getValue(toInt(VehicleProperty::GECKO_AC_ENVIR_TEMPV));
}
int ACSysInfoMessage::getIndoorTempValue() {
    return getValue(toInt(VehicleProperty::GECKO_AC_INDOORTEMP));
}
int ACSysInfoMessage::getEnvirTempValue() {
    return getValue(toInt(VehicleProperty::GECKO_AC_ENVIR_TEMP));
}
int ACSysInfoMessage::getOHXOutTempValue() {
    return getValue(toInt(VehicleProperty::GECKO_TMS_OHX_OUT_TMP));
}
int ACSysInfoMessage::getEvapTempValue() {
    return getValue(toInt(VehicleProperty::GECKO_TMS_EVAP_TMP));
}
int ACSysInfoMessage::getEvapOutTempValue() {
    return getValue(toInt(VehicleProperty::GECKO_TMS_EVAP_OUT_TMP));
}
int ACSysInfoMessage::getChillerOutTempValue() {
    return getValue(toInt(VehicleProperty::GECKO_TMS_CHILLER_OUT_TMP));
}

std::vector<uint8_t> ACSysInfoMessage::generateDataPacket() {
    std::vector<uint8_t> dataPacket;
    dataPacket.push_back(ACSysInfoId);
    uint16_t packlength = 0;
    dataPacket.push_back((packlength >> 8) & 0xFF);
    dataPacket.push_back(packlength & 0xFF);
    packlength = 0;
    //左区温度反馈
    left_temp = getLeftTempValue();
    dataPacket.push_back(left_temp);
    packlength = packlength + 1;
    //手动强制除霜开关反馈
    manual_defrost_fb = getManualDefrostFBValue();
    dataPacket.push_back(manual_defrost_fb);
    packlength = packlength + 1;
    //AC AUTO状态
    ac_auto_fb = getACAutoFBValue();
    dataPacket.push_back(ac_auto_fb);
    packlength = packlength + 1;
    //出风模式状态
    blower_mode = getBlowerModeValue();
    dataPacket.push_back(blower_mode);
    packlength = packlength + 1;
    //风量档位实际值
    front_fan_spd = getFanSpdValue();
    dataPacket.push_back(front_fan_spd);
    packlength = packlength + 1;
    //AC Power状态反馈
    ac_power_fb = getACPowerFBValue();
    dataPacket.push_back(ac_power_fb);
    packlength = packlength + 1;
    //空调在自动模式下的吹风状态
    auto_blower_mode = getAutoBlowerModeValue();
    dataPacket.push_back(auto_blower_mode);
    packlength = packlength + 1;
    //空调模式设置状态
    hybrid_eco_mode = getHybridECOModeValue();
    dataPacket.push_back(hybrid_eco_mode);
    packlength = packlength + 1;
    //内外循环设置状态
    air_inlet_mode = getAirInletModeValue();
    dataPacket.push_back(air_inlet_mode);
    packlength = packlength + 1;
    //后视镜加热状态
    rear_view_mirr_heating_sts = getRearViewMirrorHeatingStValue();
    dataPacket.push_back(rear_view_mirr_heating_sts);
    packlength = packlength + 1;
    //强制除霜请求
    extd_defrost_actv_req = getExtendedDefrostActiveReqValue();
    dataPacket.push_back(extd_defrost_actv_req);
    packlength = packlength + 1;
    //环境温度有效性
    envir_tempV = getEnvirTempValValue();
    dataPacket.push_back(envir_tempV);
    packlength = packlength + 1;
    //室内温度
    indoor_temp = getIndoorTempValue();
    dataPacket.push_back(indoor_temp);
    packlength = packlength + 1;
    //环境温度
    envir_temp = getEnvirTempValue();
    dataPacket.push_back(envir_temp);
    packlength = packlength + 1;
    //室外换热器出口温度
    ohx_out_tmp = getOHXOutTempValue();
    dataPacket.push_back(ohx_out_tmp);
    packlength = packlength + 1;
    //蒸发器表面温度
    evap_tmp = getEvapTempValue();
    dataPacket.push_back(evap_tmp);
    packlength = packlength + 1;
    //蒸发器出口温度
    evap_out_tmp = getEvapOutTempValue();
    dataPacket.push_back(evap_out_tmp);
    packlength = packlength + 1;
    //chiller出口温度
    chiller_out_tmp = getChillerOutTempValue();
    dataPacket.push_back(chiller_out_tmp);
    packlength = packlength + 1;

    dataPacket[1] = (packlength >> 8) & 0xFF;
    dataPacket[2] = packlength & 0xFF;
    return dataPacket;
}