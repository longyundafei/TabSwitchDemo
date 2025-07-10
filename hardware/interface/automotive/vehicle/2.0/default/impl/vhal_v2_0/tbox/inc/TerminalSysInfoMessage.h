#ifndef TERMINALSYSINFOMESSAGE_H
#define TERMINALSYSINFOMESSAGE_H

#include "RealTimeMessageBase.h"
#include <cutils/properties.h>
#define PROPERTY_VALUE_MAX_LEN 250

class TerminalSysInfoMessage : public RealTimeMessageBase{
  public:
    using RealTimeMessageBase::RealTimeMessageBase;

    int getCountryCodeValue();
    int getCountryCodeIndValue();
    /*int getRemotePowerCtrlReqValue();
    int getRemoteSteerWhlHeatReqValue();
    int getRemoteMainDrSeatHeatReqValue();*/
    int getACPowerCtrlValue();
    int getECOModeSetValue();
    int getTempSetValue();
    int getAIRInletModeSetValue();
    int getFanSpdSetValue();
    int getDefrostCtrlValue();
    int getBlowerModeSetValue();
    int getACAutoCtlValue();
    int getVentilationValue();
    int getACRemoteCtlFlagValue();
    int getACTempSetValue();
    int getRemoteDefrostCtlValue();

    std::vector<uint8_t> generateDataPacket();

    void printMessage();
  public:
    uint8_t remt_power_ctrl_req;            //远程上下电请求
    uint8_t remt_steer_whl_heat_req;        //远程开关方向盘加热请求
    uint8_t remt_mai_dr_seat_heat_req;      //远程开关座椅加热加热请求

  private:
    uint8_t TerminalSysInfoId = 0xA2;
    uint8_t country_code;                   //当前国家
    uint8_t country_id;                     //指示当前所在国家

    uint8_t ac_power_ctl;                   //HU空调开/关控制信号
    uint8_t hybrid_eco_mode_set;            //HU软开关空调运行模式设置信号
    uint8_t left_temp_set;                  //HU软开关空调温度设置信号
    uint8_t air_inlet_mode_set;             //空调内/外循环调节开关设置信号
    uint8_t front_fan_spd_set;              //HU空调风量挡位调节信号
    uint8_t manual_defrost_ctl;             //HU强制除霜开关
    uint8_t blower_mode_set;                //HU空调出风模式设置信号
    uint8_t ac_auto_ctl;                    //HU空调AUTO开关
    uint8_t open_ventilation;               //远程通风
    uint8_t ac_remt_ctrl_flag;              //远程空调
    uint8_t ac_temp_set;                    //远程空调温度设置
    uint8_t remote_defrost_ctl;             //远程强制除霜
    uint8_t drive_range_mode = 0;           //车主选择的续航里程
};
#endif