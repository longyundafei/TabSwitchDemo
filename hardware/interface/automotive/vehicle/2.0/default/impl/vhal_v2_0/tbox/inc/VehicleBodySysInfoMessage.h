#ifndef VEHICLEBODYSYSINFOMESSAGE_H
#define VEHICLEBODYSYSINFOMESSAGE_H

#include "RealTimeMessageBase.h"

class VehicleBodySysInfoMessage : public RealTimeMessageBase{
  public:
    using RealTimeMessageBase::RealTimeMessageBase;
    int getLeftTurnLampStValue();
    int getRightTurnLampStValue();
    int getHighBeamStValue();
    int getLowBeamStValue();
    int getDayRunLightStValue();
    int getFrontFogLampStValue();
    int getRearFogLampStValue();
    int getPosLampStValue();
    int getReverseLampStValue();
    int getBrakeLampStValue();
    int getBackLampStValue();
    int getAutoLampStValue();
    int getFrontFogLightSwitchValue();
    int getRearFogLightSwitchValue();
    int getTurnLightStValue();
    int getLightIntensityValue();
    int getFollowMeHomeCtrlFBValue();
    int getBassHornReqValue();
    int getTrunkAjarStValue();
    int getFLDoorAjarStValue();
    int getFRDoorAjarStValue();
    int getRLDoorAjarStValue();
    int getRRDoorAjarStValue();
    int getFrontLeftDoorHandleSwitchValue();
    int getFrontRightDoorHandleSwitchValue();
    int getFLPressureStValue();
    int getFRPressureStValue();
    int getRLPressureStValue();
    int getRRPressureStValue();
    int getFrontWiperCtrlValue();
    int getFrontWiperMotorStValue();
    int getFrontWashStValue();
    int getFLWindowStValue();
    int getFRWindowStValue();
    int getFLWindowMotorStValue();
    int getFRWindowMotorStValue();
    int getBodyWarnStValue();
    int getPSAuthentResultValue();
    int getRKEReqValue();
    int getDriverPEAuthentAccessedValue();
    int getFRPEAuthentAccessedValue();
    int getPSReqValue();
    int getWiperIntervalTimeSetFBValue();
    int getBCMStartReqValue();
    int getOverspdCtrlLockSetFBValue();
    int getKeyInCarStValue();
    int getAuthentKeyCountValue();
    int getForbidenKeyCountValue();
    int getAvaluebleKeyInCarValue();
    int getKeyStValue();
    int getKeyForbidenValue();
    int getKeyLowBattLevelWarnValue();
    int getWarnKeyOutReminderValue();
    int getIndicationKeyCloserValue();
    int getPEPSIlluminatedStValue();
    int getSteerWheelHeatStValue();
    int getRearMirrorHeatStValue();
    int getLogoActiveFlagValue();
    int getMainDriverSeatHeatStValue();
    int getRFInitStValue();
    int getHoodAjarStValValue();
    int getHoodAjarStValue();
    int getDriverSeatOccupantSensorStValue();
    int getHeavyRainIndValue();
    int getSavePowerStValue();
    int getDomeLightStValue();
    int getCarriageLightStValue();
    int getWiperStopPosStValue();
    int getFLPressureValue();
    int getFRPressureValue();
    int getRLPressureValue();
    int getRRPressureValue();

    std::vector<uint8_t> generateDataPacket();

    void printMessage();

  private:
    uint8_t VehicleBodySysInfoId = 0xAA;
    uint8_t left_turn_lamp_st;                //左转向灯状态
    uint8_t right_turn_lamp_st;               //右转向灯状态
    uint8_t high_beam_st;                     //远光灯状态
    uint8_t low_beam_st;                      //近光灯状态
    uint8_t day_run_light_st;                 //日间行车灯状态
    uint8_t front_fog_lamp_st;                //前雾灯状态
    uint8_t rear_fog_lamp_st;                 //后雾灯状态
    uint8_t pos_lamp_st;                      //位置灯状态
    uint8_t reverse_lamp_st;                  //倒车灯状态
    uint8_t brake_lamp_st;                    //制动灯状态
    uint8_t back_lamp_st;                     //背光灯状态
    uint8_t auto_lamp_st;                     //自动大灯功能状态
    uint8_t frnt_fog_light_switch;            //前雾灯开关状态
    uint8_t rear_fog_light_switch;            //后雾灯开关状态
    uint8_t turn_light_st;                    //转向灯状态
    uint8_t light_intensity;                  //白天黑夜状态
    uint8_t flwr_me_hm_ctrl_fb;               //伴我回家延时时间反馈
    uint8_t bass_horn_req;                    //低音喇叭请求
    uint8_t trunk_ajar_st;                    //行李箱开关状态
    uint8_t fl_door_ajar_st;                  //左前门开关状态
    uint8_t fr_door_ajar_st;                  //右前门开关状态
    uint8_t rl_door_ajar_st;                  //左后门开关状态
    uint8_t rr_door_ajar_st;                  //右后门开关状态
    uint8_t frnt_le_door_hndl_swt;            //主驾门把手微动开关
    uint8_t frnt_ri_door_hndl_swt;            //副驾门把手微动开关
    uint8_t fl_pressure_st;                   //左前胎压状态
    uint8_t fr_pressure_st;                   //右前胎压状态
    uint8_t rl_pressure_st;                   //左后胎压状态
    uint8_t rr_pressure_st;                   //右后胎压状态
    uint8_t front_wiper_ctrl;                 //前雨刮控制状态
    uint8_t front_wiper_motor_st;             //前雨刮电机状态
    uint8_t front_wash_st;                    //前洗涤状态
    uint8_t fl_window_st;                     //左前车窗状态
    uint8_t fr_window_st;                     //右前车窗状态
    uint8_t fl_window_motor_st;               //左前车窗电机状态
    uint8_t fr_window_motor_st;               //右前车窗电机状态
    uint8_t body_warn_st;                     //车身防盗报警状态
    uint8_t ps_authent_result;                //PS认证结果
    uint8_t rke_req;                          //RKE 请求
    uint8_t drv_pe_authent_acsd;              //驾驶侧PE认证访问
    uint8_t fr_pe_authent_acsd;               //右前门PE认证访问
    uint8_t ps_req;                           //PS请求
    uint8_t wipr_intl_time_set_fb;            //雨刮间歇时间设置反馈
    uint8_t bcm_start_req;                    //BCM_StartReq
    uint8_t overspd_cntrl_lock_set_fb;        //车速超出四门闭锁设置反馈
    uint8_t key_in_car_st;                    //钥匙在车内状态
    uint8_t authent_key_count;                //已匹配钥匙数量
    uint8_t frbdn_key_count;                  //禁用钥匙数量
    uint8_t avalueble_key_in_car;             //车内存在有效钥匙
    uint8_t key_st;                           //钥匙状态
    uint8_t key_frbdn;                        //钥匙被禁止
    uint8_t key_low_batt_lvl_warn;            //钥匙电池低电量报警
    uint8_t warn_key_out_reminder;            //警告钥匙离开车内
    uint8_t indication_key_closer;            //提示请将钥匙靠近线圈
    uint8_t illmnd_sts;                       //PEPS迎宾状态
    uint8_t steer_whl_heat_st;                //方向盘加热状态
    uint8_t rear_mirror_heat_st;              //后视镜加热状态
    uint8_t logo_active_flag;                 //伴我回家/迎宾/照我上车LOGO点亮标志位
    uint8_t main_driver_seat_heat_st;         //主驾座椅加热状态
    uint8_t rf_init_st;                       //射频初始化状态
    uint8_t hood_ajar_st_v;                   //前舱盖微动开关状态有效性
    uint8_t hood_ajar_st;                     //前舱盖微动开关状态
    uint8_t drv_seat_occupant_sensor_st;      //主驾占座
    uint8_t heavy_rain_ind;                   //暴雨指示
    uint8_t save_power_st;                    //节电输出状态
    uint8_t dome_light_st;                    //前顶灯输出状态
    uint8_t carriage_light_st;                //货厢灯输出状态
    uint8_t wiper_stop_pos_st ;               //雨刮归位状态
    uint8_t fl_pressure;                      //左前胎压值
    uint8_t fr_pressure;                      //右前胎压值
    uint8_t rl_pressure;                      //左后胎压值
    uint8_t rr_pressure;                      //右后胎压值

};
#endif