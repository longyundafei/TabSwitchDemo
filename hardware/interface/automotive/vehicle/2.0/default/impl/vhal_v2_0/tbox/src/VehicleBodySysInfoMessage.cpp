#include "VehicleBodySysInfoMessage.h"
int VehicleBodySysInfoMessage::getLeftTurnLampStValue() {
    int faultFB = getValue(toInt(VehicleProperty::GECKO_BCM_LETURNLIFAULTFB));
    int ret = 0;
    if(faultFB != 0x00){
        ret = faultFB << 4;
    } else {
        int hazardLightSt = getValue(toInt(VehicleProperty::HAZARD_LIGHTS_STATE));
        int st = 0x00;
        if(hazardLightSt == 0x00){
            st = getValue(toInt(VehicleProperty::GECKO_LEFT_TURN_SIGNAL_STATE));
        } else {
            st = 0x01;
        }
        ret = (faultFB << 4) | st;
    }
    return ret;
}

int VehicleBodySysInfoMessage::getRightTurnLampStValue() {
    int faultFB = getValue(toInt(VehicleProperty::GECKO_BCM_RITURNLIFAULTFB));
    int ret = 0;
    if(faultFB != 0x00){
        ret = faultFB << 4;
    } else {
        int hazardLightSt = getValue(toInt(VehicleProperty::HAZARD_LIGHTS_STATE));
        int st = 0x00;
        if(hazardLightSt == 0x00){
            st = getValue(toInt(VehicleProperty::GECKO_RIGHT_TURN_SIGNAL_STATE));
        } else {
            st = 0x01;
        }
        ret = (faultFB << 4) | st;
    }
    return ret;
}
int VehicleBodySysInfoMessage::getHighBeamStValue() {
    int faultFB = getValue(toInt(VehicleProperty::GECKO_BCM_HIGH_BEAM_LI_FAULT_FB));
    int ret = 0;
    if(faultFB != 0x00){
        ret = faultFB << 4;
    } else {
        int st = getValue(toInt(VehicleProperty::HIGH_BEAM_LIGHTS_STATE));
        ret = (faultFB << 4) | st;
    }
    return ret;
}
int VehicleBodySysInfoMessage::getLowBeamStValue() {
    int faultFB = getValue(toInt(VehicleProperty::GECKO_BCM_LOW_BEAM_LI_FAULT_FB));
    int ret = 0;
    if(faultFB != 0x00){
        ret = faultFB << 4;
    } else {
        int st = getValue(toInt(VehicleProperty::HEADLIGHTS_STATE));
        ret = (faultFB << 4) | st;
    }
    return ret;
}
int VehicleBodySysInfoMessage::getDayRunLightStValue() {
    int faultFB = getValue(toInt(VehicleProperty::GECKO_BCM_DAYRUNLIFAULTFB));
    int ret = 0;
    if(faultFB != 0x00){
        ret = faultFB << 4;
    } else {
        int st = getValue(toInt(VehicleProperty::GECKO_BCM_DAY_RUN_LIGHT_STA));
        ret = (faultFB << 4) | st;
    }
    return ret;
}
int VehicleBodySysInfoMessage::getFrontFogLampStValue() {
    int faultFB = getValue(toInt(VehicleProperty::GECKO_BCM_FRONTFOGLIFAULTFB));
    int ret = 0;
    if(faultFB != 0x00){
        ret = faultFB << 4;
    } else {
        int st = getValue(toInt(VehicleProperty::GECKO_FRONT_FOG_LIGHT_STATUS));
        ret = (faultFB << 4) | st;
    }
    return ret;
}
int VehicleBodySysInfoMessage::getRearFogLampStValue() {
    int faultFB = getValue(toInt(VehicleProperty::GECKO_BCM_REARFOGLIFAULTFB));
    int ret = 0;
    if(faultFB != 0x00){
        ret = faultFB << 4;
    } else {
        int st = getValue(toInt(VehicleProperty::FOG_LIGHTS_STATE));
        ret = (faultFB << 4) | st;
    }
    return ret;
}
int VehicleBodySysInfoMessage::getPosLampStValue() {
    int faultFB = getValue(toInt(VehicleProperty::GECKO_BCM_POSLIFAULTFB));
    int ret = 0;
    if(faultFB != 0x00){
        ret = faultFB << 4;
    } else {
        int st = getValue(toInt(VehicleProperty::CABIN_LIGHTS_STATE));
        ret = (faultFB << 4) | st;
    }
    return ret;
}
int VehicleBodySysInfoMessage::getReverseLampStValue() {
    int faultFB = getValue(toInt(VehicleProperty::GECKO_BCM_REVERSELIFAULTFB));
    int ret = 0;
    if(faultFB != 0x00){
        ret = faultFB << 4;
    } else {
        int st = getValue(toInt(VehicleProperty::GECKO_BCM_REVERSE_LAMP_STA));
        ret = (faultFB << 4) | st;
    }
    return ret;
}
int VehicleBodySysInfoMessage::getBrakeLampStValue() {
    int faultFB = getValue(toInt(VehicleProperty::GECKO_BCM_BRAKELIFAULTFB));
    int ret = 0;
    if(faultFB != 0x00){
        ret = faultFB << 4;
    } else {
        int st = getValue(toInt(VehicleProperty::GECKO_BCM_BRAKE_LAMP_STA));
        ret = (faultFB << 4) | st;
    }
    return ret;
}
int VehicleBodySysInfoMessage::getBackLampStValue() {
    int faultFB = getValue(toInt(VehicleProperty::GECKO_BCM_BACKLIFAULTFB));
    int ret = 0;
    if(faultFB != 0x00){
        ret = faultFB << 4;
    } else {
        int st = getValue(toInt(VehicleProperty::GECKO_BCM_BACK_LAMP_STA));
        ret = (faultFB << 4) | st;
    }
    return ret;
}
int VehicleBodySysInfoMessage::getAutoLampStValue() {
    return getValue(toInt(VehicleProperty::GECKO_AUTO_LAMP_STATUS));
}
int VehicleBodySysInfoMessage::getFrontFogLightSwitchValue() {
    return getValue(toInt(VehicleProperty::GECKO_BCM_FRNT_FOG_LI_SW_SIG));
}
int VehicleBodySysInfoMessage::getRearFogLightSwitchValue() {
    return getValue(toInt(VehicleProperty::GECKO_BCM_REAR_FOG_LI_SW_SIG));
}
int VehicleBodySysInfoMessage::getTurnLightStValue() {
    return getValue(toInt(VehicleProperty::GECKO_BCM_TURN_LI_STS));
}
int VehicleBodySysInfoMessage::getLightIntensityValue() {
    return getValue(toInt(VehicleProperty::GECKO_BCM_LIGHT_INTENSITY));
}
int VehicleBodySysInfoMessage::getFollowMeHomeCtrlFBValue() {
    return getValue(toInt(VehicleProperty::GECKO_ACCOMPANY_BACK_HOME_DURATION));
}
int VehicleBodySysInfoMessage::getBassHornReqValue() {
    return getValue(toInt(VehicleProperty::GECKO_BCM_BASS_HORN_REQ));
}
int VehicleBodySysInfoMessage::getTrunkAjarStValue() {
    return getValue(toInt(VehicleProperty::GECKO_TR_CLS_ST));
}
int VehicleBodySysInfoMessage::getFLDoorAjarStValue() {
    return getValue(toInt(VehicleProperty::GECKO_FL_DOOR_STATE));
}
int VehicleBodySysInfoMessage::getFRDoorAjarStValue() {
    return getValue(toInt(VehicleProperty::GECKO_FR_DOOR_STATE));
}
int VehicleBodySysInfoMessage::getRLDoorAjarStValue() {
    return getValue(toInt(VehicleProperty::GECKO_RL_DOOR_STATE));
}
int VehicleBodySysInfoMessage::getRRDoorAjarStValue() {
    return getValue(toInt(VehicleProperty::GECKO_RR_DOOR_STATE));
}
int VehicleBodySysInfoMessage::getFrontLeftDoorHandleSwitchValue() {
    return getValue(toInt(VehicleProperty::GECKO_BCM_FRNT_LE_DOOR_HNDL_SWT));
}
int VehicleBodySysInfoMessage::getFrontRightDoorHandleSwitchValue() {
    return getValue(toInt(VehicleProperty::GECKO_BCM_FRNT_RI_DOOR_HNDL_SWT));
}
int VehicleBodySysInfoMessage::getFLPressureStValue() {
    return getValue(toInt(VehicleProperty::GECKO_TPMS_PRESSURE_STA_FL));
}
int VehicleBodySysInfoMessage::getFRPressureStValue() {
    return getValue(toInt(VehicleProperty::GECKO_TPMS_PRESSURE_STA_FR));
}
int VehicleBodySysInfoMessage::getRLPressureStValue() {
    return getValue(toInt(VehicleProperty::GECKO_TPMS_PRESSURE_STA_RL));
}
int VehicleBodySysInfoMessage::getRRPressureStValue() {
    return getValue(toInt(VehicleProperty::GECKO_TPMS_PRESSURE_STA_RR));
}
int VehicleBodySysInfoMessage::getFrontWiperCtrlValue() {
    return getValue(toInt(VehicleProperty::GECKO_BCM_WIPER_CTRL_FRONT));
}
int VehicleBodySysInfoMessage::getFrontWiperMotorStValue() {
    return getValue(toInt(VehicleProperty::GECKO_BCM_FRONT_WIPER_MOTOR_STS));
}
int VehicleBodySysInfoMessage::getFrontWashStValue() {
    return getValue(toInt(VehicleProperty::GECKO_BCM_FRONTWASHSTS));
}
int VehicleBodySysInfoMessage::getFLWindowStValue() {
    return getValue(toInt(VehicleProperty::GECKO_BCM_FL_WINDOW_STS));
}
int VehicleBodySysInfoMessage::getFRWindowStValue() {
    return getValue(toInt(VehicleProperty::GECKO_BCM_FR_WINDOW_STS));
}
int VehicleBodySysInfoMessage::getFLWindowMotorStValue() {
    return getValue(toInt(VehicleProperty::GECKO_BCM_FL_WINDOWMOTOR_STS));
}
int VehicleBodySysInfoMessage::getFRWindowMotorStValue() {
    return getValue(toInt(VehicleProperty::GECKO_BCM_FR_WINDOWMOTOR_STS));
}
int VehicleBodySysInfoMessage::getBodyWarnStValue() {
    return getValue(toInt(VehicleProperty::GECKO_BCM_BODYWARNSTS));
}
int VehicleBodySysInfoMessage::getPSAuthentResultValue() {
    return getValue(toInt(VehicleProperty::GECKO_BCM_PSAUTHENTRES));
}
int VehicleBodySysInfoMessage::getRKEReqValue() {
    return getValue(toInt(VehicleProperty::GECKO_BCM_RKEREQ));
}
int VehicleBodySysInfoMessage::getDriverPEAuthentAccessedValue() {
    return getValue(toInt(VehicleProperty::GECKO_BCM_DRV_PE_AUTHENT_ACSD));
}
int VehicleBodySysInfoMessage::getFRPEAuthentAccessedValue() {
    return getValue(toInt(VehicleProperty::GECKO_BCM_FRPE_AUTHENT_ACSD));
}
int VehicleBodySysInfoMessage::getPSReqValue() {
    return getValue(toInt(VehicleProperty::GECKO_BCM_PS_REQ));
}
int VehicleBodySysInfoMessage::getWiperIntervalTimeSetFBValue() {
    return getValue(toInt(VehicleProperty::GECKO_WIPER_INTERVAL_TIME));
}
int VehicleBodySysInfoMessage::getBCMStartReqValue() {
    return getValue(toInt(VehicleProperty::GECKO_BCM_START_REQ));
}
int VehicleBodySysInfoMessage::getOverspdCtrlLockSetFBValue() {
    return getValue(toInt(VehicleProperty::GECKO_BCM_OVERSPDCNTRLLOCKSETF));
}
int VehicleBodySysInfoMessage::getKeyInCarStValue() {
    return getValue(toInt(VehicleProperty::GECKO_BCM_KEYINCARSTA));
}
int VehicleBodySysInfoMessage::getAuthentKeyCountValue() {
    return getValue(toInt(VehicleProperty::GECKO_BCM_AUTHENT_KEY_NR));
}
int VehicleBodySysInfoMessage::getForbidenKeyCountValue() {
    return getValue(toInt(VehicleProperty::GECKO_BCM_FRBDN_KEY_NR));
}
int VehicleBodySysInfoMessage::getAvaluebleKeyInCarValue() {
    return getValue(toInt(VehicleProperty::GECKO_BCM_AVALUEBLEKEYINCAR));
}
int VehicleBodySysInfoMessage::getKeyStValue() {
    return getValue(toInt(VehicleProperty::GECKO_BCM_KEY_STA));
}
int VehicleBodySysInfoMessage::getKeyForbidenValue() {
    return getValue(toInt(VehicleProperty::GECKO_BCM_KEY_FRBDN));
}
int VehicleBodySysInfoMessage::getKeyLowBattLevelWarnValue() {
    return getValue(toInt(VehicleProperty::GECKO_KEY_LOW_BATT_LVL_WARN));
}
int VehicleBodySysInfoMessage::getWarnKeyOutReminderValue() {
    return getValue(toInt(VehicleProperty::GECKO_BCM_WARN_KEY_OUT_REMINDER));
}
int VehicleBodySysInfoMessage::getIndicationKeyCloserValue() {
    return getValue(toInt(VehicleProperty::GECKO_BCM_INDICATION_KEY_CLOSER));
}
int VehicleBodySysInfoMessage::getPEPSIlluminatedStValue() {
    return getValue(toInt(VehicleProperty::GECKO_BCM_ILLMND_STS));
}
int VehicleBodySysInfoMessage::getSteerWheelHeatStValue() {
    return getValue(toInt(VehicleProperty::GECKO_BCM_STEERWHLHEATSTS));
}
int VehicleBodySysInfoMessage::getRearMirrorHeatStValue() {
    return getValue(toInt(VehicleProperty::GECKO_BCM_REARMIRRORHEATSTS));
}
int VehicleBodySysInfoMessage::getLogoActiveFlagValue() {
    return getValue(toInt(VehicleProperty::GECKO_BCM_LOGOACTIVEFLAG));
}
int VehicleBodySysInfoMessage::getMainDriverSeatHeatStValue() {
    return getValue(toInt(VehicleProperty::GECKO_BCM_MAIDRVARSEATSTS));
}
int VehicleBodySysInfoMessage::getRFInitStValue() {
    return getValue(toInt(VehicleProperty::GECKO_BCM_RFINITSTA));
}
int VehicleBodySysInfoMessage::getHoodAjarStValValue() {
    return getValue(toInt(VehicleProperty::GECKO_BCM_AJAR_STA_V_HOOD));
}
int VehicleBodySysInfoMessage::getHoodAjarStValue() {
    return getValue(toInt(VehicleProperty::GECKO_BCM_AJARSTA_HOOD));
}
int VehicleBodySysInfoMessage::getDriverSeatOccupantSensorStValue() {
    return getValue(toInt(VehicleProperty::GECKO_BCM_DRV_SEAT_OCCUPANT_SENSOR_STS));
}
int VehicleBodySysInfoMessage::getHeavyRainIndValue() {
    return getValue(toInt(VehicleProperty::GECKO_BCM_HEAVY_RAIN_IND));
}
int VehicleBodySysInfoMessage::getSavePowerStValue() {
    return getValue(toInt(VehicleProperty::GECKO_BCM_SAVE_POWER_STA));
}
int VehicleBodySysInfoMessage::getDomeLightStValue() {
    return getValue(toInt(VehicleProperty::GECKO_BCM_DOME_LIGHT_STA));
}
int VehicleBodySysInfoMessage::getCarriageLightStValue() {
    return getValue(toInt(VehicleProperty::GECKO_BCM_CARRIAGELIGHTSTA));
}
int VehicleBodySysInfoMessage::getWiperStopPosStValue() {
    return getValue(toInt(VehicleProperty::GECKO_BCM_WIPER_STOP_POS_STA));
}
int VehicleBodySysInfoMessage::getFLPressureValue() {
    //return getValue(toInt(VehicleProperty::GECKO_TPMS_PRESSURE_VALUE_FL));
    return 0xFF;
}
int VehicleBodySysInfoMessage::getFRPressureValue() {
    //return getValue(toInt(VehicleProperty::GECKO_TPMS_PRESSURE_VALUE_FR));
    return 0xFF;
}
int VehicleBodySysInfoMessage::getRLPressureValue() {
    //return getValue(toInt(VehicleProperty::GECKO_TPMS_PRESSURE_VALUE_RL));
    return 0xFF;
}
int VehicleBodySysInfoMessage::getRRPressureValue() {
    //return getValue(toInt(VehicleProperty::GECKO_TPMS_PRESSURE_VALUE_RR));
    return 0xFF;
}

std::vector<uint8_t> VehicleBodySysInfoMessage::generateDataPacket() {
    std::vector<uint8_t> dataPacket;
    dataPacket.push_back(VehicleBodySysInfoId);
    uint16_t packlength = 0;
    dataPacket.push_back((packlength >> 8) & 0xFF);
    dataPacket.push_back(packlength & 0xFF);
    packlength = 0;
    //左转向灯状态
    left_turn_lamp_st = getLeftTurnLampStValue();
    dataPacket.push_back(left_turn_lamp_st);
    packlength = packlength + 1;
    //右转向灯状态
    right_turn_lamp_st = getRightTurnLampStValue();
    dataPacket.push_back(right_turn_lamp_st);
    packlength = packlength + 1;
    //远光灯状态
    high_beam_st = getHighBeamStValue();
    dataPacket.push_back(high_beam_st);
    packlength = packlength + 1;
    //近光灯状态
    low_beam_st = getLowBeamStValue();
    dataPacket.push_back(low_beam_st);
    packlength = packlength + 1;
    //日间行车灯状态
    day_run_light_st = getDayRunLightStValue();
    dataPacket.push_back(day_run_light_st);
    packlength = packlength + 1;
    //前雾灯状态
    front_fog_lamp_st = getFrontFogLampStValue();
    dataPacket.push_back(front_fog_lamp_st);
    packlength = packlength + 1;
    //后雾灯状态
    rear_fog_lamp_st = getRearFogLampStValue();
    dataPacket.push_back(rear_fog_lamp_st);
    packlength = packlength + 1;
    //位置灯状态
    pos_lamp_st = getPosLampStValue();
    dataPacket.push_back(pos_lamp_st);
    packlength = packlength + 1;
    //倒车灯状态
    reverse_lamp_st = getReverseLampStValue();
    dataPacket.push_back(reverse_lamp_st);
    packlength = packlength + 1;
    //制动灯状态
    brake_lamp_st = getBrakeLampStValue();
    dataPacket.push_back(brake_lamp_st);
    packlength = packlength + 1;
    //背光灯状态
    back_lamp_st = getBackLampStValue();
    dataPacket.push_back(back_lamp_st);
    packlength = packlength + 1;
    //自动大灯功能状态
    auto_lamp_st = getAutoLampStValue();
    dataPacket.push_back(auto_lamp_st);
    packlength = packlength + 1;
    //前雾灯开关状态
    frnt_fog_light_switch = getFrontFogLightSwitchValue();
    dataPacket.push_back(frnt_fog_light_switch);
    packlength = packlength + 1;
    //后雾灯开关状态
    rear_fog_light_switch = getRearFogLightSwitchValue();
    dataPacket.push_back(rear_fog_light_switch);
    packlength = packlength + 1;
    //转向灯状态
    turn_light_st = getTurnLightStValue();
    dataPacket.push_back(turn_light_st);
    packlength = packlength + 1;
    //白天黑夜状态
    light_intensity = getLightIntensityValue();
    dataPacket.push_back(light_intensity);
    packlength = packlength + 1;
    //伴我回家延时时间反馈
    flwr_me_hm_ctrl_fb = getFollowMeHomeCtrlFBValue();
    dataPacket.push_back(flwr_me_hm_ctrl_fb);
    packlength = packlength + 1;
    //低音喇叭请求
    bass_horn_req = getBassHornReqValue();
    dataPacket.push_back(bass_horn_req);
    packlength = packlength + 1;
    //行李箱开关状态
    trunk_ajar_st = getTrunkAjarStValue();
    dataPacket.push_back(trunk_ajar_st);
    packlength = packlength + 1;
    //左前门开关状态
    fl_door_ajar_st = getFLDoorAjarStValue();
    dataPacket.push_back(fl_door_ajar_st);
    packlength = packlength + 1;
    //右前门开关状态
    fr_door_ajar_st = getFRDoorAjarStValue();
    dataPacket.push_back(fr_door_ajar_st);
    packlength = packlength + 1;
    //左后门开关状态
    rl_door_ajar_st = getRLDoorAjarStValue();
    dataPacket.push_back(rl_door_ajar_st);
    packlength = packlength + 1;
    //右后门开关状态
    rr_door_ajar_st = getRRDoorAjarStValue();
    dataPacket.push_back(rr_door_ajar_st);
    packlength = packlength + 1;
    //主驾门把手微动开关
    frnt_le_door_hndl_swt = getFrontLeftDoorHandleSwitchValue();
    dataPacket.push_back(frnt_le_door_hndl_swt);
    packlength = packlength + 1;
    //副驾门把手微动开关
    frnt_ri_door_hndl_swt = getFrontRightDoorHandleSwitchValue();
    dataPacket.push_back(frnt_ri_door_hndl_swt);
    packlength = packlength + 1;
    //左前胎压状态
    fl_pressure_st = getFLPressureStValue();
    dataPacket.push_back(fl_pressure_st);
    packlength = packlength + 1;
    //右前胎压状态
    fr_pressure_st = getFRPressureStValue();
    dataPacket.push_back(fr_pressure_st);
    packlength = packlength + 1;
    //左后胎压状态
    rl_pressure_st = getRLPressureStValue();
    dataPacket.push_back(rl_pressure_st);
    packlength = packlength + 1;
    //右后胎压状态
    rr_pressure_st = getRRPressureStValue();
    dataPacket.push_back(rr_pressure_st);
    packlength = packlength + 1;
    //前雨刮控制状态
    front_wiper_ctrl = getFrontWiperCtrlValue();
    dataPacket.push_back(front_wiper_ctrl);
    packlength = packlength + 1;
    //前雨刮电机状态
    front_wiper_motor_st = getFrontWiperMotorStValue();
    dataPacket.push_back(front_wiper_motor_st);
    packlength = packlength + 1;
    //前洗涤状态
    front_wash_st = getFrontWashStValue();
    dataPacket.push_back(front_wash_st);
    packlength = packlength + 1;
    //左前车窗状态
    fl_window_st = getFLWindowStValue();
    dataPacket.push_back(fl_window_st);
    packlength = packlength + 1;
    //右前车窗状态
    fr_window_st = getFRWindowStValue();
    dataPacket.push_back(fr_window_st);
    packlength = packlength + 1;
    //左前车窗电机状态
    fl_window_motor_st = getFLWindowMotorStValue();
    dataPacket.push_back(fl_window_motor_st);
    packlength = packlength + 1;
    //右前车窗电机状态
    fr_window_motor_st = getFRWindowMotorStValue();
    dataPacket.push_back(fr_window_motor_st);
    packlength = packlength + 1;
    //车身防盗报警状态
    body_warn_st = getBodyWarnStValue();
    dataPacket.push_back(body_warn_st);
    packlength = packlength + 1;
    //PS认证结果
    ps_authent_result = getPSAuthentResultValue();
    dataPacket.push_back(ps_authent_result);
    packlength = packlength + 1;
    //RKE 请求
    rke_req = getRKEReqValue();
    dataPacket.push_back(rke_req);
    packlength = packlength + 1;
    //驾驶侧PE认证访问
    drv_pe_authent_acsd = getDriverPEAuthentAccessedValue();
    dataPacket.push_back(drv_pe_authent_acsd);
    packlength = packlength + 1;
    //右前门PE认证访问
    fr_pe_authent_acsd = getFRPEAuthentAccessedValue();
    dataPacket.push_back(fr_pe_authent_acsd);
    packlength = packlength + 1;
    //PS请求
    ps_req = getPSReqValue();
    dataPacket.push_back(ps_req);
    packlength = packlength + 1;
    //雨刮间歇时间设置反馈
    wipr_intl_time_set_fb = getWiperIntervalTimeSetFBValue();
    dataPacket.push_back(wipr_intl_time_set_fb);
    packlength = packlength + 1;
     //BCM_StartReq
     bcm_start_req = getBCMStartReqValue();
    dataPacket.push_back(bcm_start_req);
    packlength = packlength + 1;
    //车速超出四门闭锁设置反馈
    overspd_cntrl_lock_set_fb = getOverspdCtrlLockSetFBValue();
    dataPacket.push_back(overspd_cntrl_lock_set_fb);
    packlength = packlength + 1;
    //钥匙在车内状态
    key_in_car_st = getKeyInCarStValue();
    dataPacket.push_back(key_in_car_st);
    packlength = packlength + 1;
    //已匹配钥匙数量
    authent_key_count = getAuthentKeyCountValue();
    dataPacket.push_back(authent_key_count);
    packlength = packlength + 1;
    //禁用钥匙数量
    frbdn_key_count = getForbidenKeyCountValue();
    dataPacket.push_back(frbdn_key_count);
    packlength = packlength + 1;
    //车内存在有效钥匙
    avalueble_key_in_car = getAvaluebleKeyInCarValue();
    dataPacket.push_back(avalueble_key_in_car);
    packlength = packlength + 1;
    //钥匙状态
    key_st = getKeyStValue();
    dataPacket.push_back(key_st);
    packlength = packlength + 1;
    //钥匙被禁止
    key_frbdn = getKeyForbidenValue();
    dataPacket.push_back(key_frbdn);
    packlength = packlength + 1;
    //钥匙电池低电量报警
    key_low_batt_lvl_warn = getKeyLowBattLevelWarnValue();
    dataPacket.push_back(key_low_batt_lvl_warn);
    packlength = packlength + 1;
    //警告钥匙离开车内
    warn_key_out_reminder = getWarnKeyOutReminderValue();
    dataPacket.push_back(warn_key_out_reminder);
    packlength = packlength + 1;
    //提示请将钥匙靠近线圈
    indication_key_closer = getIndicationKeyCloserValue();
    dataPacket.push_back(indication_key_closer);
    packlength = packlength + 1;
    //PEPS迎宾状态
    illmnd_sts = getPEPSIlluminatedStValue();
    dataPacket.push_back(illmnd_sts);
    packlength = packlength + 1;
    //方向盘加热状态
    steer_whl_heat_st = getSteerWheelHeatStValue();
    dataPacket.push_back(steer_whl_heat_st);
    packlength = packlength + 1;
    //后视镜加热状态
    rear_mirror_heat_st = getRearMirrorHeatStValue();
    dataPacket.push_back(rear_mirror_heat_st);
    packlength = packlength + 1;
    //伴我回家/迎宾/照我上车LOGO点亮标志位
    logo_active_flag = getLogoActiveFlagValue();
    dataPacket.push_back(logo_active_flag);
    packlength = packlength + 1;
    //主驾座椅加热状态
    main_driver_seat_heat_st = getMainDriverSeatHeatStValue();
    dataPacket.push_back(main_driver_seat_heat_st);
    packlength = packlength + 1;
    //射频初始化状态
    rf_init_st = getRFInitStValue();
    dataPacket.push_back(rf_init_st);
    packlength = packlength + 1;
    //前舱盖微动开关状态有效性
    hood_ajar_st_v = getHoodAjarStValValue();
    dataPacket.push_back(hood_ajar_st_v);
    packlength = packlength + 1;
    //前舱盖微动开关状态
    hood_ajar_st = getHoodAjarStValue();
    dataPacket.push_back(hood_ajar_st);
    packlength = packlength + 1;
    //主驾占座
    drv_seat_occupant_sensor_st = getDriverSeatOccupantSensorStValue();
    dataPacket.push_back(drv_seat_occupant_sensor_st);
    packlength = packlength + 1;
    //暴雨指示
    heavy_rain_ind = getHeavyRainIndValue();
    dataPacket.push_back(heavy_rain_ind);
    packlength = packlength + 1;
    //节电输出状态
    save_power_st = getSavePowerStValue();
    dataPacket.push_back(save_power_st);
    packlength = packlength + 1;
    //前顶灯输出状态
    dome_light_st = getDomeLightStValue();
    dataPacket.push_back(dome_light_st);
    packlength = packlength + 1;
    //货厢灯输出状态
    carriage_light_st = getCarriageLightStValue();
    dataPacket.push_back(carriage_light_st);
    packlength = packlength + 1;
    //雨刮归位状态
    wiper_stop_pos_st = getWiperStopPosStValue();
    dataPacket.push_back(wiper_stop_pos_st);
    packlength = packlength + 1;
    //左前胎压值
    fl_pressure = getFLPressureValue();
    dataPacket.push_back(fl_pressure);
    packlength = packlength + 1;
    //右前胎压值
    fr_pressure = getFRPressureValue();
    dataPacket.push_back(fr_pressure);
    packlength = packlength + 1;
    //左后胎压值
    rl_pressure = getRLPressureValue();
    dataPacket.push_back(rl_pressure);
    packlength = packlength + 1;
    //右后胎压值
    rr_pressure = getRRPressureValue();
    dataPacket.push_back(rr_pressure);
    packlength = packlength + 1;

    dataPacket[1] = (packlength >> 8) & 0xFF;
    dataPacket[2] = packlength & 0xFF;
    return dataPacket;
}