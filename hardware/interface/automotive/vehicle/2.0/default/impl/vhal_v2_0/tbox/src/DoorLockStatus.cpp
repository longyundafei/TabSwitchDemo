#include "DoorLockStatus.h"

int DoorLockStatus::getleft_front_door_statusValue() {
    return getValue(toInt(VehicleProperty::GECKO_FL_DOOR_STATE));
}
int DoorLockStatus::getright_front_door_statusValue() {
    return getValue(toInt(VehicleProperty::GECKO_FR_DOOR_STATE));
}
int DoorLockStatus::getleft_rear_door_statusValue() {
    return 0xff;
}
int DoorLockStatus::getright_rear_door_statusValue() {
    return getValue(toInt(VehicleProperty::GECKO_RR_DOOR_STATE));
}
int DoorLockStatus::gettrunk_door_statusValue() {
    return getValue(toInt(VehicleProperty::GECKO_TR_CLS_ST));
}
int DoorLockStatus::getleft_front_door_lock_statusValue() {
    return getValue(toInt(VehicleProperty::GECKO_IPU_OBCINPUTCURRENT));
}
int DoorLockStatus::getdrive_lock_statusValue() {
    return getValue(toInt(VehicleProperty::GECKO_BCM_DRIVELOCKST));
}

std::vector<uint8_t> DoorLockStatus::generateDataPacket() {
    std::vector<uint8_t> dataPacket;
    dataPacket.push_back(DoorLockStatusId);
    uint16_t packlength = 0;
    dataPacket.push_back((packlength >> 8) & 0xFF);
    dataPacket.push_back(packlength & 0xFF);
    packlength = 0;
    left_front_door_status = getleft_front_door_statusValue();
    dataPacket.push_back(left_front_door_status);
    packlength = packlength + 1;
    right_front_door_status = getright_front_door_statusValue();
    dataPacket.push_back(right_front_door_status);
    packlength = packlength + 1;
    left_rear_door_status = getleft_rear_door_statusValue();
    dataPacket.push_back(left_rear_door_status);
    packlength = packlength + 1;
    right_rear_door_status = getright_rear_door_statusValue();
    dataPacket.push_back(right_rear_door_status);
    packlength = packlength + 1;
    trunk_door_status = gettrunk_door_statusValue();
    dataPacket.push_back(trunk_door_status);
    packlength = packlength + 1;
    left_front_door_lock_status = getdrive_lock_statusValue();
    dataPacket.push_back(left_front_door_lock_status);
    packlength = packlength + 1;
    right_front_door_lock_status = getdrive_lock_statusValue();
    dataPacket.push_back(right_front_door_lock_status);
    packlength = packlength + 1;
    left_rear_door_lock_status = 0xff;
    dataPacket.push_back(left_rear_door_lock_status);
    packlength = packlength + 1;
    right_rear_door_lock_status = getdrive_lock_statusValue();
    dataPacket.push_back(right_rear_door_lock_status);
    packlength = packlength + 1;
    trunk_door_lock_status = getdrive_lock_statusValue();
    dataPacket.push_back(trunk_door_lock_status);
    packlength = packlength + 1;
    left_front_window_status = 0xff;
    dataPacket.push_back(left_front_window_status);
    packlength = packlength + 1;
    right_front_window_status = 0xff;
    dataPacket.push_back(right_front_window_status);
    packlength = packlength + 1;
    left_rear_window_status = 0xff;
    dataPacket.push_back(left_rear_window_status);
    packlength = packlength + 1;
    right_rear_window_status = 0xff;
    dataPacket.push_back(right_rear_window_status);
    packlength = packlength + 1;
    front_sunroof_status = 0xff;
    dataPacket.push_back(front_sunroof_status);
    packlength = packlength + 1;
    rear_sunroof_status = 0xff;
    dataPacket.push_back(rear_sunroof_status);
    packlength = packlength + 1;
	
    dataPacket[1] = (packlength >> 8) & 0xFF;
    dataPacket[2] = packlength & 0xFF;
    return dataPacket;
}