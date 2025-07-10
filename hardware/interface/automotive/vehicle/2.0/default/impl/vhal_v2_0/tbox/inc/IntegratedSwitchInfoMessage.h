#ifndef INTEGRATEDSWITCHINFOMESSAGE_H
#define INTEGRATEDSWITCHINFOMESSAGE_H

#include "RealTimeMessageBase.h"

class IntegratedSwitchInfoMessage : public RealTimeMessageBase{
  public:
    using RealTimeMessageBase::RealTimeMessageBase;
    int getGearShiftLeverReqPosValue();
    int getRight11SwitchValue();
    int getRight12SwitchValue();
    int getRight13SwitchValue();
    int getFrontWiperSwitchStValue();
    int getLowBeamSwitchStValue();
    int getHiBeamSwitchStValue();
    int getOverVehBeamSwitchStValue();
    int getLampAutoStValue();
    int getLeftTurnLampSwitchStValue();
    int getRightTurnLampSwitchStValue();
    int getDrvSeatbeltBuckleStValue();
    int getPassSeatbeltBuckleStValue();
    int getMidSeatbeltBuckleStValue();
    int getCrashOutputValue();
    int getMidSeatOccupantSensorStValue();
    int getPassSeatOccupantSensorStValue();

    std::vector<uint8_t> generateDataPacket();

    void printMessage();

  private:
    uint8_t integratedSwitchInfoId = 0xA1;
    uint8_t gear_shift_lever_req_pos;                //换挡杆请求位置
    uint8_t right_11_switch;               //右侧开关11状态
    uint8_t right_12_switch;                 //右侧开关12状态
    uint8_t right_13_switch;                //右侧开关13状态
    uint8_t front_wiper_switch_state;                     //前雨刮开关状态
    uint8_t low_beam_switch_state;           //近光灯开关状态
    uint8_t hi_beam_switch_state;          //远光灯开关状态
    uint8_t over_veh_beam_switch_state;            //超车灯开关状态
    uint8_t lamp_auto_state;           //自动大灯状态
    uint8_t left_turn_lamp_switch_state;                //左转向灯开关状态
    uint8_t right_turn_lamp_switch_state;              //右转向灯开关状态
    uint8_t drv_seatbelt_buckle_status;             //主驾安全带未系告警
    uint8_t pass_seatbelt_buckle_status;               //副驾安全带未系告警
    uint8_t mid_seatbelt_buckle_status;              //副驾（中座）安全带未系告警
    uint8_t crash_output_status;                  //碰撞信号
    uint8_t mid_seat_occupant_sensor_status;                   //乘客安全带未系告警
    uint8_t pass_seat_occupant_sensor_status;                   //乘客安全带未系告警
};
#endif