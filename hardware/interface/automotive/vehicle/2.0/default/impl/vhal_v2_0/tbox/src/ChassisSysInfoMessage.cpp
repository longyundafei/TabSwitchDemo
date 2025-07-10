#include "ChassisSysInfoMessage.h"
int ChassisSysInfoMessage::getDynamicApplyStValue() {
    return getValue(toInt(VehicleProperty::GECKO_EHB_EPB_DYNAMIC_APPLY_STA));
}
int ChassisSysInfoMessage::getBrakePedalValue() {
    int valide = getValue(toInt(VehicleProperty::GECKO_EHB_BRKPEDL_VALVLD));
    int ret = 0x7FFF;
    if(valide == 0x01){
        ret = getValue(toInt(VehicleProperty::GECKO_EHB_BRKPEDL_VAL));
        if(ret == 0xFFFF){
            ret = 0x7FFF;
        }
    }
    return ret;
}
int ChassisSysInfoMessage::getBrakePedalPercentValue() {
    int valide = getValue(toInt(VehicleProperty::GECKO_EHB_BRKPEDL_VALPERCVLD));
    int ret = 0xFF;
    if(valide == 0x01){
        ret = getValue(toInt(VehicleProperty::GECKO_EHB_BRKPEDL_VALPERC));
    }
    return ret;
}
int ChassisSysInfoMessage::getBrakeSedalTravelSensorFailValue() {
    return getValue(toInt(VehicleProperty::GECKO_EHB_PEDL_TRVL_SNSR_FAIL_PDT));
}
int ChassisSysInfoMessage::getBrakeLightOnReqValue() {
    return getValue(toInt(VehicleProperty::GECKO_EHB_REQ_BRK_LI_ON));
}
int ChassisSysInfoMessage::getBrakePedalTravelSensorFailValue() {
    return getValue(toInt(VehicleProperty::GECKO_EHB_PEDL_TRVL_SNSR_FAIL_PDF));
}
int ChassisSysInfoMessage::getRunoutPressureValue() {
    return getValue(toInt(VehicleProperty::GECKO_EHB_P_RUNOUT_PRESSURE));
}
int ChassisSysInfoMessage::getBrakePedalAppliedValue() {
    int valide = getValue(toInt(VehicleProperty::GECKO_EHB_BRAKE_PEDAL_APPLIED_Q));
    if(valide == 0x01){
        return getValue(toInt(VehicleProperty::GECKO_EHB_BRAKE_PEDAL_APPLIED));
    } else {
        return 0xFF;
    }
}
int ChassisSysInfoMessage::getBrakeTempHighValue() {
    return getValue(toInt(VehicleProperty::GECKO_EHB_CDD_TEMP_OFF));
}
/*int ChassisSysInfoMessage::getOutputRodHydraulicTarValue() {
    return getValue(toInt(VehicleProperty::GECKO_EHB_S_OUTPUT_ROD_HYDRAULIC_TARGET));
}*/
int ChassisSysInfoMessage::getEHBOutputRoodDriverPercValue() {
    return getValue(toInt(VehicleProperty::GECKO_EHB_S_OUTPUT_ROD_DRIVER_PERC));
}
int ChassisSysInfoMessage::getEHBRightEPBActuatorStValue() {
    return getValue(toInt(VehicleProperty::GECKO_EHB_EPB_ACTUATOR_ST_R));
}
int ChassisSysInfoMessage::getAdaptiveBrakeAssistActiveStValue() {
    int valide = getValue(toInt(VehicleProperty::GECKO_EHB_ABA_AVAILABLE_STA));
    if(valide == 0x01){
        return getValue(toInt(VehicleProperty::GECKO_EHB_ABA_ACTIVE_STA));
    } else {
        return 0xFF;
    }
}
int ChassisSysInfoMessage::getAutomaticWarningBrakeActiveStValue() {
    int valide = getValue(toInt(VehicleProperty::GECKO_EHB_AWB_AVAILABLE_STA));
    if(valide == 0x01){
        return getValue(toInt(VehicleProperty::GECKO_EHB_AWB_ACTIVE_STA));
    } else {
        return 0xFF;
    }
}
int ChassisSysInfoMessage::getAutomaticEmergencyBrakeActiveStValue() {
    int valide = getValue(toInt(VehicleProperty::GECKO_EHB_AEB_AVAILABLE_STA));
    if(valide == 0x01){
        return getValue(toInt(VehicleProperty::GECKO_EHB_AEB_ACTIVE_STA));
    } else {
        return 0xFF;
    }
}
int ChassisSysInfoMessage::getCDPReqValue() {
    return getValue(toInt(VehicleProperty::GECKO_EHB_CDP_REQUEST));
}
int ChassisSysInfoMessage::getLeftEPBActuatorStValue() {
    return getValue(toInt(VehicleProperty::GECKO_EHB_EPB_ACTUATOR_ST_L));
}
int ChassisSysInfoMessage::getEPBWarningLampStValue() {
    return getValue(toInt(VehicleProperty::GECKO_EHB_EPB_WARN_LAMP_STA));
}
int ChassisSysInfoMessage::getEPBFaultStValue() {
    return getValue(toInt(VehicleProperty::GECKO_EHB_EPB_FLTSTS));
}
int ChassisSysInfoMessage::getEPBSysStValue() {
    return getValue(toInt(VehicleProperty::GECKO_ELECTRONIC_PARKING_MODE));
}
int ChassisSysInfoMessage::getBrakeLiquidLevelWarningValue() {
    return getValue(toInt(VehicleProperty::GECKO_EHB_BRAKE_LIQUID_LEVEL_WARN));
}
int ChassisSysInfoMessage::getEHBFailStValue() {
    return getValue(toInt(VehicleProperty::GECKO_EHB_FAILURESTS));
}
int ChassisSysInfoMessage::getAVHLampReqValue() {
    return getValue(toInt(VehicleProperty::GECKO_EHB_AVH_LAMP_REQ));
}
int ChassisSysInfoMessage::getHHCStValue() {
    return getValue(toInt(VehicleProperty::GECKO_EHB_HHC_ACTIVE_STA));
}
int ChassisSysInfoMessage::getRgnBrakeStValue() {
    return getValue(toInt(VehicleProperty::GECKO_EHB_RGN_BRK_ST));
}
int ChassisSysInfoMessage::getAVHApplyStValue() {
    return getValue(toInt(VehicleProperty::GECKO_AUTO_PARKING_ON));
}
int ChassisSysInfoMessage::getEHBExtReqStValue() {
    return getValue(toInt(VehicleProperty::GECKO_EHB_EXT_REQ_STATUS));
}
int ChassisSysInfoMessage::getEHBBLAValue() {
    return getValue(toInt(VehicleProperty::GECKO_EHB_BLA));
}
int ChassisSysInfoMessage::getEHBWarningOnValue() {
    return getValue(toInt(VehicleProperty::GECKO_EHB_HMI_WARNING_ON));
}
int ChassisSysInfoMessage::getVehicleStandstillStValue() {
    return getValue(toInt(VehicleProperty::GECKO_ESC_VHCL_STANDSTILL_STA));
}
int ChassisSysInfoMessage::getFLWheelDirValue() {
    return getValue(toInt(VehicleProperty::GECKO_ESC_WHEEL_DIRECTION_FL));
}
int ChassisSysInfoMessage::getLFWheelSpdKPHValue() {
    int valide = getValue(toInt(VehicleProperty::GECKO_ESC_WHEEL_SPEED_KPH_V_FL));
    int ret = 0x1FFF;
    if(valide == 0x01){
        ret = getValue(toInt(VehicleProperty::GECKO_ESC_WHEEL_SPEED_KPH_FL));
        if(ret == 0xFFFF){
            ret = 0x1FFF;
        }
    }
    return ret;
}
int ChassisSysInfoMessage::getFRWheelDirValue() {
    return getValue(toInt(VehicleProperty::GECKO_ESC_WHEEL_DIRECTION_FR));
}
int ChassisSysInfoMessage::getFRWheelSpdKPHValue() {
    int valide = getValue(toInt(VehicleProperty::GECKO_ESC_WHEEL_SPEED_KPH_V_FR));
    int ret = 0x1FFF;
    if(valide == 0x01){
        ret = getValue(toInt(VehicleProperty::GECKO_ESC_WHEEL_SPEED_KPH_FR));
        if(ret == 0xFFFF){
            ret = 0x1FFF;
        }
    }
    return ret;
}
int ChassisSysInfoMessage::getMasterCylinderPressureValue() {
    int valide = getValue(toInt(VehicleProperty::GECKO_ESC_MASTER_CYLINDER_PRESR_V));
    if(valide == 0x01){
        return getValue(toInt(VehicleProperty::GECKO_ESC_MASTER_CYLINDER_PRESR));
    } else {
        return 0xFF;
    }
}
int ChassisSysInfoMessage::getRLWheelDirValue() {
    return getValue(toInt(VehicleProperty::GECKO_ESC_WHEEL_DIRECTION_RL));
}
int ChassisSysInfoMessage::getRLWheelSpdKPHValue() {
    int valide = getValue(toInt(VehicleProperty::GECKO_ESC_WHEEL_SPEED_KPH_V_RL));
    int ret = 0x1FFF;
    if(valide == 0x01){
        ret = getValue(toInt(VehicleProperty::GECKO_ESC_WHEEL_SPEED_KPH_RL));
        if(ret == 0xFFFF){
            ret = 0x1FFF;
        }
    }
    return ret;
}
int ChassisSysInfoMessage::getRRWheelDirValue() {
    return getValue(toInt(VehicleProperty::GECKO_ESC_WHEEL_DIRECTION_RR));
}
int ChassisSysInfoMessage::getRRWheelSpdKPHValue() {
    int valide = getValue(toInt(VehicleProperty::GECKO_ESC_WHEEL_SPEED_KPH_V_RR));
    int ret = 0x1FFF;
    if(valide == 0x01){
        ret = getValue(toInt(VehicleProperty::GECKO_ESC_WHEEL_SPEED_KPH_RR));
        if(ret == 0xFFFF){
            ret = 0x1FFF;
        }
    }
    return ret;
}
int ChassisSysInfoMessage::getLongtdAccSensorValue() {
    int valide = getValue(toInt(VehicleProperty::GECKO_ESC_LONG_ACC_SENSOR_VALUE_VLD));
    int ret = 0x3FFF;
    if(valide == 0x01){
        ret = getValue(toInt(VehicleProperty::GECKO_ESC_LONG_ACC_SENSOR_VALUE));
        if(ret == 0xFFFF){
            ret = 0x3FFF;
        }
    }
    return ret;
}
int ChassisSysInfoMessage::getLatAccSensorValue() {
    int valide = getValue(toInt(VehicleProperty::GECKO_ESC_LAT_ACC_SENSOR_VALUE_VLD));
    int ret = 0x3FFF;
    if(valide == 0x01){
        ret = getValue(toInt(VehicleProperty::GECKO_ESC_LAT_ACC_SENSOR_VALUE));
        if(ret == 0xFFFF){
            ret = 0x3FFF;
        }
    }
    return ret;
}
int ChassisSysInfoMessage::getYawRateValue() {
    int valide = getValue(toInt(VehicleProperty::GECKO_ESC_YAW_RATE_VLD));
    int ret = 0xFFFF;
    if(valide == 0x01){
        ret = getValue(toInt(VehicleProperty::GECKO_ESC_YAW_RATE));
    }
    return ret;
}
int ChassisSysInfoMessage::getESCRightEPBActuatorStValue() {
    return getValue(toInt(VehicleProperty::GECKO_ESC_EPB_ACTUATOR_ST_R));
}
int ChassisSysInfoMessage::getESCorTCSFailedValue() {
    return getValue(toInt(VehicleProperty::GECKO_ESC_ES_COR_TCS_FAILED));
}
int ChassisSysInfoMessage::getABSActiveStValue() {
    return getValue(toInt(VehicleProperty::GECKO_ESC_ABSACTIVESTA));
}
int ChassisSysInfoMessage::getVehicleSpdValue() {
    return getValue(toInt(VehicleProperty::PERF_VEHICLE_SPEED));
}
int ChassisSysInfoMessage::getEBDFailedValue() {
    return getValue(toInt(VehicleProperty::GECKO_ESC_EBD_FAILED));
}
int ChassisSysInfoMessage::getABSFailedValue() {
    return getValue(toInt(VehicleProperty::GECKO_ABS_ABS_FAIL_STATUS));
}
int ChassisSysInfoMessage::getTCSActiveStValue() {
    return getValue(toInt(VehicleProperty::GECKO_ESC_TCS_ACTIVE_STA));
}
int ChassisSysInfoMessage::getESPActiveStValue() {
    return getValue(toInt(VehicleProperty::GECKO_ESC_ESP_ACTIVE_STA));
}
int ChassisSysInfoMessage::getReqBrakeLightOnValue() {
    return getValue(toInt(VehicleProperty::GECKO_ESC_REQ_BRAKE_LIGHT_ON));
}
int ChassisSysInfoMessage::getRMIActiveStValue() {
    return getValue(toInt(VehicleProperty::GECKO_ESC_RMIACTIVESTA));
}
int ChassisSysInfoMessage::getCDPActiveValue() {
    return getValue(toInt(VehicleProperty::GECKO_ESC_CDP_ACTIVE));
}
int ChassisSysInfoMessage::getESSActiveStValue() {
    return getValue(toInt(VehicleProperty::GECKO_ESC_ESS_ACTIVE));
}
int ChassisSysInfoMessage::getEBDActiveStValue() {
    return getValue(toInt(VehicleProperty::GECKO_ESC_EBD_ACTIVE_STA));
}
int ChassisSysInfoMessage::getESCDisValue() {
    return getValue(toInt(VehicleProperty::GECKO_VEHICLE_STABILITY_ON));
}
int ChassisSysInfoMessage::getSlopeValue() {
    int valide = getValue(toInt(VehicleProperty::GECKO_ESC_SLOPEPERCENTV));
    if(valide == 0x02){
        return getValue(toInt(VehicleProperty::GECKO_ESC_SLOPEPERCENT));
    } else {
        return 0xFF;
    }
}
int ChassisSysInfoMessage::getHapticWarningServiceAvailValue() {
    return getValue(toInt(VehicleProperty::GECKO_EPS_LAT_MC_HPTC_WRN_SRV_AVL));
}
int ChassisSysInfoMessage::getLatLevel12DrivingServiceAvailValue() {
    return getValue(toInt(VehicleProperty::GECKO_EPS_LAT_DRV_LVL12_SRV_AVL));
}
int ChassisSysInfoMessage::getEPSMotoreTorqueValue() {
    return getValue(toInt(VehicleProperty::GECKO_EPS_MOT_TQ));
}
int ChassisSysInfoMessage::getMaxInterRPCTorqueValue() {
    return getValue(toInt(VehicleProperty::GECKO_EPS_PSCM_INT_RPC_MAX_TRQ));
}
int ChassisSysInfoMessage::getMinInterRPCTorqueValue() {
    return getValue(toInt(VehicleProperty::GECKO_EPS_PSCM_INT_RPC_MIN_TRQ));
}
int ChassisSysInfoMessage::getLatMCHapticWarningActValue() {
    return getValue(toInt(VehicleProperty::GECKO_EPS_LAT_MC_HPTC_WRN_ACTIVE));
}
int ChassisSysInfoMessage::getLatMCSteerTorqueActValue() {
    return getValue(toInt(VehicleProperty::GECKO_EPS_LAT_MC_STEER_TQ_ACTIVE));
}
int ChassisSysInfoMessage::getAngleSysAvailStValue() {
    return getValue(toInt(VehicleProperty::GECKO_EPS_LKS_AGL_SYS_STS));
}
int ChassisSysInfoMessage::getMaxAllowedRotSpdValue() {
    return getValue(toInt(VehicleProperty::GECKO_EPS_MAX_ALLWD_ROT_SPD));
}
int ChassisSysInfoMessage::getMaxAllowedAngleValue() {
    return getValue(toInt(VehicleProperty::GECKO_EPS_MAX_ALLWD_AGL));
}
int ChassisSysInfoMessage::getEPSteerTorqueValue() {
    return getValue(toInt(VehicleProperty::GECKO_EPS_STEER_TQ));
}
int ChassisSysInfoMessage::getLKSAngleCtrlAvailStValue() {
    return getValue(toInt(VehicleProperty::GECKO_EPS_LKS_AGL_CTRL_AVL_STS));
}
int ChassisSysInfoMessage::getLKSAbortFBValue() {
    return getValue(toInt(VehicleProperty::GECKO_EPS_LKS_ABORT_FB));
}
int ChassisSysInfoMessage::getTorisonBarTorqueQFValue() {
    return getValue(toInt(VehicleProperty::GECKO_EPS_TORSION_BAR_TORQUE_QF));
}
int ChassisSysInfoMessage::getSteerWhellAngleValue() {
    int valide = getValue(toInt(VehicleProperty::GECKO_EPS_STEER_WHEEL_ANGLE_VALID));
    if(valide == 0x01){
        return getValue(toInt(VehicleProperty::GECKO_EPS_STEER_WHEEL_ANGLE));
    } else {
        return 0xFFFF;
    }
}
int ChassisSysInfoMessage::getSteerWheelRotSpdValue() {
    return getValue(toInt(VehicleProperty::GECKO_EPS_ROT_SPEED));
}
int ChassisSysInfoMessage::getSteerWheelAngleCalValue() {
    return getValue(toInt(VehicleProperty::GECKO_EPS_STEER_WHEEL_ANGLE_CALIBRATED));
}
int ChassisSysInfoMessage::getSteerWheelAngleFailValue() {
    return getValue(toInt(VehicleProperty::GECKO_EPS_STEER_WHEEL_ANGLE_FAILURE));
}
int ChassisSysInfoMessage::getElectricPowerStValue() {
    return getValue(toInt(VehicleProperty::GECKO_EPS_ELECTRIC_POWER_STA));
}
int ChassisSysInfoMessage::getRorisionBarTorqueDirValue() {
    int valide = getValue(toInt(VehicleProperty::GECKO_EPS_TORSION_BAR_TORQUE_VALID));
    if(valide == 0x01){
        return getValue(toInt(VehicleProperty::GECKO_EPS_TORSION_BAR_TORQUE_DIR));
    } else {
        return 0xFF;
    }
}
int ChassisSysInfoMessage::getElecSteerModeStValue() {
    return getValue(toInt(VehicleProperty::GECKO_POWER_STEERING_MODE));
}
int ChassisSysInfoMessage::getEPSModeSetInhibitFBValue() {
    return getValue(toInt(VehicleProperty::GECKO_EPS_MODESETINHIBITFB));
}
int ChassisSysInfoMessage::getBasicElecSteerFailValue() {
    return getValue(toInt(VehicleProperty::GECKO_EPS_BASICELECSTEERFAILED));
}
int ChassisSysInfoMessage::getTorsionBarTorqueValue() {
    return getValue(toInt(VehicleProperty::GECKO_EPS_TORSION_BAR_TORQUE));
}
int ChassisSysInfoMessage::getElecPowerConsumptionValue() {
    return getValue(toInt(VehicleProperty::GECKO_EPS_ELECT_POWER_CONSUMPTION));
}
int ChassisSysInfoMessage::getEPSAdvancedModStValue() {
    return getValue(toInt(VehicleProperty::GECKO_EPS_ADVANCED_MODE_STATUS));
}
int ChassisSysInfoMessage::getEPSFaultLevelValue() {
    return getValue(toInt(VehicleProperty::GECKO_EPS_RUN_STATE));
}
int ChassisSysInfoMessage::getHodHansoffStValue() {
    return getValue(toInt(VehicleProperty::GECKO_EPS_SWS_HODHANDSOFFSTS));
}

std::vector<uint8_t> ChassisSysInfoMessage::generateDataPacket() {
    std::vector<uint8_t> dataPacket;
    dataPacket.push_back(ChassisSysInfoId);
    uint16_t packlength = 0;
    dataPacket.push_back((packlength >> 8) & 0xFF);
    dataPacket.push_back(packlength & 0xFF);
    packlength = 0;
    //动态制动状态
    dynamic_apply_sta = getDynamicApplyStValue();
    dataPacket.push_back(dynamic_apply_sta);
    packlength = packlength + 1;
    //制动踏板行程
    brk_pedl_val = getBrakePedalValue();
    dataPacket.push_back((brk_pedl_val >> 8) & 0xFF);
    dataPacket.push_back(brk_pedl_val & 0xFF);
    packlength = packlength + 2;
    //制动踏板行程-百分比
    brk_pedl_val_perc = getBrakePedalPercentValue();
    dataPacket.push_back(brk_pedl_val_perc);
    packlength = packlength + 1;
    //制动行程传感器故障
    pedl_trvl_snsr_fail_pdt = getBrakeSedalTravelSensorFailValue();
    dataPacket.push_back(pedl_trvl_snsr_fail_pdt);
    packlength = packlength + 1;
    //制动灯点亮请求
    req_brk_li_on = getBrakeLightOnReqValue();
    dataPacket.push_back(req_brk_li_on);
    packlength = packlength + 1;
    //制动踏板行程传感器故障
    pedl_trvl_snsr_fail_pdf = getBrakePedalTravelSensorFailValue();
    dataPacket.push_back(pedl_trvl_snsr_fail_pdf);
    packlength = packlength + 1;
    //拐点压力值
    runout_pressure = getRunoutPressureValue();
    dataPacket.push_back(runout_pressure);
    packlength = packlength + 1;
    //驾驶员是否踩下制动踏板
    brake_pedal_applied =  getBrakePedalAppliedValue();
    dataPacket.push_back(brake_pedal_applied);
    packlength = packlength + 1;
    //指示制动盘温度是否过高
    cdd_temp_off = getBrakeTempHighValue();
    dataPacket.push_back(cdd_temp_off);
    packlength = packlength + 1;
    //EHB主缸目标压力信号 remove later
    /*output_rod_hydraulic_target = getOutputRodHydraulicTarValue();
    dataPacket.push_back((output_rod_hydraulic_target >> 8) & 0xFF);
    dataPacket.push_back(output_rod_hydraulic_target & 0xFF);
    packlength = packlength + 2;*/
    //EHB输入推杆百分比
    output_rod_driver_perc = getEHBOutputRoodDriverPercValue();
    dataPacket.push_back(output_rod_driver_perc);
    packlength = packlength + 1;
    //右Moc状态
    actuator_st_r = getEHBRightEPBActuatorStValue();
    dataPacket.push_back(actuator_st_r);
    packlength = packlength + 1;
    //自适应制动辅助激活状态
    aba_active_sta = getAdaptiveBrakeAssistActiveStValue();
    dataPacket.push_back(aba_active_sta);
    packlength = packlength + 1;
    //自动警告制动激活状态
    awb_active_sta = getAutomaticWarningBrakeActiveStValue();
    dataPacket.push_back(awb_active_sta);
    packlength = packlength + 1;
    //自动紧急制动激活状态
    aeb_active_sta = getAutomaticEmergencyBrakeActiveStValue();
    dataPacket.push_back(aeb_active_sta);
    packlength = packlength + 1;
    //动态驻车请求
    cdp_request = getCDPReqValue();
    dataPacket.push_back(cdp_request);
    packlength = packlength + 1;
    //左Moc状态
    actuator_st_l = getLeftEPBActuatorStValue();
    dataPacket.push_back(actuator_st_l);
    packlength = packlength + 1;
    //EPB故障灯状态
    epb_warn_lamp_sta = getEPBWarningLampStValue();
    dataPacket.push_back(epb_warn_lamp_sta);
    packlength = packlength + 1;
    //EPB故障状态
    epb_flt_sts = getEPBFaultStValue();
    dataPacket.push_back(epb_flt_sts);
    packlength = packlength + 1;
    //EPB执行机构统一状态
    epb_system_state = getEPBSysStValue();
    dataPacket.push_back(epb_system_state);
    packlength = packlength + 1;
    //制动液位报警
    brake_liquid_level_warn = getBrakeLiquidLevelWarningValue();
    dataPacket.push_back(brake_liquid_level_warn);
    packlength = packlength + 1;
    //EHB故障状态
    ehb_failure_sts = getEHBFailStValue();
    dataPacket.push_back(ehb_failure_sts);
    packlength = packlength + 1;
    //AVH点灯请求
    avh_lamp_req = getAVHLampReqValue();
    dataPacket.push_back(avh_lamp_req);
    packlength = packlength + 1;
    //HHC状态
    hhc_active_sta =  getHHCStValue();
    dataPacket.push_back(hhc_active_sta);
    packlength = packlength + 1;
    //制动能量回收状态
    rgn_brk_st = getRgnBrakeStValue();
    dataPacket.push_back(rgn_brk_st);
    packlength = packlength + 1;
    //AVH功能状态
    avh_appld_st = getAVHApplyStValue();
    dataPacket.push_back(avh_appld_st);
    packlength = packlength + 1;
    //电子助力器外部请求可用状态
    ext_req_status = getEHBExtReqStValue();
    dataPacket.push_back(ext_req_status);
    packlength = packlength + 1;
    //电子助力器制动灯请求
    bla = getEHBBLAValue();
    dataPacket.push_back(bla);
    packlength = packlength + 1;
    //EHB警告灯请求
    hmi_warning_on = getEHBWarningOnValue();
    dataPacket.push_back(hmi_warning_on);
    packlength = packlength + 1;
    //车辆静止状态
    vhcl_standstill_sta = getVehicleStandstillStValue();
    dataPacket.push_back(vhcl_standstill_sta);
    packlength = packlength + 1;
    //ESC左前轮方向
    wheel_direction_fl = getFLWheelDirValue();
    dataPacket.push_back(wheel_direction_fl);
    packlength = packlength + 1;
    //ESC左前轮速度
    wheel_speed_kph_fl = getLFWheelSpdKPHValue();
    dataPacket.push_back((wheel_speed_kph_fl >> 8) & 0xFF);
    dataPacket.push_back(wheel_speed_kph_fl & 0xFF);
    packlength = packlength + 2;
    //ESC右前轮方向
    wheel_direction_fr = getFRWheelDirValue();
    dataPacket.push_back(wheel_direction_fr);
    packlength = packlength + 1;
    //ESC右前轮速度
    wheel_speed_kph_fr = getFRWheelSpdKPHValue();
    dataPacket.push_back((wheel_speed_kph_fr >> 8) & 0xFF);
    dataPacket.push_back(wheel_speed_kph_fr & 0xFF);
    packlength = packlength + 2;
    //主缸压力
    master_cylinder_presr = getMasterCylinderPressureValue();
    dataPacket.push_back(master_cylinder_presr);
    packlength = packlength + 1;
    //ESC左后轮方向
    wheel_direction_rl = getRLWheelDirValue();
    dataPacket.push_back(wheel_direction_rl);
    packlength = packlength + 1;
    //ESC左后轮速度
    wheel_speed_kph_rl = getRLWheelSpdKPHValue();
    dataPacket.push_back((wheel_speed_kph_rl >> 8) & 0xFF);
    dataPacket.push_back(wheel_speed_kph_rl & 0xFF);
    packlength = packlength + 2;
    //ESC右后轮方向
    wheel_direction_rr =getRRWheelDirValue();
    dataPacket.push_back(wheel_direction_rr);
    packlength = packlength + 1;
    //ESC右后轮速度
    wheel_speed_kph_rr = getRRWheelSpdKPHValue();
    dataPacket.push_back((wheel_speed_kph_rr >> 8) & 0xFF);
    dataPacket.push_back(wheel_speed_kph_rr & 0xFF);
    packlength = packlength + 2;
    //纵向加速度值
    long_acc_sensor_value = getLongtdAccSensorValue();
    dataPacket.push_back((long_acc_sensor_value >> 8) & 0xFF);
    dataPacket.push_back(long_acc_sensor_value & 0xFF);
    packlength = packlength + 2;
    //横向加速度值
    lat_acc_sensor_value = getLatAccSensorValue();
    dataPacket.push_back((lat_acc_sensor_value >> 8) & 0xFF);
    dataPacket.push_back(lat_acc_sensor_value & 0xFF);
    packlength = packlength + 2;
    //横摆角速度
    yaw_rate = getYawRateValue();
    dataPacket.push_back((yaw_rate >> 8) & 0xFF);
    dataPacket.push_back(yaw_rate & 0xFF);
    packlength = packlength + 2;
    //右Moc状态
    epb_actuator_st_r = getESCRightEPBActuatorStValue();
    dataPacket.push_back(epb_actuator_st_r);
    packlength = packlength + 1;
    //ESC/TCS故障
    esc_or_tcs_failed = getESCorTCSFailedValue();
    dataPacket.push_back(esc_or_tcs_failed);
    packlength = packlength + 1;
    //ABS激活状态
    abs_active_sta = getABSActiveStValue();
    dataPacket.push_back(abs_active_sta);
    packlength = packlength + 1;
    //车速
    vehicle_speed = getVehicleSpdValue();
    dataPacket.push_back((vehicle_speed >> 8) & 0xFF);
    dataPacket.push_back(vehicle_speed & 0xFF);
    packlength = packlength + 2;
    //EBD故障
    ebd_failed = getEBDFailedValue();
    dataPacket.push_back(ebd_failed);
    packlength = packlength + 1;
    //ABS故障/正常电流运行状态
    esc_abs_failed = getABSFailedValue();
    dataPacket.push_back(esc_abs_failed);
    packlength = packlength + 1;
    //TCS激活状态
    tcs_active_sta = getTCSActiveStValue();
    dataPacket.push_back(tcs_active_sta);
    packlength = packlength + 1;
    //ESP激活状态
    esp_active_sta = getESPActiveStValue();
    dataPacket.push_back(esp_active_sta);
    packlength = packlength + 1;
    //请求点亮刹车灯
    req_brake_light_on = getReqBrakeLightOnValue();
    dataPacket.push_back(req_brake_light_on);
    packlength = packlength + 1;
    //RMI 激活状态
    rmi_active_sta = getRMIActiveStValue();
    dataPacket.push_back(rmi_active_sta);
    packlength = packlength + 1;
    //CDP激活状态
    cdp_active = getCDPActiveValue();
    dataPacket.push_back(cdp_active);
    packlength = packlength + 1;
    //ESS激活状态
    ess_active = getESSActiveStValue();
    dataPacket.push_back(ess_active);
    packlength = packlength + 1;
    //电子制动力分配激活信号
    ebd_active_sta = getEBDActiveStValue();
    dataPacket.push_back(ebd_active_sta);
    packlength = packlength + 1;
    //ESC关闭状态
    esc_disable = getESCDisValue();
    dataPacket.push_back(esc_disable);
    packlength = packlength + 1;
    //坡度信号
    slope_percent = getSlopeValue();
    dataPacket.push_back(slope_percent);
    packlength = packlength + 1;
    //响应触觉请求的可用性
    lat_mc_hptc_wrn_srv_avl = getHapticWarningServiceAvailValue();
    dataPacket.push_back(lat_mc_hptc_wrn_srv_avl);
    packlength = packlength + 1;
    //横向驾驶服务可用性
    lat_drv_lvl12_srv_avl = getLatLevel12DrivingServiceAvailValue();
    dataPacket.push_back(lat_drv_lvl12_srv_avl);
    packlength = packlength + 1;
    //EPS电机的实际转矩输出
    eps_mot_tq = getEPSMotoreTorqueValue();
    dataPacket.push_back((eps_mot_tq >> 8) & 0xFF);
    dataPacket.push_back(eps_mot_tq & 0xFF);
    packlength = packlength + 2;
    //内部允许的最大RPC扭矩
    pscm_int_rpc_max_trq = getMaxInterRPCTorqueValue();
    dataPacket.push_back((pscm_int_rpc_max_trq >> 8) & 0xFF);
    dataPacket.push_back(pscm_int_rpc_max_trq & 0xFF);
    packlength = packlength + 2;
    //内部允许的最小RPC扭矩
    pscm_int_rpc_min_trq = getMinInterRPCTorqueValue();
    dataPacket.push_back((pscm_int_rpc_min_trq >> 8) & 0xFF);
    dataPacket.push_back(pscm_int_rpc_min_trq & 0xFF);
    packlength = packlength + 2;
    //触觉警告服务激活
    lat_mc_hptc_wrn_active = getLatMCHapticWarningActValue();
    dataPacket.push_back(lat_mc_hptc_wrn_active);
    packlength = packlength + 1;
    //横向运动控制转向扭矩服务激活
    lat_mc_steer_tq_active = getLatMCSteerTorqueActValue();
    dataPacket.push_back(lat_mc_steer_tq_active);
    packlength = packlength + 1;
    //横向角度控制可用状态
    lks_agl_sys_sts = getAngleSysAvailStValue();
    dataPacket.push_back(lks_agl_sys_sts);
    packlength = packlength + 1;
    //最大允许控制角速度
    max_allwd_rot_spd = getMaxAllowedRotSpdValue();
    dataPacket.push_back((max_allwd_rot_spd >> 8) & 0xFF);
    dataPacket.push_back(max_allwd_rot_spd & 0xFF);
    packlength = packlength + 2;
    //最大允许控制角度
    max_allwd_agl = getMaxAllowedAngleValue();
    dataPacket.push_back((max_allwd_agl >> 8) & 0xFF);
    dataPacket.push_back(max_allwd_agl & 0xFF);
    packlength = packlength + 2;
    //EPS转向扭矩
    eps_steer_tq = getEPSteerTorqueValue();
    dataPacket.push_back((eps_steer_tq >> 8) & 0xFF);
    dataPacket.push_back(eps_steer_tq & 0xFF);
    packlength = packlength + 2;
    //横向角度控制激活状态
    lks_agl_ctrl_avl_sts = getLKSAngleCtrlAvailStValue();
    dataPacket.push_back(lks_agl_ctrl_avl_sts);
    packlength = packlength + 1;
    //LKS中断原因反馈
    lks_abort_fb = getLKSAbortFBValue();
    dataPacket.push_back(lks_abort_fb);
    packlength = packlength + 1;
    //扭矩传感器测量结果的有效性
    torsion_bar_torque_qf = getTorisonBarTorqueQFValue();
    dataPacket.push_back(torsion_bar_torque_qf);
    packlength = packlength + 1;
    //方向盘转角
    steer_wheel_angle = getSteerWhellAngleValue();
    dataPacket.push_back((steer_wheel_angle >> 8) & 0xFF);
    dataPacket.push_back(steer_wheel_angle & 0xFF);
    packlength = packlength + 2;
    //方向盘转角速度
    steer_wheel_rot_speed = getSteerWheelRotSpdValue();
    dataPacket.push_back((steer_wheel_rot_speed >> 8) & 0xFF);
    dataPacket.push_back(steer_wheel_rot_speed & 0xFF);
    packlength = packlength + 2;
    //方向盘角度的标定
    steer_wheel_angle_calibrated = getSteerWheelAngleCalValue();
    dataPacket.push_back(steer_wheel_angle_calibrated);
    packlength = packlength + 1;
    //方向盘转角失效
    steer_wheel_angle_failure = getSteerWheelAngleFailValue();
    dataPacket.push_back(steer_wheel_angle_failure);
    packlength = packlength + 1;
    //方向盘助力状态
    electric_power_sta = getElectricPowerStValue();
    dataPacket.push_back(electric_power_sta);
    packlength = packlength + 1;
    //验证所测扭力杆扭矩
    torsion_bar_torque_dir = getRorisionBarTorqueDirValue();
    dataPacket.push_back(torsion_bar_torque_dir);
    packlength = packlength + 1;
    //电动助力模式状态
    elec_steer_mode_sta = getElecSteerModeStValue();
    dataPacket.push_back(elec_steer_mode_sta);
    packlength = packlength + 1;
    //EPS模式设置抑制反馈
    mode_set_inhibit_fb = getEPSModeSetInhibitFBValue();
    dataPacket.push_back(mode_set_inhibit_fb);
    packlength = packlength + 1;
    //基本助力功能发生故障
    basic_elec_steer_failed = getBasicElecSteerFailValue();
    dataPacket.push_back(basic_elec_steer_failed);
    packlength = packlength + 1;
    //测量扭力杆扭矩
    torsion_bar_torque = getTorsionBarTorqueValue();
    dataPacket.push_back((torsion_bar_torque >> 8) & 0xFF);
    dataPacket.push_back(torsion_bar_torque & 0xFF);
    packlength = packlength + 2;
    //EPS助力电流信号
    eps_elect_power_consumption = getElecPowerConsumptionValue();
    dataPacket.push_back(eps_elect_power_consumption);
    packlength = packlength + 1;
    //EPS高级模式状态
    eps_advanced_mode_status = getEPSAdvancedModStValue();
    dataPacket.push_back(eps_advanced_mode_status);
    packlength = packlength + 1;
    //EPS故障等级
    eps_fault_lvl = getEPSFaultLevelValue();
    dataPacket.push_back(eps_fault_lvl);
    packlength = packlength + 1;
    //驾驶员脱手探测状态
    hod_hands_off_sts = getHodHansoffStValue();
    dataPacket.push_back(hod_hands_off_sts);
    packlength = packlength + 1;

    dataPacket[1] = (packlength >> 8) & 0xFF;
    dataPacket[2] = packlength & 0xFF;
    return dataPacket;
}