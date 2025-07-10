#ifndef CHASSISSYSINFOMESSAGE_H
#define CHASSISSYSINFOMESSAGE_H

#include "RealTimeMessageBase.h"

class ChassisSysInfoMessage : public RealTimeMessageBase{
  public:
    using RealTimeMessageBase::RealTimeMessageBase;
    int getDynamicApplyStValue();
    int getBrakePedalValue();
    int getBrakePedalPercentValue();
    int getBrakeSedalTravelSensorFailValue();
    int getBrakeLightOnReqValue();
    int getBrakePedalTravelSensorFailValue();
    int getRunoutPressureValue();
    int getBrakePedalAppliedValue();
    int getBrakeTempHighValue();
    //int getOutputRodHydraulicTarValue();
    int getEHBOutputRoodDriverPercValue();
    int getEHBRightEPBActuatorStValue();
    int getAdaptiveBrakeAssistActiveStValue();
    int getAutomaticWarningBrakeActiveStValue();
    int getAutomaticEmergencyBrakeActiveStValue();
    int getCDPReqValue();
    int getLeftEPBActuatorStValue();
    int getEPBWarningLampStValue();
    int getEPBFaultStValue();
    int getEPBSysStValue();
    int getBrakeLiquidLevelWarningValue();
    int getEHBFailStValue();
    int getAVHLampReqValue();
    int getHHCStValue();
    int getRgnBrakeStValue();
    int getAVHApplyStValue();
    int getEHBExtReqStValue();
    int getEHBBLAValue();
    int getEHBWarningOnValue();
    int getVehicleStandstillStValue();
    int getFLWheelDirValue();
    int getLFWheelSpdKPHValue();
    int getFRWheelDirValue();
    int getFRWheelSpdKPHValue();
    int getMasterCylinderPressureValue();
    int getRLWheelDirValue();
    int getRLWheelSpdKPHValue();
    int getRRWheelDirValue();
    int getRRWheelSpdKPHValue();
    int getLongtdAccSensorValue();
    int getLatAccSensorValue();
    int getYawRateValue();
    int getESCRightEPBActuatorStValue();
    int getESCorTCSFailedValue();
    int getABSActiveStValue();
    int getVehicleSpdValue();
    int getEBDFailedValue();
    int getABSFailedValue();
    int getTCSActiveStValue();
    int getESPActiveStValue();
    int getReqBrakeLightOnValue();
    int getRMIActiveStValue();
    int getCDPActiveValue();
    int getESSActiveStValue();
    int getEBDActiveStValue();
    int getESCDisValue();
    int getSlopeValue();
    int getHapticWarningServiceAvailValue();
    int getLatLevel12DrivingServiceAvailValue();
    int getEPSMotoreTorqueValue();
    int getMaxInterRPCTorqueValue();
    int getMinInterRPCTorqueValue();
    int getLatMCHapticWarningActValue();
    int getLatMCSteerTorqueActValue();
    int getAngleSysAvailStValue();
    int getMaxAllowedRotSpdValue();
    int getMaxAllowedAngleValue();
    int getEPSteerTorqueValue();
    int getLKSAngleCtrlAvailStValue();
    int getLKSAbortFBValue();
    int getTorisonBarTorqueQFValue();
    int getSteerWhellAngleValue();
    int getSteerWheelRotSpdValue();
    int getSteerWheelAngleCalValue();
    int getSteerWheelAngleFailValue();
    int getElectricPowerStValue();
    int getRorisionBarTorqueDirValue();
    int getElecSteerModeStValue();
    int getEPSModeSetInhibitFBValue();
    int getBasicElecSteerFailValue();
    int getTorsionBarTorqueValue();
    int getElecPowerConsumptionValue();
    int getEPSAdvancedModStValue();
    int getEPSFaultLevelValue();
    int getHodHansoffStValue();

    std::vector<uint8_t> generateDataPacket();

    void printMessage();

  private:
    uint8_t ChassisSysInfoId = 0xA5;
    uint8_t dynamic_apply_sta;                    //动态制动状态
    uint16_t brk_pedl_val;                        //制动踏板行程
    uint8_t brk_pedl_val_perc;                    //制动踏板行程-百分比
    uint8_t pedl_trvl_snsr_fail_pdt;              //制动行程传感器故障
    uint8_t req_brk_li_on;                        //制动灯点亮请求
    uint8_t pedl_trvl_snsr_fail_pdf;              //制动踏板行程传感器故障
    uint8_t runout_pressure;                      //拐点压力值
    uint8_t brake_pedal_applied;                  //驾驶员是否踩下制动踏板
    uint8_t cdd_temp_off;                         //指示制动盘温度是否过高
    //uint16_t output_rod_hydraulic_target;       //EHB主缸目标压力信号
    uint8_t output_rod_driver_perc;               //EHB输入推杆百分比
    uint8_t actuator_st_r;                        //右Moc状态
    uint8_t aba_active_sta;                       //自适应制动辅助激活状态
    uint8_t awb_active_sta;                       //自动警告制动激活状态
    uint8_t aeb_active_sta;                       //自动紧急制动激活状态
    uint8_t cdp_request;                          //动态驻车请求
    uint8_t actuator_st_l;                        //左Moc状态
    uint8_t epb_warn_lamp_sta;                    //EPB故障灯状态
    uint8_t epb_flt_sts;                          //EPB故障状态
    uint8_t epb_system_state;                     //EPB执行机构统一状态
    uint8_t brake_liquid_level_warn;              //制动液位报警
    uint8_t ehb_failure_sts;                      //EHB故障状态
    uint8_t avh_lamp_req;                         //AVH点灯请求
    uint8_t hhc_active_sta;                       //HHC状态
    uint8_t rgn_brk_st;                           //制动能量回收状态
    uint8_t avh_appld_st;                         //AVH功能状态
    uint8_t ext_req_status;                       //电子助力器外部请求可用状态
    uint8_t bla;                                  //电子助力器制动灯请求
    uint8_t hmi_warning_on;                       //EHB警告灯请求
    uint8_t vhcl_standstill_sta;                  //车辆静止状态
    uint8_t wheel_direction_fl;                   //ESC左前轮方向
    uint16_t wheel_speed_kph_fl;                  //ESC左前轮速度
    uint8_t wheel_direction_fr;                   //ESC右前轮方向
    uint16_t wheel_speed_kph_fr;                  //ESC右前轮速度
    uint8_t master_cylinder_presr;                //主缸压力
    uint8_t wheel_direction_rl;                   //ESC左后轮方向
    uint16_t wheel_speed_kph_rl;                  //ESC左后轮速度
    uint8_t wheel_direction_rr;                   //ESC右后轮方向
    uint16_t wheel_speed_kph_rr;                  //ESC右后轮速度
    uint16_t long_acc_sensor_value;               //纵向加速度值
    uint16_t lat_acc_sensor_value;                //横向加速度值
    uint16_t yaw_rate;                            //横摆角速度
    uint8_t epb_actuator_st_r;                    //右Moc状态
    uint8_t esc_or_tcs_failed;                    //ESC/TCS故障
    uint8_t abs_active_sta;                       //ABS激活状态
    uint16_t vehicle_speed;                       //车速
    uint8_t ebd_failed;                           //EBD故障
    uint8_t esc_abs_failed;                       //ABS故障/正常电流运行状态
    uint8_t tcs_active_sta;                       //TCS激活状态
    uint8_t esp_active_sta;                       //ESP激活状态
    uint8_t req_brake_light_on;                   //请求点亮刹车灯
    uint8_t rmi_active_sta;                       //RMI 激活状态
    uint8_t cdp_active;                           //CDP激活状态
    uint8_t ess_active;                           //ESS激活状态
    uint8_t ebd_active_sta;                       //电子制动力分配激活信号
    uint8_t esc_disable;                          //ESC关闭状态
    uint8_t slope_percent;                        //坡度信号
    uint8_t lat_mc_hptc_wrn_srv_avl;              //响应触觉请求的可用性
    uint8_t lat_drv_lvl12_srv_avl;                //横向驾驶服务可用性
    uint16_t eps_mot_tq;                          //EPS电机的实际转矩输出
    uint16_t pscm_int_rpc_max_trq;                //内部允许的最大RPC扭矩
    uint16_t pscm_int_rpc_min_trq;                //内部允许的最小RPC扭矩
    uint8_t lat_mc_hptc_wrn_active;               //触觉警告服务激活
    uint8_t lat_mc_steer_tq_active;               //横向运动控制转向扭矩服务激活
    uint8_t lks_agl_sys_sts;                      //横向角度控制可用状态
    uint16_t max_allwd_rot_spd;                   //最大允许控制角速度
    uint16_t max_allwd_agl;                       //最大允许控制角度
    uint16_t eps_steer_tq;                        //EPS转向扭矩
    uint8_t lks_agl_ctrl_avl_sts;                 //横向角度控制激活状态
    uint8_t lks_abort_fb ;                        //LKS中断原因反馈
    uint8_t torsion_bar_torque_qf;                //扭矩传感器测量结果的有效性
    uint16_t steer_wheel_angle;                   //方向盘转角
    uint16_t steer_wheel_rot_speed;               //方向盘转角速度
    uint8_t steer_wheel_angle_calibrated;         //方向盘角度的标定
    uint8_t steer_wheel_angle_failure;            //方向盘转角失效
    uint8_t electric_power_sta;                   //方向盘助力状态
    uint8_t torsion_bar_torque_dir;               //验证所测扭力杆扭矩
    uint8_t elec_steer_mode_sta;                  //电动助力模式状态
    uint8_t mode_set_inhibit_fb;                  //EPS模式设置抑制反馈
    uint8_t basic_elec_steer_failed;              //基本助力功能发生故障
    uint16_t torsion_bar_torque;                  //测量扭力杆扭矩
    uint8_t eps_elect_power_consumption;          //EPS助力电流信号
    uint8_t eps_advanced_mode_status;             //EPS高级模式状态
    uint8_t eps_fault_lvl;                        //EPS故障等级
    uint8_t hod_hands_off_sts;                    //驾驶员脱手探测状态
};
#endif