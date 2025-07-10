#include "CustomInfoMessage.h"
#include <iostream>

CustomInfoMessage::CustomInfoMessage() {
    hvacValue.resize(8, 0);
    carDoorValue.resize(16, 0);
    carStatusValue.resize(29, 0);
    dataCollectionValue.resize(85, 0);
    customInfoValue.resize(57, 0);
}

void CustomInfoMessage::updateCustomInfoMessage() {
    for (const auto& item : mVehicleCtl.mItemList) {
        if (item.opt.propId == (int)VehicleProperty::HVAC_POWER_ON) {
            hvacValue[0] = static_cast<uint8_t>(std::stoi(item.right_value) + 1);
            customInfoValue[52] = static_cast<uint8_t>(std::stoi(item.right_value));
        } else if (item.opt.propId == (int)VehicleProperty::HVAC_RECIRC_ON) {
            hvacValue[1] = static_cast<uint8_t>(std::stoi(item.right_value));
        } else if (item.opt.propId == (int)VehicleProperty::HVAC_FAN_DIRECTION) {
            hvacValue[2] = static_cast<uint8_t>(std::stoi(item.right_value));
        } else if (item.opt.propId == (int)VehicleProperty::HVAC_AC_ON) {
            hvacValue[3] = static_cast<uint8_t>(std::stoi(item.right_value));
        } else if (item.opt.propId == (int)VehicleProperty::GECKO_HVAC_PTC_HEAT_ON) {
            hvacValue[4] = static_cast<uint8_t>(std::stoi(item.right_value));
            customInfoValue[48] = static_cast<uint8_t>(std::stoi(item.right_value));
        } else if (item.opt.propId == (int)VehicleProperty::HVAC_DEFROSTER) {
            hvacValue[5] = static_cast<uint8_t>(std::stoi(item.right_value));
        } else if (item.opt.propId == (int)VehicleProperty::HVAC_FAN_SPEED) {
            hvacValue[6] = static_cast<uint8_t>(std::stoi(item.right_value));
        } else if (item.opt.propId == (int)VehicleProperty::HVAC_TEMPERATURE_SET) {
            hvacValue[7] = static_cast<uint8_t>(std::stoi(item.right_value));
        } else if ((item.opt.propId == 371198722) && (item.right_areaId == 1)) {  // 371198722:车门
            carDoorValue[0] = static_cast<uint8_t>(std::stoi(item.right_value));
        } else if ((item.opt.propId == 371198722) && (item.right_areaId == 4)) {
            carDoorValue[1] = static_cast<uint8_t>(std::stoi(item.right_value));
        } else if ((item.opt.propId == 371198722) && (item.right_areaId == 16)) {
            carDoorValue[2] = static_cast<uint8_t>(std::stoi(item.right_value));
        } else if ((item.opt.propId == 371198722) && (item.right_areaId == 64)) {
            carDoorValue[3] = static_cast<uint8_t>(std::stoi(item.right_value));
        } else if (item.opt.propId == 557850752) {  // 557850752 :后备门
            carDoorValue[4] = static_cast<uint8_t>(std::stoi(item.right_value));
        } else if (item.opt.propId == 557850753) {  // 557850753: 左前门锁
            carDoorValue[5] = static_cast<uint8_t>(std::stoi(item.right_value));
        } else if (item.opt.propId == 557850750) {  // 557850750: 防盗状态
            carStatusValue[0] = static_cast<uint8_t>(std::stoi(item.right_value));
        } else if (item.opt.propId == 557850647) {  // 557850647: 高压状态
            carStatusValue[1] = static_cast<uint8_t>(std::stoi(item.right_value));
        } else if (item.opt.propId == 557850710) {  // 557850710: 程序版本切换控制状态
            carStatusValue[2] = static_cast<uint8_t>(std::stoi(item.right_value));
        } else if (item.opt.propId == 557850757) {  // 557850757: 锁车控制标志
            carStatusValue[3] = static_cast<uint8_t>(std::stoi(item.right_value));
        } else if (item.opt.propId == 557850766) {  // 557850766: 锁车控制状态
            carStatusValue[4] = static_cast<uint8_t>(std::stoi(item.right_value));
        } else if (item.opt.propId == 557850755) {  // 557850755: 动力电池保养维护标志
            carStatusValue[8] = static_cast<uint8_t>(std::stoi(item.right_value));
            customInfoValue[54] = static_cast<uint8_t>(std::stoi(item.right_value));
        } else if (item.opt.propId == 557850756) {  // 557850756: 动力电池保养维护状态
            carStatusValue[9] = static_cast<uint8_t>(std::stoi(item.right_value));
            customInfoValue[55] = static_cast<uint8_t>(std::stoi(item.right_value));
        } else if (item.opt.propId == 291504904) {  // 291504904: 剩余续航里程
            carStatusValue[10] = (static_cast<uint16_t>(std::stoi(item.right_value)) >> 8) & 0xFF;
            carStatusValue[11] = (static_cast<uint16_t>(std::stoi(item.right_value))) & 0xFF;
        } else if (item.opt.propId == 291504905) {  // 291504905: 剩余电量
            carStatusValue[12] = static_cast<uint8_t>(std::stoi(item.right_value));
        } else if (item.opt.propId == 557850751) {  // 557850751: 充/放电枪连接状态
            carStatusValue[13] = static_cast<uint8_t>(std::stoi(item.right_value));
        } /*else if (item.opt.propId == 557850624) {                                    //
         557850624: 剩余充电时间 carStatusValue[16] =
         (static_cast<uint16_t>(std::stoi(item.right_value))>> 8) & 0xFF; carStatusValue[17] =
         (static_cast<uint16_t>(std::stoi(item.right_value))) & 0xFF; } else if (item.opt.propId ==
         559947778) {                                    // 559947778: 充电电流 carStatusValue[18] =
         static_cast<uint8_t>(std::stoi(item.right_value)); } else if (item.opt.propId == 559947777)
         {                                    // 559947777: 充电电压 carStatusValue[19] =
         (static_cast<uint16_t>(std::stoi(item.right_value))>> 8) & 0xFF; carStatusValue[20] =
         (static_cast<uint16_t>(std::stoi(item.right_value))) & 0xFF;
         }*/
        else if ((item.opt.propId == 392168201) &&
                 (item.right_areaId == 1)) {  // 392168201: 左前轮胎气压
            carStatusValue[21] = static_cast<uint8_t>(std::stoi(item.right_value));
        } else if ((item.opt.propId == 392168201) &&
                   (item.right_areaId == 2)) {  // 392168201: 右前轮胎气压
            carStatusValue[22] = static_cast<uint8_t>(std::stoi(item.right_value));
        } else if ((item.opt.propId == 392168201) &&
                   (item.right_areaId == 4)) {  // 392168201: 左后轮胎气压
            carStatusValue[23] = static_cast<uint8_t>(std::stoi(item.right_value));
        } else if ((item.opt.propId == 392168201) &&
                   (item.right_areaId == 8)) {  // 392168201: 右后轮胎气压
            carStatusValue[24] = static_cast<uint8_t>(std::stoi(item.right_value));
        } else if (item.opt.propId == 557850720) {  // 557850720: 动力电池实时许用放电功率
            dataCollectionValue[0] =
                    (static_cast<uint16_t>(std::stoi(item.right_value)) >> 8) & 0xFF;
            dataCollectionValue[1] = (static_cast<uint16_t>(std::stoi(item.right_value))) & 0xFF;
        } else if (item.opt.propId == 557850721) {  // 557850721: BMS自检状态（BMS初始化状态）
            dataCollectionValue[2] = static_cast<uint8_t>(std::stoi(item.right_value));
        } else if (item.opt.propId == 557850722) {  // 557850722: BMS运行模式
            dataCollectionValue[3] = static_cast<uint8_t>(std::stoi(item.right_value));
        } else if (item.opt.propId == 557850723) {  // 557850723: 动力电池当前电量
            dataCollectionValue[4] =
                    (static_cast<uint16_t>(std::stoi(item.right_value)) >> 8) & 0xFF;
            dataCollectionValue[5] = (static_cast<uint16_t>(std::stoi(item.right_value))) & 0xFF;
        } else if (item.opt.propId == 557850712) {  // 557850712: 本次（实时）充电电量
            dataCollectionValue[6] =
                    (static_cast<uint16_t>(std::stoi(item.right_value)) >> 8) & 0xFF;
            dataCollectionValue[7] = (static_cast<uint16_t>(std::stoi(item.right_value))) & 0xFF;
        } else if (item.opt.propId == 557850724) {  // 557850724: SOH（电池健康度）
            dataCollectionValue[8] =
                    (static_cast<uint16_t>(std::stoi(item.right_value)) >> 8) & 0xFF;
            dataCollectionValue[9] = (static_cast<uint16_t>(std::stoi(item.right_value))) & 0xFF;
        } else if (item.opt.propId == 557850725) {  // 557850725: BMS正极接触器状态
            dataCollectionValue[10] = static_cast<uint8_t>(std::stoi(item.right_value));
        } else if (item.opt.propId == 557850726) {  // 557850726: BMS负极接触器状态
            dataCollectionValue[11] = static_cast<uint8_t>(std::stoi(item.right_value));
        } else if (item.opt.propId == 557850727) {  // 557850727: 实时充电电流请求值
            dataCollectionValue[13] =
                    (static_cast<uint16_t>(std::stoi(item.right_value)) >> 8) & 0xFF;
            dataCollectionValue[14] = (static_cast<uint16_t>(std::stoi(item.right_value))) & 0xFF;
        } else if (item.opt.propId == 557850728) {  // 557850728: 实时充电电压请求值
            dataCollectionValue[15] =
                    (static_cast<uint16_t>(std::stoi(item.right_value)) >> 8) & 0xFF;
            dataCollectionValue[16] = (static_cast<uint16_t>(std::stoi(item.right_value))) & 0xFF;
        } else if (item.opt.propId == 557850729) {  // 557850729: 实时直流充电电压
            dataCollectionValue[17] =
                    (static_cast<uint16_t>(std::stoi(item.right_value)) >> 8) & 0xFF;
            dataCollectionValue[18] = (static_cast<uint16_t>(std::stoi(item.right_value))) & 0xFF;
        } else if (item.opt.propId == 557850730) {  // 557850730: 实时直流充电电流
            dataCollectionValue[19] =
                    (static_cast<uint16_t>(std::stoi(item.right_value)) >> 8) & 0xFF;
            dataCollectionValue[20] = (static_cast<uint16_t>(std::stoi(item.right_value))) & 0xFF;
        } else if (item.opt.propId == 557850731) {  // 557850731: 快充接触器控制命令
            dataCollectionValue[21] = static_cast<uint8_t>(std::stoi(item.right_value));
        } else if (item.opt.propId == 557850732) {  // 557850732: 快充口负极触点温度
            dataCollectionValue[22] = static_cast<uint8_t>(std::stoi(item.right_value));
        } else if (item.opt.propId == 557850733) {  // 557850733: 快充口正极触点温度
            dataCollectionValue[23] = static_cast<uint8_t>(std::stoi(item.right_value));
        } else if (item.opt.propId == 557850734) {  // 557850734: DCDC输出电压
            dataCollectionValue[24] =
                    (static_cast<uint16_t>(std::stoi(item.right_value)) >> 8) & 0xFF;
            dataCollectionValue[25] = (static_cast<uint16_t>(std::stoi(item.right_value))) & 0xFF;
        } else if (item.opt.propId == 557850735) {  // 557850735: DCDC输出电流
            dataCollectionValue[26] =
                    (static_cast<uint16_t>(std::stoi(item.right_value)) >> 8) & 0xFF;
            dataCollectionValue[27] = (static_cast<uint16_t>(std::stoi(item.right_value))) & 0xFF;
        } else if (item.opt.propId == 557850736) {  // 557850736: DCDC工作温度
            dataCollectionValue[28] = static_cast<uint8_t>(std::stoi(item.right_value));
        } else if (item.opt.propId == 557850737) {  // 557850737: HCM工作使能状态
            dataCollectionValue[29] = static_cast<uint8_t>(std::stoi(item.right_value));
        } else if (item.opt.propId == 557850719) {  // 557850719: 快充接触器状态
            dataCollectionValue[30] = static_cast<uint8_t>(std::stoi(item.right_value));
        } else if (item.opt.propId == 557850738) {  // 557850738: 高压环路互锁状态
            dataCollectionValue[31] = static_cast<uint8_t>(std::stoi(item.right_value));
        } else if (item.opt.propId == 557850739) {  // 557850739: MCU状态
            dataCollectionValue[32] = static_cast<uint8_t>(std::stoi(item.right_value));
        } else if (item.opt.propId == 557850740) {  // 557850740: CP占空比
            dataCollectionValue[33] = static_cast<uint8_t>(std::stoi(item.right_value));
        } else if (item.opt.propId == 557850741) {  // 557850741: OBC输入充电电流
            dataCollectionValue[34] =
                    (static_cast<uint16_t>(std::stoi(item.right_value)) >> 8) & 0xFF;
            dataCollectionValue[35] = (static_cast<uint16_t>(std::stoi(item.right_value))) & 0xFF;
        } else if (item.opt.propId == 557850742) {  // 557850742: OBC输入充电电压
            dataCollectionValue[36] =
                    (static_cast<uint16_t>(std::stoi(item.right_value)) >> 8) & 0xFF;
            dataCollectionValue[37] = (static_cast<uint16_t>(std::stoi(item.right_value))) & 0xFF;
        } else if (item.opt.propId == 557850743) {  // 557850743: OBC输出充电电流
            dataCollectionValue[38] =
                    (static_cast<uint16_t>(std::stoi(item.right_value)) >> 8) & 0xFF;
            dataCollectionValue[39] = (static_cast<uint16_t>(std::stoi(item.right_value))) & 0xFF;
        } else if (item.opt.propId == 557850744) {  // 557850743: OBC输出充电电压
            dataCollectionValue[40] =
                    (static_cast<uint16_t>(std::stoi(item.right_value)) >> 8) & 0xFF;
            dataCollectionValue[41] = (static_cast<uint16_t>(std::stoi(item.right_value))) & 0xFF;
        } else if (item.opt.propId == 557850714) {  // 557850714: 低压蓄电池电压值
            dataCollectionValue[42] =
                    (static_cast<uint16_t>(std::stoi(item.right_value)) >> 8) & 0xFF;
            dataCollectionValue[43] = (static_cast<uint16_t>(std::stoi(item.right_value))) & 0xFF;
        } else if (item.opt.propId == 557850715) {  // 557850715: VCU对冷却系统开关使能
            dataCollectionValue[44] = static_cast<uint8_t>(std::stoi(item.right_value));
        } else if (item.opt.propId == 557850747) {  // 557850747: VCU对MCU请求转矩
            dataCollectionValue[45] =
                    (static_cast<uint16_t>(std::stoi(item.right_value)) >> 8) & 0xFF;
            dataCollectionValue[46] = (static_cast<uint16_t>(std::stoi(item.right_value))) & 0xFF;
        } else if (item.opt.propId == 557850748) {  // 557850748: 空调制冷功率
            dataCollectionValue[83] = static_cast<uint8_t>(std::stoi(item.right_value));
        } else if (item.opt.propId == 557850749) {  // 557850749: 真空泵系统故障标志
            dataCollectionValue[84] = static_cast<uint8_t>(std::stoi(item.right_value));
        } else if (item.opt.propId == 557850710) {  // 557850710: 程序版本切换控制状态
            customInfoValue[9] = static_cast<uint8_t>(std::stoi(item.right_value));
        } else if (item.opt.propId == 557850711) {  // 557850711: 锁车控制状态(VCU 反馈状态)
            customInfoValue[10] = static_cast<uint8_t>(std::stoi(item.right_value));
        } else if (item.opt.propId == 557850712) {  // 557850712: 本次充电电量
            customInfoValue[11] = static_cast<uint8_t>(std::stoi(item.right_value));
        } else if (item.opt.propId == 557850707) {  // 557850707: VCU 版本号
            for (size_t i = 1; i < item.right_value.length() / 4 + 1; i++) {
                std::string subString = item.right_value.substr(-(i * 4), 4);
                uint8_t value = static_cast<uint8_t>(std::stoi(subString, nullptr, 16));
                customInfoValue[20 - i] = value;
            }
        } else if (item.opt.propId == 557850757) {  // 557850757: 锁车控制标志(下行指令输入)
            customInfoValue[21] = static_cast<uint8_t>(std::stoi(item.right_value));
        } else if (item.opt.propId == 557850713) {  // 557850713: 车辆 ON信号
            customInfoValue[26] = static_cast<uint8_t>(std::stoi(item.right_value));
        } else if (item.opt.propId == 557850714) {  // 557850714: 车辆小电瓶电压
            customInfoValue[30] = (static_cast<uint16_t>(std::stoi(item.right_value)) >> 8) & 0xFF;
            customInfoValue[31] = (static_cast<uint16_t>(std::stoi(item.right_value))) & 0xFF;
        } else if (item.opt.propId == 557850715) {  // 557850715: 水泵故障
            customInfoValue[45] = static_cast<uint8_t>(std::stoi(item.right_value));
        } else if (item.opt.propId == 557850716) {  // 557850716: PTC 维电器实际状态
            customInfoValue[49] = static_cast<uint8_t>(std::stoi(item.right_value));
        } else if (item.opt.propId == 557850718) {  // 557850718: 动力电池加热继电器状态
            customInfoValue[50] = static_cast<uint8_t>(std::stoi(item.right_value));
        } else if (item.opt.propId == 557850719) {  // 557850719: 快充继电器粘连故障
            customInfoValue[51] = static_cast<uint8_t>(std::stoi(item.right_value));
        } else if (item.opt.propId == 289408001) {  // 289408001: 手刹状态
            customInfoValue[53] = static_cast<uint8_t>(std::stoi(item.right_value));
        }
    }

    std::fill(carDoorValue.begin() + 6, carDoorValue.end(), 0xFF);
}
std::vector<uint8_t> CustomInfoMessage::getHvacValue() {
    std::vector<uint8_t> dataPacket;
    dataPacket.push_back(customHvacId);          // 0x92:空调
    dataPacket.push_back((0x0009 >> 8) & 0xFF);  // 0x0009 长度9
    dataPacket.push_back(0x0009 & 0xFF);
    dataPacket.insert(dataPacket.end(), hvacValue.begin(), hvacValue.end());
    dataPacket.push_back(0xff);  // 0xff 协议预留
    return dataPacket;
}

std::vector<uint8_t> CustomInfoMessage::getVehicleDoorData() {
    std::vector<uint8_t> dataPacket;
    dataPacket.push_back(customCarDoorId);       // 0x91:车门
    dataPacket.push_back((0x0010 >> 8) & 0xFF);  // 0x0010 长度16
    dataPacket.push_back(0x0010 & 0xFF);
    dataPacket.insert(dataPacket.end(), carDoorValue.begin(), carDoorValue.end());
    return dataPacket;
}

std::vector<uint8_t> CustomInfoMessage::getVehicleStatusData() {
    std::vector<uint8_t> dataPacket;
    dataPacket.push_back(customVehicleStatusId);  // 0x90:整车状态
    dataPacket.push_back((0x001D >> 8) & 0xFF);   // 0x001d 长度29
    dataPacket.push_back(0x001D & 0xFF);
    dataPacket.insert(dataPacket.end(), carStatusValue.begin(), carStatusValue.end());
    return dataPacket;
}

std::vector<uint8_t> CustomInfoMessage::getDataCollectionData() {
    std::vector<uint8_t> dataPacket;
    dataPacket.push_back(dataCollectionFlagId);  // 0x8B:自定义数据采集
    dataPacket.push_back((0x0055 >> 8) & 0xFF);  // 0x0055 长度85
    dataPacket.push_back(0x0055 & 0xFF);
    dataPacket.insert(dataPacket.end(), dataCollectionValue.begin(), dataCollectionValue.end());
    return dataPacket;
}

std::vector<uint8_t> CustomInfoMessage::getCustomInfoData() {
    std::vector<uint8_t> dataPacket;
    dataPacket.push_back(customInfoFlagsId);     // 0x8A:自定义信息
    dataPacket.push_back((0x0039 >> 8) & 0xFF);  // 0x0039 长度57
    dataPacket.push_back(0x0039 & 0xFF);
    dataPacket.insert(dataPacket.end(), customInfoValue.begin(), customInfoValue.end());
    return dataPacket;
}

CustomVehicleStatusMessage::CustomVehicleStatusMessage(
        uint16_t headerData, uint8_t antiTheft, uint8_t highPressure, uint8_t programVersionSwitch,
        uint8_t lockCarCtrlFlag, uint8_t lockCarCtrlStatus, uint8_t maintenanceTips,
        uint8_t paymentTips, uint8_t annualReviewTips, uint8_t powerBatteryMaintenanceFlag,
        uint8_t powerBatteryMaintenancestatus, uint16_t remainingCruisingRange,
        uint8_t remainingPower, uint8_t chargeGunConnectStatus, uint8_t chargeStatus,
        uint8_t chargingTargetPower, uint16_t remainingChargeTime, uint8_t chargingCurrent,
        uint16_t chargingVoltage, uint8_t leftFrontTirePressure, uint8_t rightFrontTirePressure,
        uint8_t leftRearTirePressure, uint8_t rightRearTirePressure, uint32_t protocolReserv)
    : headerData(headerData),
      antiTheft(antiTheft),
      highPressure(highPressure),
      programVersionSwitch(programVersionSwitch),
      lockCarCtrlFlag(lockCarCtrlFlag),
      lockCarCtrlStatus(lockCarCtrlStatus),
      maintenanceTips(maintenanceTips),
      paymentTips(paymentTips),
      annualReviewTips(annualReviewTips),
      powerBatteryMaintenanceFlag(powerBatteryMaintenanceFlag),
      powerBatteryMaintenancestatus(powerBatteryMaintenancestatus),
      remainingCruisingRange(remainingCruisingRange),
      remainingPower(remainingPower),
      chargeGunConnectStatus(chargeGunConnectStatus),
      chargeStatus(chargeStatus),
      chargingTargetPower(chargingTargetPower),
      remainingChargeTime(remainingChargeTime),
      chargingCurrent(chargingCurrent),
      chargingVoltage(chargingVoltage),
      leftFrontTirePressure(leftFrontTirePressure),
      rightFrontTirePressure(rightFrontTirePressure),
      leftRearTirePressure(leftRearTirePressure),
      rightRearTirePressure(rightRearTirePressure),
      protocolReserv(protocolReserv) {}

std::vector<uint8_t> CustomVehicleStatusMessage::generateDataPacket() {
    std::vector<uint8_t> dataPacket;
    dataPacket.push_back(customVehicleStatusId);
    dataPacket.push_back((headerData >> 8) & 0xFF);
    dataPacket.push_back(headerData & 0xFF);
    dataPacket.push_back(antiTheft);
    dataPacket.push_back(highPressure);
    dataPacket.push_back(programVersionSwitch);
    dataPacket.push_back(lockCarCtrlFlag);
    dataPacket.push_back(lockCarCtrlStatus);
    dataPacket.push_back(maintenanceTips);
    dataPacket.push_back(paymentTips);
    dataPacket.push_back(annualReviewTips);
    dataPacket.push_back(powerBatteryMaintenanceFlag);
    dataPacket.push_back(powerBatteryMaintenancestatus);
    dataPacket.push_back((remainingCruisingRange >> 8) & 0xFF);
    dataPacket.push_back(remainingCruisingRange & 0xFF);
    dataPacket.push_back(remainingPower);
    dataPacket.push_back(chargeGunConnectStatus);
    dataPacket.push_back(chargeStatus);
    dataPacket.push_back(chargingTargetPower);
    dataPacket.push_back((remainingChargeTime >> 8) & 0xFF);
    dataPacket.push_back(remainingChargeTime & 0xFF);
    dataPacket.push_back(chargingCurrent);
    dataPacket.push_back((chargingVoltage >> 8) & 0xFF);
    dataPacket.push_back(chargingVoltage & 0xFF);
    dataPacket.push_back(leftFrontTirePressure);
    dataPacket.push_back(rightFrontTirePressure);
    dataPacket.push_back(leftRearTirePressure);
    dataPacket.push_back(rightRearTirePressure);
    dataPacket.push_back((protocolReserv >> 24) & 0xFF);
    dataPacket.push_back((protocolReserv >> 16) & 0xFF);
    dataPacket.push_back((protocolReserv >> 8) & 0xFF);
    dataPacket.push_back(protocolReserv & 0xFF);

    return dataPacket;
}

void CustomVehicleStatusMessage::printMessage() {
    /*std::cout << "防盗状态: " << antiTheft << std::endl;
    std::cout << "高压状态: " << highPressure << std::endl;
    std::cout << "程序版本切换控制状态: " << programVersionSwitch << std::endl;
    std::cout << "锁车控制标志: " << lockCarCtrlFlag << std::endl;
    std::cout << "锁车控制状态: " << lockCarCtrlStatus << std::endl;
    std::cout << "保养提示: " << maintenanceTips << std::endl;
    std::cout << "缴费提示: " << paymentTips << std::endl;
    std::cout << "年审提示: " << annualReviewTips << std::endl;
    std::cout << "动力电池维护标志: " << powerBatteryMaintenanceFlag << std::endl;
    std::cout << "动力电池维护状态: " << powerBatteryMaintenancestatus << std::endl;
    std::cout << "剩余行驶里程: " << remainingCruisingRange << std::endl;
    std::cout << "剩余电量: " << remainingPower << std::endl;
    std::cout << "电枪连接状态: " << chargeGunConnectStatus << std::endl;
    std::cout << "充电状态: " << chargeStatus << std::endl;
    std::cout << "充电目标电量: " << chargingTargetPower << std::endl;
    std::cout << "充电剩余时间: " << remainingChargeTime << std::endl;
    std::cout << "充电电流: " << chargingCurrent << std::endl;
    std::cout << "充电电压: " << chargingVoltage << std::endl;
    std::cout << "左前轮胎气压: " << leftFrontTirePressure << std::endl;
    std::cout << "右前轮胎气压: " << rightFrontTirePressure << std::endl;
    std::cout << "左后轮胎气压: " << leftRearTirePressure << std::endl;
    std::cout << "右后轮胎气压: " << rightRearTirePressure << std::endl;
    std::cout << "协议预留: " << protocolReserv << std::endl;*/
    LOG(INFO) << "防盗状态: " << antiTheft << std::endl;
    LOG(INFO) << "高压状态: " << highPressure << std::endl;
    LOG(INFO) << "程序版本切换控制状态: " << programVersionSwitch << std::endl;
    LOG(INFO) << "锁车控制标志: " << lockCarCtrlFlag << std::endl;
    LOG(INFO) << "锁车控制状态: " << lockCarCtrlStatus << std::endl;
    LOG(INFO) << "保养提示: " << maintenanceTips << std::endl;
    LOG(INFO) << "缴费提示: " << paymentTips << std::endl;
    LOG(INFO) << "年审提示: " << annualReviewTips << std::endl;
    LOG(INFO) << "动力电池维护标志: " << powerBatteryMaintenanceFlag << std::endl;
    LOG(INFO) << "动力电池维护状态: " << powerBatteryMaintenancestatus << std::endl;
    LOG(INFO) << "剩余行驶里程: " << remainingCruisingRange << std::endl;
    LOG(INFO) << "剩余电量: " << remainingPower << std::endl;
    LOG(INFO) << "电枪连接状态: " << chargeGunConnectStatus << std::endl;
    LOG(INFO) << "充电状态: " << chargeStatus << std::endl;
    LOG(INFO) << "充电目标电量: " << chargingTargetPower << std::endl;
    LOG(INFO) << "充电剩余时间: " << remainingChargeTime << std::endl;
    LOG(INFO) << "充电电流: " << chargingCurrent << std::endl;
    LOG(INFO) << "充电电压: " << chargingVoltage << std::endl;
    LOG(INFO) << "左前轮胎气压: " << leftFrontTirePressure << std::endl;
    LOG(INFO) << "右前轮胎气压: " << rightFrontTirePressure << std::endl;
    LOG(INFO) << "左后轮胎气压: " << leftRearTirePressure << std::endl;
    LOG(INFO) << "右后轮胎气压: " << rightRearTirePressure << std::endl;
    LOG(INFO) << "协议预留: " << protocolReserv << std::endl;
}

CustomDataCollectionFlags::CustomDataCollectionFlags(
        uint16_t headerData, uint16_t RealtimeDischargePower, uint8_t bmsSelftestStatus,
        uint8_t bmsOperationMode, uint16_t powerBatteryRealtimePower,
        uint16_t RealtimeChargingPower, uint16_t SOH, uint8_t bmsPositiveContactorStatus,
        uint8_t bmsNegativeContactorStatus, uint8_t chargingMode,
        uint16_t RealtimeChargingCurrentRequest, uint16_t RealtimeChargingVoltageRequest,
        uint16_t RealtimeDCChargingVoltage, uint16_t RealtimeDCChargingCurrent,
        uint8_t fastChargeContactorCtrlCmd, uint8_t fastChargeNegaTouchpointTemp,
        uint8_t fastChargeposiTouchpointTemp, uint16_t dcdcOutputVoltage,
        uint16_t dcdcOutputCurrent, uint8_t dcdcOperatingTemp, uint8_t hcmWorkState,
        uint8_t fastChargeContactorStatus, uint8_t highVoltageLoopInterlockState, uint8_t mcuStatus,
        uint8_t cpDutyCycle, uint16_t obcInputChargingCurrent, uint16_t obcInputChargingVoltage,
        uint16_t obcOutputChargingCurrent, uint16_t obcOutputChargingVoltage,
        uint16_t lowVoltageBatteryVoltageValue, uint8_t vcuCoolingSystemSwitch,
        uint16_t vcuRequestTorqueToMcu, uint8_t indicatorPowerSystem,
        uint64_t vcuHardwareVersionNumber, uint64_t vcuSoftwareVersionNumber,
        uint64_t bmsSoftwareVersionNumber, uint64_t obcHardwareVersionNumber,
        uint64_t obcSoftwareVersionNumber, uint8_t aircondRefrigerationPower,
        uint8_t vacuumPumpSystemFaultFlag)
    : headerData(headerData),
      RealtimeDischargePower(RealtimeDischargePower),
      bmsSelftestStatus(bmsSelftestStatus),
      bmsOperationMode(bmsOperationMode),
      powerBatteryRealtimePower(powerBatteryRealtimePower),
      RealtimeChargingPower(RealtimeChargingPower),
      SOH(SOH),
      bmsPositiveContactorStatus(bmsPositiveContactorStatus),
      bmsNegativeContactorStatus(bmsNegativeContactorStatus),
      chargingMode(chargingMode),
      RealtimeChargingCurrentRequest(RealtimeChargingCurrentRequest),
      RealtimeChargingVoltageRequest(RealtimeChargingVoltageRequest),
      RealtimeDCChargingVoltage(RealtimeDCChargingVoltage),
      RealtimeDCChargingCurrent(RealtimeDCChargingCurrent),
      fastChargeContactorCtrlCmd(fastChargeContactorCtrlCmd),
      fastChargeNegaTouchpointTemp(fastChargeNegaTouchpointTemp),
      fastChargeposiTouchpointTemp(fastChargeposiTouchpointTemp),
      dcdcOutputVoltage(dcdcOutputVoltage),
      dcdcOutputCurrent(dcdcOutputCurrent),
      dcdcOperatingTemp(dcdcOperatingTemp),
      hcmWorkState(hcmWorkState),
      fastChargeContactorStatus(fastChargeContactorStatus),
      highVoltageLoopInterlockState(highVoltageLoopInterlockState),
      mcuStatus(mcuStatus),
      cpDutyCycle(cpDutyCycle),
      obcInputChargingCurrent(obcInputChargingCurrent),
      obcInputChargingVoltage(obcInputChargingVoltage),
      obcOutputChargingCurrent(obcOutputChargingCurrent),
      obcOutputChargingVoltage(obcOutputChargingVoltage),
      lowVoltageBatteryVoltageValue(lowVoltageBatteryVoltageValue),
      vcuCoolingSystemSwitch(vcuCoolingSystemSwitch),
      vcuRequestTorqueToMcu(vcuRequestTorqueToMcu),
      indicatorPowerSystem(indicatorPowerSystem),
      vcuHardwareVersionNumber(vcuHardwareVersionNumber),
      vcuSoftwareVersionNumber(vcuSoftwareVersionNumber),
      bmsSoftwareVersionNumber(bmsSoftwareVersionNumber),
      obcHardwareVersionNumber(obcHardwareVersionNumber),
      obcSoftwareVersionNumber(obcSoftwareVersionNumber),
      aircondRefrigerationPower(aircondRefrigerationPower),
      vacuumPumpSystemFaultFlag(vacuumPumpSystemFaultFlag) {}

std::vector<uint8_t> CustomDataCollectionFlags::generateDataPacket() {
    std::vector<uint8_t> dataPacket;
    dataPacket.push_back(customDataCollectionFlagsId);
    dataPacket.push_back((headerData >> 8) & 0xFF);
    dataPacket.push_back(headerData & 0xFF);
    dataPacket.push_back((RealtimeDischargePower >> 8) & 0xFF);
    dataPacket.push_back(RealtimeDischargePower & 0xFF);
    dataPacket.push_back(bmsSelftestStatus);
    dataPacket.push_back(bmsOperationMode);
    dataPacket.push_back((powerBatteryRealtimePower >> 8) & 0xFF);
    dataPacket.push_back(powerBatteryRealtimePower & 0xFF);
    dataPacket.push_back((RealtimeChargingPower >> 8) & 0xFF);
    dataPacket.push_back(RealtimeChargingPower & 0xFF);
    dataPacket.push_back((SOH >> 8) & 0xFF);
    dataPacket.push_back(SOH & 0xFF);
    dataPacket.push_back(bmsPositiveContactorStatus);
    dataPacket.push_back(bmsNegativeContactorStatus);
    dataPacket.push_back(chargingMode);
    dataPacket.push_back((RealtimeChargingCurrentRequest >> 8) & 0xFF);
    dataPacket.push_back(RealtimeChargingCurrentRequest & 0xFF);
    dataPacket.push_back((RealtimeChargingVoltageRequest >> 8) & 0xFF);
    dataPacket.push_back(RealtimeChargingVoltageRequest & 0xFF);
    dataPacket.push_back((RealtimeDCChargingVoltage >> 8) & 0xFF);
    dataPacket.push_back(RealtimeDCChargingVoltage & 0xFF);
    dataPacket.push_back((RealtimeDCChargingCurrent >> 8) & 0xFF);
    dataPacket.push_back(RealtimeDCChargingCurrent & 0xFF);
    dataPacket.push_back(fastChargeContactorCtrlCmd);
    dataPacket.push_back(fastChargeNegaTouchpointTemp);
    dataPacket.push_back(fastChargeposiTouchpointTemp);
    dataPacket.push_back((dcdcOutputVoltage >> 8) & 0xFF);
    dataPacket.push_back(dcdcOutputVoltage & 0xFF);
    dataPacket.push_back((dcdcOutputCurrent >> 8) & 0xFF);
    dataPacket.push_back(dcdcOutputCurrent & 0xFF);
    dataPacket.push_back(dcdcOperatingTemp);
    dataPacket.push_back(hcmWorkState);
    dataPacket.push_back(fastChargeContactorStatus);
    dataPacket.push_back(highVoltageLoopInterlockState);
    dataPacket.push_back(mcuStatus);
    dataPacket.push_back(cpDutyCycle);
    dataPacket.push_back((obcInputChargingCurrent >> 8) & 0xFF);
    dataPacket.push_back(obcInputChargingCurrent & 0xFF);
    dataPacket.push_back((obcInputChargingVoltage >> 8) & 0xFF);
    dataPacket.push_back(obcInputChargingVoltage & 0xFF);
    dataPacket.push_back((obcOutputChargingCurrent >> 8) & 0xFF);
    dataPacket.push_back(obcOutputChargingCurrent & 0xFF);
    dataPacket.push_back((obcOutputChargingVoltage >> 8) & 0xFF);
    dataPacket.push_back(obcOutputChargingVoltage & 0xFF);
    dataPacket.push_back((lowVoltageBatteryVoltageValue >> 8) & 0xFF);
    dataPacket.push_back(lowVoltageBatteryVoltageValue & 0xFF);
    dataPacket.push_back(vcuCoolingSystemSwitch);
    dataPacket.push_back((vcuRequestTorqueToMcu >> 8) & 0xFF);
    dataPacket.push_back(vcuRequestTorqueToMcu & 0xFF);
    dataPacket.push_back(indicatorPowerSystem);
    for (int i = 6; i >= 0; --i) {
        dataPacket.push_back((vcuHardwareVersionNumber >> (i * 8)) & 0xFF);
    }
    for (int i = 6; i >= 0; --i) {
        dataPacket.push_back((vcuSoftwareVersionNumber >> (i * 8)) & 0xFF);
    }
    for (int i = 6; i >= 0; --i) {
        dataPacket.push_back((bmsSoftwareVersionNumber >> (i * 8)) & 0xFF);
    }
    for (int i = 6; i >= 0; --i) {
        dataPacket.push_back((obcHardwareVersionNumber >> (i * 8)) & 0xFF);
    }
    for (int i = 6; i >= 0; --i) {
        dataPacket.push_back((obcSoftwareVersionNumber >> (i * 8)) & 0xFF);
    }
    dataPacket.push_back(aircondRefrigerationPower);
    dataPacket.push_back(vacuumPumpSystemFaultFlag);

    return dataPacket;
}

void CustomDataCollectionFlags::printMessage() {
    /*std::cout << "动力电池实时许用放电功率: " << RealtimeDischargePower << std::endl;
    std::cout << "BMS自检状态: " << bmsSelftestStatus << std::endl;
    std::cout << "BMS运行模式: " << bmsOperationMode << std::endl;
    std::cout << "动力电池当前电量: " << powerBatteryRealtimePower << std::endl;
    std::cout << "本次充电电量: " << RealtimeChargingPower << std::endl;
    std::cout << "SOH: " << SOH << std::endl;
    std::cout << "BMS正极接触器状态: " << bmsPositiveContactorStatus << std::endl;
    std::cout << "BMS负极接触器状态: " << bmsNegativeContactorStatus << std::endl;
    std::cout << "充电模式: " << chargingMode << std::endl;
    std::cout << "实时充电电流请求值: " << RealtimeChargingCurrentRequest << std::endl;
    std::cout << "实时充电电压请求值: " << RealtimeChargingVoltageRequest << std::endl;*/
    LOG(INFO) << "动力电池实时许用放电功率: " << RealtimeDischargePower;
    LOG(INFO) << "BMS自检状态: " << bmsSelftestStatus;
    LOG(INFO) << "BMS运行模式: " << bmsOperationMode;
    LOG(INFO) << "动力电池当前电量: " << powerBatteryRealtimePower;
    LOG(INFO) << "本次充电电量: " << RealtimeChargingPower;
    LOG(INFO) << "SOH: " << SOH;
    LOG(INFO) << "BMS正极接触器状态: " << bmsPositiveContactorStatus;
    LOG(INFO) << "BMS负极接触器状态: " << bmsNegativeContactorStatus;
    LOG(INFO) << "充电模式: " << chargingMode;
    LOG(INFO) << "实时充电电流请求值: " << RealtimeChargingCurrentRequest;
    LOG(INFO) << "实时充电电压请求值: " << RealtimeChargingVoltageRequest;
}

