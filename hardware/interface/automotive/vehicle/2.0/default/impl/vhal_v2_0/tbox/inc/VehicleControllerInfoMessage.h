#ifndef VEHICLECONTROLLERINFOMESSAGE_H
#define VEHICLECONTROLLERINFOMESSAGE_H

#include "RealTimeMessageBase.h"

class VehicleControllerInfoMessage : public RealTimeMessageBase{
  public:
    using RealTimeMessageBase::RealTimeMessageBase;
    int getActualWheelTorqueValue();
    int getDriverReqMotorTorqueValue();
    int getMaxAllowedDriveTorqueValue();
    int getPwrRecupActlWhlTqStValue();
    int getPwrRecupMaxAvlbWhlTqStValue();
    int getBrkPwrRecupMaxAvlbWhlTqValue();
    int getPwrRecupActWhlTqValue();
    int getVCUReqDriveOffValue();
    int getBrkPwrRecupActlWhlTqStValue();
    int getBrkPwrRecupActWhlTqValue();
    int getActlMotorRotateSpdValue();
    int getWLTPDrivingRangeDistValue();
    int getDrivingRangeDistEstValue();
    int getVCUInfoDispValue();
    int getIBSChargeNumAllowValue();
    int getIBSBMSBatSOCAllowValue();
    int getIBSWUPVoltAllowValue();
    int getIBSRTCWakeChgNumValue();
    int getIBSRTCChrgStValue();
    int getIBSRTCWakeupChrgFailNumValue();
    int getIBSForbidenRTCWakeupFlgValue();
    int getVehicleActDrivingPwrValue();
    int getVehicleActRecupPwrValue();
    int getOperatLicenceFBValue();
    int getVersionFBValue();
    int getSpdLimitLevelFBValue();
    int getRemoteLockFBValue();
    int getRemoteCarSearchFBValue();
    int getVersionSetFBValue();
    int getSpdLimitLevelSetFBValue();
    int getRemotePwrLockFBValue();
    int getRemotePwrCtrlFBValue();
    int getRemoteSteerwhlHeatFBValue();
    int getRemoteMainDriverSeatHeatFBValue();
    int getPwrTrainActOperatingModeValue();
    int getVehicleActGearPosValue();
    int getSysPwrModeValue();
    int getBatLowVolStValue();
    int getLimpHomeStValue();
    int getHazardLampStValue();
    int getCentralLockStValue();
    int getSysPwrStValue();
    int getFLDoorLockStValue();
    int getVehicleActDrvMod1Value();
    int getDiagTesterOnlineValue();
    int getHDCStValue();
    int getAcclPedalPosValue();
    int getBrkPedalPosValue();
    int getEPBReqValue();
    int getCruiseCtrlStValue();
    int getCruiseCtrlTarSpdValue();
    int getWeightEstValue();
    int getWeightDistributValue();
    int getACCTqReqValue();
    int getAcclPedalPosSensorStValue();
    int getDrvReqTqOvverrideFCMValue();
    int getACCTarAXValue();
    int getACCTarAXReqValue();
    int getBrkTqReqValue();
    int getACCStandstillReqValue();

    std::vector<uint8_t> generateDataPacket();

    void printMessage();

  private:
    uint8_t VehicleControllerInfoId = 0xAB;
    uint16_t act_whl_tq;                            //实际轮端扭矩
    uint16_t drv_req_whl_tq;                        //驾驶员请求轮端扭矩
    uint16_t drv_whl_tq_allwd_max;                  //最大允许驱动轮端扭矩
    uint8_t pwr_recup_actl_whl_tq_st;               //能量回收状态
    uint8_t pwr_recup_max_avlb_whl_tq_st;           //最大允许轮端扭矩状态
    uint16_t brk_pwr_recup_max_avlb_whl_tq;         //制动能量回收的最大可用扭矩
    uint16_t pwr_recup_act_whl_tq;                  //功率回收实际车轮扭矩
    uint8_t acc_req_drive_off;                      //VCU请求起步
    uint8_t brk_pwr_recup_actl_whl_tq_st;           //制动能量回收轮端扭矩状态
    uint16_t brk_pwr_recup_act_whl_tq;              //制动能量回收实际轮端扭矩
    uint16_t actl_motor_rotate_spd;                 //实际电机端转速
    uint16_t wltp_drvg_rng_dst;                     //WLTP续航里程距离
    uint16_t drv_range_dist_est;                    //预估续驶里程距离
    uint8_t vcu_info_disp;                          //VCU信息提示
    uint8_t ibs_chg_num_allow;                      //IBS补电允许（次数）
    uint8_t ibs_bms_bat_soc_allow;                  //IBS补电允许（电池SOC）
    uint8_t ibs_wup_volt_allow;                     //IBS补电允许（蓄电池电压）
    uint8_t ibs_rtcwk_chg_num;                      //IBS补电次数（RTC唤醒次数）
    uint8_t ibs_rtc_chrg_st;                        //IBS补电状态
    uint8_t ibs_rtcwu_chg_fail_num;                 //IBS补电失败累计次数
    uint8_t ibs_fb_rtcw_flg;                        //禁止RTC唤醒补电
    uint8_t veh_act_driving_pwr;                    //车辆实际驾驶强度
    uint8_t veh_act_recuperate_pwr;                 //车辆实际回收强度
    uint8_t operat_licence_fb;                      //远程运营许可
    uint8_t version_fb;                             //远程锁车版本
    uint8_t speed_limit_level_fb;                   //远程限速等级
    uint8_t remote_lock_fb;                         //远程控制锁反馈
    uint8_t remote_car_search_fb;                   //远程寻车反馈
    uint8_t vesion_set_fb;                          //远程锁车版本切换反馈
    uint8_t speed_limit_level_set_fb;               //远程限速执行反馈
    uint8_t remote_power_lock_fb;                   //远程禁止上电执行反馈
    uint8_t remt_power_ctrl_fb;                     //远程上下电反馈
    uint8_t remt_steer_whl_heat_fb;                 //远程开关方向盘加热反馈
    uint8_t remt_main_driver_seat_heat_fb;          //远程开关座椅加热加热反馈
    uint8_t pt_actl_oprt_mode;                      //动力总成运行模式
    uint8_t vhcl_actl_gear_pos;                     //车辆实际档位
    uint8_t sys_pwr_mode;                           //系统电源模式
    uint8_t bat_low_vol_st;                         //12V电池低压状态
    uint8_t limp_home_st;                           //跛行模式
    uint8_t hazard_lamp_st;                         //警示灯状态
    uint8_t central_lock_st;                        //中控锁状态
    uint8_t sys_power_st;                           //整车电源状态
    uint8_t door_lock_st_fl;                        //左前门锁状态
    uint8_t veh_act_drv_mod1;                       //车辆实际驾驶模式1
    uint8_t diag_tester_online;                     //OBD口诊断仪在线状态信号
    uint8_t hdc_st;                                 //HDC状态
    uint8_t accl_pedal_pos;                         //加速踏板位置
    uint8_t brk_pedal_pos;                          //制动踏板位置
    uint8_t req_epb;                                //EPB拉起/释放请求
    uint8_t cruise_ctrl_st;                         //定速巡航状态
    uint16_t cruise_ctr_tgt_spd;                    //定速巡航目标速度
    uint16_t wgh_est;                               //载重估计值
    uint8_t wgh_distb;                              //载重分布
    uint8_t acc_trq_ctrl_avl;                       //VCU允许ACC开启标志位
    uint8_t accl_pedal_pos_snsr_st;                 //加速踏板位置传感器状态
    uint8_t drv_req_torq_override_fcm;              //驾驶员请求扭矩大于FCM请求扭矩
    uint8_t acc_tgt_ax;                             //ACC目标减速度
    uint8_t acc_tgt_ax_req;                         //ACC目标减速度请求
    uint16_t bra_tor_req;                           //制动扭矩请求
    uint8_t acc_standstill_req;                     //ACC请求VLC减速到停止
};
#endif