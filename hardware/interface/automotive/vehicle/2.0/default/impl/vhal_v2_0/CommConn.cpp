/*
 * Copyright (C) 2018 The Android Open Source Project
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

#define LOG_TAG "CommConn"
#include "CommConn.h"
#include <android/hardware/automotive/vehicle/2.0/IVehicle.h>
#include <log/log.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <iostream>
#include <sstream>
#include <thread>
#include <iomanip>
#include "CommunicationManager.h"
#include "MessageParser.h"
#include "SendQueue.h"
#include "SendCanMap.h"
#include "body_can_ccm_4_0.h"
#include "chassise_can_ccm_4_0.h"
#include "ev_can_ccm_4_0.h"
#include "me11_cmx_baccanfd_ots_car_v02.h"
#include "mcu.h"
#include "HandlerCanData.h"
#include <android-base/logging.h>
#include <android-base/properties.h>
#include <openssl/md5.h>
#include <sys/reboot.h>
#include "DebugManager.h"
#include "ConfigurationWordParse.h" 
namespace android {
namespace hardware {
namespace automotive {
namespace vehicle {
namespace V2_0 {

namespace impl {
void printVec(std::vector<uint8_t> msg){
    std::string buf;
    int size = msg.size();
            for (int i = 0; i < size; i++) {
                std::ostringstream ss;
                ss.clear();
                int data = msg[i];
                ss << std::hex << data;
                buf += "0x";
                buf += ss.str();
                buf += ", ";
            }
    ALOGI("spi1028 ----%s", buf.c_str());
}

void CommConn::start() {
    mManager.setType(mVehicleConf.mComunication);
    mManager.openDevice(mVehicleConf);
    //SPIcv = std::make_shared<std::condition_variable>();
    //mReadThread = std::make_unique<std::thread>(std::bind(&CommConn::readThread, this));
    //mWriteThread = std::make_unique<std::thread>(std::bind(&CommConn::writeThread, this));
    if(mVehicleConf.mVersion=="1.1"){
        mCreateCanMsgThread = std::make_unique<std::thread>(std::bind(&CommConn::createCanMsgThread_V1_1, this));
        mProcessCanMsgThread = std::make_unique<std::thread>(std::bind(&CommConn::ProcessCanMsgThread_V1_1, this));
        mTransferThread = std::make_unique<std::thread>(std::bind(&CommConn::transferThread_V1_1, this));
        mHandlerCanDataThread = std::make_unique<std::thread>(std::bind(&CommConn::HandlerCanDataThread, this));
        mHandlerCreatespiThread = std::make_unique<std::thread>(std::bind(&CommConn::HandlerspiDataThread, this));
        mHandlerjsonspiThread = std::make_unique<std::thread>(std::bind(&CommConn::HandlerjsonspiThread, this));
    }else if(mVehicleConf.mVersion=="1.2"){
#if USE_VARY_DATA
		init_pb_vary_buff_s();
#else
		init_pb_buff_s();
#endif
		mTransferThread = std::make_unique<std::thread>(std::bind(&CommConn::transferThread_V1_2, this));
		mCreateCanMsgThread = std::make_unique<std::thread>(std::bind(&CommConn::createCanMsgThread_V1_2, this));
		mHandlerCanDataThread = std::make_unique<std::thread>(std::bind(&CommConn::HandlerCanDataThread, this));
        mHandlerjsonspiThread = std::make_unique<std::thread>(std::bind(&CommConn::HandlerjsonspiThread_V1_2, this));
        mMCULogThread = std::make_unique<std::thread>(std::bind(&CommConn::readMCULogThread, this));
    }else{
        mCreateCanMsgThread = std::make_unique<std::thread>(std::bind(&CommConn::createCanMsgThread, this));
        mProcessCanMsgThread = std::make_unique<std::thread>(std::bind(&CommConn::ProcessCanMsgThread, this));
        mTransferThread = std::make_unique<std::thread>(std::bind(&CommConn::transferThread, this));
        mHandlerCanDataThread = std::make_unique<std::thread>(std::bind(&CommConn::HandlerCanDataThread, this));
    }


}

void CommConn::stop() {
    if (mReadThread->joinable()) {
        mReadThread->join();
    }
    if (mWriteThread->joinable()) {
        mWriteThread->join();
    }
    if (mTransferThread->joinable()) {
        mTransferThread->join();
    }
    if (mCreateCanMsgThread->joinable()) {
        mCreateCanMsgThread->join();
    }
    if (mProcessCanMsgThread->joinable()) {
        mProcessCanMsgThread->join();
    }
    if (mHandlerCanDataThread->joinable()) {
        mHandlerCanDataThread->join();
    }
}

// CCM310 100
// CCM311 100
// CCM66B 100
// CCM353 100
// CCM354 100
// CCM321 100
// CCM558 1000
// CCM630 1000
// CCM66C 100

void createTestMsg() {
    //  struct ev_can_ccm_4_0_gw_vcu_0_x352_t dst;

    struct chassise_can_ccm_4_0_gw_abs_171_t dst;
    dst.abs_abs_fail_status = 1;

    //  dst.vcu_faultlevel = 1;

    uint16_t messageID;
    BusIdentifier busIdentifier;
    uint8_t command = 0x01;
    uint8_t responseFlag;
    //    messageID = 0x352;
    messageID = 0x171;
    //    busIdentifier = EV_CAN;
    busIdentifier = CH_CAN;
    responseFlag = 0x00;
    MessageParser mparse;
    std::vector<uint8_t> message = mparse.makePack(messageID, static_cast<void*>(&dst),
                                                   busIdentifier, command, responseFlag);

    std::string buf = "";
    for (int i = 0; i < 18; i++) {
        std::ostringstream ss;
        ss.clear();
        int data = message[i];
        ss << std::setw(2) << std::setfill('0') << std::hex << data;
        // buf += "0x";
        buf += ss.str();
        buf += " ";
    }
    ALOGI("createTestMsg %zu buffer1:%s", message.size(), buf.c_str());
}
int CommConn::GetSpiStartAck() {
    if(mStartCmd.dequeueTimeout())
        return 0;
    else
        return -1;
}
void CommConn::SpiSetAck() {
    std::vector<uint8_t> packet;
    mStartCmd.enqueue(packet);
}

std::string CommConn::millisecondsToDateTime(int64_t ms) {
    std::chrono::seconds sec(ms / 1000);
    int milli = ms % 1000;

    std::chrono::time_point<std::chrono::system_clock> tp(sec);

    std::time_t tt = std::chrono::system_clock::to_time_t(tp);
    std::tm* timeinfo = std::localtime(&tt);

    char buffer[80];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);

    std::stringstream ss;
    ss << std::setw(3) << std::setfill('0') << milli;

    std::string dateTimeStr(buffer);
    dateTimeStr += "." + ss.str();
    return dateTimeStr;
}
void CommConn::writeMCUUpdateFlag(std::string buf){

    std::ofstream istrm(MCUUpdateFlag.c_str());
    istrm <<buf <<std::endl;
    LOG(INFO)<<"mcu_update_flag: "<<buf;
    sync();
}

std::string CommConn::readMCUUpdateFlag(){
    std::string str;
    str.clear();
    std::ifstream ifs(MCUUpdateFlag.c_str());
    if(!ifs){
        //LOG(ERROR)<<"zhugang mcu_update_flag:  open file fail!";
        return str;
    }
    getline(ifs,str);
    return str;
}

void CommConn::writeMCUAbStatus(std::string buf){

    std::ofstream istrm(MCUAbStatus.c_str());
    istrm <<buf <<std::endl;
    LOG(INFO)<<"mcu_ab_status: "<<buf;
    sync();
}

std::string CommConn::readMCUAbStatus(){
    std::string str;
    str.clear();
    std::ifstream ifs(MCUAbStatus.c_str());
    if(!ifs){
        //LOG(ERROR)<<"zhugang mcu_ab_status:  open file fail!";
        return str;
    }
    getline(ifs,str);
    return str;
}
#define CHUNK_SIZE 512
// 文件读取函数
bool readFileChunk(const std::string &filePath, char *buffer, std::size_t &bytesRead, std::ifstream &fileStream) {
    // 检查文件流是否已经打开
    if (!fileStream.is_open()) {
        fileStream.open(filePath, std::ios::in | std::ios::binary);
        if (!fileStream) {
            LOG(ERROR)<<"mcu update Unable to open file: "<< filePath;
            return false;
        }
    }

    // 读取文件
    fileStream.read(buffer, CHUNK_SIZE);
    bytesRead = fileStream.gcount();

    // 检查是否到达文件末尾
    if (fileStream.eof()) {
        fileStream.close();
    }

    return true;
}
// 获取文件大小
off_t getFileSize(const char* filePath) {
    struct stat st;
    if(stat(filePath, &st) == 0) {
        return st.st_size; // 文件大小
    } else {
        perror("Error getting file size");
        return -1; // 错误处理
    }
}
std::string calculateMd5(const std::string &filename) {
    MD5_CTX ctx;
    MD5_Init(&ctx);

    std::ifstream file(filename, std::ifstream::binary);
    if (!file) {
        std::cerr << "无法打开文件: " << filename << std::endl;
        return "open file err";
    }

    char buf[1024];
//    while (file.read(buf, sizeof(buf))) {
//        MD5_Update(&ctx, buf, file.gcount());
//    }
    while(file){
        memset(buf,0,sizeof(buf));
        file.read(buf, sizeof(buf));
        MD5_Update(&ctx, buf, file.gcount());
    }

    unsigned char digest[MD5_DIGEST_LENGTH];
    MD5_Final(digest, &ctx);

    file.close();

    std::stringstream ss;
    for (int i = 0; i < MD5_DIGEST_LENGTH; ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(digest[i]);
    }

    return ss.str();
}

bool fileExists(const std::string& filePath) {
    std::ifstream file(filePath);
    return file.good();
}
void CommConn::createCanMsgThread_V1_1() {
    mpAC_HAVCState = mPropertyManager->findCProperty("sc_ccm_ac_havc_state");
    mpACkey = mPropertyManager->findCProperty("sc_ccm_a_ckey");
    mpHeatKey = mPropertyManager->findCProperty("sc_ccm_heat_key");
    mpFreshReciKey = mPropertyManager->findCProperty("sc_ccm_fresh_reci_key");
    mpFrontDefrostKey = mPropertyManager->findCProperty("sc_ccm_front_defrost_key");
    mpBlowerLevel = mPropertyManager->findCProperty("sc_ccm_blower_level");
    mpAIRDistributionModeReq = mPropertyManager->findCProperty("sc_ccm_air_distribution_mode_req");
    mpLeftSetTemp = mPropertyManager->findCProperty("sc_ccm_left_set_temp");
    mpLeftBackTemp = mPropertyManager->findCProperty("bc_ac_left_set_temperature");
    mpRearFogLampSet = mPropertyManager->findCProperty("sc_ccm_rear_fog_lamp_set");
    mpDriveModeReq = mPropertyManager->findCProperty("sc_ccm_drive_mode_req");
    mpRechrgnLvReq = mPropertyManager->findCProperty("sc_ccm_rechrgn_lv_req");
    mpLeftMirrorSet = mPropertyManager->findCProperty("sc_ccm_left_mirror_set");
    mpRightMirrorSet = mPropertyManager->findCProperty("sc_ccm_right_mirror_set");
    mpAutoUnlockSet = mPropertyManager->findCProperty("sc_ccm_auto_unlock_req");
    mpAutoLockSet = mPropertyManager->findCProperty("sc_ccm_auto_lock_req");
    mpParkUnlockSet = mPropertyManager->findCProperty("sc_ccm_park_un_lock_req");
    mpBCAvasState = mPropertyManager->findCProperty("sc_ccm_avasset");  // 低速行人功能
    mpCreepCtrCmd = mPropertyManager->findCProperty("sc_ccm_creep_ctr_cmd");
    mpSinglePedCtrCmd = mPropertyManager->findCProperty("sc_ccm_single_ped_ctr_cmd");
    mpTotalOdometer = mPropertyManager->findCProperty("sc_ccm_vehicle_spd_dsp");
    mpRemtCtrlFlag = mPropertyManager->findCProperty("sc_ccm_remt_ctrl_flag");
    mpRemoteLock   = mPropertyManager->findCProperty("sc_ccm_remote_lock");
    mVcuVersion = mPropertyManager->findCProperty("vcu_version");
    mSpeedLimitLevel =  mPropertyManager->findCProperty("sc_ec_ccm_speed_limit_level");
    mSetValueFlag =  mPropertyManager->findCProperty("set_value_flag");

    mpOpenDoorWarnSet = mPropertyManager->findCProperty("sc_ccm_bcm_open_door_warn_set");
    mpChargingConditions = mPropertyManager->findCProperty("sc_ccm_charging_conditions");
    mpOperatLicence = mPropertyManager->findCProperty("sc_ccm_operat_licence");
    mpVehicleSpdDsp = mPropertyManager->findCProperty("sc_ccm_vehicle_spd_dsp");
    mpRemaRangeDispMode = mPropertyManager->findCProperty("sc_rema_range_display_mode_down");
    mpActualRemaRange = mPropertyManager->findCProperty("ec_vcu_actual_rema_range");
    mpActualRemaRangeVd = mPropertyManager->findCProperty("ec_vcu_actual_rema_range_vd");
    mpCltcRemaRange = mPropertyManager->findCProperty("ec_vcu_cltc_rema_range");
    mpCltcRemaRangeVd = mPropertyManager->findCProperty("ec_vcu_cltc_rema_range_vd");
    mpCentralLockSwitchReq = mPropertyManager->findCProperty("sc_ccm_central_lock_switch");
    mpEnergyRecovLevelCmd = mPropertyManager->findCProperty("sc_ccm_energy_recov_level_cmd");
    mpCCMVersion = mPropertyManager->findCProperty("sc_ccm_version");
    mpCCMVehTarSoc = mPropertyManager->findCProperty("chraging_target_soc");
    mpCCMTargetChrgCurr = mPropertyManager->findCProperty("sc_ec_ccm_target_chrg_curr");
    mpHybridEcoMode = mPropertyManager->findCProperty("sc_hybird_eco_mode");
    mpPwrOffReq     = mPropertyManager->findCProperty("sc_pwr_off_req");
    mpWiperIntervalTime = mPropertyManager->findCProperty("sc_wiper_interval_time");
    mpFlwrMeHmCtrl  = mPropertyManager->findCProperty("sc_flwr_me_hm_ctrl");
    mpIllmndUnLockSts = mPropertyManager->findCProperty("sc_illmnd_unlock_sts");
    mpIllmndLockSts = mPropertyManager->findCProperty("sc_illmnd_lock_sts");
    mpLockHornOnSt  = mPropertyManager->findCProperty("sc_lock_horn_on_st");
    mpEscFuncEnaStaResvd = mPropertyManager->findCProperty("sc_esc_func_ena_sta_resvd");
    mpAvhEnb = mPropertyManager->findCProperty("sc_avh_enb");
    mpEpbModal = mPropertyManager->findCProperty("sc_epb_modal");
    mpBrkDisWipSw = mPropertyManager->findCProperty("sc_brk_dis_wip_sw");
    mpElecSteerModeSet = mPropertyManager->findCProperty("sc_elec_steer_mode_set");
    mpDrivingPowerSt = mPropertyManager->findCProperty("sc_driving_power_set");
    mpCCMTboxRemoteLock = mPropertyManager->findCProperty("sc_ccm_tbox_remote_lock");
    mpCCMTboxRemoteCarSearch = mPropertyManager->findCProperty("sc_ccm_remote_car_search");
    mpHuVehMaxCurrent =  mPropertyManager->findCProperty("sc_hu_veh_max_current");
    mpAcUnlockVentSet =  mPropertyManager->findCProperty("sc_hu_ac_unlock_vent_set");
    mPropertyManager->setspiQueue(&mSpiDownQueue);

    //
    int communication_start = 0;
    while (true) {
        //创建对应报文
        //周期性的报文
        //单次收发的报文
        if(communication_start==0){
        //如果完成通讯握手，则进入下一步communication_start = 1
            uint16_t messageID;
            BusIdentifier busIdentifier;
            uint8_t command = 0x02;
            uint8_t responseFlag;
            messageID = 0xfffe;
            busIdentifier = BODY_CAN;
            responseFlag = 0x00;
            MessageParser mparse;
            uint8_t tempdata[8];
            memset(tempdata,0,sizeof(tempdata));
            std::vector<uint8_t> message = mparse.makeTRANPack(messageID, tempdata,
                                                           busIdentifier, command, responseFlag);
            std::string buf;
                buf = "";
                for (int i = 0; i < 17; i++) {
                    std::ostringstream ss;
                    ss.clear();
                    int data = message[i];
                    ss << std::hex << data;
                    buf += "0x";
                    buf += ss.str();
                    buf += ", ";
                }
            ALOGI("create Send FULL %zu buffer1:%s", message.size(), buf.c_str());
            std::vector<uint8_t> buffer{};
            std::vector<uint8_t> msg;
            std::vector<uint8_t> temp_msg = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
            msg.insert(msg.end(), message.begin(), message.end());
            for (int i = 1; i< 60; i++) {
                msg.insert(msg.end(), temp_msg.begin(), temp_msg.end());
            }
            msg.insert(msg.begin(), static_cast<uint8_t>(spicounter & 0xFF));
            msg.insert(msg.begin(), static_cast<uint8_t>((spicounter >> 8) & 0xFF));
            msg.insert(msg.begin(), static_cast<uint8_t>((spicounter >> 16) & 0xFF));
            msg.insert(msg.begin(), static_cast<uint8_t>((spicounter >> 24) & 0xFF));
            spicounter++;
            //添加对应id的信号回应,确认回应状态
            int repeate = 0;
            do{
                mSpiDownQueue.enqueue(msg);
                if(GetSpiStartAck()==0){
                    communication_start = 1;
                    break;
                }
                repeate++;
            }while(repeate <300);
        }else if(communication_start == 1){
        //第二步和mcu进行状态同步
            communication_start = 2;
            Json::Value root;
            root["Vin"] = "?";
            // 输出JSON字符串
            Json::StreamWriterBuilder builder;
            std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
            std::ostringstream oss;
            writer->write(root, &oss);
            // 获取字符串
            std::string str = oss.str();
            // 将字符串转换为 std::vector<uint8_t>
            std::vector<uint8_t> vec(str.begin(), str.end());
            mPropertyManager->jsonQueueEnqueue(vec);
            syncVersionInfo("McuVersion", "?");
            syncVersionInfo("SocVersion", android::base::GetProperty("gecko.soc.version", "SV00.00"));
            getVolume();
        }else{
            createCanMsgHuBBac();
            //createCanMsgCCM311V1_1();
            //createCanMsgCCM353V1_1();
            //createCanMsgCCM354V1_1();
            createCanMsgHuABac();
            createCanMsgHuCBac();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
}

void CommConn::createCanMsgThread() {
    mpAC_HAVCState = mPropertyManager->findCProperty("sc_ccm_ac_havc_state");
    mpACkey = mPropertyManager->findCProperty("sc_ccm_a_ckey");
    mpHeatKey = mPropertyManager->findCProperty("sc_ccm_heat_key");
    mpFreshReciKey = mPropertyManager->findCProperty("sc_ccm_fresh_reci_key");
    mpFrontDefrostKey = mPropertyManager->findCProperty("sc_ccm_front_defrost_key");
    mpBlowerLevel = mPropertyManager->findCProperty("sc_ccm_blower_level");
    mpAIRDistributionModeReq = mPropertyManager->findCProperty("sc_ccm_air_distribution_mode_req");
    mpLeftSetTemp = mPropertyManager->findCProperty("sc_ccm_left_set_temp");
    mpRearFogLampSet = mPropertyManager->findCProperty("sc_ccm_rear_fog_lamp_set");
    mpDriveModeReq = mPropertyManager->findCProperty("sc_ccm_drive_mode_req");
    mpRechrgnLvReq = mPropertyManager->findCProperty("sc_ccm_rechrgn_lv_req");
    mpLeftMirrorSet = mPropertyManager->findCProperty("sc_ccm_left_mirror_set");
    mpRightMirrorSet = mPropertyManager->findCProperty("sc_ccm_right_mirror_set");
    mpAutoUnlockSet = mPropertyManager->findCProperty("sc_ccm_auto_unlock_req");
    mpAutoLockSet = mPropertyManager->findCProperty("sc_ccm_auto_lock_req");
    mpParkUnlockSet = mPropertyManager->findCProperty("sc_ccm_park_un_lock_req");
    mpBCAvasState = mPropertyManager->findCProperty("sc_ccm_avasset");  // 低速行人功能
    mpCreepCtrCmd = mPropertyManager->findCProperty("sc_ccm_creep_ctr_cmd");
    mpSinglePedCtrCmd = mPropertyManager->findCProperty("sc_ccm_single_ped_ctr_cmd");
    mpTotalOdometer = mPropertyManager->findCProperty("sc_ccm_vehicle_spd_dsp");
    mpRemtCtrlFlag = mPropertyManager->findCProperty("sc_ccm_remt_ctrl_flag");
    mpRemoteLock   = mPropertyManager->findCProperty("sc_ccm_remote_lock");
    mVcuVersion = mPropertyManager->findCProperty("vcu_version");
    mSpeedLimitLevel =  mPropertyManager->findCProperty("sc_ec_ccm_speed_limit_level");
    mSetValueFlag =  mPropertyManager->findCProperty("set_value_flag");

    mpOpenDoorWarnSet = mPropertyManager->findCProperty("sc_ccm_bcm_open_door_warn_set");
    mpChargingConditions = mPropertyManager->findCProperty("sc_ccm_charging_conditions");
    mpOperatLicence = mPropertyManager->findCProperty("sc_ccm_operat_licence");
    mpVehicleSpdDsp = mPropertyManager->findCProperty("sc_ccm_vehicle_spd_dsp");
    mpRemaRangeDispMode = mPropertyManager->findCProperty("sc_rema_range_display_mode");
    mpActualRemaRange = mPropertyManager->findCProperty("ec_vcu_actual_rema_range");
    mpActualRemaRangeVd = mPropertyManager->findCProperty("ec_vcu_actual_rema_range_vd");
    mpCltcRemaRange = mPropertyManager->findCProperty("ec_vcu_cltc_rema_range");
    mpCltcRemaRangeVd = mPropertyManager->findCProperty("ec_vcu_cltc_rema_range_vd");
    mpCentralLockSwitchReq = mPropertyManager->findCProperty("sc_ccm_central_lock_switch");
    mpEnergyRecovLevelCmd = mPropertyManager->findCProperty("sc_ccm_energy_recov_level_cmd");
    mpCCMVersion = mPropertyManager->findCProperty("sc_ccm_version");
    mpCCMVehTarSoc = mPropertyManager->findCProperty("chraging_target_soc");
    mpCCMTargetChrgCurr = mPropertyManager->findCProperty("sc_ec_ccm_target_chrg_curr");

    ALOGI("hu0506 %s %d %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p", __func__, __LINE__,
          mpAC_HAVCState, mpACkey, mpHeatKey, mpFreshReciKey, mpFrontDefrostKey, mpBlowerLevel, mpAIRDistributionModeReq, mpLeftSetTemp, 
          mpRearFogLampSet,mpDriveModeReq,mpRechrgnLvReq,mpLeftMirrorSet,mpRightMirrorSet,mpAutoUnlockSet,mpAutoLockSet,mpParkUnlockSet, 
          mpBCAvasState,mpCreepCtrCmd,mpSinglePedCtrCmd,mVcuVersion,mSpeedLimitLevel,mSetValueFlag);
    ALOGI("hu0506 %s %d %p %p %p %p %p %p %p %p %p %p %p", __func__, __LINE__,
          mpOpenDoorWarnSet, mpChargingConditions, mpOperatLicence, mpVehicleSpdDsp, mpRemaRangeDispMode, mpActualRemaRange, mpActualRemaRangeVd, mpCltcRemaRange, 
          mpCltcRemaRangeVd,mpCentralLockSwitchReq,mpEnergyRecovLevelCmd);
    /*
    mpWorkReq = mPropertyManager->findCProperty("sc_str_work_req_0x66b");
    mpNmBit = mPropertyManager->findCProperty("sc_ccm_nm_bit_0x66b");
    mpOtaModel = mPropertyManager->findCProperty("sc_ota_model");
    mpOpenDoorWarnSet = mPropertyManager->findCProperty("sc_ccm_bcm_open_door_warn_set");
    mpTpmsLearningValid = mPropertyManager->findCProperty("sc_ccm_tpms_learning_valid");
    mpRemtCtrlFlag = mPropertyManager->findCProperty("sc_ccm_remt_ctrl_flag");
    mpCpuTemperatureLow = mPropertyManager->findCProperty("sc_tbox_cpu_temperature_low");
    mpSocSoftwareNum = mPropertyManager->findCProperty("sc_ccm_soc_sofeware_num");
    mpSocHardwareNum = mPropertyManager->findCProperty("sc_ccm_soc_hardware_num");
    mpMcuSoftwareNum = mPropertyManager->findCProperty("sc_ccm_mcu_sofeware_num");
    mpMcuHardwareNum = mPropertyManager->findCProperty("sc_ccm_mcu_hardware_num");
    mpTimeStatus = mPropertyManager->findCProperty("sc_tbox_time_status");
    mpYear = mPropertyManager->findCProperty("sc_tbox_year");
    mpMonth = mPropertyManager->findCProperty("sc_tbox_month");
    mpDay = mPropertyManager->findCProperty("sc_tbox_day");
    mpHour = mPropertyManager->findCProperty("sc_tbox_hour");
    mpMinute = mPropertyManager->findCProperty("sc_tbox_minute");
    mpSecond = mPropertyManager->findCProperty("sc_tbox_second");
    mpWorkReqc = mPropertyManager->findCProperty("sc_str_work_req_0x66c");
    mpNmBitc = mPropertyManager->findCProperty("sc_ccm_nm_bit_0x66c");
    */
    //std::this_thread::sleep_for(std::chrono::seconds(20));
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    ALOGI("%s %d", __func__,__LINE__ );
    uint16_t messageID;
    BusIdentifier busIdentifier;
    uint8_t command = 0x02;
    uint8_t responseFlag;
    messageID = 0xfffe;
    busIdentifier = BODY_CAN;
    responseFlag = 0x00;
    MessageParser mparse;
    uint8_t tempdata[8];
    memset(tempdata,0,sizeof(tempdata));
    std::vector<uint8_t> message = mparse.makeTRANPack(messageID, tempdata,
                                                   busIdentifier, command, responseFlag);
    //SendCanMap::getInstance()->insert_data(messageID, message, busIdentifier);                 
    std::string buf;
        buf = "";
        for (int i = 0; i < 17; i++) {
            std::ostringstream ss;
            ss.clear();
            int data = message[i];
            ss << std::hex << data;
            buf += "0x";
            buf += ss.str();
            buf += ", ";
        }
    ALOGI("create Send FULL %zu buffer1:%s", message.size(), buf.c_str());
    std::vector<uint8_t> buffer{};
    std::vector<uint8_t> msg;
    std::vector<uint8_t> temp_msg = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    msg.insert(msg.end(), message.begin(), message.end());
    for (int i = 1; i< 60; i++) {
        msg.insert(msg.end(), temp_msg.begin(), temp_msg.end());
    }
    uint32_t spicounter = 0;

    msg.insert(msg.begin(), static_cast<uint8_t>(spicounter & 0xFF));
    msg.insert(msg.begin(), static_cast<uint8_t>((spicounter >> 8) & 0xFF));
    msg.insert(msg.begin(), static_cast<uint8_t>((spicounter >> 16) & 0xFF));
    msg.insert(msg.begin(), static_cast<uint8_t>((spicounter >> 24) & 0xFF));
    ALOGI("%s %d", __func__,__LINE__);
    mManager.transferData(buffer,msg);
    ALOGI("%s %d", __func__,__LINE__);
//    std::lock_guard<std::mutex> lock(mtx); // 获取互斥锁
    SPIready = true; // 设置条件变量为true，表示数据已经准备好了
//    SPIcv.notify_one(); // 唤醒等待的消费者线程
    ALOGI("%s %d", __func__,__LINE__);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    while (true) {
        createCanMsgCCM310();
        createCanMsgCCM311();
        createCanMsgCCM353();
//        createBodyCanMsgCCM353();
        createCanMsgCCM354();
//      createBodyCanMsgCCM354();
//        if(0 == ccm352_is_runing){
//            createCanMsgCCM352();    
//        }
        // test function
        //        createTestMsg();
        //        createCanMsgCCM66B();
        //        createCanMsgCCM353();
        //        createCanMsgCCM354();
        //        createCanMsgCCM321();
        //        if(mCCM558Counter%10==0){
        //            createCanMsgCCM558();
        //        }
        //        if(mCCM630Counter%10==0){
        //            createCanMsgCCM630();
        //        }
	//        createCanMsgCCM66C();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}
void CommConn::createCanMsgCCM353V1_1() {
    struct ev_can_ccm_4_0_ccm_0_x353_t adjust;
    adjust.ccm_drive_mode_req = mPropertyManager->getCProValue(mpDriveModeReq);
    adjust.ccm_rechrgn_lv_req = mPropertyManager->getCProValue(mpRechrgnLvReq);
    adjust.total_odometer = mPropertyManager->getCProValue(mpTotalOdometer);
    adjust.ccm_operat_licence = mPropertyManager->getCProValue(mpOperatLicence);
    adjust.ccm_charging_conditions = mPropertyManager->getCProValue(mpChargingConditions);
    adjust.ccm_bcm_open_door_warn_set = mPropertyManager->getCProValue(mpOpenDoorWarnSet);
    adjust.ccm_version = mPropertyManager->getCProValue(mpCCMVersion);
    adjust.ccm_speed_limit_level = mPropertyManager->getCProValue(mSpeedLimitLevel);
    adjust.ccm_veh_tar_soc = mPropertyManager->getCProValue(mpCCMVehTarSoc);
    adjust.ccm_target_chrg_curr = mPropertyManager->getCProValue(mpCCMTargetChrgCurr);
    

    uint16_t messageID;
    BusIdentifier busIdentifier;
    uint8_t command = 0x02;
    uint8_t responseFlag;
    messageID = 0x353;
    busIdentifier = static_cast<BusIdentifier>(BusIdentifier::EV_CAN);
    responseFlag = 0x00;
    MessageParser mparse;
    std::vector<uint8_t> message = mparse.makePack(messageID, static_cast<void*>(&adjust),
                                                   busIdentifier, command, responseFlag);
    mCanQueue.enqueue(message);
}


void CommConn::createCanMsgCCM354V1_1() {
    struct ev_can_ccm_4_0_ccm_0_x354_t adjust;
    adjust.ccm_creep_ctr_cmd = mPropertyManager->getCProValue(mpCreepCtrCmd);
    adjust.ccm_single_ped_ctr_cmd = mPropertyManager->getCProValue(mpSinglePedCtrCmd);
    adjust.ccm_remt_ctrl_flag = mPropertyManager->getCProValue(mpRemtCtrlFlag);
    adjust.ccm_rema_range_disp_vd = mPropertyManager->getCProValue(mpRemaRangeDispMode);
    adjust.ccm_energy_recov_level_cmd = mPropertyManager->getCProValue(mpEnergyRecovLevelCmd);
    uint16_t messageID;
    BusIdentifier busIdentifier;
    uint8_t command = 0x02;
    uint8_t responseFlag;
    messageID = 0x354;
    busIdentifier = EV_CAN;
    responseFlag = 0x00;
    MessageParser mparse;
    std::vector<uint8_t> message = mparse.makePack(messageID, static_cast<void*>(&adjust),
                                                   busIdentifier, command, responseFlag);
    mCanQueue.enqueue(message);
}

void CommConn::createCanMsgHuBBac() {
    // name                          freq    default
    // CCM_AC_HAVCState              3       00
    // CCM_ACkey                     3       00
    // CCM_HeatKey                   3       00
    // CCM_FreshReciKey              3       00
    // CCM_FrontDefrostKey           3       00
    // CCM_BlowerLevel               NA
    // CCM_AIRDistributionModeReq    3       07
    // CCM_LeftSetTemp               NA
    struct me11_cmx_baccanfd_ots_car_v02_07_20240520_hu_b_bac_t ac;
    // mpBCAvasState

    ac.hu_ac_power_ctl = mPropertyManager->getCProValue(mpAC_HAVCState);
    if (ac.hu_ac_power_ctl != 00) {
        if ((mCountrerCCM_AC_HAVCState % 2 == 0) && (mCountrerCCM_AC_HAVCState != 0)) {
            mPropertyManager->updateCPropertyValue(mpAC_HAVCState, 0);
            mCountrerCCM_AC_HAVCState = 0;
        } else {
            mCountrerCCM_AC_HAVCState++;
        }
    }
    ac.hu_ac_auto_ctl = mPropertyManager->getCProValue(mpACkey);
    if (ac.hu_ac_auto_ctl != 00) {
        if ((mCountrerCCM_ACkey % 2 == 0) && (mCountrerCCM_ACkey != 0)) {
            mPropertyManager->updateCPropertyValue(mpACkey, 0);
            mCountrerCCM_ACkey = 0;
        } else {
            mCountrerCCM_ACkey++;
        }
    }
    ac.hu_ac_state = mPropertyManager->getCProValue(mpHuAcState);
    if (ac.hu_ac_state != 00) {
        if ((mCountrer_HuAcState % 4 == 0) && (mCountrer_HuAcState != 0)) {
            mPropertyManager->updateCPropertyValue(mpHuAcState, 0);
            mCountrer_HuAcState = 0;
        } else {
            mCountrer_HuAcState++;
        }
    }

    ac.hu_bcmota_mode_set = mPropertyManager->getCProValue(mpHuBcmotaModeSet);
    if (ac.hu_bcmota_mode_set != 00) {
        if ((mCountrer_HuBcmotaModeSet % 4 == 0) && (mCountrer_HuBcmotaModeSet != 0)) {
            mPropertyManager->updateCPropertyValue(mpHuBcmotaModeSet, 0);
            mCountrer_HuBcmotaModeSet = 0;
        } else {
            mCountrer_HuBcmotaModeSet++;
        }
    }
    ac.tbox_ac_remt_ctrl_flag = mPropertyManager->getCProValue(mpRemtCtrlFlag);
    if (ac.tbox_ac_remt_ctrl_flag != 00) {
        if ((mCountrerCCM_RemtCtrlFlag % 2 == 0) && (mCountrerCCM_RemtCtrlFlag != 0)) {
            mPropertyManager->updateCPropertyValue(mpRemtCtrlFlag, 0);
            mCountrerCCM_RemtCtrlFlag = 0;
        } else {
            mCountrerCCM_RemtCtrlFlag++;
        }
    }
    ac.hu_air_inlet_mode_set = mPropertyManager->getCProValue(mpFreshReciKey);
    if (ac.hu_air_inlet_mode_set != 00) {
        if ((mCountrerCCM_FreshReciKey % 2 == 0) && (mCountrerCCM_FreshReciKey != 0)) {
            mPropertyManager->updateCPropertyValue(mpFreshReciKey, 0);
            mCountrerCCM_FreshReciKey = 0;
        } else {
            mCountrerCCM_FreshReciKey++;
        }
    }
    ac.hu_manual_defrost_ctl = mPropertyManager->getCProValue(mpFrontDefrostKey);
    if (ac.hu_manual_defrost_ctl != 00) {
        if ((mCountrerCCM_FrontDefrostKey % 2 == 0) && (mCountrerCCM_FrontDefrostKey != 0)) {
            mPropertyManager->updateCPropertyValue(mpFrontDefrostKey, 0);
            mCountrerCCM_FrontDefrostKey = 0;
        } else {
            mCountrerCCM_FrontDefrostKey++;
        }
    }
    int32_t blowl_value = static_cast<int32_t>(mPropertyManager->getCProValue(mpBlowerLevel));
    if (blowl_value != 0) {
        ac.hu_front_fan_spd_set = blowl_value;
        mCountrerCCM_BlowerLevel++;
        if (mCountrerCCM_BlowerLevel == 3) {
            mPropertyManager->updateCPropertyValue(mpBlowerLevel, 0);
            mCountrerCCM_BlowerLevel = 0;
        }
    }
    int32_t blowm_value = static_cast<int32_t>(mPropertyManager->getCProValue(mpAIRDistributionModeReq));
    if(blowm_value != 0) {
        ac.hu_blower_mode_set = blowm_value;
        mCountrerCCM_AIRDistributionModeReq++;
        if (mCountrerCCM_AIRDistributionModeReq == 3) {
            mPropertyManager->updateCPropertyValue(mpAIRDistributionModeReq, 0);
            mCountrerCCM_AIRDistributionModeReq = 0;
        }
    }
    ac.hu_hybrid_eco_mode_set = mPropertyManager->getCProValue(mpHybridEcoMode);
    if((mCountrerHu_HibridEcoModeSet % 2 == 0) && (mCountrerHu_HibridEcoModeSet != 0)) {
        mPropertyManager->updateCPropertyValue(mpHybridEcoMode, 0);
        mCountrerHu_HibridEcoModeSet = 0;
    } else {
        mCountrerHu_HibridEcoModeSet++;
    }
    int32_t t_value = static_cast<int32_t>(mPropertyManager->getCProValue(mpLeftSetTemp));
    if(t_value < 100) {
        if(t_value != 0) {
            ac.hu_left_temp_set = t_value;
            mCountrerCCM_TboxAcTempSet++;
            if(mCountrerCCM_TboxAcTempSet == 3) {
                mPropertyManager->updateCPropertyValue(mpLeftSetTemp, 0);
                mCountrerCCM_TboxAcTempSet = 0;
            }
        }
    } else {
       if(t_value != 100) {
           ac.tbox_ac_temp_set = t_value - 100;
            mCountrerCCM_TboxAcTempSet++;
            if(mCountrerCCM_TboxAcTempSet == 3) {
                mPropertyManager->updateCPropertyValue(mpLeftSetTemp, 0);
                mCountrerCCM_TboxAcTempSet = 0;
            }
       }
    }
    ac.hu_pwr_off_req = mPropertyManager->getCProValue(mpPwrOffReq);
    if((mCountrerHu_PwrOffReq % 2 == 0) && (mCountrerHu_PwrOffReq != 0)) {
        mPropertyManager->updateCPropertyValue(mpPwrOffReq, 0);
        mCountrerHu_PwrOffReq = 0;
    } else {
        mCountrerHu_PwrOffReq++;
    }
    int32_t wipr_value = static_cast<int32_t>(mPropertyManager->getCProValue(mpWiperIntervalTime));
    if(wipr_value != 0) {
         ac.hu_wipr_intl_time_set = wipr_value;
         mCountrerHu_WiperIntervalTime++;
         if(mCountrerHu_WiperIntervalTime == 3){
             mPropertyManager->updateCPropertyValue(mpWiperIntervalTime, 0);
             mCountrerHu_WiperIntervalTime = 0;
         }
    }
    int32_t flwr_value = static_cast<int32_t>(mPropertyManager->getCProValue(mpFlwrMeHmCtrl));
    if(flwr_value != 0) {
        ac.hu_flwr_me_hm_ctrl = flwr_value;
        mCountrerHu_FlwrMeHmCtrl++;
        if(mCountrerHu_FlwrMeHmCtrl == 3) {
            mPropertyManager->updateCPropertyValue(mpFlwrMeHmCtrl, 0);
            mCountrerHu_FlwrMeHmCtrl = 0;
        }
    }
    int32_t park_value = static_cast<int32_t>(mPropertyManager->getCProValue(mpParkUnlockSet));
    if(park_value != 0) {
        ac.hu_park_auto_unlck_set = park_value;
        mCountrerHu_ParkUnlockSet++;
        if(mCountrerHu_ParkUnlockSet == 3) {
            mPropertyManager->updateCPropertyValue(mpParkUnlockSet, 0);
            mCountrerHu_ParkUnlockSet = 0;
	}
    }
    ac.hu_illmnd_unlock_sts = mPropertyManager->getCProValue(mpIllmndUnLockSts);
    if((mCountrerHu_IllmndUnLockSts % 2 == 0) && (mCountrerHu_IllmndUnLockSts != 0)) {
        mPropertyManager->updateCPropertyValue(mpIllmndUnLockSts, 0);
        mCountrerHu_IllmndUnLockSts = 0;
    } else {
        mCountrerHu_IllmndUnLockSts++;
    }
    ac.hu_illmnd_lock_sts = mPropertyManager->getCProValue(mpIllmndLockSts);
    if((mCountrerHu_IllmndLockSts % 2 == 0) && (mCountrerHu_IllmndLockSts != 0)) {
        mPropertyManager->updateCPropertyValue(mpIllmndLockSts, 0);
        mCountrerHu_IllmndLockSts = 0;
    } else {
        mCountrerHu_IllmndLockSts++;
    }
    ac.hu_lock_horn_on_st = mPropertyManager->getCProValue(mpLockHornOnSt);
    if((mCountrerHu_LockHornOnSt % 2 == 0) && (mCountrerHu_LockHornOnSt != 0)) {
        mPropertyManager->updateCPropertyValue(mpLockHornOnSt, 0);
        mCountrerHu_LockHornOnSt = 0;
    } else {
        mCountrerHu_LockHornOnSt++;
    }
    ac.hu_peristalsis_mode_enb = mPropertyManager->getCProValue(mpCreepCtrCmd);
    if((mCountrerHu_CreepCtrCmd % 2 == 0) && (mCountrerHu_CreepCtrCmd != 0)) {
        mPropertyManager->updateCPropertyValue(mpCreepCtrCmd, 0);
        mCountrerHu_CreepCtrCmd = 0;
    } else {
        mCountrerHu_CreepCtrCmd++;
    }
    int32_t drvm_value = static_cast<int32_t>(mPropertyManager->getCProValue(mpDriveModeReq));
    if(park_value != 0) {
        ac.hu_driving_mode_select = drvm_value;
        mCountrerHu_DrivingModeSelect++;
        if(mCountrerHu_DrivingModeSelect == 3) {
            mPropertyManager->updateCPropertyValue(mpDriveModeReq, 0);
            mCountrerHu_DrivingModeSelect = 0;
        }
    }
    int32_t recup_value = static_cast<int32_t>(mPropertyManager->getCProValue(mpEnergyRecovLevelCmd));
    if(recup_value != 0x65) {
        ac.hu_recuperate_power_set = mPropertyManager->getCProValue(mpEnergyRecovLevelCmd);
        mCountrerHu_RecuperatePowerSet++;
        if(mCountrerHu_RecuperatePowerSet == 3) {
            mPropertyManager->updateCPropertyValue(mpEnergyRecovLevelCmd, 0x65);
            mCountrerHu_RecuperatePowerSet = 0;
        }
    }
    ac.hu_driving_power_set = mPropertyManager->getCProValue(mpDrivingPowerSt);
    if((mCountrerHu_DrivingPowerSet % 2 == 0) && (mCountrerHu_DrivingPowerSet != 0)) {
        mPropertyManager->updateCPropertyValue(mpDrivingPowerSt, 0x65);
        mCountrerHu_DrivingPowerSet = 0;
    } else {
        mCountrerHu_DrivingPowerSet++;
    }

    ac.t_box_remote_lock = mPropertyManager->getCProValue(mpCCMTboxRemoteLock);
    if((mCountrerHu_RemoteLock % 2 == 0) && (mCountrerHu_RemoteLock != 0)) {
        mPropertyManager->updateCPropertyValue(mpCCMTboxRemoteLock, 0);
        mCountrerHu_RemoteLock = 0;
    } else {
        mCountrerHu_RemoteLock++;
    }

    ac.t_box_remote_car_search = mPropertyManager->getCProValue(mpCCMTboxRemoteCarSearch);
    if((mCountrerHu_RemoteSearch % 2 == 0) && (mCountrerHu_RemoteSearch != 0)) {
        mPropertyManager->updateCPropertyValue(mpCCMTboxRemoteCarSearch, 0);
        mCountrerHu_RemoteSearch = 0;
    } else {
        mCountrerHu_RemoteSearch++;
    }
    ac.hu_veh_max_current = mPropertyManager->getCProValue(mpHuVehMaxCurrent);
    if((mCountrerHu_VehMaxCurrent % 2 == 0) && (mCountrerHu_VehMaxCurrent != 0)) {
        mPropertyManager->updateCPropertyValue(mpHuVehMaxCurrent, 0);
        mCountrerHu_VehMaxCurrent = 0;
    } else {
        mCountrerHu_VehMaxCurrent++;
    }
    ac.hu_ac_unlock_vent_set = mPropertyManager->getCProValue(mpAcUnlockVentSet);
    if((mCountrerAc_UnlockVentSet % 2 == 0) && (mCountrerAc_UnlockVentSet != 0)) {
        mPropertyManager->updateCPropertyValue(mpAcUnlockVentSet, 0);
        mCountrerAc_UnlockVentSet = 0;
    } else {
        mCountrerAc_UnlockVentSet++;
    }
    ac.cdc_back_ladjval = mPropertyManager->getCProValue(mpCdcBackLadjval);
    if((mCountrer_CdcBackLadjval % 2 == 0) && (mCountrer_CdcBackLadjval!=0)) {
        mPropertyManager->updateCPropertyValue(mpCdcBackLadjval, 0);
        mCountrer_CdcBackLadjval = 0;
    } else {
        mCountrer_CdcBackLadjval++;
    }
    ac.hu_tpms_reset_req = mPropertyManager->getCProValue(mpHuTpmsResetReq);
    if((mCountrer_HuTpmsResetReq % 2 == 0) && (mCountrer_HuTpmsResetReq!=0)) {
        mPropertyManager->updateCPropertyValue(mpHuTpmsResetReq, 0);
        mCountrer_HuTpmsResetReq = 0;
    } else {
        mCountrer_HuTpmsResetReq++;
    }

    ac.hu_mai_drvr_seat_heat = mPropertyManager->getCProValue(mpHuMaiDrvrSeatHeat);
    if((mCountrer_HuMaiDrvrSeatHeat % 2 == 0) && (mCountrer_HuMaiDrvrSeatHeat!=0)) {
        mPropertyManager->updateCPropertyValue(mpHuMaiDrvrSeatHeat, 0);
        mCountrer_HuMaiDrvrSeatHeat = 0;
    } else {
        mCountrer_HuMaiDrvrSeatHeat++;
    }
    ac.hu_steer_whl_heat = mPropertyManager->getCProValue(mpHuSteerWhlHeat);
    if((mCountrer_HuSteerWhlHeat % 2 == 0) && (mCountrer_HuSteerWhlHeat!=0)) {
        mPropertyManager->updateCPropertyValue(mpHuSteerWhlHeat, 0);
        mCountrer_HuSteerWhlHeat = 0;
    } else {
        mCountrer_HuSteerWhlHeat++;
    }
    ac.tbox_remote_power_lock = mPropertyManager->getCProValue(mpTboxRemotePowerLock);
    if((mCountrer_TboxRemotePowerLock % 2 == 0) && (mCountrer_TboxRemotePowerLock!=0)) {
        mPropertyManager->updateCPropertyValue(mpTboxRemotePowerLock, 0);
        mCountrer_TboxRemotePowerLock = 0;
    } else {
        mCountrer_TboxRemotePowerLock++;
    }
    ac.tbox_remote_power_lock_v = mPropertyManager->getCProValue(mpTboxRemotePowerLockV);
    if((mCountrer_TboxRemotePowerLockV % 2 == 0) && (mCountrer_TboxRemotePowerLockV!=0)) {
        mPropertyManager->updateCPropertyValue(mpTboxRemotePowerLockV, 0);
        mCountrer_TboxRemotePowerLockV = 0;
    } else {
        mCountrer_TboxRemotePowerLockV++;
    }
    ac.tbox_remt_steer_whl_heat_req = mPropertyManager->getCProValue(mpTboxRemtSteerWhlHeatReq);
    if((mCountrer_TboxRemtSteerWhlHeatReq % 2 == 0) && (mCountrer_TboxRemtSteerWhlHeatReq!=0)) {
        mPropertyManager->updateCPropertyValue(mpTboxRemtSteerWhlHeatReq, 0);
        mCountrer_TboxRemtSteerWhlHeatReq = 0;
    } else {
        mCountrer_TboxRemtSteerWhlHeatReq++;
    }
    ac.tbox_remote_defrost_ctl = mPropertyManager->getCProValue(mpTboxRemoteDefrostCtl);
    if((mCountrer_TboxRemoteDefrostCtl % 2 == 0) && (mCountrer_TboxRemoteDefrostCtl!=0)) {
        mPropertyManager->updateCPropertyValue(mpTboxRemoteDefrostCtl, 0);
        mCountrer_TboxRemoteDefrostCtl = 0;
    } else {
        mCountrer_TboxRemoteDefrostCtl++;
    }
    ac.tbox_remt_power_ctrl_req = mPropertyManager->getCProValue(mpTboxRemtPowerCtrlReq);
    if((mCountrer_TboxRemtPowerCtrlReq % 2 == 0) && (mCountrer_TboxRemtPowerCtrlReq!=0)) {
        mPropertyManager->updateCPropertyValue(mpTboxRemtPowerCtrlReq, 0);
        mCountrer_TboxRemtPowerCtrlReq = 0;
    } else {
        mCountrer_TboxRemtPowerCtrlReq++;
    }
    ac.tbox_remt_mai_dr_seat_heat_req = mPropertyManager->getCProValue(mpTboxRemtMaiDrSeatHeatReq);
    if((mCountrer_TboxRemtMaiDrSeatHeatReq % 2 == 0) && (mCountrer_TboxRemtMaiDrSeatHeatReq!=0)) {
        mPropertyManager->updateCPropertyValue(mpTboxRemtMaiDrSeatHeatReq, 0);
        mCountrer_TboxRemtMaiDrSeatHeatReq = 0;
    } else {
        mCountrer_TboxRemtMaiDrSeatHeatReq++;
    }
    ac.hu_bcm_cargo_light_set = mPropertyManager->getCProValue(mpHuBcmCargoLightSet);
    if((mCountrer_HuBcmCargoLightSet % 2 == 0) && (mCountrer_HuBcmCargoLightSet!=0)) {
        mPropertyManager->updateCPropertyValue(mpHuBcmCargoLightSet, 0);
        mCountrer_HuBcmCargoLightSet = 0;
    } else {
        mCountrer_HuBcmCargoLightSet++;
    }
    ac.tbox_vcu_version = mPropertyManager->getCProValue(mpCCMVersion);
    ac.tbox_vcu_operat_licence = mPropertyManager->getCProValue(mpOperatLicence);
    ac.tbox_vcu_charging_conditions = mPropertyManager->getCProValue(mpChargingConditions);
    ac.tbox_vcu_ccm_speed_limit_level = mPropertyManager->getCProValue(mSpeedLimitLevel);
    uint16_t messageID;
    BusIdentifier busIdentifier;
    uint8_t command = 0x02;
    uint8_t responseFlag;
    messageID = 0x01D0;
    busIdentifier = EV_CAN;
    responseFlag = 0x00;
    MessageParser mparse;
    std::vector<uint8_t> message = mparse.makePack(messageID, static_cast<void*>(&ac),
                                                   busIdentifier, command, responseFlag);
    std::string buf;
    buf = "";
    for (int i = 0; i < 41; i++) {
        std::ostringstream ss;
        ss.clear();
        int data = message[i];
        ss << std::hex << data;
        buf += "0x";
        buf += ss.str();
         buf += ", ";
    }
    if (DebugManager::getInstance().isOpenDebug()) {
        ALOGI("createCanMsgHuBBac can %zu buffer1:%s", message.size(), buf.c_str());
    }
    uint8_t sendBuf[SPI_PACKAGE_SIZE];
    sendBuf[0] = busIdentifier;
    sendBuf[1] = (messageID >> 8) & 0xff;
    sendBuf[2] = messageID & 0xff;
    sendBuf[3] = message.size() - 9;
    memcpy(&sendBuf[4],message.data()+8,message.size()-9);
    pb_add_raw_vary_data(sendBuf,message.size()+4-9,0x01);
}

void CommConn::createCanMsgHuABac() {
    struct me11_cmx_baccanfd_ots_car_v02_07_20240520_hu_a_bac_t ac;
    ac.hu_esc_func_ena_sta_resvd = mPropertyManager->getCProValue(mpEscFuncEnaStaResvd);
    if((mCountrerHu_EscFuncEnaStaResvd % 2 == 0) && (mCountrerHu_EscFuncEnaStaResvd != 0)) {
        mPropertyManager->updateCPropertyValue(mpEscFuncEnaStaResvd, 0);
        mCountrerHu_EscFuncEnaStaResvd = 0;
    } else {
        mCountrerHu_EscFuncEnaStaResvd++;
    }

    ac.hu_avh_enb = mPropertyManager->getCProValue(mpAvhEnb);
    if((mCountrerHu_AvhEnb % 2 == 0) && (mCountrerHu_AvhEnb != 0)) {
        mPropertyManager->updateCPropertyValue(mpAvhEnb, 0);
        mCountrerHu_AvhEnb = 0;
    } else {
        mCountrerHu_AvhEnb++;
    }
    ac.hu_epb_modal = mPropertyManager->getCProValue(mpEpbModal);
    if((mCountrerHu_EpbModal % 2 == 0) && (mCountrerHu_EpbModal != 0)) {
        mPropertyManager->updateCPropertyValue(mpEpbModal, 0x3);
        mCountrerHu_EpbModal = 0;
    } else {
        mCountrerHu_EpbModal++;
    }
    ac.hu_brk_dis_wip_sw = mPropertyManager->getCProValue(mpBrkDisWipSw);
    ac.hu_elec_steer_mode_set = mPropertyManager->getCProValue(mpElecSteerModeSet);
    ac.hu_aeb_enable_sw = mPropertyManager->getCProValue(mpHuAebEnableSw);
    if((mCountrerHu_AebEnableSw % 2 == 0) && (mCountrerHu_AebEnableSw != 0)) {
        mPropertyManager->updateCPropertyValue(mpHuAebEnableSw, 0);
        mCountrerHu_AebEnableSw = 0;
    } else {
        mCountrerHu_AebEnableSw++;
    }
    ac.hu_elk_enb_sw = mPropertyManager->getCProValue(mpHuElkEnbSw);
    if((mCountrerHu_ElkEnbSw % 2 == 0) && (mCountrerHu_ElkEnbSw != 0)) {
        mPropertyManager->updateCPropertyValue(mpHuElkEnbSw, 0);
        mCountrerHu_ElkEnbSw = 0;
    } else {
        mCountrerHu_ElkEnbSw++;
    }
    ac.hu_ldw_enable_sw = mPropertyManager->getCProValue(mpHuLdwEnableSw);
    if((mCountrerHu_LdwEnableSw % 2 == 0) && (mCountrerHu_LdwEnableSw != 0)) {
        mPropertyManager->updateCPropertyValue(mpHuLdwEnableSw, 0);
        mCountrerHu_LdwEnableSw = 0;
    } else {
        mCountrerHu_LdwEnableSw++;
    }
    ac.hu_lka_enable_sw = mPropertyManager->getCProValue(mpHuLkaEnableSw);
    if((mCountrerHu_LkaEnableSw % 2 == 0) && (mCountrerHu_LkaEnableSw != 0)) {
        mPropertyManager->updateCPropertyValue(mpHuLkaEnableSw, 0);
        mCountrerHu_LkaEnableSw = 0;
    } else {
        mCountrerHu_LkaEnableSw++;
    }
    ac.hu_rcta_enable_sw = mPropertyManager->getCProValue(mpHuRctaEnableSw);
    if((mCountrerHu_RctaEnableSw % 2 == 0) && (mCountrerHu_RctaEnableSw != 0)) {
        mPropertyManager->updateCPropertyValue(mpHuRctaEnableSw, 0);
        mCountrerHu_RctaEnableSw = 0;
    } else {
        mCountrerHu_RctaEnableSw++;
    }
    ac.hu_bsd_enable_sw = mPropertyManager->getCProValue(mpHuBsdEnableSw);
    if((mCountrerHu_BsdEnableSw % 2 == 0) && (mCountrerHu_BsdEnableSw != 0)) {
        mPropertyManager->updateCPropertyValue(mpHuBsdEnableSw, 0);
        mCountrerHu_BsdEnableSw = 0;
    } else {
        mCountrerHu_BsdEnableSw++;
    }
    ac.hu_dow_ena_sw = mPropertyManager->getCProValue(mpHuDowEnaSw);
    if((mCountrerHu_DowEnaSw % 2 == 0) && (mCountrerHu_DowEnaSw != 0)) {
        mPropertyManager->updateCPropertyValue(mpHuDowEnaSw, 0);
        mCountrerHu_DowEnaSw = 0;
    } else {
        mCountrerHu_DowEnaSw++;
    }
    ac.hu_rcw_enable_sw = mPropertyManager->getCProValue(mpHuRcwEnableSw);
    if((mCountrerHu_RcwEnableSw % 2 == 0) && (mCountrerHu_RcwEnableSw != 0)) {
        mPropertyManager->updateCPropertyValue(mpHuRcwEnableSw, 0);
        mCountrerHu_RcwEnableSw = 0;
    } else {
        mCountrerHu_RcwEnableSw++;
    }
    ac.hu_spd_downin_cor_enable_sw = mPropertyManager->getCProValue(mpHuSpdDowninCorEnableSw);
    if((mCountrerHu_SpdDowninCorEnableSw % 2 == 0) && (mCountrerHu_SpdDowninCorEnableSw != 0)) {
        mPropertyManager->updateCPropertyValue(mpHuSpdDowninCorEnableSw, 0);
        mCountrerHu_SpdDowninCorEnableSw = 0;
    } else {
        mCountrerHu_SpdDowninCorEnableSw++;
    }


    ac.hu_slwf_enable_sw = mPropertyManager->getCProValue(mphu_slwf_enable_sw);
    if((mCountrer_hu_slwf_enable_sw % 2 == 0) && (mCountrer_hu_slwf_enable_sw != 0)) {
        mPropertyManager->updateCPropertyValue(mphu_slwf_enable_sw, 0);
        mCountrer_hu_slwf_enable_sw = 0;
    } else {
        mCountrer_hu_slwf_enable_sw++;
    }

    ac.hu_slif_enable_sw = mPropertyManager->getCProValue(mphu_slif_enable_sw);
    if((mCountrer_hu_slif_enable_sw % 2 == 0) && (mCountrer_hu_slif_enable_sw != 0)) {
        mPropertyManager->updateCPropertyValue(mphu_slif_enable_sw, 0);
        mCountrer_hu_slif_enable_sw = 0;
    } else {
        mCountrer_hu_slif_enable_sw++;
    }

    ac.hu_fcw_enable_sw = mPropertyManager->getCProValue(mphu_fcw_enable_sw);
    if((mCountrer_hu_fcw_enable_sw % 2 == 0) && (mCountrer_hu_fcw_enable_sw != 0)) {
        mPropertyManager->updateCPropertyValue(mphu_fcw_enable_sw, 0);
        mCountrer_hu_fcw_enable_sw = 0;
    } else {
        mCountrer_hu_fcw_enable_sw++;
    }

    ac.hu_ldw_alert_method_sw = mPropertyManager->getCProValue(mphu_ldw_alert_method_sw);
    if((mCountrer_hu_ldw_alert_method_sw % 2 == 0) && (mCountrer_hu_ldw_alert_method_sw != 0)) {
        mPropertyManager->updateCPropertyValue(mphu_ldw_alert_method_sw, 0);
        mCountrer_hu_ldw_alert_method_sw = 0;
    } else {
        mCountrer_hu_ldw_alert_method_sw++;
    }

    ac.hu_ica_avd_truck_set = mPropertyManager->getCProValue(mphu_ica_avd_truck_set);
    if((mCountrer_hu_ica_avd_truck_set % 2 == 0) && (mCountrer_hu_ica_avd_truck_set != 0)) {
        mPropertyManager->updateCPropertyValue(mphu_ica_avd_truck_set, 0);
        mCountrer_hu_ica_avd_truck_set = 0;
    } else {
        mCountrer_hu_ica_avd_truck_set++;
    }

    ac.hu_ica_ena_swt = mPropertyManager->getCProValue(mphu_ica_ena_swt);
    if((mCountrer_hu_ica_ena_swt % 2 == 0) && (mCountrer_hu_ica_ena_swt != 0)) {
        mPropertyManager->updateCPropertyValue(mphu_ica_ena_swt, 0);
        mCountrer_hu_ica_ena_swt = 0;
    } else {
        mCountrer_hu_ica_ena_swt++;
    }
    
    ac.hu_hma_enable_sw = mPropertyManager->getCProValue(mpHuHmaEnableSw);
    if((mCountrer_HuHmaEnableSw % 2 == 0) && (mCountrer_HuHmaEnableSw != 0)) {
        mPropertyManager->updateCPropertyValue(mpHuHmaEnableSw, 0);
        mCountrer_HuHmaEnableSw = 0;
    } else {
        mCountrer_HuHmaEnableSw++;
    }

    ac.hu_navi_country_code = mPropertyManager->getCProValue(mpHuNaviCountryCode);
    ac.hu_nav_country_id = mPropertyManager->getCProValue(mpHuNavCountryId);
    //ALOGI("hu_navi_country_code:%d", ac.hu_navi_country_code );

    ac.hu_req_epb_vld = 0x1;
    ac.hu_v_set_dis = 0x7f;
    ac.hu_dms_head_roll_agl = 0x5a;
    ac.hu_dms_head_yaw_agl = 0xb4;
    ac.hu_dms_head_pitch_agl = 0xb4;
    ac.hu_dms_sgt_hozl_agl = 0xb4;
    ac.hu_dms_sgt_vert_agl = 0xb4;
    uint16_t messageID;
    BusIdentifier busIdentifier;
    uint8_t command = 0x02;
    uint8_t responseFlag;
    messageID = 0x01EB;
    busIdentifier = EV_CAN;
    responseFlag = 0x00;
    MessageParser mparse;
    std::vector<uint8_t> message = mparse.makePack(messageID, static_cast<void*>(&ac),
                                                   busIdentifier, command, responseFlag);
    std::string buf;
    buf = "";
    for (int i = 0; i < 73; i++) {
        std::ostringstream ss;
        ss.clear();
        int data = message[i];
        ss << std::hex << data;
        buf += "0x";
        buf += ss.str();
         buf += ", ";
    }
    if (DebugManager::getInstance().isOpenDebug()) {
        ALOGI("createCanMsgHuABac can %zu buffer1:%s", message.size(), buf.c_str());
    }
    uint8_t sendBuf[SPI_PACKAGE_SIZE];
    sendBuf[0] = busIdentifier;
    sendBuf[1] = (messageID >> 8) & 0xff;
    sendBuf[2] = messageID & 0xff;
    sendBuf[3] = message.size() - 9;
    memcpy(&sendBuf[4],message.data()+8,message.size()-9);
    pb_add_raw_vary_data(sendBuf,message.size()+4-9,0x01);
}

void CommConn::createCanMsgHuCBac() {
    struct me11_cmx_baccanfd_ots_car_v02_07_20240520_hu_c_bac_t ac;
    ac.hu_avas_set_key = mPropertyManager->getCProValue(mpHuAvasSetKey);
    if((mCountrerHu_AvasSetKey % 2 == 0) && (mCountrerHu_AvasSetKey != 0)) {
        mPropertyManager->updateCPropertyValue(mpHuAvasSetKey, 0);
        mCountrerHu_AvasSetKey = 0;
    } else {
        mCountrerHu_AvasSetKey++;
    }
    ac.hu_tpms_learning_req = mPropertyManager->getCProValue(mpHuTpmsLearningReq);
    if((mCountrerHu_TpmsLearningReq % 2 == 0) && (mCountrerHu_TpmsLearningReq != 0)) {
        mPropertyManager->updateCPropertyValue(mpHuTpmsLearningReq, 0);
        mCountrerHu_TpmsLearningReq = 0;
    } else {
        mCountrerHu_TpmsLearningReq++;
    }
    ac.hu_veh_tar_soc = mPropertyManager->getCProValue(mpHuVehTarSoc);
    if((mCountrerHu_VehTarSoc % 2 == 0) && (mCountrerHu_VehTarSoc != 0)) {
        mPropertyManager->updateCPropertyValue(mpHuVehTarSoc, 0);
        mCountrerHu_VehTarSoc = 0;
    } else {
        mCountrerHu_VehTarSoc++;
    }
    uint16_t messageID;
    BusIdentifier busIdentifier;
    uint8_t command = 0x02;
    uint8_t responseFlag;
    messageID = 0x02FB;
    busIdentifier = EV_CAN;
    responseFlag = 0x00;
    MessageParser mparse;
    std::vector<uint8_t> message = mparse.makePack(messageID, static_cast<void*>(&ac),
                                                   busIdentifier, command, responseFlag);
    std::string buf;
    buf = "";
    for (int i = 0; i < 17; i++) {
        std::ostringstream ss;
        ss.clear();
        int data = message[i];
        ss << std::hex << data;
        buf += "0x";
        buf += ss.str();
         buf += ", ";
    }
    if (DebugManager::getInstance().isOpenDebug()) {
        ALOGI("createCanMsgHuCBac can %zu buffer1:%s", message.size(), buf.c_str());
    }
    uint8_t sendBuf[SPI_PACKAGE_SIZE];
    sendBuf[0] = busIdentifier;
    sendBuf[1] = (messageID >> 8) & 0xff;
    sendBuf[2] = messageID & 0xff;
    sendBuf[3] = message.size() - 9;
    memcpy(&sendBuf[4],message.data()+8,message.size()-9);
    pb_add_raw_vary_data(sendBuf,message.size()+4-9,0x01);
}

void CommConn::createCanMsgFcmABac() {
    struct me11_cmx_baccanfd_ots_car_v02_07_20240520_fcm_a_bac_t ac;
    ac.fcm_hma_highbeam_req = 0;
    ac.fcm_aeb_rec_req = 0;
    ac.fcm_adas_drvr_req = 0;
    ac.fcm_tja_takeover_req = 0;
    ac.fcm_tau_gap_set = 0;
    uint16_t messageID;
    BusIdentifier busIdentifier;
    uint8_t command = 0x02;
    uint8_t responseFlag;
    messageID = 0x0175;
    busIdentifier = EV_CAN;
    responseFlag = 0x00;
    MessageParser mparse;
    std::vector<uint8_t> message = mparse.makePack(messageID, static_cast<void*>(&ac),
                                                   busIdentifier, command, responseFlag);
    std::string buf;
    buf = "";
    for (int i = 0; i < 59; i++) {
        std::ostringstream ss;
        ss.clear();
        int data = message[i];
        ss << std::hex << data;
        buf += "0x";
        buf += ss.str();
         buf += ", ";
    }
    ALOGI("createCanMsgFcmABac can %zu buffer1:%s", message.size(), buf.c_str());
    uint8_t sendBuf[SPI_PACKAGE_SIZE];
    sendBuf[0] = busIdentifier;
    sendBuf[1] = (messageID >> 8) & 0xff;
    sendBuf[2] = messageID & 0xff;
    sendBuf[3] = message.size() - 9;
    memcpy(&sendBuf[4],message.data()+8,message.size()-9);
    pb_add_raw_vary_data(sendBuf,message.size()+4-9,0x01);
}

void CommConn::createCanMsgNMAsrCdcBac() {
    struct me11_cmx_baccanfd_ots_car_v02_07_20240520_nm_asr_cdc_bac_t ac;
    ac.cdc_node_address = 0x22;
    int sendCAWUTSignalTimes = mPropertyManager->getCProValue(mpCdcActiveWakeUpTimeout) * 10;
    ac.cdc_active_wakeup = mPropertyManager->getCProValue(mpCdcActiveWakeUp);
    if(sendCAWUTSignalTimes) {
        if(ac.cdc_active_wakeup & (sendCAWUTSignalTimes < mCountrerCdc_ActiveWakeUp)) {
            mPropertyManager->updateCPropertyValue(mpCdcActiveWakeUp, 0);
            mPropertyManager->updateCPropertyValue(mpCdcActiveWakeUpTimeout, 0);
            mCountrerCdc_ActiveWakeUp = 0;
        } else {
            mCountrerCdc_ActiveWakeUp++;
        }
    }
    int sendCWURTSignalTimes = mPropertyManager->getCProValue(mpCdcWakeUpReasTimeout) * 10;
    ac.cdc_wakeup_reas = mPropertyManager->getCProValue(mpCdcWakeUpReas);
    if(sendCWURTSignalTimes) {
        if(ac.cdc_wakeup_reas & (sendCWURTSignalTimes < mCountrerCdc_WakeUpReas)) {
            mPropertyManager->updateCPropertyValue(mpCdcWakeUpReas, 0);
            mPropertyManager->updateCPropertyValue(mpCdcWakeUpReasTimeout, 0);
            mCountrerCdc_WakeUpReas = 0;
        } else {
            mCountrerCdc_WakeUpReas++;
        }
    }
    int sendCKARRRTSignalTimes = mPropertyManager->getCProValue(mpCdcKeepAwakeReasRemReqTimeout) * 10;
    ac.cdc_keep_awake_reas_rem_req = mPropertyManager->getCProValue(mpCdcKeepAwakeReasRemReq);
    if(sendCKARRRTSignalTimes) {
        if(ac.cdc_keep_awake_reas_rem_req & (sendCKARRRTSignalTimes < mCountrerCdc_KeepAwakeReasRemReq)) {
            mPropertyManager->updateCPropertyValue(mpCdcKeepAwakeReasRemReq, 0);
            mPropertyManager->updateCPropertyValue(mpCdcKeepAwakeReasRemReqTimeout, 0);
            mCountrerCdc_KeepAwakeReasRemReq = 0;
        } else {
            mCountrerCdc_KeepAwakeReasRemReq++;
        }
    }

    int sendCKARFRTSignalTimes = mPropertyManager->getCProValue(mpCdcKeepAwakeReasFacResetTimeout) * 10;
    ac.cdc_keep_awake_reas_fac_reset = mPropertyManager->getCProValue(mpCdcKeepAwakeReasFacReset);
    if(sendCKARFRTSignalTimes) {
        if(ac.cdc_keep_awake_reas_fac_reset & (sendCKARFRTSignalTimes < mCountrerCdc_KeepAwakeReasFacReset)) {
            mPropertyManager->updateCPropertyValue(mpCdcKeepAwakeReasFacReset, 0);
            mPropertyManager->updateCPropertyValue(mpCdcKeepAwakeReasFacResetTimeout, 0);
            mCountrerCdc_KeepAwakeReasFacReset = 0;
        } else {
            mCountrerCdc_KeepAwakeReasFacReset++;
        }
    }
    int sendCKARRTSignalTimes = mPropertyManager->getCProValue(mpCdcKeepAwakeReasRestartTimeout) * 10;
    ac.cdc_keep_awake_reas_restart = mPropertyManager->getCProValue(mpCdcKeepAwakeReasRestart);
    if(sendCKARRTSignalTimes) {
        if(ac.cdc_keep_awake_reas_restart & (sendCKARRTSignalTimes < mCountrerCdc_KeepAwakeReasRestart)) {
            mPropertyManager->updateCPropertyValue(mpCdcKeepAwakeReasRestart, 0);
            mPropertyManager->updateCPropertyValue(mpCdcKeepAwakeReasRestartTimeout, 0);
            mCountrerCdc_KeepAwakeReasRestart = 0;
        } else {
            mCountrerCdc_KeepAwakeReasRestart++;
        }
    }
    int sendCKARDOTSignalTimes = mPropertyManager->getCProValue(mpCdcKeepAwakeReasDOTimeout) * 10;
    ac.cdc_keep_awake_reas_do = mPropertyManager->getCProValue(mpCdcKeepAwakeReasDO);
    if(sendCKARDOTSignalTimes) {
        if(ac.cdc_keep_awake_reas_do & (sendCKARDOTSignalTimes < mCountrerCdc_KeepAwakeReasDO)) {
            mPropertyManager->updateCPropertyValue(mpCdcKeepAwakeReasDO, 0);
            mPropertyManager->updateCPropertyValue(mpCdcKeepAwakeReasDOTimeout, 0);
            mCountrerCdc_KeepAwakeReasDO = 0;
        } else {
            mCountrerCdc_KeepAwakeReasDO++;
        }
    }
    int sendCKARLUTSignalTimes = mPropertyManager->getCProValue(mpCdcKeepAwakeReasLocUpTimeout) * 10;
    ac.cdc_keep_awake_reas_loc_up = mPropertyManager->getCProValue(mpCdcKeepAwakeReasLocUp);
    if(sendCKARLUTSignalTimes) {
        if(ac.cdc_keep_awake_reas_loc_up & (sendCKARLUTSignalTimes < mCountrerCdc_KeepAwakeReasLocUp)) {
            mPropertyManager->updateCPropertyValue(mpCdcKeepAwakeReasLocUp, 0);
            mPropertyManager->updateCPropertyValue(mpCdcKeepAwakeReasLocUpTimeout, 0);
            mCountrerCdc_KeepAwakeReasLocUp = 0;
        } else {
            mCountrerCdc_KeepAwakeReasLocUp++;
        }
    }
    int sendCKASTSiganlTimes = mPropertyManager->getCProValue(mpCdcKeepAwakeStaTimeout) * 10;
    ac.cdc_keep_awake_sta = mPropertyManager->getCProValue(mpCdcKeepAwakeSta);
    if(sendCKASTSiganlTimes) {
        if(ac.cdc_keep_awake_sta & (sendCKASTSiganlTimes < mCountrerCdc_KeepAwakeSta)) {
            mPropertyManager->updateCPropertyValue(mpCdcKeepAwakeSta, 0);
            mPropertyManager->updateCPropertyValue(mpCdcKeepAwakeStaTimeout, 0);
            mCountrerCdc_KeepAwakeSta = 0;
        } else {
            mCountrerCdc_KeepAwakeSta++;
        }
    }
    uint16_t messageID;
    BusIdentifier busIdentifier;
    uint8_t command = 0x02;
    uint8_t responseFlag;
    messageID = 0x0422;
    busIdentifier = EV_CAN;
    responseFlag = 0x00;
    MessageParser mparse;
    std::vector<uint8_t> message = mparse.makePack(messageID, static_cast<void*>(&ac),
                                                   busIdentifier, command, responseFlag);
    std::string buf;
    buf = "";
    for (int i = 0; i < 17; i++) {
        std::ostringstream ss;
        ss.clear();
        int data = message[i];
        ss << std::hex << data;
        buf += "0x";
        buf += ss.str();
         buf += ", ";
    }
    ALOGI("createCanMsgNMAsrCdcBac can %zu buffer1:%s", message.size(), buf.c_str());
    uint8_t sendBuf[SPI_PACKAGE_SIZE];
    sendBuf[0] = busIdentifier;
    sendBuf[1] = (messageID >> 8) & 0xff;
    sendBuf[2] = messageID & 0xff;
    sendBuf[3] = message.size() - 9;
    memcpy(&sendBuf[4],message.data()+8,message.size()-9);
    pb_add_raw_vary_data(sendBuf,message.size()+4-9,0x01);
}


void CommConn::createCanMsgTboxBjsTimeBac() {
    mCounterTbox3B3++;
    if(0 == (mCounterTbox3B3 % 10)) {
        mCounterTbox3B3 = 0;
        struct me11_cmx_baccanfd_ots_car_v02_07_20240520_tbox_bjs_time_bac_t ac;
        // 获取当前时间
        std::time_t now = std::time(nullptr);
        std::tm* timeinfo = std::localtime(&now);
        ac.tbox_bjs_time_year = static_cast<uint8_t>(timeinfo->tm_year % 100);
        ac.tbox_bjs_time_month = static_cast<uint8_t>(timeinfo->tm_mon + 1);
        ac.tbox_bjs_time_day = static_cast<uint8_t>(timeinfo->tm_mday);
        ac.tbox_bjs_time_hour = static_cast<uint8_t>(timeinfo->tm_hour);
        ac.tbox_bjs_time_min = static_cast<uint8_t>(timeinfo->tm_min);
        ac.tbox_bjs_time_sec = static_cast<uint8_t>(timeinfo->tm_sec);
        uint16_t messageID;
        BusIdentifier busIdentifier;
        uint8_t command = 0x02;
        uint8_t responseFlag;
        messageID = 0x03B3;
        busIdentifier = EV_CAN;
        responseFlag = 0x00;
        MessageParser mparse;
        std::vector<uint8_t> message = mparse.makePack(messageID, static_cast<void*>(&ac),
                                                   busIdentifier, command, responseFlag);
        std::string buf;
        buf = "";
        for (int i = 0; i < (int)message.size(); i++) {
            std::ostringstream ss;
            ss.clear();
            int data = message[i];
            ss << std::hex << data;
            buf += "0x";
            buf += ss.str();
            buf += ", ";
        }
        ALOGI("createCanMsgTboxBjsTimeBac can %zu buffer1:%s", message.size(), buf.c_str());
        uint8_t sendBuf[SPI_PACKAGE_SIZE];
        sendBuf[0] = busIdentifier;
        sendBuf[1] = (messageID >> 8) & 0xff;
        sendBuf[2] = messageID & 0xff;
        sendBuf[3] = message.size() - 9;
        memcpy(&sendBuf[4],message.data()+8,message.size()-9);
        pb_add_raw_vary_data(sendBuf,message.size()+4-9,0x01);
    }
}


void CommConn::createCanMsgCCM311V1_1() {
    // struct body_can_ccm_4_0_ccm_311_t {
    //     uint8_t ccm_rear_fog_lamp_set;
    //     uint8_t ccm_left_mirror_set;
    //     uint8_t ccm_park_un_lock_req;
    //     uint8_t ccm_auto_lock_req;
    //     uint8_t ccm_auto_unlock_req;
    //     uint8_t ccm_remt_contr_lamp_ctr_cmd;
    //     uint8_t ccm_remote_car_search;
    //     uint8_t ccm_remote_lock;
    //     uint8_t ccm_right_mirror_set;
    // }
    struct body_can_ccm_4_0_ccm_311_t adjust;
    adjust.ccm_rear_fog_lamp_set = mPropertyManager->getCProValue(mpRearFogLampSet);
    adjust.ccm_left_mirror_set = mPropertyManager->getCProValue(mpLeftMirrorSet);
    adjust.ccm_right_mirror_set = mPropertyManager->getCProValue(mpRightMirrorSet);
    adjust.ccm_auto_unlock_req = mPropertyManager->getCProValue(mpAutoUnlockSet);
    adjust.ccm_auto_lock_req = mPropertyManager->getCProValue(mpAutoLockSet);
    adjust.ccm_park_un_lock_req = mPropertyManager->getCProValue(mpParkUnlockSet);
    adjust.ccm_remote_lock = mPropertyManager->getCProValue(mpRemoteLock);
    adjust.ccm_central_lock_switch_req = mPropertyManager->getCProValue(mpCentralLockSwitchReq);
    uint16_t messageID;
    BusIdentifier busIdentifier;
    uint8_t command = 0x02;
    uint8_t responseFlag;
    messageID = 0x311;
    busIdentifier = BODY_CAN;
    responseFlag = 0x00;
    MessageParser mparse;
    std::vector<uint8_t> message = mparse.makePack(messageID, static_cast<void*>(&adjust),
                                                   busIdentifier, command, responseFlag);
    std::string buf;
    bool unequal = ((mvCCM311.size() != message.size()) ||
                    (!std::equal(mvCCM311.begin(), mvCCM311.end(), message.begin())));
    if (unequal) {
        mvCCM311.clear();
        mvCCM311.insert(mvCCM311.end(), message.begin(), message.end());
        buf = "";
        for (int i = 0; i < 18; i++) {
            std::ostringstream ss;
            ss.clear();
            int data = message[i];
            ss << std::hex << data;
            buf += "0x";
            buf += ss.str();
            buf += ", ";
        }
        ALOGI("can311 %zu buffer1:%s", message.size(), buf.c_str());
    }
    mCanQueue.enqueue(message);
}

void CommConn::createCanMsgCCM310() {
    // name                          freq    default
    // CCM_AC_HAVCState              3       00
    // CCM_ACkey                     3       00
    // CCM_HeatKey                   3       00
    // CCM_FreshReciKey              3       00
    // CCM_FrontDefrostKey           3       00
    // CCM_BlowerLevel               NA
    // CCM_AIRDistributionModeReq    3       07
    // CCM_LeftSetTemp               NA
    struct body_can_ccm_4_0_ccm_310_t ac;
    // mpBCAvasState

    ac.ccm_ac_havc_state = mPropertyManager->getCProValue(mpAC_HAVCState);
    if (ac.ccm_ac_havc_state != 00) {
        if ((mCountrerCCM_AC_HAVCState % 2 == 0) && (mCountrerCCM_AC_HAVCState != 0)) {
            mPropertyManager->updateCPropertyValue(mpAC_HAVCState, 0);
            mCountrerCCM_AC_HAVCState = 0;
        } else {
            mCountrerCCM_AC_HAVCState++;
        }
    }
    ac.ccm_a_ckey = mPropertyManager->getCProValue(mpACkey);
    if (ac.ccm_a_ckey != 00) {
        if ((mCountrerCCM_ACkey % 2 == 0) && (mCountrerCCM_ACkey != 0)) {
            mPropertyManager->updateCPropertyValue(mpACkey, 0);
            mCountrerCCM_ACkey = 0;
        } else {
            mCountrerCCM_ACkey++;
        }
    }
    ac.ccm_heat_key = mPropertyManager->getCProValue(mpHeatKey);
    if (ac.ccm_heat_key != 00) {
        if ((mCountrerCCM_HeatKey % 2 == 0) && (mCountrerCCM_HeatKey != 0)) {
            mPropertyManager->updateCPropertyValue(mpHeatKey, 0);
            mCountrerCCM_HeatKey = 0;
            ;
        } else {
            mCountrerCCM_HeatKey++;
        }
    }
    ac.ccm_fresh_reci_key = mPropertyManager->getCProValue(mpFreshReciKey);
    if (ac.ccm_fresh_reci_key != 00) {
        if ((mCountrerCCM_FreshReciKey % 2 == 0) && (mCountrerCCM_FreshReciKey != 0)) {
            mPropertyManager->updateCPropertyValue(mpFreshReciKey, 0);
            mCountrerCCM_FreshReciKey = 0;
        } else {
            mCountrerCCM_FreshReciKey++;
        }
    }
    ac.ccm_front_defrost_key = mPropertyManager->getCProValue(mpFrontDefrostKey);
    if (ac.ccm_front_defrost_key != 00) {
        if ((mCountrerCCM_FrontDefrostKey % 2 == 0) && (mCountrerCCM_FrontDefrostKey != 0)) {
            mPropertyManager->updateCPropertyValue(mpFrontDefrostKey, 0);
            mCountrerCCM_FrontDefrostKey = 0;
        } else {
            mCountrerCCM_FrontDefrostKey++;
        }
    }
    ac.ccm_blower_level = mPropertyManager->getCProValue(mpBlowerLevel);
    int value = static_cast<int>(mPropertyManager->getCProValue(mpAIRDistributionModeReq));
    ac.ccm_air_distribution_mode_req = value;
    if ((mCountrerCCM_AIRDistributionModeReq % 2 == 0) &&
        (mCountrerCCM_AIRDistributionModeReq != 0)) {
        mPropertyManager->updateCPropertyValue(mpAIRDistributionModeReq, 7);
        mCountrerCCM_AIRDistributionModeReq = 0;
    } else {
        mCountrerCCM_AIRDistributionModeReq++;
    }
    ac.ccm_left_set_temp = mPropertyManager->getCProValue(mpLeftSetTemp);
    ac.ccm_vehicle_spd_dsp = mPropertyManager->getCProValue(mpVehicleSpdDsp);
    ac.ccm_avasset = mPropertyManager->getCProValue(mpBCAvasState);
    // ALOGI("dxy ac.ccm_avasset %d ",ac.ccm_avasset);
    uint16_t messageID;
    BusIdentifier busIdentifier;
    uint8_t command = 0x02;
    uint8_t responseFlag;
    messageID = 0x310;
    busIdentifier = BODY_CAN;
    responseFlag = 0x00;
    MessageParser mparse;
    std::vector<uint8_t> message = mparse.makePack(messageID, static_cast<void*>(&ac),
                                                   busIdentifier, command, responseFlag);
    std::string buf;
    bool unequal = ((mvCCM310.size() != message.size()) ||
                    (!std::equal(mvCCM310.begin(), mvCCM310.end(), message.begin())));
    if (unequal) {
        mvCCM310.clear();
        mvCCM310.insert(mvCCM310.end(), message.begin(), message.end());
        buf = "";
        for (int i = 0; i < 18; i++) {
            std::ostringstream ss;
            ss.clear();
            int data = message[i];
            ss << std::hex << data;
            buf += "0x";
            buf += ss.str();
            buf += ", ";
        }
        ALOGI("can310 %zu buffer1:%s", message.size(), buf.c_str());
    }
    //SendQueue::getInstance()->enqueue(message);
    SendCanMap::getInstance()->insert_data(messageID, message, busIdentifier);
}
void CommConn::createCanMsgCCM311() {
    // struct body_can_ccm_4_0_ccm_311_t {
    //     uint8_t ccm_rear_fog_lamp_set;
    //     uint8_t ccm_left_mirror_set;
    //     uint8_t ccm_park_un_lock_req;
    //     uint8_t ccm_auto_lock_req;
    //     uint8_t ccm_auto_unlock_req;
    //     uint8_t ccm_remt_contr_lamp_ctr_cmd;
    //     uint8_t ccm_remote_car_search;
    //     uint8_t ccm_remote_lock;
    //     uint8_t ccm_right_mirror_set;
    // }
    struct body_can_ccm_4_0_ccm_311_t adjust;
    adjust.ccm_rear_fog_lamp_set = mPropertyManager->getCProValue(mpRearFogLampSet);
    adjust.ccm_left_mirror_set = mPropertyManager->getCProValue(mpLeftMirrorSet);
    adjust.ccm_right_mirror_set = mPropertyManager->getCProValue(mpRightMirrorSet);
    adjust.ccm_auto_unlock_req = mPropertyManager->getCProValue(mpAutoUnlockSet);
    adjust.ccm_auto_lock_req = mPropertyManager->getCProValue(mpAutoLockSet);
    adjust.ccm_park_un_lock_req = mPropertyManager->getCProValue(mpParkUnlockSet);
    adjust.ccm_remote_lock = mPropertyManager->getCProValue(mpRemoteLock);
    adjust.ccm_central_lock_switch_req = mPropertyManager->getCProValue(mpCentralLockSwitchReq);
    uint16_t messageID;
    BusIdentifier busIdentifier;
    uint8_t command = 0x02;
    uint8_t responseFlag;
    messageID = 0x311;
    busIdentifier = BODY_CAN;
    responseFlag = 0x00;
    MessageParser mparse;
    std::vector<uint8_t> message = mparse.makePack(messageID, static_cast<void*>(&adjust),
                                                   busIdentifier, command, responseFlag);
    std::string buf;
    bool unequal = ((mvCCM311.size() != message.size()) ||
                    (!std::equal(mvCCM311.begin(), mvCCM311.end(), message.begin())));
    if (unequal) {
        mvCCM311.clear();
        mvCCM311.insert(mvCCM311.end(), message.begin(), message.end());
        buf = "";
        for (int i = 0; i < 18; i++) {
            std::ostringstream ss;
            ss.clear();
            int data = message[i];
            ss << std::hex << data;
            buf += "0x";
            buf += ss.str();
            buf += ", ";
        }
        ALOGI("can311 %zu buffer1:%s", message.size(), buf.c_str());
    }
    //SendQueue::getInstance()->enqueue(message);
    SendCanMap::getInstance()->insert_data(messageID, message, busIdentifier);
}
/*
void CommConn::createCanMsgCCM66B() {
//struct body_can_ccm_4_0_ccm_66_b_t {
//    uint8_t str_work_req;
//    uint8_t ccm_nm_bit;
//}
    struct body_can_ccm_4_0_ccm_66_b_t adjust;
    adjust.str_work_req = mPropertyManager->getCProValue(mpWorkReq);
    adjust.ccm_nm_bit = mPropertyManager->getCProValue(mpNmBit);
    uint16_t messageID;
    BusIdentifier busIdentifier;
    uint8_t command = 0x02;
    uint8_t responseFlag;
    messageID = 0x66B;
    busIdentifier = BODY_CAN;
    responseFlag = 0x00;
    MessageParser mparse;
    ALOGI("hu0506 %s %d", __func__,__LINE__);
    std::vector<uint8_t> message = mparse.makePack(messageID, static_cast<void*>(&adjust),
busIdentifier, command, responseFlag); ALOGI("hu0506 %s %d", __func__,__LINE__);
    SendQueue::getInstance()->enqueue(message);
    ALOGI("hu0506 %s %d", __func__,__LINE__);
}
*/
void CommConn::createCanMsgCCM353() {
    // struct body_can_ccm_4_0_ccm_0_x353_t {
    //     uint8_t ota_model;
    //     uint8_t ccm_bcm_open_door_warn_set;
    // }
    struct ev_can_ccm_4_0_ccm_0_x353_t adjust;
    // adjust.ota_model = mPropertyManager->getCProValue(mpOtaModel);
    // adjust.ccm_bcm_open_door_warn_set = mPropertyManager->getCProValue(mpOpenDoorWarnSet);
    adjust.ccm_drive_mode_req = mPropertyManager->getCProValue(mpDriveModeReq);
    adjust.ccm_rechrgn_lv_req = mPropertyManager->getCProValue(mpRechrgnLvReq);
    adjust.total_odometer = mPropertyManager->getCProValue(mpTotalOdometer);
    adjust.ccm_operat_licence = mPropertyManager->getCProValue(mpOperatLicence);
    adjust.ccm_charging_conditions = mPropertyManager->getCProValue(mpChargingConditions);
    adjust.ccm_bcm_open_door_warn_set = mPropertyManager->getCProValue(mpOpenDoorWarnSet);
    adjust.ccm_version = mPropertyManager->getCProValue(mpCCMVersion);
    adjust.ccm_speed_limit_level = mPropertyManager->getCProValue(mSpeedLimitLevel);
    adjust.ccm_veh_tar_soc = mPropertyManager->getCProValue(mpCCMVehTarSoc);
    adjust.ccm_target_chrg_curr = mPropertyManager->getCProValue(mpCCMTargetChrgCurr);
    uint16_t messageID;
    BusIdentifier busIdentifier;
    uint8_t command = 0x02;
    uint8_t responseFlag;
    messageID = 0x353;
    busIdentifier = static_cast<BusIdentifier>(BusIdentifier::EV_CAN);
    responseFlag = 0x00;
    MessageParser mparse;
//    ALOGI("dxy--- cm_veh_tar_soc =  %d", adjust.ccm_veh_tar_soc);
    //    ALOGI("hu0506 %s %d", __func__, __LINE__);
    std::vector<uint8_t> message = mparse.makePack(messageID, static_cast<void*>(&adjust),
                                                   busIdentifier, command, responseFlag);
    // std::string buf;
    // buf.clear();
    // for (int i = 0; i < (int)message.size(); i++) {
    //     std::ostringstream ss;
    //     ss.clear();
    //     int data = message[i];
    //     ss << std::hex << data;
    //     buf += "0x";
    //     buf += ss.str();
    //     buf += ", ";
    //     }
    // LOG(INFO) << "dxy----mpOpenDoorWarnSet =" << buf ;                                                   
    //    ALOGI("hu0506 %s %d", __func__, __LINE__);
    //SendQueue::getInstance()->enqueue(message);
    SendCanMap::getInstance()->insert_data(messageID, message, busIdentifier);
    //    ALOGI("hu0506 %s %d", __func__, __LINE__);
}


void CommConn::createCanMsgCCM354() {
    struct ev_can_ccm_4_0_ccm_0_x354_t adjust;
    adjust.ccm_creep_ctr_cmd = mPropertyManager->getCProValue(mpCreepCtrCmd);
    adjust.ccm_single_ped_ctr_cmd = mPropertyManager->getCProValue(mpSinglePedCtrCmd);
    adjust.ccm_remt_ctrl_flag = mPropertyManager->getCProValue(mpRemtCtrlFlag);
    adjust.ccm_rema_range_disp_vd = mPropertyManager->getCProValue(mpRemaRangeDispMode);
    adjust.ccm_energy_recov_level_cmd = mPropertyManager->getCProValue(mpEnergyRecovLevelCmd);
    uint16_t messageID;
    BusIdentifier busIdentifier;
    uint8_t command = 0x02;
    uint8_t responseFlag;
    messageID = 0x354;
    busIdentifier = EV_CAN;
    responseFlag = 0x00;
    MessageParser mparse;
    std::vector<uint8_t> message = mparse.makePack(messageID, static_cast<void*>(&adjust),
                                                   busIdentifier, command, responseFlag);
    //SendQueue::getInstance()->enqueue(message);
    SendCanMap::getInstance()->insert_data(messageID, message, busIdentifier);
    //    ALOGI("hu0506 %s %d", __func__, __LINE__);
}

void CommConn::getVolume()
{
    Json::Value second;
    second["Navi"] = "?";
    second["Voice"] = "?";
    second["Multi"] = "?";
    second["Tel"] = "?";
    second["Alarm"] = "?";
    second["Key"] = "?";

    Json::StreamWriterBuilder builder;
    builder["indentation"] = ""; // 设置缩进为空字符串，即不缩进
    std::string json_str = Json::writeString(builder, second);

    Json::Value final_root;
    final_root["Vol"] = json_str;

    std::string final_json_str = Json::writeString(builder, final_root);

    LOG(INFO) <<"getVolume -- str:"<<final_json_str;
    std::vector<uint8_t> vec(final_json_str.begin(), final_json_str.end());
    mPropertyManager->jsonQueueEnqueue(vec);
}

void CommConn::syncVersionInfo(std::string version, std::string value) {
     Json::Value root;
     root[version] = value;
     // 输出JSON字符串
     Json::StreamWriterBuilder builder;
     std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
     std::ostringstream oss;
     writer->write(root, &oss);
     // 获取字符串
     std::string str = oss.str();
     // 将字符串转换为 std::vector<uint8_t>
     LOG(INFO) <<"syncVersionInfo -- str:"<<str;
     std::vector<uint8_t> vec(str.begin(), str.end());
     mPropertyManager->jsonQueueEnqueue(vec);
}

/**
void CommConn::createCanMsgCCM352() {
    uint8_t tempValue = mPropertyManager->getCProValue(mSetValueFlag);
    int num = 0;
    // wjj----GECKO_CCM_VCU_VERSION makePack done
    LOG(INFO) << "wjj----GECKO_CCM_VCU_VERSION makePack done =" << tempValue ;
    while(tempValue == 1) {
    struct ev_can_ccm_4_0_gw_vcu_0_x352_t adjust;
    adjust.vcu_version = mPropertyManager->getCProValue(mVcuVersion);
    adjust.vcu_speed_limit_level = mPropertyManager->getCProValue(mSpeedLimitLevel);        
    LOG(INFO) << "wjj----GECKO_CCM_VCU_VERSION makePack done in while  =" << tempValue ;
    uint16_t messageID;
    BusIdentifier busIdentifier;
    uint8_t command = 0x02;
    uint8_t responseFlag;
    messageID = 0x352;
    busIdentifier = EV_CAN;
    responseFlag = 0x00;
    MessageParser mparse;
    std::vector<uint8_t> message = mparse.makePack(messageID, static_cast<void*>(&adjust),
                                                   busIdentifier, command, responseFlag);
    SendQueue::getInstance()->enqueue(message);

    std::string buf;
    buf.clear();
    for (int i = 0; i < (int)message.size(); i++) {
        std::ostringstream ss;
        ss.clear();
        int data = message[i];
        ss << std::hex << data;
        buf += "0x";
        buf += ss.str();
        buf += ", ";
        }
    LOG(INFO) << "dxy----gotoseting vcu version =" << buf ;

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    num++;
    if(num == 100) {
       tempValue = 2;
       mPropertyManager->updateCAPropertyValue("set_value_flag", std::to_string(2));
       }
    }
}
*/

void CommConn::createCanMsgCCM352() {
    struct ev_can_ccm_4_0_gw_vcu_0_x352_t adjust;
    adjust.vcu_version = mPropertyManager->getCProValue(mVcuVersion);
    adjust.vcu_speed_limit_level = mPropertyManager->getCProValue(mSpeedLimitLevel);
    uint8_t tempValue = mPropertyManager->getCProValue(mSetValueFlag);
    int num = 0;
    // LOG(INFO) << "wjj----GECKO_CCM_VCU_VERSION makePack done = " << tempValue ;
    while(tempValue == 1) {
    // LOG(INFO) << "wjj----GECKO_CCM_VCU_VERSION makePack done in while  =" << tempValue ;
    ccm352_is_runing = 1;
    uint16_t messageID;
    BusIdentifier busIdentifier;
    uint8_t command = 0x02;
    uint8_t responseFlag;
    messageID = 0x352;
    busIdentifier = EV_CAN;
    responseFlag = 0x00;
    MessageParser mparse;
    std::vector<uint8_t> message = mparse.makePack(messageID, static_cast<void*>(&adjust),
                                                   busIdentifier, command, responseFlag);
    // LOG(INFO) << "wjj----GECKO_CCM_VCU_VERSION going send  =" << tempValue ;
    //SendQueue::getInstance()->enqueue(message);
    SendCanMap::getInstance()->insert_data(messageID, message, busIdentifier);
        std::string buf;
    buf.clear();
    for (int i = 0; i < (int)message.size(); i++) {
        std::ostringstream ss;
        ss.clear();
        int data = message[i];
        ss << std::hex << data;
        buf += "0x";
        buf += ss.str();
        buf += ", ";
        }
    LOG(INFO) << "wjj----GECKO_CCM_VCU_VERSION buff  =" << buf ;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    num++;
    if(num == 100) {
        ccm352_is_runing = 0;
       tempValue = 2;
       mPropertyManager->updateCAPropertyValue("set_value_flag", std::to_string(2));
       }
    }
}

void CommConn::createBodyCanMsgCCM353() {
    uint16_t messageID = 0x353;
    BusIdentifier busIdentifier = BusIdentifier::BODY_CAN;
    uint8_t command = 0x02;
    uint8_t responseFlag = 0x00;
    body_can_ccm_4_0_ccm_0_x353_t adjust;
    adjust.ccm_bcm_open_door_warn_set = mPropertyManager->getCProValue(mpOpenDoorWarnSet);
    MessageParser mparse;
    std::vector<uint8_t> message =
    mparse.makePack(messageID, static_cast<void*>(&adjust), busIdentifier,
                           command, responseFlag);
    //SendQueue::getInstance()->enqueue(message);
    SendCanMap::getInstance()->insert_data(messageID, message, busIdentifier);
}

void CommConn::createBodyCanMsgCCM354() {
    struct body_can_ccm_4_0_ccm_0_x354_t adjust;
    adjust.ccm_remt_ctrl_flag = mPropertyManager->getCProValue(mpRemtCtrlFlag);
    uint16_t messageID;
    BusIdentifier busIdentifier;
    uint8_t command = 0x02;
    uint8_t responseFlag;
    messageID = 0x354;
    busIdentifier = BODY_CAN;
    responseFlag = 0x00;
    MessageParser mparse;
    std::vector<uint8_t> message = mparse.makePack(messageID, static_cast<void*>(&adjust),
                                                   busIdentifier, command, responseFlag);
    SendCanMap::getInstance()->insert_data(messageID, message, busIdentifier);
}
/*
void CommConn::createCanMsgCCM321() {
    struct ev_can_ccm_4_0_ccm_321_t adjust;
    adjust.tbox_cpu_temperature_low = mPropertyManager->getCProValue(mpCpuTemperatureLow);
    uint16_t messageID;
    BusIdentifier busIdentifier;
    uint8_t command = 0x02;
    uint8_t responseFlag;
    messageID = 0x321;
    busIdentifier = BODY_CAN;
    responseFlag = 0x00;
    MessageParser mparse;
    ALOGI("hu0506 %s %d", __func__,__LINE__);
    std::vector<uint8_t> message = mparse.makePack(messageID, static_cast<void*>(&adjust),
busIdentifier, command, responseFlag); ALOGI("hu0506 %s %d", __func__,__LINE__);
    SendQueue::getInstance()->enqueue(message);
    ALOGI("hu0506 %s %d", __func__,__LINE__);
}
void CommConn::createCanMsgCCM558() {
//struct ev_can_ccm_4_0_ccm_558_t {
//uint16_t ccm_soc_sofeware_num;
//uint16_t ccm_soc_hardware_num;
//uint16_t ccm_mcu_sofeware_num;
//uint16_t ccm_mcu_hardware_num;
//}
    struct ev_can_ccm_4_0_ccm_558_t adjust;
    adjust.ccm_soc_sofeware_num = mPropertyManager->getCProValue(mpSocSoftwareNum);
    adjust.ccm_soc_hardware_num = mPropertyManager->getCProValue(mpSocHardwareNum);
    adjust.ccm_mcu_sofeware_num = mPropertyManager->getCProValue(mpMcuSoftwareNum);
    adjust.ccm_mcu_hardware_num = mPropertyManager->getCProValue(mpMcuHardwareNum);
    uint16_t messageID;
    BusIdentifier busIdentifier;
    uint8_t command = 0x02;
    uint8_t responseFlag;
    messageID = 0x558;
    busIdentifier = BODY_CAN;
    responseFlag = 0x00;
    MessageParser mparse;
    ALOGI("hu0506 %s %d", __func__,__LINE__);
    std::vector<uint8_t> message = mparse.makePack(messageID, static_cast<void*>(&adjust),
busIdentifier, command, responseFlag); ALOGI("hu0506 %s %d", __func__,__LINE__);
    SendQueue::getInstance()->enqueue(message);
    ALOGI("hu0506 %s %d", __func__,__LINE__);
}
void CommConn::createCanMsgCCM630() {
//struct ev_can_ccm_4_0_ccm_630_t {
//uint8_t tbox_time_status;
//uint8_t tbox_year;
//uint8_t tbox_month;
//uint8_t tbox_day;
//uint8_t tbox_hour;
//uint8_t tbox_minute;
//uint8_t tbox_second;
//}
    struct ev_can_ccm_4_0_ccm_630_t adjust;
    adjust.tbox_time_status = mPropertyManager->getCProValue(mpTimeStatus);
    adjust.tbox_year = mPropertyManager->getCProValue(mpYear);
    adjust.tbox_month = mPropertyManager->getCProValue(mpMonth);
    adjust.tbox_day = mPropertyManager->getCProValue(mpDay);
    adjust.tbox_hour = mPropertyManager->getCProValue(mpHour);
    adjust.tbox_minute = mPropertyManager->getCProValue(mpMinute);
    adjust.tbox_second = mPropertyManager->getCProValue(mpSecond);
    uint16_t messageID;
    BusIdentifier busIdentifier;
    uint8_t command = 0x02;
    uint8_t responseFlag;
    messageID = 0x630;
    busIdentifier = BODY_CAN;
    responseFlag = 0x00;
    MessageParser mparse;
    ALOGI("hu0506 %s %d", __func__,__LINE__);
    std::vector<uint8_t> message = mparse.makePack(messageID, static_cast<void*>(&adjust),
busIdentifier, command, responseFlag); ALOGI("hu0506 %s %d", __func__,__LINE__);
    SendQueue::getInstance()->enqueue(message);
    ALOGI("hu0506 %s %d", __func__,__LINE__);
}

void CommConn::createCanMsgCCM66C() {
//struct ev_can_ccm_4_0_ccm_66_c_t {
//uint8_t str_work_req;
//uint8_t ccm_nm_bit;
//}
//
    ev_can_ccm_4_0_ccm_66_c_t adjust;
    adjust.str_work_req = mPropertyManager->getCProValue(mpWorkReqc);
    adjust.ccm_nm_bit = mPropertyManager->getCProValue(mpNmBitc);
    uint16_t messageID;
    BusIdentifier busIdentifier;
    uint8_t command = 0x02;
    uint8_t responseFlag;
    messageID = 0x66C;
    busIdentifier = EV_CAN;
    responseFlag = 0x00;
    MessageParser mparse;
    //ALOGI("hu0506 %s %d", __func__,__LINE__);
    std::vector<uint8_t> message = mparse.makePack(messageID, static_cast<void*>(&adjust),
            busIdentifier, command, responseFlag);
    //ALOGI("hu0506 %s %d", __func__,__LINE__);
    SendQueue::getInstance()->enqueue(message);
    //ALOGI("hu0506 %s %d", __func__,__LINE__);
}
*/
void CommConn::sendMessage(vhal_proto::EmulatorMessage const& msg) {
    int numBytes = msg.ByteSize();
    std::vector<uint8_t> buffer(static_cast<size_t>(numBytes));
    if (!msg.SerializeToArray(buffer.data(), numBytes)) {
        ALOGE("%s: SerializeToString failed!", __func__);
        return;
    }

    write(buffer);
}

void CommConn::reback_uds_message(const hidl_vec<uint8_t>& data) {
    memcpy(reback_uds_msg, &data[0], 8);
    // create_uds_canmsg(&reback_uds_msg[0]);
    uint16_t messageID;
    BusIdentifier busIdentifier;
    uint8_t responseFlag;
    uint8_t command = 0x02;

    messageID = 0x708;
    busIdentifier = UDS_CAN;
    responseFlag = 0x00;
    MessageParser mparse;
    std::vector<uint8_t> message = mparse.makePack(messageID, static_cast<void*>(reback_uds_msg),
                                                   busIdentifier, command, responseFlag);
    ALOGI("create_uds_canmsg hu0506 %s %d", __func__, __LINE__);
    //SendQueue::getInstance()->enqueue(message);
    SendCanMap::getInstance()->insert_data(messageID, message, busIdentifier);
}

// void CommConn::ProcessCanMsgThread() {
//     std::string buf;
//     unsigned char tempbuf[17];
//     std::vector<uint8_t> buffer;
//     int dequeuelength = 17;
//     LOG(INFO) << __func__ << __LINE__;
//     std::this_thread::sleep_for(std::chrono::seconds(5));
//     LOG(INFO) << __func__ << __LINE__;
//     while (true) {
//         if ((dequeuelength == 17) && (mrecvqueue.length() >= 17)) {
//             mrecvqueue.dequeue(tempbuf, 17);
//             buffer.clear();
//             buffer.insert(buffer.end(), tempbuf, tempbuf + 17);
//             buf.clear();
//             /*
//             for (int i = 0; i < 17; i++) {
//                 std::ostringstream ss;
//                 ss.clear();
//                 int data = buffer[i];
//                 ss << std::hex << data;
//                 buf += "0x";
//                 buf += ss.str();
//                 buf += ", ";
//             }
//             */
//             //if(mrecvqueue.length()==0)
//             ALOGI("dxy q size %zu buffer1:%s", mrecvqueue.length(), buf.c_str());
//             if (mMessageProcessor->checkpack(buffer) == 0) {
//                 mMessageProcessor->processCanReadMessage(buffer);
//                 dequeuelength = 17;
//             } else {
//                 dequeuelength = 1;
//             }
//         } else if ((dequeuelength == 1) && (mrecvqueue.length() >= 1)) {
//             mrecvqueue.dequeue(tempbuf, 1);
//             if(tempbuf[0] == 0x02){
//                 buffer.erase(buffer.begin());
//                 buffer.push_back(tempbuf[0]);

//                 mrecvqueue.dequeue(tempbuf, 16);
//                 buffer.insert(buffer.end(), tempbuf, tempbuf + 16);

//                 buf.clear();
//                 // for (int i = 0; i < 17; i++) {
//                 //     std::ostringstream ss;
//                 //     ss.clear();
//                 //     int data = buffer[i];
//                 //     ss << std::hex << data;
//                 //     buf += "0x";
//                 //     buf += ss.str();
//                 //     buf += ", ";
//                 // }
//                  ALOGI("dxy q size %zu buffer:%s", mrecvqueue.length(), buf.c_str());
//                 if (mMessageProcessor->checkpack(buffer) == 0) {
//                     mMessageProcessor->processCanReadMessage(buffer);
//                     dequeuelength = 17;
//                 } else {
//                     dequeuelength = 1;
//                 }
//             }else{
//                 dequeuelength = 1;
//             }
            
            
//         } else {
//             std::this_thread::sleep_for(std::chrono::milliseconds(5));
//         }
//     }
// }
bool isBufferEqual(const std::vector<uint8_t>& buffer, const std::vector<uint8_t>& targetBuffer) {
    for (size_t i = 0; i < buffer.size(); ++i) {
        if (buffer[i] != targetBuffer[i]) {
            return false;
        }
    }
    return true;
}
bool validateChecksum(const std::vector<uint8_t>& packet) {
    uint8_t checksum = 0;
    int size = packet.size();

    // 异或校验
    for (int i = 0; i < size - 1; i++) {
        checksum ^= packet[i];
    }
    

    // 检查校验和是否正确
    return (checksum == packet.back());
}

bool CommConn::isValidInt(const std::string& str) {
    if (str.empty()) return false;

    size_t start = 0;
    if (str[0] == '+' || str[0] == '-') start = 1;
    for (size_t i = start; i < str.size(); ++i) {
        if (!isdigit(str[i])) return false;
    }
    return true;
}

bool CommConn::isAlphanumeric(const std::string& str) {
    for (char ch : str) {
        if (ch == '-')continue;
        if (!isalnum(ch)) {
            return false; // 如果发现任何非字母数字的字符，则返回false
        }
    }
    return true; // 如果所有字符都是字母数字，则返回true
}

void CommConn::VinSet(std::string msg){
    ALOGI("cmd get to set VinSet %s ",msg.c_str());
    if(!isAlphanumeric(msg)){
        ALOGE("vin irregularity  VinSet %s ",msg.c_str());
        return;
    } 

    mPropertyManager->updateCPropertyValue("gecko_vin",msg);
    mVehicleConf.saveConfig("Vin", msg);
    cv_vin.notify_one();
}

void CommConn::ABStatusSet(std::string msg){
    ALOGI("cmd get to set ABStatus %s ",msg.c_str());
	uint8_t var = std::stoi(msg);
    if((var&0x80)==0x80){

        writeMCUAbStatus("1");
	    ALOGI("cmd get to set ABstatus to B");
        android::base::SetProperty("gecko.mcu.abstatus", "B"); 
	}else{
        writeMCUAbStatus("0");
	    ALOGI("cmd get to set ABStatus to A");
        android::base::SetProperty("gecko.mcu.abstatus", "A"); 
	}
    //mVehicleConf.saveConfig("ABStatus", msg);
}

void CommConn::SNSet(std::string msg){
    ALOGI("cmd get to set SNSet %s ",msg.c_str());
    if(!isAlphanumeric(msg)){
        ALOGE("SN irregularity  %s ",msg.c_str());
        return;
    } 
    android::base::SetProperty("gecko.serial.number", msg.c_str()); 
    mVehicleConf.saveConfig("SN", msg);
    mGetSNFlag = 1;
    cv_SN.notify_one();
}

void CommConn::HeadlightHSet(std::string msg){
    if(!isValidInt(msg))
    {
        ALOGE(" HeadlightHSet msg:%s is err", msg.c_str());
        return;
    }
    ALOGI("cmd get to set HeadlightHSet %s ",msg.c_str());
    int32_t headlightH = static_cast<int32_t>(std::stoi(msg.c_str()));
    mPropertyManager->updateCPropertyValueNoCheck("gecko_headlighth", headlightH);
}

void CommConn::F187Set(std::string msg){
    ALOGI("cmd get to set F187Set %s ",msg.c_str());
    if(!isAlphanumeric(msg)){
        ALOGE("F187 irregularity  %s ",msg.c_str());
        return;
    }
    android::base::SetProperty("gecko.oem.part.no", msg.c_str());
    mVehicleConf.saveConfig("F187", msg);
    mGetF187Flag = 1;
    cv_F187.notify_one();
}

void CommConn::McuVersionGet(std::string msg){
    mPropertyManager->updateCPropertyValue("gecko_mcu_version",msg);
}

void CommConn::SocVersionSet(std::string msg){
    mPropertyManager->updateCPropertyValue("gecko_soc_version",msg);
}

void CommConn::TRAVSet(std::string msg) {
    ALOGI("cmd get to set TRAVSet %s ",msg.c_str());
}

void  CommConn::RangeModeSet(std::string msg){
    if(!isValidInt(msg))
    {
        ALOGE(" RangeModeSet msg:%s is err", msg.c_str());
        return;
    }
    // RangeModeSet
    ALOGI("cmd get to set RangeModeSet %s ",msg.c_str());
    int32_t rema_range_mode = static_cast<int32_t>(std::stoi(msg.c_str()));
    mPropertyManager->updateCPropertyValueNoCheck("sc_rema_range_display_mode",rema_range_mode);
}


void CommConn::MaleageSet(std::string msg) {
    if(!isValidInt(msg))
    {
        ALOGE(" MaleageSet msg:%s is err", msg.c_str());
        return;
    }
    ALOGI("cmd get to set MaleageSet %s ",msg.c_str());
    int32_t mc_travel_range = static_cast<int32_t>(std::stoi(msg.c_str()));
    mPropertyManager->updateCPropertyValueNoCheck("mc_travel_range", mc_travel_range);
}

void CommConn::ShortMileageSet(std::string msg) {
    if(!isValidInt(msg) || msg.length() > 7)
    {
        ALOGE(" ShortMileageSet msg:%s is err", msg.c_str());
        return;
    }

    ALOGI("cmd get to set ShortMileageSet %s ",msg.c_str());
    int32_t value = std::stoi(msg.c_str());
    mPropertyManager->updateCPropertyValueNoCheck("mcu_short_mileage", value);
}

void CommConn::TRTimeSet(std::string msg) {
    ALOGI("cmd get to set TRTimeSet %s ",msg.c_str());
    int32_t mc_travel_time = static_cast<int32_t>(std::stoi(msg.c_str()));
    mPropertyManager->updateCPropertyValueNoCheck("mc_travel_time", mc_travel_time);
}

void CommConn::SpeedSet(std::string msg) {
    if(!isValidInt(msg))
    {
        ALOGE(" SpeedSet msg:%s is err", msg.c_str());
        return;
    }
    ALOGI("cmd get to set SpeedSet %s ",msg.c_str());
    int32_t speed = static_cast<int32_t>(std::stoi(msg.c_str()));
    mPropertyManager->updateCPropertyValueNoCheck("mc_travel_average_speed", speed);
}

void CommConn::AvgEngCSet(std::string msg) {
    if(!isValidInt(msg))
    {
        ALOGE(" AvgEngCSet msg:%s is err", msg.c_str());
        return;
    }
    ALOGI("cmd get to set AvgEngCSet %s ",msg.c_str());
    int32_t mc_average_energy_consumption = static_cast<int32_t>(std::stoi(msg.c_str()));
    mPropertyManager->updateCPropertyValueNoCheck("mc_average_energy_consumption", mc_average_energy_consumption);
}

void CommConn::INRSet(std::string msg) {
    if(!isValidInt(msg))
    {
        ALOGE(" INRSet msg:%s is err", msg.c_str());
        return;
    }
    mPropertyManager->updateCPropertyValueNoCheck("mc_remind_inr", std::stoi(msg.c_str()));
}

void CommConn::DISSet(std::string msg){
    if(!isValidInt(msg))
    {
        ALOGE(" DISSet msg:%s is err", msg.c_str());
        return;
    }
    ALOGI("cmd get to set DISSet %s ",msg.c_str());
    mPropertyManager->updateCPropertyValueNoCheck("mc_remind_distance", std::stoi(msg.c_str()));
}

void CommConn::StartODOSet(std::string msg){
    mGetMtceFlag = 1;
    ALOGI("cmd get to set StartODOSet %s ",msg.c_str());
    int32_t startOdo = std::stof(msg.c_str()) * 10;
    ALOGD("StartODOSet start_odo value:%d", startOdo);
    mPropertyManager->updateCPropertyValueNoCheck("mc_remind_start_odo", startOdo);
}

void CommConn::StartTimeSet(std::string msg){
    if(!isValidInt(msg) || msg.length() > 10){
        ALOGE("StartTimeSet msg is err");
        return;
    }
    mGetMtceFlag = 1; 
    mPropertyManager->updateCPropertyValueNoCheck("mc_remind_start_time", std::stoll(msg.c_str()));
}

void CommConn::CountryIdSet(std::string msg){
    if(!isValidInt(msg)){
        ALOGE("CountryIdSet msg is err");
        return;
    }
    mPropertyManager->updateCPropertyValueNoCheck("hu_nav_country_id", std::stoll(msg.c_str()));
}

void CommConn::ValueSet(std::string msg){
    if(!isValidInt(msg))
    {
        ALOGE(" ValueSet msg:%s is err", msg.c_str());
        return;
    }
    mGetBackLightFlag = 1; 
    mPropertyManager->updateCPropertyValueNoCheck("mc_light_level", std::stoi(msg.c_str()));
    cv_BackLight.notify_one();
}

void CommConn::DriveModeSet(std::string msg){
    if(!isValidInt(msg))
    {
        ALOGE(" DriveModeSet msg:%s is err", msg.c_str());
        return;
    }
    int32_t value = mPropertyManager->getCProValue(mPropertyManager->findCProperty("ec_vcu_drive_mode"));
    int32_t target = std::stoi(msg.c_str());
    ALOGD("DriveModeSet vcu_drive_mode value:%d  target:%d", value, target);
    if(value != 0) {
        mPropertyManager->updateCPropertyValueNoCheck("ec_vcu_drive_mode", target);
        mPropertyManager->updateCPropertyValueNoCheck("sc_ccm_drive_mode_req", target);
    }
}

void CommConn::SinglePedCtrSet(std::string msg){
    if(!isValidInt(msg))
    {
        ALOGE(" SinglePedCtrSet msg:%s is err", msg.c_str());
        return;
    }
    mPropertyManager->updateCPropertyValueNoCheck("ec_vcu_single_ped_sts", std::stoi(msg.c_str()));
    mPropertyManager->updateCPropertyValueNoCheck("sc_ccm_single_ped_ctr_cmd", std::stoi(msg.c_str()));
}

void CommConn::CreepCtrSet(std::string msg){
    if(!isValidInt(msg))
    {
        ALOGE(" CreepCtrSet msg:%s is err", msg.c_str());
        return;
    }
    mPropertyManager->updateCPropertyValueNoCheck("ec_vcu_creep_sts", std::stoi(msg.c_str()));
    mPropertyManager->updateCPropertyValueNoCheck("sc_ccm_creep_ctr_cmd", std::stoi(msg.c_str()));
}

void CommConn::ParkUnLockSet(std::string msg){
    if(!isValidInt(msg))
    {
        ALOGE(" ParkUnLockSet msg:%s is err", msg.c_str());
        return;
    }
    mPropertyManager->updateCPropertyValueNoCheck("bc_bcm_park_un_lock_ack", std::stoi(msg.c_str()));
    mPropertyManager->updateCPropertyValueNoCheck("sc_ccm_park_un_lock_req", std::stoi(msg.c_str()));
}

void CommConn::E2PRomSet(std::string msg){
    if(!isValidInt(msg))
    {
        ALOGE(" E2PRomSet msg:%s is err", msg.c_str());
        return;
    }
    int32_t res = static_cast<int32_t>(std::stoi(msg.c_str()));
    mPropertyManager->updateCPropertyValueNoCheck("gecko_e2p_rom_result", res);
}

void CommConn::KeyCodeSet(std::string msg){
    if(!isValidInt(msg))
    {
        ALOGE(" KeyCodeSet msg:%s is err", msg.c_str());
        return;
    }
    int32_t res = static_cast<int32_t>(std::stoi(msg.c_str()));
    mPropertyManager->updateCPropertyValueNoCheck("gecko_key_code_result", res);
}

void CommConn::LBATVolSet(std::string msg){
    android::base::SetProperty("persist.low.battery.volt", msg.c_str());
}

void CommConn::CHGCONDSet(std::string msg){
    if(!isValidInt(msg))
    {
        ALOGE(" CHGCONDSet msg:%s is err", msg.c_str());
        return;
    }
    android::base::SetProperty("persist.tbox.chargingcondition", msg.c_str());
    int32_t res = static_cast<int32_t>(std::stoi(msg.c_str()));
    mPropertyManager->updateCPropertyValueNoCheck("sc_ccm_charging_conditions", res);
}

void CommConn::SLLevelSet(std::string msg){
    if(!isValidInt(msg))
    {
        ALOGE(" CLLevelSet msg:%s is err", msg.c_str());
        return;
    }
    android::base::SetProperty("persist.tbox.speedlimitlevel", msg.c_str());
    int32_t res = static_cast<int32_t>(std::stoi(msg.c_str()));
    mPropertyManager->updateCPropertyValueNoCheck("sc_ec_ccm_speed_limit_level", res);
}


void CommConn::SLVersionSet(std::string msg){
    if(!isValidInt(msg))
    {
        ALOGE(" CLVersionSet msg:%s is err", msg.c_str());
        return;
    }
    android::base::SetProperty("persist.tbox.versionreq", msg.c_str());
    int32_t res = static_cast<int32_t>(std::stoi(msg.c_str()));
    mPropertyManager->updateCPropertyValueNoCheck("sc_ccm_version", res);
}

void CommConn::SLOLSet(std::string msg){
    if(!isValidInt(msg))
    {
        ALOGE(" CLOLSet msg:%s is err", msg.c_str());
        return;
    }
    android::base::SetProperty("persist.tbox.operatlicense", msg.c_str());
    int32_t res = static_cast<int32_t>(std::stoi(msg.c_str()));
    mPropertyManager->updateCPropertyValueNoCheck("sc_ccm_operat_licence", res);
}

static std::string mRealdata;
static int16_t mFMTotalCounter=0; 
void CommConn::FMSet(std::string msg){
    ALOGI(" FMSet msg:%s", msg.c_str());
    std::string type = msg.substr(0, 2);
    std::string data = msg.substr(2);
    if(type == "02"){
        mPropertyManager->updateCPropertyValueNoCheck("fm_search", data);
    } else if(type == "04"){
        int16_t index = std::stoi(data.substr(0, 2));
        int16_t count = std::stoi(data.substr(2).substr(0, 2));
        std::string temp = "0";
        ALOGE("FMSet=== index:%d count:%d", index, count);
        mFMTotalCounter = mFMTotalCounter + count;
        ALOGE("FMSet==== mFMTotalCounter:%d", mFMTotalCounter);
        if(count != 0) {
            mRealdata.append(data.substr(4));
        }
        if(index == 0) {
             
            if(mFMTotalCounter < 10) {
                data = temp.append(std::to_string(mFMTotalCounter)).append(mRealdata);
	    } else {
                data = std::to_string(mFMTotalCounter).append(mRealdata);
            }
            ALOGE("FMSet=== data:%s", data.c_str());
            mPropertyManager->updateCPropertyValueNoCheck("bc_fm_auto_seek_res", data);
            mFMTotalCounter = 0;
            mRealdata.clear();
            data.clear();
        }
    }
}

void CommConn::setTboxLinkTsp()
{
    std::string linkst = android::base::GetProperty("persist.tbox.linkst", "0");
    if(linkst == "2"){
        std::string key = "TBOXLINKTSP";
        std::string value = linkst;
        mPropertyManager->singleJsonEnQueue(key, value);
        ALOGI("setTboxLinkTsp linkst:%s", linkst.c_str());
    }
}
// 自定义trim函数
std::string trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\n\r");
    size_t end = str.find_last_not_of(" \t\n\r");

    if (start == std::string::npos || end == std::string::npos) {
        return "";
    }
 
    return str.substr(start, end - start + 1);
}

void CommConn::HardWareVersionSet(std::string msg)
{
    mGetHardWareVersionFlag = 1;
    std::string value = trim(msg.c_str());
    ALOGI("cmd get to set HardWareVersionSet %s ", value.c_str());
    android::base::SetProperty("gecko.mcu.hardware.version", value.c_str());
    android::base::SetProperty("gecko.soc.hardware.version", value.c_str());
    //mPropertyManager->updateCPropertyValue("sc_ec_ccm_mcu_hardware_num", std::stoi(msg.c_str()));
}

void CommConn::SchCHGONSet(std::string  msg)
{
    if(msg.length() == 0 || msg.length() > 3){
        ALOGE("SchCHGONSet msg:%s is err", msg.c_str());
        return;
    }
    int32_t value = static_cast<int32_t>(std::stoi(msg.c_str()));
    mPropertyManager->updateCPropertyValue("ec_vcu_chraging_mode_sp", value);
    ALOGI("SchCHGONSet %d", value);
    SchCHGQueryFlag = true;
}

void CommConn::SchCHGCIRCSet(std::string  msg)
{
    if(msg.length() == 0 || msg.length() > 3){
        ALOGE("SchCHGCIRCSet msg:%s is err", msg.c_str());
        return;
    }
    int32_t value = static_cast<int32_t>(std::stoi(msg.c_str()));
    mPropertyManager->updateCPropertyValue("appointment_mode", value);
    ALOGI("SchCHGCIRCSet %d", value);
}

void CommConn::SchCHGWEEKSet(std::string  msg)
{
    if(msg.length() == 0 || msg.length() > 3){
        ALOGE("SchCHGWEEKSet msg:%s is err", msg.c_str());
        return;
    }
    int num = std::stoi(msg.c_str());

    int mask = 1 << 0;
    int32_t d0 = (num & mask) >> 0; 
    mPropertyManager->updateCPropertyValue("sunday_loop", d0);

    mask = 1 << 1;
    int32_t d1 = (num & mask) >> 1;
    mPropertyManager->updateCPropertyValue("monday_loop", d1);

    mask = 1 << 2;
    int32_t d2 = (num & mask) >> 2;
    mPropertyManager->updateCPropertyValue("tuesday_loop", d2);

    mask = 1 << 3;
    int32_t d3 = (num & mask) >> 3;
    mPropertyManager->updateCPropertyValue("wednesday_loop", d3);

    mask = 1 << 4;
    int32_t d4 = (num & mask) >> 4;
    mPropertyManager->updateCPropertyValue("thursday_loop", d4);

    mask = 1 << 5;
    int32_t d5 = (num & mask) >> 5;
    mPropertyManager->updateCPropertyValue("friday_loop", d5);

    mask = 1 << 6;
    int32_t d6 = (num & mask) >> 6;
    mPropertyManager->updateCPropertyValue("saturday_loop", d6);
    ALOGI("SchCHGWEEKSet %s d0:%d d1:%d d2:%d d3:%d d4:%d d5:%d d6:%d ",msg.c_str(),d0,d1,d2,d3,d4,d5,d6);
        
}

void CommConn::SchCHGStartTimeSet(std::string  msg)
{
    if(msg.length() == 0 || msg.length() > 7){
        ALOGE("SchCHGStartTimeSet msg:%s is err", msg.c_str());
        return;
    }
    int n = msg.find(":");
    std::string hour = msg.substr(0, n);
    mPropertyManager->updateCPropertyValue("chraging_begin_time_hour", static_cast<int32_t>(std::stoi(hour.c_str())));
    std::string minute = msg.substr(n + 1);
    mPropertyManager->updateCPropertyValue("chraging_begin_time_minute", static_cast<int32_t>(std::stoi(minute.c_str())));
    ALOGI("SchCHGStartTimeSet %s:%s ",hour.c_str(), minute.c_str());
}

void CommConn::SchCHGEndTimeSet(std::string  msg)
{
    if(msg.length() == 0 || msg.length() > 7){
        ALOGE("SchCHGEndTimeSet msg:%s is err", msg.c_str());
        return;
    }
    int n = msg.find(":");
    std::string hour = msg.substr(0, n);
    mPropertyManager->updateCPropertyValue("chraging_end_time_hour", static_cast<int32_t>(std::stoi(hour.c_str())));
    std::string minute = msg.substr(n + 1);
    mPropertyManager->updateCPropertyValue("chraging_end_time_minute", static_cast<int32_t>(std::stoi(minute.c_str())));
    ALOGI("SchCHGEndTimeSet %s:%s ",hour.c_str(), minute.c_str());
}

void CommConn::SchCHGTGTSocSet(std::string  msg)
{
    if(msg.length() == 0 || msg.length() > 4){
        ALOGE("SchCHGEndTimeSet msg:%s is err", msg.c_str());
        return;
    }
    int32_t value = static_cast<int32_t>(std::stoi(msg.c_str()));
    mPropertyManager->updateCPropertyValue("chraging_target_soc", value);
    ALOGI("SchCHGTGTSocSet %d", value);
}


void CommConn::CPUTempSet(std::string msg)
{
    if(msg == "?")
    {
        const char* filePath = "/sys/class/thermal/thermal_zone0/temp";
        std::ifstream tempFile(filePath);
        std::string temperature;
        std::getline(tempFile, temperature);
        tempFile.close();
        std::string key {"CPUTemp"};
        int32_t cputemp = std::stoi(temperature);
        if(cputemp > 115000) {
            mPropertyManager->singleJsonEnQueue(key, temperature);
        }
    }  
}
//#define VENDOR_VEHICLE_FEATURE_ID             16
//#define VENDOR_VEHICLE_VIN_ID                 17
//#define VENDOR_ECU_SN_ID                      18
//#define VENDOR_VEHICLE_TSP_ID                 19
//#define VENDOR_VEHICLE_TYPE_ID                20
//#define VENDOR_VEHICLE_OEM_DEV_ID             21
int CommConn::vehicle_vendor_storage_write(uint16_t id, uint16_t len, const std::vector<uint8_t>& write_value)
{
    int ret = 0;
    uint32_t i = 0;
    uint8_t p_buf[1124];
    struct gecko_vendor_req *req;

    if (write_value.empty()) {
        ALOGE("ITEM process_cmd value is empty");
        return -1;
    }

    req = (struct gecko_vendor_req *)p_buf;
    int sys_fd = open("/dev/vendor_storage", O_RDWR, 0);
    if (sys_fd < 0) {
        ALOGE("vendor_storage open fail\n");
        return -1;  // 打开设备失败
    }

    req->tag = VENDOR_REQ_TAG;
    req->id = id;
    req->len = len;

    if (write_value.size() < len) {
        ALOGE("Error: write_value length (%zu) is less than expected (%u)\n", write_value.size(), len);
        close(sys_fd);
        return -3;  // 数据长度不足
    }
    memcpy(req->data, write_value.data(), len);
    std::string hex_string;
    for (i=0;i<len;i++) {
        char buf[4];
        snprintf(buf, sizeof(buf), "%02X ", req->data[i]);
        hex_string += buf;
    }
    ALOGE("vehicle_vendor_storage_write: %s", hex_string.c_str());
    ret = ioctl(sys_fd, VENDOR_WRITE_IO, req);
    if (ret) {
        ALOGE("write vendor storage error %d \n", ret);
        close(sys_fd);
        return -2;
    }

    ALOGE("write vendor storage success\n");
    close(sys_fd);
    return 0;
}

int CommConn::vehicle_vendor_storage_read(uint16_t id, uint16_t len, std::vector<uint8_t>& out_data)
{
    int ret = 0;
    uint32_t i = 0;
    uint8_t p_buf[1124];
    struct gecko_vendor_req *req;

    req = (struct gecko_vendor_req *)p_buf;
    int sys_fd = open("/dev/vendor_storage", O_RDWR, 0);
    if (sys_fd < 0) {
        ALOGE("vendor_storage open fail\n");
        return -1;
    }

    req->tag = VENDOR_REQ_TAG;
    req->id = id;
    req->len = 512;

    memset(req->data, 0xAA, len);
    ret = ioctl(sys_fd, VENDOR_READ_IO, req);
    if (ret == 0) {
        out_data.assign(req->data, req->data + len);
        if (!out_data.empty()) {
            std::string hex_string;
            for (i=0;i<len;i++) {
                char buf[4];
                snprintf(buf, sizeof(buf), "%02X ", out_data[i]);
                hex_string += buf;
            }
            ALOGE("vehicle_vendor_storage_read: %s", hex_string.c_str());
        }
        ALOGE("read vendor storage success\n");
        close(sys_fd);
        return 0;
    }
    else {
        ALOGE("read vendor storage error ret %d\n", ret);
        close(sys_fd);
        return -2;  // IO 操作失败
    }
}

int CommConn::ItemGetByVehicleService(uint16_t id, std::vector<uint8_t>& out_data, uint16_t len)
{
    int ret = 0;
    std::vector<uint8_t> write_value;
    //ALOGE("vehicle_storage ItemGetByVehicleService len %d, id %d",len, id);
    //ret = vehicle_vendor_storage_read_or_write(VENDOR_STD_READ, write_value, id, len, out_data);
    ret = vehicle_vendor_storage_read(id, len, out_data);
    if (0 != ret) {
        ALOGE("vendor storage read err, ret %d", ret);
    }
    return ret;
}
int CommConn::ItemSetByFactoryMode(const std::vector<uint8_t>& value, uint16_t id, uint16_t len)
{
    int ret = 0;
    ret = vehicle_vendor_storage_write(id, len, value);
    if (0 != ret) {
        ALOGE("vendor storage write err, ret %d", ret);
    }
    return ret;
}

/*
ECU_SN:
vendor_id = 0xF18C;
VIN_ID:
vendor_id = 0xF190;
VECH_CONFIG:
vendor_id = 0xF016;
OEM_DEV_NUM:
vendor_id = 0xF187;
FEATURE_CONF:
vendor_id = 0xF101;
TSP_CONFIG:
vendor_id = 0xF011;
*/
void CommConn::ItemSetByMcu(std::string msg)
{
    int ret = 0;
    std::vector<uint8_t> out_data;
    std::string id_buf;
    std::string len_buf;
    std::string value_str;
    std::vector<uint8_t> value;
    std::vector<uint8_t> out_value;
    uint16_t hex_len = 0;
    uint16_t string_len = 0;
    uint16_t config_id = 0;
    uint16_t storage_id = 0;
    uint8_t length_err = 0;
    uint8_t config_id_err = 0;
    std::string gecko_vin_str;
    std::string gecko_oem_no_str;
    std::string result;
    if (msg.size() < 8) {
        ALOGE("ITEM Message too short!");
        return;
    }
    ALOGI("cmd get to set ItemSetByMcu %s ",msg.c_str());

    id_buf = msg.substr(0, 4);
    len_buf = msg.substr(4, 4);
    config_id = std::stoul(id_buf, nullptr, 16);
    hex_len = std::stoul(len_buf, nullptr, 16); //字节数

    string_len = 2*hex_len;
    value_str = msg.substr(8, string_len);

    // 解析16进制字符串，转换为 uint8_t 数组
    value.clear();
    for (size_t i = 0; i < value_str.length(); i += 2) {
        std::string byte_str = value_str.substr(i, 2);  // 取两个字符
        uint8_t byte = static_cast<uint8_t>(std::stoul(byte_str, nullptr, 16));  // 转换为 uint8_t
        value.push_back(byte);
    }

    ALOGE("vehicle_storage ItemSetByMcu %s ",msg.c_str());
    switch (config_id)
    {
        case 0xf101:
            storage_id = VENDOR_VEHICLE_FEATURE_ID;
            if (hex_len != VEHICLE_FEATURE_MSG_LEN) {
                ALOGE("hex_len %d err, need %d",hex_len, VEHICLE_FEATURE_MSG_LEN);
                length_err = 1;
            }
            break;
        case 0xf011:
            storage_id = VENDOR_VEHICLE_TSP_ID;
            if (hex_len != VEHICLE_TSP_MSG_LEN) {
                ALOGE("hex_len %d err, need %d",hex_len, VEHICLE_TSP_MSG_LEN);
                length_err = 1;
            }
            break;
        case 0xf016:
            storage_id = VENDOR_VEHICLE_TYPE_ID;
            if (hex_len != VEHICLE_TYPE_MSG_LEN) {
                ALOGE("hex_len %d err, need %d",hex_len, VEHICLE_TYPE_MSG_LEN);
                length_err = 1;
            }
            break;
        case 0xf187:
            storage_id = VENDOR_VEHICLE_OEM_DEV_ID;
            if (hex_len != VEHICLE_OEM_MSG_LEN) {
                ALOGE("hex_len %d err, need %d",hex_len, VEHICLE_OEM_MSG_LEN);
                length_err = 1;
            }
            break;
        case 0xf190:
            storage_id = VENDOR_VEHICLE_VIN_ID;
            if (hex_len != VEHICLE_VIN_MSG_LEN) {
                ALOGE("hex_len %d err, need %d",hex_len, VEHICLE_VIN_MSG_LEN);
                length_err = 1;
            }
            break;
        case 0xf18C:
            storage_id = VENDOR_ECU_SN_ID;
            if (hex_len != VEHICLE_ECU_MSG_LEN) {
                ALOGE("hex_len %d err, need %d",hex_len, VEHICLE_ECU_MSG_LEN);
                length_err = 1;
            }
            break;
        case 0xff00:
            storage_id = VENDOR_SET_FLAG_ID;
            if (hex_len != VENDOR_SET_FLAG_LEN) {
                ALOGE("hex_len %d err, need %d",hex_len, VENDOR_SET_FLAG_LEN);
                length_err = 1;
            }
            break;
        case 0xff01:
            storage_id = VENDOR_MCU_VERSION_ID;
            if (hex_len != MCU_VERSION_MSG_LEN) {
                ALOGE("hex_len %d err, need %d",hex_len, MCU_VERSION_MSG_LEN);
                length_err = 1;
            }
            break;
        case 0xff02:
            storage_id = VENDOR_MCU_AB_PT_ID;
            if (hex_len != MCU_AB_PT_MSG_LEN) {
                ALOGE("hex_len %d err, need %d",hex_len, MCU_AB_PT_MSG_LEN);
                length_err = 1;
            }
            break;
        default:
            ALOGE("Unknown config_id: 0x%x", config_id);
            config_id_err = 1;
            break;
    }
    if (length_err) {
        ALOGE("hex_len err do nothing...");
        return;
    }
    if (config_id_err) {
        ALOGE("config_id err do nothing...");
        return;
    }
    // 打印 write value   的每个字节，按十六进制格式输出
    ALOGE("vehicle_storage ItemSetByMcu value: hexlen %d", hex_len);
    for (size_t i = 0; i < value.size(); ++i) {
        ALOGE("%02X ", value[i]);
    }
    //ret = vehicle_vendor_storage_read_or_write(VENDOR_STD_WR, value, vendor_storage_id, hex_len, out_data);
    ret = vehicle_vendor_storage_write(storage_id, hex_len, value);
    if (0 == ret) {
        switch (storage_id) {
            case VENDOR_VEHICLE_VIN_ID:
                //更新属性有问题
                //gecko_vin_str = std::string(value.begin(), value.end());
                //mPropertyManager->updateCPropertyValue("gecko_vin", gecko_vin_str);
                //cv_vin.notify_one();
                //break;
            case VENDOR_VEHICLE_OEM_DEV_ID:
                //gecko_oem_no_str = std::string(value.begin(), value.end());
                //mPropertyManager->updateCPropertyValue("gecko.oem.part.no", gecko_oem_no_str);
                //break;
            case VENDOR_VEHICLE_TYPE_ID:
            case VENDOR_VEHICLE_TSP_ID:
            case VENDOR_VEHICLE_FEATURE_ID:
            case VENDOR_ECU_SN_ID:
            case VENDOR_SET_FLAG_ID:
                ALOGE("Storage write success for storage_id: %u", storage_id);
                break;
            case VENDOR_MCU_AB_PT_ID:
               if((value[0]&0x80)==0x80){
                    ALOGI("cmd get to set ABstatus to B");
                    android::base::SetProperty("gecko.mcu.abstatus", "B");
                }else{
                    ALOGI("cmd get to set ABStatus to A");
                    android::base::SetProperty("gecko.mcu.abstatus", "A");
                }
                break;
            case VENDOR_MCU_VERSION_ID:
                for (size_t i = 0; i < value.size(); ++i) {
                    result += static_cast<char> (value[i]);
                }
                android::base::SetProperty("gecko.mcu.version", result.c_str());
                ALOGE("Storage write success for storage_id: %u", storage_id);
                break;
            default:
                ALOGE("Unexpected storage_id: %u", storage_id);
                break;
        }
        json_config_id = config_id;
        vendor_store_id = storage_id;
    } else {
        json_config_id = 0;
        vendor_store_id = 0;
        ALOGE("ItemToSoc vendor storage write err, ret %d", ret);
    }
    //ret = vehicle_vendor_storage_read(vendor_storage_id, hex_len, out_value);
}


static int32_t mSendCPUTempCounter = 0;
static int32_t mNoramlCPUTempCounter = 0;
static int32_t mSendSIMCardCounter = 0;
static int32_t mSendWifiInfoCounter = 0;
static int32_t mSendCameraStsCounter = 0;
void CommConn::sendCPUTempToMcu()
{
    const char* filePath = "/sys/class/thermal/thermal_zone0/temp";
    std::ifstream tempFile(filePath);
    std::string temperature;
    std::getline(tempFile, temperature);
    tempFile.close();
    std::string key {"CPUTemp"};
    int32_t cputemp = std::stoi(android::base::GetProperty("persist.sys.cputemp.test", temperature));
    temperature = android::base::GetProperty("persist.sys.cputemp.test", temperature);
    if(cputemp > 115000) {
        mNoramlCPUTempCounter = 0;
        mSendCPUTempCounter++;
        if(mSendCPUTempCounter ==20) {
            ALOGI("sendCPUTempToMcu cputemp=%d", cputemp);
            mPropertyManager->singleJsonEnQueue(key, temperature);
            mSendCPUTempCounter = 0;
        }
    } else {
        if(mNoramlCPUTempCounter < 10) {
            mNoramlCPUTempCounter++;
        }
        mSendCPUTempCounter = 0;
        if(mNoramlCPUTempCounter ==10) {
            ALOGI("sendCPUTempToMcu revert to normal cputemp=%d", cputemp);
            mPropertyManager->singleJsonEnQueue(key, temperature);
            mNoramlCPUTempCounter = 11;
        }
    }
}


void CommConn::CPULoadSet(std::string msg)
{
    if(msg == "?")
    {
        std::string key {"CPULoad"};
        std::string value = std::to_string(mCpuUsage);
        mPropertyManager->singleJsonEnQueue(key, value);
    }  
}

void CommConn::SIMCardStsSet(std::string msg)
{
    if(msg == "?")
    {
        mPropertyManager->updateCPropertyValueNoCheck("config_query_sim_card", 1);
    } 
}

void CommConn::sendSIMCardStsToMcu()
{
    mSendSIMCardCounter++;
    if (mSendSIMCardCounter==100) {
        mPropertyManager->updateCPropertyValueNoCheck("config_query_sim_card", 1);
        mSendSIMCardCounter = 0;
    }
}

void CommConn::TBOXLINKTSPSet(std::string msg)
{
    if(msg == "?")
    {
        ConfigDBC* mp = mPropertyManager->findCProperty("gecko_tbox_link_tsp");
        int32_t value = mPropertyManager->getCProValue(mp);

        std::string key = "TBOXLINKTSP";
        std::string strValue = std::to_string(value);
        mPropertyManager->singleJsonEnQueue(key, strValue);
        //mPropertyManager->updateCPropertyValueNoCheck("config_query_tbox_tsp", 1);
    }
}

void CommConn::Strength4GSet(std::string msg)
{
    if(msg == "?")
    {
        mPropertyManager->updateCPropertyValueNoCheck("config_query_4g_strength", 1);
    }  
}

void CommConn::CCMModeSet(std::string  msg)
{
    if(msg.length() == 0 || msg.length() > 3){
        ALOGE("CCMModeSet msg:%s is err", msg.c_str());
        return;
    }
    mPropertyManager->updateCPropertyValue("ccm_mode", std::stoi(msg.c_str()));
}

void CommConn::WIFIStrength(std::string msg)
{
    if(msg == "?")
    {
        mPropertyManager->updateCPropertyValueNoCheck("config_query_wifi_strength", 1);
    }  
}

void CommConn::WIFIStatus(std::string msg)
{
    if(msg == "?")
    {
        mPropertyManager->updateCPropertyValueNoCheck("config_query_wifi_status", 1);
    }  
}

void CommConn::sendWifiInfo() {
    mSendWifiInfoCounter++;
    if(mSendWifiInfoCounter==10) {
        mPropertyManager->updateCPropertyValueNoCheck("config_query_wifi_strength", 1);
        mPropertyManager->updateCPropertyValueNoCheck("config_query_wifi_status", 1);
        mSendWifiInfoCounter=0;
    }
}

void CommConn::GPSLon(std::string msg)
{
    if(msg == "?")
    {
        mPropertyManager->updateCPropertyValueNoCheck("config_query_gps_lon", 1);
    }  
}

void CommConn::GPSlat(std::string msg)
{
    if(msg == "?")
    {
        mPropertyManager->updateCPropertyValueNoCheck("config_query_gps_lat", 1);
    }  
}


void CommConn::RechargeSet(std::string msg){
    ALOGI("cmd get to set RechargeSet %s ",msg.c_str());
}

void  CommConn::powerStrategyModeSet(std::string msg){
    ALOGI("cmd get to set gecko_ccm_soc_power_status %s",msg.c_str());

    std::string key {"PowerStatus"};
    mPropertyManager->singleJsonEnQueue(key, msg);
    ALOGW("dxy-- cmd power status repail PowerStatus = %s  value = %s ", key.c_str(),msg.c_str());
    if(mCountBCM_PowerStatus > 0){
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    std::vector<uint8_t> vecMsg;
    if(msg=="5"){
        std::vector<uint8_t> vecMsg{5};
        mPropertyManager->PowerStausEnqueue(vecMsg);
    }else if(msg=="3"){
        std::vector<uint8_t> vecMsg{3};
        mPropertyManager->PowerStausEnqueue(vecMsg);
    }else if(msg=="4"){
        std::vector<uint8_t> vecMsg{4};
        mPropertyManager->PowerStausEnqueue(vecMsg);
    }
    //mPropertyManager->updateCPropertyValue("gecko_ccm_soc_power_status",msg);
}

void CommConn::volumeNaviSet(std::string  msg){
    mPropertyManager->updateCPropertyValue("gecko_mcu_voiume_navigation", msg);
}

void CommConn::volumeVoiceSet(std::string  msg){
    mPropertyManager->updateCPropertyValue("gecko_mcu_voiume_voic", msg);
}

void CommConn::volumeMultiSet(std::string  msg){
    mPropertyManager->updateCPropertyValue("gecko_mcu_voiume_multimedia", msg);
}

void CommConn::volumeTelSet(std::string  msg){
    mPropertyManager->updateCPropertyValue("gecko_mcu_voiume_phone", msg);
}

void CommConn::volumeAlarmSet(std::string  msg){
    mPropertyManager->updateCPropertyValue("gecko_mcu_voiume_alarm", msg);
}

void CommConn::volumeKeySet(std::string  msg){
    mPropertyManager->updateCPropertyValue("gecko_mcu_voiume_key", msg);
}

void CommConn::process_cmd(std::string key, std::string value){
    if(value.empty()) {
        LOG(INFO)<<"cmd process_cmd value is empty key:" << key;
        return;
    }
    int i = 0;
    do{
        //LOG(INFO)<<"process_cmd key:" << key<< "i:"<<i;
        if(strcmp(key.c_str(), spiCmd[i].msg) == 0){
            if(spiCmd[i].set!=NULL){
                spiCmd[i].set(value);
            }
        }
        i++;
    }while(i<SPI_CMD_SIZE);
}

void CommConn::parseDate(const Json::Value& root) {
    std::string l = root.toStyledString();
    LOG(INFO)<<"cmd get "  << "l: " << l << std::endl;
    if (!root.isObject()) {
        LOG(INFO)<<"cmd get " << "Expected an object at the root level." << std::endl;
        return;
    }

    for (const auto& member : root.getMemberNames()) {
        LOG(INFO)<<"cmd get "  << "Key: " << member << std::endl;
        const Json::Value& value = root[member];
  
        if(member == "SchCHG")
        {
            if (value.isObject()) {
                LOG(INFO)<<"SchCHG cmd get "  << "This key contains an object:" << std::endl;
                for (const auto& subMember : value.getMemberNames()) {
                    LOG(INFO)<<"SchCHG cmd get "  << "  Sub-Key: " << subMember << ", Value: " << value[subMember].asString() << std::endl;
                    process_cmd_SchCHG(subMember, value[subMember].asString());
                }
            } 
        }


        if(member == "WIFI")
        {
            if (value.isObject()) {
                LOG(INFO)<<"VariantConfig cmd get "  << "This key contains an object:" << std::endl;
                for (const auto& subMember : value.getMemberNames()) {
                    LOG(INFO)<<"WIFI cmd get "  << "  Sub-Key: " << subMember << ", Value: " << value[subMember].asString() << std::endl;
                    process_cmd_WIFI(subMember, value[subMember].asString());
                }
            }
        }

        if(member == "GPS")
        {
            if (value.isObject()) {
                LOG(INFO)<<"VariantConfig cmd get "  << "This key contains an object:" << std::endl;
                for (const auto& subMember : value.getMemberNames()) {
                    LOG(INFO)<<"GPS cmd get "  << "  Sub-Key: " << subMember << ", Value: " << value[subMember].asString() << std::endl;
                    process_cmd_GPS(subMember, value[subMember].asString());
                }
            }
        }


        if (value.isObject()) {
            LOG(INFO)<<"cmd get "  << "This key contains an object:" << std::endl;
            for (const auto& subMember : value.getMemberNames()) {
                LOG(INFO)<<"cmd get "  << "  Sub-Key: " << subMember << ", Value: " << value[subMember].asString() << std::endl;
                process_cmd(subMember, value[subMember].asString());
            }
        } else {
            if (member == "PowerStatus") {
                LOG(INFO)<<"zhugang cmd get "  << "Value: " << value.asString() << std::endl;
            }
            process_cmd(member, value.asString());
            if (member == "PowerStatus") {
                mCountBCM_PowerStatus++;
                LOG(INFO)<<"cmd get count power_status = " << mCountBCM_PowerStatus << std::endl;
            }
        }
    }
}

void CommConn::process_cmd(Json::Value::Members &members,Json::Value &root){
    int i = 0;
    std::string cmd;
    std::string msg = members[0];
    std::string key = root[members[0]].asString();
    do{

        if(memcmp(spiCmd[i].msg,msg.c_str(),strlen(spiCmd[i].msg))==0){
            if(spiCmd[i].set!=NULL){
                spiCmd[i].set(key);
            }
        }
        i++;
    }while(i<SPI_CMD_SIZE);
}

void CommConn::process_cmd_SchCHG(std::string key, std::string value){
    if(value.empty()) {
        LOG(INFO)<<"cmd process_cmd value is empty key:" << key;
        return;
    }
    int i = 0;
    do{
        if(strcmp(key.c_str(), spiCmdSchCHG[i].msg) == 0){
            if(spiCmdSchCHG[i].set!=NULL){
                spiCmdSchCHG[i].set(value);
            }
        }
        i++;
    }while(i<6);
}

void CommConn::process_cmd_WIFI(std::string key, std::string value)
{
    if(value.empty()) {
        LOG(INFO)<<"cmd process_cmd value is empty key:" << key;
        return;
    }
    int i = 0;
    do{
        if(strcmp(key.c_str(), spiCmdWIFI[i].msg) == 0){
            if(spiCmdWIFI[i].set!=NULL){
                spiCmdWIFI[i].set(value);
            }
        }
        i++;
    }while(i<2);
}

void CommConn::process_cmd_GPS(std::string key, std::string value)
{
    if(value.empty()) {
        LOG(INFO)<<"cmd process_cmd value is empty key:" << key;
        return;
    }
    int i = 0;
    do{
        if(strcmp(key.c_str(), spiCmdGPS[i].msg) == 0){
            if(spiCmdGPS[i].set!=NULL){
                spiCmdGPS[i].set(value);
            }
        }
        i++;
    }while(i<2);
}

void CommConn::ProcessCanMsgThread_V1_1() {
    std::string buf;
    std::vector<uint8_t> buffer;
    std::vector<uint8_t> spirecv;
    Json::CharReaderBuilder builder;
    std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
    Json::Value root;
    std::string errors;
    bool parsingSuccessful;
    while (true) {
        spirecv = mSpiUpQueue.dequeue();
        if(spirecv.size()!=17*60){
            ALOGI("recv size err");
            continue;
        }
        int pack_start = 0;
        uint16_t pack_lenght = 0;
        uint16_t can_cmd_id = 0;
        uint8_t cmd_id = 0;
        uint32_t spiCounterReconstructed;
        while(1){
            if(spirecv[pack_start]==0x02){
                cmd_id = spirecv[pack_start+1];
                pack_lenght = 
                    static_cast<uint16_t>(spirecv[pack_start+3]) |
                    (static_cast<uint16_t>(spirecv[pack_start+4]) << 8);
                can_cmd_id = 
                    static_cast<uint16_t>(spirecv[pack_start+6]) |
                    (static_cast<uint16_t>(spirecv[pack_start+7]) << 8);
                std::vector<uint8_t> packet(spirecv.begin() + pack_start, spirecv.begin() + pack_start+pack_lenght+6);
//                printVec(packet);
                //检测校验值
                if(validateChecksum(packet)){
                    if(cmd_id==0x09){
                        SpiSetAck();
                        std::vector<uint8_t> json_str(packet.begin() + 5, packet.begin() + 5 +pack_lenght);
                        const char* json_cstr = reinterpret_cast<const char*>(json_str.data());
                        size_t json_size = json_str.size();
                        parsingSuccessful = reader->parse(json_cstr, json_cstr + json_size, &root, &errors);
                       // parsingSuccessful = reader->parse(json_str.begin(), json_str.begin() + json_str.size(), &root, &errors);
                        if (!parsingSuccessful) {
                            std::cout << "Error parsing json: " << errors << std::endl;
                        }
                        Json::Value::Members members = root.getMemberNames();
                        std::string key;
                        if (!members.empty()) {
                            key = root[members[0]].asString();
                            ALOGI("cmd get %s %s",members[0].c_str(),key.c_str());
                            parseDate(root);
                        }else{
                            ALOGI("json err");
                        }
                    }else{
                        switch(can_cmd_id){
                            case 0xfffe:
                                buf.clear();
                                for (int i = 0; i < 17; i++) {
                                    std::ostringstream ss;
                                    ss.clear();
                                    int data = packet[i];
                                    ss << std::hex << data;
                                    buf += "0x";
                                    buf += ss.str();
                                    buf += ", ";
                                }
                                spiCounterReconstructed = 
                                    static_cast<uint32_t>(packet[11]) |
                                    (static_cast<uint32_t>(packet[10]) << 8) |
                                    (static_cast<uint32_t>(packet[9]) << 16) |
                                    (static_cast<uint32_t>(packet[8]) << 24);
                                SpiSetAck();
                            break;
                            default:
                                if (mMessageProcessor->checkpack(packet) == 0) {
                                    mMessageProcessor->processCanReadMessage(packet);
                                }
                        }
                    }
                }
                pack_start = pack_start+pack_lenght+6;
            }else{
                break;
            }
        }
    }
}
void CommConn::ProcessCanMsgThread() {
    std::string buf;
    unsigned char tempbuf[17];
    std::vector<uint8_t> buffer;
    int dequeuelength = 17;
    LOG(INFO) << __func__ << __LINE__;
    std::this_thread::sleep_for(std::chrono::seconds(5));
    LOG(INFO) << __func__ << __LINE__;
    while (true) {
        if ((dequeuelength == 17) && (mrecvqueue.length() >= 17)) {
            mrecvqueue.dequeue(tempbuf, 17);
            buffer.clear();
            buffer.insert(buffer.end(), tempbuf, tempbuf + 17);
            buf.clear();
            /*
            for (int i = 0; i < 17; i++) {
                std::ostringstream ss;
                ss.clear();
                int data = buffer[i];
                ss << std::hex << data;
                buf += "0x";
                buf += ss.str();
                buf += ", ";
            }
            */
            //if(mrecvqueue.length()==0)
            // ALOGI("dxy q size %zu buffer1:%s", mrecvqueue.length(), buf.c_str());
            if (mMessageProcessor->checkpack(buffer) == 0) {
                mMessageProcessor->processCanReadMessage(buffer);
                dequeuelength = 17;
            } else {
                //dequeuelength = 1;
            }
        } else if ((dequeuelength == 1) && (mrecvqueue.length() >= 1)) {
            mrecvqueue.dequeue(tempbuf, 1);
            buffer.erase(buffer.begin());
            buffer.push_back(tempbuf[0]);
            buf.clear();
            for (int i = 0; i < 17; i++) {
                std::ostringstream ss;
                ss.clear();
                int data = buffer[i];
                ss << std::hex << data;
                buf += "0x";
                buf += ss.str();
                buf += ", ";
            }
            // ALOGI("dxy q size %zu buffer-1:%s", mrecvqueue.length(), buf.c_str());
            if (mMessageProcessor->checkpack(buffer) == 0) {
                mMessageProcessor->processCanReadMessage(buffer);
                dequeuelength = 17;
            } else {
                //dequeuelength = 1;
            }
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    }
}

void CommConn::HandlerCanDataThread()
{
    HandlerCanData::getInstance()->HandlerData();
}
void CommConn::HandlerjsonspiThread() {
    std::vector<uint8_t> msg;
    uint8_t command = 0x09;
    uint8_t responseFlag = 0x00;
    MessageParser mparse;
    int repeate = 0;
    while(1){
        msg = mPropertyManager->jsonQueuedequeue();
        std::vector<uint8_t> message = mparse.makePack(msg,command,responseFlag);
        repeate = 0;
        do{
            mCanQueue.enqueue(message);
            if(GetSpiStartAck()==0){
                break;
            }
            repeate++;
        }while(repeate<3);
        if(repeate>=3){
        }
    }
}
void CommConn::HandlerjsonspiThread_V1_2() {
    std::vector<uint8_t> msg;
    while(1){
        if(MCUUpdate){
            LOG(INFO)<<"mcu update no other data is sent 0";
            std::this_thread::sleep_for(std::chrono::milliseconds(10000));
            continue;
        }
        msg = mPropertyManager->jsonQueuedequeue();
        if(msg.size()<120){
#if USE_VARY_DATA
            pb_add_raw_vary_data(msg.data(),msg.size(),0x09);
#else
			pb_add_raw_data(msg.data(),msg.size(),0x09);
#endif
        }else{
	        ALOGI("zhugang json too long %s\r\n",msg.data());	
        }
    }
}
void CommConn::HandlerspiDataThread()
{
    int i = 0;
    std::vector<uint8_t> msg;
    std::vector<uint8_t> msg_it;
    std::vector<uint8_t> temp_msg = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int addr = 0;
    while(1){
        i = mCanQueue.getsize();
        msg.clear();
        msg_it.clear();
        addr = 0;
        if(i==0){
            //全填充0，并生成下行数据
            msg.resize(17*60);
            for(int i = 0;i<17*60;i++)
                msg[i] = 0;
        }else{
            while(1){
                //填充报文
                if(mCanQueue.getsize()==0){
                    //将
                    msg.resize(17*60);
                    for(int i = addr;i<17*60;i++)
                        msg[i] = 0;
                    break;
                }
                if(addr+mCanQueue.getfrontsize()>17*60){
                    break;
                }
                msg_it = mCanQueue.dequeue();
                msg.insert(msg.end(), msg_it.begin(), msg_it.end());
                addr = addr + msg_it.size();
            }
        }
        msg.insert(msg.begin(), static_cast<uint8_t>(spicounter & 0xFF));
        msg.insert(msg.begin(), static_cast<uint8_t>((spicounter >> 8) & 0xFF));
        msg.insert(msg.begin(), static_cast<uint8_t>((spicounter >> 16) & 0xFF));
        msg.insert(msg.begin(), static_cast<uint8_t>((spicounter >> 24) & 0xFF));
        spicounter++;
        mSpiDownQueue.enqueue(msg);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void CommConn::readThread() {
    std::vector<uint8_t> buffer;
    while (true) {
        if(mManager.getType() == SPI){
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            continue;
        }
        buffer.clear();
        mManager.receiveData(buffer);
        int buf_size = buffer.size();
        if (buf_size <= 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            continue;
        }
        mrecvqueue.enqueue(buffer.data(), buffer.size());
    }
}

void CommConn::writeThread() {
    std::vector<uint8_t> msg;
    //    std::string buf;
    while (true) {
        if(mManager.getType() == SPI){
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            continue;
        }
        msg = SendQueue::getInstance()->dequeue();
        // buf = "";
        // for(int i = 0; i < 18; i++){
        //     std::ostringstream ss;
        //     ss.clear();
        //     int data = msg[i];
        //     ss << std::hex << data;
        //     buf += "0x";
        //     buf += ss.str();
        //     buf += ", ";
        // }
        // ALOGI("hu0506 q w size %zu buffer1:%s", msg.size(), buf.c_str());
        mManager.SendMessage(msg);
    }
}

void CommConn::resetSomeProp() {
//    mPropertyManager->updateCPropertyValue(mpLeftMirrorSet, 0);
//    mPropertyManager->updateCPropertyValue(mpRightMirrorSet, 0);
    mPropertyManager->updateCPropertyValue(mpCentralLockSwitchReq, 0);     
}
void CommConn::transferThread_V1_1() {
    std::vector<uint8_t> buffer{};
    std::vector<uint8_t> msg;
    while (true) {
        //报文队列有内容则开始传输
        buffer.clear();
//        msg = mSpiDownQueue.dequeueTimeouta();
        if(msg.size()>0){
        }else{
            unsigned char tempbuf[17*60+4];
            memset(tempbuf,0,sizeof(tempbuf));
            msg.insert(msg.end(), tempbuf, tempbuf + 17*60+4);
        }
        //将报文合并成需要传输的报文
        mManager.transferData(buffer,msg);
        buffer.erase(buffer.begin(), buffer.begin() + 4);
        mSpiUpQueue.enqueue(buffer);
    }
}

void printVec1(uint8_t *data1,int len){
    std::string buf;
            for (int i = 0; i < len; i++) {
                std::ostringstream ss;
                ss.clear();
                int data = data1[i];
                ss << std::hex<< std::setw(2)<<std::setfill('0') << data;
                //buf += "0x";
                buf += ss.str();
                buf += " ";
            }
   // ALOGI("spi1028 body %s", buf.c_str());
}

void printVec2(uint8_t *data1,int len){
    std::string buf;
            for (int i = 0; i < len; i++) {
                std::ostringstream ss;
                ss.clear();
                int data = data1[i];
                ss << std::hex << data;
                buf += "0x";
                buf += ss.str();
                buf += ", ";
            }
   // ALOGI("zwt spi buf: %s", buf.c_str());
}

void CommConn::sendOtaCtlCmd(uint8_t ctlCmd)
{
	ALOGI("zwt spi sendOtaCtlCmd ctlCmd = %d\r\n",ctlCmd);

	if(ctlCmd == 1)
	{
		//stop json transfer
		mPropertyManager->stopJsonQueueEnqueue();
	}else if(ctlCmd == 0){
		//resume json transfer
		mPropertyManager->resumeJsonQueueEnqueue();
	}else{
		ALOGI("zwt spi sendOtaCtlCmd unknown ctlCmd\r\n");
	}

	return;
}

void CommConn::sendOtaData(uint32_t addr,const uint8_t *data,uint32_t len)
{
	ALOGI("zwt spi sendOtaData addr = 0x%x,data[0] = 0x%x,data[1] = 0x%x,data[2] = 0x%x,len = %d\r\n", addr,data[0],data[1],data[2],len);
	
	uint8_t otaCanData[SPI_PACKAGE_SIZE];  //max can data size = SPI_PACKAGE_SIZE - 13
	uint16_t readMsgIdx = 0;
	uint8_t msgNum = 0;
	uint16_t consLen = (6+7);
	uint16_t dataLen = 0;
	uint16_t remainLen = 0;
	if(len > SPI_PACKAGE_SIZE - consLen)
	{
		while(readMsgIdx < len)
		{
			memset(otaCanData,0,sizeof(otaCanData));
			if(msgNum == 0)
			{
				//has len and addr
				otaCanData[0] = 0x00;
				otaCanData[1] = addr & 0x000000ff;
				otaCanData[2] = (addr >> 8) & 0x000000ff;
				otaCanData[3] = (addr >> 16) & 0x000000ff;
				otaCanData[4] = (addr >> 24) & 0x000000ff;
				dataLen = len;
				otaCanData[5] = dataLen & 0x000000ff;
				otaCanData[6] = (dataLen >> 8) & 0x000000ff;
				memcpy(&otaCanData[7],data+readMsgIdx,SPI_PACKAGE_SIZE-consLen);
				
				/*for(uint32_t i = 0; i < SPI_PACKAGE_SIZE-consLen+7; i++)
				{
					ALOGI("zwt spi msgNum = %d,otaCanData i = %d,0x%x",msgNum,i,otaCanData[i]);
				}*/
				
#if USE_VARY_DATA
				pb_add_raw_vary_data(otaCanData,SPI_PACKAGE_SIZE-consLen+7,0x0a);
#endif
				readMsgIdx += (SPI_PACKAGE_SIZE-consLen);
				msgNum++;
				
			}else{
				//no addr and len 
				otaCanData[0] = msgNum;
				remainLen = len - readMsgIdx;
				if(remainLen >= SPI_PACKAGE_SIZE - 6 -1)
				{
					dataLen = SPI_PACKAGE_SIZE - 6 -1;
					memcpy(&otaCanData[1],data+readMsgIdx,dataLen);		
					/*for(uint32_t i = 0; i < dataLen+1; i++)
					{
						ALOGI("zwt spi msgNum = %d,otaCanData i = %d,0x%x",msgNum,i,otaCanData[i]);
					}*/
#if USE_VARY_DATA					
					pb_add_raw_vary_data(otaCanData,dataLen+1,0x0a);
#endif
					readMsgIdx += dataLen;
					msgNum++;

				}else{
					dataLen = remainLen;
					memcpy(&otaCanData[1],data+readMsgIdx,dataLen);		
					/*for(uint32_t i = 0; i < dataLen+1; i++)
					{
						ALOGI("zwt spi msgNum = %d,otaCanData i = %d,0x%x",msgNum,i,otaCanData[i]);
					}*/
#if USE_VARY_DATA						
					pb_add_raw_vary_data(otaCanData,dataLen+1,0x0a);
#endif
					readMsgIdx += dataLen;
					msgNum++;

				}
			}

		}
		//ALOGI("zwt spi readMsgIdx = %d\r\n",readMsgIdx);
	}else{
		otaCanData[0] = 0;
		otaCanData[1] = addr & 0x000000ff;
		otaCanData[2] = (addr >> 8) & 0x000000ff;
		otaCanData[3] = (addr >> 16) & 0x000000ff;
		otaCanData[4] = (addr >> 24) & 0x000000ff;
		otaCanData[5] = len & 0x000000ff;
		otaCanData[6] = (len >> 8) & 0x000000ff;
	
		memcpy(&otaCanData[7],data,len);

		/*for(uint32_t i = 0; i < len+7; i++)
		{
			ALOGI("zwt spi otaCanData i = %d,0x%x",i,otaCanData[i]);
		}*/
#if USE_VARY_DATA
		pb_add_raw_vary_data(otaCanData,len+7,0x0a);
#endif

	}

	ALOGI("zwt spi sendOtaData,msgNum = %d\r\n",msgNum);
	return;
}

void CommConn::setOtaCallBack(const sp<IVehicleOtaCallback>& callback)
{
	ALOGI("zwt spi setOtaCallBack");
	otaCallback = callback;
	return;
}

uint8_t CommConn::getCKS(uint8_t *buf, int size)
{
	int i;
	unsigned char cks;
	cks = *buf++;
	for(i = 1; i < size; i++)
	{
		cks ^= *buf++;
	}
	return cks;
}

#if USE_VARY_DATA
void CommConn::makeEmptyVaryMsg(uint8_t *data,uint8_t response)
{
	data[0] = 0x02;
	data[1] = 0x00;
	data[2] = response;
	data[3] = 0x0c; //low byte
	data[4] = 0x00; //high byte
	for(int j = 0;j<12;j++)
	{
		data[j+5] = 0x00;
	}
	data[17] = getCKS(data,17);

	return;
}

void CommConn::getQueueVaryMsg(/*uint8_t *data,*/uint8_t response)
{
	uint8_t nextMsgLen = 0;
	uint8_t nextMsgId = 0;
	uint8_t sendBufOffset = 0;
	uint8_t emptyMsgFlag = 0;
	uint8_t queueData[SPI_PACKAGE_SIZE];
	uint8_t queueDatalen;

	if(first2msgflag) {
		first2msgflag--;
		if(first2msgflag) {
			psendBuf = sendBuf[1];
		} else {
			psendBuf = sendBuf[0];
		}
	} else {
		if(psendBuf == sendBuf[0]) {
			if(!errorspisendcounter()) {
				psendBuf = sendBuf[1];
			} else {
				psendBuf = sendBuf[1];
				sendBufOffset += *(psendBuf + 3) + 5;
				goto getQueueMsg0;
			}
		} else {
			if(!errorspisendcounter()) {
				psendBuf = sendBuf[0];
			} else {
				psendBuf = sendBuf[0];
				sendBufOffset += *(psendBuf + 3) + 5;
				goto getQueueMsg0;
			}
		}
	}
	
	memset(psendBuf,0,SPI_PACKAGE_SIZE);
	sendBufOffset = 5;  //前5个字节有固定值
	while(1)
	{	
		nextMsgLen = get_next_vary_msg_len();
		if(nextMsgLen == 0)
		{
			if(emptyMsgFlag == 0)
			{
				//no data in psendBuf
				//make empty msg
				makeEmptyVaryMsg(psendBuf,response);
				lastQueueMsgId = 0;
				return;
			}else{
				//has data in psendBuf
				//make msg end
				break;
			}
		}else{
			nextMsgId = get_next_vary_msg_id();
			if((sendBufOffset + nextMsgLen) > (SPI_PACKAGE_SIZE -1) || nextMsgId == 0x09)
			{	//next msgid is 01 0a or 09,and 01 or 0a msg too long ,can not insert to sendBuf
			
				//send json msg
				if(nextMsgId == 0x09)
				{
					if(emptyMsgFlag == 0)
					{
						//no data in psendBuf
						memset(queueData,0,SPI_PACKAGE_SIZE);
				 		queueDatalen = pb_get_vary_buff(queueData);
				 		memcpy(&psendBuf[sendBufOffset],queueData,queueDatalen);
				 		sendBufOffset += queueDatalen;
						lastQueueMsgId = nextMsgId;
					}
				}else{
					//01 msg or 04 msg,all make msg end
				}

				//make msg end 
				break;
			}else{
				//next msgid is 01 or 0a and msg can insert to sendBuf
				if(nextMsgId == lastQueueMsgId || lastQueueMsgId == 0)
				{
					//next msg 
					//insert data to sendBuf
					memset(queueData,0,SPI_PACKAGE_SIZE);
					queueDatalen = pb_get_vary_buff(queueData);
					memcpy(&psendBuf[sendBufOffset],queueData,queueDatalen);
					sendBufOffset += queueDatalen;
					lastQueueMsgId = nextMsgId;
				}else{
					//make msg end 
					break;
				}
				
			}
		}
		emptyMsgFlag = 1;
	}
	
	psendBuf[0] = 0x02;
	
	if(lastQueueMsgId == 0x01) {  //psendBuf msg id 01
		psendBuf[1] = 0x01;
	}else if(lastQueueMsgId == 0x09){ //psendBuf msg id 09
		psendBuf[1] = 0x09;
	}else if(lastQueueMsgId == 0x0a){ //psendBuf msg id 0a
		psendBuf[1] = 0x0a;
	}else{
		ALOGI("zwt spi getQueueVaryMsg id error,msg id = 0x%x\r\n",lastQueueMsgId);
	}
	
getQueueMsg0:
	psendBuf[2] = response;
	psendBuf[3] = (sendBufOffset-5) & 0xff; //low byte
	psendBuf[4] = (sendBufOffset-5) >> 8; //high byte
	psendBuf[sendBufOffset] = getCKS(psendBuf,sendBufOffset);
	lastQueueMsgId = 0;    //psendBuf msg all send
	
	return;
}

uint8_t CommConn::processVaryMsg(uint8_t *recvBuf)
{
	//process
	//ALOGI("zwt spi process msg\r\n");
	uint16_t msgLen = 0;
	uint16_t readMsgIdx = 0;
	uint8_t msgNum = 0;
	uint8_t msgBody[SPI_PACKAGE_SIZE];
	// 保存接收到的数据到文件
	//std::ofstream outfile("/data/vendor/tbox/spi_recv_data.txt", std::ios::app);
	//if (outfile.is_open()) {
	//	for (size_t i = 0; i < SPI_PACKAGE_SIZE; ++i) {
	//		outfile << std::hex << std::setw(2) << std::setfill('0') 
	//				<< static_cast<unsigned int>(recvBuf[i]);
	//		// 每16个字节换行，便于查看
	//		if ((i + 1) % 16 == 0) {
	//			outfile << std::endl;
	//		} else {
	//			outfile << " ";  // 字节间用空格分隔
	//		}
	//	}
	//	outfile << std::endl;  // 每次保存后换行
	//	outfile.close();
	//}
	memset(msgBody,0,sizeof(msgBody));
	memset(mCanVaryData,0,sizeof(mCanVaryData));

	//ALOGI("zwt spi process vary msg,recvBuf[1] = 0x%x\r\n",recvBuf[1]);
    if(recvBuf[1]==0x01){
		//ALOGI("zwt spi process vary msg 0x01\r\n");
	    msgLen = (recvBuf[4] << 8) | recvBuf[3];
		//ALOGE("zwt spi process msg 0x01,msgLen = %d\r\n",msgLen);
	    if(msgLen > SPI_PACKAGE_SIZE-6)
	    {
	    	ALOGE("zwt spi process vary msg 0x01,msgLen error,msgLen = %d\r\n",msgLen);
	    }else{
			memcpy(msgBody,&recvBuf[5],msgLen);
			while(readMsgIdx < msgLen)
			{
				mCanVaryData[msgNum].busId = msgBody[readMsgIdx];
				readMsgIdx++;
				
				mCanVaryData[msgNum].canId = msgBody[readMsgIdx] << 8 | msgBody[readMsgIdx+1];
				readMsgIdx += 2;
				
				mCanVaryData[msgNum].canBufLen = msgBody[readMsgIdx];
				readMsgIdx++;

				if(mCanVaryData[msgNum].canBufLen > 0 && mCanVaryData[msgNum].canBufLen <= 128)
				{
					memcpy(mCanVaryData[msgNum].canBuf,&msgBody[readMsgIdx],mCanVaryData[msgNum].canBufLen);
				}else{
					ALOGE("zwt spi process vary msg 0x01,mCanVaryData[%d].canBufLen error,canBufLen = %d\r\n",msgNum,mCanVaryData[msgNum].canBufLen);
				}
				readMsgIdx += mCanVaryData[msgNum].canBufLen;

                printVec1(mCanVaryData[msgNum].canBuf, mCanVaryData[msgNum].canBufLen);

				std::vector<uint8_t> canvec(mCanVaryData[msgNum].canBuf, &mCanVaryData[msgNum].canBuf[mCanVaryData[msgNum].canBufLen]);
                //printVec(canvec);

	    	    // ALOGE("zhugang %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\r\n",msgBody[i*11+0],msgBody[i*11+1],msgBody[i*11+2],msgBody[i*11+3],
                //                  msgBody[i*11+4],msgBody[i*11+5],msgBody[i*11+6],msgBody[i*11+7],
                //                  msgBody[i*11+8],msgBody[i*11+9],msgBody[i*11+10]);
                

                BusIdentifier busIdentifier;
                busIdentifier = static_cast<BusIdentifier>(mCanVaryData[msgNum].busId);
                //CanMap::getInstance()->insert_data(mCanVaryData[msgNum].canId,canvec, busIdentifier);
                if(mCanVaryData[msgNum].canId == 0x125){
                    printVec(canvec);
                }
                mMessageProcessor->processCanReadMessage1(mCanVaryData[msgNum].canId,canvec, busIdentifier);			
				msgNum++;
			}    	
	    	//ALOGI("zwt spi process msg ok,msgNum = %d\r\n",msgNum);
			
	    }
    }else if(recvBuf[1]==0x09){
    		ALOGI("zwt spi process vary msg 0x09\r\n");
            Json::CharReaderBuilder builder;
            std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
            Json::Value root;
            std::string errors;
            bool parsingSuccessful;

	    	//ALOGI("hqh spi process \r\n");

            /*uint8_t strRecvBuf[SPI_PACKAGE_SIZE*5+1];
            memset(strRecvBuf,0,sizeof(strRecvBuf));
            for(uint8_t j = 0; j<SPI_PACKAGE_SIZE;j++)
            {
                sprintf((char*)&strRecvBuf[j*5],"0x%02x,",recvBuf[j]);
            }
            ALOGI("hqh json recv data:%s\r\n",strRecvBuf);*/

            memcpy(msgBody, recvBuf, SPI_PACKAGE_SIZE);
            SpiSetAck();
            std::vector<uint8_t> json_str(&msgBody[5], &msgBody[SPI_PACKAGE_SIZE]);
            const char* json_cstr = reinterpret_cast<const char*>(json_str.data());
            size_t json_size = json_str.size();
            parsingSuccessful = reader->parse(json_cstr, json_cstr + json_size, &root, &errors);
            // parsingSuccessful = reader->parse(json_str.begin(), json_str.begin() + json_str.size(), &root, &errors);
            if (!parsingSuccessful) {
               LOG(ERROR)<<"cmd get Error parsing json: " << errors << std::endl;
                //LOG(ERROR)<<"cmd get Error parsing json contents: " << printVec()json_cstr << std::endl;
                std::string l = root.toStyledString();
                LOG(INFO)<<"cmd get Error"  << "root: " << l << std::endl;
                //printVec(json_str);
            }else{
                LOG(INFO)<<"zhugang json str "  << json_cstr << std::endl;
                parseDate(root);
            }
    }else if(recvBuf[1]==0x0a){
    	ALOGI("zwt spi process vary msg 0x0a\r\n");
		msgLen = (recvBuf[4] << 8) | recvBuf[3];
		ALOGI("zwt spi process msg 0x0a,msgLen = %d\r\n",msgLen);
	    if(msgLen > SPI_PACKAGE_SIZE-6)
	    {
	    	ALOGE("zwt spi process vary msg 0x0a,msgLen error,msgLen = %d\r\n",msgLen);
	    }else{
			uint8_t frameNo;
			uint16_t frameLen;
			frameLen = (recvBuf[4] << 8) | recvBuf[3];
			frameNo = recvBuf[5];
			ALOGI("zwt spi frameNo = %d,frameLen = %d\r\n",frameNo,frameLen);
			if(!frameNo) {
				txCanId = (recvBuf[9] << 24) | (recvBuf[8] << 16) | (recvBuf[7] << 8) | (recvBuf[6]);
				txMsgLen = (recvBuf[11] <<8) | recvBuf[10];
				txMsgIndxLen = 0;
				if(txMsgLen <= SPI_PACKAGE_SIZE - 13) {
					//one msg in single frame
					if(txMsgLen == 0)
					{
						//recv empty msg,put 8 byte cc
						ALOGI("zwt spi one msg in single frame,recv empty msg,put 8 byte cc\r\n");
						txMsgLen = 8;
						char filePac[8] = {0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,0xcc};
						memcpy(txMsgBuff,filePac,txMsgLen);
					}else{
						//recv normal msg
						memcpy(txMsgBuff, recvBuf+12, txMsgLen);
					}
					ALOGI("zwt spi one msg in single frame,txMsgLen = %d,txCanId = 0x%x\r\n",txMsgLen,txCanId);
					
					//call back event
					if(otaCallback == nullptr)
					{
						ALOGE("zwt spi otaCallback is nullptr 00 \r\n");
					}else{
						ALOGI("zwt spi process msg 0x0a,00 txCanId = 0x%x\r\n",txCanId);
						uint32_t respAddr = txCanId;
						std::vector<uint8_t> respData;
						respData.insert(respData.begin(), txMsgBuff, txMsgBuff+txMsgLen);
						uint32_t respLen = txMsgLen;
				        int maxRetries = 3;
						int retryCount = 0;
						while (retryCount < maxRetries) {
							ALOGI("zwt spi process msg 0x0a,00 respAddr = 0x%x,respData[0] = 0x%x,respLen = %d\r\n",respAddr,respData[0],respLen);
						    auto status = otaCallback->otaEvent(StatusCode::OK,respAddr,respData,respLen);
						    if (status.isOk()) {
						        //success
						        break;
						    }else {
						        //error
						        ALOGE("zwt spi 0 otaCallback->otaEvent failed: %s,retry\r\n", status.description().c_str());
						        retryCount++;
						        usleep(2*1000); // 延时2毫秒
						    }
						}

						if (retryCount == maxRetries) {
						    ALOGE("zwt spi 0 otaCallback->otaEvent error,maxRetries = %d\r\n",maxRetries);
						}
					}
				} else {
					//msg in much frame
					memcpy(txMsgBuff, recvBuf+12, txMsgLen);
					txMsgIndxLen = txMsgLen;
					nextFrameNo = 1;
					ALOGI("zwt spi msg in much frame,txMsgLen = %d\r\n",txMsgLen);
				}
			} else if(frameNo == nextFrameNo) {
				if((frameLen-1) > txMsgLen - txMsgIndxLen) {
					//more msg end
					memcpy(&txMsgBuff[txMsgIndxLen], recvBuf+6, txMsgLen - txMsgIndxLen);
					txMsgIndxLen = txMsgLen;

					ALOGI("zwt spi more msg end,frameNo = %d,nextFrameNo = %d\r\n",frameNo,nextFrameNo);
					//more ota can msg.
					uint16_t txMsgParseIdx = 0;
					uint16_t txMsgParseNum = 0;
					canUdsData mCanUdsData[128];
					while(txMsgParseIdx < msgLen)
					{
						uint32_t respAddr;
						std::vector<uint8_t> respData;
						uint32_t respLen;
						
						respAddr = (txMsgBuff[txMsgParseIdx+3] << 24) | (txMsgBuff[txMsgParseIdx+2] << 16) | (txMsgBuff[txMsgParseIdx+1] << 8) | (txMsgBuff[txMsgParseIdx]);
						txMsgParseIdx += 4;
						ALOGI("zwt spi process msg 0x0a,11 txCanId = 0x%x\r\n",respAddr);
						
						respLen = (txMsgBuff[txMsgParseIdx+1] <<8) | txMsgBuff[txMsgParseIdx];
						txMsgParseIdx += 2;
						
						respData.insert(respData.begin(), &txMsgBuff[txMsgParseIdx], txMsgBuff+respLen);
						txMsgParseIdx += mCanUdsData[txMsgParseNum].canBufLen;
			
						txMsgParseNum++;

						//call back event
						if(otaCallback == nullptr)
						{
							ALOGE("zwt spi otaCallback is nullptr 11 \r\n");
						}else{
							int maxRetries = 3;
							int retryCount = 0;
							while (retryCount < maxRetries) {
								ALOGI("zwt spi process msg 0x0a,11 respAddr = 0x%x,respData[0] = 0x%x,respLen = %d\r\n",respAddr,respData[0],respLen);
							    auto status = otaCallback->otaEvent(StatusCode::OK,respAddr,respData,respLen);
							    if (status.isOk()) {
							        //success
							        break;
							    }else{
							        //error
							        ALOGE("zwt spi 1 otaCallback->otaEvent failed: %s,retry\r\n", status.description().c_str());
							        retryCount++;
							        usleep(2*1000); // 延时2毫秒
							    }
							}

							if (retryCount == maxRetries) {
							    ALOGE("zwt spi 1 otaCallback->otaEvent error,maxRetries = %d\r\n",maxRetries);
							}
						}
					}
					
				} else if((frameLen-1) == txMsgLen - txMsgIndxLen) {
					//one msg end
					memcpy(&txMsgBuff[txMsgIndxLen], recvBuf+6, txMsgLen - txMsgIndxLen);
					txMsgIndxLen = txMsgLen;
					
					ALOGI("zwt spi one msg end,frameLen = %d,txMsgLen = %d,txMsgIndxLen = %d\r\n",frameLen,txMsgLen,txMsgIndxLen);
					//call back event
					if(otaCallback == nullptr)
					{
						ALOGE("zwt spi otaCallback is nullptr 22\r\n");
					}else{
						ALOGI("zwt spi process msg 0x0a,22 txCanId = 0x%x\r\n",txCanId);
						uint32_t respAddr = txCanId;
						std::vector<uint8_t> respData;
						respData.insert(respData.begin(), txMsgBuff, txMsgBuff+txMsgLen);
						uint32_t respLen = txMsgLen;
						int maxRetries = 3;
						int retryCount = 0;
						while (retryCount < maxRetries) {
							ALOGI("zwt spi process msg 0x0a,22 respAddr = 0x%x,respData[0] = 0x%x,respLen = %d\r\n",respAddr,respData[0],respLen);
						    auto status = otaCallback->otaEvent(StatusCode::OK,respAddr,respData,respLen);
						    if (status.isOk()) {
						        //success
						        break;
						    } else {
						        //error
						        ALOGE("zwt 2 spi otaCallback->otaEvent failed: %s,retry\r\n", status.description().c_str());
						        retryCount++;
						        usleep(2*1000); // 延时2毫秒
						    }
						}

						if (retryCount == maxRetries) {
						    ALOGE("zwt spi 2 otaCallback->otaEvent error,maxRetries = %d\r\n",maxRetries);
						}
					}
				} else {
					//one msg in much frame
					memcpy(&txMsgBuff[txMsgIndxLen], recvBuf+6, frameLen - 1);
					txMsgIndxLen += (frameLen - 1);
					nextFrameNo++;
					ALOGI("zwt spi one msg in much frame\r\n");
				}
			} else {
				ALOGI("zwt spi processVaryMsg error,frameNo = %d,nextFrameNo = %d\r\n",frameNo,nextFrameNo);
			}
	    }

	}else{
		ALOGI("zwt spi processVaryMsg error,recvBuf[1] = 0x%x\r\n",recvBuf[1]);
	}
	
	//return flag
	if(recvBuf[2] == 0)
		return 0;
	else if(recvBuf[2] == 1)
		return 1;
	else if(recvBuf[2] == 2)
		return 2;
	else
		return 2;//error reply
	return 0;
	
}

void CommConn::pb_add_raw_vary_data(uint8_t *data,uint8_t len,uint8_t msgid)
{

    //printVec1(data,len);
	pthread_mutex_lock(&mutexVaryData);
	
    uint16_t index = 0;
    pb_raw_data_s *raw = NULL;

    index = g_pw_vary_buff.ed_idx;
    raw = &g_pw_vary_buff.buff[index];

	if(raw->length > 0) 
	{
		pthread_mutex_unlock(&mutexVaryData);
		//printf("raw data full\r\n");
		return;
	}
			
	memcpy(raw->data,data,len);
    raw->length = len;
	raw->msgid = msgid;
	
	g_pw_vary_buff.ed_idx++;
	if(g_pw_vary_buff.ed_idx == PB_MAX_BUFF_LEN)
		g_pw_vary_buff.ed_idx = 0;

	pthread_mutex_unlock(&mutexVaryData);
	
}


uint8_t CommConn::pb_get_vary_buff(uint8_t* data)
{
    int len = 0;
	uint8_t data_len = 0;
    len = (g_pw_vary_buff.ed_idx + PB_MAX_BUFF_LEN - g_pw_vary_buff.bg_idx) % PB_MAX_BUFF_LEN;
    if (len >= 0 && g_pw_vary_buff.buff[g_pw_vary_buff.bg_idx].length > 0){	
		
		data_len = g_pw_vary_buff.buff[g_pw_vary_buff.bg_idx].length;
		memcpy(data,g_pw_vary_buff.buff[g_pw_vary_buff.bg_idx].data,data_len);
		memset(g_pw_vary_buff.buff[g_pw_vary_buff.bg_idx].data,0,sizeof(PB_RAW_DATA_LEN));
		
        g_pw_vary_buff.buff[g_pw_vary_buff.bg_idx].length = 0;
        g_pw_vary_buff.bg_idx++;
        g_pw_vary_buff.bg_idx %= PB_MAX_BUFF_LEN;		
    }
	
	return data_len;
}

uint8_t CommConn::get_next_vary_msg_len()
{
	int len = 0;
	uint8_t data_len = 0;
	len = (g_pw_vary_buff.ed_idx + PB_MAX_BUFF_LEN - g_pw_vary_buff.bg_idx) % PB_MAX_BUFF_LEN;
	if (len >= 0 && g_pw_vary_buff.buff[g_pw_vary_buff.bg_idx].length > 0){	
		
		data_len = g_pw_vary_buff.buff[g_pw_vary_buff.bg_idx].length;
	}

	return data_len;
}

uint8_t CommConn::get_next_vary_msg_id()
{
	int len = 0;
	uint8_t msg_id = 0;
	len = (g_pw_vary_buff.ed_idx + PB_MAX_BUFF_LEN - g_pw_vary_buff.bg_idx) % PB_MAX_BUFF_LEN;
	if (len >= 0 && g_pw_vary_buff.buff[g_pw_vary_buff.bg_idx].length > 0){	
		
		msg_id = g_pw_vary_buff.buff[g_pw_vary_buff.bg_idx].msgid;
	}

	return msg_id;
}


uint16_t CommConn::vary_buf_len()
{
	uint16_t len = 0;
	len = (g_pw_vary_buff.ed_idx + PB_MAX_BUFF_LEN - g_pw_vary_buff.bg_idx) % PB_MAX_BUFF_LEN;
	ALOGI("g_pw_buff.ed_idx = %d,g_pw_buff.bg_idx = %d,len = %d\r\n",g_pw_vary_buff.ed_idx,g_pw_vary_buff.bg_idx,len);	

	return len;
}

void CommConn::init_pb_vary_buff_s()
{
	g_pw_vary_buff.bg_idx = g_pw_vary_buff.ed_idx = 0;

	for(int i = 0; i < PB_MAX_BUFF_LEN; i++)
	{
		g_pw_vary_buff.buff[i].length = 0;
		g_pw_vary_buff.buff[i].msgid = 0;
		memset(&(g_pw_vary_buff.buff[i]),0,sizeof(PB_RAW_DATA_LEN));
	}

	memset(sendBuf,0,sizeof(sendBuf));
}
#else
void CommConn::makeEmptyMsg(uint8_t *data,uint8_t response)
{
	data[0] = 0x02;
	data[1] = 0x00;
	data[2] = response;
	data[3] = 0x0b; //low byte
	data[4] = 0x00; //high byte
	for(int j = 0;j<11;j++)
	{
		data[j+5] = 0x00;
	}
	data[16] = getCKS(data,16);

	return;
}

void CommConn::getQueueMsg(/*uint8_t *data,*/uint8_t response)
{
	uint8_t nextMsgLen = 0;
	uint8_t nextMsgId = 0;
	uint8_t sendBufOffset = 0;
	uint8_t emptyMsgFlag = 0;
	uint8_t queueData[SPI_PACKAGE_SIZE];
	uint8_t queueDatalen;

	if(first2msgflag) {
		first2msgflag--;
		if(first2msgflag) {
			psendBuf = sendBuf[1];
		} else {
			psendBuf = sendBuf[0];
		}
	} else {
		if(psendBuf == sendBuf[0]) {
			if(!errorspisendcounter()) {
				psendBuf = sendBuf[1];
			} else {
				psendBuf = sendBuf[1];
				sendBufOffset += *(psendBuf + 3) + 5;
				goto getQueueMsg0;
			}
		} else {
			if(!errorspisendcounter()) {
				psendBuf = sendBuf[0];
			} else {
				psendBuf = sendBuf[0];
				sendBufOffset += *(psendBuf + 3) + 5;
				goto getQueueMsg0;
			}
		}
	}
	//memset(data,0,SPI_PACKAGE_SIZE);
	sendBufOffset = 5;  //前5个字节有固定值
	while(1)
	{	
		nextMsgLen = get_next_msg_len();
		if(nextMsgLen == 0)
		{
			if(emptyMsgFlag == 0)
			{
				//make empty msg
				makeEmptyMsg(psendBuf,response);
				return;
			}else{
				//make msg end
				break;
			}
		}else{
			nextMsgId = get_next_msg_id();
			if((sendBufOffset + nextMsgLen) > (SPI_PACKAGE_SIZE -1) || nextMsgId == 0x09)
			{
				//send json msg
				if(nextMsgId == 0x09)
				{
					if(emptyMsgFlag == 0)
					{
						memset(queueData,0,SPI_PACKAGE_SIZE);
				 		queueDatalen = pb_get_buff(queueData);
				 		memcpy(&psendBuf[sendBufOffset],queueData,queueDatalen);
				 		sendBufOffset += queueDatalen;
					}
				}
				
				//make msg end 
				break;
			}else{
				//insert data to sendBuf
				 memset(queueData,0,SPI_PACKAGE_SIZE);
				 queueDatalen = pb_get_buff(queueData);
				 memcpy(&psendBuf[sendBufOffset],queueData,queueDatalen);
				 sendBufOffset += queueDatalen;
			}
		}
		emptyMsgFlag = 1;
	}
	
	psendBuf[0] = 0x02;
	if(nextMsgId == 0x09) {
		psendBuf[1] = 0x09;
	} else {
		psendBuf[1] = 0x01;
	}
getQueueMsg0:
	psendBuf[2] = response;
	psendBuf[3] = (sendBufOffset-5) & 0xff; //low byte
	psendBuf[4] = (sendBufOffset-5) >> 8; //high byte
	psendBuf[sendBufOffset] = getCKS(psendBuf,sendBufOffset);
	
	return;
}

uint8_t CommConn::processMsg(uint8_t *recvBuf)
{
	//process
	//ALOGI("zwt spi process msg\r\n");
	uint16_t msgLen = 0;
	uint8_t msgNum = 0;
	//uint16_t msgBody[SPI_PACKAGE_SIZE];
	uint8_t msgBody[SPI_PACKAGE_SIZE];
	canData mCanData[CAN_DATA_NUM]; //(SPI_PACKAGE_SIZE-6)/11
	
	memset(msgBody,0,sizeof(msgBody));
	memset(mCanData,0,sizeof(mCanData));

    if(recvBuf[1]==0x01){
	    msgLen = (recvBuf[4] << 8) | recvBuf[3];
	    if(msgLen%11 != 0)
	    {
	    	ALOGE("zwt spi process msg,msgLen error,msgLen = %d\r\n",msgLen);
	    }else{
	    	msgNum = msgLen/11;
			//ALOGI("zwt spi process msg,msgLen = %d,msgNum = %d\r\n",msgLen,msgNum);
	    	memcpy(msgBody,&recvBuf[5],msgLen);
	    	for(uint8_t i = 0; i<msgNum; i++)
	    	{
	    		mCanData[i].busId = msgBody[i*11+0];
	    		mCanData[i].canId = msgBody[i*11+1] << 8 | msgBody[i*11+2];
	    		memcpy(mCanData[i].canBuf,&msgBody[i*11+3],8);
                std::vector<uint8_t> canvec(&msgBody[i*11+0], &msgBody[i*11+0] + 11);
	    	    // ALOGE("zhugang %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\r\n",msgBody[i*11+0],msgBody[i*11+1],msgBody[i*11+2],msgBody[i*11+3],
                //                  msgBody[i*11+4],msgBody[i*11+5],msgBody[i*11+6],msgBody[i*11+7],
                //                  msgBody[i*11+8],msgBody[i*11+9],msgBody[i*11+10]);
                mMessageProcessor->processCanReadMessage1(canvec);
	    	}
	    	
	    	//ALOGI("zwt spi process msg ok\r\n");
	    }
    }else if(recvBuf[1]==0x09){
            Json::CharReaderBuilder builder;
            std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
            Json::Value root;
            std::string errors;
            bool parsingSuccessful;

	    	//ALOGI("hqh spi process \r\n");

            /*uint8_t strRecvBuf[SPI_PACKAGE_SIZE*5+1];
            memset(strRecvBuf,0,sizeof(strRecvBuf));
            for(uint8_t j = 0; j<SPI_PACKAGE_SIZE;j++)
            {
                sprintf((char*)&strRecvBuf[j*5],"0x%02x,",recvBuf[j]);
            }
            ALOGI("hqh json recv data:%s\r\n",strRecvBuf);*/

            memcpy(msgBody, recvBuf, SPI_PACKAGE_SIZE);
            SpiSetAck();
            std::vector<uint8_t> json_str(&msgBody[5], &msgBody[SPI_PACKAGE_SIZE]);
            const char* json_cstr = reinterpret_cast<const char*>(json_str.data());
            size_t json_size = json_str.size();
            parsingSuccessful = reader->parse(json_cstr, json_cstr + json_size, &root, &errors);
            // parsingSuccessful = reader->parse(json_str.begin(), json_str.begin() + json_str.size(), &root, &errors);
            if (!parsingSuccessful) {
               LOG(ERROR)<<"cmd get Error parsing json: " << errors << std::endl;
                //LOG(ERROR)<<"cmd get Error parsing json contents: " << printVec()json_cstr << std::endl;
                std::string l = root.toStyledString();
                LOG(INFO)<<"cmd get Error"  << "root: " << l << std::endl;
                //printVec(json_str);
            }else{
                LOG(INFO)<<"zhugang json str "  << json_cstr << std::endl;
                parseDate(root);
            }
    }
	
	//return flag
	if(recvBuf[2] == 0)
		return 0;
	else if(recvBuf[2] == 1)
		return 1;
	else if(recvBuf[2] == 2)
		return 2;
	else
		return 2;//error reply
	return 0;
	
}

void CommConn::pb_add_raw_data(uint8_t *data,uint8_t len,uint8_t msgid)
{

    //printVec1(data,len);
	pthread_mutex_lock(&mutexData);
	
    uint16_t index = 0;
    pb_raw_data_s *raw = NULL;

    index = g_pw_buff.ed_idx;
    raw = &g_pw_buff.buff[index];

	if(raw->length > 0) 
	{
		pthread_mutex_unlock(&mutexData);
		//printf("raw data full\r\n");
		return;
	}
			
	memcpy(raw->data,data,len);
    raw->length = len;
	raw->msgid = msgid;
	
	g_pw_buff.ed_idx++;
	if(g_pw_buff.ed_idx == PB_MAX_BUFF_LEN)
		g_pw_buff.ed_idx = 0;

	pthread_mutex_unlock(&mutexData);
	
}


uint8_t CommConn::pb_get_buff(uint8_t* data)
{
    int len = 0;
	uint8_t data_len = 0;
    len = (g_pw_buff.ed_idx + PB_MAX_BUFF_LEN - g_pw_buff.bg_idx) % PB_MAX_BUFF_LEN;
    if (len >= 0 && g_pw_buff.buff[g_pw_buff.bg_idx].length > 0){	
		
		data_len = g_pw_buff.buff[g_pw_buff.bg_idx].length;
		memcpy(data,g_pw_buff.buff[g_pw_buff.bg_idx].data,data_len);
		memset(g_pw_buff.buff[g_pw_buff.bg_idx].data,0,sizeof(PB_RAW_DATA_LEN));
		
        g_pw_buff.buff[g_pw_buff.bg_idx].length = 0;
        g_pw_buff.bg_idx++;
        g_pw_buff.bg_idx %= PB_MAX_BUFF_LEN;		
    }
	
	return data_len;
}

uint8_t CommConn::get_next_msg_len()
{
	int len = 0;
	uint8_t data_len = 0;
	len = (g_pw_buff.ed_idx + PB_MAX_BUFF_LEN - g_pw_buff.bg_idx) % PB_MAX_BUFF_LEN;
	if (len >= 0 && g_pw_buff.buff[g_pw_buff.bg_idx].length > 0){	
		
		data_len = g_pw_buff.buff[g_pw_buff.bg_idx].length;
	}

	return data_len;
}

uint8_t CommConn::get_next_msg_id()
{
	int len = 0;
	uint8_t msg_id = 0;
	len = (g_pw_buff.ed_idx + PB_MAX_BUFF_LEN - g_pw_buff.bg_idx) % PB_MAX_BUFF_LEN;
	if (len >= 0 && g_pw_buff.buff[g_pw_buff.bg_idx].length > 0){	
		
		msg_id = g_pw_buff.buff[g_pw_buff.bg_idx].msgid;
	}

	return msg_id;
}


uint16_t CommConn::buf_len()
{
	uint16_t len = 0;
	len = (g_pw_buff.ed_idx + PB_MAX_BUFF_LEN - g_pw_buff.bg_idx) % PB_MAX_BUFF_LEN;
	ALOGI("g_pw_buff.ed_idx = %d,g_pw_buff.bg_idx = %d,len = %d\r\n",g_pw_buff.ed_idx,g_pw_buff.bg_idx,len);	

	return len;
}

void CommConn::init_pb_buff_s()
{
	g_pw_buff.bg_idx = g_pw_buff.ed_idx = 0;

	for(int i = 0; i < PB_MAX_BUFF_LEN; i++)
	{
		g_pw_buff.buff[i].length = 0;
		g_pw_buff.buff[i].msgid = 0;
		memset(&(g_pw_buff.buff[i]),0,sizeof(PB_RAW_DATA_LEN));
	}

	memset(sendBuf,0,sizeof(sendBuf));
}
#endif

int8_t CommConn::parseMsg(uint8_t *recvBuf,uint16_t len)
{
	uint16_t dataLen = 0;
	uint8_t cks;
	
	if(recvBuf[0] != 0x02) {
		ALOGI("zwt spi parse spi header error,or is a empty msg\r\n");
		return 0;
	} else {
		dataLen = (recvBuf[4] << 8) | recvBuf[3];
	}
	
	//越界 长度判断错误返回2
	if(dataLen > len - 6) {
		ALOGI("zwt spi parse spi dataLen  = %d error\r\n",dataLen);
		return 2;
	}
	cks = getCKS(recvBuf, dataLen+5);
	if(cks != recvBuf[dataLen+5]) {
        #if 1 // for debug
            std::string xbuf;
            xbuf.clear();
            for (int i = 0; i < len; i++) {
                std::ostringstream ss;
                ss.clear();
                int data = recvBuf[i];
                ss << std::hex<< std::setw(2)<<std::setfill('0') << data;
                //xbuf += "0x";
                xbuf += ss.str();
                xbuf += " ";
            }
            ALOGI("zwt spi parse spi cks error buffer:%s cks = %02x,dataLen = %d,len = %d", xbuf.c_str(),cks,dataLen,len);
        #endif

		ALOGI("zwt spi parse spi cks error\r\n");
		return 2;
	} else {
		if(recvBuf[1] == 0)
			return 0; //空内容消息
		else
			return 1; //非空内容消息，需要解析执行
	}
	
}

bool CommConn::parseJson(const std::string& filename)
{
    std::ifstream file(filename);
    if(!file.good()){
        ALOGE(" saveConfig file not exist %s", filename.c_str());
        return false;
    }
    // 1. 读取文件中的JSON数据
    if (!file.is_open()) {
        ALOGE(" saveConfig file can not open %s", filename.c_str());
        return false;
    }
    std::string fileContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();
    // 2. 使用Json::CharReader解析JSON数据
    Json::CharReaderBuilder builder;
    Json::Value root;
    std::string errors;
    std::istringstream iss(fileContent); // 将字符串转换为输入流
    bool parsingSuccessful = Json::parseFromStream(builder, iss, &root, &errors);
    if (!parsingSuccessful) {
        ALOGE(" saveConfig file parse fail %s", filename.c_str());
        return false;
    }
    // std::string oug = root.toStyledString();
    // ALOGI(" saveConfig parseJson %s", oug.c_str());
    Json::Value mergedObject;
    for (const auto& item : root) {
        for (const auto& it : item.getMemberNames()) {
            std::string key = it;
            std::string value = item[it].asString();
            //ALOGI("saveConfig key:%s, value:%s", key.c_str(), value.c_str());
            updateConfig(key, value);
        }    
    }
    return true;
}
void CommConn::updateConfig(std::string key, std::string msg)
{
    // if(key == "VehCfg"){
    //     ALOGI("saveConfig updateConfig key:%s, value:%s", key.c_str(), msg.c_str());
    //     mGetVehCfgFlag = 1;
    //     android::base::SetProperty("gecko.vehcfg", msg);
    //     return;
    // } else 
    if(key == "Vin"){
        ALOGI("saveConfig updateConfig key:%s, value:%s", key.c_str(), msg.c_str());
        mGetVinFlag = 1;
        mPropertyManager->updateCPropertyValue("gecko_vin",msg);
        return;
    } 
    // else if(key == "McuVersion"){
    //     ALOGI("saveConfig updateConfig key:%s, value:%s", key.c_str(), msg.c_str());
    //     mGetMcuVersionFlag = 1;
    //     mPropertyManager->updateCPropertyValue("gecko_mcu_version",msg);
    //     return;
    // } else if(key == "TPMS"){
    //     ALOGI("saveConfig updateConfig key:%s, value:%s", key.c_str(), msg.c_str());
        
    //     android::base::SetProperty("gecko.variant.tpms", msg);
    //     mGetTPMSFlag = 1;
    //     return;
    // } else if(key == "DMS"){
    //     ALOGI("saveConfig updateConfig key:%s, value:%s", key.c_str(), msg.c_str());
    //     android::base::SetProperty("gecko.variant.dms", msg);
    //     mGetDMSFlag = 1;
    //     return;
    // } else if(key == "PDC"){
    //     ALOGI("saveConfig updateConfig key:%s, value:%s", key.c_str(), msg.c_str());
    //     android::base::SetProperty("gecko.variant.pdc", msg);
    //     mGetPDCFlag = 1;
    //     return;
    // } else if(key == "RVC"){
    //     ALOGI("saveConfig updateConfig key:%s, value:%s", key.c_str(), msg.c_str());
    //     android::base::SetProperty("gecko.variant.rvc", msg);
    //     mGetRVCFlag = 1;
    //     return;
    // } else if(key == "SWC"){
    //     ALOGI("saveConfig updateConfig key:%s, value:%s", key.c_str(), msg.c_str());
    //     android::base::SetProperty("gecko.variant.swc", msg);
    //     mGetSWCFlag = 1; 
    //     return;
    // } else if(key == "DVR"){
    //     ALOGI("saveConfig updateConfig key:%s, value:%s", key.c_str(), msg.c_str());
    //     android::base::SetProperty("gecko.variant.dvr", msg);
    //     mGetDVRFlag = 1;
    //     return;
    // } else if(key == "CargoCam"){
    //     ALOGI("saveConfig updateConfig key:%s, value:%s", key.c_str(), msg.c_str());
    //     android::base::SetProperty("gecko.variant.cargocam", msg);
    //     mGetCargoCamFlag = 1;
    //     return;
    // } else if(key == "PowerMirrors"){
    //     ALOGI("saveConfig updateConfig key:%s, value:%s", key.c_str(), msg.c_str());
    //     android::base::SetProperty("gecko.variant.powermirrors", msg);
    //     mGetPowerMirrorsFlag = 1;
    //     return;
    // } else if(key == "BootImage"){
    //     ALOGI("saveConfig updateConfig key:%s, value:%s", key.c_str(), msg.c_str());
    //     android::base::SetProperty("persist.gecko.variant.bootimage", msg);
    //     mGetBootImageFlag = 1;
    //     return;
    // } else if(key == "TSPSite"){
    //     ALOGI("saveConfig updateConfig key:%s, value:%s", key.c_str(), msg.c_str());
    //     android::base::SetProperty("gecko.variant.tspsite", msg);
    //     mGetTSPSiteFlag = 1;
    //     return;
    // } else if(key == "Language"){
    //     ALOGI("saveConfig updateConfig key:%s, value:%s", key.c_str(), msg.c_str());
    //     android::base::SetProperty("persist.gecko.variant.language", msg);
    //     mGetLanguageFlag = 1;
    //     return;
    // } else if(key == "SocVersion"){
    //     ALOGI("saveConfig updateConfig key:%s, value:%s", key.c_str(), msg.c_str());
    //     mPropertyManager->updateCPropertyValue("gecko_soc_version",msg);
    //     return;
    // } else if(key == "Value"){
    //     ALOGI("saveConfig updateConfig key:%s, value:%s", key.c_str(), msg.c_str());
    //     mPropertyManager->updateCPropertyValueNoCheck("mc_light_level", std::stoi(msg.c_str()));
    //     return;
    // } else if(key == "INR"){
    //     ALOGI("saveConfig updateConfig key:%s, value:%s", key.c_str(), msg.c_str());
    //     mPropertyManager->updateCPropertyValue("mc_remind_inr", std::stoi(msg.c_str()));
    //     return;
    // } else if(key == "DIS"){
    //     ALOGI("saveConfig updateConfig key:%s, value:%s", key.c_str(), msg.c_str());
    //     mPropertyManager->updateCPropertyValue("mc_remind_inr", std::stoi(msg.c_str()));
    //     return;
    // } else if(key == "StartODO"){
    //     ALOGI("saveConfig updateConfig key:%s, value:%s", key.c_str(), msg.c_str());
    //     mPropertyManager->updateCPropertyValue("mc_remind_start_odo", std::stoi(msg.c_str()));
    //     return;
    // } else if(key == "StartTime"){
    //     ALOGI("saveConfig updateConfig key:%s, value:%s", key.c_str(), msg.c_str());
    //     mPropertyManager->updateCPropertyValue("mc_remind_start_time", std::stoll(msg.c_str()));
    //     return;
        
    // }else if(key == "HardWareVersion"){
    //     ALOGI("saveConfig updateConfig key:%s, value:%s", key.c_str(), msg.c_str());
    //     std::string value = trim(msg.c_str());
    //     android::base::SetProperty("gecko.mcu.hardware.version", value.c_str());
    //     android::base::SetProperty("gecko.soc.hardware.version", value.c_str());
    //     return;
    // } else if(key == "ESC"){
    //     ALOGI("saveConfig updateConfig key:%s, value:%s", key.c_str(), msg.c_str());
    //     android::base::SetProperty("gecko.abroadonfig.esc", msg);
    //     mGetESCFlag = 1;
    //     return;
    // } else if(key == "360AVM"){
    //     ALOGI("saveConfig updateConfig key:%s, value:%s", key.c_str(), msg.c_str());
    //     android::base::SetProperty("gecko.abroadonfig.360avm", msg);
    //     mGet360AVMFlag = 1;
    //     return;
    // } else if(key == "SlidingDoor"){
    //     ALOGI("saveConfig updateConfig key:%s, value:%s", key.c_str(), msg.c_str());
    //     android::base::SetProperty("gecko.abroadonfig.slidingdoor", msg);
    //     mGetSlidingDoorFlag = 1;
    //     return;
    // } else if(key == "TrunkDoor"){
    //     ALOGI("saveConfig updateConfig key:%s, value:%s", key.c_str(), msg.c_str());
    //     android::base::SetProperty("gecko.abroadonfig.trunkdoor", msg);
    //     mGetTrunkDoorFlag = 1;
    //     return;
    // } 
    else if(key == "SN"){
        ALOGI("saveConfig updateConfig key:%s, value:%s", key.c_str(), msg.c_str());
        android::base::SetProperty("gecko.serial.number", msg.c_str());
        mGetSNFlag = 1;
        return;
    } 
    // else if(key == "HeadlightH"){
    //     ALOGI("saveConfig updateConfig key:%s, value:%s", key.c_str(), msg.c_str());
    //     int32_t headlightH = static_cast<int32_t>(std::stoi(msg.c_str()));
    //     mPropertyManager->updateCPropertyValueNoCheck("gecko_headlighth", headlightH);
    //     return;
    // } 
    else if(key == "F187"){
        ALOGI("saveConfig updateConfig key:%s, value:%s", key.c_str(), msg.c_str());
        android::base::SetProperty("gecko.oem.part.no", msg.c_str());
        mGetF187Flag = 1;
        return;

    // else if(key == "DriveMode"){
    //     ALOGI("saveConfig updateConfig key:%s, value:%s", key.c_str(), msg.c_str());
    //     int32_t value = mPropertyManager->getCProValue(mPropertyManager->findCProperty("ec_vcu_drive_mode"));
    //     int32_t target = std::stoi(msg.c_str());
    //     ALOGD("DriveModeSet vcu_drive_mode value:%d  target:%d", value, target);
    //     if(value != 0) {
    //         mPropertyManager->updateCPropertyValueNoCheck("ec_vcu_drive_mode", target);
    //         mPropertyManager->updateCPropertyValueNoCheck("sc_ccm_drive_mode_req", target);
    //     }
    //     return;
    // } else if(key == "SinglePedCtr"){
    //     ALOGI("saveConfig updateConfig key:%s, value:%s", key.c_str(), msg.c_str());
    //     mPropertyManager->updateCPropertyValueNoCheck("ec_vcu_single_ped_sts", std::stoi(msg.c_str()));
    //     mPropertyManager->updateCPropertyValueNoCheck("sc_ccm_single_ped_ctr_cmd", std::stoi(msg.c_str()));
    //     return;
    // } else if(key == "CreepCtr"){
    //     ALOGI("saveConfig updateConfig key:%s, value:%s", key.c_str(), msg.c_str());
    //     mPropertyManager->updateCPropertyValueNoCheck("ec_vcu_creep_sts", std::stoi(msg.c_str()));
    //     mPropertyManager->updateCPropertyValueNoCheck("sc_ccm_creep_ctr_cmd", std::stoi(msg.c_str()));
    //     return;
    // } else if(key == "ParkUnLock"){
    //     ALOGI("saveConfig updateConfig key:%s, value:%s", key.c_str(), msg.c_str());
    //     mPropertyManager->updateCPropertyValueNoCheck("bc_bcm_park_un_lock_ack", std::stoi(msg.c_str()));
    //     mPropertyManager->updateCPropertyValueNoCheck("sc_ccm_park_un_lock_req", std::stoi(msg.c_str()));
    //     return;
     } else if(key == "ABStatus"){
         ALOGI("saveConfig updateConfig key:%s, value:%s", key.c_str(), msg.c_str());
         uint8_t var = std::stoi(msg);
         if((var&0x80)==0x80){
             writeMCUAbStatus("1");
             ALOGI("cmd get to set ABstatus to B");
             android::base::SetProperty("gecko.mcu.abstatus", "B"); 
         }else{
             writeMCUAbStatus("0");
             ALOGI("cmd get to set ABStatus to A");
             android::base::SetProperty("gecko.mcu.abstatus", "A"); 
         }
         return;
     } 
}
// void CommConn::parseSecondJson(const std::string& filename)
// {
//     std::ifstream file(filename);
//     if(!file.good()){
//         ALOGE(" saveConfig file not exist %s", filename.c_str());
//     }
//     // 1. 读取文件中的JSON数据
//     if (!file.is_open()) {
//         ALOGE(" saveConfig file can not open %s", filename.c_str());
//         return;
//     }
//     std::string fileContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
//     file.close();
//     // 2. 使用Json::CharReader解析JSON数据
//     Json::CharReaderBuilder builder;
//     Json::Value root;
//     std::string errors;
//     std::istringstream iss(fileContent); // 将字符串转换为输入流
//     bool parsingSuccessful = Json::parseFromStream(builder, iss, &root, &errors);
//     if (!parsingSuccessful) {
//         ALOGE(" saveConfig file parse fail %s", filename.c_str());
//         return;
//     }
//     std::string oug = root.toStyledString();
//     ALOGI(" saveConfig parseJson %s", oug.c_str());
//     Json::Value mergedObject;
//     for (const auto& item : root) {
//         // 将每个对象中的所有键值对添加到mergedObject中
//         for (const auto& item1 : item) {
//             oug = item1.toStyledString();
//             ALOGI(" saveConfig parseSecondJson %s", oug.c_str());
//         }
//         // oug = item.toStyledString();
//         //     ALOGI(" saveConfig parseJson111 %s", oug.c_str());
//         // parseDate(item);
        
//     }
    
// }
void CommConn::queryConfigWaitUntil(std::string key, std::condition_variable& cv_query)
{
    std::unique_lock<std::mutex> lock(mtx);
    syncVersionInfo(key, "?");
    for (size_t i = 0; i < 5; i++)
    {
        if(cv_query.wait_until(lock, std::chrono::system_clock::now() + std::chrono::milliseconds(200)) == std::cv_status::timeout)
        {
            ALOGI("queryConfigWaitUntil timeout retry key: %s", key.c_str());
            syncVersionInfo(key, "?");
        }else{
            ALOGI("queryConfigWaitUntil completed key: %s", key.c_str());
            break;
        }
    }
    
}

void CommConn::check_send_back_json()
{
    int ret = 0;
    uint16_t len = 0;
    std::vector<uint8_t> out_data;
    std::string json_value_string;
    char buffer[100];
    Json::Value root;
    char buf[4];

    if (vendor_store_id == VENDOR_ECU_SN_ID) {
        len = VEHICLE_ECU_MSG_LEN;
    }
    else if (vendor_store_id == VENDOR_VEHICLE_VIN_ID) {
        len = VEHICLE_VIN_MSG_LEN;
    }
    else if (vendor_store_id == VENDOR_VEHICLE_TYPE_ID) {
        len = VEHICLE_TYPE_MSG_LEN;
    }
    else if (vendor_store_id == VENDOR_VEHICLE_OEM_DEV_ID) {
        len = VEHICLE_OEM_MSG_LEN;
    }
    else if (vendor_store_id == VENDOR_VEHICLE_FEATURE_ID) {
        len = VEHICLE_FEATURE_MSG_LEN;
    }
    else if (vendor_store_id == VENDOR_VEHICLE_TSP_ID) {
        len = VEHICLE_TSP_MSG_LEN;
    }
    else if (vendor_store_id == VENDOR_SET_FLAG_ID) {
        len = VENDOR_SET_FLAG_LEN;
    }
    else if (vendor_store_id == VENDOR_MCU_VERSION_ID) {
        len = MCU_VERSION_MSG_LEN;
    }
    else if (vendor_store_id == VENDOR_MCU_AB_PT_ID) {
        len = MCU_AB_PT_MSG_LEN;
    }
    else {
        ALOGE("check_send_back_json: err store id return...");
        return;
    }
    ret = ItemGetByVehicleService(vendor_store_id, out_data, len);
    if (0 == ret) {
        if (!out_data.empty()) {
            std::string hex_string;
            for (uint8_t byte : out_data) {
                snprintf(buf, sizeof(buf), "%02X", byte);
                hex_string += buf;
            }
            int write_len = snprintf(buffer, sizeof(buffer), "%04X%04X", json_config_id, len);
            if (write_len > 0 && write_len < (int)sizeof(buffer)) {
                json_value_string.assign(buffer, write_len);
            } else {
                ALOGE("snprintf truncation or error! ret=%d", write_len);
            }
            json_value_string+=hex_string;
            json_value_string+="11";
            root["ItemToSoc"] = json_value_string;
            Json::StreamWriterBuilder builder;
            builder["indentation"] = "";
            std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
            std::ostringstream oss;
            writer->write(root, &oss);
            std::string str = oss.str();
            ALOGE("send_back_json: %s", str.c_str());
            std::vector<uint8_t> vec(str.begin(), str.end());
            mPropertyManager->jsonQueueEnqueue(vec);
        }
        else {
            ALOGE("send_back_json err!!!");
        }
    }
}

int CommConn::check_vendor_storage_earse()
{
    int ret = 0;
    std::vector<uint8_t> out_value;
    ret = vehicle_vendor_storage_read(VENDOR_SET_FLAG_ID, VENDOR_SET_FLAG_LEN, out_value);
    if (0 != ret) {
        ALOGE("check_vendor_storage_earse vendor storage read err, ret %d", ret);
        return -1;
    }
    std::vector<uint8_t> exp_value (8, 0x55);
    bool eq_ret = std::equal(out_value.begin(), out_value.end(), exp_value.begin());
    if (eq_ret == true) {
        ALOGE("check_vendor_storage_earse vendor storage read ok set ok %d", ret);
        return 0;
    }
    else {
        ALOGE("check_vendor_storage_earse vendor storage read ok set err %d", ret);
        return 1;
    }
}
void CommConn::createCanMsgThread_V1_2() 
{
    std::ifstream fileStream;
    std::vector<uint8_t> msg;
    std::vector<uint8_t> ack;
    std::vector<uint8_t> okmsg{'o', 'k'};
    int i = 0, j = 0;
    mpAC_HAVCState = mPropertyManager->findCProperty("sc_ccm_ac_havc_state");
    mpACkey = mPropertyManager->findCProperty("sc_ccm_a_ckey");
    mpHuAcState = mPropertyManager->findCProperty("sc_hu_ac_state");
    mpHeatKey = mPropertyManager->findCProperty("sc_ccm_heat_key");
    mpFreshReciKey = mPropertyManager->findCProperty("sc_ccm_fresh_reci_key");
    mpFrontDefrostKey = mPropertyManager->findCProperty("sc_ccm_front_defrost_key");
    mpBlowerLevel = mPropertyManager->findCProperty("sc_ccm_blower_level");
    mpAIRDistributionModeReq = mPropertyManager->findCProperty("sc_ccm_air_distribution_mode_req");
    mpLeftSetTemp = mPropertyManager->findCProperty("sc_ccm_left_set_temp");
    mpLeftBackTemp = mPropertyManager->findCProperty("bc_ac_left_set_temperature");
    mpRearFogLampSet = mPropertyManager->findCProperty("sc_ccm_rear_fog_lamp_set");
    mpDriveModeReq = mPropertyManager->findCProperty("sc_ccm_drive_mode_req");
    mpRechrgnLvReq = mPropertyManager->findCProperty("sc_ccm_rechrgn_lv_req");
    mpLeftMirrorSet = mPropertyManager->findCProperty("sc_ccm_left_mirror_set");
    mpRightMirrorSet = mPropertyManager->findCProperty("sc_ccm_right_mirror_set");
    mpAutoUnlockSet = mPropertyManager->findCProperty("sc_ccm_auto_unlock_req");
    mpAutoLockSet = mPropertyManager->findCProperty("sc_ccm_auto_lock_req");
    mpParkUnlockSet = mPropertyManager->findCProperty("sc_ccm_park_un_lock_req");
    //mpBCAvasState = mPropertyManager->findCProperty("sc_ccm_avasset");  // 低速行人功能
    mpCreepCtrCmd = mPropertyManager->findCProperty("sc_ccm_creep_ctr_cmd");
    mpSinglePedCtrCmd = mPropertyManager->findCProperty("sc_ccm_single_ped_ctr_cmd");
    mpTotalOdometer = mPropertyManager->findCProperty("sc_ccm_vehicle_spd_dsp");
    mpRemtCtrlFlag = mPropertyManager->findCProperty("sc_ccm_remt_ctrl_flag");
    mpRemoteLock   = mPropertyManager->findCProperty("sc_ccm_remote_lock");
    mVcuVersion = mPropertyManager->findCProperty("vcu_version");
    mSpeedLimitLevel =  mPropertyManager->findCProperty("sc_ec_ccm_speed_limit_level");
    mSetValueFlag =  mPropertyManager->findCProperty("set_value_flag");

    mpOpenDoorWarnSet = mPropertyManager->findCProperty("sc_ccm_bcm_open_door_warn_set");
    mpChargingConditions = mPropertyManager->findCProperty("sc_ccm_charging_conditions");
    mpOperatLicence = mPropertyManager->findCProperty("sc_ccm_operat_licence");
    mpVehicleSpdDsp = mPropertyManager->findCProperty("sc_ccm_vehicle_spd_dsp");
    mpRemaRangeDispMode = mPropertyManager->findCProperty("sc_rema_range_display_mode_down");
    mpActualRemaRange = mPropertyManager->findCProperty("ec_vcu_actual_rema_range");
    mpActualRemaRangeVd = mPropertyManager->findCProperty("ec_vcu_actual_rema_range_vd");
    mpCltcRemaRange = mPropertyManager->findCProperty("ec_vcu_cltc_rema_range");
    mpCltcRemaRangeVd = mPropertyManager->findCProperty("ec_vcu_cltc_rema_range_vd");
    mpCentralLockSwitchReq = mPropertyManager->findCProperty("sc_ccm_central_lock_switch");
    mpEnergyRecovLevelCmd = mPropertyManager->findCProperty("sc_ccm_energy_recov_level_cmd");
    mpCCMVersion = mPropertyManager->findCProperty("sc_ccm_version");
    mpCCMVehTarSoc = mPropertyManager->findCProperty("chraging_target_soc");
    mpCCMTargetChrgCurr = mPropertyManager->findCProperty("sc_ec_ccm_target_chrg_curr");
    mpHybridEcoMode = mPropertyManager->findCProperty("sc_hybird_eco_mode");
    mpPwrOffReq     = mPropertyManager->findCProperty("sc_pwr_off_req");
    mpWiperIntervalTime = mPropertyManager->findCProperty("sc_wiper_interval_time");
    mpFlwrMeHmCtrl  = mPropertyManager->findCProperty("sc_flwr_me_hm_ctrl");
    mpIllmndUnLockSts = mPropertyManager->findCProperty("sc_illmnd_unlock_sts");
    mpIllmndLockSts = mPropertyManager->findCProperty("sc_illmnd_lock_sts");
    mpLockHornOnSt  = mPropertyManager->findCProperty("sc_lock_horn_on_st");
    mpEscFuncEnaStaResvd = mPropertyManager->findCProperty("sc_esc_func_ena_sta_resvd");
    mpAvhEnb = mPropertyManager->findCProperty("sc_avh_enb");
    mpEpbModal = mPropertyManager->findCProperty("sc_epb_modal");
    mpBrkDisWipSw = mPropertyManager->findCProperty("sc_brk_dis_wip_sw");
    mpElecSteerModeSet = mPropertyManager->findCProperty("sc_elec_steer_mode_set");
    mpDrivingPowerSt = mPropertyManager->findCProperty("sc_driving_power_set");
    mpCCMTboxRemoteLock = mPropertyManager->findCProperty("sc_ccm_tbox_remote_lock");
    mpCCMTboxRemoteCarSearch = mPropertyManager->findCProperty("sc_ccm_remote_car_search");
    mpHuVehMaxCurrent = mPropertyManager->findCProperty("sc_hu_veh_max_current");
    mpAcUnlockVentSet =  mPropertyManager->findCProperty("sc_hu_ac_unlock_vent_set");
    mpHuAvasSetKey = mPropertyManager->findCProperty("sc_ccm_avasset");// 低速行人功能
    mpHuTpmsResetReq = mPropertyManager->findCProperty("sc_hu_tpms_reset_req");
    mpHuTpmsLearningReq = mPropertyManager->findCProperty("sc_hu_tpms_learning_req");
    mpHuSpdDowninCorEnableSw = mPropertyManager->findCProperty("sc_hu_spd_downin_cor_enable_sw");
    mpHuRcwEnableSw = mPropertyManager->findCProperty("sc_hu_rcw_enable_sw");
    mpHuDowEnaSw = mPropertyManager->findCProperty("sc_hu_dow_ena_sw");
    mpHuBsdEnableSw = mPropertyManager->findCProperty("sc_hu_bsd_enable_sw");
    mpHuRctaEnableSw = mPropertyManager->findCProperty("sc_hu_rcta_enable_sw");
    mpHuLkaEnableSw = mPropertyManager->findCProperty("sc_hu_lka_enable_sw");
    mpHuLdwEnableSw = mPropertyManager->findCProperty("sc_hu_ldw_enable_sw");
    mpHuElkEnbSw = mPropertyManager->findCProperty("sc_hu_elk_enb_sw");
    mpHuAebEnableSw = mPropertyManager->findCProperty("sc_hu_aeb_enable_sw");
    mpHuVehTarSoc = mPropertyManager->findCProperty("sc_charging_full_soc");
    mpCdcBackLadjval = mPropertyManager->findCProperty("sc_cdc_back_ladjval");
    mpHuBcmotaModeSet = mPropertyManager->findCProperty("hu_bcmota_mode_set");
    mpHuMaiDrvrSeatHeat = mPropertyManager->findCProperty("sc_hu_mai_drvr_seat_heat");
    mpHuSteerWhlHeat = mPropertyManager->findCProperty("sc_hu_steer_whl_heat");
    mpHuHmaEnableSw = mPropertyManager->findCProperty("sc_hu_hma_enable_sw");
    mphu_ica_ena_swt = mPropertyManager->findCProperty("sc_hu_ica_ena_swt");
    mphu_ica_avd_truck_set = mPropertyManager->findCProperty("sc_hu_ica_avd_truck_set");
    mphu_ldw_alert_method_sw = mPropertyManager->findCProperty("sc_hu_ldw_alert_method_sw");
    mphu_fcw_enable_sw = mPropertyManager->findCProperty("sc_hu_fcw_enable_sw");
    mphu_slif_enable_sw = mPropertyManager->findCProperty("sc_hu_slif_enable_sw");
    mphu_slwf_enable_sw = mPropertyManager->findCProperty("sc_hu_slwf_enable_sw");
    mpTboxRemotePowerLock = mPropertyManager->findCProperty("sc_tbox_remote_power_lock");
    mpTboxRemotePowerLockV = mPropertyManager->findCProperty("sc_tbox_remote_power_lock_v");
    mpTboxRemtSteerWhlHeatReq = mPropertyManager->findCProperty("sc_tbox_remt_steer_whl_heat_req");
    mpTboxRemoteDefrostCtl = mPropertyManager->findCProperty("sc_tbox_remote_defrost_ctl");
    mpTboxRemtPowerCtrlReq = mPropertyManager->findCProperty("sc_tbox_remt_power_ctrl_req");
    mpTboxRemtMaiDrSeatHeatReq = mPropertyManager->findCProperty("sc_tbox_remt_mai_dr_seat_heat_req");
    mpHuNavCountryId = mPropertyManager->findCProperty("hu_nav_country_id");
    mpHuNaviCountryCode = mPropertyManager->findCProperty("hu_navi_country_code");
    mpCdcActiveWakeUp = mPropertyManager->findCProperty("sc_cdc_active_wakeup");
    mpCdcWakeUpReas   = mPropertyManager->findCProperty("sc_cdc_wakeup_reas");
    mpCdcKeepAwakeReasRemReq = mPropertyManager->findCProperty("sc_cdc_keep_awake_reas_rem_req");
    mpCdcKeepAwakeReasFacReset = mPropertyManager->findCProperty("sc_cdc_keep_awake_reas_fac_reset");
    mpCdcKeepAwakeReasRestart = mPropertyManager->findCProperty("sc_cdc_keep_awake_reas_restart");
    mpCdcKeepAwakeReasDO = mPropertyManager->findCProperty("sc_cdc_keep_awake_reas_do");
    mpCdcKeepAwakeReasLocUp = mPropertyManager->findCProperty("sc_cdc_keep_awake_reas_loc_up");
    mpCdcKeepAwakeSta = mPropertyManager->findCProperty("sc_cdc_keep_awake_sta");
    mpCdcActiveWakeUpTimeout = mPropertyManager->findCProperty("sc_cdc_active_wakeup_t");
    mpCdcWakeUpReasTimeout   = mPropertyManager->findCProperty("sc_cdc_wakeup_reas_t");
    mpCdcKeepAwakeReasRemReqTimeout = mPropertyManager->findCProperty("sc_cdc_keep_awake_reas_rem_req_t");
    mpCdcKeepAwakeReasFacResetTimeout = mPropertyManager->findCProperty("sc_cdc_keep_awake_reas_fac_reset_t");
    mpCdcKeepAwakeReasRestartTimeout = mPropertyManager->findCProperty("sc_cdc_keep_awake_reas_restart_t");
    mpCdcKeepAwakeReasDOTimeout = mPropertyManager->findCProperty("sc_cdc_keep_awake_reas_do_t");
    mpCdcKeepAwakeReasLocUpTimeout = mPropertyManager->findCProperty("sc_cdc_keep_awake_reas_loc_up_t");
    mpCdcKeepAwakeStaTimeout = mPropertyManager->findCProperty("sc_cdc_keep_awake_sta_t");
    mpHuBcmCargoLightSet = mPropertyManager->findCProperty("sc_hu_bcm_cargo_light_set");

    int communication_start = 0;
    while (true) {
        if(0==communication_start){
            std::this_thread::sleep_for(std::chrono::milliseconds(6000));
            android::base::SetProperty("gecko.mcu.hardware.version", "HW:D.01.01");
            android::base::SetProperty("gecko.soc.hardware.version", "HW:D.01.01");
            for(int i = 0; i < 3; i++){
                if(parseJson("/data/vendor/vehicle/config.json")){
                    break;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
            conf_ck_ret = check_vendor_storage_earse();
            if (conf_ck_ret == -1) {
                for (i = 0; i<3; i++) {
                    conf_ck_ret = check_vendor_storage_earse();
                    if (conf_ck_ret == 0) {
                        break;
                    }
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            mConfigWordParser.initConfigurationWordData(mPropertyManager);
            mSpeedLimitLevel =  mPropertyManager->findCProperty("sc_ec_ccm_speed_limit_level");
            mpChargingConditions = mPropertyManager->findCProperty("sc_ccm_charging_conditions");
            mpOperatLicence = mPropertyManager->findCProperty("sc_ccm_operat_licence");
            mpCCMVersion = mPropertyManager->findCProperty("sc_ccm_version");
            mpHuNavCountryId = mPropertyManager->findCProperty("hu_nav_country_id");
            mpBrkDisWipSw = mPropertyManager->findCProperty("sc_brk_dis_wip_sw");
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            std::this_thread::sleep_for(std::chrono::milliseconds(50));      
            syncVersionInfo("SocVersion", android::base::GetProperty("ro.build.id", "SV00.00"));
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            if(mGetBackLightFlag == 0){
                queryConfigWaitUntil("BackLight", cv_BackLight);
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            syncVersionInfo("Mtce", "?");
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            syncVersionInfo("HardWareVersion", "?");
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            setTboxLinkTsp();
            std::string code = android::base::GetProperty("persist.navi.country.code", "0");
            if(code != "0"){
                ALOGI("update persist.navi.country.code:%s", code.c_str());
                mPropertyManager->updateCPropertyValue(mpHuNaviCountryCode, std::stoi(code));
            }

            std::string fcw = android::base::GetProperty("persist.fcw.enable.sw", "0");
            if(fcw != "0"){
                ALOGI("update persist.fcw.enable.sw:%s", fcw.c_str());
                mPropertyManager->updateCPropertyValue(mphu_fcw_enable_sw, std::stoi(fcw));
            }

            std::string aeb = android::base::GetProperty("persist.aeb.enable.sw", "0");
            if(aeb != "0"){
                ALOGI("update persist.aeb.enable.sw:%s", aeb.c_str());
                mPropertyManager->updateCPropertyValue(mpHuAebEnableSw, std::stoi(aeb));
            }

            communication_start = 1;
        }else{

            createCanMsgHuABac();
            createCanMsgHuBBac();
            createCanMsgHuCBac();
            createCanMsgNMAsrCdcBac();
            createCanMsgTboxBjsTimeBac();
            j++;
            if (j==10) {
                send_heartbeat_msg();
                j=0;
            }
            sendCPUTempToMcu();
            sendSIMCardStsToMcu();
            sendWifiInfo();
            mSendCameraStsCounter++;
            if(mSendCameraStsCounter%10 == 0) {
                sendCameraStsToMcu();
                mSendCameraStsCounter = 0;
            }
            if (0 != json_config_id) {
                check_send_back_json();
                json_config_id = 0;
                vendor_store_id = 0;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));      
        }
    }
}

void CommConn::clearspisendcounter(void)
{
	if(psendBuf == sendBuf[0])
		spisendcounter[1] = 0;
	else
		spisendcounter[0] = 0;
}

uint8_t CommConn::incspisendcounter(void)
{
	if(psendBuf == sendBuf[0]) {
		spisendcounter[1]++;
		return spisendcounter[1];
	} else {
		spisendcounter[0]++;
		return spisendcounter[0];
	}
}

uint8_t CommConn::errorspisendcounter(void)
{
	if(psendBuf == sendBuf[0]) {
		return spisendcounter[1];
	} else {
		return spisendcounter[0];
	}
}
uint8_t CommConn::adjust_spitrnullmsg(void)
{
	if(psendBuf == sendBuf[0])
		return sendBuf[1][1];
	else
		return sendBuf[0][1];
}

void CommConn::makeStartOtaMsg(uint8_t *data) //json msg
{
	Json::Value root;
    root["MCU_OTA"] = "2";
    // 输出JSON字符串
    Json::StreamWriterBuilder builder;
    builder["indentation"] = "";
    std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
    std::ostringstream oss;
    writer->write(root, &oss);
    // 获取字符串
    std::string str = oss.str(); 			   
    ALOGI("zwt spi mcu update send cmd to mcu %s\n\r",str.c_str());
    std::vector<uint8_t> vec(str.begin(), str.end());
				   
	data[0] = 0x02;
	data[1] = 0x09;
	data[2] = 0;
	data[3] = vec.size() & 0xff; //low byte
	data[4] = (vec.size() >> 8) & 0xff; //high byte
	memcpy(&data[5],vec.data(),vec.size());
	data[5+vec.size()] = getCKS(data,5+vec.size());

	return;
}


//收到包号的回复，返回0
//没收到包号的回复，返回1
//收到MCU返回升级成功消息，返回2
uint8_t CommConn::parseOtaMsg(uint8_t *recvBuf,uint16_t len)
{
	uint16_t dataLen = 0;
	uint8_t cks;

//判断包头是否有误	
	if(recvBuf[0] != 0x02) {
		ALOGI("zwt spi parse ota spi header error,or is a empty msg\r\n");
		return 1;
	} else {
		dataLen = (recvBuf[4] << 8) | recvBuf[3];
	}
	
//越界 长度判断错误返回1
	if(dataLen > len - 6) {
		ALOGI("zwt spi parse ota len error\r\n");
		return 1;
	}

//校验错误
	cks = getCKS(recvBuf, dataLen+5);
	if(cks != recvBuf[dataLen+5]) {
		ALOGI("zwt spi parse spi cks error\r\n");
		return 1;
	} else {
		if(recvBuf[1] == 0)
		{
			//空消息
			ALOGI("zwt spi parse spi empty msg\r\n");
			return 1;
		}else{

			if(recvBuf[2] == 2)
			{
				//上一包接收错误
				ALOGI("zwt spi parse spi last msg is error\r\n");
				return 1;
			}else{
				//收到的消息是正确的ack消息
				if(recvBuf[1] == 0x04){
					//解析升级包的回复,在getQueueMsg中判断，如果序号回复确认了，才组下一包，不然触发重发机制，重发三次。
					otaAckIndex = recvBuf[6] << 8 | recvBuf[5];
					if(otaIndex == otaAckIndex)
					{
						ALOGI("zwt spi recv otaAckIndex = %d\r\n",otaAckIndex);
						otaIndex++;
						otaReSendCount = 0;
						return 0;
					}else{
						return 1;
					}
				}else if(recvBuf[1] == 0x05){
					writeMCUUpdateFlag("2");
					ALOGI("zwt spi update mcu success\r\n");
					return 2;
				}

			}
			
		}
	}

	return 0;

}

void CommConn::makeOtaEmptyMsg(uint8_t *data)
{
	data[0] = 0x02;
	data[1] = 0x00;
	data[2] = 0;
	data[3] = 0x0b; //low byte
	data[4] = 0x00; //high byte
	for(int j = 0;j<11;j++)
	{
		data[j+5] = 0x00;
	}
	data[16] = getCKS(data,16);

	return;
}

void CommConn::reboot()
{
	//编译不过
	//__reboot( LINUX_REBOOT_MAGIC1, LINUX_REBOOT_MAGIC2, LINUX_REBOOT_CMD_RESTART,NULL );
}

void CommConn::startMcuOta()
{
	uint8_t repeat = 0;
	do {
			writeMCUUpdateFlag("1");
			android::base::SetProperty("gecko.mcu.update.permission", "666");
			updateFlag = readMCUUpdateFlag();
			if(updateFlag == "1") 
			{
				ALOGI("zwt spi updateFlag = %s\r\n",updateFlag.c_str());
				break;
			}
		repeat ++;
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}while(repeat < 10);

	makeStartOtaMsg(psendBuf); //发送ota start消息到mcu，mcu重启，引起SOC重启

	return;
}

//return 0:组下一包消息成功。
//return 1:组下一包消息失败。
//retutn 2:其他错误
uint8_t CommConn::sendMcuOtaMsg()
{
	uint8_t sendBufOffset = 0;

	if(needMakeMsgFlag == 1)
	{
		//发新的一包数据
		uint8_t otaDataLen = SPI_PACKAGE_SIZE-6;  //6字节为包头和校验码
		uint8_t otaPackage[otaDataLen];
		
		if(fileSizeSendFlag == 0)
		{
			//开始发文件大小和md5
			uint8_t filesize1 = 0;
	        uint8_t filesize2 = 0;
	        uint8_t filesize3 = 0;
	        uint8_t filesize4 = 0;
			//判断文件是否存在
			for(int i = 0; i < 10; i++)
			{
				if(fileExists(gFilePath) == false)
				{
					ALOGI("zwt spi update file %s don't exist\r\n",gFilePath.c_str());
				}else{
					ALOGI("zwt spi update file exist\r\n");
					break;
				}
				usleep(1000*10);

				if(i >= 9)
				{
					ALOGI("zwt spi update file don't exist,error\r\n");
					return 2;
				}

			}
			
			//设置标志位
	        android::base::SetProperty("gecko.mcu.updateing", "1");

			//计算文件大小
	        otafilesize = getFileSize(gFilePath.c_str());
	        ALOGI("zwt spi mcu update fileSize %lu",otafilesize);
	  
	        filesize1 = otafilesize & 0xFF; 
	        filesize2 = (otafilesize >> 8) & 0xFF; 
	        filesize3 = (otafilesize >> 16) & 0xFF; 
	        filesize4 = (otafilesize >> 24) & 0xFF;
			otaPackage[0] = otaIndex & 0xff;
			otaPackage[1] = (otaIndex >> 8) & 0xff;
			otaPackage[2] = filesize4;
			otaPackage[3] = filesize3;
			otaPackage[4] = filesize2;
			otaPackage[5] = filesize1;

			//计算MD5
			std::string filemd5 = calculateMd5(gFilePath);
	        ALOGI("zwt spi mcu update md5: %s\n",filemd5.c_str());
	        for (size_t i = 0, j = 0; i < filemd5.length(); i += 2,j++) {
	            std::string byteString = filemd5.substr(i, 2);
	            uint8_t byte = static_cast<uint8_t>(strtol(byteString.c_str(), nullptr, 16));
	            otaPackage[6+j] = byte;
				//ALOGI("zwt spi mcu update md5: 0x%x,j = %zu\n",otaPackage[6+j],j);
	        }

			//组包
			sendBufOffset = 5;  //前5个字节有固定值
			psendBuf[0] = 0x02;
			psendBuf[1] = 0x04;
			psendBuf[2] = 0;
			memcpy(&psendBuf[sendBufOffset],otaPackage,22);
			sendBufOffset += 22;
			psendBuf[3] = (sendBufOffset-5) & 0xff; //low byte
			psendBuf[4] = (sendBufOffset-5) >> 8; //high byte
			psendBuf[sendBufOffset] = getCKS(psendBuf,sendBufOffset);


			//ALOGI("zwt spi first msg len= %d\r\n",SPI_PACKAGE_SIZE);
			//printVec2(psendBuf,SPI_PACKAGE_SIZE);

			//备份一次数据，如果不能收到回复，需要重发
			memcpy(sendOtaLastBuf,psendBuf,SPI_PACKAGE_SIZE);

			//设置下次读文件的标志位
			fileSizeSendFlag = 1;

			needParseAckFlag = 0;
			return 0;
		}else{

			//打开文件
			if(fp == NULL)
			{
				fp = fopen(gFilePath.c_str(), "rb");
				if (fp == NULL) {
					ALOGI("zwt spi open update file: %s failed\n",gFilePath.c_str());
					return 2;
				}else{
					ALOGI("zwt spi open update file: %s success\n",gFilePath.c_str());
				}
			}
			
			//顺序发文件内容
			size_t bytesRead = 0;
			uint8_t readLen =  0;
			if(4096 - fileReadLen >= 246)
			{
				//凑4K数据，差值大于等于246;应该读246内容;
				//进一步判断剩余的文件大小，大于等于246，则读246大小内容，不够246，读剩下所有内容;
				if(otafilesize - fileReadLenTotal >= 246)
				{
					readLen = 246;
				}else{
					readLen = otafilesize - fileReadLenTotal;
				}
			}else{
				//凑4K数据，差值小于等于246，读剩下的差值大小的内容;
				//进一步判断剩余的文件大小，剩下文件的大小，大于差值，读差值的内容;
				//剩下文件的大小，小于差值，读剩余文件的内容;
				if(4096 - fileReadLen <= otafilesize - fileReadLenTotal)
				{
					readLen = 4096 - fileReadLen;
				}else{
					readLen = otafilesize - fileReadLenTotal;
				}
			}
			ALOGI("zwt spi read file,readLen %d\r\n",readLen);
			memset(otaPackage,0,otaDataLen);
			bytesRead = fread(otaPackage+4, readLen, 1, fp);
			if(bytesRead > 0){
				otaPackage[0] = otaIndex & 0xff;
				otaPackage[1] = (otaIndex >> 8) & 0xff;
				otaPackage[2] = (readLen >> 8) & 0xff;
				otaPackage[3] = readLen & 0xff;
				psendBuf[0] = 0x02;
				psendBuf[1] = 0x04;
				psendBuf[2] = 0;
				sendBufOffset = 5;	//前5个字节有固定值
				memcpy(&psendBuf[sendBufOffset],otaPackage,readLen+4);
				sendBufOffset += (readLen+4);
				
				psendBuf[3] = (sendBufOffset-5) & 0xff; //low byte
				psendBuf[4] = (sendBufOffset-5) >> 8; //high byte
				psendBuf[sendBufOffset] = getCKS(psendBuf,sendBufOffset);

				fileReadLen += readLen;
				ALOGI("zwt spi read file,fileReadLen = %d\r\n",fileReadLen);
				
				if(fileReadLen == 4096) fileReadLen = 0;

				fileReadLenTotal += readLen;

				ALOGI("zwt spi read file,otafilesize = %ld,fileReadLenTotal = %ld\r\n",otafilesize,fileReadLenTotal);

				float mcuOtaProg = 0;
				int mcuOtaProgInt = 0;
				mcuOtaProg = (float)fileReadLenTotal/otafilesize;
				mcuOtaProgInt = mcuOtaProg*100;

				char strMcuOtaProg[50];
				memset(strMcuOtaProg,0,sizeof(strMcuOtaProg));
				sprintf(strMcuOtaProg, "%d", mcuOtaProgInt);
				android::base::SetProperty("ota.mcu.progvalue", strMcuOtaProg);

				//备份一次数据，如果不能收到回复，需要重发
				memcpy(sendOtaLastBuf,psendBuf,SPI_PACKAGE_SIZE);

				needParseAckFlag = 0;
				return 0;
			}else if (bytesRead == 0) {
				ALOGI("zwt spi read update file end\r\n");
				if(fp != NULL)
				{
					fclose(fp);
				}
				makeOtaEmptyMsg(psendBuf);
				needParseAckFlag = 1;
				return 0;
			}else{
				ALOGI("zwt spi read update file error\r\n");
				if(fp != NULL)
				{
					fclose(fp);
				}
				makeOtaEmptyMsg(psendBuf);
				needParseAckFlag = 1;
				return 0;
			}

		}
	}else{
		//发空包，读上一个发出去的包的回复
		ALOGI("zwt spi send EmptyMsg\r\n");
		makeOtaEmptyMsg(psendBuf);
		needParseAckFlag = 1;
		return 0;
	}

	return 0;
}

void CommConn:: send_heartbeat_msg() {
    uint8_t soc_pwr_rdy = 0;
    std::vector<uint8_t> hb_can_value={0x2,0x66,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    hb_idx++;
    if (hb_idx == 255) {
        hb_idx = 0;
    }
    hb_can_value[3] = hb_idx; //心跳 index 计数
    hb_can_value[4] = 0x22; //表示hal 心跳报文
    soc_pwr_rdy = mPropertyManager->get_soc_power_on_ready();
    if (soc_pwr_rdy) {
        ALOGI("send_heartbeat_msg soc_pwr_rdy:%d", soc_pwr_rdy);
        hb_can_value[5] = 0x01; //表示 电源策略正常运行，可以进行休眠
    }
    if (conf_ck_ret != 0) {
        hb_can_value[6] = 0x55;  //表示 vendor storage set 被修改
    }
    uint8_t command = 0x02;
    uint16_t messageID = 0x666;
    BusIdentifier busIdentifier = EV_CAN;
    uint8_t responseFlag = 0x00;

    std::vector<uint8_t> packet;
    MessageParser mparse;
    mparse.constructDataPacket(packet, hb_can_value, command, responseFlag);
    std::string buf;
    buf = "";
    for (int i = 0; i < (int)packet.size(); i++) {
        std::ostringstream ss;
        ss.clear();
        int data = packet[i];
        ss << std::hex << data;
        buf += "0x";
        buf += ss.str();
        buf += ", ";
    }
    ALOGI("send_heartbeat_msg %zu buffer:%s", packet.size(), buf.c_str());
    uint8_t sendBuf[SPI_PACKAGE_SIZE];
    sendBuf[0] = busIdentifier;
    sendBuf[1] = (messageID >> 8) & 0xff;
    sendBuf[2] = messageID & 0xff;
    sendBuf[3] = packet.size() - 9;
    memcpy(&sendBuf[4],packet.data()+8,packet.size()-9);
    pb_add_raw_vary_data(sendBuf,packet.size()+4-9,0x01);
}

//消息的格式
//msg[0]:0x02
//msg[1]: 0:空消息,非0:不是空消息,需要mcu解析内容,01:二进制,09:json,04:升级包,0x0a:uds
//msg[2]= 不是空消息时有效，0：表示这是一条新消息，1和2表示这是回复消息，1：上次收到的正确，2：上次收到的错误。

void CommConn::transferThread_V1_2() {
	int ret = 0;
	uint8_t recvBuf[SPI_PACKAGE_SIZE];
	uint16_t len = SPI_PACKAGE_SIZE;
	int8_t ackType = -1;
	recvOkNum = 0;
	recvErrorNum = 0;
	recvErrorRetNum = 0;
	
	ALOGI("zwt spi transferThread_V1_2,start\r\n");
    while (true) {
		//usleep(1000*10); //正常通讯，10ms 读写一次数据
        std::this_thread::sleep_for(std::chrono::microseconds(1));

//OTA
		updateFlag = readMCUUpdateFlag();
		//ALOGI("zwt spi ---new msg cycle updateFlag = %s,gecko.mcu.update = %s\r\n",updateFlag.c_str(),android::base::GetProperty("gecko.mcu.update", "0").c_str());
		if(android::base::GetProperty("gecko.mcu.update", "0")=="20240711"){
			ALOGI("zwt spi start MCU OTA\r\n");
			startMcuOta();
		}else if(updateFlag == "1"){
			
			//test
			ALOGI("zwt spi new ota package,otaIndex = %d\r\n",otaIndex);
			if(otaIndex == 0)
			{
				usleep(1000*1000*20); //测试代码，logcat可以抓到所有打印，之后去掉
			}
			//test end

			//选择a、b分区不同的文件路径
			abStatus = readMCUAbStatus();
			if(abStatus == "0")
			{
				g_ABStatus = 0;
				//当前为a分区，需要升级b分区
				gFilePath = filePathB;
			}else{
				g_ABStatus = 1;
				//当前为b分区，需要升级a分区
				gFilePath = filePathA;
			}
				
			//判断是否需要重发消息
			if(otaAckState == 1)
			{
				//ack error，重发上一包
				memcpy(psendBuf,sendOtaLastBuf,SPI_PACKAGE_SIZE);
				needParseAckFlag = 0;
				
				ALOGI("zwt spi resend otaReSendCount = %d,otaIndex = %d\r\n",otaReSendCount,otaIndex);
				if(otaReSendCount<3)
				{
					otaReSendCount++;
				}else{
					//超过三次发送失败，SOC重启
					
					uint8_t repeat = 0;
					do {
						writeMCUUpdateFlag("2");
						android::base::SetProperty("gecko.mcu.update.permission", "666");
						updateFlag = readMCUUpdateFlag();
						if(updateFlag == "2") 
						{
							ALOGI("zwt spi updateFlag = %s\r\n",updateFlag.c_str());
							break;
						}
						repeat ++;
						std::this_thread::sleep_for(std::chrono::milliseconds(100));
					}while(repeat < 10);
	
					reboot();
				}
				
			}else{
				//ack ok，发送下一个包
				//1：发空包，查询上一包的回复。
				//2：上一包的回复正确，组新的下一包发送。
				sendMcuOtaMsg();
			}
		}
//OTA end


		/*mMcuToSocComErr++;
		//ALOGI("zwt spi mMcuToSocComErr %d\r\n",mMcuToSocComErr);
		if(mMcuToSocComErr>6000 && updateFlag == "0"){
			//如果1分钟无通讯则关机
//			ALOGI("zwt spi transferThread_V1_2,start\r\n");
			android::base::SetProperty("sys.powerctl", "shutdown"); 
		}*/
		
		//*****/
		if(psendBuf[1] != 0x00)
		{
			uint16_t lenDataSend = psendBuf[4] << 8 | psendBuf[3];
			uint16_t lenpSendBuf = lenDataSend+6;
			if(lenpSendBuf > 256)
			{
				ALOGE("!!!!!!!!!!!!!!!!!!!!!!!zwt spi lenpSendBuf = %d error!!!!!!!!!\r\n",lenpSendBuf);
				lenpSendBuf = 256;
			}
			uint8_t strSendBuf[32*3+1];
			uint16_t msgPrintIdx = 0;
			uint16_t remainPrintLen = 0;
			//ALOGI("zwt spi sl=%d,data:",lenpSendBuf);
			while(msgPrintIdx < lenpSendBuf)
			{
				remainPrintLen = lenpSendBuf-msgPrintIdx;
				if(remainPrintLen <=32)
				{
					memset(strSendBuf,0,sizeof(strSendBuf));
					for(uint8_t j = 0; j<remainPrintLen;j++)
				    {
				        sprintf((char*)&strSendBuf[j*3],"%02x,",psendBuf[j+msgPrintIdx]);
				    }
			        ALOGI("zwt spi %s\r\n",strSendBuf);
					msgPrintIdx += remainPrintLen;
				}else{
					memset(strSendBuf,0,sizeof(strSendBuf));
					for(uint8_t j = 0; j<32;j++)
					{
						sprintf((char*)&strSendBuf[j*3],"%02x,",psendBuf[j+msgPrintIdx]);
					}
					ALOGI("zwt spi %s\r\n",strSendBuf);
					msgPrintIdx += 32;

				}
			}
		}
		//*****/
	
        ALOGI("%s %d\r\n",__func__,__LINE__);
		//send and recv msg
		memset(recvBuf,0,sizeof(recvBuf));
		ret = mManager.transferData_V1_2(psendBuf,recvBuf,SPI_PACKAGE_SIZE);
		

        ALOGI("%s %d\r\n",__func__,__LINE__);
		//*****/
		if(recvBuf[1] != 0x00)
		{
			uint16_t lenDataRecv = recvBuf[4] << 8 | recvBuf[3];
			uint16_t lenrecvBuf = lenDataRecv+6;

			if(lenrecvBuf > 256 || lenDataRecv == 0xffff)
			{
				ALOGE("!!!!!!!!!!!!!!!!!!!!!!!zwt spi lenrecvBuf = %d,lenDataRecv = %d error!!!!!!!!!\r\n",lenrecvBuf,lenDataRecv);
				lenrecvBuf = 256;
			}

			uint8_t strRevBuf[32*3+1];
			uint16_t msgPrintIdx = 0;
			uint16_t remainPrintLen = 0;
			//ALOGI("zwt spi rl=%d,data:",lenrecvBuf);
			while(msgPrintIdx < lenrecvBuf)
			{
				remainPrintLen = lenrecvBuf-msgPrintIdx;
				if(remainPrintLen <=32)
				{
					memset(strRevBuf,0,sizeof(strRevBuf));
					for(uint8_t j = 0; j<remainPrintLen;j++)
				    {
				        sprintf((char*)&strRevBuf[j*3],"%02x,",recvBuf[j+msgPrintIdx]);
				    }
			        //ALOGI("zwt spi %s\r\n",strRevBuf);
					msgPrintIdx += remainPrintLen;
				}else{
					memset(strRevBuf,0,sizeof(strRevBuf));
					for(uint8_t j = 0; j<32;j++)
					{
						sprintf((char*)&strRevBuf[j*3],"%02x,",recvBuf[j+msgPrintIdx]);
					}
					//ALOGI("zwt spi %s\r\n",strRevBuf);
					msgPrintIdx += 32;

				}
			}
		}
		//*****/
		
			
        ALOGI("%s %d\r\n",__func__,__LINE__);
		if(ret < 0)
		{
			ALOGE("zwt spi transfer error\r\n");
			continue;
		}
        ALOGI("%s %d\r\n",__func__,__LINE__);

		if(updateFlag == "1")
		{
			//OTA通讯流程解析逻辑，读到回复包
			if(needParseAckFlag == 1)
			{
				ret = parseOtaMsg(recvBuf, len);
				if(ret == 0)
				{
					//不需要重发的消息，下一包组新包。
					otaAckState = 0; 
					needMakeMsgFlag = 1;
					
					//适配MCU擦除时间，设置延迟时间
					if(otaIndex == 18 || otaIndex == 17)
					{
						ALOGE("zwt spi get otaIndex 18 ack,sleep 3S \r\n");
						//如果是第18包，凑够4096，等3s
						usleep(1000*1000*3); //等待3s 等待mcu擦除flash
					}
				}else if(ret == 1){
					//需要重发消息
					otaAckState = 1; 
				}else{
					//升级完成，收'05'消息
					otaAckState = 0; 
					needMakeMsgFlag = 0;
				}
			}else{
				//忽略本包读取的数据，下一包发空包，读回复的内容
				needMakeMsgFlag = 0;

				//如果是重发的消息，则清楚标志位，重新收回复消息
				if(otaAckState == 1)
				{
					otaAckState = 0;
				}
			}

			//适配MCU擦除时间，设置延迟时间
			//otaIndex是18的倍数，因为收到17包后，确认回复之后，otaIndex会加1
			if(otaIndex % 18 == 0 ||otaIndex % 17 == 0)
			{
				usleep(1000*100); //凑够整数4096，等待100ms 等待mcu擦除flash
			}else{
				usleep(1000*10);
			}
			
		}else{
			//正常通讯流程解析逻辑
			ackType = parseMsg(recvBuf, len);
			if(ackType == 1) { //收到的消息非空，内容也是对的。
				ALOGI("zwt spi recv right message\r\n");
				uint8_t procesRet = 0;
	            mMcuToSocComErr = 0;
#if USE_VARY_DATA
				procesRet = processVaryMsg(recvBuf);
#else
				procesRet = processMsg(recvBuf);
#endif
				if(procesRet < 2) {
					clearspisendcounter();
					recvOkNum++;
					//ALOGI("zwt spi ack,send new msg\r\n");
#if USE_VARY_DATA
					getQueueVaryMsg(ackType);
#else
					getQueueMsg(ackType);
#endif
				} else if(procesRet == 2) {
					recvErrorRetNum++;
					if(incspisendcounter() > 3) {
						clearspisendcounter();
					} else {
					}
#if USE_VARY_DATA
					getQueueVaryMsg(ackType);
#else
					getQueueMsg(ackType);
#endif

				} else {
				}			
			} else if(ackType == 2) { //收到的消息非空，内容有错误
				ALOGI("zwt spi recv error message\r\n");
				if(adjust_spitrnullmsg()) {
					if(incspisendcounter() > 3) {
							clearspisendcounter();
					} else {
					}
#if USE_VARY_DATA
					getQueueVaryMsg(ackType);
#else
					getQueueMsg(ackType);
#endif
				} else {//send new msg to soc,ignore null msg
					//ALOGI("zwt spi ack,send new message\r\n");
					clearspisendcounter();
#if USE_VARY_DATA
					getQueueVaryMsg(ackType);
#else
					getQueueMsg(ackType);
#endif
				}
				recvErrorNum++;
			} else if(ackType == 0) { //收到空消息，直接发送一条新消息
				recvOkNum++;
				clearspisendcounter();
				ALOGI("zwt spi recv empty message,send new msg\r\n");
#if USE_VARY_DATA
					getQueueVaryMsg(ackType);
#else
					getQueueMsg(ackType);
#endif
			} else {
				//null
			}

		}

    }
}


int is_3th_bit_set(unsigned int num) {
    return (num & (1 << 2)) != 0;
}

static int32_t g_old_camera_sts = 0;
void CommConn::sendCameraStsToMcu()
{
	/*
	 * ME11车 一共8路摄像头
	 *
	 * /dev/mycamhotplugtp9932  操作4路
	 * /dev/mycamhotplugtp2815  操作4路
	 *
	 */
	 
	 /* 编译生成bin push去执行效果
	 rk3568_s:/system/bin # ./mycamhotplugme11test
		后摄像头                在线
		行车记录仪摄像头        在线
		疲劳驾驶摄像头          在线
		货仓监控摄像头          在线
		前环视摄像头            在线
		左环视摄像头            在线
		右环视摄像头            不在线
		后环视摄像头            在线
	 */

    int fd_tp9932 = -1;
    int fd_tp2815 = -1;

    uint8_t value = 0xFF;
    int32_t result = 0xFF;
    fd_tp9932 = open("/dev/mycamhotplugtp9932", O_RDWR);
    if (fd_tp9932 < 0) {
        printf("failed to open device /dev/mycamhotplugtp9932 \n");
        result = result & 0x0F;
    }

    fd_tp2815 = open("/dev/mycamhotplugtp2815", O_RDWR);
    if (fd_tp2815 < 0) {
        printf("failed to open device /dev/mycamhotplugtp2815 \n");
        result = result & 0xF0;
    }
    if (ioctl(fd_tp9932, EXAMPLE_IOCTL_CH0, &value) == 0) {
        if (1 == is_3th_bit_set(value)) {
        } else {
             result = result & 0xEF;
        }
    } else {
        result = result & 0xEF;
    }
    if (ioctl(fd_tp9932, EXAMPLE_IOCTL_CH1, &value) == 0) {
        if (1 == is_3th_bit_set(value)) {
        } else {
            result = result & 0xDF;
        }
    } else {
        result = result & 0xDF;
    }
    if (ioctl(fd_tp9932, EXAMPLE_IOCTL_CH2, &value) == 0) {
        if (1 == is_3th_bit_set(value)) {
        } else {
            result = result & 0xBF;
        }
    } else {
        result = result & 0xBF;
    }
    if (ioctl(fd_tp9932, EXAMPLE_IOCTL_CH3, &value) == 0) {
        if (1 == is_3th_bit_set(value)) {
        } else {
            result = result & 0x7F;
        }
    } else {
        result = result & 0x7F;
    }

    if (ioctl(fd_tp2815, TP2815_IOCTL_CH5, &value) == 0) {
        if (1 == is_3th_bit_set(value)) {
        } else {
            result = result & 0xF7;
        }
    } else {
        result = result & 0xF7;
    }
    if (ioctl(fd_tp2815, TP2815_IOCTL_CH6, &value) == 0) {
        if (1 == is_3th_bit_set(value)) {
        } else {
            result = result & 0xFB;
        }
    } else {
        result = result & 0xFB;
    }
    if (ioctl(fd_tp2815, TP2815_IOCTL_CH7, &value) == 0) {
        if (1 == is_3th_bit_set(value)) {
        } else {
            result = result & 0xFD;
        }
    } else {
        result = result & 0xFD;
    }
    if (ioctl(fd_tp2815, TP2815_IOCTL_CH8, &value) == 0) {
        if (1 == is_3th_bit_set(value)) {
        } else {
            result = result & 0xFE;
        }
    } else {
        result = result & 0xFE;
    }
    close(fd_tp9932);
    close(fd_tp2815);
    //result = std::stoi(android::base::GetProperty("persist.gecko.camera.sts", std::to_string(result)));
    if(g_old_camera_sts != result) {
        g_old_camera_sts = result;
        ALOGI("cyzhao sendCameraStsToMcu res=%d", result);
        std::string key {"CameraSts"};
        std::string value = std::to_string(result);
        mPropertyManager->singleJsonEnQueue(key, value);
    }
}

void CommConn::getSchCHG()
{
    Json::Value root;
     root["SchCHG"] = "?";
     // 输出JSON字符串
     Json::StreamWriterBuilder builder;
     std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
     std::ostringstream oss;
     writer->write(root, &oss);
     // 获取字符串
     std::string str = oss.str();
     // 将字符串转换为 std::vector<uint8_t>
     LOG(INFO) <<"getSchCHG -- str:"<<str;
     std::vector<uint8_t> vec(str.begin(), str.end());
     mPropertyManager->jsonQueueEnqueue(vec);
}

void CommConn::readMCULogThread()
{
    int count = 0;
    bool queryFlag = false;
    while(true){
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        if(count > 50 && !queryFlag)
        {
            queryFlag = true;
            getSchCHG();
            mPropertyManager->updateCPropertyValue("sc_ccm_vehicle_spd_dsp", -1);
        }

        count++;
        if (count > 10000000)
        {
            count = 0;
        }

    }
}

void CommConn::transferThread() {
    std::vector<uint8_t> buffer{};
    std::vector<uint8_t> msg;
    SendCanMap* send_ins = SendCanMap::getInstance();
    //SendQueue* send_ins = SendQueue::getInstance();
    uint32_t spicounter = 1;
    ALOGI("%s %d", __func__,__LINE__);
//    std::unique_lock<std::mutex> lock(mtx);
//    ALOGI("%s %d %d %d", __func__,__LINE__,lock.owns_lock(),SPIready);
    while (!SPIready) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    ALOGI("%s %d", __func__,__LINE__);
    SPIready = false;
//    std::this_thread::sleep_for(std::chrono::seconds(2));
    while (true) {
        buffer.clear();
        #if 1
        //ALOGI("lihy transferThread send_ins->msg_num:%d", send_ins->msg_num);
        msg = send_ins->demap_to_spi();
        //msg = send_ins->dequeue_to_spi();
        #else
        msg = SendQueue::getInstance()->dequeue();
        #endif
        msg.insert(msg.begin(), static_cast<uint8_t>(spicounter & 0xFF));
        msg.insert(msg.begin(), static_cast<uint8_t>((spicounter >> 8) & 0xFF));
        msg.insert(msg.begin(), static_cast<uint8_t>((spicounter >> 16) & 0xFF));
        msg.insert(msg.begin(), static_cast<uint8_t>((spicounter >> 24) & 0xFF));
        spicounter++;

        mManager.transferData(buffer,msg);
        #if 0
        int buf_size = buffer.size();
        if (buf_size <= 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            continue;
        }
        #endif
        resetSomeProp();
        buffer.erase(buffer.begin(), buffer.begin() + 4);
        mrecvqueue.enqueue(buffer.data(), buffer.size());
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

    }
}
}  // namespace impl

}  // namespace V2_0
}  // namespace vehicle
}  // namespace automotive
}  // namespace hardware
}  // namespace android
