#include "CustomInfoFlags.h"
#define PROPERTY_VALUE_MAX_LEN 250

int CustomInfoFlags::getprogramVersionSwitchControlStatusValue() {
    return getValue(toInt(VehicleProperty::GECKO_MCU_CCM_VERSION));
}
int CustomInfoFlags::getlockControlStatusValue() {
    int level = getValue(toInt(VehicleProperty::GECKO_MCU_SPEED_LIMIT_LEVEL));
    //LOG(INFO) << "getlockControlStatusValue 1:" << level;
    if(level ==1){
        int charging = getValue(toInt(VehicleProperty::GECKO_MCU_CHARGING_CONDIONS));
        //LOG(INFO) << "getlockControlStatusValue 2:" << charging;
        if(charging == 1){
            level = 6;
        }
    }
    //LOG(INFO) << "getlockControlStatusValue:" << level;
    return level;
}
int CustomInfoFlags::getchargingQuantityValue() {
    return getRawPropValue(toInt(VehicleProperty::GECKO_BMS_SINGLECHRGENERGY))*10;
}
int CustomInfoFlags::getvehicleChargeFlagValue() {
    int value = 0;
    int val = 0;
    val = getRawValue(toInt(VehicleProperty::GECKO_VCU_CHRG_STS));
    switch (val) {
        case 0:
        value = 0;
        break;
        case 1:
        value = 0;
        break;
        case 2:
        value = 2;
        break;
        case 3:
        value = 1;
        break;
        case 4:
        value = 0;
        break;
        case 5:
        value = 0;
        break;
        default:
        value = 0xfe;
        break;
    }
    return value;
}

/*void CustomInfoFlags::setSpeedLimitLevel(uint8_t level) {
    switch(level){
        case 0x55:
            mReqSpeedLimitLevel = 0x01;
            break;
        case 0x56:
            mReqSpeedLimitLevel = 0x02;
            break;
        case 0x57:
            mReqSpeedLimitLevel = 0x03;
            break;
        case 0x58:
            mReqSpeedLimitLevel = 0x04;
            break;
        case 0x59:
            mReqSpeedLimitLevel = 0x05;
            break;
        case 0x60:
            mReqSpeedLimitLevel = 0x06;
            break;
        case 0xaa:
            mReqSpeedLimitLevel = 0x00;
            break;
        default:
            LOG(INFO) << "CustomInfoFlags cannot handle SpeedLimitLevel msg, do nothing ";
            break;
    }
    LOG(INFO) << "CustomInfoFlags setSpeedLimitLevel = " << mReqSpeedLimitLevel;
}*/

int CustomInfoFlags::getlockControlFlagValue() {
    //return getValue(toInt(VehicleProperty::GECKO_MCU_SPEED_LIMIT_LEVEL));
    char ret[PROPERTY_VALUE_MAX_LEN] = "\0";
    if(property_get("persist.tbox.speedlimitlevel", ret, nullptr)!=0) {
        int level = std::stoi(ret);
        //LOG(INFO) << "getlock_vehicle_control_flagValue 1:" << level;
        if(level ==1){
            memset(ret, 0, sizeof(ret));
            if(property_get("persist.tbox.chargingcondition", ret, nullptr)!=0){
                int charging = std::stoi(ret);
                //LOG(INFO) << "getlock_vehicle_control_flagValue 2:" << charging;
                if(charging == 1){
                    level = 6;
                }
            }
        }
        return level;
    } else {
        return 0;
    }
    //return (int)mReqSpeedLimitLevel;
}
int CustomInfoFlags::getvehicleOnSignalValue() {
    return getValue(toInt(VehicleProperty::GECKO_VCU_IGON));
}
int CustomInfoFlags::getvehicleCanWakeupState() {
    /*int can0 = getValue(toInt(VehicleProperty::GECKO_CAN_COM_STS_EV));
    int can1 = getValue(toInt(VehicleProperty::GECKO_CAN_COM_STS_BODY));
    int can2 = getValue(toInt(VehicleProperty::GECKO_CAN_COM_STS_OBD));
    int can3 = getValue(toInt(VehicleProperty::GECKO_CAN_COM_STS_CH));
    int state = can0 || can1 || can2 || can3;*/
    int state = 1;
    return state;
}
int CustomInfoFlags::getvehicleCanState() {
    /*int can0 = getValue(toInt(VehicleProperty::GECKO_CAN_COM_STS_EV));
    int can1 = getValue(toInt(VehicleProperty::GECKO_CAN_COM_STS_BODY));
    int can2 = getValue(toInt(VehicleProperty::GECKO_CAN_COM_STS_OBD));
    int can3 = getValue(toInt(VehicleProperty::GECKO_CAN_COM_STS_CH));
    int state = can0 | can1 << 1 | can2<< 2 | can3 << 3;*/
    int state = 1;
    return state;
}
int CustomInfoFlags::getvehicleSmallBatteryVoltage() {
    return getValue(toInt(VehicleProperty::GECKO_VCU_POWERSUPPLYVOLTAG))*100;
}
int CustomInfoFlags::getmaintenance_reminderValue() {
    int ret = getValue(toInt(VehicleProperty::GECKO_VEHICLE_REMIND_WARNING_ST));
    if(ret != 0) {
        ret = 1;
    }
    return ret;
}
int CustomInfoFlags::getpayment_reminderValue() {
    int ret = getValue(toInt(VehicleProperty::GECKO_VEHICLE_PAY_WARNING_ST));
    if(ret != 0) {
        ret = 1;
    }
    return ret;
}
int CustomInfoFlags::getannual_review_reminderValue() {
    int ret = getValue(toInt(VehicleProperty::GECKO_VEHICLE_ANNUAL_REVIEW_WARNING_ST));
    if(ret != 0) {
        ret = 1;
    }
    return ret;
}
int CustomInfoFlags::getvehicleChargeSignalValue() {
    return getValue(toInt(VehicleProperty::GECKO_VCU_CHRG_STS));
}
int CustomInfoFlags::getvcuVersionNumberValue() {
    return getValue(toInt(VehicleProperty::GECKO_VCU_SOFEWARE_NUM));
}
int CustomInfoFlags::getpumpFaultValue() {
    return getValue(toInt(VehicleProperty::GECKO_VCU_MOTWATERPUMPSTATUS));
}
int CustomInfoFlags::getptcWorkingStatusValue() {
    return getValue(toInt(VehicleProperty::GECKO_HVAC_PTC_HEAT_ON));
}
int CustomInfoFlags::getptcRelayActualStatusValue() {
    int val = 0;
    /*val = val | (getValue(toInt(VehicleProperty::GECKO_VCU_PTC1RELSTA))-1);
    val = val | (getValue(toInt(VehicleProperty::GECKO_VCU_PTC2RELSTA))-1);

    if((val!=0)&&(val!=1)){
        val = 0xfe;
    }*/
    val = getValue(toInt(VehicleProperty::GECKO_VCU_PTC2RELSTA));
    val = val << 4;
    val = val | getValue(toInt(VehicleProperty::GECKO_VCU_PTC1RELSTA));
    return val;
}
int CustomInfoFlags::getpowerBatteryHeatingRelayStatusValue() {
    return getValue(toInt(VehicleProperty::GECKO_BMS_HEATRELAYSTATUS));
}
int CustomInfoFlags::getquickChargeRelayAdhesionFaultValue() {
    return getValue(toInt(VehicleProperty::GECKO_VCU_DCCHRGRLYSTS));
}
int CustomInfoFlags::getairConditioningWorkingStatusValue() {
    int ACPower = 0;
    int value = 0;
    int faultCode = getValue(toInt(VehicleProperty::GECKO_HVAC_CONTROL_ERROR));
    //LOG(INFO) << __func__ << "faultCode: " << faultCode;
    if (1 == faultCode) {
        LOG(INFO) << __func__ << " return 2";
        return 2;
    } else {
        ACPower = getValue(toInt(VehicleProperty::HVAC_POWER_ON));
        //LOG(INFO) << __func__ << "havc_state: " << ACPower;
        switch (ACPower){
            case 3:
            value = 0xFF;
            break;
            default:
            value = ACPower;
        }
        //LOG(INFO) << __func__ << " return " << value;
        return value;
    }
}
int CustomInfoFlags::gethandbrakeStatusValue() {
    return getValue(toInt(VehicleProperty::GECKO_VCU_PARKSTATE));
}
int CustomInfoFlags::getpowerBatteryMaintenanceFlagValue() {
    return getValue(toInt(VehicleProperty::GECKO_CCM_BATTMAINTAIN_CTR_CMD));
}
int CustomInfoFlags::getpowerBatteryMaintenanceStatusValue() {
    return getValue(toInt(VehicleProperty::GECKO_CCM_BATTMAINTAIN_STS));
}
int CustomInfoFlags::getVCUVersion() {
    return getValue(toInt(VehicleProperty::GECKO_VCU_SOFEWARE_NUM));
}

std::vector<uint8_t> CustomInfoFlags::generateDataPacket() {
    std::vector<uint8_t> dataPacket;
    dataPacket.push_back(customInfoFlagsId);
    uint16_t packlength = 0;
    dataPacket.push_back((packlength >> 8) & 0xFF);
    dataPacket.push_back(packlength & 0xFF);
    packlength = 0;
    deviceModelNumber = 0x09;
    dataPacket.push_back(deviceModelNumber);
    packlength = packlength + 1;
    dataPacket.push_back(powerSupplyIdentification);
    packlength = packlength + 1;
    int VCUVer = getVCUVersion();
    firmwareVersion[4] = static_cast<uint8_t>((VCUVer >> 8) & 0xFF);
    firmwareVersion[3] = static_cast<uint8_t>(VCUVer & 0xFF);;
    firmwareVersion[2] = 0;
    firmwareVersion[1] = 0;
    firmwareVersion[0] = 0;
    dataPacket.push_back(firmwareVersion[4]);
    dataPacket.push_back(firmwareVersion[3]);
    dataPacket.push_back(firmwareVersion[2]);
    dataPacket.push_back(firmwareVersion[1]);
    dataPacket.push_back(firmwareVersion[0]);
    packlength = packlength + 5;
    relayStatus = 0xff;
    dataPacket.push_back(relayStatus);
    packlength = packlength + 1;
    terminalBindingStatus = 0xff;
    dataPacket.push_back(terminalBindingStatus);
    packlength = packlength + 1;
    programVersionSwitchControlStatus = getprogramVersionSwitchControlStatusValue();
    dataPacket.push_back(programVersionSwitchControlStatus);
    packlength = packlength + 1;
    lockControlStatus = getlockControlStatusValue();
    dataPacket.push_back(lockControlStatus);
    packlength = packlength + 1;
    chargingQuantity = getchargingQuantityValue();
    dataPacket.push_back((chargingQuantity >> 8) & 0xFF);
    dataPacket.push_back(chargingQuantity & 0xFF);
    packlength = packlength + 2;
    int vcuver = getvcuVersionNumberValue();
    vcuVersionNumber[7] = 'S';
    vcuVersionNumber[6] = 'V';
    vcuVersionNumber[5] = (vcuver >> 12) & 0x0f;
    vcuVersionNumber[4] = (vcuver >> 8) & 0x0f;
    vcuVersionNumber[3] = '.';
    vcuVersionNumber[2] = (vcuver >> 4) & 0x0f;
    vcuVersionNumber[1] = vcuver & 0x0f;
    vcuVersionNumber[0] = 0x00;
    dataPacket.push_back(vcuVersionNumber[7]);
    dataPacket.push_back(vcuVersionNumber[6]);
    dataPacket.push_back(vcuVersionNumber[5]);
    dataPacket.push_back(vcuVersionNumber[4]);
    dataPacket.push_back(vcuVersionNumber[3]);
    dataPacket.push_back(vcuVersionNumber[2]);
    dataPacket.push_back(vcuVersionNumber[1]);
    dataPacket.push_back(vcuVersionNumber[0]);
    packlength = packlength + 8;
    vehicleChargeFlag = getvehicleChargeFlagValue();
    dataPacket.push_back(vehicleChargeFlag);
    packlength = packlength + 1;
    lockControlFlag = getlockControlFlagValue();
    dataPacket.push_back(lockControlFlag);
    packlength = packlength + 1;
    faultCount = 0xFF;
    dataPacket.push_back(faultCount);
    packlength = packlength + 1;
    dataPacket.push_back((vehicleTypeIdentification >> 8) & 0xFF);
    dataPacket.push_back(vehicleTypeIdentification & 0xFF);
    packlength = packlength + 2;
    terminalWorkingStatus = 0x01;
    dataPacket.push_back(terminalWorkingStatus);
    packlength = packlength + 1;
    vehicleOnSignal = getvehicleOnSignalValue();
    dataPacket.push_back(vehicleOnSignal);
    packlength = packlength + 1;
//    vehicleChargeSignal = getvehicleChargeSignalValue();
    //参照对照表无信号处理
    vehicleChargeSignal = 0;
    dataPacket.push_back(vehicleChargeSignal);
    packlength = packlength + 1;
    canWakeUpStatus = getvehicleCanWakeupState();
    dataPacket.push_back(canWakeUpStatus);
    packlength = packlength + 1;
    canState = getvehicleCanState();
    dataPacket.push_back(canState);
    packlength = packlength + 1;
    vehicleSmallBatteryVoltage = getvehicleSmallBatteryVoltage();
    dataPacket.push_back((vehicleSmallBatteryVoltage >> 8) & 0xFF);
    dataPacket.push_back(vehicleSmallBatteryVoltage & 0xFF);
    packlength = packlength + 2;
    internalBatteryVoltage = 0xFFFF;
    dataPacket.push_back((internalBatteryVoltage >> 8) & 0xFF);
    dataPacket.push_back(internalBatteryVoltage & 0xFF);
    packlength = packlength + 2;
    LinkIpLength = 0;
    dataPacket.push_back(LinkIpLength);
    packlength = packlength + 1;
    if(LinkIpLength!=0){
        for(uint8_t i = 0;i<packlength;i++){
            dataPacket.push_back(LinkIpPort[i]);
            packlength = packlength + 1;
        }
    }
    dataPacket.push_back(LinkStatus);
    packlength = packlength + 1;
    maintenanceReminder = getmaintenance_reminderValue();
    dataPacket.push_back(maintenanceReminder);
    packlength = packlength + 1;
    paymentReminder = getpayment_reminderValue();
    dataPacket.push_back(paymentReminder);
    packlength = packlength + 1;
    annualReviewReminder = getannual_review_reminderValue();
    dataPacket.push_back(annualReviewReminder);
    packlength = packlength + 1;
    dataPacket.push_back(networkSignal);
    packlength = packlength + 1;
    dataPacket.push_back(gpsWorkingStatus);
    packlength = packlength + 1;
    dataPacket.push_back(emmcWorkingStatus);
    packlength = packlength + 1;
    airConditioningFeedbackCurrent = 0xffff;
    dataPacket.push_back((airConditioningFeedbackCurrent >> 8) & 0xFF);
    dataPacket.push_back(airConditioningFeedbackCurrent & 0xFF);
    packlength = packlength + 2;
//    pumpFault = getpumpFaultValue();
    //参考对照表写ff
    pumpFault = 0xff;
    dataPacket.push_back(pumpFault);
    packlength = packlength + 1;
    vacuumPumpStatus = 0xff;
    dataPacket.push_back(vacuumPumpStatus);
    packlength = packlength + 1;
    vacuumDegree = 0xff;
    dataPacket.push_back(vacuumDegree);
    packlength = packlength + 1;
    ptcWorkingStatus = getptcWorkingStatusValue();
    dataPacket.push_back(ptcWorkingStatus);
    packlength = packlength + 1;
    ptcRelayActualStatus = getptcRelayActualStatusValue();
    dataPacket.push_back(ptcRelayActualStatus);
    packlength = packlength + 1;
    powerBatteryHeatingRelayStatus = getpowerBatteryHeatingRelayStatusValue();
    dataPacket.push_back(powerBatteryHeatingRelayStatus);
    packlength = packlength + 1;
    quickChargeRelayAdhesionFault = getquickChargeRelayAdhesionFaultValue();
    dataPacket.push_back(quickChargeRelayAdhesionFault);
    packlength = packlength + 1;
    airConditioningWorkingStatus = getairConditioningWorkingStatusValue();
    dataPacket.push_back(airConditioningWorkingStatus);
    packlength = packlength + 1;
    handbrakeStatus = gethandbrakeStatusValue();
    dataPacket.push_back(handbrakeStatus);
    packlength = packlength + 1;
    powerBatteryMaintenanceFlag = getpowerBatteryMaintenanceFlagValue();
    powerBatteryMaintenanceFlag = 0xff;
    dataPacket.push_back(powerBatteryMaintenanceFlag);
    packlength = packlength + 1;
    powerBatteryMaintenanceStatus = getpowerBatteryMaintenanceStatusValue();
    powerBatteryMaintenanceStatus = 0xff;
    dataPacket.push_back(powerBatteryMaintenanceStatus);
    packlength = packlength + 1;
    voiceAlarmModuleOnlineStatus = 0xff;
    dataPacket.push_back(voiceAlarmModuleOnlineStatus);
    packlength = packlength + 1;

    dataPacket[1] = (packlength >> 8) & 0xFF;
    dataPacket[2] = packlength & 0xFF;
    return dataPacket;
}

void CustomInfoFlags::updateConfig(int Sampling) {
    if(Sampling == 1){
        LinkStatus = 1;
    } else {
        LinkStatus = 0;
    }
}
