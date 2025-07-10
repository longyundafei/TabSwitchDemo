/*
 * Copyright (C) 2017 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef android_hardware_automotive_vehicle_V2_0_impl_CommBase_H_
#define android_hardware_automotive_vehicle_V2_0_impl_CommBase_H_

#include <android/hardware/automotive/vehicle/2.0/IVehicle.h>
#include <string>
#include <thread>
#include <vector>
#include <queue>
#include "SerialCommunication.h"
#include "BinaryQueue.h"
#include "SendQueueV1_1.h"
#include "property_manager.h"
#include "VehicleHalProto.pb.h"
#include "CommunicationManager.h"
#include "ConfigurationWordParse.h"
#include "vehicle_config_parser.h"
#include <sys/ioctl.h>

namespace android {
namespace hardware {
namespace automotive {
namespace vehicle {
namespace V2_0 {

namespace impl {

/**
 * MessageProcess is an interface implemented by VehicleEmulator to process messages received
 * over a CommConn.
 */
class MessageProcessor {
   public:
    virtual ~MessageProcessor() = default;

    /**
     * Process a single message received over a CommConn. Populate the given respMsg with the reply
     * message we should send.
     */
    virtual void processMessage(vhal_proto::EmulatorMessage const& rxMsg,
                                vhal_proto::EmulatorMessage& respMsg) = 0;

    virtual int checkpack(const std::vector<uint8_t>& message) = 0;
    virtual void processCanReadMessage(const std::vector<uint8_t>& message) = 0;
    virtual void processCanReadMessage1(uint16_t messageID, std::vector<uint8_t>& dataContent, uint8_t busIdentifier) = 0;
    virtual void processCanWriteMessage() = 0;
    virtual void createUdsSendBackMsg(const hidl_vec<uint8_t>& data) = 0;
};

#define USE_VARY_DATA		1
#define SPI_PACKAGE_SIZE	256
#define PB_MAX_BUFF_LEN     100
#define PB_RAW_DATA_LEN     SPI_PACKAGE_SIZE

//
//台架新线束摄像头分布：
//
//EXAMPLE_IOCTL_CH0 后摄
//EXAMPLE_IOCTL_CH1 行车记录仪
//EXAMPLE_IOCTL_CH2 疲劳检测
//EXAMPLE_IOCTL_CH3 货仓
//
//TP2815_IOCTL_CH5 前环视
//TP2815_IOCTL_CH6 左环视
//TP2815_IOCTL_CH7 右环视
//TP2815_IOCTL_CH8 后环视
#define EXAMPLE_IOCTL_MAGIC 'E'
#define EXAMPLE_IOCTL_CH0 _IOR(EXAMPLE_IOCTL_MAGIC, 0, int)
#define EXAMPLE_IOCTL_CH1 _IOR(EXAMPLE_IOCTL_MAGIC, 1, int)
#define EXAMPLE_IOCTL_CH2 _IOR(EXAMPLE_IOCTL_MAGIC, 2, int)
#define EXAMPLE_IOCTL_CH3 _IOR(EXAMPLE_IOCTL_MAGIC, 3, int)

#define TP2815_IOCTL_MAGIC 'F'
#define TP2815_IOCTL_CH5 _IOR(TP2815_IOCTL_MAGIC, 5, int)
#define TP2815_IOCTL_CH6 _IOR(TP2815_IOCTL_MAGIC, 6, int)
#define TP2815_IOCTL_CH7 _IOR(TP2815_IOCTL_MAGIC, 7, int)
#define TP2815_IOCTL_CH8 _IOR(TP2815_IOCTL_MAGIC, 8, int)


#define VENDOR_REQ_TAG      0x56524551
#define VENDOR_READ_IO      _IOW('v', 0x01, unsigned int)
#define VENDOR_WRITE_IO     _IOW('v', 0x02, unsigned int)
#define VENDOR_VEHICLE_FEATURE_ID             (16)
#define VENDOR_VEHICLE_VIN_ID                 (17)
#define VENDOR_ECU_SN_ID                      (18)
#define VENDOR_VEHICLE_TSP_ID                 (19)
#define VENDOR_VEHICLE_TYPE_ID                (20)
#define VENDOR_VEHICLE_OEM_DEV_ID             (21)
#define VENDOR_MCU_VERSION_ID                 (22)
#define VENDOR_MCU_AB_PT_ID                   (23)

#define VENDOR_SET_FLAG_ID                    (50)

#define VEHICLE_FEATURE_MSG_LEN               (22)
#define VEHICLE_VIN_MSG_LEN                   (17)
#define VEHICLE_ECU_MSG_LEN                   (15)
#define VEHICLE_TSP_MSG_LEN                   (4)
#define VEHICLE_TYPE_MSG_LEN                  (2)
#define VEHICLE_OEM_MSG_LEN                   (16)
#define MCU_VERSION_MSG_LEN                   (11)
#define MCU_AB_PT_MSG_LEN                     (1)

#define VENDOR_SET_FLAG_LEN                   (8)

#if USE_VARY_DATA
	#define CAN_VARY_DATA_NUM		((SPI_PACKAGE_SIZE-6)/12)
	typedef struct canVaryData_t {
	    uint8_t busId;
		uint16_t canId;
		uint8_t canBufLen;
	    uint8_t canBuf[128];
	}canVaryData;
#else
	#define CAN_DATA_NUM		((SPI_PACKAGE_SIZE-6)/11)
	typedef struct canData_t {
	    uint8_t busId;
		uint16_t canId;
	    uint8_t canBuf[8];
	}canData;

#endif

#define CAN_UDS_NUM  (SPI_PACKAGE_SIZE-6-6)
typedef struct canUdsData_t {
    uint32_t canId;
	uint16_t canBufLen;
    uint8_t canBuf[CAN_UDS_NUM];
}canUdsData;

typedef struct pb_raw_data_s{
    uint8_t length;
	uint8_t msgid;
    uint8_t data[PB_RAW_DATA_LEN];
}pb_raw_data_s;

typedef struct pb_buff_s{
    uint8_t bg_idx;
    uint8_t ed_idx;
    pb_raw_data_s buff[PB_MAX_BUFF_LEN];
} pb_buff_s;
typedef std::function<void(std::string)> func_ptr;

typedef struct spiProcess {
    const char *msg;
    func_ptr set;
}spiProcess_t;
#define SPI_CMD_SIZE 48
/**
 * This is the interface that both PipeComm and SocketComm use to represent a connection. The
 * connection will listen for commands on a separate 'read' thread.
 */
class CommConn {
   public:
    CommConn(MessageProcessor* messageProcessor,CommunicationType comType ) : mMessageProcessor(messageProcessor),communicationType(comType),mManager(comType) {
        spiCmd[0] = {"Vin", std::bind(&CommConn::VinSet, this, std::placeholders::_1)};
	    spiCmd[1] = {"McuVersion", std::bind(&CommConn::McuVersionGet, this, std::placeholders::_1)};
        spiCmd[2] = {"SocVersion", std::bind(&CommConn::SocVersionSet, this, std::placeholders::_1)};
        spiCmd[3] = {"TRAV", std::bind(&CommConn::TRAVSet, this, std::placeholders::_1)};
        spiCmd[4] = {"Recharge", std::bind(&CommConn::RechargeSet, this, std::placeholders::_1)};
        spiCmd[5] = {"RangeMode",std::bind(&CommConn::RangeModeSet, this, std::placeholders::_1)};
        spiCmd[6] = {"PowerStatus",std::bind(&CommConn::powerStrategyModeSet, this, std::placeholders::_1)};
        spiCmd[7] = {"Navi",std::bind(&CommConn::volumeNaviSet, this, std::placeholders::_1)};
        spiCmd[8] = {"Voice",std::bind(&CommConn::volumeVoiceSet, this, std::placeholders::_1)};
        spiCmd[9] = {"Multi",std::bind(&CommConn::volumeMultiSet, this, std::placeholders::_1)};
        spiCmd[10] = {"Tel",std::bind(&CommConn::volumeTelSet, this, std::placeholders::_1)};
        spiCmd[11] = {"Alarm",std::bind(&CommConn::volumeAlarmSet, this, std::placeholders::_1)};
        spiCmd[12] = {"Key",std::bind(&CommConn::volumeKeySet, this, std::placeholders::_1)};
        spiCmd[13] = {"Maleage",NULL};
        spiCmd[14] = {"TRTime",NULL};
        spiCmd[15] = {"AvgEngC",NULL};

        spiCmd[16] = {"SN", std::bind(&CommConn::SNSet, this, std::placeholders::_1)};
        spiCmd[17] = {"HeadlightH", std::bind(&CommConn::HeadlightHSet, this, std::placeholders::_1)};
        spiCmd[18] = {"Short_Mileage",NULL};
        spiCmd[19] = {"CPUTemp", std::bind(&CommConn::CPUTempSet, this, std::placeholders::_1)};
        spiCmd[20] = {"CPULoad",std::bind(&CommConn::CPULoadSet, this, std::placeholders::_1)};
        spiCmd[21] = {"SIMCardSts", std::bind(&CommConn::SIMCardStsSet, this, std::placeholders::_1)};
        spiCmd[22] = {"TBOXLINKTSP", std::bind(&CommConn::TBOXLINKTSPSet, this, std::placeholders::_1)};
        spiCmd[23] = {"4GStrength", std::bind(&CommConn::Strength4GSet, this, std::placeholders::_1)};
        spiCmd[24] = {"CCMMode", std::bind(&CommConn::CCMModeSet, this, std::placeholders::_1)};

        spiCmd[25] = {"INR",std::bind(&CommConn::INRSet, this, std::placeholders::_1)};
        spiCmd[26] = {"DIS",std::bind(&CommConn::DISSet, this, std::placeholders::_1)};
        spiCmd[27] = {"StartODO",std::bind(&CommConn::StartODOSet, this, std::placeholders::_1)};
        spiCmd[28] = {"StartTime",std::bind(&CommConn::StartTimeSet, this, std::placeholders::_1)};
        spiCmd[29] = {"Value", std::bind(&CommConn::ValueSet, this, std::placeholders::_1)};
        spiCmd[30] = {"HardWareVersion", std::bind(&CommConn::HardWareVersionSet, this, std::placeholders::_1)};
        spiCmd[31] = {"Speed", std::bind(&CommConn::SpeedSet, this, std::placeholders::_1)};
        spiCmd[32] = {"F187", std::bind(&CommConn::F187Set, this, std::placeholders::_1)};
        spiCmd[33] = {"DriveMode", std::bind(&CommConn::DriveModeSet, this, std::placeholders::_1)};
        spiCmd[34] = {"SinglePedCtr", std::bind(&CommConn::SinglePedCtrSet, this, std::placeholders::_1)};
        spiCmd[35] = {"CreepCtr", std::bind(&CommConn::CreepCtrSet, this, std::placeholders::_1)};
        spiCmd[36] = {"ParkUnLock", std::bind(&CommConn::ParkUnLockSet, this, std::placeholders::_1)};
        spiCmd[37] = {"fm_ctrl", std::bind(&CommConn::FMSet, this, std::placeholders::_1)};
        spiCmd[38] = {"ABStatus", std::bind(&CommConn::ABStatusSet, this, std::placeholders::_1)};
        spiCmd[39] = {"ItemToSoc", std::bind(&CommConn::ItemSetByMcu, this, std::placeholders::_1)};
        spiCmd[40] = {"CountryId", std::bind(&CommConn::CountryIdSet, this, std::placeholders::_1)};
        spiCmd[41] = {"E2PRom", std::bind(&CommConn::E2PRomSet, this, std::placeholders::_1)};
        spiCmd[42] = {"KEYCode", std::bind(&CommConn::KeyCodeSet, this, std::placeholders::_1)};
        spiCmd[43] = {"LBATVol", std::bind(&CommConn::LBATVolSet, this, std::placeholders::_1)};
        spiCmd[44] = {"CHGCOND", std::bind(&CommConn::CHGCONDSet, this, std::placeholders::_1)};
        spiCmd[45] = {"SLLevel", std::bind(&CommConn::SLLevelSet, this, std::placeholders::_1)};
        spiCmd[46] = {"SLVersion", std::bind(&CommConn::SLVersionSet, this, std::placeholders::_1)};
        spiCmd[47] = {"SLOL", std::bind(&CommConn::SLOLSet, this, std::placeholders::_1)};
        spiCmdWIFI[0] = {"Strength",std::bind(&CommConn::WIFIStrength, this, std::placeholders::_1)};
        spiCmdWIFI[1] = {"Status",std::bind(&CommConn::WIFIStatus, this, std::placeholders::_1)};

        spiCmdGPS[0] = {"Lon",std::bind(&CommConn::GPSLon, this, std::placeholders::_1)};
        spiCmdGPS[1] = {"lat",std::bind(&CommConn::GPSlat, this, std::placeholders::_1)};

        spiCmdSchCHG[0] = {"ON",std::bind(&CommConn::SchCHGONSet, this, std::placeholders::_1)};
        spiCmdSchCHG[1] = {"CIRC",std::bind(&CommConn::SchCHGCIRCSet, this, std::placeholders::_1)};
        spiCmdSchCHG[2] = {"WEEK",std::bind(&CommConn::SchCHGWEEKSet, this, std::placeholders::_1)};
        spiCmdSchCHG[3] = {"Start",std::bind(&CommConn::SchCHGStartTimeSet, this, std::placeholders::_1)};
        spiCmdSchCHG[4] = {"End",std::bind(&CommConn::SchCHGEndTimeSet, this, std::placeholders::_1)};
        spiCmdSchCHG[5] = {"TGTSoc",std::bind(&CommConn::SchCHGTGTSocSet, this, std::placeholders::_1)};
    }

    virtual ~CommConn() {}

    /**
     * Start the read thread reading messages from this connection.
     */
    virtual void start();

    /**
     * Closes a connection if it is open.
     */
    virtual void stop();

    /**
     * Returns true if the connection is open and available to send/receive.
     */
    virtual bool isOpen() = 0;

    /**
     * Blocking call to read data from the connection.
     *
     * @return std::vector<uint8_t> Serialized protobuf data received from emulator.  This will be
     *              an empty vector if the connection was closed or some other error occurred.
     */
    virtual std::vector<uint8_t> read() = 0;

    /**
     * Transmits a string of data to the emulator.
     *
     * @param data Serialized protobuf data to transmit.
     *
     * @return int Number of bytes transmitted, or -1 if failed.
     */
    virtual int write(const std::vector<uint8_t>& data) = 0;
	
	virtual void sendOtaData(uint32_t addr,const uint8_t *data,uint32_t len);
	virtual void sendOtaCtlCmd(uint8_t ctlCmd);
	virtual void setOtaCallBack(const sp<IVehicleOtaCallback>& callback);

    /**
     * Serialized and send the given message to the other side.
     */
    void sendMessage(vhal_proto::EmulatorMessage const& msg);

    void setPropertyManager(PropertyManager* mp) {
        mPropertyManager = mp;
    }
    void reback_uds_message(const hidl_vec<uint8_t>& data);
    void VinSet(std::string);
    void E2PRomSet(std::string);
    void KeyCodeSet(std::string);
    void CHGCONDSet(std::string);
    void SLLevelSet(std::string);
    void SLVersionSet(std::string);
    void SLOLSet(std::string);
    void LBATVolSet(std::string);
    void ABStatusSet(std::string);
    void CountryIdSet(std::string);
    void SocVersionSet(std::string);
    void McuVersionGet(std::string);
    void TRAVSet(std::string);
    void RechargeSet(std::string);
    void RangeModeSet(std::string);
    void powerStrategyModeSet(std::string);
    void volumeNaviSet(std::string  msg);
    void volumeVoiceSet(std::string  msg);
    void volumeMultiSet(std::string  msg);
    void volumeTelSet(std::string  msg);
    void volumeAlarmSet(std::string  msg);
    void volumeKeySet(std::string  msg);
    void MaleageSet(std::string  msg);
    void TRTimeSet(std::string  msg);
    void AvgEngCSet(std::string  msg);
    void F187Set(std::string);
    void DriveModeSet(std::string);
    void SinglePedCtrSet(std::string);
    void CreepCtrSet(std::string);
    void ParkUnLockSet(std::string);
    void FMSet(std::string msg);
    void SpeedSet(std::string  msg);
    void StartVSet(std::string  msg);
    void EndVSet(std::string  msg);
    void RTimeSet(std::string  msg);
    void INRSet(std::string  msg);
    void DISSet(std::string  msg);
    void StartODOSet(std::string  msg);
    void StartTimeSet(std::string  msg);
    void ValueSet(std::string  msg);
    void HardWareVersionSet(std::string msg);
    void SNSet(std::string);
    void HeadlightHSet(std::string);
    void ShortMileageSet(std::string  msg);
    void CPUTempSet(std::string msg);
    void CPULoadSet(std::string msg);
    void SIMCardStsSet(std::string msg);
    void TBOXLINKTSPSet(std::string msg);
    void Strength4GSet(std::string msg);
    void CCMModeSet(std::string  msg);
    void sendCPUTempToMcu();
    void sendSIMCardStsToMcu(); 
    void sendWifiInfo();
    void sendCameraStsToMcu();
    void getSchCHG();
    void SchCHGONSet(std::string  msg);
    void SchCHGCIRCSet(std::string  msg);
    void SchCHGWEEKSet(std::string  msg);
    void SchCHGStartTimeSet(std::string  msg);
    void SchCHGEndTimeSet(std::string  msg);
    void SchCHGTGTSocSet(std::string  msg);
    void ItemSetByMcu(std::string  msg);
    void WIFIStrength(std::string msg);
    void WIFIStatus(std::string msg);
    void GPSLon(std::string msg);
    void GPSlat(std::string msg);

    void process_cmd(std::string key, std::string value);
    void parseDate(const Json::Value& root);
    void process_cmd_SchCHG(std::string key, std::string value);
    void process_cmd_WIFI(std::string key, std::string value);
    void process_cmd_GPS(std::string key, std::string value);
    int ItemSetByFactoryMode(const std::vector<uint8_t>& value, uint16_t id, uint16_t len);
    int ItemGetByVehicleService(uint16_t id, std::vector<uint8_t>& out_data, uint16_t len);
    void check_send_back_json();
    int vehicle_vendor_storage_read_or_write(int rw, const std::vector<uint8_t>& write_value, uint16_t id, uint16_t len, std::vector<uint8_t>& out_value);
    int vehicle_vendor_storage_write(uint16_t id, uint16_t len, const std::vector<uint8_t>& write_value);
    int vehicle_vendor_storage_read(uint16_t id, uint16_t len, std::vector<uint8_t>& out_data);
    bool SchCHGQueryFlag = false;
    int mCountBCM_PowerStatus{0};
   protected:
    VehicleConfigParser mVehicleConf{"vendor/etc/gecko-vehicle-config.json"};
    std::unique_ptr<std::thread> mReadThread;
    std::unique_ptr<std::thread> mWriteThread;
    std::unique_ptr<std::thread> mTransferThread;
    std::unique_ptr<std::thread> mCreateCanMsgThread;
    std::unique_ptr<std::thread> mProcessCanMsgThread;
    std::unique_ptr<std::thread> mHandlerCanDataThread;
    std::unique_ptr<std::thread> mHandlerCreatespiThread;
    std::unique_ptr<std::thread> mHandlerjsonspiThread;
    std::unique_ptr<std::thread> mMCULogThread;
    MessageProcessor* mMessageProcessor;
    CommunicationType communicationType;
    CommunicationManager mManager;
    ConfigurationWordParse mConfigWordParser;
    BinaryQueue mrecvqueue{1024*1024};
    PropertyManager* mPropertyManager;
    ConfigDBC* mpAC_HAVCState;
    ConfigDBC* mpACkey;
    ConfigDBC* mpHuAcState;
    ConfigDBC* mpHeatKey;
    ConfigDBC* mpFreshReciKey;
    ConfigDBC* mpFrontDefrostKey;
    ConfigDBC* mpBlowerLevel;
    ConfigDBC* mpAIRDistributionModeReq;
    ConfigDBC* mpLeftSetTemp;
    ConfigDBC* mpLeftBackTemp;
    ConfigDBC* mpRearFogLampSet;
    ConfigDBC* mpDriveModeReq;
    ConfigDBC* mpRechrgnLvReq;
    ConfigDBC* mpLeftMirrorSet;
    ConfigDBC* mpRightMirrorSet;
    ConfigDBC* mpAutoUnlockSet;
    ConfigDBC* mpAutoLockSet;
    ConfigDBC* mpParkUnlockSet;
    ConfigDBC* mpBCAvasState; //低速行人碰撞
    ConfigDBC* mpSinglePedCtrCmd;
    ConfigDBC* mpCreepCtrCmd;
    ConfigDBC* mpTotalOdometer;
    ConfigDBC* mpRemtCtrlFlag;
    ConfigDBC* mpRemoteLock;
    ConfigDBC* mVcuVersion;
    ConfigDBC* mSpeedLimitLevel;
    ConfigDBC* mSetValueFlag;  //
    ConfigDBC* mpHybridEcoMode;
    ConfigDBC* mpPwrOffReq;
    ConfigDBC* mpWiperIntervalTime; 
    ConfigDBC* mpFlwrMeHmCtrl;
    ConfigDBC* mpIllmndLockSts;
    ConfigDBC* mpIllmndUnLockSts;
    ConfigDBC* mpLockHornOnSt;
    ConfigDBC* mpEscFuncEnaStaResvd;
    ConfigDBC* mpAvhEnb;
    ConfigDBC* mpEpbModal;
    ConfigDBC* mpElecSteerModeSet;
    ConfigDBC* mpBrkDisWipSw;
    ConfigDBC* mpDrivingPowerSt;
    ConfigDBC* mpCCMTboxRemoteLock;
    ConfigDBC* mpCCMTboxRemoteCarSearch;
    ConfigDBC* mpHuVehMaxCurrent;
    ConfigDBC* mpAcUnlockVentSet;
    ConfigDBC* mpHuSpdDowninCorEnableSw;
    ConfigDBC* mpHuRcwEnableSw;
    ConfigDBC* mpHuDowEnaSw;
    ConfigDBC* mpHuBsdEnableSw;
    ConfigDBC* mpHuRctaEnableSw;
    ConfigDBC* mpHuLkaEnableSw;
    ConfigDBC* mpHuLdwEnableSw;
    ConfigDBC* mpHuElkEnbSw;
    ConfigDBC* mpHuAebEnableSw;
    ConfigDBC* mpHuAvasSetKey;
    ConfigDBC* mpHuTpmsLearningReq;
    ConfigDBC* mpHuVehTarSoc;
    ConfigDBC* mpCdcBackLadjval;
    ConfigDBC* mpHuTpmsResetReq;
    ConfigDBC* mpHuBcmotaModeSet;
    ConfigDBC* mpHuMaiDrvrSeatHeat;
    ConfigDBC* mpHuSteerWhlHeat; 

    ConfigDBC* mpHuHmaEnableSw;
    ConfigDBC* mphu_ica_ena_swt;
    ConfigDBC* mphu_ica_avd_truck_set; 
    ConfigDBC* mphu_ldw_alert_method_sw;
    ConfigDBC* mphu_fcw_enable_sw;
    ConfigDBC* mphu_slif_enable_sw;
    ConfigDBC* mphu_slwf_enable_sw;
    ConfigDBC* mpTboxRemotePowerLock;
    ConfigDBC* mpTboxRemotePowerLockV;
    ConfigDBC* mpTboxRemtSteerWhlHeatReq;
    ConfigDBC* mpTboxRemoteDefrostCtl;
    ConfigDBC* mpTboxRemtPowerCtrlReq;
    ConfigDBC* mpTboxRemtMaiDrSeatHeatReq;
    ConfigDBC* mpHuNaviCountryCode;
    ConfigDBC* mpHuNavCountryId;
    ConfigDBC* mpCdcActiveWakeUp;
    ConfigDBC* mpCdcWakeUpReas;
    ConfigDBC* mpCdcKeepAwakeReasRemReq;
    ConfigDBC* mpCdcKeepAwakeReasFacReset; 
    ConfigDBC* mpCdcKeepAwakeReasRestart;
    ConfigDBC* mpCdcKeepAwakeReasDO;
    ConfigDBC* mpCdcKeepAwakeReasLocUp;
    ConfigDBC* mpCdcKeepAwakeSta;
    ConfigDBC* mpCdcActiveWakeUpTimeout;
    ConfigDBC* mpCdcWakeUpReasTimeout;
    ConfigDBC* mpCdcKeepAwakeReasRemReqTimeout;
    ConfigDBC* mpCdcKeepAwakeReasFacResetTimeout;
    ConfigDBC* mpCdcKeepAwakeReasRestartTimeout;
    ConfigDBC* mpCdcKeepAwakeReasDOTimeout;
    ConfigDBC* mpCdcKeepAwakeReasLocUpTimeout;
    ConfigDBC* mpCdcKeepAwakeStaTimeout;
    ConfigDBC* mpHuBcmCargoLightSet;
    Property* mpMcuInfoVersion;

    /**
    ConfigDBC* mpWorkReq;
    ConfigDBC* mpNmBit; 
    ConfigDBC* mpOtaModel;
    ConfigDBC* mpOpenDoorWarnSet;
    ConfigDBC* mpTpmsLearningValid;
    ConfigDBC* mpRemtCtrlFlag;
    ConfigDBC* mpCpuTemperatureLow;
    ConfigDBC* mpSocSoftwareNum;
    ConfigDBC* mpSocHardwareNum;
    ConfigDBC* mpMcuSoftwareNum;
    ConfigDBC* mpMcuHardwareNum;
    ConfigDBC* mpTimeStatus;
    ConfigDBC* mpYear;
    ConfigDBC* mpMonth;
    ConfigDBC* mpDay;
    ConfigDBC* mpHour;
    ConfigDBC* mpMinute;
    ConfigDBC* mpSecond;
    ConfigDBC* mpWorkReqc;
    ConfigDBC* mpNmBitc;
    */
    ConfigDBC* mpOpenDoorWarnSet;
    ConfigDBC* mpOperatLicence;
    ConfigDBC* mpChargingConditions; 
    ConfigDBC* mpVehicleSpdDsp;
    ConfigDBC* mpRemaRangeDispMode;
    ConfigDBC* mpActualRemaRange;
    ConfigDBC* mpActualRemaRangeVd;
    ConfigDBC* mpCltcRemaRange;
    ConfigDBC* mpCltcRemaRangeVd;
    ConfigDBC* mpCentralLockSwitchReq;
    ConfigDBC* mpEnergyRecovLevelCmd;
    ConfigDBC* mpCCMVersion;
    ConfigDBC* mpCCMVehTarSoc;
    ConfigDBC* mpCCMTargetChrgCurr;
    int mGetSnFlag = 0;
    int mGetHeadlightFlag = 0;

    int mCounterTbox3B3=0;
    int mCounterRemoteLock=0;
    int mCounterRemoteCarSearch = 0;
    int mGetVinFlag = 0;
    int mGetMcuVersionFlag = 0;
    int mGetSocVersionFlag = 0;
    int mGetBackLightFlag = 0;
    int mGetMtceFlag = 0;
    int mGetHardWareVersionFlag = 0;
    int mCountrerCCM_AC_HAVCState=0;
    int mCountrerCCM_ACkey=0;
    int mCountrer_HuAcState = 0;
    int mCountrerCCM_HeatKey=0;
    int mCountrerCCM_FreshReciKey=0;
    int mCountrerCCM_FrontDefrostKey=0;
    int mCountrerCCM_BlowerLevel=0;
    int mCountrerCCM_AIRDistributionModeReq=0;
    int mCountrerCCM_LeftSetTemp=0;
    int mCountrerCCM_TboxAcTempSet=0;
    int mCountrerCCM_RemtCtrlFlag=0;
    int mCountrerCCM_LeftMirrorSet=0;
    int mCountrerCCM_RightMirrorSet=0;
    int ccm352_is_runing = 0;
    int mCounterCCM_CentralLock_SwitchReq=0;
    int mCountrerHu_HibridEcoModeSet = 0;
    int mCountrerHu_PwrOffReq = 0; 
    int mCountrerHu_WiperIntervalTime = 0;
    int mCountrerHu_FlwrMeHmCtrl = 0;
    int mCountrerHu_ParkUnlockSet = 0;
    int mCountrerHu_IllmndLockSts = 0;
    int mCountrerHu_IllmndUnLockSts = 0;
    int mCountrerHu_LockHornOnSt = 0;
    int mCountrerHu_CreepCtrCmd = 0;
    int mCountrerHu_EscFuncEnaStaResvd = 0;
    int mCountrerHu_AvhEnb = 0;
    int mCountrerHu_DrivingModeSelect = 0;
    int mCountrerHu_RecuperatePowerSet = 0;
    int mCountrerHu_DrivingPowerSet = 0;
    int mCountrerHu_EpbModal = 0;
    int mCountrerHu_ElecSteerModeSet = 0;
    int mCountrerHu_RemoteLock = 0;
    int mCountrerHu_RemoteSearch = 0; 
    int mCountrerHu_VehMaxCurrent = 0;
    int mCountrerAc_UnlockVentSet = 0;
    int mCountrerHu_SpdDowninCorEnableSw = 0;
    int mCountrerHu_RcwEnableSw = 0;
    int mCountrerHu_DowEnaSw = 0;
    int mCountrerHu_BsdEnableSw = 0;
    int mCountrerHu_RctaEnableSw = 0;
    int mCountrerHu_LkaEnableSw = 0;
    int mCountrerHu_LdwEnableSw = 0;
    int mCountrerHu_ElkEnbSw = 0;
    int mCountrerHu_AebEnableSw = 0;
    int mCountrerHu_AvasSetKey = 0;
    int mCountrerHu_TpmsLearningReq = 0;
    int mCountrerHu_VehTarSoc = 0;
    int mCountrer_CdcBackLadjval = 0;
    int mCountrer_HuTpmsResetReq = 0;
    int mCountrer_HuBcmotaModeSet = 0;
    int mCountrer_HuSteerWhlHeat = 0;
    int mCountrer_HuMaiDrvrSeatHeat = 0; 

    int mCountrer_HuHmaEnableSw = 0; 
    int mCountrer_hu_ica_ena_swt = 0; 
    int mCountrer_hu_ica_avd_truck_set = 0; 
    int mCountrer_hu_ldw_alert_method_sw = 0; 
    int mCountrer_hu_fcw_enable_sw = 0; 
    int mCountrer_hu_slif_enable_sw = 0; 
    int mCountrer_hu_slwf_enable_sw = 0; 
    int mCountrer_TboxRemotePowerLock = 0;
    int mCountrer_TboxRemotePowerLockV = 0;
    int mCountrer_TboxRemtSteerWhlHeatReq = 0;
    int mCountrer_TboxRemoteDefrostCtl = 0;
    int mCountrer_TboxRemtPowerCtrlReq = 0;
    int mCountrer_TboxRemtMaiDrSeatHeatReq = 0;
    int mCountrer_HuBcmCargoLightSet = 0;
    int mCountrerCdc_ActiveWakeUp = 0;
    int mCountrerCdc_WakeUpReas = 0;
    int mCountrerCdc_KeepAwakeReasRemReq = 0;
    int mCountrerCdc_KeepAwakeReasFacReset = 0;
    int mCountrerCdc_KeepAwakeReasRestart = 0;
    int mCountrerCdc_KeepAwakeReasDO = 0;
    int mCountrerCdc_KeepAwakeReasLocUp = 0;
    int mCountrerCdc_KeepAwakeSta = 0;
    int mCpuUsage;
    std::vector<uint8_t> mvCCM310;
    std::vector<uint8_t> mvCCM311;
    std::vector<uint8_t> mvCCM66B;
    std::vector<uint8_t> mvCCM353;
    std::vector<uint8_t> mvCCM354;
    std::vector<uint8_t> mvCCM321;
    std::vector<uint8_t> mvCCM558;
    std::vector<uint8_t> mvCCM630;
    std::vector<uint8_t> mvCCM66C;
    std::mutex mtx;
    std::condition_variable SPIcv;
//    std::shared_ptr<std::condition_variable> SPIcv;
    std::mutex cv_mutex;
    bool SPIready = false; // 线程调度的条件变量
    bool MCUUpdate = false;
    int  mMcuToSocComErr = 0;
    spiProcess_t spiCmdWIFI[2];
    spiProcess_t spiCmdGPS[2];
    spiProcess_t spiCmdSchCHG[6];

    std::condition_variable cv_vin;
    std::condition_variable cv_VehCfg;
    std::condition_variable cv_SN;
    std::condition_variable cv_F187;
    std::condition_variable cv_BackLight;
    int mGetVehCfgFlag = 0;
    int mGetSNFlag = 0;
    int mGetF187Flag = 0;
    // int mGetTPMSFlag = 0;
    // int mGetDMSFlag = 0;
    // int mGetPDCFlag = 0;
    // int mGetRVCFlag = 0;
    // int mGetSWCFlag = 0;
    // int mGetDVRFlag = 0;
    // int mGetCargoCamFlag = 0;
    // int mGetPowerMirrorsFlag = 0;
    // int mGetBootImageFlag = 0;
    // int mGetTSPSiteFlag = 0;
    // int mGetLanguageFlag = 0;
    // int mGetESCFlag = 0;
    // int mGet360AVMFlag = 0;
    // int mGetSlidingDoorFlag = 0;
    // int mGetTrunkDoorFlag = 0;
    void queryConfigWaitUntil(std::string key, std::condition_variable& cv_query);
    bool parseJson(const std::string& filename);
    //void parseSecondJson(const std::string& filename);
    void updateConfig(std::string key, std::string msg);
    /*
    int mCCM558Counter=0;
    int mCCM630Counter=0;
    */
    /**
     * A thread that reads messages in a loop, and responds. You can stop this thread by calling
     * stop().
     */
    void ProcessCanMsgThread();
    void readThread();
    void writeThread();
    void transferThread();
    void HandlerCanDataThread();
    void HandlerspiDataThread();
    void HandlerjsonspiThread();
    void HandlerjsonspiThread_V1_2();
    void createCanMsgThread();
    void createCanMsgHuBBac();
    void createCanMsgCCM311V1_1();
    void createCanMsgCCM353V1_1();
    void createCanMsgCCM354V1_1();
	void createCanMsgThread_V1_2();
    void createCanMsgCCM310();
    void createCanMsgCCM311();
    void createCanMsgCCM353();
    void createBodyCanMsgCCM353();
    void createCanMsgCCM354();
    void createBodyCanMsgCCM354();
    void createCanMsgCCM352();
    void syncVersionInfo(std::string, std::string);
    void resetSomeProp();
    void ProcessCanMsgThread_V1_1();
    void createCanMsgThread_V1_1();
    void transferThread_V1_1();
	void transferThread_V1_2();
    void readMCULogThread();
    int GetSpiStartAck();
    void SpiSetAck();
    void setTboxLinkTsp();
    void process_cmd(Json::Value::Members &members,Json::Value &root);
    void getVolume();
    void createCanMsgHuABac();
    std::string millisecondsToDateTime(int64_t ms);
    void writeMCUUpdateFlag(std::string buf);
    void createCanMsgHuCBac();
    void createCanMsgFcmABac();
    void createCanMsgNMAsrCdcBac();
    void createCanMsgTboxBjsTimeBac();
    bool isValidInt(const std::string& str);
    std::string readMCUUpdateFlag();
	void writeMCUAbStatus(std::string buf);
	std::string readMCUAbStatus();
	uint8_t getCKS(uint8_t *buf, int size);
	int8_t parseMsg(uint8_t *recvBuf,uint16_t len);
	#if USE_VARY_DATA
	uint8_t processVaryMsg(uint8_t *recvBuf);
	void getQueueVaryMsg(/*uint8_t *data,*/uint8_t response);
	void makeEmptyVaryMsg(uint8_t *data,uint8_t response);
	void pb_add_raw_vary_data(uint8_t *data,uint8_t len,uint8_t msgid);
	uint8_t pb_get_vary_buff(uint8_t* data);
	uint8_t get_next_vary_msg_len();
	uint8_t get_next_vary_msg_id();
	uint16_t vary_buf_len();
	void init_pb_vary_buff_s();
	#else
	uint8_t processMsg(uint8_t *recvBuf);
	void getQueueMsg(/*uint8_t *data,*/uint8_t response);
	void makeEmptyMsg(uint8_t *data,uint8_t response);
	void pb_add_raw_data(uint8_t *data,uint8_t len,uint8_t msgid);
	uint8_t pb_get_buff(uint8_t* data);
	uint8_t get_next_msg_len();
	uint8_t get_next_msg_id();
	uint16_t buf_len();
	void init_pb_buff_s();
	#endif
	uint8_t adjust_spitrnullmsg(void);
	uint8_t errorspisendcounter(void);
	uint8_t incspisendcounter(void);
	void clearspisendcounter(void);
	void makeStartOtaMsg(uint8_t *data);
	uint8_t parseOtaMsg(uint8_t *recvBuf,uint16_t len);
	void makeOtaEmptyMsg(uint8_t *data);
	void reboot();
	void startMcuOta();
	uint8_t sendMcuOtaMsg();
	void testFileRead();
    bool isAlphanumeric(const std::string& str);
    void send_heartbeat_msg();
    int check_vendor_storage_earse();
    /*
    void createCanMsgCCM67B();
    void createCanMsgCCM355();
    void createCanMsgCCM322();
    void createCanMsgCCM559();
    void createCanMsgCCM631();
    void createCanMsgCCM67C();
    */
    private:
    uint8_t reback_uds_msg[8] = {0};
    SendQueueV1_1 mCanQueue;
    SendQueueV1_1 mSpiDownQueue;
    SendQueueV1_1 mStartCmd;
    SendQueueV1_1 mSpiUpQueue;
    uint32_t spicounter = 0;
    spiProcess_t spiCmd[SPI_CMD_SIZE];
    std::string MCUUpdateFlag = "/data/vendor/tbox/mcu_update_flag.txt";
	std::string MCUAbStatus = "/data/vendor/tbox/mcu_ab_status.txt";
	std::string filePath = "/data/vendor/tbox/boot0.bin";
    std::string filePathA = "/data/vendor/tbox/boot_a.bin";
	std::string filePathB = "/data/vendor/tbox/boot_b.bin";
	std::string gFilePath;
	int g_ABStatus;
	#if USE_VARY_DATA
	pb_buff_s g_pw_vary_buff;
	pthread_mutex_t mutexVaryData = PTHREAD_MUTEX_INITIALIZER;
	#else
	pb_buff_s g_pw_buff;
	pthread_mutex_t mutexData = PTHREAD_MUTEX_INITIALIZER;
	#endif
	uint32_t recvOkNum;
	uint32_t recvErrorNum;
	uint32_t recvErrorRetNum;
	uint8_t sendBuf[2][SPI_PACKAGE_SIZE];
	uint8_t *psendBuf = sendBuf[0];
	uint8_t first2msgflag = 2;
	uint8_t spisendcounter[2] = {0, 0};
	uint8_t sendOtaLastBuf[SPI_PACKAGE_SIZE];
	uint8_t fileSizeSendFlag = 0;
	uint8_t otaAckState = 0;
	uint8_t otaReSendCount = 0;
	uint16_t otaIndex = 0;
	uint16_t otaAckIndex = 0;
	uint8_t needParseAckFlag = 0;
	uint8_t needMakeMsgFlag = 1;
	std::string updateFlag;
	std::string abStatus;
	uint16_t fileReadLen = 0;
	off_t otafilesize = 0;
	off_t fileReadLenTotal = 0;
	FILE *fp = NULL;
	sp<IVehicleOtaCallback> otaCallback;
	uint8_t lastQueueMsgId = 0;
	uint16_t txMsgLen, txMsgIndxLen, nextFrameNo;
	uint32_t txCanId;
	uint8_t txMsgBuff[4096+128];
	canVaryData mCanVaryData[CAN_VARY_DATA_NUM]; //(SPI_PACKAGE_SIZE-6)/12
    uint16_t json_config_id = 0;
    uint16_t vendor_store_id = 0;
    uint8_t  hb_idx;
    int conf_ck_ret = 0;
//    BinaryQueue m_queue(300);
//    void enqueueData(std::queue<uint8_t>& q, const std::vector<uint8_t>& data);
//    std::vector<uint8_t> dequeueData(std::queue<uint8_t>& q, int dequeueLength);
};

}  // namespace impl

}  // namespace V2_0
}  // namespace vehicle
}  // namespace automotive
}  // namespace hardware
}  // namespace android

#endif  // android_hardware_automotive_vehicle_V2_0_impl_CommBase_H_
