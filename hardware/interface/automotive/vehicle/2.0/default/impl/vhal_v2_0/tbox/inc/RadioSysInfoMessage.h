#ifndef RADIOSYSINFOMESSAGE_H
#define RADIOSYSINFOMESSAGE_H

#include "RealTimeMessageBase.h"

class RadioSysInfoMessage : public RealTimeMessageBase{
  public:
    using RealTimeMessageBase::RealTimeMessageBase;
    int getRightRCTAWarningValue();
    int getRCTASetFBValue();
    int getBSDSetFBValue();
    int getLeftBSDWarningValue();
    int getRightBSDWarningValue();
    int getRCTBDecelarationValue();
    int getRCWWarningValue();
    int getFOVChgSetFBValue();
    int getRCTBDeceReqValue();
    int getLeftRCTAWarningValue();
    int getDOWSetFBValue();
    int getRearCornerBlindnessValue();
    int getRearCornerFailurValue();
    int getRCWSetFBValue();
    int getDOWRLWarningValue();
    int getDOWRRWarningValue();
    int getDOWFLWarningValue();
    int getDOWFRWarningValue();
    int getDOWLWarningValue();
    int getDOWRWarningValue();


    std::vector<uint8_t> generateDataPacket();

    void printMessage();

  private:
    uint8_t RadioSysInfoId = 0xA3;
    uint8_t rcta_warn_right;                //右侧RCTA报警
    uint8_t rcta_set_fb;               //RCTA设置反馈
    uint8_t bsd_set_fb;                 //BSD设置反馈
    uint8_t bsd_warn_left;                //左侧BSD报警
    uint8_t bsd_warn_right;                     //右侧BSD报警
    uint8_t rctb_decelaration;           //RCTB减速度值
    uint8_t rcw_warning;          //RCW报警信号
    uint8_t fov_chg_set_fb;            //FOV模式切换请求
    uint8_t rctb_req;           //RCTB减速度请求
    uint8_t rcta_warn_left;                //左侧RCTA报警
    uint8_t dow_set_fb;              //DOW设置反馈
    uint8_t rear_corner_blindness;             //雷达遮挡状态
    uint8_t rear_corner_failure;               //雷达故障状态
    uint8_t rcw_set_fb;              //RCW设置反馈
    uint8_t dow_warn_rl;                  //DOW左后门报警信号
    uint8_t dow_warn_rr;                   //DOW右后门报警信号
    uint8_t dow_warn_fl;                   //DOW左前门报警信号
    uint8_t dow_warn_fr;                   //DOW左前门报警信号
    uint8_t dow_warn_left;                   //DOW右侧报警信号
    uint8_t dow_warn_right;                   //DOW左侧报警信号
};
#endif