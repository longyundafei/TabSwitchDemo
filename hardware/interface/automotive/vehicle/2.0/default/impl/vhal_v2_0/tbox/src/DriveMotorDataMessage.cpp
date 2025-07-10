#include "DriveMotorDataMessage.h"
#include <iostream>
DriveMotorDataMessage::DriveMotorDataMessage(LockMap<int32_t, TboxDataDeclaration>& tboxdecl,VehicleController& vehicle):RealTimeMessageBase(tboxdecl,vehicle){
    MotorData motorData;
    motorData.motorNumber = 1;
    motorDataList.push_back(motorData);
}
uint32_t DriveMotorDataMessage::getmotorStatusValue() {
    return getValue(toInt(VehicleProperty::GECKO_VCU_MCU_ERR));
}
uint32_t DriveMotorDataMessage::getcontrollerTemperatureValue() {
    return getRawValue(toInt(VehicleProperty::GECKO_MCU_IGBTTEMP));
}
uint32_t DriveMotorDataMessage::getmotorSpeedValue() {
    return getValue(toInt(VehicleProperty::ENGINE_RPM));
}
uint32_t DriveMotorDataMessage::getmotorTorqueValue() {
    return getValue(toInt(VehicleProperty::GECKO_MCU_MOTORTORQUE));
}
uint32_t DriveMotorDataMessage::getmotorTemperatureValue() {
    return getValue(toInt(VehicleProperty::GECKO_ENGINE_TEMP));
}
uint32_t DriveMotorDataMessage::getcontrollerInputVoltageValue() {
    return getValue(toInt(VehicleProperty::GECKO_MCU_BUSVOLT));
}
uint32_t DriveMotorDataMessage::getcontrollerBusCurrentValue() {
    return getValue(toInt(VehicleProperty::GECKO_MCU_BUSCURRENT));
}
std::vector<uint8_t> DriveMotorDataMessage::generateDataPacket() {
    std::vector<uint8_t> dataPacket;

    // 添加信息id
    dataPacket.push_back(drivemotorid);

    // 添加驱动电机个数
    dataPacket.push_back(motorCount);

    // 添加每个驱动电机总成信息
    for (auto& motorData : motorDataList) {
        // 添加驱动电机序号
        dataPacket.push_back(motorData.motorNumber);

        motorData.motorStatus = getmotorStatusValue();
        // 添加驱动电机状态
        dataPacket.push_back(motorData.motorStatus);

        uint32_t temperature = getcontrollerTemperatureValue();
        if((temperature <= 255) && (temperature >= 0)){
            motorData.controllerTemperature = temperature;
        }
        // 添加驱动电机控制器温度
        dataPacket.push_back(motorData.controllerTemperature);

        motorData.motorSpeed = getmotorSpeedValue();
        // 添加驱动电机转速
        dataPacket.push_back((motorData.motorSpeed >> 8) & 0xFF);
        dataPacket.push_back(motorData.motorSpeed & 0xFF);

        motorData.motorTorque = getmotorTorqueValue();
        // 添加驱动电机转矩
        dataPacket.push_back((motorData.motorTorque >> 8) & 0xFF);
        dataPacket.push_back(motorData.motorTorque & 0xFF);

        motorData.motorTemperature = getmotorTemperatureValue();
//        motorData.motorTemperature = getmotorTemperatureValue(559947794);
        // 添加驱动电机温度
        dataPacket.push_back(motorData.motorTemperature);

        motorData.controllerInputVoltage = getcontrollerInputVoltageValue();
        // 添加电机控制器输入电压
        dataPacket.push_back((motorData.controllerInputVoltage >> 8) & 0xFF);
        dataPacket.push_back(motorData.controllerInputVoltage & 0xFF);

        motorData.controllerBusCurrent = getcontrollerBusCurrentValue();
        // 添加电机控制器母线电流
        dataPacket.push_back((motorData.controllerBusCurrent >> 8) & 0xFF);
        dataPacket.push_back(motorData.controllerBusCurrent & 0xFF);
    }

    return dataPacket;
}

void DriveMotorDataMessage::printMessage() {
    /*std::cout << "信息ID: " << static_cast<int>(drivemotorid) << std::endl;
    std::cout << "驱动电机个数: " << static_cast<int>(motorCount) << std::endl;

    for (const auto& motorData : motorDataList) {
        std::cout << "驱动电机序号: " << static_cast<int>(motorData.motorNumber) << std::endl;
        std::cout << "驱动电机状态: " << static_cast<int>(motorData.motorStatus) << std::endl;
        std::cout << "驱动电机控制器温度: " << static_cast<int>(motorData.controllerTemperature)
                  << std::endl;
        std::cout << "驱动电机转速: " << motorData.motorSpeed << " r/min" << std::endl;
        std::cout << "驱动电机转矩: " << motorData.motorTorque / 10.0 << " N·m" << std::endl;
        std::cout << "驱动电机温度: " << static_cast<int>(motorData.motorTemperature) << std::endl;
        std::cout << "电机控制器输入电压: " << motorData.controllerInputVoltage / 10.0 << " V"
                  << std::endl;
        std::cout << "电机控制器母线电流: " << motorData.controllerBusCurrent / 10.0 << " A"
                  << std::endl;
    }*/

    LOG(INFO) << "信息ID: " << static_cast<int>(drivemotorid);
    LOG(INFO)  << "驱动电机个数: " << static_cast<int>(motorCount);

    for (const auto& motorData : motorDataList) {
        LOG(INFO)  << "驱动电机序号: " << static_cast<int>(motorData.motorNumber);
        LOG(INFO)  << "驱动电机状态: " << static_cast<int>(motorData.motorStatus);
        LOG(INFO)  << "驱动电机控制器温度: " << static_cast<int>(motorData.controllerTemperature);
        LOG(INFO)  << "驱动电机转速: " << motorData.motorSpeed << " r/min";
        LOG(INFO)  << "驱动电机转矩: " << motorData.motorTorque / 10.0 << " N·m";
        LOG(INFO)  << "驱动电机温度: " << static_cast<int>(motorData.motorTemperature);
        LOG(INFO)  << "电机控制器输入电压: " << motorData.controllerInputVoltage / 10.0 << " V";
        LOG(INFO)  << "电机控制器母线电流: " << motorData.controllerBusCurrent / 10.0 << " A";
    }
}
