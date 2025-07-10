#include "FrontCamInfoMessage.h"

int FrontCamInfoMessage::getLongtdFuncActiveFlgValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_LGT_FCTACTV_FLG));
}
int FrontCamInfoMessage::getELKStValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_ELKSTS));
}
int FrontCamInfoMessage::getEPSSetSteeringWheelAngleValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_EPS_SET_STEERWHLAGL));
}
int FrontCamInfoMessage::getEPSModeReqValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_DAEPS_MODE_REQ));
}
int FrontCamInfoMessage::getDriverWarningVibrateReqValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_DRVR_WARNVIB_REQ));
}
int FrontCamInfoMessage::getEPSControleReqValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_EPS_CTRL_REQ));
}
int FrontCamInfoMessage::getEPSTorqueReqValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_STEER_TQREQ_ACTIVE));
}
int FrontCamInfoMessage::getLCCStValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_ACTDRV_ASSTSTS));
}
int FrontCamInfoMessage::getLCCSuppressOrAbortConditionValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_TJAICA_SUPPS_ABORTN));
}
int FrontCamInfoMessage::getTurnLampReqValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_TURN_LAMP_REQ));
}
int FrontCamInfoMessage::getOvertakeAssistantStValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_OVERTAKE_ASSSYSSTS));
}
int FrontCamInfoMessage::getLKATorqueFactReqVaule() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_LKATORQ_FACT_REQ));
}
int FrontCamInfoMessage::getLKALDWSuppsAbortnValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_LKALDW_SUPPS_ABORTN));
}
int FrontCamInfoMessage::getHMASuppsAbortnValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_HMA_SUPPS_ABORTN));
}
int FrontCamInfoMessage::getELKSuppsAbortnValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_ELK_SUPPS_ABORTN));
}
int FrontCamInfoMessage::getACCAEBTJAICAFaultSt() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_FLTSTS));
}
int FrontCamInfoMessage::getACCTorqueReqActiveStValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_ACCWHL_TQREQACTV));
}
int FrontCamInfoMessage::getACCTorqueReqValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_ACCWHL_TQREQVAL));
}
int FrontCamInfoMessage::getFCMACCWheelDecelerationValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_ACCWHL_DECE));
}
int FrontCamInfoMessage::getACCTargetAccelerationValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_ACCTAR_ACCRN));
}
int FrontCamInfoMessage::getCurveDecelerationStValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_CRV_DECEL_STA));
}
int FrontCamInfoMessage::getACCTargetAccelerationActiveStValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_ACCTAR_ACCRN_REQACTV));
}
int FrontCamInfoMessage::getACCSuppressOrAbortConditionValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_ACCSUPP_ABORT));
}
int FrontCamInfoMessage::getACCShutdownModeValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_ACC_SHUTDOWN_MODE));
}
int FrontCamInfoMessage::getACCTargetDecelerationValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_ACC_TGTAX));
}
int FrontCamInfoMessage::getACCReqDecelerateToStopValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_ACC_REQSTANDSTILL));
}
int FrontCamInfoMessage::getACCDriveOffReqValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_ACC_REQDRIVEOFF));
}
int FrontCamInfoMessage::getACCTargetDecelerationReqValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_ACC_TGTAX_REQ));
}
int FrontCamInfoMessage::getACCModeValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_ACC_MODE));
}
int FrontCamInfoMessage::getEPBReqValue() {
    int validat = getValue(toInt(VehicleProperty::GECKO_FCM_EPB_REQV));
    if(validat == 0x01){
        return getValue(toInt(VehicleProperty::GECKO_FCM_EPB_REQ));
    } else {
        return 0xFF;
    }
}
int FrontCamInfoMessage::getAheadObjIDValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_AHDOBJID));
}
int FrontCamInfoMessage::getAEBTargetDecelerationValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_AEB_TGTAX));
}
int FrontCamInfoMessage::getAEBStandstillStValue() {
    int stV = getValue(toInt(VehicleProperty::GECKO_FCM_AEB_STANDSTILLSTSV));
    if(stV == 0x01){
        return getValue(toInt(VehicleProperty::GECKO_FCM_AEB_STANDSTILLSTS));
    } else {
        return 0xFF;
    }
}
int FrontCamInfoMessage::getAEBReqValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_AEB_REQ));
}
int FrontCamInfoMessage::getABAReqValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_ABA_REQ));
}
int FrontCamInfoMessage::getEBAReqValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_EBA_REQ));
}
int FrontCamInfoMessage::getAWBLevelValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_AWB_LEVEL));
}
int FrontCamInfoMessage::getABALevelValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_ABA_LEVEL));
}
int FrontCamInfoMessage::getAEBStValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_AEB_STATUS));
}
int FrontCamInfoMessage::getESPPrefillReqValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_ABP_REQ));
}
int FrontCamInfoMessage::getAWBReqValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_AWB_REQ));
}
int FrontCamInfoMessage::getHMAStValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_HMA_STS));
}
int FrontCamInfoMessage::getHMAHighbeamReqValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_HMA_HIGHBEAMREQ));
}
int FrontCamInfoMessage::getAEBSetFBValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_AEBSETFB));
}
int FrontCamInfoMessage::getAEBRecReqValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_AEBRECREQ));
}
int FrontCamInfoMessage::getELKSetFBValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_ELKSETFB));
}
int FrontCamInfoMessage::getHMASetFBValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_HMASETFB));
}
int FrontCamInfoMessage::getCamInitStValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_FCM_INITIALIZATION));
}
int FrontCamInfoMessage::getSLWFSetFBValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_SLWFSETFB));
}
int FrontCamInfoMessage::getCurrentRoadSpeedLimitStValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_TSRSPDLIMSTS));
}
int FrontCamInfoMessage::getISACascadeSoundAlarmValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_TSRSPDLIMWARNAU));
}
int FrontCamInfoMessage::getOverSpeedLimitWarnStValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_TSRSPDLIMWARN));
}
int FrontCamInfoMessage::getSLIFSetFBValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_SLIFSETFB));
}
int FrontCamInfoMessage::getELKRightModeValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_ELKRIGHTMODE));
}
int FrontCamInfoMessage::getELKLeftModeValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_ELKLEFTMODE));
}
int FrontCamInfoMessage::getTrafficSignValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_TRFCSIGN));
}
int FrontCamInfoMessage::getTSRLgtDstValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_TSRLGTDST));
}
int FrontCamInfoMessage::getTSRSetFBValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_TSR_SETFB));
}
int FrontCamInfoMessage::getTSRLatDstValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_TSR_LATDST));
}
int FrontCamInfoMessage::getTSRSpdLimValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_TSRSPDLIM));
}
int FrontCamInfoMessage::getSLIFOperStValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_SLIFOPERSTS));
}
int FrontCamInfoMessage::getLCCTakeoverReqValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_TJATAKEOVERREQ));
}
int FrontCamInfoMessage::getHandsOffWarningValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_AUDIOWARNHANDSOFF));
}
int FrontCamInfoMessage::getActDrvAsstStHmiValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_ACTDRVASSTSTSHMI));
}
int FrontCamInfoMessage::getLKAAudioWarningValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_LKA_AUDIOWARNING));
}
int FrontCamInfoMessage::getCamFaultStValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_FAULTTEXTINFO));
}
int FrontCamInfoMessage::getACCTarIDValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_TAROBJACC));
}
int FrontCamInfoMessage::getICATruckSetFBValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_ICATRUCKSETFB));
}
int FrontCamInfoMessage::getACCStartValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_ACCSTART));
}
int FrontCamInfoMessage::getADASUnavailReasonValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_ADASNOTAVLREAS));
}
int FrontCamInfoMessage::getACCTarSpdValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_VSETDIS));
}
int FrontCamInfoMessage::getACCTakeoverReqValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_ACCTAKEOVERREQ));
}
int FrontCamInfoMessage::getLCCSetFBValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_ICASETFB));
}
int FrontCamInfoMessage::getADASDriverReqValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_ADASDRVRREQ));
}
int FrontCamInfoMessage::getTAUGapSetValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_TAUGAPSET));
}
int FrontCamInfoMessage::getADASQuitReasonValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_ADASQUITREAS));
}
int FrontCamInfoMessage::getPCWLatentWarningValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_PCWLATENTWARN));
}
int FrontCamInfoMessage::getPCWPreWarningValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_PCWPREWARN));
}
int FrontCamInfoMessage::getACCStValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_ACCMODEHMI));
}
int FrontCamInfoMessage::getLDWSetFBValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_LDWSETFB));
}
int FrontCamInfoMessage::getLDWStValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_LDWSTS));
}
int FrontCamInfoMessage::getLKASuppsReasonValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_TEXTINFOLKA1));
}
int FrontCamInfoMessage::getLKASetFBValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_LKASETFB));
}
int FrontCamInfoMessage::getLeftLineColorValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_LELINECOLORFCT));
}
int FrontCamInfoMessage::getLKAStValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_LKASTS));
}
int FrontCamInfoMessage::getLDWAlertModeSetFBValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_LDWALERTMETHODSETFB));
}
int FrontCamInfoMessage::getRightLineColorValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_RILINECOLORFCT));
}
int FrontCamInfoMessage::getAEBObjIDValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_AEBOBJID));
}
int FrontCamInfoMessage::getLateralLimitValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_LATERALLIMIT));
}
int FrontCamInfoMessage::getLDWAudioWarningValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_LDWAUDIOWARN));
}
int FrontCamInfoMessage::getRAEBWorkStValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_RAEB_WORKSTS));
}
int FrontCamInfoMessage::getFCWSetFBValue() {
    return getValue(toInt(VehicleProperty::GECKO_FCM_FCWSETFB));
}

std::vector<uint8_t> FrontCamInfoMessage::generateDataPacket() {
    LOG(INFO) << "FrontCamInfoMessage, generateDataPacket ";
    std::vector<uint8_t> dataPacket;
    dataPacket.push_back(FrontCamInfoMessageID);
    uint16_t packlength = 0;
    dataPacket.push_back((packlength >> 8) & 0xFF);
    dataPacket.push_back(packlength & 0xFF);
    packlength = 0;
    //纵向功能激活标志位
    lgt_fct_actv_flg = getLongtdFuncActiveFlgValue();
    dataPacket.push_back(lgt_fct_actv_flg);
    packlength = packlength + 1;
    //ELK工作状态
    elk_sts = getELKStValue();
    dataPacket.push_back(elk_sts);
    packlength = packlength + 1;
    //控制器请求的转角值
    eps_set_steer_whl_agl = getEPSSetSteeringWheelAngleValue();
    dataPacket.push_back((eps_set_steer_whl_agl >> 8) & 0xFF);
    dataPacket.push_back(eps_set_steer_whl_agl & 0xFF);
    packlength = packlength + 2;
    //EPS模式切换请求
    daeps_mode_req = getEPSModeReqValue();
    dataPacket.push_back(daeps_mode_req);
    packlength = packlength + 1;
    //方向盘振动请求
    drvr_warn_vib_req = getDriverWarningVibrateReqValue();
    dataPacket.push_back(drvr_warn_vib_req);
    packlength = packlength + 1;
    //EPS角度控制请求和控制激活激活请求
    eps_ctrl_req = getEPSControleReqValue();
    dataPacket.push_back(eps_ctrl_req);
    packlength = packlength + 1;
    //EPS扭矩请求
    eps_tq_req = getEPSTorqueReqValue();
    dataPacket.push_back(eps_tq_req);
    packlength = packlength + 1;
    //LCC状态机
    lcc_st = getLCCStValue();
    dataPacket.push_back(lcc_st);
    packlength = packlength + 1;
    //LCC抑制、退出条件
    tjaica_supps_abortn = getLCCSuppressOrAbortConditionValue();
    dataPacket.push_back(tjaica_supps_abortn);
    packlength = packlength + 1;
    //转向灯请求
    turn_lamp_req = getTurnLampReqValue();
    dataPacket.push_back(turn_lamp_req);
    packlength = packlength + 1;
    //超车辅助功能系统状态
    overtake_ass_sys_sts = getOvertakeAssistantStValue();
    dataPacket.push_back(overtake_ass_sys_sts);
    packlength = packlength + 1;
    //EPS助力请求系数
    lka_torq_fact_req = getLKATorqueFactReqVaule();
    dataPacket.push_back(lka_torq_fact_req);
    packlength = packlength + 1;
    //LKA_LDW抑制条件
    lkaldw_supps_abortn = getLKALDWSuppsAbortnValue();
    dataPacket.push_back(lkaldw_supps_abortn);
    packlength = packlength + 1;
    //HMA抑制条件
    hma_supps_abortn = getHMASuppsAbortnValue();
    dataPacket.push_back(hma_supps_abortn);
    packlength = packlength + 1;
    //ELK抑制条件
    elk_supps_abortn = getELKSuppsAbortnValue();
    dataPacket.push_back(elk_supps_abortn);
    packlength = packlength + 1;
    //ACC/AEB/TJA/ICA故障状态
    flt_sts = getACCAEBTJAICAFaultSt();
    dataPacket.push_back(flt_sts);
    packlength = packlength + 1;
    //ACC轮端扭矩请求激活状态
    acc_whl_tq_req_actv = getACCTorqueReqActiveStValue();
    dataPacket.push_back(acc_whl_tq_req_actv);
    packlength = packlength + 1;
    //ACC轮端扭矩请求值
    acc_whl_tq_req = getACCTorqueReqValue();
    dataPacket.push_back((acc_whl_tq_req >> 8) & 0xFF);
    dataPacket.push_back(acc_whl_tq_req & 0xFF);
    packlength = packlength + 2;
    //FCM计算的轮端减速度
    acc_whl_dece = getFCMACCWheelDecelerationValue();
    dataPacket.push_back((acc_whl_dece >> 8) & 0xFF);
    dataPacket.push_back(acc_whl_dece & 0xFF);
    packlength = packlength + 2;
    //ACC目标加速度值
    acc_tar_accrn = getACCTargetAccelerationValue();
    dataPacket.push_back(acc_tar_accrn);
    packlength = packlength + 1;
    //ACC弯道降速系统状态
    crv_decelx_sta = getCurveDecelerationStValue();
    dataPacket.push_back(crv_decelx_sta);
    packlength = packlength + 1;
    //ACC目标加速度激活状态
    acc_tar_accrn_req_actv = getACCTargetAccelerationActiveStValue();
    dataPacket.push_back(acc_tar_accrn_req_actv);
    packlength = packlength + 1;
    //ACC抑制/退出条件
    acc_supp_abort = getACCSuppressOrAbortConditionValue();
    dataPacket.push_back(acc_supp_abort);
    packlength = packlength + 1;
    //ACC快退/慢退请求
    acc_shutdown_mode = getACCShutdownModeValue();
    dataPacket.push_back(acc_shutdown_mode);
    packlength = packlength + 1;
    //ACC目标减速度值
    acc_tgt_ax = getACCTargetDecelerationValue();
    dataPacket.push_back(acc_tgt_ax);
    packlength = packlength + 1;
    //ACC减速到停止状态
    acc_req_stand_still = getACCReqDecelerateToStopValue();
    dataPacket.push_back(acc_req_stand_still);
    packlength = packlength + 1;
    //ACC请求启动
    acc_req_drive_off = getACCDriveOffReqValue();
    dataPacket.push_back(acc_req_drive_off);
    packlength = packlength + 1;
    //ACC目标减速度请求
    acc_tgt_ax_req = getACCTargetDecelerationReqValue();
    dataPacket.push_back(acc_tgt_ax_req);
    packlength = packlength + 1;
    //ACC状态机
    acc_mode = getACCModeValue();
    dataPacket.push_back(acc_mode);
    packlength = packlength + 1;
    //EPB请求
    epb_req = getEPBReqValue();
    dataPacket.push_back(epb_req);
    packlength = packlength + 1;
    //前方对象ID
    ahd_obj_id = getAheadObjIDValue();
    dataPacket.push_back(ahd_obj_id);
    packlength = packlength + 1;
    //AEB目标减速度值
    aeb_tgt_ax = getAEBTargetDecelerationValue();
    dataPacket.push_back((aeb_tgt_ax >> 8) & 0xFF);
    dataPacket.push_back(aeb_tgt_ax & 0xFF);
    packlength = packlength + 2;
    //AEB刹停状态
    aeb_standstill_sts = getAEBStandstillStValue();
    dataPacket.push_back(aeb_standstill_sts);
    packlength = packlength + 1;
    //AEB刹停状态
    aeb_req = getAEBReqValue();
    dataPacket.push_back(aeb_req);
    packlength = packlength + 1;
    //ABA请求
    aba_req = getABAReqValue();
    dataPacket.push_back(aba_req);
    packlength = packlength + 1;
    //EBA请求
    eba_req = getEBAReqValue();
    dataPacket.push_back(eba_req);
    packlength = packlength + 1;
    //AWB等级
    awb_level = getAWBLevelValue();
    dataPacket.push_back(awb_level);
    packlength = packlength + 1;
    //ABA等级
    aba_level = getABALevelValue();
    dataPacket.push_back(aba_level);
    packlength = packlength + 1;
    //指示AEB系统的状态
    aeb_status = getAEBStValue();
    dataPacket.push_back(aeb_status);
    packlength = packlength + 1;
    //预充压请求
    abp_req = getESPPrefillReqValue();
    dataPacket.push_back(abp_req);
    packlength = packlength + 1;
    //AWB请求
    awb_req = getAWBReqValue();
    dataPacket.push_back(awb_req);
    packlength = packlength + 1;
    //智能远近光灯状态
    hma_sts = getHMAStValue();
    dataPacket.push_back(hma_sts);
    packlength = packlength + 1;
    //远光灯请求
    hma_highbeam_req = getHMAHighbeamReqValue();
    dataPacket.push_back(hma_highbeam_req);
    packlength = packlength + 1;
    //AEB设置反馈信号
    aeb_set_fb = getAEBSetFBValue();
    dataPacket.push_back(aeb_set_fb);
    packlength = packlength + 1;
    //AEB录制请求
    aeb_rec_req = getAEBRecReqValue();
    dataPacket.push_back(aeb_rec_req);
    packlength = packlength + 1;
    //ELK设置反馈信号
    elk_set_fb = getELKSetFBValue();
    dataPacket.push_back(elk_set_fb);
    packlength = packlength + 1;
    //HMA设置反馈信号
    hma_set_fb = getHMASetFBValue();
    dataPacket.push_back(hma_set_fb);
    packlength = packlength + 1;
    //用于指示摄像头初始化的状态
    fcm_intialization = getCamInitStValue();
    dataPacket.push_back(fcm_intialization);
    packlength = packlength + 1;
    //SLWF功能开关状态反馈
    slwf_set_fb = getSLWFSetFBValue();
    dataPacket.push_back(slwf_set_fb);
    packlength = packlength + 1;
    //指示当前道路的限速状态
    tsr_spd_lim_sts = getCurrentRoadSpeedLimitStValue();
    dataPacket.push_back(tsr_spd_lim_sts);
    packlength = packlength + 1;
    //ISA功能的级联声音报警
    tsr_spd_lim_warn_au = getISACascadeSoundAlarmValue();
    dataPacket.push_back(tsr_spd_lim_warn_au);
    packlength = packlength + 1;
    //限速报警
    tsr_spd_lim_warn = getOverSpeedLimitWarnStValue();
    dataPacket.push_back(tsr_spd_lim_warn);
    packlength = packlength + 1;
    //SLIF功能开关状态反馈
    slif_set_fb = getSLIFSetFBValue();
    dataPacket.push_back(slif_set_fb);
    packlength = packlength + 1;
    //ELK右侧工作模式
    elk_right_mode = getELKRightModeValue();
    dataPacket.push_back(elk_right_mode);
    packlength = packlength + 1;
    //ELK左侧工作模式
    elk_left_mode = getELKLeftModeValue();
    dataPacket.push_back(elk_left_mode);
    packlength = packlength + 1;
    //交通标识
    trfc_sign = getTrafficSignValue();
    dataPacket.push_back(trfc_sign);
    packlength = packlength + 1;
    //交通标识纵向距离
    tsr_lgt_dst = getTSRLgtDstValue();
    dataPacket.push_back((tsr_lgt_dst >> 8) & 0xFF);
    dataPacket.push_back(tsr_lgt_dst & 0xFF);
    packlength = packlength + 2;
    //交通标志识别设置反馈
    tsr_set_fb = getTSRSetFBValue();
    dataPacket.push_back(tsr_set_fb);
    packlength = packlength + 1;
    //交通标识横向距离
    tsr_lat_dst = getTSRLatDstValue();
    dataPacket.push_back(tsr_lat_dst);
    packlength = packlength + 1;
    //交通限速值
    tsr_spd_lim = getTSRSpdLimValue();
    dataPacket.push_back(tsr_spd_lim);
    packlength = packlength + 1;
    //SLIF功能运行状态
    slif_oper_sts = getSLIFOperStValue();
    dataPacket.push_back(slif_oper_sts);
    packlength = packlength + 1;
    //LCC接管请求
    lcc_tackover_req = getLCCTakeoverReqValue();
    dataPacket.push_back(lcc_tackover_req);
    packlength = packlength + 1;
    //脱手警告
    hands_off_warning = getHandsOffWarningValue();
    dataPacket.push_back(hands_off_warning);
    packlength = packlength + 1;
    //LCC状态机
    act_drv_asst_sts_hmi = getActDrvAsstStHmiValue();
    dataPacket.push_back(act_drv_asst_sts_hmi);
    packlength = packlength + 1;
    //LKA音频报警
    lka_audio_warning = getLKAAudioWarningValue();
    dataPacket.push_back(lka_audio_warning);
    packlength = packlength + 1;
    //摄像头故障状态
    cam_fault_st = getCamFaultStValue();
    dataPacket.push_back(cam_fault_st);
    packlength = packlength + 1;
    //ACC目标ID
    tar_obj_acc = getACCTarIDValue();
    dataPacket.push_back(tar_obj_acc);
    packlength = packlength + 1;
    //躲避卡车设置反馈
    ica_truck_set_fb = getICATruckSetFBValue();
    dataPacket.push_back(ica_truck_set_fb);
    packlength = packlength + 1;
    //前车起步提醒
    acc_start = getACCStartValue();
    dataPacket.push_back(acc_start);
    packlength = packlength + 1;
    //ADAS功能不可用原因
    adas_not_avl_reas = getADASUnavailReasonValue();
    dataPacket.push_back(adas_not_avl_reas);
    packlength = packlength + 1;
    //ACC目标车速
    v_set_dis = getACCTarSpdValue();
    dataPacket.push_back((v_set_dis >> 8) & 0xFF);
    dataPacket.push_back(v_set_dis & 0xFF);
    packlength = packlength + 2;
    //ACC接管提醒
    acc_take_over_req = getACCTakeoverReqValue();
    dataPacket.push_back(acc_take_over_req);
    packlength = packlength + 1;
    //LCC设置反馈
    ica_set_fb = getLCCSetFBValue();
    dataPacket.push_back(ica_set_fb);
    packlength = packlength + 1;
    //ADAS功能向驾驶员提醒请求
    adas_drvr_req = getADASDriverReqValue();
    dataPacket.push_back(adas_drvr_req);
    packlength = packlength + 1;
    //ACC时距
    tau_gap_set = getTAUGapSetValue();
    dataPacket.push_back(tau_gap_set);
    packlength = packlength + 1;
    //adas功能退出原因
    adas_quit_reas = getADASQuitReasonValue();
    dataPacket.push_back(adas_quit_reas);
    packlength = packlength + 1;
    //前碰撞预警预报警
    pcw_latent_warn = getPCWLatentWarningValue();
    dataPacket.push_back(pcw_latent_warn);
    packlength = packlength + 1;
    //前碰撞预警
    pcw_pre_warn = getPCWPreWarningValue();
    dataPacket.push_back(pcw_pre_warn);
    packlength = packlength + 1;
    //ACC状态机
    acc_mode_hmi = getACCStValue();
    dataPacket.push_back(acc_mode_hmi);
    packlength = packlength + 1;
    //LDW设置反馈信号
    ldw_set_fb = getLDWSetFBValue();
    dataPacket.push_back(ldw_set_fb);
    packlength = packlength + 1;
    //LDW状态
    ldw_sts = getLDWStValue();
    dataPacket.push_back(ldw_sts);
    packlength = packlength + 1;
    //LKA抑制原因
    text_info_lka1 = getLKASuppsReasonValue();
    dataPacket.push_back(text_info_lka1);
    packlength = packlength + 1;
    //LKA设置反馈
    lka_set_fb = getLKASetFBValue();
    dataPacket.push_back(lka_set_fb);
    packlength = packlength + 1;
    //左车道线颜色
    le_line_color_fct = getLeftLineColorValue();
    dataPacket.push_back(le_line_color_fct);
    packlength = packlength + 1;
    //LKA状态
    lka_sts = getLKAStValue();
    dataPacket.push_back(lka_sts);
    packlength = packlength + 1;
    //LDW报警方式设置反馈
    ldw_alert_method_set_fb = getLDWAlertModeSetFBValue();
    dataPacket.push_back(ldw_alert_method_set_fb);
    packlength = packlength + 1;
    //右车道线颜色
    ri_line_color_fct = getRightLineColorValue();
    dataPacket.push_back(ri_line_color_fct);
    packlength = packlength + 1;
    //AEB目标ID
    aeb_obj_id = getAEBObjIDValue();
    dataPacket.push_back((aeb_obj_id >> 8) & 0xFF);
    dataPacket.push_back(aeb_obj_id & 0xFF);
    packlength = packlength + 2;
    //LCC横向接管请求
    lateral_limit = getLateralLimitValue();
    dataPacket.push_back(lateral_limit);
    packlength = packlength + 1;
    //LDW音频警告
    ldw_audio_warn = getLDWAudioWarningValue();
    dataPacket.push_back(ldw_audio_warn);
    packlength = packlength + 1;
    //后向AEB工作状态
    raeb_work_sts = getRAEBWorkStValue();
    dataPacket.push_back(raeb_work_sts);
    packlength = packlength + 1;
    //FCW设置反馈
    fcw_set_fb = getFCWSetFBValue();
    dataPacket.push_back(fcw_set_fb);
    packlength = packlength + 1;

    dataPacket[1] = (packlength >> 8) & 0xFF;
    dataPacket[2] = packlength & 0xFF;
    return dataPacket;
}