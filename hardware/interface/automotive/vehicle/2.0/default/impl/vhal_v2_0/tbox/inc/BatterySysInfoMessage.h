#ifndef BATTERYSYSINFOMESSAGE_H
#define BATTERYSYSINFOMESSAGE_H

#include "RealTimeMessageBase.h"

class BatterySysInfoMessage : public RealTimeMessageBase{
  public:
    using RealTimeMessageBase::RealTimeMessageBase;
    int getBMSThermalRunawayWrnValue();
    int getInsulationWrningValue();
    int getBMSSupplyVoltStValue();
    int getPreChrgContactorStValue();
    int getPositiveContactorStValue();
    int getNegativeContactorStValue();
    int getBMSRunStValue();
    int getDischrgInterlockStValue();
    int getChrgInterlockStValue();
    int getBMSEmeShutdownReqValue();
    int getPwrTrainConnectStValue();
    int getBMSThermRunawayVlvStValue();
    int getBMSThermRunawaySnsrFltValue();
    int getBatBalanceStValue();
    int getBMSSysFailStValue();
    //int getBMSErrTabIndexValue();
    int getBMSBatSocRptValue();
    int getBMSBatSohValue();
    int getTempMaxDCChrgSocketValue();
    int getBMSPCBMaxTempValue();
    int getMaxAllowChrgCurDCValue();
    int getElectronicLockCtrlValue();
    int getMaxAllowChrgVolDCValue();
    int getDCSChrgAplusValue();
    int getDCSChrgReqValue();
    int getConnectorStDcChrgValue();
    int getElectronicLockStValue();
    int getBMSACChrgAtclStValue();
    int getBMSOutletCoolantActlTempValue();
    int getBMSInletCoolantActlTempValue();
    int getBMSISOResistanceValue();
    int getMaxChrgVoltACValue();
    int getMaxACChrgCurrentValue();
    int getDCSInletConnectStValue();
    int getRemChrgTimeValue();
    int getDCSActlChrgCurrentValue();
    int getDCSActOprtModeValue();
    int getDCSActlChrgVoltValue();
    int getDCSActlChrgPwrValue();
    int getHVBatCellTempAveValue();
    int getMaxConDisCurntValue();
    int getPwrRecupMaxConChrgCurntValue();
    int getMaxInstanDisCurntValue();
    int getPwrRecupMaxInstanChrgCurntValue();
    int getCellVoltAveValue();
    int getNegativeContactorTempValue();
    int getMaxCellTempLimitValue();
    int getMaxCellVolLimitValue();
    int getMinCellTempLimitValue();
    int getPwrLimdFlgValue();
    int getMinCellVolLimitValue();
    int getBatteryTypeValue();
    int getBMSHWVerValue();
    int getBMSPackIDValue();
    int getBMSSWVerValue();
    int getBatCapacityValue();
    int getNomCellCapValue();
    int getChrgTarSOCValue();
    std::vector<uint8_t> generateDataPacket();

    void printMessage();
  public:
    uint16_t bms_err_tab_index[20];                   //BMS故障码索引

  private:
    uint8_t BatterySysInfoId = 0xA7;
    uint8_t bms_ThermalrunawayWrn;                     //电池包热失控报警信号
    uint8_t insulation_wrning;                         //绝缘故障报警
    uint8_t bms_supply_voltage_status;                 //BMS供电电压状态
    uint8_t contactor_state_precharge;                 //预充继电器状态
    uint8_t contactor_state_positive;                  //主正继电器状态
    uint8_t contactor_state_negative;                  //主负继电器状态
    uint8_t bms_run_state;                             //电池管理系统运行状态
    uint8_t discharge_interlock_sta;                   //放电高压互锁状态
    uint8_t charge_interlock_sta;                      //充电高压互锁状态
    uint8_t bms_emeshutdown_req;                       //BMS紧急下电请求
    uint8_t pwr_train_connect_sta;                     //动力总成连接状态
    uint8_t bms_therm_runaway_vlv_st;                  //BMS热失控防爆阀状态
    uint8_t bms_therm_runaway_snsr_flt;                //热失控传感器故障
    uint8_t batt_balance_sta;                          //电池包均衡状态
    uint8_t bms_sys_fail_sta;                          //BMS失效状态

    uint16_t bms_bat_soc_rpt;                          //SOC_Rpt_电池表显剩余电量
    uint16_t bms_bat_soh;                              //SOH_高压电池健康状态
    uint8_t temp_max_dc_charge_socket;                 //直流充电座最高温度
    uint8_t bms_pcb_temp_max;                          //BMS板载最高温度
    uint16_t max_allow_chrg_cur_dc;                    //直流充电最大允许充电电流
    uint8_t electronic_lock_ctrl;                      //电子锁开关控制
    uint16_t max_allow_chrg_vol_dc;                    //直流充电最大允许充电电压
    uint8_t dcs_chrg_a_plus;                           //充电桩A+辅助信号
    uint8_t dcs_charge_request;                        //直流充电请求
    uint8_t contactor_state_dc_charger;                //直流充电继电器状态
    uint8_t electronic_lock_state;                     //电子锁开关状态
    uint8_t bms_ac_chrg_atcl_sta;                      //BMS交流充电实际状态
    uint8_t bms_outlet_coolant_actl_temp;              //电池实际出水口温度
    uint8_t bms_inlet_coolant_actl_temp;               //电池实际入水口温度
    uint16_t bms_iso_resistance;                       //高压电池对地绝缘阻值
    uint16_t max_chrg_voltage_ac;                      //交流充电最大允许充电电压
    uint16_t max_chrg_current_ac;                      //交流充电最大允许充电电流
    uint8_t dcs_inlet_connect_sta;                     //直流枪插抢连接状态
    uint16_t rem_chrg_time;                            //剩余充电时间
    uint16_t dcs_actl_chrg_current;                    //直流充电桩实际输出电流
    uint8_t dcs_act_oprt_mode;                         //直流充电实际状态
    uint16_t dcs_actl_chrg_voltage;                    //直流充电桩实际输出电压
    uint16_t dcs_actl_chrg_power;                      //桩实际输入功率
    uint8_t hv_bat_cell_temp_ave;                      //平均电芯温度
    uint16_t max_con_dis_curnt;                        //最大持续放电电流
    uint16_t pwr_recup_max_con_chrg_curnt;             //能量回收最大持续充电电流
    uint16_t max_instan_dis_curnt;                     //电池最大瞬时放电电流
    uint16_t pwr_recup_max_instan_chrg_curnt;          //能量回收最大瞬时充电电流
    uint16_t cell_vol_ave;                             //所有电池包平均单体电压
    uint8_t contactor_temp_negative;                   //主负继电器铜牌温度
    uint8_t max_cell_temp_limit;                       //最高电芯温度限制值
    uint16_t max_cell_vol_limit;                       //最高单体电压限制值
    uint8_t min_cell_temp_limit;                       //最低电芯温度限制值
    uint8_t pwr_limd_flg;                              //充电桩功率小于车辆允许功率标志位
    uint16_t min_cell_vol_limit;                       //最低单体电压限制值
    uint8_t battery_type;                              //电池类型
    uint8_t bms_hw_ver;                                //BMS硬件版本
    uint8_t bms_pack_id;                               //电池包识别标识
    uint8_t bms_sw_ver;                                //BMS软件版本
    uint8_t bat_capacity;                              //高压电池容量
    uint8_t nom_cell_cap;                              //名义单体容量
    uint8_t chrg_target_soc;                           //充电目标SOC反馈
};
#endif