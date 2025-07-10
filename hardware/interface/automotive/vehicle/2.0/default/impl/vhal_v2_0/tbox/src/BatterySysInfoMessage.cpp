#include "BatterySysInfoMessage.h"
int BatterySysInfoMessage::getBMSThermalRunawayWrnValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_THERMALRUNAWAYWRN));
}
int BatterySysInfoMessage::getInsulationWrningValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_INSULATION_WRNING_DISTGSH));
}
int BatterySysInfoMessage::getBMSSupplyVoltStValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_SUPPLY_VOLTAGE_STATUS));
}
int BatterySysInfoMessage::getPreChrgContactorStValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_CONTACTOR_STATE_PRECHARGE));
}
int BatterySysInfoMessage::getPositiveContactorStValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_CONTACTOR_STATE_POSITIVE));
}
int BatterySysInfoMessage::getNegativeContactorStValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_CONTACTOR_STATE_NEGATIVE));
}
int BatterySysInfoMessage::getBMSRunStValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_RUN_STATE));
}
int BatterySysInfoMessage::getDischrgInterlockStValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_DIS_CHARGE_INTERLOCK_STA));
}
int BatterySysInfoMessage::getChrgInterlockStValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_CHARGE_INTERLOCK_STA));
}
int BatterySysInfoMessage::getBMSEmeShutdownReqValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_EMESHUTDOWN_REQ));
}
int BatterySysInfoMessage::getPwrTrainConnectStValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_PWR_TRAIN_CONNECT_STA));
}
int BatterySysInfoMessage::getBMSThermRunawayVlvStValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_THERM_RUNAWAY_VLV_ST));
}
int BatterySysInfoMessage::getBMSThermRunawaySnsrFltValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_THERM_RUNAWAY_SNSR_FLT));
}
int BatterySysInfoMessage::getBatBalanceStValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_BATT_BALANCE_STA));
}
int BatterySysInfoMessage::getBMSSysFailStValue() {
    return getValue(toInt(VehicleProperty::GECKO_VCU_HV_BATT_ERR));
}
int BatterySysInfoMessage::getBMSBatSocRptValue() {
    return getValue(toInt(VehicleProperty::EV_BATTERY_LEVEL));
}
int BatterySysInfoMessage::getBMSBatSohValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_BAT_SOH));
}
int BatterySysInfoMessage::getTempMaxDCChrgSocketValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_TEMP_MAX_DC_CHARGE_SOCKET));
}
int BatterySysInfoMessage::getBMSPCBMaxTempValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_PCB_TEMP_MAX));
}
int BatterySysInfoMessage::getMaxAllowChrgCurDCValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_MAX_ALLOW_CHRG_CUR_DC));
}
int BatterySysInfoMessage::getElectronicLockCtrlValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_ELECTRONIC_LOCK_CTRL));
}
int BatterySysInfoMessage::getMaxAllowChrgVolDCValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_MAX_ALLOW_CHRG_VOL_DC));
}
int BatterySysInfoMessage::getDCSChrgAplusValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_DCS_CHRG_A_PLUS));
}
int BatterySysInfoMessage::getDCSChrgReqValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_DCS_CHARGE_REQUEST));
}
int BatterySysInfoMessage::getConnectorStDcChrgValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_CONTACTOR_STATE_DC_CHARGER));
}
int BatterySysInfoMessage::getElectronicLockStValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_ELECTRONIC_LOCK_STATE));
}
int BatterySysInfoMessage::getBMSACChrgAtclStValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_AC_CHRG_ATCL_STA));
}
int BatterySysInfoMessage::getBMSOutletCoolantActlTempValue() {
    int valide = getValue(toInt(VehicleProperty::GECKO_BMS_OUTLET_COOLANT_ACTL_TEMP_VALID));
    if(valide == 0x01){
        return getValue(toInt(VehicleProperty::GECKO_BMS_OUTLET_COOLANT_ACTL_TEMP));
    } else {
        return 0xFF;
    }
}
int BatterySysInfoMessage::getBMSInletCoolantActlTempValue() {
    int valide = getValue(toInt(VehicleProperty::GECKO_BMS_INLET_COOLANT_ACTL_TEMP_VALID));
    if(valide == 0x01){
        return getValue(toInt(VehicleProperty::GECKO_BMS_INLET_COOLANT_ACTL_TEMP));
    } else {
        return 0xFF;
    }
}
int BatterySysInfoMessage::getBMSISOResistanceValue() {
    return getRawPropValue(toInt(VehicleProperty::GECKO_BMS_ISORESISTANCE));
}
int BatterySysInfoMessage::getMaxChrgVoltACValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_MAX_CHRG_VOLTAGE_AC));
}
int BatterySysInfoMessage::getMaxACChrgCurrentValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_MAX_CHRG_CURRENT_AC));
}
int BatterySysInfoMessage::getDCSInletConnectStValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_DCS_INLET_CONNECT_STA));
}
int BatterySysInfoMessage::getRemChrgTimeValue() {
    return getValue(toInt(VehicleProperty::GECKO_EV_CHARGE_TIME_REMAINING));
}
int BatterySysInfoMessage::getDCSActlChrgCurrentValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_DCS_ACTL_CHRG_CURRENT));
}
int BatterySysInfoMessage::getDCSActOprtModeValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_DCS_ACT_OPRT_MODE));
}
int BatterySysInfoMessage::getDCSActlChrgVoltValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_DCS_ACTLCHRGVOLTAGE));
}
int BatterySysInfoMessage::getDCSActlChrgPwrValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_DCS_ACTL_CHRG_POWER));
}
int BatterySysInfoMessage::getHVBatCellTempAveValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_HV_BAT_CELL_TEMP_AVE));
}
int BatterySysInfoMessage::getMaxConDisCurntValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_MAX_CON_DIS_CURNT));
}
int BatterySysInfoMessage::getPwrRecupMaxConChrgCurntValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_PWR_RECUP_MAX_CON_CHRG_CURNT));
}
int BatterySysInfoMessage::getMaxInstanDisCurntValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_MAX_INSTAN_DIS_CURNT));
}
int BatterySysInfoMessage::getPwrRecupMaxInstanChrgCurntValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_PWR_RECUP_MAX_INSTAN_CHRG_CURNT));
}
int BatterySysInfoMessage::getCellVoltAveValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_CELL_VOL_AVE));
}
int BatterySysInfoMessage::getNegativeContactorTempValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_CONTACTOR_TEMP_NEGATIVE));
}
int BatterySysInfoMessage::getMaxCellTempLimitValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_MAX_CELL_TEMP_LIMIT));
}
int BatterySysInfoMessage::getMaxCellVolLimitValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_MAX_CELL_VOL_LIMIT));
}
int BatterySysInfoMessage::getMinCellTempLimitValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_MIN_CELL_TEMP_LIMIT));
}
int BatterySysInfoMessage::getPwrLimdFlgValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_PWR_LIMD_FLG));
}
int BatterySysInfoMessage::getMinCellVolLimitValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_MIN_CELL_VOL_LIMIT));
}
int BatterySysInfoMessage::getBatteryTypeValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_BATTERY_TYPE));
}
int BatterySysInfoMessage::getBMSHWVerValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_HW_VERSION));
}
int BatterySysInfoMessage::getBMSPackIDValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_PACK_IDENTITY));
}
int BatterySysInfoMessage::getBMSSWVerValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_SW_VERSION));
}
int BatterySysInfoMessage::getBatCapacityValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_BAT_CAPACITY));
}
int BatterySysInfoMessage::getNomCellCapValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_NOM_CELL_CAP));
}
int BatterySysInfoMessage::getChrgTarSOCValue() {
    return getValue(toInt(VehicleProperty::GECKO_CCM_CHRAGING_TARGET_SOC));
}
std::vector<uint8_t> BatterySysInfoMessage::generateDataPacket() {
    std::vector<uint8_t> dataPacket;
    dataPacket.push_back(BatterySysInfoId);
    uint16_t packlength = 0;
    dataPacket.push_back((packlength >> 8) & 0xFF);
    dataPacket.push_back(packlength & 0xFF);
    packlength = 0;
    //电池包热失控报警信号
    bms_ThermalrunawayWrn = getBMSThermalRunawayWrnValue();
    dataPacket.push_back(bms_ThermalrunawayWrn);
    packlength = packlength + 1;
    //绝缘故障报警
    insulation_wrning = getInsulationWrningValue();
    dataPacket.push_back(insulation_wrning);
    packlength = packlength + 1;
    //BMS供电电压状态
    bms_supply_voltage_status = getBMSSupplyVoltStValue();
    dataPacket.push_back(bms_supply_voltage_status);
    packlength = packlength + 1;
    //预充继电器状态
    contactor_state_precharge = getPreChrgContactorStValue();
    dataPacket.push_back(contactor_state_precharge);
    packlength = packlength + 1;
    //主正继电器状态
    contactor_state_positive = getPositiveContactorStValue();
    dataPacket.push_back(contactor_state_positive);
    packlength = packlength + 1;
    //主负继电器状态
    contactor_state_negative = getNegativeContactorStValue();
    dataPacket.push_back(contactor_state_negative);
    packlength = packlength + 1;
    //电池管理系统运行状态
    bms_run_state = getBMSRunStValue();
    dataPacket.push_back(bms_run_state);
    packlength = packlength + 1;
    //放电高压互锁状态
    discharge_interlock_sta = getDischrgInterlockStValue();
    dataPacket.push_back(discharge_interlock_sta);
    packlength = packlength + 1;
    //充电高压互锁状态
    charge_interlock_sta = getChrgInterlockStValue();
    dataPacket.push_back(charge_interlock_sta);
    packlength = packlength + 1;
    //BMS紧急下电请求
    bms_emeshutdown_req = getBMSEmeShutdownReqValue();
    dataPacket.push_back(bms_emeshutdown_req);
    packlength = packlength + 1;
    //动力总成连接状态
    pwr_train_connect_sta = getPwrTrainConnectStValue();
    dataPacket.push_back(pwr_train_connect_sta);
    packlength = packlength + 1;
    //BMS热失控防爆阀状态
    bms_therm_runaway_vlv_st = getBMSThermRunawayVlvStValue();
    dataPacket.push_back(bms_therm_runaway_vlv_st);
    packlength = packlength + 1;
    //热失控传感器故障
    bms_therm_runaway_snsr_flt = getBMSThermRunawaySnsrFltValue();
    dataPacket.push_back(bms_therm_runaway_snsr_flt);
    packlength = packlength + 1;
    //电池包均衡状态
    batt_balance_sta = getBatBalanceStValue();
    dataPacket.push_back(batt_balance_sta);
    packlength = packlength + 1;
    //BMS失效状态
    bms_sys_fail_sta = getBMSSysFailStValue();
    dataPacket.push_back(bms_sys_fail_sta);
    packlength = packlength + 1;
    //BMS故障码索引
    for (auto i = 0; i < 20; i++){
        dataPacket.push_back((bms_err_tab_index[i] >> 8) & 0xFF);
        dataPacket.push_back(bms_err_tab_index[i] & 0xFF);
    }
    packlength = packlength + 40;
    //SOC_Rpt_电池表显剩余电量
    bms_bat_soc_rpt = getBMSBatSocRptValue();
    dataPacket.push_back((bms_bat_soc_rpt >> 8) & 0xFF);
    dataPacket.push_back(bms_bat_soc_rpt & 0xFF);
    packlength = packlength + 2;
    //SOH_高压电池健康状态
    bms_bat_soh = getBMSBatSohValue();
    dataPacket.push_back((bms_bat_soh >> 8) & 0xFF);
    dataPacket.push_back(bms_bat_soh & 0xFF);
    packlength = packlength + 2;
    //直流充电座最高温度
    temp_max_dc_charge_socket = getTempMaxDCChrgSocketValue();
    dataPacket.push_back(temp_max_dc_charge_socket);
    packlength = packlength + 1;
    //BMS板载最高温度
    bms_pcb_temp_max = getBMSPCBMaxTempValue();
    dataPacket.push_back(bms_pcb_temp_max);
    packlength = packlength + 1;
    //直流充电最大允许充电电流
    max_allow_chrg_cur_dc = getMaxAllowChrgCurDCValue();
    dataPacket.push_back((max_allow_chrg_cur_dc >> 8) & 0xFF);
    dataPacket.push_back(max_allow_chrg_cur_dc & 0xFF);
    packlength = packlength + 2;
    //电子锁开关控制
    electronic_lock_ctrl = getElectronicLockCtrlValue();
    dataPacket.push_back(electronic_lock_ctrl);
    packlength = packlength + 1;
    //直流充电最大允许充电电压
    max_allow_chrg_vol_dc = getMaxAllowChrgVolDCValue();
    dataPacket.push_back((max_allow_chrg_vol_dc >> 8) & 0xFF);
    dataPacket.push_back(max_allow_chrg_vol_dc & 0xFF);
    packlength = packlength + 2;
    //充电桩A+辅助信号
    dcs_chrg_a_plus = getDCSChrgAplusValue();
    dataPacket.push_back(dcs_chrg_a_plus);
    packlength = packlength + 1;
    //直流充电请求
    dcs_charge_request = getDCSChrgReqValue();
    dataPacket.push_back(dcs_charge_request);
    packlength = packlength + 1;
    //直流充电继电器状态
    contactor_state_dc_charger = getConnectorStDcChrgValue();
    dataPacket.push_back(contactor_state_dc_charger);
    packlength = packlength + 1;
    //电子锁开关状态
    electronic_lock_state = getElectronicLockStValue();
    dataPacket.push_back(electronic_lock_state);
    packlength = packlength + 1;
    //BMS交流充电实际状态
    bms_ac_chrg_atcl_sta = getBMSACChrgAtclStValue();
    dataPacket.push_back(bms_ac_chrg_atcl_sta);
    packlength = packlength + 1;
    //电池实际出水口温度
    bms_outlet_coolant_actl_temp = getBMSOutletCoolantActlTempValue();
    dataPacket.push_back(bms_outlet_coolant_actl_temp);
    packlength = packlength + 1;
    //电池实际入水口温度
    bms_inlet_coolant_actl_temp = getBMSInletCoolantActlTempValue();
    dataPacket.push_back(bms_inlet_coolant_actl_temp);
    packlength = packlength + 1;
    //高压电池对地绝缘阻值
    bms_iso_resistance = getBMSISOResistanceValue();
    dataPacket.push_back((bms_iso_resistance >> 8) & 0xFF);
    dataPacket.push_back(bms_iso_resistance & 0xFF);
    packlength = packlength + 2;
    //交流充电最大允许充电电压
    max_chrg_voltage_ac = getMaxChrgVoltACValue();
    dataPacket.push_back((max_chrg_voltage_ac >> 8) & 0xFF);
    dataPacket.push_back(max_chrg_voltage_ac & 0xFF);
    packlength = packlength + 2;
    //交流充电最大允许充电电流
    max_chrg_current_ac = getMaxACChrgCurrentValue();
    dataPacket.push_back((max_chrg_current_ac >> 8) & 0xFF);
    dataPacket.push_back(max_chrg_current_ac & 0xFF);
    packlength = packlength + 2;
    //直流枪插抢连接状态
    dcs_inlet_connect_sta = getDCSInletConnectStValue();
    dataPacket.push_back(dcs_inlet_connect_sta);
    packlength = packlength + 1;
    //剩余充电时间
    rem_chrg_time = getRemChrgTimeValue();
    dataPacket.push_back((rem_chrg_time >> 8) & 0xFF);
    dataPacket.push_back(rem_chrg_time & 0xFF);
    packlength = packlength + 2;
    //直流充电桩实际输出电流
    dcs_actl_chrg_current = getDCSActlChrgCurrentValue();
    dataPacket.push_back((dcs_actl_chrg_current >> 8) & 0xFF);
    dataPacket.push_back(dcs_actl_chrg_current & 0xFF);
    packlength = packlength + 2;
    //直流充电实际状态
    dcs_act_oprt_mode = getDCSActOprtModeValue();
    dataPacket.push_back(dcs_act_oprt_mode);
    packlength = packlength + 1;
    //直流充电桩实际输出电压
    dcs_actl_chrg_voltage = getDCSActlChrgVoltValue();
    dataPacket.push_back((dcs_actl_chrg_voltage >> 8) & 0xFF);
    dataPacket.push_back(dcs_actl_chrg_voltage & 0xFF);
    packlength = packlength + 2;
    //桩实际输入功率
    dcs_actl_chrg_power = getDCSActlChrgPwrValue();
    dataPacket.push_back((dcs_actl_chrg_power >> 8) & 0xFF);
    dataPacket.push_back(dcs_actl_chrg_power & 0xFF);
    packlength = packlength + 2;
    //平均电芯温度
    hv_bat_cell_temp_ave = getHVBatCellTempAveValue();
    dataPacket.push_back(hv_bat_cell_temp_ave);
    packlength = packlength + 1;
    //最大持续放电电流
    max_con_dis_curnt = getMaxConDisCurntValue();
    dataPacket.push_back((max_con_dis_curnt >> 8) & 0xFF);
    dataPacket.push_back(max_con_dis_curnt & 0xFF);
    packlength = packlength + 2;
    //能量回收最大持续充电电流
    pwr_recup_max_con_chrg_curnt = getPwrRecupMaxConChrgCurntValue();
    dataPacket.push_back((pwr_recup_max_con_chrg_curnt >> 8) & 0xFF);
    dataPacket.push_back(pwr_recup_max_con_chrg_curnt & 0xFF);
    packlength = packlength + 2;
    //电池最大瞬时放电电流
    max_instan_dis_curnt = getMaxInstanDisCurntValue();
    dataPacket.push_back((max_instan_dis_curnt >> 8) & 0xFF);
    dataPacket.push_back(max_instan_dis_curnt & 0xFF);
    packlength = packlength + 2;
    //能量回收最大瞬时充电电流
    pwr_recup_max_instan_chrg_curnt = getPwrRecupMaxInstanChrgCurntValue();
    dataPacket.push_back((pwr_recup_max_instan_chrg_curnt >> 8) & 0xFF);
    dataPacket.push_back(pwr_recup_max_instan_chrg_curnt & 0xFF);
    packlength = packlength + 2;
    //所有电池包平均单体电压
    cell_vol_ave = getCellVoltAveValue();
    dataPacket.push_back((cell_vol_ave >> 8) & 0xFF);
    dataPacket.push_back(cell_vol_ave & 0xFF);
    //dataPacket.push_back(cell_vol_ave);
    packlength = packlength + 2;
    //主负继电器铜牌温度
    contactor_temp_negative = getNegativeContactorTempValue();
    dataPacket.push_back(contactor_temp_negative);
    packlength = packlength + 1;
    //最高电芯温度限制值
    max_cell_temp_limit = getMaxCellTempLimitValue();
    dataPacket.push_back(max_cell_temp_limit);
    packlength = packlength + 1;
    //最高单体电压限制值
    max_cell_vol_limit = getMaxCellVolLimitValue();
    dataPacket.push_back((max_cell_vol_limit >> 8) & 0xFF);
    dataPacket.push_back(max_cell_vol_limit & 0xFF);
    //dataPacket.push_back(max_cell_vol_limit);
    packlength = packlength + 2;
    //最低电芯温度限制值
    min_cell_temp_limit = getMinCellTempLimitValue();
    dataPacket.push_back(min_cell_temp_limit);
    packlength = packlength + 1;
    //充电桩功率小于车辆允许功率标志位
    pwr_limd_flg = getPwrLimdFlgValue();
    dataPacket.push_back(pwr_limd_flg);
    packlength = packlength + 1;
    //最低单体电压限制值
    min_cell_vol_limit = getMinCellVolLimitValue();
    dataPacket.push_back((min_cell_vol_limit >> 8) & 0xFF);
    dataPacket.push_back(min_cell_vol_limit & 0xFF);
    packlength = packlength + 2;
    //电池类型
    battery_type =getBatteryTypeValue();
    dataPacket.push_back(battery_type);
    packlength = packlength + 1;
    //BMS硬件版本
    bms_hw_ver = getBMSHWVerValue();
    dataPacket.push_back(bms_hw_ver);
    packlength = packlength + 1;
    //电池包识别标识
    bms_pack_id = getBMSPackIDValue();
    dataPacket.push_back(bms_pack_id);
    packlength = packlength + 1;
    //BMS软件版本
    bms_sw_ver = getBMSSWVerValue();
    dataPacket.push_back(bms_sw_ver);
    packlength = packlength + 1;
    //高压电池容量
    bat_capacity = getBatCapacityValue();
    dataPacket.push_back(bat_capacity);
    packlength = packlength + 1;
    //名义单体容量
    nom_cell_cap = getNomCellCapValue();
    dataPacket.push_back(nom_cell_cap);
    packlength = packlength + 1;
    //充电目标SOC反馈
    chrg_target_soc = getChrgTarSOCValue();
    dataPacket.push_back(chrg_target_soc);
    packlength = packlength + 1;

    dataPacket[1] = (packlength >> 8) & 0xFF;
    dataPacket[2] = packlength & 0xFF;
    return dataPacket;
}