#include "VehicleInfoMessage.h"
#include <android-base/logging.h>
#include <iostream>
#include <iomanip>
#define PROPERTY_VALUE_MAX_LEN 250
VehicleInfoMessage::VehicleInfoMessage(VehicleController& vehicle) {
    mVehicle = vehicle;
}

std::vector<uint8_t> VehicleInfoMessage::generateDataPacket(const std::string& uniqueID) {
    std::vector<uint8_t> dataPacket;
    // 添加自定义信息长度 2
    uint16_t length = 288;
    dataPacket.push_back((length >> 8) & 0xFF);
    dataPacket.push_back(length & 0xFF);
    //添加车载终端设备型号 1
    uint8_t model = 0x0C;
    dataPacket.push_back(model);
    //添加整车类型 2
    uint16_t vehicleModle = 0x0071;
    dataPacket.push_back((vehicleModle >> 8) & 0xFF);
    dataPacket.push_back(vehicleModle & 0xFF);
    //首次获取VIN时间 6

    LOG(INFO) << "VehicleInfoMessage::generateDataPacket " << year << "," << month << "," << day << "," << hour << "," << minute << "," << second;
    dataPacket.push_back(year);
    dataPacket.push_back(month);
    dataPacket.push_back(day);
    dataPacket.push_back(hour);
    dataPacket.push_back(minute);
    dataPacket.push_back(second);
    //首次获取VIN的经度4
    int32_t longitudeValue = fabs(longitude)*1000000;
    dataPacket.push_back((longitudeValue >> 24) & 0xFF);
    dataPacket.push_back((longitudeValue >> 16) & 0xFF);
    dataPacket.push_back((longitudeValue >> 8) & 0xFF);
    dataPacket.push_back(longitudeValue & 0xFF);
    //首次获取VIN的纬度 4
    int32_t latitudeValue = fabs(latitude)*1000000;
    dataPacket.push_back((latitudeValue >> 24) & 0xFF);
    dataPacket.push_back((latitudeValue >> 16) & 0xFF);
    dataPacket.push_back((latitudeValue >> 8) & 0xFF);
    dataPacket.push_back(latitudeValue & 0xFF);
    //ICCID 20
    char propV[PROPERTY_VALUE_MAX_LEN] = "\0";
    if(property_get("persist.iccid", propV, nullptr)!=0){
        iccid = propV;
    } else {
        LOG(INFO) << "ERROR : can not get iccid!!!";
    }
    std::vector<uint8_t> ICCID = stringToBinary(iccid);
    dataPacket.insert(dataPacket.end(), ICCID.begin(), ICCID.end());
    //VIN 17
    std::vector<uint8_t> vin = stringToBinary(uniqueID);
    dataPacket.insert(dataPacket.end(), vin.begin(), vin.end());
    //IMEI 32
    memset(propV, 0, sizeof(propV));
    if(property_get("persist.imei", propV, nullptr)!=0){
        imei = propV;
    } else {
        LOG(INFO) << "ERROR : can not get imei!!!";
    }
    while(imei.size()<32){
        imei.append("0");
    }
    std::vector<uint8_t> IMEI = stringToBinary(imei);
    dataPacket.insert(dataPacket.end(), IMEI.begin(), IMEI.end());
    //IMSI 32
    memset(propV, 0, sizeof(propV));
    if(property_get("persist.imsi", propV, nullptr)!=0){
        imsi = propV;
    } else {
        LOG(INFO) << "ERROR : can not get imsi!!!";
    }
    while(imsi.size()<32){
        imsi.append("0");
    }
    std::vector<uint8_t> IMSI = stringToBinary(imsi);
    dataPacket.insert(dataPacket.end(), IMSI.begin(), IMSI.end());
    //SIM/N 32
    std::string simn = "0";
    while(simn.size()<32){
        simn.append("0");
    }
	std::vector<uint8_t> SIMN = stringToBinary(simn);
    dataPacket.insert(dataPacket.end(), SIMN.begin(), SIMN.end());
    //SN 32
    std::string sn = "0";
    memset(propV, 0, sizeof(propV));
    if(property_get("gecko.serial.number", propV, nullptr)!=0){
        sn = propV;
    } else {
        LOG(INFO) << "ERROR : can not get sn!!!";
    }
    while(sn.size()<32){
        sn.append("0");
    }
    std::vector<uint8_t> SN = stringToBinary(sn);
    dataPacket.insert(dataPacket.end(), SN.begin(), SN.end());
    //车载终端硬件版本号 7
    StatusCode refStatus;
    VehiclePropValue refValue;
    //mVehicle.getPropertybyPropId((int)VehicleProperty::GECKO_CCM_SOC_HARDWARE_NUM, 0, refValue, refStatus);
    //int socHW = refValue.value.int32Values[0];
	uint8_t IVIHWVer[7]= {'1','.','2','0','.','I','H'};
	dataPacket.push_back(IVIHWVer[6]);
	dataPacket.push_back(IVIHWVer[5]);
	dataPacket.push_back(IVIHWVer[4]);
    dataPacket.push_back(IVIHWVer[3]);
    dataPacket.push_back(IVIHWVer[2]);
    dataPacket.push_back(IVIHWVer[1]);
    dataPacket.push_back(IVIHWVer[0]);
    //车载终端软件版本号 7 从ro.build.id中获取
    std::string socSW;
    memset(propV, 0, sizeof(propV));
    uint8_t IVISWVer[7]= {0};
    if(property_get("ro.build.id", propV, nullptr)!=0){
        socSW = propV;
        LOG(INFO) << "soc sw :" << socSW;
        IVISWVer[6] = static_cast<uint8_t>(socSW[0]);
        IVISWVer[5] = static_cast<uint8_t>(socSW[1]);
        IVISWVer[4] = static_cast<uint8_t>(socSW[3]);
        IVISWVer[3] = static_cast<uint8_t>(socSW[4]);
        IVISWVer[2] = static_cast<uint8_t>(socSW[5]);
        IVISWVer[1] = static_cast<uint8_t>(socSW[6]);
        IVISWVer[0] = static_cast<uint8_t>(socSW[7]);
        LOG(INFO) << "soc sw :" << static_cast<int>(IVISWVer[6])
                                << "," << static_cast<int>(IVISWVer[5])
                                << "," << static_cast<int>(IVISWVer[4])
                                << "," << static_cast<int>(IVISWVer[3])
                                << "," << static_cast<int>(IVISWVer[2])
                                << "," << static_cast<int>(IVISWVer[1])
                                << "," << static_cast<int>(IVISWVer[0]);
    } else {
        LOG(INFO) << "ERROR : can not get gecko.soc.version!!!";
    }
	dataPacket.push_back(IVISWVer[6]);
	dataPacket.push_back(IVISWVer[5]);
	dataPacket.push_back(IVISWVer[4]);
    dataPacket.push_back(IVISWVer[3]);
    dataPacket.push_back(IVISWVer[2]);
    dataPacket.push_back(IVISWVer[1]);
    dataPacket.push_back(IVISWVer[0]);
    //GW硬件版本号 7
    //mVehicle.getPropertybyPropId((int)VehicleProperty::GECKO_CCM_MCU_HARDWARE_NUM, 0, refValue, refStatus);
    //int gwHW = refValue.value.int32Values[0];
	uint8_t GWHWVer[7]= {'3','.','2','0','.','G','H'};
	dataPacket.push_back(GWHWVer[6]);
	dataPacket.push_back(GWHWVer[5]);
	dataPacket.push_back(GWHWVer[4]);
    dataPacket.push_back(GWHWVer[3]);
    dataPacket.push_back(GWHWVer[2]);
    dataPacket.push_back(GWHWVer[1]);
    dataPacket.push_back(GWHWVer[0]);
    //GW软件版本号 7 从gecko.mcu.version中获取
    std::string gwSW;
    memset(propV, 0, sizeof(propV));
    uint8_t GWSWVer[7]= {0};
    if(property_get("gecko.mcu.version", propV, nullptr)!=0){
        gwSW = propV;
        LOG(INFO) << "gw sw :" << gwSW;
        GWSWVer[6] = static_cast<uint8_t>(gwSW[0]);
        GWSWVer[5] = static_cast<uint8_t>(gwSW[1]);
        GWSWVer[4] = static_cast<uint8_t>(gwSW[3]);
        GWSWVer[3] = static_cast<uint8_t>(gwSW[4]);
        GWSWVer[2] = static_cast<uint8_t>(gwSW[5]);
        GWSWVer[1] = static_cast<uint8_t>(gwSW[6]);
        GWSWVer[0] = static_cast<uint8_t>(gwSW[7]);
        LOG(INFO) << "gw sw :" << static_cast<int>(GWSWVer[6])
                                << "," << static_cast<int>(GWSWVer[5])
                                << "," << static_cast<int>(GWSWVer[4])
                                << "," << static_cast<int>(GWSWVer[3])
                                << "," << static_cast<int>(GWSWVer[2])
                                << "," << static_cast<int>(GWSWVer[1])
                                << "," << static_cast<int>(GWSWVer[0]);
    } else {
        LOG(INFO) << "ERROR : can not get mcu!!!";
    }

	dataPacket.push_back(GWSWVer[6]);
	dataPacket.push_back(GWSWVer[5]);
	dataPacket.push_back(GWSWVer[4]);
    dataPacket.push_back(GWSWVer[3]);
    dataPacket.push_back(GWSWVer[2]);
    dataPacket.push_back(GWSWVer[1]);
    dataPacket.push_back(GWSWVer[0]);
    //BMS硬件版本号 7
    mVehicle.getPropertybyPropId((int)VehicleProperty::GECKO_BMS_HARDWAREVERSION, 0, refValue, refStatus);
    int bmsHW = refValue.value.int32Values[0];
	uint8_t BMSHWVer[7] = {0};
    BMSHWVer[6] = static_cast<uint8_t>((bmsHW >> 8) & 0xFF);
    BMSHWVer[5] = static_cast<uint8_t>(bmsHW & 0xFF);
	dataPacket.push_back(BMSHWVer[6]);
	dataPacket.push_back(BMSHWVer[5]);
	dataPacket.push_back(BMSHWVer[4]);
    dataPacket.push_back(BMSHWVer[3]);
    dataPacket.push_back(BMSHWVer[2]);
    dataPacket.push_back(BMSHWVer[1]);
    dataPacket.push_back(BMSHWVer[0]);
    //BMS软件版本号 7
    //mVehicle.getPropertybyPropId((int)VehicleProperty::GECKO_BMS_SOFTWAREVERSION, 0, refValue, refStatus);
    //int bmsSW = refValue.value.int32Values[0];
	uint8_t BMSSWVer[7]= {0};
    //BMSSWVer[6] = static_cast<uint8_t>((bmsSW >> 8) & 0xFF);
    //BMSSWVer[5] = static_cast<uint8_t>(bmsSW & 0xFF);
	dataPacket.push_back(BMSSWVer[6]);
	dataPacket.push_back(BMSSWVer[5]);
	dataPacket.push_back(BMSSWVer[4]);
    dataPacket.push_back(BMSSWVer[3]);
    dataPacket.push_back(BMSSWVer[2]);
    dataPacket.push_back(BMSSWVer[1]);
    dataPacket.push_back(BMSSWVer[0]);
    //MCU硬件版本号 7
    mVehicle.getPropertybyPropId((int)VehicleProperty::GECKO_MCU_HARDWAREVERSION, 0, refValue, refStatus);
    int mcuHW = refValue.value.int32Values[0];
	uint8_t MCUHWVer[7]= {0};
    MCUHWVer[6] = static_cast<uint8_t>((mcuHW >> 16) & 0xFF);
    MCUHWVer[5] = static_cast<uint8_t>((mcuHW >> 8) & 0xFF);
    MCUHWVer[4] = static_cast<uint8_t>(mcuHW & 0xFF);
	dataPacket.push_back(MCUHWVer[6]);
	dataPacket.push_back(MCUHWVer[5]);
	dataPacket.push_back(MCUHWVer[4]);
    dataPacket.push_back(MCUHWVer[3]);
    dataPacket.push_back(MCUHWVer[2]);
    dataPacket.push_back(MCUHWVer[1]);
    dataPacket.push_back(MCUHWVer[0]);
    //MCU软件版本号 7
    mVehicle.getPropertybyPropId((int)VehicleProperty::GECKO_MCU_SOFTWAREVERSION, 0, refValue, refStatus);
    int mcuSW = refValue.value.int32Values[0];
	uint8_t MCUSWVer[7]= {0};
    MCUSWVer[6] = static_cast<uint8_t>((mcuSW >> 16) & 0xFF);
    MCUSWVer[5] = static_cast<uint8_t>((mcuSW >> 8) & 0xFF);
    MCUSWVer[4] = static_cast<uint8_t>(mcuSW & 0xFF);
	dataPacket.push_back(MCUSWVer[6]);
	dataPacket.push_back(MCUSWVer[5]);
	dataPacket.push_back(MCUSWVer[4]);
    dataPacket.push_back(MCUSWVer[3]);
    dataPacket.push_back(MCUSWVer[2]);
    dataPacket.push_back(MCUSWVer[1]);
    dataPacket.push_back(MCUSWVer[0]);
    //IPU 硬件版本号 7
    //mVehicle.getPropertybyPropId((int)VehicleProperty::GECKO_IPU_HARDWARE_NUM, 0, refValue, refStatus);
    //int ipuHW = refValue.value.int32Values[0];
	uint8_t IPUHWVer[7] = {0};
    //IPUHWVer[6] = static_cast<uint8_t>((ipuHW >> 8) & 0xFF);
    //IPUHWVer[5] = static_cast<uint8_t>(ipuHW & 0xFF);
	dataPacket.push_back(IPUHWVer[6]);
	dataPacket.push_back(IPUHWVer[5]);
	dataPacket.push_back(IPUHWVer[4]);
    dataPacket.push_back(IPUHWVer[3]);
    dataPacket.push_back(IPUHWVer[2]);
    dataPacket.push_back(IPUHWVer[1]);
    dataPacket.push_back(IPUHWVer[0]);
    //IPU 软件版本号 7
    //mVehicle.getPropertybyPropId((int)VehicleProperty::GECKO_IPU_SOFEWARE_NUM, 0, refValue, refStatus);
    //int ipuSW = refValue.value.int32Values[0];
	uint8_t IPUSWVer[7] = {0};
    //IPUSWVer[6] = static_cast<uint8_t>((ipuSW >> 8) & 0xFF);
    //IPUSWVer[5] = static_cast<uint8_t>(ipuSW & 0xFF);
	dataPacket.push_back(IPUSWVer[6]);
	dataPacket.push_back(IPUSWVer[5]);
	dataPacket.push_back(IPUSWVer[4]);
    dataPacket.push_back(IPUSWVer[3]);
    dataPacket.push_back(IPUSWVer[2]);
    dataPacket.push_back(IPUSWVer[1]);
    dataPacket.push_back(IPUSWVer[0]);
    //VCU硬件版本号 7
    //mVehicle.getPropertybyPropId((int)VehicleProperty::GECKO_VCU_HARDWARE_NUM, 0, refValue, refStatus);
    //int vcuHW = refValue.value.int32Values[0];
	uint8_t VCUHWVer[7] = {0};
    //VCUHWVer[6] = static_cast<uint8_t>((vcuHW >> 8) & 0xFF);
    //VCUHWVer[5] = static_cast<uint8_t>(vcuHW & 0xFF);
	dataPacket.push_back(VCUHWVer[6]);
	dataPacket.push_back(VCUHWVer[5]);
	dataPacket.push_back(VCUHWVer[4]);
    dataPacket.push_back(VCUHWVer[3]);
    dataPacket.push_back(VCUHWVer[2]);
    dataPacket.push_back(VCUHWVer[1]);
    dataPacket.push_back(VCUHWVer[0]);
    //VCU软件版本号 7
    mVehicle.getPropertybyPropId((int)VehicleProperty::GECKO_VCU_SOFEWARE_NUM, 0, refValue, refStatus);
    int vcuSW = refValue.value.int32Values[0];
	uint8_t VCUSWVer[7] = {0};
    VCUSWVer[6] = static_cast<uint8_t>((vcuSW >> 8) & 0xFF);
    VCUSWVer[5] = static_cast<uint8_t>(vcuSW & 0xFF);
	dataPacket.push_back(VCUSWVer[6]);
	dataPacket.push_back(VCUSWVer[5]);
	dataPacket.push_back(VCUSWVer[4]);
    dataPacket.push_back(VCUSWVer[3]);
    dataPacket.push_back(VCUSWVer[2]);
    dataPacket.push_back(VCUSWVer[1]);
    dataPacket.push_back(VCUSWVer[0]);
    //BCM硬件版本号 7
    mVehicle.getPropertybyPropId((int)VehicleProperty::GECKO_BCM_HARDWARE_NUM, 0, refValue, refStatus);
    int bcmHW = refValue.value.int32Values[0];
	uint8_t BCMHWVer[7] = {0};
    BCMHWVer[6] = static_cast<uint8_t>((bcmHW >> 8) & 0xFF);
    BCMHWVer[5] = static_cast<uint8_t>(bcmHW & 0xFF);
	dataPacket.push_back(BCMHWVer[6]);
	dataPacket.push_back(BCMHWVer[5]);
	dataPacket.push_back(BCMHWVer[4]);
    dataPacket.push_back(BCMHWVer[3]);
    dataPacket.push_back(BCMHWVer[2]);
    dataPacket.push_back(BCMHWVer[1]);
    dataPacket.push_back(BCMHWVer[0]);
    //BCM软件版本号 7
    mVehicle.getPropertybyPropId((int)VehicleProperty::GECKO_BCM_SOFEWARE_NUM, 0, refValue, refStatus);
    int bcmSW = refValue.value.int32Values[0];
	uint8_t BCMSWVer[7] = {0};
    BCMSWVer[6] = static_cast<uint8_t>((bcmSW >> 8) & 0xFF);
    BCMSWVer[5] = static_cast<uint8_t>(bcmSW & 0xFF);
	dataPacket.push_back(BCMSWVer[6]);
	dataPacket.push_back(BCMSWVer[5]);
	dataPacket.push_back(BCMSWVer[4]);
    dataPacket.push_back(BCMSWVer[3]);
    dataPacket.push_back(BCMSWVer[2]);
    dataPacket.push_back(BCMSWVer[1]);
    dataPacket.push_back(BCMSWVer[0]);
    //Reserved 8
    dataPacket.push_back(0xFF);
	dataPacket.push_back(0xFF);
	dataPacket.push_back(0xFF);
	dataPacket.push_back(0xFF);
	dataPacket.push_back(0xFF);
	dataPacket.push_back(0xFF);
	dataPacket.push_back(0xFF);
	dataPacket.push_back(0xFF);
    return dataPacket;
}
std::vector<uint8_t> VehicleInfoMessage::stringToBinary(const std::string& str) {
    std::vector<uint8_t> binary;
    for (char c : str) {
        std::bitset<8> bits(c);
        uint8_t byte = static_cast<uint8_t>(bits.to_ulong());
        binary.push_back(byte);
    }
    return binary;
}