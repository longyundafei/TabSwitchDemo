#ifndef BATTERYCHARGINGINFOMESSAGE_H
#define BATTERYCHARGINGINFOMESSAGE_H

#include "RealTimeMessageBase.h"

class BatteryChargingInfoMessage : public RealTimeMessageBase{
  public:
    using RealTimeMessageBase::RealTimeMessageBase;
    int getSPN256000TimeoutValue();
    int getSPN2560AATimeoutValue();
    int getSPN3903CMLCTSTimeoutValue();
    int getSPN3904CROTimeoutValue();
    int getSPN3905CCSTimeoutValue();
    int getSPN3906CSTTimeoutValue();
    int getSPN3907CSTTimeoutValue();
    int getBMSIdentifyTimeoutValue();
    int getBatteryParamTimeoutValue();
    int getBMSFinishChrgTimeoutValue();
    int getBattStTimeoutValue();
    int getBattChrgReqTimeoutValue();
    int getBMSAbortChrgTimeoutValue();
    int getBMSChrgStatisticsTimeoutValue();
    int getAbortAchiveSOCValue();
    int getAbortAchiveTotalVoltValue();
    int getAbortAchiveSingleVoltValue();
    int getAbortAchiveDCSValue();
    int getInsulationFaultValue();
    int getConnectorTempFaultValue();
    int getBSTAbortFaultBMSTempValue();
    int getBSTConnectorFaultValue();
    int getBattTempFaultValue();
    int getHVFaultValue();
    int getVoltFaultValue();
    int getBSTOtherFaultValue();
    int getCurrentErrValue();
    int getBSTVoltErrValue();
    int getAbortAchiveConditionValue();
    int getAbortByUserValue();
    int getAbortByFaultValue();
    int getAbortByBMSValue();
    int getChargerTempFaultValue();
    int getCSTConnectorFaultValue();
    int getChargerInnerTempFaultValue();
    int getEnergeTransmitFaultValue();
    int getChargerStopFaultValue();
    int getCSTOtherFaultValue();
    int getCurrentMismatchValue();
    int getCSTVoltErrValue();

    std::vector<uint8_t> generateDataPacket();

    void printMessage();

  private:
    uint8_t BatteryChargingInfoId = 0xA8;
    uint8_t spn2560_00_timeout;                  //接收SPN2560=0X00的充电机辨识报文超时
    uint8_t spn2560_aa_timeout;                  //接收SPN2560=0XAA的充电机辨识报文超时
    uint8_t spn3903_cml_cts_timeout;             //接收充电机的时间同步和充电机最大输出能力超时
    uint8_t spn3904_cro_timeout;                 //接收充电机完成充电准备报文超时
    uint8_t spn3905_ccs_timeout;                 //接收充电机充电状态报文超时
    uint8_t spn3906_cst_timeout;                 //接收充电机中止充电报文超时
    uint8_t spn3907_cst_timeout;                 //接收充电机充电统计报文超时
    uint8_t bms_identify_timeout;                //接收BMS和车辆的辨识报文超时
    uint8_t battery_parameter_timeout;           //接收电池充电参数报文超时
    uint8_t bms_finish_charge_timeout;           //接收BMS完成充电准备报文超时
    uint8_t battery_state_timeout;               //接收电池充电总状态报文超时
    uint8_t battery_charge_require_timeout;      //接收电池充电要求报文超时
    uint8_t bms_abort_charge_timeout;            //接收BMS中止充电报文超时
    uint8_t bms_charge_statistics_timeout;       //接收BMS充电统计报文超时
    uint8_t abort_achive_soc;                    //达到所需的SOC目标值
    uint8_t abort_achive_total_volt;             //达到总电压设定值
    uint8_t abort_achive_single_volt;            //到达单体电压设定值
    uint8_t abort_achive_dcs;                    //充电机主动中止
    uint8_t insulation_fault;                    //绝缘故障
    uint8_t connector_temp_fault;                //输出连接器过温故障
    uint8_t bst_abort_fault_bsm_temp;            //BMS元件、输出连接线过温
    uint8_t bst_connector_fault;                 //充电连接器故障
    uint8_t battery_temp_fault;                  //电池组温度过高故障
    uint8_t hv_fault;                            //高压继电器故障
    uint8_t voltage_fault;                       //检测点2电压检测故障
    uint8_t bst_other_fault;                     //其他故障
    uint8_t current_error;                       //电流过大
    uint8_t bst_voltage_error;                   //电压异常
    uint8_t abort_achive_condition;              //到达充电机设定的条件中止
    uint8_t abort_by_user;                       //人工中止
    uint8_t abort_by_fault;                      //故障中止
    uint8_t abort_by_bms;                        //BMS主动中止
    uint8_t charger_temp_fault;                  //充电机过温故障
    uint8_t cst_connector_fault;                 //充电机连接器故障
    uint8_t charger_inner_temp_fault;            //充电机内部过温故障
    uint8_t energe_transmit_fault;               //所需电量不能传送
    uint8_t charger_stop_fault;                  //充电机急停故障
    uint8_t cst_other_fault;                     //其他故障
    uint8_t current_mismatch;                    //电流不匹配
    uint8_t cst_voltage_error;                   //电压异常
};
#endif