#ifndef REVERSEASISTANTSYSINFOMESSAGE_H
#define REVERSEASISTANTSYSINFOMESSAGE_H

#include "RealTimeMessageBase.h"

class ReverseAssistantSysInfoMessage : public RealTimeMessageBase{
  public:
    using RealTimeMessageBase::RealTimeMessageBase;
    int getRLDistValue();
    int getRLMDistValue();
    int getRRMDistValue();
    int getRRDistValue();
    int getSDWRSLDistValue();
    int getSDWRSLMidDistValue();
    int getSDWRSRMidDistValue();
    int getSDWRSRDistValue();
    int getFLDistValue();
    int getFMLDistValue();
    int getFMRDistValue();
    int getFRDistValue();
    int getSDWFSLDistValue();
    int getSDWFSLMidDistValue();
    int getSDWFSRMidDistValue();
    int getSDWFSRDistValue();
    int getBuzzerAlarmPatternValue();
    int getSwitchStFBValue();
    int getPPStValue();
    int getRAEBWorkStValue();
    int getRAEBSetFBValue();
    int getRAEBBrkgReqValue();
    int getRAEBBrkgValue();
    int getRAEBDistToCollisionValue();

    std::vector<uint8_t> generateDataPacket();

    void printMessage();

  private:
    uint8_t ReverseAssistantSysInfoId = 0xA4;
    uint16_t rl_dist;                //倒车雷达后左雷达障碍物距离
    uint16_t rlm_dist;               //倒车雷达后左中雷达障碍物距离
    uint16_t rrm_dist;                 //倒车雷达后右中雷达障碍物距离
    uint16_t rr_dist;                //倒车雷达右后雷达障碍物距离
    uint16_t sdwrsl_dist;                     //后左侧区域障碍物距离
    uint16_t sdwrsl_mid_dist;           //后左中侧区域障碍物距离
    uint16_t sdwrsr_mid_dist;          //后右中侧区域障碍物距离
    uint16_t sdwrsr_dist;            //后右侧区域障碍物距离
    uint16_t fl_dist;           //倒车雷达左前障碍物距离
    uint16_t fml_dist;                //倒车雷达前左中障碍物距离
    uint16_t fmr_dist;              //倒车雷达前右中障碍物距离
    uint16_t fr_dist;             //倒车雷达右前障碍物距离
    uint16_t sdwfsl_dist;               //前左侧区域障碍物距离
    uint16_t sdwfsl_mid_dist;              //前左中侧区域障碍物距离
    uint16_t sdwfsr_mid_dist;                  //前右中侧区域障碍物距离
    uint16_t sdwfsr_dist;                   //前右侧区域障碍物距离
    uint8_t buzzer_alarm_pattern;                   //倒车雷达声音报警
    uint8_t switch_st_fb;                   //开关状态反馈信号
    uint8_t pp_sta;                   //倒车雷达系统状态
    uint8_t raeb_work_sts;                   //后向AEB工作状态
    uint8_t raeb_set_fb;                   //后向AEB设置反馈
    uint8_t raeb_brkg_req;                   //后向AEB激活请求
    uint16_t raeb_brkg_req_val;                   //后向AEB减速度值
    uint16_t raeb_distance_to_collision;                   //后碰撞预警距离报警
};
#endif