#include "BatteryChargingInfoMessage.h"
int BatteryChargingInfoMessage::getSPN256000TimeoutValue() {
    return getValue(toInt(VehicleProperty::GECKO_BEM_SPN2560_00));
}
int BatteryChargingInfoMessage::getSPN2560AATimeoutValue() {
    return getValue(toInt(VehicleProperty::GECKO_BEM_SPN2560_AA));
}
int BatteryChargingInfoMessage::getSPN3903CMLCTSTimeoutValue() {
    return getValue(toInt(VehicleProperty::GECKO_BEM_SPN3903_CML_CTS));
}
int BatteryChargingInfoMessage::getSPN3904CROTimeoutValue() {
    return getValue(toInt(VehicleProperty::GECKO_BEM_SPN3904_CRO));
}
int BatteryChargingInfoMessage::getSPN3905CCSTimeoutValue() {
    return getValue(toInt(VehicleProperty::GECKO_BEM_SPN3905_CCS));
}
int BatteryChargingInfoMessage::getSPN3906CSTTimeoutValue() {
    return getValue(toInt(VehicleProperty::GECKO_BEM_SPN3906_CST));
}
int BatteryChargingInfoMessage::getSPN3907CSTTimeoutValue() {
    return getValue(toInt(VehicleProperty::GECKO_BEM_SPN3907_CSD));
}
int BatteryChargingInfoMessage::getBMSIdentifyTimeoutValue() {
    return getValue(toInt(VehicleProperty::GECKO_CEM_BMS_IDENTIFY_TIMOUT));
}
int BatteryChargingInfoMessage::getBatteryParamTimeoutValue() {
    return getValue(toInt(VehicleProperty::GECKO_CEM_BATTERY_PARAMETER_TIMOUT));
}
int BatteryChargingInfoMessage::getBMSFinishChrgTimeoutValue() {
    return getValue(toInt(VehicleProperty::GECKO_CEM_BMS_FINISH_CHARGE_TIMEOUT));
}
int BatteryChargingInfoMessage::getBattStTimeoutValue() {
    return getValue(toInt(VehicleProperty::GECKO_CEM_BATTERY_STATE_TIMEOUT));
}
int BatteryChargingInfoMessage::getBattChrgReqTimeoutValue() {
    return getValue(toInt(VehicleProperty::GECKO_CEM_BATTERY_CHARGE_REQUIRE_TIMEOUT));
}
int BatteryChargingInfoMessage::getBMSAbortChrgTimeoutValue() {
    return getValue(toInt(VehicleProperty::GECKO_CEM_BMS_ABORT_CHARGE_TIMEOUT));
}
int BatteryChargingInfoMessage::getBMSChrgStatisticsTimeoutValue() {
    return getValue(toInt(VehicleProperty::GECKO_CEM_BMS_CHARGE_STATISTICS_TIMEOUT));
}
int BatteryChargingInfoMessage::getAbortAchiveSOCValue() {
    return getValue(toInt(VehicleProperty::GECKO_BST_ABORT_ACHIVE_SOC));
}
int BatteryChargingInfoMessage::getAbortAchiveTotalVoltValue() {
    return getValue(toInt(VehicleProperty::GECKO_BST_ABORT_ACHIVE_TOTAL_VOLT));
}
int BatteryChargingInfoMessage::getAbortAchiveSingleVoltValue() {
    return getValue(toInt(VehicleProperty::GECKO_BST_ABORT_ACHIVE_SINGLE_VOLT));
}
int BatteryChargingInfoMessage::getAbortAchiveDCSValue() {
    return getValue(toInt(VehicleProperty::GECKO_BST_ABORT_ACHIVE_DCS));
}
int BatteryChargingInfoMessage::getInsulationFaultValue() {
    return getValue(toInt(VehicleProperty::GECKO_BST_ABORT_FAULT_INSULATION));
}
int BatteryChargingInfoMessage::getConnectorTempFaultValue() {
    return getValue(toInt(VehicleProperty::GECKO_BST_ABORT_FAULT_CONNECTOR_TEMP));
}
int BatteryChargingInfoMessage::getBSTAbortFaultBMSTempValue() {
    return getValue(toInt(VehicleProperty::GECKO_BST_ABORT_FAULT_BMS_TEMP));
}
int BatteryChargingInfoMessage::getBSTConnectorFaultValue() {
    return getValue(toInt(VehicleProperty::GECKO_BST_ABORT_FAULT_CONNECTOR));
}
int BatteryChargingInfoMessage::getBattTempFaultValue() {
    return getValue(toInt(VehicleProperty::GECKO_BST_ABORT_FAULT_BATTERY_TEMP));
}
int BatteryChargingInfoMessage::getHVFaultValue() {
    return getValue(toInt(VehicleProperty::GECKO_BST_ABORT_FAULT_HV));
}
int BatteryChargingInfoMessage::getVoltFaultValue() {
    return getValue(toInt(VehicleProperty::GECKO_BST_ABORT_FAULT_VOLTAGE));
}
int BatteryChargingInfoMessage::getBSTOtherFaultValue() {
    return getValue(toInt(VehicleProperty::GECKO_BST_ABORT_FAULT_OTHER));
}
int BatteryChargingInfoMessage::getCurrentErrValue() {
    return getValue(toInt(VehicleProperty::GECKO_BST_ABORT_ERROR_CURRENT));
}
int BatteryChargingInfoMessage::getBSTVoltErrValue() {
    return getValue(toInt(VehicleProperty::GECKO_BST_ABORT_ERROR_VOLTAGE));
}
int BatteryChargingInfoMessage::getAbortAchiveConditionValue() {
    return getValue(toInt(VehicleProperty::GECKO_CST_ABORT_ACHIVE_CONDITION));
}
int BatteryChargingInfoMessage::getAbortByUserValue() {
    return getValue(toInt(VehicleProperty::GECKO_CST_ABORT_BY_USER));
}
int BatteryChargingInfoMessage::getAbortByFaultValue() {
    return getValue(toInt(VehicleProperty::GECKO_CST_ABORT_BY_FAULT));
}
int BatteryChargingInfoMessage::getAbortByBMSValue() {
    return getValue(toInt(VehicleProperty::GECKO_CST_ABORT_BY_BMS));
}
int BatteryChargingInfoMessage::getChargerTempFaultValue() {
    return getValue(toInt(VehicleProperty::GECKO_CST_ABORT_FAULT_CHARGER_TEMP));
}
int BatteryChargingInfoMessage::getCSTConnectorFaultValue() {
    return getValue(toInt(VehicleProperty::GECKO_CST_ABORT_FAULT_CONNETOR));
}
int BatteryChargingInfoMessage::getChargerInnerTempFaultValue() {
    return getValue(toInt(VehicleProperty::GECKO_CST_ABORT_FAULT_CHARGER_INNER_TEMP));
}
int BatteryChargingInfoMessage::getEnergeTransmitFaultValue() {
    return getValue(toInt(VehicleProperty::GECKO_CST_ABORT_FAULT_ENERGE_TRANSMIT));
}
int BatteryChargingInfoMessage::getChargerStopFaultValue() {
    return getValue(toInt(VehicleProperty::GECKO_CST_ABORT_FAULT_CHARGER_STOP));
}
int BatteryChargingInfoMessage::getCSTOtherFaultValue() {
    return getValue(toInt(VehicleProperty::GECKO_CST_ABORT_FAULT_OTHER));
}
int BatteryChargingInfoMessage::getCurrentMismatchValue() {
    return getValue(toInt(VehicleProperty::GECKO_CST_ABORT_ERROR_CURRENT));
}
int BatteryChargingInfoMessage::getCSTVoltErrValue() {
    return getValue(toInt(VehicleProperty::GECKO_CST_ABORT_ERROR_VOLTAGE));
}
std::vector<uint8_t> BatteryChargingInfoMessage::generateDataPacket() {
    std::vector<uint8_t> dataPacket;
    dataPacket.push_back(BatteryChargingInfoId);
    uint16_t packlength = 0;
    dataPacket.push_back((packlength >> 8) & 0xFF);
    dataPacket.push_back(packlength & 0xFF);
    packlength = 0;
    //接收SPN2560=0X00的充电机辨识报文超时
    spn2560_00_timeout = getSPN256000TimeoutValue();
    dataPacket.push_back(spn2560_00_timeout);
    packlength = packlength + 1;
    //接收SPN2560=0XAA的充电机辨识报文超时
    spn2560_aa_timeout = getSPN2560AATimeoutValue();
    dataPacket.push_back(spn2560_aa_timeout);
    packlength = packlength + 1;
    //接收充电机的时间同步和充电机最大输出能力超时
    spn3903_cml_cts_timeout = getSPN3903CMLCTSTimeoutValue();
    dataPacket.push_back(spn3903_cml_cts_timeout);
    packlength = packlength + 1;
    //接收充电机完成充电准备报文超时
    spn3904_cro_timeout = getSPN3904CROTimeoutValue();
    dataPacket.push_back(spn3904_cro_timeout);
    packlength = packlength + 1;
    //接收充电机充电状态报文超时
    spn3905_ccs_timeout = getSPN3905CCSTimeoutValue();
    dataPacket.push_back(spn3905_ccs_timeout);
    packlength = packlength + 1;
    //接收充电机中止充电报文超时
    spn3906_cst_timeout = getSPN3906CSTTimeoutValue();
    dataPacket.push_back(spn3906_cst_timeout);
    packlength = packlength + 1;
    //接收充电机充电统计报文超时
    spn3907_cst_timeout = getSPN3907CSTTimeoutValue();
    dataPacket.push_back(spn3907_cst_timeout);
    packlength = packlength + 1;
    //接收BMS和车辆的辨识报文超时
    bms_identify_timeout = getBMSIdentifyTimeoutValue();
    dataPacket.push_back(bms_identify_timeout);
    packlength = packlength + 1;
    //接收电池充电参数报文超时
    battery_parameter_timeout = getBatteryParamTimeoutValue();
    dataPacket.push_back(battery_parameter_timeout);
    packlength = packlength + 1;
    //接收BMS完成充电准备报文超时
    bms_finish_charge_timeout = getBMSFinishChrgTimeoutValue();
    dataPacket.push_back(bms_finish_charge_timeout);
    packlength = packlength + 1;
    //接收电池充电总状态报文超时
    battery_state_timeout = getBattStTimeoutValue();
    dataPacket.push_back(battery_state_timeout);
    packlength = packlength + 1;
    //接收电池充电要求报文超时
    battery_charge_require_timeout = getBattChrgReqTimeoutValue();
    dataPacket.push_back(battery_charge_require_timeout);
    packlength = packlength + 1;
    //接收BMS中止充电报文超时
    bms_abort_charge_timeout = getBMSAbortChrgTimeoutValue();
    dataPacket.push_back(bms_abort_charge_timeout);
    packlength = packlength + 1;
    //接收BMS充电统计报文超时
    bms_charge_statistics_timeout = getBMSChrgStatisticsTimeoutValue();
    dataPacket.push_back(bms_charge_statistics_timeout);
    packlength = packlength + 1;
    //达到所需的SOC目标值
    abort_achive_soc = getAbortAchiveSOCValue();
    dataPacket.push_back(abort_achive_soc);
    packlength = packlength + 1;
    //达到总电压设定值
    abort_achive_total_volt = getAbortAchiveTotalVoltValue();
    dataPacket.push_back(abort_achive_total_volt);
    packlength = packlength + 1;
    //到达单体电压设定值
    abort_achive_single_volt = getAbortAchiveSingleVoltValue();
    dataPacket.push_back(abort_achive_single_volt);
    packlength = packlength + 1;
    //充电机主动中止
    abort_achive_dcs = getAbortAchiveDCSValue();
    dataPacket.push_back(abort_achive_dcs);
    packlength = packlength + 1;
    //绝缘故障
    insulation_fault = getInsulationFaultValue();
    dataPacket.push_back(insulation_fault);
    packlength = packlength + 1;
    //输出连接器过温故障
    connector_temp_fault = getConnectorTempFaultValue();
    dataPacket.push_back(connector_temp_fault);
    packlength = packlength + 1;
    //BMS元件、输出连接线过温
    bst_abort_fault_bsm_temp = getBSTAbortFaultBMSTempValue();
    dataPacket.push_back(bst_abort_fault_bsm_temp);
    packlength = packlength + 1;
    //充电连接器故障
    bst_connector_fault = getBSTConnectorFaultValue();
    dataPacket.push_back(bst_connector_fault);
    packlength = packlength + 1;
    //电池组温度过高故障
    battery_temp_fault = getBattTempFaultValue();
    dataPacket.push_back(battery_temp_fault);
    packlength = packlength + 1;
    //高压继电器故障
    hv_fault = getHVFaultValue();
    dataPacket.push_back(hv_fault);
    packlength = packlength + 1;
    //检测点2电压检测故障
    voltage_fault = getVoltFaultValue();
    dataPacket.push_back(voltage_fault);
    packlength = packlength + 1;
    //其他故障
    bst_other_fault= getBSTOtherFaultValue();
    dataPacket.push_back(bst_other_fault);
    packlength = packlength + 1;
    //电流过大
    current_error = getCurrentErrValue();
    dataPacket.push_back(current_error);
    packlength = packlength + 1;
    //电压异常
    bst_voltage_error = getBSTVoltErrValue();
    dataPacket.push_back(bst_voltage_error);
    packlength = packlength + 1;
    //到达充电机设定的条件中止
    abort_achive_condition = getAbortAchiveConditionValue();
    dataPacket.push_back(abort_achive_condition);
    packlength = packlength + 1;
    //人工中止
    abort_by_user = getAbortByUserValue();
    dataPacket.push_back(abort_by_user);
    packlength = packlength + 1;
    //故障中止
    abort_by_fault = getAbortByFaultValue();
    dataPacket.push_back(abort_by_fault);
    packlength = packlength + 1;
    //BMS主动中止
    abort_by_bms = getAbortByBMSValue();
    dataPacket.push_back(abort_by_bms);
    packlength = packlength + 1;
    //充电机过温故障
    charger_temp_fault = getChargerTempFaultValue();
    dataPacket.push_back(charger_temp_fault);
    packlength = packlength + 1;
    //充电机连接器故障
    cst_connector_fault = getCSTConnectorFaultValue();
    dataPacket.push_back(cst_connector_fault);
    packlength = packlength + 1;
    //充电机内部过温故障
    charger_inner_temp_fault = getChargerInnerTempFaultValue();
    dataPacket.push_back(charger_inner_temp_fault);
    packlength = packlength + 1;
    //所需电量不能传送
    energe_transmit_fault = getEnergeTransmitFaultValue();
    dataPacket.push_back(energe_transmit_fault);
    packlength = packlength + 1;
    //充电机急停故障
    charger_stop_fault = getChargerStopFaultValue();
    dataPacket.push_back(charger_stop_fault);
    packlength = packlength + 1;
    //其他故障
    cst_other_fault = getCSTOtherFaultValue();
    dataPacket.push_back(cst_other_fault);
    packlength = packlength + 1;
    //电流不匹配
    current_mismatch = getCurrentMismatchValue();
    dataPacket.push_back(current_mismatch);
    packlength = packlength + 1;
    //电压异常
    cst_voltage_error = getCSTVoltErrValue();
    dataPacket.push_back(cst_voltage_error);
    packlength = packlength + 1;

    dataPacket[1] = (packlength >> 8) & 0xFF;
    dataPacket[2] = packlength & 0xFF;
    return dataPacket;
}