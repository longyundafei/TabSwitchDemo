#include "ReverseAssistantSysInfoMessage.h"
int ReverseAssistantSysInfoMessage::getRLDistValue() {
    return getValue(toInt(VehicleProperty::GECKO_PP_RLDIST));
}
int ReverseAssistantSysInfoMessage::getRLMDistValue() {
    return getValue(toInt(VehicleProperty::GECKO_PP_RLMDIST));
}
int ReverseAssistantSysInfoMessage::getRRMDistValue() {
    return getValue(toInt(VehicleProperty::GECKO_PP_RRMDIST));
}
int ReverseAssistantSysInfoMessage::getRRDistValue() {
    return getValue(toInt(VehicleProperty::GECKO_PP_RRDIST));
}
int ReverseAssistantSysInfoMessage::getSDWRSLDistValue() {
    int validat = getValue(toInt(VehicleProperty::GECKO_PP_SENSOR_FAULT_STS_RL));
    if(validat == 0x00){
        return getValue(toInt(VehicleProperty::GECKO_PP_SDWRSL_DIST));
    } else {
        return 0xFFFF;
    }
}
int ReverseAssistantSysInfoMessage::getSDWRSLMidDistValue() {
    int validat = getValue(toInt(VehicleProperty::GECKO_PP_SENSOR_FAULT_STS_RLM));
    if(validat == 0x00){
        return getValue(toInt(VehicleProperty::GECKO_PP_SDWRSL_MID_DIST));
    } else {
        return 0xFFFF;
    }
}
int ReverseAssistantSysInfoMessage::getSDWRSRMidDistValue() {
    int validat = getValue(toInt(VehicleProperty::GECKO_PP_SENSOR_FAULT_STS_RRM));
    if(validat == 0x00){
        return getValue(toInt(VehicleProperty::GECKO_PP_SDWRSR_MID_DIST));
    } else {
        return 0xFFFF;
    }
}
int ReverseAssistantSysInfoMessage::getSDWRSRDistValue() {
    int validat = getValue(toInt(VehicleProperty::GECKO_PP_SENSOR_FAULT_STS_RR));
    if(validat == 0x00){
        return getValue(toInt(VehicleProperty::GECKO_PP_SDWRSRDIST));
    } else {
        return 0xFFFF;
    }
}
int ReverseAssistantSysInfoMessage::getFLDistValue() {
    return getValue(toInt(VehicleProperty::GECKO_PP_FLDIST));
}
int ReverseAssistantSysInfoMessage::getFMLDistValue() {
    return getValue(toInt(VehicleProperty::GECKO_PP_FMLDIST));
}
int ReverseAssistantSysInfoMessage::getFMRDistValue() {
    return getValue(toInt(VehicleProperty::GECKO_PP_FMRDIST));
}
int ReverseAssistantSysInfoMessage::getFRDistValue() {
    return getValue(toInt(VehicleProperty::GECKO_PP_FRDIST));
}
int ReverseAssistantSysInfoMessage::getSDWFSLDistValue() {
    int validat = getValue(toInt(VehicleProperty::GECKO_PP_SENSORFAULTSTSFL));
    if(validat == 0x00){
        return getValue(toInt(VehicleProperty::GECKO_PP_SDWFSLDIST));
    } else {
        return 0xFFFF;
    }
}
int ReverseAssistantSysInfoMessage::getSDWFSLMidDistValue() {
    int validat = getValue(toInt(VehicleProperty::GECKO_PP_SENSORFAULTSTSFLM));
    if(validat == 0x00){
        return getValue(toInt(VehicleProperty::GECKO_PP_SDWFSLMIDDIST));
    } else {
        return 0xFFFF;
    }
}
int ReverseAssistantSysInfoMessage::getSDWFSRMidDistValue() {
    int validat = getValue(toInt(VehicleProperty::GECKO_PP_SENSORFAULTSTSFRM));
    if(validat == 0x00){
        return getValue(toInt(VehicleProperty::GECKO_PP_SDWFSRMIDDIST));
    } else {
        return 0xFFFF;
    }
}
int ReverseAssistantSysInfoMessage::getSDWFSRDistValue() {
    int validat = getValue(toInt(VehicleProperty::GECKO_PP_SENSORFAULTSTSFR));
    if(validat == 0x00){
        return getValue(toInt(VehicleProperty::GECKO_PP_SDWFSRDIST));
    } else {
        return 0xFFFF;
    }
}
int ReverseAssistantSysInfoMessage::getBuzzerAlarmPatternValue() {
    return getValue(toInt(VehicleProperty::GECKO_PDC1_BUZZER_ALARM_PATTERN));
}
int ReverseAssistantSysInfoMessage::getSwitchStFBValue() {
    return getValue(toInt(VehicleProperty::GECKO_PP_SWFDBK));
}
int ReverseAssistantSysInfoMessage::getPPStValue() {
    return getValue(toInt(VehicleProperty::GECKO_PP_STA));
}
int ReverseAssistantSysInfoMessage::getRAEBWorkStValue() {
    return getValue(toInt(VehicleProperty::GECKO_PP_RAEB_WORK_STS));
}
int ReverseAssistantSysInfoMessage::getRAEBSetFBValue() {
    return getValue(toInt(VehicleProperty::GECKO_PP_RAEB_SET_FB));
}
int ReverseAssistantSysInfoMessage::getRAEBBrkgReqValue() {
    return getValue(toInt(VehicleProperty::GECKO_PP_RAEB_BRKG_REQ));
}
int ReverseAssistantSysInfoMessage::getRAEBBrkgValue() {
    return getValue(toInt(VehicleProperty::GECKO_PP_RAEB_BRKG_REQ_VAL));
}
int ReverseAssistantSysInfoMessage::getRAEBDistToCollisionValue() {
    return getValue(toInt(VehicleProperty::GECKO_PP_RAEB_DISTANCE_TO_COLLISION));
}
std::vector<uint8_t> ReverseAssistantSysInfoMessage::generateDataPacket() {
    std::vector<uint8_t> dataPacket;
    dataPacket.push_back(ReverseAssistantSysInfoId);
    uint16_t packlength = 0;
    dataPacket.push_back((packlength >> 8) & 0xFF);
    dataPacket.push_back(packlength & 0xFF);
    packlength = 0;
    //倒车雷达后左雷达障碍物距离
    rl_dist = getRLDistValue();
    dataPacket.push_back((rl_dist >> 8) & 0xFF);
    dataPacket.push_back(rl_dist & 0xFF);
    packlength = packlength + 2;
    //倒车雷达后左中雷达障碍物距离
    rlm_dist = getRLMDistValue();
    dataPacket.push_back((rlm_dist >> 8) & 0xFF);
    dataPacket.push_back(rlm_dist & 0xFF);
    packlength = packlength + 2;
    //倒车雷达后右中雷达障碍物距离
    rrm_dist = getRRMDistValue();
    dataPacket.push_back((rrm_dist >> 8) & 0xFF);
    dataPacket.push_back(rrm_dist & 0xFF);
    packlength = packlength + 2;
    //倒车雷达右后雷达障碍物距离
    rr_dist = getRRDistValue();
    dataPacket.push_back((rr_dist >> 8) & 0xFF);
    dataPacket.push_back(rr_dist & 0xFF);
    packlength = packlength + 2;
    //后左侧区域障碍物距离
    sdwrsl_dist = getSDWRSLDistValue();
    dataPacket.push_back((sdwrsl_dist >> 8) & 0xFF);
    dataPacket.push_back(sdwrsl_dist & 0xFF);
    packlength = packlength + 2;
    //后左中侧区域障碍物距离
    sdwrsl_mid_dist = getSDWRSLMidDistValue();
    dataPacket.push_back((sdwrsl_mid_dist >> 8) & 0xFF);
    dataPacket.push_back(sdwrsl_mid_dist & 0xFF);
    packlength = packlength + 2;
    //后右中侧区域障碍物距离
    sdwrsr_mid_dist = getSDWRSRMidDistValue();
    dataPacket.push_back((sdwrsr_mid_dist >> 8) & 0xFF);
    dataPacket.push_back(sdwrsr_mid_dist & 0xFF);
    packlength = packlength + 2;
    //后右侧区域障碍物距离
    sdwrsr_dist = getSDWRSRDistValue();
    dataPacket.push_back((sdwrsr_dist >> 8) & 0xFF);
    dataPacket.push_back(sdwrsr_dist & 0xFF);
    packlength = packlength + 2;
    //倒车雷达左前障碍物距离
    fl_dist = getFLDistValue();
    dataPacket.push_back((fl_dist >> 8) & 0xFF);
    dataPacket.push_back(fl_dist & 0xFF);
    packlength = packlength + 2;
    //倒车雷达前左中障碍物距离
    fml_dist = getFMLDistValue();
    dataPacket.push_back((fml_dist >> 8) & 0xFF);
    dataPacket.push_back(fml_dist & 0xFF);
    packlength = packlength + 2;
    //倒车雷达前右中障碍物距离
    fmr_dist = getFMRDistValue();
    dataPacket.push_back((fmr_dist >> 8) & 0xFF);
    dataPacket.push_back(fmr_dist & 0xFF);
    packlength = packlength + 2;
    //倒车雷达右前障碍物距离
    fr_dist = getFRDistValue();
    dataPacket.push_back((fr_dist >> 8) & 0xFF);
    dataPacket.push_back(fr_dist & 0xFF);
    packlength = packlength + 2;
    //前左侧区域障碍物距离
    sdwfsl_dist = getSDWFSLDistValue();
    dataPacket.push_back((sdwfsl_dist >> 8) & 0xFF);
    dataPacket.push_back(sdwfsl_dist & 0xFF);
    packlength = packlength + 2;
    //前左中侧区域障碍物距离
    sdwfsl_mid_dist = getSDWFSLMidDistValue();
    dataPacket.push_back((sdwfsl_mid_dist >> 8) & 0xFF);
    dataPacket.push_back(sdwfsl_mid_dist & 0xFF);
    packlength = packlength + 2;
    //前右中侧区域障碍物距离
    sdwfsr_mid_dist = getSDWFSRMidDistValue();
    dataPacket.push_back((sdwfsr_mid_dist >> 8) & 0xFF);
    dataPacket.push_back(sdwfsr_mid_dist & 0xFF);
    packlength = packlength + 2;
    //前右侧区域障碍物距离
    sdwfsr_dist = getSDWFSRDistValue();
    dataPacket.push_back((sdwfsr_dist >> 8) & 0xFF);
    dataPacket.push_back(sdwfsr_dist & 0xFF);
    packlength = packlength + 2;
    //倒车雷达声音报警
    buzzer_alarm_pattern = getBuzzerAlarmPatternValue();
    dataPacket.push_back(buzzer_alarm_pattern);
    packlength = packlength + 1;
    //开关状态反馈信号
    switch_st_fb = getSwitchStFBValue();
    dataPacket.push_back(switch_st_fb);
    packlength = packlength + 1;
    //倒车雷达系统状态
    pp_sta = getPPStValue();
    dataPacket.push_back(pp_sta);
    packlength = packlength + 1;
    //后向AEB工作状态
    raeb_work_sts = getRAEBWorkStValue();
    dataPacket.push_back(raeb_work_sts);
    packlength = packlength + 1;
    //后向AEB设置反馈
    raeb_set_fb = getRAEBSetFBValue();
    dataPacket.push_back(raeb_set_fb);
    packlength = packlength + 1;
    //后向AEB激活请求
    raeb_brkg_req = getRAEBBrkgReqValue();
    dataPacket.push_back(raeb_brkg_req);
    packlength = packlength + 1;
    //后向AEB减速度值
    raeb_brkg_req_val = getRAEBBrkgValue();
    dataPacket.push_back((raeb_brkg_req_val >> 8) & 0xFF);
    dataPacket.push_back(raeb_brkg_req_val & 0xFF);
    packlength = packlength + 2;
    //后碰撞预警距离报警
    raeb_distance_to_collision = getRAEBDistToCollisionValue();
    dataPacket.push_back((raeb_distance_to_collision >> 8) & 0xFF);
    dataPacket.push_back(raeb_distance_to_collision & 0xFF);
    packlength = packlength + 2;

    dataPacket[1] = (packlength >> 8) & 0xFF;
    dataPacket[2] = packlength & 0xFF;
    return dataPacket;
}