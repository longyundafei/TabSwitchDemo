#include "PowerSysInfoMessage.h"
int PowerSysInfoMessage::getCCorCPConStValue() {
    return getValue(toInt(VehicleProperty::GECKO_IPU_OBC_C_COR_CP_CNCTD_STS));
}
int PowerSysInfoMessage::getCCResisdentValue() {
    return getValue(toInt(VehicleProperty::GECKO_IPU_OBC_CC_RESISDENT));
}
int PowerSysInfoMessage::getCPDutyValue() {
    return getValue(toInt(VehicleProperty::GECKO_IPU_OBC_CP_DUTY));
}
int PowerSysInfoMessage::getCPMaxVoltValue() {
    return getValue(toInt(VehicleProperty::GECKO_IPU_OBC_CP_MAX_VOLT));
}
int PowerSysInfoMessage::getOBCProcessCodeValue() {
    return getValue(toInt(VehicleProperty::GECKO_IPU_OBC_PROCESS_CODE));
}
int PowerSysInfoMessage::getDCCLowVolActlVoltValue() {
    return getValue(toInt(VehicleProperty::GECKO_IPU_DCC_LOW_VOL_ACTL_VOL));
}
int PowerSysInfoMessage::getDCCActlOprtModeValue() {
    return getValue(toInt(VehicleProperty::GECKO_IPU_DCCACTL_OPTMODE));
}
int PowerSysInfoMessage::getDCCLowVoltActlCurValue() {
    return getValue(toInt(VehicleProperty::GECKO_IPU_DCC_LOW_VOL_ACTL_CUR));
}
int PowerSysInfoMessage::getHVILFailStValue() {
    return getValue(toInt(VehicleProperty::GECKO_IPU_HVIL_FAIL_STA));
}
int PowerSysInfoMessage::getDCDCSysFailStValue() {
    return getValue(toInt(VehicleProperty::GECKO_IPU_DCC_SYSFAILSTA));
}
int PowerSysInfoMessage::getIPUOverTempFailValue() {
    return getValue(toInt(VehicleProperty::GECKO_IPU_OVER_TEMPERATURE_FAIL));
}
int PowerSysInfoMessage::getDCCDeratOprtStValue() {
    return getValue(toInt(VehicleProperty::GECKO_IPU_DCC_DERATING_OPRT_STA));
}
int PowerSysInfoMessage::getDCCPrechrgCompltValue() {
    return getValue(toInt(VehicleProperty::GECKO_IPU_DCC_PRECHARGE_COMPLETE));
}
int PowerSysInfoMessage::getOBCConnectMainStValue() {
    return getValue(toInt(VehicleProperty::GECKO_IPU_OBC_CONNECT_MAIN_STA));
}
int PowerSysInfoMessage::getOBCHVDCActlCurValue() {
    return getValue(toInt(VehicleProperty::GECKO_IPU_OBC_HVDC_ACTL_CUR));
}
int PowerSysInfoMessage::getOBCSysFailStValue() {
    return getValue(toInt(VehicleProperty::GECKO_IPU_OBC_SYS_FAIL_STA));
}
int PowerSysInfoMessage::getOBCInletConnStValue() {
    return getValue(toInt(VehicleProperty::GECKO_IPU_OBC_INLET_CONNECT_STA));
}
int PowerSysInfoMessage::getOBCReqValue() {
    return getValue(toInt(VehicleProperty::GECKO_IPU_OBC_REQUEST));
}
int PowerSysInfoMessage::getOBCHVDCActlVoltValue() {
    return getValue(toInt(VehicleProperty::GECKO_IPU_OBC_HVDC_ACTL_VOL));
}
int PowerSysInfoMessage::getOBCActlOprtModeValue() {
    return getValue(toInt(VehicleProperty::GECKO_IPU_OBC_ACTL_OPRT_MODE));
}
int PowerSysInfoMessage::getOBCInletActlPowerValue() {
    return getValue(toInt(VehicleProperty::GECKO_IPU_OBC_INLELTACTLPOWER));
}
int PowerSysInfoMessage::getDCDCHVActlCurValue() {
    return getValue(toInt(VehicleProperty::GECKO_IPU_DCC_HIGH_VOLT_ACTL_CUR));
}
int PowerSysInfoMessage::getDCDCHVActlVoltValue() {
    return getValue(toInt(VehicleProperty::GECKO_IPU_DCC_HIGH_VOLT_ACTL_VOL));
}
int PowerSysInfoMessage::getDevInternalTempValue() {
    return getValue(toInt(VehicleProperty::GECKO_IPU_DEVICE_INT_TEMP));
}
int PowerSysInfoMessage::getOBCTempInletValue() {
    return getValue(toInt(VehicleProperty::GECKO_IPU_OBC_TEMP_INLET));
}
int PowerSysInfoMessage::getS2StValue() {
    return getValue(toInt(VehicleProperty::GECKO_IPU_OBC_S2_STATE));
}
int PowerSysInfoMessage::getOBCWakeupModeValue() {
    return getValue(toInt(VehicleProperty::GECKO_IPU_OBC_WAKE_UP_MODE));
}
int PowerSysInfoMessage::getOBCInletElecLockValue() {
    return getValue(toInt(VehicleProperty::GECKO_IPU_OBC_INLET_ELECTRONIC_LOCK));
}
int PowerSysInfoMessage::getOBCACMaxPwAllowValue() {
    return getValue(toInt(VehicleProperty::GECKO_IPU_OBC_AC_MAX_PW_ALLOW));
}
int PowerSysInfoMessage::getVoltOkl3Value() {
    return getValue(toInt(VehicleProperty::GECKO_IPU_OBC_VOLTAGE_OK_L3));
}
int PowerSysInfoMessage::getVoltOkl2Value() {
    return getValue(toInt(VehicleProperty::GECKO_IPU_OBC_VOLTAGE_OK_L2));
}
int PowerSysInfoMessage::getVoltOkl1Value() {
    return getValue(toInt(VehicleProperty::GECKO_IPU_OBC_VOLTAGE_OK_L1));
}
int PowerSysInfoMessage::getUACActl1Value() {
    return getValue(toInt(VehicleProperty::GECKO_IPU_OBC_UAC_ACT_L1));
}
int PowerSysInfoMessage::getUACActl2Value() {
    return getValue(toInt(VehicleProperty::GECKO_IPU_OBC_UAC_ACT_L2));
}
int PowerSysInfoMessage::getUACActl3Value() {
    return getValue(toInt(VehicleProperty::GECKO_IPU_OBC_UAC_ACT_L3));
}
int PowerSysInfoMessage::getIACActl1Value() {
    return getValue(toInt(VehicleProperty::GECKO_IPU_OBC_IAC_ACT_L1));
}
int PowerSysInfoMessage::getIACActl2Value() {
    return getValue(toInt(VehicleProperty::GECKO_IPU_OBC_IAC_ACT_L2));
}
int PowerSysInfoMessage::getIACActl3Value() {
    return getValue(toInt(VehicleProperty::GECKO_IPU_OBC_IAC_ACT_L3));
}
std::vector<uint8_t> PowerSysInfoMessage::generateDataPacket() {
    std::vector<uint8_t> dataPacket;
    dataPacket.push_back(PowerSysInfoId);
    uint16_t packlength = 0;
    dataPacket.push_back((packlength >> 8) & 0xFF);
    dataPacket.push_back(packlength & 0xFF);
    packlength = 0;
    //CCorCP连接状态
    cc_or_cp_cnctd_sts = getCCorCPConStValue();
    dataPacket.push_back(cc_or_cp_cnctd_sts);
    packlength = packlength + 1;
    //CC电阻值
    cc_resisdent = getCCResisdentValue();
    dataPacket.push_back((cc_resisdent >> 8) & 0xFF);
    dataPacket.push_back(cc_resisdent & 0xFF);
    packlength = packlength + 2;
    //CP信号占空比
    cp_duty = getCPDutyValue();
    dataPacket.push_back(cp_duty);
    packlength = packlength + 1;
    //CP信号幅值电压
    cp_max_volt = getCPMaxVoltValue();
    dataPacket.push_back(cp_max_volt);
    packlength = packlength + 1;
    //OBC流程码
    obc_process_code = getOBCProcessCodeValue();
    dataPacket.push_back(obc_process_code);
    packlength = packlength + 1;
    //DCC低压端实际电压
    dcc_low_vol_actl_vol = getDCCLowVolActlVoltValue();
    dataPacket.push_back(dcc_low_vol_actl_vol);
    packlength = packlength + 1;
    //DCC实际运行模式
    dcc_actl_oprt_mode = getDCCActlOprtModeValue();
    dataPacket.push_back(dcc_actl_oprt_mode);
    packlength = packlength + 1;
    //DCC低压端实际电流
    dcc_low_vol_actl_cur = getDCCLowVoltActlCurValue();
    dataPacket.push_back((dcc_low_vol_actl_cur >> 8) & 0xFF);
    dataPacket.push_back(dcc_low_vol_actl_cur & 0xFF);
    packlength = packlength + 2;
    //高压互锁系统故障
    hvil_fail_sta = getHVILFailStValue();
    dataPacket.push_back(hvil_fail_sta);
    packlength = packlength + 1;
    //DCDC系统失效状态
    dcdc_sys_fail_sta = getDCDCSysFailStValue();
    dataPacket.push_back(dcdc_sys_fail_sta);
    packlength = packlength + 1;
    //IPU过温故障
    ipu_over_temperature_fail = getIPUOverTempFailValue();
    dataPacket.push_back(ipu_over_temperature_fail);
    packlength = packlength + 1;
    //DCC降级运行模式
    dcc_derating_oprt_sta = getDCCDeratOprtStValue();
    dataPacket.push_back(dcc_derating_oprt_sta);
    packlength = packlength + 1;
    //DCC预充完成标志位
    dcc_precharge_complete = getDCCPrechrgCompltValue();
    dataPacket.push_back(dcc_precharge_complete);
    packlength = packlength + 1;
    //OBC直流侧高压回路连接状态
    obc_connect_main_sta = getOBCConnectMainStValue();
    dataPacket.push_back(obc_connect_main_sta);
    packlength = packlength + 1;
    //OBC充放电工作时，HVDC侧电流
    obc_hvdc_actl_cur = getOBCHVDCActlCurValue();
    dataPacket.push_back(obc_hvdc_actl_cur);
    packlength = packlength + 1;
    //交流充电系统失效状态
    obc_sys_fail_sta = getOBCSysFailStValue();
    dataPacket.push_back(obc_sys_fail_sta);
    packlength = packlength + 1;
    //交流充电枪连接状态
    obc_inlet_connct_sta = getOBCInletConnStValue();
    dataPacket.push_back(obc_inlet_connct_sta);
    packlength = packlength + 1;
    //交流充电机根据充放电模式，请求充电或者放电
    obc_request = getOBCReqValue();
    dataPacket.push_back(obc_request);
    packlength = packlength + 1;
    //OBC充放电工作时，HVDC侧电压
    obc_hvdc_actl_vol = getOBCHVDCActlVoltValue();
    dataPacket.push_back((obc_hvdc_actl_vol >> 8) & 0xFF);
    dataPacket.push_back(obc_hvdc_actl_vol & 0xFF);
    packlength = packlength + 2;
    //交流充电系统实际运行模式
    obc_actl_oprt_mode = getOBCActlOprtModeValue();
    dataPacket.push_back(obc_actl_oprt_mode);
    packlength = packlength + 1;
    //枪实际功率
    obc_inlelt_actl_power = getOBCInletActlPowerValue();
    dataPacket.push_back((obc_inlelt_actl_power >> 8) & 0xFF);
    dataPacket.push_back(obc_inlelt_actl_power & 0xFF);
    packlength = packlength + 2;
    //DCDC高压侧实际电流
    dcdc_high_volt_actl_cur = getDCDCHVActlCurValue();
    dataPacket.push_back((dcdc_high_volt_actl_cur >> 8) & 0xFF);
    dataPacket.push_back(dcdc_high_volt_actl_cur & 0xFF);
    packlength = packlength + 2;
    //DCDC高压侧实际电压
    dcdc_high_volt_actl_vol = getDCDCHVActlVoltValue();
    dataPacket.push_back((dcdc_high_volt_actl_vol >> 8) & 0xFF);
    dataPacket.push_back(dcdc_high_volt_actl_vol & 0xFF);
    packlength = packlength + 2;
    //设备内部温度
    device_int_temp = getDevInternalTempValue();
    dataPacket.push_back(device_int_temp);
    packlength = packlength + 1;
    //充电枪温度
    obc_temp_inlet = getOBCTempInletValue();
    dataPacket.push_back(obc_temp_inlet);
    packlength = packlength + 1;
    //GB S2开关状态
    s2_state = getS2StValue();
    dataPacket.push_back(s2_state);
    packlength = packlength + 1;
    //OBC唤醒VCU的模式
    obc_wake_up_mode = getOBCWakeupModeValue();
    dataPacket.push_back(obc_wake_up_mode);
    packlength = packlength + 1;
    //充电枪电子锁状态
    obc_inlet_electronic_lock = getOBCInletElecLockValue();
    dataPacket.push_back(obc_inlet_electronic_lock);
    packlength = packlength + 1;
    //充放电中，OBC允许最大充放电功率
    obc_ac_max_pw_allow = getOBCACMaxPwAllowValue();
    dataPacket.push_back((obc_ac_max_pw_allow >> 8) & 0xFF);
    dataPacket.push_back(obc_ac_max_pw_allow & 0xFF);
    packlength = packlength + 2;
    //充电枪L3电压状态
    voltage_ok_l3 = getVoltOkl3Value();
    dataPacket.push_back(voltage_ok_l3);
    packlength = packlength + 1;
    //充电枪L2电压状态
    voltage_ok_l2 = getVoltOkl2Value();
    dataPacket.push_back(voltage_ok_l2);
    packlength = packlength + 1;
    //充电枪L1电压状态
    voltage_ok_l1 = getVoltOkl1Value();
    dataPacket.push_back(voltage_ok_l1);
    packlength = packlength + 1;
    //L1相电压
    uac_act_l1 = getUACActl1Value();
    dataPacket.push_back((uac_act_l1 >> 8) & 0xFF);
    dataPacket.push_back(uac_act_l1 & 0xFF);
    packlength = packlength + 2;
    //L2相电压
    uac_act_l2 = getUACActl2Value();
    dataPacket.push_back((uac_act_l2 >> 8) & 0xFF);
    dataPacket.push_back(uac_act_l2 & 0xFF);
    packlength = packlength + 2;
    //L3相电压
    uac_act_l3 = getUACActl3Value();
    dataPacket.push_back((uac_act_l3 >> 8) & 0xFF);
    dataPacket.push_back(uac_act_l3 & 0xFF);
    packlength = packlength + 2;
    //L1相电流
    iac_act_l1 = getIACActl1Value();
    dataPacket.push_back((iac_act_l1 >> 8) & 0xFF);
    dataPacket.push_back(iac_act_l1 & 0xFF);
    packlength = packlength + 2;
    //L2相电流
    iac_act_l2 = getIACActl2Value();
    dataPacket.push_back((iac_act_l2 >> 8) & 0xFF);
    dataPacket.push_back(iac_act_l2 & 0xFF);
    packlength = packlength + 2;
    //L3相电流
    iac_act_l3 = getIACActl3Value();
    dataPacket.push_back((iac_act_l3 >> 8) & 0xFF);
    dataPacket.push_back(iac_act_l3 & 0xFF);
    packlength = packlength + 2;

    dataPacket[1] = (packlength >> 8) & 0xFF;
    dataPacket[2] = packlength & 0xFF;
    return dataPacket;
}