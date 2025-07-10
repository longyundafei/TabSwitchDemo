#include "RadioSysInfoMessage.h"
int RadioSysInfoMessage::getRightRCTAWarningValue() {
    return getValue(toInt(VehicleProperty::GECKO_CRRR_RCTA_WARN_RIGHT));
}
int RadioSysInfoMessage::getRCTASetFBValue() {
    return getValue(toInt(VehicleProperty::GECKO_CRRR_RCTA_SETFB));
}
int RadioSysInfoMessage::getBSDSetFBValue() {
    return getValue(toInt(VehicleProperty::GECKO_CRRR_BSD_SETFB));
}
int RadioSysInfoMessage::getLeftBSDWarningValue() {
    return getValue(toInt(VehicleProperty::GECKO_CRRR_BSD_WARN_LEFT));
}
int RadioSysInfoMessage::getRightBSDWarningValue() {
    return getValue(toInt(VehicleProperty::GECKO_CRRR_BSD_WARN_RIGHT));
}
int RadioSysInfoMessage::getRCTBDecelarationValue() {
    return getValue(toInt(VehicleProperty::GECKO_CRRR_RCTB_DECELARATION));
}
int RadioSysInfoMessage::getRCWWarningValue() {
    return getValue(toInt(VehicleProperty::GECKO_CRRR_RCW_WARNING));
}
int RadioSysInfoMessage::getFOVChgSetFBValue() {
    return getValue(toInt(VehicleProperty::GECKO_CRRR_FOV_CHG_SET_FB));
}
int RadioSysInfoMessage::getRCTBDeceReqValue() {
    int valide = getValue(toInt(VehicleProperty::GECKO_CRRR_RCTB_BRKG_VLD));
    if(valide == 0x00){
        return getValue(toInt(VehicleProperty::GECKO_CRRR_RCTB_REQ));
    } else {
        return 0xFF;
    }
}
int RadioSysInfoMessage::getLeftRCTAWarningValue() {
    return getValue(toInt(VehicleProperty::GECKO_CRRR_RCTA_WARN_LEFT));
}
int RadioSysInfoMessage::getDOWSetFBValue() {
    return getValue(toInt(VehicleProperty::GECKO_CRRR_DOWSETFB));
}
int RadioSysInfoMessage::getRearCornerBlindnessValue() {
    return getValue(toInt(VehicleProperty::GECKO_CRRR_REARCORNERBLINDNESS));
}
int RadioSysInfoMessage::getRearCornerFailurValue() {
    return getValue(toInt(VehicleProperty::GECKO_CRRR_REARCORNERFAILURE));
}
int RadioSysInfoMessage::getRCWSetFBValue() {
    return getValue(toInt(VehicleProperty::GECKO_CRRR_RCW_SETFB));
}
int RadioSysInfoMessage::getDOWRLWarningValue() {
    return getValue(toInt(VehicleProperty::GECKO_CRRR_DOWWARNRL));
}
int RadioSysInfoMessage::getDOWRRWarningValue() {
    return getValue(toInt(VehicleProperty::GECKO_CRRR_DOWWARNRR));
}
int RadioSysInfoMessage::getDOWFLWarningValue() {
    return getValue(toInt(VehicleProperty::GECKO_CRRR_DOWWARNFL));
}
int RadioSysInfoMessage::getDOWFRWarningValue() {
    return getValue(toInt(VehicleProperty::GECKO_CRRR_DOWWARNFR));
}
int RadioSysInfoMessage::getDOWLWarningValue() {
    return getValue(toInt(VehicleProperty::GECKO_CRRR_DOW_WARN_LEFT));
}
int RadioSysInfoMessage::getDOWRWarningValue() {
    return getValue(toInt(VehicleProperty::GECKO_CRRR_DOW_WARN_RIGHT));
}
std::vector<uint8_t> RadioSysInfoMessage::generateDataPacket() {
    std::vector<uint8_t> dataPacket;
    dataPacket.push_back(RadioSysInfoId);
    uint16_t packlength = 0;
    dataPacket.push_back((packlength >> 8) & 0xFF);
    dataPacket.push_back(packlength & 0xFF);
    packlength = 0;
    //右侧RCTA报警
    rcta_warn_right = getRightRCTAWarningValue();
    dataPacket.push_back(rcta_warn_right);
    packlength = packlength + 1;
    //RCTA设置反馈
    rcta_set_fb = getRCTASetFBValue();
    dataPacket.push_back(rcta_set_fb);
    packlength = packlength + 1;
    //BSD设置反馈
    bsd_set_fb = getBSDSetFBValue();
    dataPacket.push_back(bsd_set_fb);
    packlength = packlength + 1;
    //左侧BSD报警
    bsd_warn_left = getLeftBSDWarningValue();
    dataPacket.push_back(bsd_warn_left);
    packlength = packlength + 1;
    //右侧BSD报警
    bsd_warn_right = getRightBSDWarningValue();
    dataPacket.push_back(bsd_warn_right);
    packlength = packlength + 1;
    //RCTB减速度值
    rctb_decelaration = getRCTBDecelarationValue();
    dataPacket.push_back(rctb_decelaration);
    packlength = packlength + 1;
    //RCW报警信号
    rcw_warning = getRCWWarningValue();
    dataPacket.push_back(rcw_warning);
    packlength = packlength + 1;
    //FOV模式切换请求
    fov_chg_set_fb = getFOVChgSetFBValue();
    dataPacket.push_back(fov_chg_set_fb);
    packlength = packlength + 1;
    //RCTB减速度请求
    rctb_req = getRCTBDeceReqValue();
    dataPacket.push_back(rctb_req);
    packlength = packlength + 1;
    //左侧RCTA报警
    rcta_warn_left = getLeftRCTAWarningValue();
    dataPacket.push_back(rcta_warn_left);
    packlength = packlength + 1;
    //DOW设置反馈
    dow_set_fb = getDOWSetFBValue();
    dataPacket.push_back(dow_set_fb);
    packlength = packlength + 1;
    //雷达遮挡状态
    rear_corner_blindness = getRearCornerBlindnessValue();
    dataPacket.push_back(rear_corner_blindness);
    packlength = packlength + 1;
    //雷达故障状态
    rear_corner_failure = getRearCornerFailurValue();
    dataPacket.push_back(rear_corner_failure);
    packlength = packlength + 1;
    //RCW设置反馈
    rcw_set_fb = getRCWSetFBValue();
    dataPacket.push_back(rcw_set_fb);
    packlength = packlength + 1;
    //DOW左后门报警信号
    dow_warn_rl = getDOWRLWarningValue();
    dataPacket.push_back(dow_warn_rl);
    packlength = packlength + 1;
    //DOW右后门报警信号
    dow_warn_rr = getDOWRRWarningValue();
    dataPacket.push_back(dow_warn_rr);
    packlength = packlength + 1;
    //DOW左前门报警信号
    dow_warn_fl = getDOWFLWarningValue();
    dataPacket.push_back(dow_warn_fl);
    packlength = packlength + 1;
    //DOW左前门报警信号
    dow_warn_fr = getDOWFRWarningValue();
    dataPacket.push_back(dow_warn_fr);
    packlength = packlength + 1;
    //DOW右侧报警信号
    dow_warn_left = getDOWLWarningValue();
    dataPacket.push_back(dow_warn_left);
    packlength = packlength + 1;
    //DOW左侧报警信号
    dow_warn_right = getDOWRWarningValue();
    dataPacket.push_back(dow_warn_right);
    packlength = packlength + 1;

    dataPacket[1] = (packlength >> 8) & 0xFF;
    dataPacket[2] = packlength & 0xFF;
    return dataPacket;
}