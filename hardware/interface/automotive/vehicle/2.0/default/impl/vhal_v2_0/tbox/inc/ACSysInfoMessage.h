#ifndef ACSYSINFOMESSAGE_H
#define ACSYSINFOMESSAGE_H

#include "RealTimeMessageBase.h"

class ACSysInfoMessage : public RealTimeMessageBase{
  public:
    using RealTimeMessageBase::RealTimeMessageBase;
    int getLeftTempValue();
    int getManualDefrostFBValue();
    int getACAutoFBValue();
    int getBlowerModeValue();
    int getFanSpdValue();
    int getACPowerFBValue();
    int getAutoBlowerModeValue();
    int getHybridECOModeValue();
    int getAirInletModeValue();
    int getRearViewMirrorHeatingStValue();
    int getExtendedDefrostActiveReqValue();
    int getEnvirTempValValue();
    int getIndoorTempValue();
    int getEnvirTempValue();
    int getOHXOutTempValue();
    int getEvapTempValue();
    int getEvapOutTempValue();
    int getChillerOutTempValue();

    std::vector<uint8_t> generateDataPacket();

    void printMessage();

  private:
    uint8_t ACSysInfoId = 0xA9;
    uint8_t left_temp;                       //左区温度反馈
    uint8_t manual_defrost_fb;               //手动强制除霜开关反馈
    uint8_t ac_auto_fb;                      //AC AUTO状态
    uint8_t blower_mode;                     //出风模式状态
    uint8_t front_fan_spd;                   //风量档位实际值
    uint8_t ac_power_fb;                     //AC Power状态反馈
    uint8_t auto_blower_mode;                //空调在自动模式下的吹风状态
    uint8_t hybrid_eco_mode;                 //空调模式设置状态
    uint8_t air_inlet_mode;                  //内外循环设置状态
    uint8_t rear_view_mirr_heating_sts;      //后视镜加热状态
    uint8_t extd_defrost_actv_req;           //强制除霜请求
    uint8_t envir_tempV;                     //环境温度有效性
    uint8_t indoor_temp;                     //室内温度
    uint8_t envir_temp;                      //环境温度
    uint8_t ohx_out_tmp;                     //室外换热器出口温度
    uint8_t evap_tmp;                        //蒸发器表面温度
    uint8_t evap_out_tmp;                    //蒸发器出口温度
    uint8_t chiller_out_tmp;                 //chiller出口温度
};
#endif