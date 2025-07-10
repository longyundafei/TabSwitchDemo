#ifndef POWERSYSINFOMESSAGE_H
#define POWERSYSINFOMESSAGE_H

#include "RealTimeMessageBase.h"

class PowerSysInfoMessage : public RealTimeMessageBase{
  public:
    using RealTimeMessageBase::RealTimeMessageBase;
    int getCCorCPConStValue();
    int getCCResisdentValue();
    int getCPDutyValue();
    int getCPMaxVoltValue();
    int getOBCProcessCodeValue();
    int getDCCLowVolActlVoltValue();
    int getDCCActlOprtModeValue();
    int getDCCLowVoltActlCurValue();
    int getHVILFailStValue();
    int getDCDCSysFailStValue();
    int getIPUOverTempFailValue();
    int getDCCDeratOprtStValue();
    int getDCCPrechrgCompltValue();
    int getOBCConnectMainStValue();
    int getOBCHVDCActlCurValue();
    int getOBCSysFailStValue();
    int getOBCInletConnStValue();
    int getOBCReqValue();
    int getOBCHVDCActlVoltValue();
    int getOBCActlOprtModeValue();
    int getOBCInletActlPowerValue();
    int getDCDCHVActlCurValue();
    int getDCDCHVActlVoltValue();
    int getDevInternalTempValue();
    int getOBCTempInletValue();
    int getS2StValue();
    int getOBCWakeupModeValue();
    int getOBCInletElecLockValue();
    int getOBCACMaxPwAllowValue();
    int getVoltOkl3Value();
    int getVoltOkl2Value();
    int getVoltOkl1Value();
    int getUACActl1Value();
    int getUACActl2Value();
    int getUACActl3Value();
    int getIACActl1Value();
    int getIACActl2Value();
    int getIACActl3Value();

    std::vector<uint8_t> generateDataPacket();

    void printMessage();

  private:
    uint8_t PowerSysInfoId = 0xA6;
    uint8_t cc_or_cp_cnctd_sts;                //CCorCP连接状态
    uint16_t cc_resisdent;                     //CC电阻值
    uint8_t cp_duty;                           //CP信号占空比
    uint8_t cp_max_volt;                       //CP信号幅值电压
    uint8_t obc_process_code;                  //OBC流程码
    uint8_t dcc_low_vol_actl_vol;              //DCC低压端实际电压
    uint8_t dcc_actl_oprt_mode;                //DCC实际运行模式
    uint16_t dcc_low_vol_actl_cur;             //DCC低压端实际电流
    uint8_t hvil_fail_sta;                     //高压互锁系统故障
    uint8_t dcdc_sys_fail_sta;                 //DCDC系统失效状态
    uint8_t ipu_over_temperature_fail;         //IPU过温故障
    uint8_t dcc_derating_oprt_sta;             //DCC降级运行模式
    uint8_t dcc_precharge_complete;            //DCC预充完成标志位
    uint8_t obc_connect_main_sta;              //OBC直流侧高压回路连接状态
    uint8_t obc_hvdc_actl_cur;                 //OBC充放电工作时，HVDC侧电流
    uint8_t obc_sys_fail_sta;                  //交流充电系统失效状态
    uint8_t obc_inlet_connct_sta;              //交流充电枪连接状态
    uint8_t obc_request;                       //交流充电机根据充放电模式，请求充电或者放电
    uint16_t obc_hvdc_actl_vol;                //OBC充放电工作时，HVDC侧电压
    uint8_t obc_actl_oprt_mode;                //交流充电系统实际运行模式
    uint16_t obc_inlelt_actl_power;            //枪实际功率
    uint16_t dcdc_high_volt_actl_cur;          //DCDC高压侧实际电流
    uint16_t dcdc_high_volt_actl_vol;          //DCDC高压侧实际电压
    uint8_t device_int_temp;                   //设备内部温度
    uint8_t obc_temp_inlet;                    //充电枪温度
    uint8_t s2_state;                          //GB S2开关状态
    uint8_t obc_wake_up_mode;                  //OBC唤醒VCU的模式
    uint8_t obc_inlet_electronic_lock;         //充电枪电子锁状态
    uint16_t obc_ac_max_pw_allow;              //充放电中，OBC允许最大充放电功率
    uint8_t voltage_ok_l3;                     //充电枪L3电压状态
    uint8_t voltage_ok_l2;                     //充电枪L2电压状态
    uint8_t voltage_ok_l1;                     //充电枪L1电压状态
    uint16_t uac_act_l1;                       //L1相电压
    uint16_t uac_act_l2;                       //L2相电压
    uint16_t uac_act_l3;                       //L3相电压
    uint16_t iac_act_l1;                       //L1相电流
    uint16_t iac_act_l2;                       //L2相电流
    uint16_t iac_act_l3;                       //L3相电流
};
#endif