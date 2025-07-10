#include "VehicleControllerInfoMessage.h"
int VehicleControllerInfoMessage::getActualWheelTorqueValue() {
    int valid = getValue(toInt(VehicleProperty::GECKO_VCU_ACT_WHL_TQ_VLD));
    int ret = 0xFFFF;
    if(valid == 0x01){
        ret = getValue(toInt(VehicleProperty::GECKO_VCU_ACT_WHL_TQ));
    }
    return ret;
}
int VehicleControllerInfoMessage::getDriverReqMotorTorqueValue() {
    int valid = getValue(toInt(VehicleProperty::GECKO_VCU_DRV_REQ_WHL_TQ_VLD));
    int ret = 0xFFFF;
    if(valid == 0x01){
        ret = getValue(toInt(VehicleProperty::GECKO_VCU_DRV_REQ_WHL_TQ));
    }
    return ret;
}
int VehicleControllerInfoMessage::getMaxAllowedDriveTorqueValue() {
    return getValue(toInt(VehicleProperty::GECKO_VCU_DRV_WHL_TQ_ALLWD_MAX));
}
int VehicleControllerInfoMessage::getPwrRecupActlWhlTqStValue() {
    return getValue(toInt(VehicleProperty::GECKO_VCU_PWR_RECUP_ACTL_WHL_TQ_STS));
}
int VehicleControllerInfoMessage::getPwrRecupMaxAvlbWhlTqStValue() {
    return getValue(toInt(VehicleProperty::GECKO_VCU_PWR_RECUP_MAX_AVLB_WHL_TQ_STS));
}
int VehicleControllerInfoMessage::getBrkPwrRecupMaxAvlbWhlTqValue() {
    return getValue(toInt(VehicleProperty::GECKO_VCU_BRK_PWR_RECUP_MAX_AVLB_WHL_TQ));
}
int VehicleControllerInfoMessage::getPwrRecupActWhlTqValue() {
    return getValue(toInt(VehicleProperty::GECKO_VCU_PWR_RECUP_ACT_WHL_TQ));
}
int VehicleControllerInfoMessage::getVCUReqDriveOffValue() {
    return getValue(toInt(VehicleProperty::GECKO_VCU_ACC_REQ_DRIVE_OFF));
}
int VehicleControllerInfoMessage::getBrkPwrRecupActlWhlTqStValue() {
    return getValue(toInt(VehicleProperty::GECKO_VCU_BRK_PWR_RECUP_ACTL_WHL_TQ_STS));
}
int VehicleControllerInfoMessage::getBrkPwrRecupActWhlTqValue() {
    return getValue(toInt(VehicleProperty::GECKO_VCU_BRK_PWR_RECUP_ACT_WHL_TQ));
}
int VehicleControllerInfoMessage::getActlMotorRotateSpdValue() {
    int valid = getValue(toInt(VehicleProperty::GECKO_VCU_ACTL_MOTOR_ROTATE_SPD_V));
    int ret = 0x6FFF;
    if(valid == 0x01){
        ret = getValue(toInt(VehicleProperty::GECKO_VCU_ACTL_MOTOR_ROTATE_SPD));
        if(ret == 0xFFFF){
            ret = 0x6FFF;
        }
    }
    return ret;
}
int VehicleControllerInfoMessage::getWLTPDrivingRangeDistValue() {
    int valid = getValue(toInt(VehicleProperty::GECKO_EV_VCU_ACTUAL_REMA_RANGE_VD));
    int ret = 0xFFFF;
    if(valid == 0x01){
        ret = getValue(toInt(VehicleProperty::RANGE_REMAINING));
    }
    return ret;
}
int VehicleControllerInfoMessage::getDrivingRangeDistEstValue() {
    int valid = getValue(toInt(VehicleProperty::GECKO_EV_VCU_CLTC_REMA_RANGE_VD));
    int ret = 0xFFFF;
    if(valid == 0x01){
        ret = getValue(toInt(VehicleProperty::GECKO_CLTC_RANGE_REMAINING));
    }
    return ret;
}
int VehicleControllerInfoMessage::getVCUInfoDispValue() {
    return getValue(toInt(VehicleProperty::GECKO_VCU_INFODISP));
}
int VehicleControllerInfoMessage::getIBSChargeNumAllowValue() {
    return getValue(toInt(VehicleProperty::GECKO_VCU_SIBS_CHGNUMALLOW));
}
int VehicleControllerInfoMessage::getIBSBMSBatSOCAllowValue() {
    return getValue(toInt(VehicleProperty::GECKO_VCU_SIBS_BMSBATSOCALLOW));
}
int VehicleControllerInfoMessage::getIBSWUPVoltAllowValue() {
    return getValue(toInt(VehicleProperty::GECKO_VCU_SIBS_WUPVOLTALLOW));
}
int VehicleControllerInfoMessage::getIBSRTCWakeChgNumValue() {
    return getValue(toInt(VehicleProperty::GECKO_VCU_SIBS_RTCWUCHGNUM));
}
int VehicleControllerInfoMessage::getIBSRTCChrgStValue() {
    return getValue(toInt(VehicleProperty::GECKO_VCU_SIBS_RTCCHRGST));
}
int VehicleControllerInfoMessage::getIBSRTCWakeupChrgFailNumValue() {
    return getValue(toInt(VehicleProperty::GECKO_VCU_SIBS_RTCWUCHGFAILNUM));
}
int VehicleControllerInfoMessage::getIBSForbidenRTCWakeupFlgValue() {
    return getValue(toInt(VehicleProperty::GECKO_VCU_SIBS_FBRTCWUPFLG));
}
int VehicleControllerInfoMessage::getVehicleActDrivingPwrValue() {
    return getValue(toInt(VehicleProperty::GECKO_DRIVING_POWER_ST));
}
int VehicleControllerInfoMessage::getVehicleActRecupPwrValue() {
    return getValue(toInt(VehicleProperty::GECKO_CCM_ENERGY_RECOV_LEVEL_CMD));
}
int VehicleControllerInfoMessage::getOperatLicenceFBValue() {
    return getValue(toInt(VehicleProperty::GECKO_VCU_OPERATLICENCEFB));
}
int VehicleControllerInfoMessage::getVersionFBValue() {
    return getValue(toInt(VehicleProperty::GECKO_VCU_VERSIONFB));
}
int VehicleControllerInfoMessage::getSpdLimitLevelFBValue() {
    return getValue(toInt(VehicleProperty::GECKO_CCM_SPEED_LIMIT_LEVEL));
}
int VehicleControllerInfoMessage::getRemoteLockFBValue() {
    return getValue(toInt(VehicleProperty::GECKO_CCM_REMOTE_LOCK));
}
int VehicleControllerInfoMessage::getRemoteCarSearchFBValue() {
    return getValue(toInt(VehicleProperty::GECKO_CCM_REMOTE_CAR_SEARCH));
}
int VehicleControllerInfoMessage::getVersionSetFBValue() {
    return getValue(toInt(VehicleProperty::GECKO_VCU_VERSIONSETFB));
}
int VehicleControllerInfoMessage::getSpdLimitLevelSetFBValue() {
    return getValue(toInt(VehicleProperty::GECKO_VCU_SPEEDLIMITLEVELSETFB));
}
int VehicleControllerInfoMessage::getRemotePwrLockFBValue() {
    return getValue(toInt(VehicleProperty::GECKO_CCU_REMOTEPOWERLOCKFB));
}
int VehicleControllerInfoMessage::getRemotePwrCtrlFBValue() {
    return getValue(toInt(VehicleProperty::GECKO_CCU_REMTPOWERCTRLFB));
}
int VehicleControllerInfoMessage::getRemoteSteerwhlHeatFBValue() {
    return getValue(toInt(VehicleProperty::GECKO_CCU_REMTSTEERWHLHEATFB));
}
int VehicleControllerInfoMessage::getRemoteMainDriverSeatHeatFBValue() {
    return getValue(toInt(VehicleProperty::GECKO_CCU_REMTMAIDRSEATHEATFB));
}
int VehicleControllerInfoMessage::getPwrTrainActOperatingModeValue() {
    return getValue(toInt(VehicleProperty::GECKO_VCU_PT_ST));
}
int VehicleControllerInfoMessage::getVehicleActGearPosValue() {
    int valid = getValue(toInt(VehicleProperty::GECKO_VCU_VHCLACTLGEARPOSV));
    int ret = 0xFF;
    if(valid == 0x01){
        ret = getValue(toInt(VehicleProperty::CURRENT_GEAR));
    }
    return ret;
}
int VehicleControllerInfoMessage::getSysPwrModeValue() {
    int valid = getValue(toInt(VehicleProperty::GECKO_BCM_SYSPWRMODEV));
    int ret = 0xFF;
    if(valid == 0x01){
        ret = getValue(toInt(VehicleProperty::GECKO_BCM_SYSPWRMODE));
    }
    return ret;
}
int VehicleControllerInfoMessage::getBatLowVolStValue() {
    return getValue(toInt(VehicleProperty::GECKO_12V_BAT_LOW_VOL_STA));
}
int VehicleControllerInfoMessage::getLimpHomeStValue() {
    return getValue(toInt(VehicleProperty::GECKO_VCU_DRIVE_PWR_LIM));
}
int VehicleControllerInfoMessage::getHazardLampStValue() {
    return getValue(toInt(VehicleProperty::GECKO_BCM_HAZARD_LAMPSTS));
}
int VehicleControllerInfoMessage::getCentralLockStValue() {
    return getValue(toInt(VehicleProperty::GECKO_BCM_CENTRALLOCKSTS));
}
int VehicleControllerInfoMessage::getSysPwrStValue() {
    return getValue(toInt(VehicleProperty::GECKO_BCM_SYSPOWERSTS));
}
int VehicleControllerInfoMessage::getFLDoorLockStValue() {
    return getValue(toInt(VehicleProperty::GECKO_BCM_DOORLOCKSTA_FL));
}
int VehicleControllerInfoMessage::getVehicleActDrvMod1Value() {
    return getValue(toInt(VehicleProperty::GECKO_VEHICLE_DRIVE_MODE));
}
int VehicleControllerInfoMessage::getDiagTesterOnlineValue() {
    return getValue(toInt(VehicleProperty::GECKO_CCU_DIAGTESTERONLINE));
}
int VehicleControllerInfoMessage::getHDCStValue() {
    return getValue(toInt(VehicleProperty::GECKO_VCU_HDC_STATUS));
}
int VehicleControllerInfoMessage::getAcclPedalPosValue() {
    return getRawPropValue(toInt(VehicleProperty::GECKO_VCU_ACCLPEDAL_POS));
}
int VehicleControllerInfoMessage::getBrkPedalPosValue() {
    return getValue(toInt(VehicleProperty::GECKO_VCU_BRKPEDAL_POS));
}
int VehicleControllerInfoMessage::getEPBReqValue() {
    return getValue(toInt(VehicleProperty::GECKO_VCU_REQ_EPB));
}
int VehicleControllerInfoMessage::getCruiseCtrlStValue() {
    return getValue(toInt(VehicleProperty::GECKO_VCU_CRUISE_CTRL_STA));
}
int VehicleControllerInfoMessage::getCruiseCtrlTarSpdValue() {
    int ret = getValue(toInt(VehicleProperty::GECKO_VCU_CRUISE_CTR_TGT_SPD));
    if(ret == 0xFFFF){
       ret = 0x1FFF;
    }
    return ret;
}
int VehicleControllerInfoMessage::getWeightEstValue() {
    int valid = getValue(toInt(VehicleProperty::GECKO_VCU_WGHCONFIDENCE));
    int ret = 0x1FFF;
    if(valid == 0x01){
        ret = getValue(toInt(VehicleProperty::GECKO_VCU_WGHEST));
        if(ret ==  0xFFFF){
            ret = 0x1FFF;
        }
    }
    return ret;
}
int VehicleControllerInfoMessage::getWeightDistributValue() {
    return getValue(toInt(VehicleProperty::GECKO_VCU_WGHDISTB));
}
int VehicleControllerInfoMessage::getACCTqReqValue() {
    return getValue(toInt(VehicleProperty::GECKO_VCU_ACC_TRQ_CTRL_AVL));
}
int VehicleControllerInfoMessage::getAcclPedalPosSensorStValue() {
    return getValue(toInt(VehicleProperty::GECKO_VCU_ACCL_PEDAL_POS_SNSR_STA));
}
int VehicleControllerInfoMessage::getDrvReqTqOvverrideFCMValue() {
    return getValue(toInt(VehicleProperty::GECKO_VCU_DRV_REQ_TORQ_OVERRIDE_FCM));
}
int VehicleControllerInfoMessage::getACCTarAXValue() {
    return getValue(toInt(VehicleProperty::GECKO_VCU_ACC_TGT_AX));
}
int VehicleControllerInfoMessage::getACCTarAXReqValue() {
    return getValue(toInt(VehicleProperty::GECKO_VCU_ACC_TGT_AX_REQ));
}
int VehicleControllerInfoMessage::getBrkTqReqValue() {
    return getValue(toInt(VehicleProperty::GECKO_VCU_BRA_TOR_REQ));
}
int VehicleControllerInfoMessage::getACCStandstillReqValue() {
    return getValue(toInt(VehicleProperty::GECKO_VCU_ACC_STANDSTILL_REQ));
}

std::vector<uint8_t> VehicleControllerInfoMessage::generateDataPacket() {
    std::vector<uint8_t> dataPacket;
    dataPacket.push_back(VehicleControllerInfoId);
    uint16_t packlength = 0;
    dataPacket.push_back((packlength >> 8) & 0xFF);
    dataPacket.push_back(packlength & 0xFF);
    packlength = 0;
    //实际轮端扭矩
    act_whl_tq = getActualWheelTorqueValue();
    dataPacket.push_back((act_whl_tq >> 8) & 0xFF);
    dataPacket.push_back(act_whl_tq & 0xFF);
    packlength = packlength + 2;
    //驾驶员请求轮端扭矩
    drv_req_whl_tq = getDriverReqMotorTorqueValue();
    dataPacket.push_back((drv_req_whl_tq >> 8) & 0xFF);
    dataPacket.push_back(drv_req_whl_tq & 0xFF);
    packlength = packlength + 2;
    //最大允许驱动轮端扭矩
    drv_req_whl_tq = getMaxAllowedDriveTorqueValue();
    dataPacket.push_back((drv_req_whl_tq >> 8) & 0xFF);
    dataPacket.push_back(drv_req_whl_tq & 0xFF);
    packlength = packlength + 2;
    //能量回收状态
    pwr_recup_actl_whl_tq_st = getPwrRecupActlWhlTqStValue();
    dataPacket.push_back(pwr_recup_actl_whl_tq_st);
    packlength = packlength + 1;
    //最大允许轮端扭矩状态
    pwr_recup_max_avlb_whl_tq_st = getPwrRecupMaxAvlbWhlTqStValue();
    dataPacket.push_back(pwr_recup_max_avlb_whl_tq_st);
    packlength = packlength + 1;
    //制动能量回收的最大可用扭矩
    brk_pwr_recup_max_avlb_whl_tq = getBrkPwrRecupMaxAvlbWhlTqValue();
    dataPacket.push_back((brk_pwr_recup_max_avlb_whl_tq >> 8) & 0xFF);
    dataPacket.push_back(brk_pwr_recup_max_avlb_whl_tq & 0xFF);
    packlength = packlength + 2;
    //功率回收实际车轮扭矩
    pwr_recup_act_whl_tq = getPwrRecupActWhlTqValue();
    dataPacket.push_back((pwr_recup_act_whl_tq >> 8) & 0xFF);
    dataPacket.push_back(pwr_recup_act_whl_tq & 0xFF);
    packlength = packlength + 2;
    //VCU请求起步
    acc_req_drive_off = getVCUReqDriveOffValue();
    dataPacket.push_back(acc_req_drive_off);
    packlength = packlength + 1;
    //制动能量回收轮端扭矩状态
    brk_pwr_recup_actl_whl_tq_st = getBrkPwrRecupActlWhlTqStValue();
    dataPacket.push_back(brk_pwr_recup_actl_whl_tq_st);
    packlength = packlength + 1;
    //制动能量回收实际轮端扭矩
    brk_pwr_recup_act_whl_tq = getBrkPwrRecupActWhlTqValue();
    dataPacket.push_back((brk_pwr_recup_act_whl_tq >> 8) & 0xFF);
    dataPacket.push_back(brk_pwr_recup_act_whl_tq & 0xFF);
    packlength = packlength + 2;
    //实际电机端转速
    actl_motor_rotate_spd = getActlMotorRotateSpdValue();
    dataPacket.push_back((actl_motor_rotate_spd >> 8) & 0xFF);
    dataPacket.push_back(actl_motor_rotate_spd & 0xFF);
    packlength = packlength + 2;
    //WLTP续航里程距离
    wltp_drvg_rng_dst = getWLTPDrivingRangeDistValue();
    dataPacket.push_back((wltp_drvg_rng_dst >> 8) & 0xFF);
    dataPacket.push_back(wltp_drvg_rng_dst & 0xFF);
    packlength = packlength + 2;
    //预估续驶里程距离
    drv_range_dist_est = getDrivingRangeDistEstValue();
    dataPacket.push_back((drv_range_dist_est >> 8) & 0xFF);
    dataPacket.push_back(drv_range_dist_est & 0xFF);
    packlength = packlength + 2;
    //VCU信息提示
    vcu_info_disp = getVCUInfoDispValue();
    dataPacket.push_back(vcu_info_disp);
    packlength = packlength + 1;
    //IBS补电允许 次数
    ibs_chg_num_allow = getIBSChargeNumAllowValue();
    dataPacket.push_back(ibs_chg_num_allow);
    packlength = packlength + 1;
    //IBS补电允许 电池SOC
    ibs_bms_bat_soc_allow = getIBSBMSBatSOCAllowValue();
    dataPacket.push_back(ibs_bms_bat_soc_allow);
    packlength = packlength + 1;
    //IBS补电允许 蓄电池电压
    ibs_wup_volt_allow = getIBSWUPVoltAllowValue();
    dataPacket.push_back(ibs_wup_volt_allow);
    packlength = packlength + 1;
    //IBS补电次数 RTC唤醒次数
    ibs_rtcwk_chg_num = getIBSRTCWakeChgNumValue();
    dataPacket.push_back(ibs_rtcwk_chg_num);
    packlength = packlength + 1;
    //IBS补电状态
    ibs_rtc_chrg_st = getIBSRTCChrgStValue();
    dataPacket.push_back(ibs_rtc_chrg_st);
    packlength = packlength + 1;
    //IBS补电失败累计次数
    ibs_rtcwu_chg_fail_num = getIBSRTCWakeupChrgFailNumValue();
    dataPacket.push_back(ibs_rtcwu_chg_fail_num);
    packlength = packlength + 1;
    //禁止RTC唤醒补电
    ibs_fb_rtcw_flg = getIBSForbidenRTCWakeupFlgValue();
    dataPacket.push_back(ibs_fb_rtcw_flg);
    packlength = packlength + 1;
    //车辆实际驾驶强度
    veh_act_driving_pwr = getVehicleActDrivingPwrValue();
    dataPacket.push_back(veh_act_driving_pwr);
    packlength = packlength + 1;
    //车辆实际回收强度
    veh_act_recuperate_pwr = getVehicleActRecupPwrValue();
    dataPacket.push_back(veh_act_recuperate_pwr);
    packlength = packlength + 1;
    //远程运营许可
    operat_licence_fb = getOperatLicenceFBValue();
    dataPacket.push_back(operat_licence_fb);
    packlength = packlength + 1;
    //远程锁车版本
    version_fb = getVersionFBValue();
    dataPacket.push_back(version_fb);
    packlength = packlength + 1;
    //远程限速等级
    speed_limit_level_fb = getSpdLimitLevelFBValue();
    dataPacket.push_back(speed_limit_level_fb);
    packlength = packlength + 1;
    //远程控制锁反馈
    remote_lock_fb = getRemoteLockFBValue();
    dataPacket.push_back(remote_lock_fb);
    packlength = packlength + 1;
    //远程寻车反馈
    remote_car_search_fb = getRemoteCarSearchFBValue();
    dataPacket.push_back(remote_car_search_fb);
    packlength = packlength + 1;
    //远程锁车版本切换反馈
    vesion_set_fb = getVersionSetFBValue();
    dataPacket.push_back(vesion_set_fb);
    packlength = packlength + 1;
    //远程限速执行反馈
    speed_limit_level_set_fb = getSpdLimitLevelSetFBValue();
    dataPacket.push_back(speed_limit_level_set_fb);
    packlength = packlength + 1;
    //远程禁止上电执行反馈
    remote_power_lock_fb = getRemotePwrLockFBValue();
    dataPacket.push_back(remote_power_lock_fb);
    packlength = packlength + 1;
    //远程上下电反馈
    remt_power_ctrl_fb = getRemotePwrCtrlFBValue();
    dataPacket.push_back(remt_power_ctrl_fb);
    packlength = packlength + 1;
    //远程开关方向盘加热反馈
    remt_steer_whl_heat_fb = getRemoteSteerwhlHeatFBValue();
    dataPacket.push_back(remt_steer_whl_heat_fb);
    packlength = packlength + 1;
    //远程开关座椅加热加热反馈
    remt_main_driver_seat_heat_fb = getRemoteMainDriverSeatHeatFBValue();
    dataPacket.push_back(remt_main_driver_seat_heat_fb);
    packlength = packlength + 1;
    //动力总成运行模式
    pt_actl_oprt_mode = getPwrTrainActOperatingModeValue();
    dataPacket.push_back(pt_actl_oprt_mode);
    packlength = packlength + 1;
    //车辆实际档位
    vhcl_actl_gear_pos = getVehicleActGearPosValue();
    dataPacket.push_back(vhcl_actl_gear_pos);
    packlength = packlength + 1;
    //系统电源模式
    sys_pwr_mode = getSysPwrModeValue();
    dataPacket.push_back(sys_pwr_mode);
    packlength = packlength + 1;
    //12V电池低压状态
    bat_low_vol_st = getBatLowVolStValue();
    dataPacket.push_back(bat_low_vol_st);
    packlength = packlength + 1;
    //跛行模式
    limp_home_st = getLimpHomeStValue();
    dataPacket.push_back(limp_home_st);
    packlength = packlength + 1;
    //警示灯状态
    hazard_lamp_st = getHazardLampStValue();
    dataPacket.push_back(hazard_lamp_st);
    packlength = packlength + 1;
    //中控锁状态
    central_lock_st = getCentralLockStValue();
    dataPacket.push_back(central_lock_st);
    packlength = packlength + 1;
    //整车电源状态
    sys_power_st = getSysPwrStValue();
    dataPacket.push_back(sys_power_st);
    packlength = packlength + 1;
    //左前门锁状态
    door_lock_st_fl = getFLDoorLockStValue();
    dataPacket.push_back(door_lock_st_fl);
    packlength = packlength + 1;
    //车辆实际驾驶模式1
    veh_act_drv_mod1 = getVehicleActDrvMod1Value();
    dataPacket.push_back(veh_act_drv_mod1);
    packlength = packlength + 1;
    //OBD口诊断仪在线状态信号
    diag_tester_online = getDiagTesterOnlineValue();
    dataPacket.push_back(diag_tester_online);
    packlength = packlength + 1;
    //HDC状态
    hdc_st = getHDCStValue();
    dataPacket.push_back(hdc_st);
    packlength = packlength + 1;
    //加速踏板位置
    accl_pedal_pos = getAcclPedalPosValue();
    dataPacket.push_back(accl_pedal_pos);
    packlength = packlength + 1;
    //制动踏板位置
    brk_pedal_pos = getBrkPedalPosValue();
    dataPacket.push_back(brk_pedal_pos);
    packlength = packlength + 1;
    //EPB拉起/释放请求
    req_epb = getEPBReqValue();
    dataPacket.push_back(req_epb);
    packlength = packlength + 1;
    //定速巡航状态
    cruise_ctrl_st = getCruiseCtrlStValue();
    dataPacket.push_back(cruise_ctrl_st);
    packlength = packlength + 1;
    //定速巡航目标速度
    cruise_ctr_tgt_spd = getCruiseCtrlTarSpdValue();
    dataPacket.push_back((cruise_ctr_tgt_spd >> 8) & 0xFF);
    dataPacket.push_back(cruise_ctr_tgt_spd & 0xFF);
    packlength = packlength + 2;
    //载重估计值
    wgh_est = getWeightEstValue();
    dataPacket.push_back((wgh_est >> 8) & 0xFF);
    dataPacket.push_back(wgh_est & 0xFF);
    packlength = packlength + 2;
    //载重分布
    wgh_distb = getWeightDistributValue();
    dataPacket.push_back(wgh_distb);
    packlength = packlength + 1;
    //VCU允许ACC开启标志位
    acc_trq_ctrl_avl = getACCTqReqValue();
    dataPacket.push_back(acc_trq_ctrl_avl);
    packlength = packlength + 1;
    //加速踏板位置传感器状态
    accl_pedal_pos_snsr_st = getAcclPedalPosSensorStValue();
    dataPacket.push_back(accl_pedal_pos_snsr_st);
    packlength = packlength + 1;
    //驾驶员请求扭矩大于FCM请求扭矩
    drv_req_torq_override_fcm = getDrvReqTqOvverrideFCMValue();
    dataPacket.push_back(drv_req_torq_override_fcm);
    packlength = packlength + 1;
    //ACC目标减速度
    acc_tgt_ax = getACCTarAXValue();
    dataPacket.push_back(acc_tgt_ax);
    packlength = packlength + 1;
    //ACC目标减速度请求
    acc_tgt_ax_req = getACCTarAXReqValue();
    dataPacket.push_back(acc_tgt_ax_req);
    packlength = packlength + 1;
    //制动扭矩请求
    bra_tor_req = getBrkTqReqValue();
    dataPacket.push_back((bra_tor_req >> 8) & 0xFF);
    dataPacket.push_back(bra_tor_req & 0xFF);
    packlength = packlength + 2;
    //ACC请求VLC减速到停止
    acc_standstill_req = getACCStandstillReqValue();
    dataPacket.push_back(acc_standstill_req);
    packlength = packlength + 1;


    dataPacket[1] = (packlength >> 8) & 0xFF;
    dataPacket[2] = packlength & 0xFF;
    return dataPacket;
}