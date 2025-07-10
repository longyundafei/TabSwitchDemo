#include "VehicleDataMessage.h"
#include <android-base/logging.h>
#include <iostream>
#include <iomanip>

uint32_t VehicleDataMessage::getvehicleStatusValue() {
    uint32_t vcuPTST = getValue(toInt(VehicleProperty::GECKO_VCU_PT_ST));
    uint32_t ret = 0xFE;
    switch(vcuPTST){
        case 0x0:
        case 0x1:
        case 0x3:
            ret = 0x02;
            break;
        case 0x2:
            ret = 0x1;
            break;
        case 0x4:
        case 0x5:
        case 0x6:
        case 0x7:
            ret = 0x3;
            break;
        default:
            ret = 0xFF;
            break;
    }
    return ret;
}
uint32_t VehicleDataMessage::getchargingStatusValue() {
    uint32_t ret = 0xFF;
    uint32_t mCurrent = getValue(toInt(VehicleProperty::GECKO_EV_BATTERY_CURRENT));
    uint32_t mSpeed = getValue(toInt(VehicleProperty::PERF_VEHICLE_SPEED));
    //LOG(INFO) << "mCurrent :" << mCurrent << ", mSpeed:" << mSpeed;
    if((mCurrent != 0xFFFF && mCurrent < 10000) && (mSpeed != 0xFF && mSpeed > 17)){
        //17.77:  1km/h
        ret = 02;
        return ret;
    }
    uint32_t mBmsDcsActOprt = getValue(toInt(VehicleProperty::GECKO_BMS_DCS_ACT_OPRT_MODE));
    uint32_t mIpuObcActlOprt = getValue(toInt(VehicleProperty::GECKO_IPU_OBC_ACTL_OPRT_MODE));
    //LOG(INFO) << "mBmsDcsActOprt :" << mBmsDcsActOprt << ", mIpuObcActlOprt:" << mIpuObcActlOprt;
    if(mBmsDcsActOprt == 0x05 || mIpuObcActlOprt == 0x02){
        ret = 0x01;
        return ret;
    }
    if(mBmsDcsActOprt == 0x06 || mIpuObcActlOprt == 0x03){
        ret = 0x04;
        return ret;
    }
    if(mBmsDcsActOprt == 0x00 || mBmsDcsActOprt == 0x01 || mBmsDcsActOprt == 0x02 || mBmsDcsActOprt == 0x03
     || mBmsDcsActOprt == 0x04|| mBmsDcsActOprt == 0x07
     || mIpuObcActlOprt == 0x00 || mIpuObcActlOprt == 0x01 || mIpuObcActlOprt > 0x04){
        ret = 0x03;
        return ret;
    }
    return ret;
}
uint32_t VehicleDataMessage::getvehicleSpeedValue() {
    int speed = getValue(toInt(VehicleProperty::PERF_VEHICLE_SPEED));
    if(speed != 0xFFFF){
        return speed*0.5625;
    } else {
        return speed;
    }
}
uint32_t VehicleDataMessage::getaccumulatedMileageValue() {
    return getValue(toInt(VehicleProperty::PERF_ODOMETER));
}
uint32_t VehicleDataMessage::gettotalVoltageValue() {
    return getValue(toInt(VehicleProperty::GECKO_EV_BATTERY_VOLTAGE));
}
uint32_t VehicleDataMessage::gettotalCurrentValue() {
    return getValue(toInt(VehicleProperty::GECKO_EV_BATTERY_CURRENT));
}
uint32_t VehicleDataMessage::getstateOfChargeValue() {
    uint32_t level = getValue(toInt(VehicleProperty::EV_BATTERY_LEVEL))/10;
    return level;
}
uint32_t VehicleDataMessage::getdcDcStatusValue() {
    uint32_t ipuDcOptMode = getValue(toInt(VehicleProperty::GECKO_IPU_DCCACTL_OPTMODE));
    uint32_t ret = 0xFF;
    switch(ipuDcOptMode){
        case 0x01:
        case 0x06:
             ret = 0x02;
             break;
        case 0x03:
             ret = 0x01;
             break;
        case 0x05:
             ret = 0xFE;
             break;
        default:
             ret = 0xFF;
             break;
    }
    return ret;
}
uint32_t VehicleDataMessage::getgearvalueValue() {
    return getValue(toInt(VehicleProperty::CURRENT_GEAR));
}
uint32_t VehicleDataMessage::getparkvalueValue() {
    return getValue(toInt(VehicleProperty::GECKO_VCU_PARKSTATE));
}
uint32_t VehicleDataMessage::getinsulationResistanceValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_ISORESISTANCE));
}
uint32_t VehicleDataMessage::getacceleratorPedalPositionValue() {
    return getValue(toInt(VehicleProperty::GECKO_VCU_ACCLPEDAL_POS));
}
uint32_t VehicleDataMessage::getbrakePedalStatusValue() {
    /*int pedalStatus = getValue(toInt(VehicleProperty::GECKO_VCU_BPSTATUS));
    int pedalPos = 0;
    uint32_t ret = 0;
    if(pedalStatus == 1){
        //获取开合值
        //pedalPos = getValue(toInt(VehicleProperty::GECKO_VCU_BRK_PDL_POS));
        pedalPos = 0;
        if(pedalPos > 250 || pedalPos == 0){
            ret = 0x65;
        } else {
            ret = pedalPos*0.4;
        }
    } else if(pedalStatus == 0){
        ret = 0;
    } else {
        ret = 0xFF;
    }
    return ret;*/
    uint32_t ret = 0;
    int brakePedalSt = getValue(toInt(VehicleProperty::GECKO_BCM_BRAKEPEDALSTA));
    if(brakePedalSt == 0x01){
        int brkPedlVld = getValue(toInt(VehicleProperty::GECKO_EHB_BRKPEDL_VALPERCVLD));
        if(brkPedlVld == 0x01){
            ret = getValue(toInt(VehicleProperty::GECKO_EHB_BRKPEDL_VALPERC));
        } else {
            ret = 0x65;
        }
    }
    return ret;
}
uint32_t VehicleDataMessage::getbrakePedalPosValue() {
    return getValue(toInt(VehicleProperty::GECKO_VCU_BRKPEDAL_POS));
}
uint32_t VehicleDataMessage::getbrakePedalVldValue() {
    return getValue(toInt(VehicleProperty::GECKO_EHB_BRKPEDL_VALVLD));
}
std::vector<uint8_t> VehicleDataMessage::generateDataPacket() {
    std::vector<uint8_t> dataPacket;

    // 添加信息id
    dataPacket.push_back(vehicleid);
    vehicleStatus = getvehicleStatusValue();
    // 添加车辆状态
    dataPacket.push_back(vehicleStatus);
    chargingStatus = getchargingStatusValue();
    // 添加充电状态
    dataPacket.push_back(chargingStatus);

    // 添加运行模式
    dataPacket.push_back(0x01);

    uint16_t speed = getvehicleSpeedValue();
    if(speed < 2200){
        vehicleSpeed = speed;
    } else {
        LOG(INFO) << "vehicleSpeed more than 220, use the last value";
    }
    // 添加车速
    dataPacket.push_back((vehicleSpeed >> 8) & 0xFF);
    dataPacket.push_back(vehicleSpeed & 0xFF);

    accumulatedMileage = getaccumulatedMileageValue();
    // 添加累计里程
    dataPacket.push_back((accumulatedMileage >> 24) & 0xFF);
    dataPacket.push_back((accumulatedMileage >> 16) & 0xFF);
    dataPacket.push_back((accumulatedMileage >> 8) & 0xFF);
    dataPacket.push_back(accumulatedMileage & 0xFF);

    int voltage = gettotalVoltageValue();
    if(voltage >= 0 && voltage <= 10000){
        totalVoltage = voltage;
    }
    // 添加总电压
    dataPacket.push_back((totalVoltage >> 8) & 0xFF);
    dataPacket.push_back(totalVoltage & 0xFF);

    int current = gettotalCurrentValue();
    if((current >= 0) && (current <= 20000)){
        totalCurrent = current;
    }
    // 添加总电流
    dataPacket.push_back((totalCurrent >> 8) & 0xFF);
    dataPacket.push_back(totalCurrent & 0xFF);

    stateOfCharge = getstateOfChargeValue();
    // 添加SOC
    dataPacket.push_back(stateOfCharge);

    dcDcStatus = getdcDcStatusValue();
    // 添加DC-DC状态
    dataPacket.push_back(dcDcStatus);

    uint8_t gearvalue = getgearvalueValue();
    //uint8_t parkvalue = getparkvalueValue();
    acceleratorPedalPosition = getacceleratorPedalPositionValue();
    uint32_t brakeKPedalPos = getbrakePedalPosValue();
    uint32_t brakeKPedalVld = getbrakePedalVldValue();
    this->gear = 0b00000000;
    if(acceleratorPedalPosition > 0){
        this->gear = this->gear | 0b00100000;
    }
    if((brakeKPedalPos > 0) && (brakeKPedalVld == 0x01)){
        this->gear = this->gear | 0b00010000;
    }
    switch(gearvalue) {
        case 0x00://P
        this->gear = this->gear | 0b00001111;
        break;
        case 0x01://N
        this->gear = this->gear | 0b00000000;
        break;
        case 0x02://D
        this->gear = this->gear | 0b00001110;
        break;
        case 0x03://R
        this->gear = this->gear | 0b00001101;
        break;
        default:
        this->gear = this->gear | 0b00000000;
        break;
    }

//    gear = gearvalue|parkvalue;
    /*if (parkvalue == 0x01 && gearvalue == 0x00) {
        this->gear = 0b00011111;  // P挡
    } else if (parkvalue == 0x01 && gearvalue == 0x01) {
        this->gear = 0b00111111;  // P挡
    } else if (parkvalue == 0x01 && gearvalue == 0x02) {
        this->gear = 0b00111111;  // P挡
    } else if (parkvalue == 0x00 && gearvalue == 0x00) {
        this->gear = 0b00000000;  // N挡
    } else if (parkvalue == 0x00 && gearvalue == 0x01) {
        this->gear = 0b00101110;  // D挡
    } else if (parkvalue == 0x00 && gearvalue == 0x02) {
        this->gear = 0b00101101;  // R挡
    } else {
        this->gear = 0b00000000;
    }*/
    //LOG(INFO) << __func__ << __LINE__ << ": " << std::setfill('0') << std::setw(2)
              //<< std::hex << static_cast<int>(this->gear);
    // 添加挡位
    dataPacket.push_back(gear);

    uint16_t resistance = getinsulationResistanceValue();
    if(resistance > 0){
        insulationResistance = resistance;
    }
//    insulationResistance  = insulationResistance * 5;
    // 添加绝缘电阻
    dataPacket.push_back((insulationResistance >> 8) & 0xFF);
    dataPacket.push_back(insulationResistance & 0xFF);

    //acceleratorPedalPosition = getacceleratorPedalPositionValue();
    // 添加加速踏板行程值
    dataPacket.push_back(acceleratorPedalPosition);

    brakePedalStatus = getbrakePedalStatusValue();
    // 添加制动踏板状态
    dataPacket.push_back(brakePedalStatus);

    return dataPacket;
}

void VehicleDataMessage::printMessage() {
/*    std::cout << "信息ID: " << static_cast<int>(vehicleid) << std::endl;
    std::cout << "车辆状态: " << static_cast<int>(vehicleStatus) << std::endl;
    std::cout << "充电状态: " << static_cast<int>(chargingStatus) << std::endl;
    std::cout << "运行模式: " << static_cast<int>(runningMode) << std::endl;
    std::cout << "车速: " << vehicleSpeed / 10.0 << " km/h" << std::endl;
    std::cout << "累计里程: " << accumulatedMileage / 10.0 << " km" << std::endl;
    std::cout << "总电压: " << totalVoltage / 10.0 << " V" << std::endl;
    std::cout << "总电流: " << totalCurrent / 10.0 << " A" << std::endl;
    std::cout << "SOC: " << static_cast<int>(stateOfCharge) << "%" << std::endl;
    std::cout << "DC-DC状态: " << static_cast<int>(dcDcStatus) << std::endl;
    std::cout << "挡位: " << static_cast<int>(gear) << std::endl;
    std::cout << "绝缘电阻: " << insulationResistance << " kn" << std::endl;
    std::cout << "添加加速踏板行程值: " << static_cast<int>(acceleratorPedalPosition) << std::endl;
    std::cout << "添加制动踏板状态: " << static_cast<int>(brakePedalStatus) << std::endl;*/

    LOG(INFO) << "信息ID: " << static_cast<int>(vehicleid);
    LOG(INFO) << "车辆状态: " << static_cast<int>(vehicleStatus);
    LOG(INFO) << "充电状态: " << static_cast<int>(chargingStatus);
    LOG(INFO) << "运行模式: " << static_cast<int>(runningMode);
    LOG(INFO) << "车速: " << vehicleSpeed / 10.0 << " km/h";
    LOG(INFO) << "累计里程: " << accumulatedMileage / 10.0 << " km";
    LOG(INFO) << "总电压: " << totalVoltage / 10.0 << " V";
    LOG(INFO) << "总电流: " << totalCurrent / 10.0 << " A";
    LOG(INFO) << "SOC: " << static_cast<int>(stateOfCharge) << "%";
    LOG(INFO) << "DC-DC状态: " << static_cast<int>(dcDcStatus);
    LOG(INFO) << "挡位: " << static_cast<int>(gear);
    LOG(INFO) << "绝缘电阻: " << insulationResistance << " kn";
    LOG(INFO) << "添加加速踏板行程值: " << static_cast<int>(acceleratorPedalPosition);
    LOG(INFO) << "添加制动踏板状态: " << static_cast<int>(brakePedalStatus);
}
