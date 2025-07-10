#ifndef FRONTCAMINFOMESSAGE_H
#define FRONTCAMINFOMESSAGE_H

#include "RealTimeMessageBase.h"
class FrontCamInfoMessage : public RealTimeMessageBase{
  public:
    using RealTimeMessageBase::RealTimeMessageBase;
    int getLongtdFuncActiveFlgValue();
    int getELKStValue();
    int getEPSSetSteeringWheelAngleValue();
    int getEPSModeReqValue();
    int getDriverWarningVibrateReqValue();
    int getEPSControleReqValue();
    int getEPSTorqueReqValue();
    int getLCCStValue();
    int getLCCSuppressOrAbortConditionValue();
    int getTurnLampReqValue();
    int getOvertakeAssistantStValue();
    int getLKATorqueFactReqVaule();
    int getLKALDWSuppsAbortnValue();
    int getHMASuppsAbortnValue();
    int getELKSuppsAbortnValue();
    int getACCAEBTJAICAFaultSt();
    int getACCTorqueReqActiveStValue();
    int getACCTorqueReqValue();
    int getFCMACCWheelDecelerationValue();
    int getACCTargetAccelerationValue();
    int getCurveDecelerationStValue();
    int getACCTargetAccelerationActiveStValue();
    int getACCSuppressOrAbortConditionValue();
    int getACCShutdownModeValue();
    int getACCTargetDecelerationValue();
    int getACCReqDecelerateToStopValue();
    int getACCDriveOffReqValue();
    int getACCTargetDecelerationReqValue();
    int getACCModeValue();
    int getEPBReqValue();
    int getAheadObjIDValue();
    int getAEBTargetDecelerationValue();
    int getAEBStandstillStValue();
    int getAEBReqValue();
    int getABAReqValue();
    int getEBAReqValue();
    int getAWBLevelValue();
    int getABALevelValue();
    int getAEBStValue();
    int getESPPrefillReqValue();
    int getAWBReqValue();
    int getHMAStValue();
    int getHMAHighbeamReqValue();
    int getAEBSetFBValue();
    int getAEBRecReqValue();
    int getELKSetFBValue();
    int getHMASetFBValue();
    int getCamInitStValue();
    int getSLWFSetFBValue();
    int getCurrentRoadSpeedLimitStValue();
    int getISACascadeSoundAlarmValue();
    int getOverSpeedLimitWarnStValue();
    int getSLIFSetFBValue();
    int getELKRightModeValue();
    int getELKLeftModeValue();
    int getTrafficSignValue();
    int getTSRLgtDstValue();
    int getTSRSetFBValue();
    int getTSRLatDstValue();
    int getTSRSpdLimValue();
    int getSLIFOperStValue();
    int getLCCTakeoverReqValue();
    int getHandsOffWarningValue();
    int getActDrvAsstStHmiValue();
    int getLKAAudioWarningValue();
    int getCamFaultStValue();
    int getACCTarIDValue();
    int getICATruckSetFBValue();
    int getACCStartValue();
    int getADASUnavailReasonValue();
    int getACCTarSpdValue();
    int getACCTakeoverReqValue();
    int getLCCSetFBValue();
    int getADASDriverReqValue();
    int getTAUGapSetValue();
    int getADASQuitReasonValue();
    int getPCWLatentWarningValue();
    int getPCWPreWarningValue();
    int getACCStValue();
    int getLDWSetFBValue();
    int getLDWStValue();
    int getLKASuppsReasonValue();
    int getLKASetFBValue();
    int getLeftLineColorValue();
    int getLKAStValue();
    int getLDWAlertModeSetFBValue();
    int getRightLineColorValue();
    int getAEBObjIDValue();
    int getLateralLimitValue();
    int getLDWAudioWarningValue();
    int getRAEBWorkStValue();
    int getFCWSetFBValue();

    std::vector<uint8_t> generateDataPacket();

    void printMessage();

  private:
    uint8_t  FrontCamInfoMessageID = 0xA0;
    uint8_t  lgt_fct_actv_flg;             //纵向功能激活标志位
    uint8_t  elk_sts;                      //ELK工作状态
    uint16_t eps_set_steer_whl_agl;        //控制器请求的转角值
    uint8_t  daeps_mode_req;               //EPS模式切换请求
    uint8_t  drvr_warn_vib_req;            //方向盘振动请求
    uint8_t  eps_ctrl_req;                 //EPS角度控制请求和控制激活激活请求
    uint8_t  eps_tq_req;                   //EPS扭矩请求
    uint8_t  lcc_st;                       //LCC状态机
    uint8_t  tjaica_supps_abortn;          //LCC抑制、退出条件
    uint8_t  turn_lamp_req;                //转向灯请求
    uint8_t  overtake_ass_sys_sts;         //超车辅助功能系统状态
    uint8_t  lka_torq_fact_req;            //EPS助力请求系数
    uint8_t  lkaldw_supps_abortn;          //LKA_LDW抑制条件
    uint8_t  hma_supps_abortn;             //HMA抑制条件
    uint8_t  elk_supps_abortn;             //ELK抑制条件
    uint8_t  flt_sts;                      //ACC/AEB/TJA/ICA故障状态
    uint8_t  acc_whl_tq_req_actv;          //ACC轮端扭矩请求激活状态
    uint16_t acc_whl_tq_req;               //ACC轮端扭矩请求值
    uint16_t acc_whl_dece;                 //FCM计算的轮端减速度
    uint8_t  acc_tar_accrn;                //ACC目标加速度值
    uint8_t  crv_decelx_sta;               //ACC弯道降速系统状态
    uint8_t  acc_tar_accrn_req_actv;       //ACC目标加速度激活状态
    uint8_t  acc_supp_abort;               //ACC抑制/退出条件
    uint8_t  acc_shutdown_mode;            //ACC快退/慢退请求
    uint8_t  acc_tgt_ax;                   //ACC目标减速度值
    uint8_t  acc_req_stand_still;          //ACC减速到停止状态
    uint8_t  acc_req_drive_off;            //ACC请求启动
    uint8_t  acc_tgt_ax_req;               //ACC目标减速度请求
    uint8_t  acc_mode;                     //ACC状态机
    uint8_t  epb_req;                      //EPB请求
    uint8_t  ahd_obj_id;                   //前方对象ID
    uint16_t aeb_tgt_ax;                   //AEB目标减速度值
    uint8_t  aeb_standstill_sts;           //AEB刹停状态
    uint8_t  aeb_req;                      //AEB刹停状态
    uint8_t  aba_req;                      //ABA请求
    uint8_t  eba_req;                      //EBA请求
    uint8_t  awb_level;                    //AWB等级
    uint8_t  aba_level;                    //ABA等级
    uint8_t  aeb_status;                   //指示AEB系统的状态
    uint8_t  abp_req;                      //预充压请求
    uint8_t  awb_req;                      //AWB请求
    uint8_t  hma_sts;                      //智能远近光灯状态
    uint8_t  hma_highbeam_req;             //远光灯请求
    uint8_t  aeb_set_fb;                   //AEB设置反馈信号
    uint8_t  aeb_rec_req;                  //AEB录制请求
    uint8_t  elk_set_fb;                   //ELK设置反馈信号
    uint8_t  hma_set_fb;                   //HMA设置反馈信号
    uint8_t  fcm_intialization;            //用于指示摄像头初始化的状态
    uint8_t  slwf_set_fb;                  //SLWF功能开关状态反馈
    uint8_t  tsr_spd_lim_sts;              //指示当前道路的限速状态
    uint8_t  tsr_spd_lim_warn_au;          //ISA功能的级联声音报警
    uint8_t  tsr_spd_lim_warn;             //限速报警（请求在车辆速度超过限速时发出警告）
    uint8_t  slif_set_fb;                  //SLIF功能开关状态反馈
    uint8_t  elk_right_mode;               //ELK右侧工作模式
    uint8_t  elk_left_mode;                //ELK左侧工作模式
    uint8_t  trfc_sign;                    //交通标识
    uint16_t tsr_lgt_dst;                  //交通标识纵向距离
    uint8_t  tsr_set_fb;                   //交通标志识别设置反馈
    uint8_t  tsr_lat_dst;                  //交通标识横向距离
    uint8_t  tsr_spd_lim;                  //交通限速值
    uint8_t  slif_oper_sts;                //SLIF功能运行状态
    uint8_t  lcc_tackover_req;             //LCC接管请求
    uint8_t  hands_off_warning;         //脱手警告
    uint8_t  act_drv_asst_sts_hmi;         //LCC状态机
    uint8_t  lka_audio_warning;            //LKA音频报警
    uint8_t  cam_fault_st;              //摄像头故障状态
    uint8_t  tar_obj_acc;                  //ACC目标ID
    uint8_t  ica_truck_set_fb;             //躲避卡车设置反馈
    uint8_t  acc_start;                    //前车起步提醒
    uint8_t  adas_not_avl_reas;            //ADAS功能不可用原因
    uint16_t v_set_dis;                    //ACC目标车速
    uint8_t  acc_take_over_req;            //ACC接管提醒
    uint8_t  ica_set_fb;                   //LCC设置反馈
    uint8_t  adas_drvr_req;                //ADAS功能向驾驶员提醒请求
    uint8_t  tau_gap_set;                  //ACC时距
    uint8_t  adas_quit_reas;               //adas功能退出原因
    uint8_t  pcw_latent_warn;              //前碰撞预警预报警
    uint8_t  pcw_pre_warn;                 //前碰撞预警
    uint8_t  acc_mode_hmi;                 //ACC状态机
    uint8_t  ldw_set_fb;                   //LDW设置反馈信号
    uint8_t  ldw_sts;                      //LDW状态
    uint8_t  text_info_lka1;               //LKA抑制原因
    uint8_t  lka_set_fb;                   //LKA设置反馈
    uint8_t  le_line_color_fct;            //左车道线颜色
    uint8_t  lka_sts;                      //LKA状态
    uint8_t  ldw_alert_method_set_fb;      //LDW报警方式设置反馈
    uint8_t  ri_line_color_fct;            //右车道线颜色
    uint16_t aeb_obj_id;                   //AEB目标ID
    uint8_t  lateral_limit;                //LCC横向接管请求
    uint8_t  ldw_audio_warn;               //LDW音频警告
    uint8_t  raeb_work_sts;                //后向AEB工作状态
    uint8_t  fcw_set_fb;                   //FCW设置反馈
};
#endif