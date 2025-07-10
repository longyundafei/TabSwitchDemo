#include "IntegratedSwitchInfoMessage.h"
int IntegratedSwitchInfoMessage::getGearShiftLeverReqPosValue() {
    int valide = getValue(toInt(VehicleProperty::GECKO_SCS_GEAR_SHIFT_LEVER_PST_REQ_VD));
    if(valide == 0x01){
        return getValue(toInt(VehicleProperty::GECKO_SCS_GEAR_SHIFT_LEVER_PST_REQ));
    } else {
        return 0xFF;
    }
}
int IntegratedSwitchInfoMessage::getRight11SwitchValue() {
    int valide = getValue(toInt(VehicleProperty::GECKO_SCS_RIGHTSWTVD));
    if(valide == 0x01){
        return getValue(toInt(VehicleProperty::GECKO_SCS_RIGHT_11SWT));
    } else {
        return 0xFF;
    }
}
int IntegratedSwitchInfoMessage::getRight12SwitchValue() {
    int valide = getValue(toInt(VehicleProperty::GECKO_SCS_RIGHTSWTVD));
    if(valide == 0x01){
        return getValue(toInt(VehicleProperty::GECKO_SCS_RIGHT_12SWT));
    } else {
        return 0xFF;
    }
}
int IntegratedSwitchInfoMessage::getRight13SwitchValue() {
    int valide = getValue(toInt(VehicleProperty::GECKO_SCS_RIGHTSWTVD));
    if(valide == 0x01){
        return getValue(toInt(VehicleProperty::GECKO_SCS_RIGHT_13SWT));
    } else {
        return 0xFF;
    }
}
int IntegratedSwitchInfoMessage::getFrontWiperSwitchStValue() {
    return getValue(toInt(VehicleProperty::GECKO_SCS_FRNTWIPR_SWTST));
}
int IntegratedSwitchInfoMessage::getLowBeamSwitchStValue() {
    return getValue(toInt(VehicleProperty::GECKO_SCS_LOWBEAM_SWTST));
}
int IntegratedSwitchInfoMessage::getHiBeamSwitchStValue() {
    return getValue(toInt(VehicleProperty::GECKO_SCS_HIBEAM_SWTST));
}
int IntegratedSwitchInfoMessage::getOverVehBeamSwitchStValue() {
    return getValue(toInt(VehicleProperty::GECKO_SCS_OVERVEHBEAM_SWTST));
}
int IntegratedSwitchInfoMessage::getLampAutoStValue() {
    return getValue(toInt(VehicleProperty::GECKO_SCS_LAMP_AUTO_ST));
}
int IntegratedSwitchInfoMessage::getLeftTurnLampSwitchStValue() {
    return getValue(toInt(VehicleProperty::GECKO_SCS_LETURNLMP_SWTST));
}
int IntegratedSwitchInfoMessage::getRightTurnLampSwitchStValue() {
    return getValue(toInt(VehicleProperty::GECKO_SCS_RITURNLMP_SWTST));
}
int IntegratedSwitchInfoMessage::getDrvSeatbeltBuckleStValue() {
    int valide = getValue(toInt(VehicleProperty::GECKO_MAIN_SEAT_SAFETY_BELT_VD));
    if(valide == 0x01){
        return getValue(toInt(VehicleProperty::GECKO_MAIN_SEAT_SAFETY_BELT_STATUS));
    } else {
        return 0xFF;
    }
}
int IntegratedSwitchInfoMessage::getPassSeatbeltBuckleStValue() {
    int valide = getValue(toInt(VehicleProperty::GECKO_SRS_PASS_SEATBELT_BUCKLE_VALID));
    if(valide == 0x01){
        return getValue(toInt(VehicleProperty::GECKO_SRS_PASS_SEATBELT_BUCKLE_STATUS));
    } else {
        return 0xFF;
    }
}
int IntegratedSwitchInfoMessage::getMidSeatbeltBuckleStValue() {
    int valide = getValue(toInt(VehicleProperty::GECKO_SRS_MIDSEATBELTBUCKLEVALID));
    if(valide == 0x01){
        return getValue(toInt(VehicleProperty::GECKO_SRS_MIDSEATBELTBUCKLESTATUS));
    } else {
        return 0xFF;
    }
}
int IntegratedSwitchInfoMessage::getCrashOutputValue() {
    return getValue(toInt(VehicleProperty::GECKO_SRS_CRASHOUTPUTSTS));
}
int IntegratedSwitchInfoMessage::getMidSeatOccupantSensorStValue() {
    return getValue(toInt(VehicleProperty::GECKO_SRS_MIDSEATOCCUPANTSENSORSTS));
}
int IntegratedSwitchInfoMessage::getPassSeatOccupantSensorStValue() {
    return getValue(toInt(VehicleProperty::GECKO_SRS_PASS_SEAT_OCCUPANT_SENSOR_STS));
}

std::vector<uint8_t> IntegratedSwitchInfoMessage::generateDataPacket() {
    std::vector<uint8_t> dataPacket;
    dataPacket.push_back(integratedSwitchInfoId);
    uint16_t packlength = 0;
    dataPacket.push_back((packlength >> 8) & 0xFF);
    dataPacket.push_back(packlength & 0xFF);
    packlength = 0;
    //换挡杆请求位置
    gear_shift_lever_req_pos = getGearShiftLeverReqPosValue();
    dataPacket.push_back(gear_shift_lever_req_pos);
    packlength = packlength + 1;
    //右侧开关11状态
    right_11_switch = getRight11SwitchValue();
    dataPacket.push_back(right_11_switch);
    packlength = packlength + 1;
    //右侧开关12状态
    right_12_switch = getRight12SwitchValue();
    dataPacket.push_back(right_12_switch);
    packlength = packlength + 1;
    //右侧开关13状态
    right_13_switch = getRight13SwitchValue();
    dataPacket.push_back(right_13_switch);
    packlength = packlength + 1;
    //前雨刮开关状态
    front_wiper_switch_state = getFrontWiperSwitchStValue();
    dataPacket.push_back(front_wiper_switch_state);
    packlength = packlength + 1;
    //近光灯开关状态
    low_beam_switch_state = getLowBeamSwitchStValue();
    dataPacket.push_back(low_beam_switch_state);
    packlength = packlength + 1;
    //远光灯开关状态
    hi_beam_switch_state = getHiBeamSwitchStValue();
    dataPacket.push_back(hi_beam_switch_state);
    packlength = packlength + 1;
    //超车灯开关状态
    over_veh_beam_switch_state = getOverVehBeamSwitchStValue();
    dataPacket.push_back(over_veh_beam_switch_state);
    packlength = packlength + 1;
    //自动大灯状态
    lamp_auto_state = getLampAutoStValue();
    dataPacket.push_back(lamp_auto_state);
    packlength = packlength + 1;
    //左转向灯开关状态
    left_turn_lamp_switch_state = getLeftTurnLampSwitchStValue();
    dataPacket.push_back(left_turn_lamp_switch_state);
    packlength = packlength + 1;
    //右转向灯开关状态
    right_turn_lamp_switch_state = getRightTurnLampSwitchStValue();
    dataPacket.push_back(right_turn_lamp_switch_state);
    packlength = packlength + 1;
    //主驾安全带未系告警
    drv_seatbelt_buckle_status = getDrvSeatbeltBuckleStValue();
    dataPacket.push_back(drv_seatbelt_buckle_status);
    packlength = packlength + 1;
    //副驾安全带未系告警
    pass_seatbelt_buckle_status = getPassSeatbeltBuckleStValue();
    dataPacket.push_back(pass_seatbelt_buckle_status);
    packlength = packlength + 1;
    //副驾（中座）安全带未系告警
    mid_seatbelt_buckle_status = getMidSeatbeltBuckleStValue();
    dataPacket.push_back(mid_seatbelt_buckle_status);
    packlength = packlength + 1;
    //碰撞信号
    crash_output_status = getCrashOutputValue();
    dataPacket.push_back(crash_output_status);
    packlength = packlength + 1;
    //乘客安全带未系告警
    mid_seat_occupant_sensor_status = getMidSeatOccupantSensorStValue();
    dataPacket.push_back(mid_seat_occupant_sensor_status);
    packlength = packlength + 1;
    //乘客安全带未系告警
    pass_seat_occupant_sensor_status = getPassSeatOccupantSensorStValue();
    dataPacket.push_back(pass_seat_occupant_sensor_status);
    packlength = packlength + 1;

    dataPacket[1] = (packlength >> 8) & 0xFF;
    dataPacket[2] = packlength & 0xFF;
    return dataPacket;
}