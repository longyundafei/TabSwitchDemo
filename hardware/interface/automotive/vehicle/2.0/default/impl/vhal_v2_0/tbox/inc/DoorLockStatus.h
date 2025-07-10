#ifndef DOORLOCKSTATUS_H
#define DOORLOCKSTATUS_H

#include "RealTimeMessageBase.h"

class DoorLockStatus : public RealTimeMessageBase{
  public:
    using RealTimeMessageBase::RealTimeMessageBase;

    std::vector<uint8_t> generateDataPacket();
    int getleft_front_door_statusValue();
    int getright_front_door_statusValue();
    int getleft_rear_door_statusValue();
    int getright_rear_door_statusValue();
    int gettrunk_door_statusValue();
    int getleft_front_door_lock_statusValue();
    int getdrive_lock_statusValue();
    void printMessage();

  private:
    uint8_t DoorLockStatusId = 0x91;
    uint8_t left_front_door_status;                //左前门状态      BCM_FLDoorAjarStatus
    uint8_t right_front_door_status;               //右前门状态      BCM_FRDoorAjarStatus
    uint8_t left_rear_door_status;                 //左后门状态      ff
    uint8_t right_rear_door_status;                //右后门状态      BCM_RRDoorAjarStatus
    uint8_t trunk_door_status;                     //后备门状态      BCM_TrClsSt
    uint8_t left_front_door_lock_status;           //左前门锁状态    m
    uint8_t right_front_door_lock_status;          //右前门锁状态    m
    uint8_t left_rear_door_lock_status;            //左后门锁状态    ff
    uint8_t right_rear_door_lock_status;           //右后门锁状态    m
    uint8_t trunk_door_lock_status;                //后备门锁状态    m
    uint8_t left_front_window_status;              //左前窗状态      ff
    uint8_t right_front_window_status;             //右前窗状态      ff
    uint8_t left_rear_window_status;               //左后窗状态      ff
    uint8_t right_rear_window_status;              //右后窗状态      ff
    uint8_t front_sunroof_status;                  //前天窗状态      ff
    uint8_t rear_sunroof_status;                   //后天窗状态      ff
};
#endif