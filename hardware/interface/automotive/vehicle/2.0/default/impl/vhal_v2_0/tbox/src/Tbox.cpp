#include "Tbox.h"
#include <android-base/logging.h>
#include <android-base/properties.h>
#define PROPERTY_VALUE_MAX_LEN 250

Tbox::Tbox()  //: mCarMessageQueue(mHidlValuesOutput)
{}
void Tbox::init() {
    //    mpCarMessageQueue = &mCarMessageQueue;
    //    mCarMessageQueue.start();
    //    mVehicleCtl.mCallback->setCarMessageQueue(mpCarMessageQueue);
    /*StatusCode refStatus;
    VehiclePropValue refValue;
    mVehicleCtl.getPropertybyPropId((int)VehicleProperty::INFO_VIN, 0, refValue, refStatus);
    LOG(INFO) << "Tbox::init()";
    if (refValue.value.stringValue.empty()){
        LOG(INFO) << "can not get VIN, using default value";
    } else {
        mMessageData.mUniqueID = refValue.value.stringValue.c_str();
        LOG(INFO) << "TBOX init get VIN : " << mMessageData.mUniqueID;
    }*/
    char propValue[PROPERTY_VALUE_MAX_LEN] = "\0";
    while(true){
        if(property_get("gecko.vin", propValue, nullptr)!=0){
            mMessageData.mUniqueID = propValue;
            LOG(INFO) << "TBOX init get VIN : " << mMessageData.mUniqueID;
            break;
        } else {
            LOG(INFO) << "ERROR !!! can not get VIN";
            std::this_thread::sleep_for(std::chrono::seconds(10));
        }
    }
    memset(propValue, 0, sizeof(propValue));
    if(property_get("persist.tbox.vin", propValue, nullptr)!=0){
        std::string vin = propValue;
        LOG(INFO) << "TBOX init update VIN 1: " << vin;
        if (vin != mMessageData.mUniqueID){
            updateVin();
        } else {
            char propValue[PROPERTY_VALUE_MAX_LEN] = "\0";
            std::string timeVin;
            if(property_get("persist.tbox.getvintime", propValue, nullptr)!=0){
                timeVin = propValue;
                mMessageData.mVehicleInfoMessage.year = static_cast<uint8_t>(std::stoi(timeVin.substr(0, 2)));
                mMessageData.mVehicleInfoMessage.month = static_cast<uint8_t>(std::stoi(timeVin.substr(2, 2)));
                mMessageData.mVehicleInfoMessage.day = static_cast<uint8_t>(std::stoi(timeVin.substr(4, 2)));
                mMessageData.mVehicleInfoMessage.hour = static_cast<uint8_t>(std::stoi(timeVin.substr(6, 2)));
                mMessageData.mVehicleInfoMessage.minute = static_cast<uint8_t>(std::stoi(timeVin.substr(8, 2)));
                mMessageData.mVehicleInfoMessage.second = static_cast<uint8_t>(std::stoi(timeVin.substr(10, 2)));
                LOG(INFO) << "TBOX init time:" << static_cast<int>(mMessageData.mVehicleInfoMessage.year);
                LOG(INFO) << "TBOX init time:" << static_cast<int>(mMessageData.mVehicleInfoMessage.month);
                LOG(INFO) << "TBOX init time:" << static_cast<int>(mMessageData.mVehicleInfoMessage.day);
                LOG(INFO) << "TBOX init time:" << static_cast<int>(mMessageData.mVehicleInfoMessage.hour);
                LOG(INFO) << "TBOX init time:" << static_cast<int>(mMessageData.mVehicleInfoMessage.minute);
                LOG(INFO) << "TBOX init time:" << static_cast<int>(mMessageData.mVehicleInfoMessage.second);
            }
            if(property_get("persist.tbox.getvinlongitude", propValue, nullptr)!=0){
                std::string longitude = propValue;
                mMessageData.mVehicleInfoMessage.longitude = stod(longitude);
            }
            if(property_get("persist.tbox.getvinlatitude", propValue, nullptr)!=0){
                std::string latitude = propValue;
                mMessageData.mVehicleInfoMessage.longitude = stod(latitude);
            }
        }
    } else {
        LOG(INFO) << "TBOX init get no VIN : ";
        updateVin();
    }
    //mVehicleStatusInfo.initVehicleStatusInfoTask();
}
void Tbox::updateVin(){
    LOG(INFO) << "TBOX init updateVin";
    property_set("persist.tbox.vin", mMessageData.mUniqueID.c_str());
    std::time_t now = std::time(nullptr);
    std::tm* timeinfo = std::localtime(&now);
    mMessageData.mVehicleInfoMessage.year = timeinfo->tm_year % 100;
    mMessageData.mVehicleInfoMessage.month = timeinfo->tm_mon + 1;
    mMessageData.mVehicleInfoMessage.day = timeinfo->tm_mday;
    mMessageData.mVehicleInfoMessage.hour = timeinfo->tm_hour;
    mMessageData.mVehicleInfoMessage.minute = timeinfo->tm_min;
    mMessageData.mVehicleInfoMessage.second = timeinfo->tm_sec;
    std::string year = std::to_string(timeinfo->tm_year % 100);  // 年份取后两位
    if(year.size() == 1){
        year = "0" + year;
    }
    std::string month = std::to_string(timeinfo->tm_mon + 1);    // 月份从1开始
    if(month.size() == 1){
        month = "0" + month;
    }
    std::string day = std::to_string(timeinfo->tm_mday);
    if(day.size() == 1){
        day = "0" + day;
    }
    std::string hour = std::to_string(timeinfo->tm_hour);
    if(hour.size() == 1){
        hour = "0" + hour;
    }
    std::string minute = std::to_string(timeinfo->tm_min);
    if(minute.size() == 1){
        minute = "0" + minute;
    }
    std::string second = std::to_string(timeinfo->tm_sec);
    if(second.size() == 1){
        second = "0" + second;
    }
    std::string longitude = "0";
    std::string latitude = "0";
    std::string time = year + month + day + hour + minute + second;
    LOG(INFO) << "TBOX init updateVin" << year << "," << month << "," << day << "," << hour << "," << minute << "," << second;
    char propValue[PROPERTY_VALUE_MAX_LEN] = "\0";
    property_set("persist.tbox.getvintime", time.c_str());
    if(property_get("persist.longitude", propValue, nullptr)!=0){
        longitude = propValue;
        mMessageData.mVehicleInfoMessage.longitude = stod(longitude);
    }
    property_set("persist.tbox.getvinlongitude", longitude.c_str());
    memset(propValue, 0, sizeof(propValue));
    if(property_get("persist.latitude", propValue, nullptr)!=0){
        latitude = propValue;
        mMessageData.mVehicleInfoMessage.latitude = stod(latitude);
    }
    property_set("persist.tbox.getvinlatitude", latitude.c_str());
}
void Tbox::HandleMessageSample(const std::vector<uint8_t>& message) {
    if (mMessageParser.parseMessage(message)) {
        LOG(INFO) << __func__ << __LINE__ << ":" << (int)mMessageParser.command;
        switch (mMessageParser.command) {
            case 1:
                if (mMessageParser.responseFlag == 1) {
                    mStateSample = MachineState::Realtime;
                    //mMessageData.saveLoginSN(true);
                    std::vector<uint8_t> message = mMessageData.generateVehicleInfoMessage();
                    LOG(INFO) << "send vehicle info message ";
                    mNetworkCommSample.SendQueueEnqueue(message);
                    std::vector<uint8_t> RealNameAuthMessage = mMessageData.generateRealNameAuthMessage();
                    mNetworkComm.SendQueueEnqueue(RealNameAuthMessage);
                    network = 1;
                }
                break;
            case 2:
                break;
            case 4:
                if (mMessageParser.responseFlag == 1) {
                    mStateSample = MachineState::Logout;
                    mNetworkCommSample.setNetLose();
                }
                break;
            case 7:
                break;
            default:
                break;
        }
    }
}
void Tbox::HandleMessage(const std::vector<uint8_t>& message) {
    if (mMessageParser.parseMessage(message)) {
        int validate = 0;
        LOG(INFO) << __func__ << __LINE__ << ":" << (int)mMessageParser.command;
        char propValue[PROPERTY_VALUE_MAX_LEN] = "\0";
        std::string testMode;
        switch (mMessageParser.command) {
            case vehicleLogin:
                if (mMessageParser.responseFlag == 1) {
                    mState = MachineState::UpReport;
                    //mMessageData.saveLoginSN(false);
                    std::vector<uint8_t> vehicleInfoMessage = mMessageData.generateVehicleInfoMessage();
                    LOG(INFO) <<  __func__ << "send vehicle info message ";
                    mNetworkComm.SendQueueEnqueue(vehicleInfoMessage);
                    if(network != 1){
                        VehiclePropValue tboxLinkSt;
                        tboxLinkSt.prop = static_cast<int>(VehicleProperty::GECKO_TBOX_LINK_TSP);
                        tboxLinkSt.areaId = 0;
                        tboxLinkSt.value.int32Values.resize(1);
                        tboxLinkSt.value.int32Values[0] = 0x02;
                        mVehicleCtl.setProperty(tboxLinkSt);
                        property_set("persist.tbox.linkst", "2");
                        LOG(INFO) <<  __func__ << "set tboxLinkSt to MCU 2";
                    }
                    network = 1;
                    std::vector<uint8_t> RealNameAuthMessage = mMessageData.generateRealNameAuthMessage();
                    mNetworkComm.state = 0;
                    mNetworkComm.mRetryCount = 0;
                    mNetworkComm.SendQueueEnqueue(RealNameAuthMessage);
                    //GB32960.3 TEST start
                    memset(propValue, 0, sizeof(propValue));
                    if(property_get("persist.gecko.tbox.testmode", propValue, nullptr)!=0){
                        testMode = propValue;
                        if(testMode == "1"){
                            mState = MachineState::Realtime;
                            LOG(INFO) << __func__ <<"GB32960_3_TEST change sm";
                        }
                    }
                    //GB32960.3 TEST end
                    if(mXmlConfig.ChangeNetParm == 1){
                        mXmlConfig.ChangeNetParm = 0;
                        LOG(INFO) <<  __func__ << " login success, reset ChangeNetParm to 0";
                    }
                } else {
                    if(mXmlConfig.ChangeNetParm == 1){
                        revertIPConfig();
                        LOG(INFO) <<  __func__ << " login fail, revert ip and set ChangeNetParm to 0";
                    }
                }
                break;
            case realtimeInformationReporting:
                break;
            case resendInformationReporting:
                break;
            case vehicleLogout:
                if (mMessageParser.responseFlag == 1) {
                    mState = MachineState::Logout;
                    mNetworkComm.setNetLose();
                }
                break;
            case heartbeat: {
                    // 心跳环节
                    //GB32960.3 TEST start
                    memset(propValue, 0, sizeof(propValue));
                    if(property_get("persist.gecko.tbox.testmode", propValue, nullptr)!=0){
                        testMode = propValue;
                        if(testMode == "1"){
                            LOG(INFO) << "GB32960_3_TEST ";
                            memset(propValue, 0, sizeof(propValue));
                            if(property_get("gecko.tbox.logoutcmd", propValue, nullptr)!=0){
                                std::string prop = propValue;
                                int cmd = stod(prop);
                                if(cmd == 1){
                                    std::vector<uint8_t> logoutmsg = mMessageData.generateLogoutMessage();
                                    mNetworkComm.SendQueueEnqueue(logoutmsg);
                                    property_set("gecko.tbox.logoutcmd", "0");
                                    LOG(INFO) << __func__ << __LINE__ << " receive send logout cmd";
                                }
                            }
                        }
                    }
                    //GB32960.3 TEST end
                }
                break;
            case timeSynchronization:{
                setSTRWorkReq(gecko_tbox_work_req);
                if (mMessageParser.responseFlag == 1) {
                    mState = MachineState::Realtime;
                }
                std::time_t now = std::time(nullptr);
                std::tm* timeinfo = std::localtime(&now);
                uint8_t year;
                uint8_t month;
                uint8_t day;
                uint8_t hour;
                uint8_t minute;
                uint8_t second;
                // 设置数据采集时间字段
                year = static_cast<uint8_t>(timeinfo->tm_year % 100);  // 年份取后两位
                month = static_cast<uint8_t>(timeinfo->tm_mon + 1);    // 月份从1开始
                day = static_cast<uint8_t>(timeinfo->tm_mday);
                hour = static_cast<uint8_t>(timeinfo->tm_hour);
                minute = static_cast<uint8_t>(timeinfo->tm_min);
                second = static_cast<uint8_t>(timeinfo->tm_sec);
                std::ostringstream ss;
                ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(year) << " ";
                ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(month) << " ";
                ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(day) << " ";
                ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hour) << " ";
                ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(minute) << " ";
                ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(second) << " ";
                LOG(INFO) << "local time: " << ss.str();
                //这里预留是否需要进行时间补偿，当前只补充逻辑，默认时间和tbox服务器时间一致
                setSTRWorkReq(gecko_tbox_no_work_req);
            } break;
            case 128: {  // 0x80
                setSTRWorkReq(gecko_tbox_work_req);
                mMessageParser.parse80Message(mMessageParser.data);
                mXmlConfig.LoadParmConfig(mMessageData.mParmMsg);
                // 在下面读入配置文件里面的实际值
                std::vector<uint8_t> paramreportmsg = mMessageData.generate80ReportMessage(
                        mMessageParser.mMsg80pack.parmNum, mMessageParser.mMsg80pack.parm);
                mNetworkComm.SendQueueEnqueue(paramreportmsg);
                setSTRWorkReq(gecko_tbox_no_work_req);
            } break;
            case 129: {  // 0x81
                LOG(INFO) << "0x81 msg";
                setSTRWorkReq(gecko_tbox_work_req);
                validate = mMessageParser.validateRemoteControleCMD(mMessageData.mUniqueID, message);
                if(validate != 1){
                    mXmlConfig.LoadParmConfig(mMessageData.mParmWMsg);
                    mMessageParser.parse81Message(mMessageParser.data, mMessageData.mParmWMsg);
                    mXmlConfig.LoadParmConfig(mMessageData.mParmMsg);
                    /*if (mMessageData.mParmWMsg.misSampling == 1) {
                        LOG(INFO) << __func__ << __LINE__;
                        mXmlConfig.ChangeNetParm = 1;
                        //                    mMessageData.mParmWMsg.misSampling = 2;
                    }*/
                    mXmlConfig.UpdateParmConfig(mMessageData.mParmWMsg);
                    mXmlConfig.LoadParmConfig(mMessageData.mParmMsg);
                    std::vector<uint8_t> list;
                    for (auto id : mMessageParser.mMsg81pack.parm) {
                        list.push_back(id.id);
                    }
                    /*std::vector<uint8_t> paramreportmsg = mMessageData.generate81ReportMessage(
                            mMessageParser.mMsg81pack.parmNum, list);*/
                    std::vector<uint8_t> paramreportmsg = mMessageData.generate81ReportMessage(mMessageParser.data);
                    mXmlConfig.ChangeNetParm = 0;
                    LOG(INFO) << __func__ << __LINE__
                              << "samp:" << (int)mMessageData.mParmMsg.misSampling
                              << "samp1:" << (int)mXmlConfig.vehicleConfig.isSampling
                              << "mpublicdomain:" << mMessageData.mParmMsg.mpublicDomain
                              << "publicDomain:" << mXmlConfig.vehicleConfig.publicDomain
                              << "mpublicPort:" << mMessageData.mParmMsg.mpublicPort
                              << "publicPort:" << mXmlConfig.vehicleConfig.publicPort
                              << "mdomainName:" << mMessageData.mParmMsg.mdomainName
                              << "domainName:" << mXmlConfig.vehicleConfig.domainName
                              << "mport:" << mMessageData.mParmMsg.mport
                              << "port:" << mXmlConfig.vehicleConfig.port;
                    //                if (mMessageData.mParmMsg.misSampling !=
                    //                mXmlConfig.vehicleConfig.isSampling) {
                    //                    LOG(INFO) << __func__ << __LINE__;
                    //                    mXmlConfig.ChangeNetParm = 1;
                    //                }
                    if (mMessageData.mParmMsg.misSampling == 1) {
                        //                    if ((mMessageData.mParmMsg.mpublicDomain !=
                        //                         mXmlConfig.vehicleConfig.publicDomain) ||
                        //                        (mMessageData.mParmMsg.mpublicPort !=
                        //                         mXmlConfig.vehicleConfig.publicPort)) {
                        //                        LOG(INFO) << __func__ << __LINE__;
                        //                        mXmlConfig.ChangeNetParm = 1;
                        //                    }
                    }
                    if ((mMessageData.mParmMsg.mdomainName !=
                                mXmlConfig.vehicleConfig.domainName) ||
                               (mMessageData.mParmMsg.mport != mXmlConfig.vehicleConfig.port)) {
                        LOG(INFO) << __func__ << __LINE__;
                        mXmlConfig.ChangeNetParm = 1;
                    }
                    if (mXmlConfig.ChangeNetParm == 1) {
                        mXmlConfig.ChangeNetParmSave.ip = mXmlConfig.vehicleConfig.domainName;
                        mXmlConfig.ChangeNetParmSave.iplen = mXmlConfig.vehicleConfig.domainNameLen;
                        mXmlConfig.ChangeNetParmSave.port = mXmlConfig.vehicleConfig.port;
                        mXmlConfig.ChangeNetParmSave.pip = mXmlConfig.vehicleConfig.publicDomain;
                        mXmlConfig.ChangeNetParmSave.piplen = mXmlConfig.vehicleConfig.publicDomainLen;
                        mXmlConfig.ChangeNetParmSave.pport = mXmlConfig.vehicleConfig.publicPort;
                    }

                    mNetworkComm.SendQueueEnqueue(paramreportmsg);
                    std::vector<uint8_t> logoutmsg = mMessageData.generateLogoutMessage();
                    if (mXmlConfig.ChangeNetParm == 1
                        && (mState == MachineState::Realtime || mState == MachineState::UpReport || mState == MachineState::Login)) {
                        LOG(INFO) << __func__ << __LINE__ << "send logout";
                        mNetworkComm.SendQueueEnqueue(logoutmsg);
                        LOG(INFO) << __func__ << __LINE__;
                        mNetworkComm.setNetLose();
                    }
                    if (mXmlConfig.vehicleConfig.isSampling == 1
                        && (mStateSample == MachineState::Realtime || mStateSample == MachineState::UpReport || mStateSample == MachineState::Login)) {
                        std::vector<uint8_t> logoutmsgsample = mMessageData.generateLogoutMessageSample();
                        mNetworkCommSample.SendQueueEnqueue(logoutmsgsample);
                        LOG(INFO) << __func__ << __LINE__ << "send sampling logout";
                    }
                    mXmlConfig.vehicleConfigInit();
#ifdef EV48
                    mCustomInfoFlags.updateConfig(mXmlConfig.vehicleConfig.isSampling);
#endif
                } else {
                    LOG(INFO) << "129 validate failed";
                }
                setSTRWorkReq(gecko_tbox_no_work_req);
            } break;
            case 130: {  // 0x82
                LOG(INFO) << "0x82 msg";
                setSTRWorkReq(gecko_tbox_work_req);
                validate = mMessageParser.validateRemoteControleCMD(mMessageData.mUniqueID, message);
                if(validate != 1){
                    mMessageParser.parse82Message(mMessageParser.data);
                    // 此处操作先应答后执行，部分操作执行后就无法应答了。
                    std::vector<uint8_t> paramreportmsg = mMessageData.generate82ReportMessage();
                    mNetworkComm.SendQueueEnqueue(paramreportmsg);
                    /*std::vector<uint8_t> logoutmsg = mMessageData.generateLogoutMessage();
                    mNetworkComm.SendQueueEnqueue(logoutmsg);
                    if(mXmlConfig.vehicleConfig.isSampling == 1){
                        std::vector<uint8_t> logoutmsgsample = mMessageData.generateLogoutMessageSample();
                        mNetworkCommSample.SendQueueEnqueue(logoutmsgsample);
                    }*/
                    RemoteCtl(mMessageParser.mMsg82pack);
                    mXmlConfig.vehicleConfigInit();
#ifdef EV48
                    mCustomInfoFlags.updateConfig(mXmlConfig.vehicleConfig.isSampling);
#endif
                } else {
                    LOG(INFO) << "130 validate failed";
                }
                setSTRWorkReq(gecko_tbox_no_work_req);
            } break;
            case eLockVehicle: {  // 0x84 lock vehicle control
                LOG(INFO) << "eLockVehicle 0x84";
                setSTRWorkReq(gecko_tbox_work_req);
                validate = mMessageParser.validateVinForRemoteControleCMD(mMessageData.mUniqueID, message);
                std::vector<uint8_t> responce;
                uint8_t ret = gecko_response_fail;
                if(validate != 0x01){
                    waitingForVehicleAwake();
                    mMessageParser.parseMsgLockVehicle(mMessageParser.data, mMessageParser.mMsgLockVehicle);
                    mLockVehicle.setLockVehicleMsg(mMessageParser.mMsgLockVehicle);
                    LOG(INFO) << "request eLockVehicle is: "<< std::hex << static_cast<int>(mMessageParser.mMsgLockVehicle.speedLimitLevel);
                    ret = mLockVehicle.setSpeedLimitLevel(mMessageParser.mMsgLockVehicle.chargingConditions, mMessageParser.mMsgLockVehicle.speedLimitLevel);
                    //mVehicleStatusMessage.setSpeedLimitLevel(mMessageParser.mMsgLockVehicle.speedLimitLevel);
                    //mCustomInfoFlags.setSpeedLimitLevel(mMessageParser.mMsgLockVehicle.speedLimitLevel);
                    saveSpeedLimitLevelReq(mMessageParser.mMsgLockVehicle.chargingConditions, mMessageParser.mMsgLockVehicle.speedLimitLevel);
                } else {
                    LOG(INFO) << "eLockVehicle validate failed";
                }
                responce.push_back(ret);
                std::vector<uint8_t> paramreportmsg =
                        mMessageData.controlMessageResponse(eLockVehicle, responce);
                mNetworkComm.SendQueueEnqueue(paramreportmsg);
                setSTRWorkReq(gecko_tbox_no_work_req);
            } break;
            case eProgramVersion: {  // 0x85 program version control
                LOG(INFO) << "eProgramVersion 0x85";
                setSTRWorkReq(gecko_tbox_work_req);
                validate = mMessageParser.validateVinForRemoteControleCMD(mMessageData.mUniqueID, message);
                std::vector<uint8_t> responce;
                uint8_t ret = gecko_response_fail;
                if(validate == 1){
                    LOG(INFO) << "eProgramVersion validate failed";
                } else {
                    waitingForVehicleAwake();
                    mMessageParser.parseMsgVersionSwitch(mMessageParser.data, mMessageParser.mMsgVersionSwitch);
                    int requestVersion = 0;
                    if (0x55 == mMessageParser.mMsgVersionSwitch.requestVersion) {
                        //锁车版本
                        requestVersion = 1;
                    } else {
                        //非锁车版本
                        requestVersion = 0;
                    }
                    mRemoteVersionSwitch.setVersionSwitchMsg(mMessageParser.mMsgVersionSwitch);
                    LOG(INFO) << "request programVersion is: "<< std::hex <<static_cast<int>(mMessageParser.mMsgVersionSwitch.requestVersion);
                    mRemoteVersionSwitch.setVersion(requestVersion);
                    //mRemoteVersionSwitch.getVCURespose();
                    ret = static_cast<int>(mRemoteVersionSwitch.checkVCUVersion());
                    LOG(INFO) << "check version switch result and result is " << static_cast<int>(ret);
                }
                responce.push_back(ret);
                std::vector<uint8_t> paramreportmsg =
                        mMessageData.controlMessageResponse(eProgramVersion, responce);
                mNetworkComm.SendQueueEnqueue(paramreportmsg);
                setSTRWorkReq(gecko_tbox_no_work_req);
            } break;
            case eMaintenance: {  // 0x88 maintenance
                LOG(INFO) << "eMaintenance 0x88";
                setSTRWorkReq(gecko_tbox_work_req);
                validate = mMessageParser.validateVinForRemoteControleCMD(mMessageData.mUniqueID, message);
                std::vector<uint8_t> responce;
                uint8_t ret = gecko_response_fail;
                if(validate == 1){
                    LOG(INFO) << "eMaintenance validate failed";
                } else {
                    waitingForVehicleAwake();
                    uint8_t maintenance = static_cast<uint8_t>(message[24]);
                    mMessageParser.serviceControlDataHandle(eMaintenance, maintenance);
                    ret = gecko_response_ok;
                }
                responce.push_back(ret);
                std::vector<uint8_t> paramreportmsg =
                        mMessageData.controlMessageResponse(eMaintenance, responce);
                mNetworkComm.SendQueueEnqueue(paramreportmsg);
                setSTRWorkReq(gecko_tbox_no_work_req);
            } break;
            case ePayMent: {  // 0x89 payment
                LOG(INFO) << "ePayMent 0x89";
                setSTRWorkReq(gecko_tbox_work_req);
                validate = mMessageParser.validateVinForRemoteControleCMD(mMessageData.mUniqueID, message);
                std::vector<uint8_t> responce;
                uint8_t ret = gecko_response_fail;
                if(validate == 1){
                    LOG(INFO) << "ePayMent validate failed";
                } else {
                    waitingForVehicleAwake();
                    uint8_t payment = static_cast<uint8_t>(message[24]);
                    mMessageParser.serviceControlDataHandle(ePayMent, payment);
                    ret = gecko_response_ok;
                }
                responce.push_back(ret);
                std::vector<uint8_t> paramreportmsg =
                        mMessageData.controlMessageResponse(ePayMent, responce);
                mNetworkComm.SendQueueEnqueue(paramreportmsg);
                setSTRWorkReq(gecko_tbox_no_work_req);
            } break;
            case eAnnualVerif: {  // 0x8a annualVerif
                LOG(INFO) << "eAnnualVerif 0x8A";
                setSTRWorkReq(gecko_tbox_work_req);
                validate = mMessageParser.validateVinForRemoteControleCMD(mMessageData.mUniqueID, message);
                std::vector<uint8_t> responce;
                uint8_t ret = gecko_response_fail;
                if(validate == 1){
                    LOG(INFO) << "eAnnualVerif validate failed";
                } else {
                    waitingForVehicleAwake();
                    uint8_t annualVerif = static_cast<uint8_t>(message[24]);
                    mMessageParser.serviceControlDataHandle(eAnnualVerif, annualVerif);
                    ret = gecko_response_ok;
                }
                responce.push_back(ret);
                std::vector<uint8_t> paramreportmsg =
                        mMessageData.controlMessageResponse(eAnnualVerif, responce);
                mNetworkComm.SendQueueEnqueue(paramreportmsg);
                setSTRWorkReq(gecko_tbox_no_work_req);
            } break;
            case eBatteryMaintenance: {  // 0x8b batteryMaintenance
                LOG(INFO) << "eBatteryMaintenance 0x8B";
                setSTRWorkReq(gecko_tbox_work_req);
                validate = mMessageParser.validateVinForRemoteControleCMD(mMessageData.mUniqueID, message);
                std::vector<uint8_t> responce;
                uint8_t ret = gecko_response_fail;
                if(validate == 1){
                    LOG(INFO) << "eBatteryMaintenance validate failed";
                } else {
                    waitingForVehicleAwake();
                    uint8_t batteryMaintenance = static_cast<uint8_t>(message[24]);
                    mMessageParser.serviceControlDataHandle(eBatteryMaintenance, batteryMaintenance);
                    ret = gecko_response_ok;
                }
                responce.push_back(ret);
                std::vector<uint8_t> paramreportmsg =
                        mMessageData.controlMessageResponse(eBatteryMaintenance, responce);
                mNetworkComm.SendQueueEnqueue(paramreportmsg);
                setSTRWorkReq(gecko_tbox_no_work_req);
            } break;
            case eQueryReCharge: {  // 0xa0 query reservation charging 预约充电配置查询
                LOG(INFO) << "eQueryReCharge 0xA0";
                setSTRWorkReq(gecko_tbox_work_req);
                validate = mMessageParser.validateRemoteControleCMD(mMessageData.mUniqueID, message);
                if(validate == 1){
                    LOG(INFO) << "eQueryReCharge validate failed";
                    break;
                }
                waitingForVehicleAwake();
                LOG(INFO) << "eQueryReCharge";
                //std::vector<uint8_t> responce = mMessageParser.queryReChargeHandle();
                std::vector<uint8_t> responce = mChargingCtl.getReservedChargingConfig();
                std::vector<uint8_t> paramreportmsg =
                        mMessageData.controlMessageResponse(eQueryReCharge, responce);
                mNetworkComm.SendQueueEnqueue(paramreportmsg);
                setSTRWorkReq(gecko_tbox_no_work_req);
            } break;
            case eQueryPowerRepl: {  // 0xA2 query Intelligent power replenishment 智能补电配置查询
                LOG(INFO) << "eQueryPowerRepl 0xa2";
                setSTRWorkReq(gecko_tbox_work_req);
                validate = mMessageParser.validateRemoteControleCMD(mMessageData.mUniqueID, message);
                if(validate == 1){
                    LOG(INFO) << "eQueryPowerRepl validate failed";
                    break;
                }
                waitingForVehicleAwake();
                LOG(INFO) << "eQueryPowerRepl";
                //std::vector<uint8_t> responce = mMessageParser.queryPowerRepHandle();
                std::vector<uint8_t> responce = mChargingCtl.getSmartDCConfig();
                std::vector<uint8_t> paramreportmsg =
                        mMessageData.controlMessageResponse(eQueryPowerRepl, responce);
                mNetworkComm.SendQueueEnqueue(paramreportmsg);
                setSTRWorkReq(gecko_tbox_no_work_req);
            } break;
            case eReCharge: {  // 0xa3  reservation charging 预约充电
                LOG(INFO) << "eReCharge 0xA3";
                setSTRWorkReq(gecko_tbox_work_req);
                validate = mMessageParser.validateVinForRemoteControleCMD(mMessageData.mUniqueID, message);
                std::vector<uint8_t> reCharge = std::vector<uint8_t>(message.begin() + 30, message.end() - 1);
                if(validate == 1){
                    LOG(INFO) << "eReCharge validate failed";
                } else {
                    waitingForVehicleAwake();
                    LOG(INFO) << "eReCharge start";
                    mChargingCtl.setReservedChargingConfig(reCharge);
                    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                }
                std::vector<uint8_t> responce = mChargingCtl.getReservedChargingConfig();
                uint8_t ret = mChargingCtl.checkReservedChargingConfigSetResult(reCharge);
                responce.push_back(ret);
                std::vector<uint8_t> paramreportmsg =
                        mMessageData.controlMessageResponse(eReCharge, responce);
                mNetworkComm.SendQueueEnqueue(paramreportmsg);
                setSTRWorkReq(gecko_tbox_no_work_req);
            } break;
            case ePowerReplenishment: {  // 0xa5  Intelligent power replenishment 智能补电配置
                LOG(INFO) << "ePowerReplenishment 0xA5";
                setSTRWorkReq(gecko_tbox_work_req);
                validate = mMessageParser.validateRemoteControleCMD(mMessageData.mUniqueID, message);
                if(validate == 1){
                    LOG(INFO) << "ePowerReplenishment validate failed";
                    break;
                }
                waitingForVehicleAwake();
                LOG(INFO) << "ePowerReplenishment";
                std::vector<uint8_t> powerRep =
                        std::vector<uint8_t>(message.begin() + 30, message.begin() + 34);
                //std::vector<uint8_t> responce = mMessageParser.powerRepHandle(powerRep);
                std::vector<uint8_t> responce = mChargingCtl.setSmartDCConfig(powerRep);
                std::vector<uint8_t> paramreportmsg =
                        mMessageData.controlMessageResponse(ePowerReplenishment, responce);
                mNetworkComm.SendQueueEnqueue(paramreportmsg);
                setSTRWorkReq(gecko_tbox_no_work_req);
            } break;
            case eVehicleDoorLock: {  // 0xa6 door lock control
                LOG(INFO) << "eVehicleDoorLock 0xA6";
                setSTRWorkReq(gecko_tbox_work_req);
                uint8_t resp = gecko_response_fail;
                validate = mMessageParser.validateRemoteControleCMD(mMessageData.mUniqueID, message);
                if(validate != 1){
                    waitingForVehicleAwake();
                    uint8_t powerSt = getBCMPowerSt();
                    LOG(INFO) << "eVehicleDoorLock: validate = " << validate << ", powerSt = " << static_cast<int>(powerSt);
                    uint8_t doorLock = static_cast<uint8_t>(message[30]);
                    if(powerSt == 0x00 || powerSt == 0x02){
                        mMessageParser.doorLockHandle(doorLock);
                        StatusCode refStatus;
                        VehiclePropValue refValue;
                        for(auto i = 0; i<200; i++){
                            std::this_thread::sleep_for(std::chrono::milliseconds(10));
                            mVehicleCtl.getPropertybyPropId((int)VehicleProperty::GECKO_CCM_REMOTE_LOCK, 0, refValue, refStatus);
                            LOG(INFO) << "eVehicleDoorLock: got response from CCU_RemoteLockFb : " <<refValue.value.int32Values[0];
                            if(refValue.value.int32Values[0] == 0x01){
                                resp = gecko_response_ok;
                                LOG(INFO) << "eVehicleDoorLock: got response from CCU_RemoteLockFb resp=" << static_cast<int>(resp);
                                break;
                            }
                        }
                        LOG(INFO) << "eVehicleDoorLock: response : " << static_cast<int>(resp);
                    } else {
                        LOG(INFO) << "eVehicleDoorLock failed !";
                    }
                }
                std::vector<uint8_t> responce;
                responce.push_back(resp);
                std::vector<uint8_t> paramreportmsg =
                mMessageData.controlMessageResponse(eVehicleDoorLock, responce);
                mNetworkComm.SendQueueEnqueue(paramreportmsg);
                LOG(INFO) << "eVehicleDoorLock: response send:" << static_cast<int>(resp);
                setSTRWorkReq(gecko_tbox_no_work_req);
            } break;
            case eVehicleSearch: {  // 0xa7 vehicle search
                LOG(INFO) << "eVehicleSearch 0xA7";
                setSTRWorkReq(gecko_tbox_work_req);
                uint8_t resp = gecko_response_fail;
                validate = mMessageParser.validateRemoteControleCMD(mMessageData.mUniqueID, message);
                waitingForVehicleAwake();
                uint8_t powerSt = getBCMPowerSt();
                uint8_t vehicleSearch = static_cast<uint8_t>(message[30]);
                LOG(INFO) << "vehicleSearch cmd is: " << static_cast<int>(vehicleSearch) << ",validate=" << validate << ", powerSt=" << static_cast<int>(powerSt);
                if(validate != 0x01 && (powerSt == 0x00 || powerSt == 0x02)){
                    VehiclePropValue remoteCarSearchProp;
                    remoteCarSearchProp.prop = static_cast<int>(VehicleProperty::GECKO_CCM_REMOTE_CAR_SEARCH);
                    remoteCarSearchProp.areaId = 0;
                    remoteCarSearchProp.value.int32Values.resize(1);
                    if(vehicleSearch == 0x03){
                        remoteCarSearchProp.value.int32Values[0] = 0x02;
                    } else {
                        remoteCarSearchProp.value.int32Values[0] = 0x01;
                    }
                    LOG(INFO) << "vehicleSearch: set GECKO_CCM_REMOTE_CAR_SEARCH : " << static_cast<int>(remoteCarSearchProp.value.int32Values[0]);
                    mVehicleCtl.setProperty(remoteCarSearchProp);
                    StatusCode refStatus;
                    VehiclePropValue refValue;
                    //read fb state for 1.5s
                    for (auto i = 0; i < 200; i++) {
                        std::this_thread::sleep_for(std::chrono::milliseconds(10));
                        mVehicleCtl.getPropertybyPropId((int)VehicleProperty::GECKO_CCM_REMOTE_CAR_SEARCH, 0, refValue, refStatus);
                        if(refValue.value.int32Values[0] == 0x01){
                            resp = gecko_response_ok;
                            LOG(INFO) << "vehicleSearch : got CCU_RemoteCarSearchFb" << static_cast<int>(resp);
                            break;
                        }
                    }
                    LOG(INFO) << "vehicleSearch: response = " << static_cast<int>(resp);
                } else {
                    LOG(INFO) << "vehicleSearch :failed!";
                }
                std::vector<uint8_t> responce;
                responce.push_back(resp);
                std::vector<uint8_t> paramreportmsg =
                        mMessageData.controlMessageResponse(eVehicleSearch, responce);
                mNetworkComm.SendQueueEnqueue(paramreportmsg);
                LOG(INFO) << "vehicleSearch : end and resp = " << static_cast<int>(resp);
                setSTRWorkReq(gecko_tbox_no_work_req);
            } break;
            case eHVACControl: {  // 0xa8 hvac
                LOG(INFO) << "eHVACControl 0xA8";
                setSTRWorkReq(gecko_tbox_work_req);
                validate = mMessageParser.validateRemoteControleCMD(mMessageData.mUniqueID, message);
                std::vector<uint8_t> responce;
                int resp = gecko_response_fail;
                int ret = 0;
                std::vector<uint8_t> paramreportmsg;
                if(validate == 1){
                    LOG(INFO) << "eHVACControl validate failed";
                } else {
                    /*validate = validateBCMPowerSt();
                    if(validate == 1){
                        LOG(INFO) << "eHVACControl validate BCM Power St failed";
                        break;
                    }*/
                    waitingForVehicleAwake();
                    mMessageParser.parseMsgHavcCtl(mMessageParser.data, mMessageParser.mMsgHavcCtl);
                    mRemoteHaveCtl.setRemoteHavcCtl(mMessageParser.mMsgHavcCtl);

                    StatusCode refStatus;
                    VehiclePropValue refValue;
                    if(mRemoteHaveCtl.mHavcCtl.air_conditioning_switch == 1){
                        int remoteHavcThreadsSize = mRemoteHaveCtl.mThreads.size();
                        LOG(INFO) << "eHVACControl : remoteHavcThreadsSize = " << remoteHavcThreadsSize;
                        ret = mRemoteHaveCtl.RemoteHavcOpen();
                        LOG(INFO) << "eHVACControl:OPEN, and ret = " << ret;
                        if(ret == 1){
                            resp = gecko_response_fail;
                        } else if(ret == 2){
                            resp = gecko_response_ok;
                        } else {
                            mRemoteHaveCtl.getHaveWorkState();
                            //check AC status
                            if(mRemoteHaveCtl.mHavcCtl.air_conditioning_switch == 0x01 && mRemoteHaveCtl.mHavcCtl.temperature_adjustment != 0xff && mRemoteHaveCtl.mHavcCtl.defrosting_switch == 0xff){
                                LOG(INFO) << "eHVACControl : got AC_TempSetFb";
                                for (auto i = 0; i < 200; i++) {
                                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                                    mVehicleCtl.getPropertybyPropId((int)VehicleProperty::GECKO_AC_TEMPSETFB, 0, refValue, refStatus);
                                    if(refValue.value.int32Values[0] == 0x01){
                                        resp = gecko_response_ok;
                                        LOG(INFO) << "eHVACControl : got AC_TempSetFb success";
                                        break;
                                    }
                                }
                            }
                            if(mRemoteHaveCtl.mHavcCtl.defrosting_switch != 0xff){
                                LOG(INFO) << "eHVACControl : got AC_RemoteDefrostCtlFb";
                                for (auto i = 0; i < 200; i++) {
                                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                                    mVehicleCtl.getPropertybyPropId((int)VehicleProperty::GECKO_AC_REMOTEDFROSTCTLFB, 0, refValue, refStatus);
                                    if(refValue.value.int32Values[0] == 0x01){
                                        resp = gecko_response_ok;
                                        LOG(INFO) << "eHVACControl : got AC_RemoteDefrostCtlFb success";
                                        break;
                                    }
                                }
                            }
                            if(mRemoteHaveCtl.mHavcCtl.defrosting_switch == 0xff && mRemoteHaveCtl.mHavcCtl.temperature_adjustment == 0xff){
                                LOG(INFO) << "eHVACControl : got AC_RemtCtrlFb";
                                for (auto i = 0; i < 200; i++) {
                                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                                    mVehicleCtl.getPropertybyPropId((int)VehicleProperty::GECKO_AC_REMTCNTLFB, 0, refValue, refStatus);
                                    if(refValue.value.int32Values[0] == 0x01){
                                        resp = gecko_response_ok;
                                        LOG(INFO) << "eHVACControl : got AC_RemtCtrlFb success";
                                        break;
                                    }
                                }
                            }
                        }
                        if(resp == gecko_response_fail && remoteHavcThreadsSize == 0){
                            mRemoteHaveCtl.RemoteHavcClose();
                            LOG(INFO) << "eHVACControl: open failed, close the AC anyway";
                        }
                        LOG(INFO) << "eHVACControl:OPEN, and resp = " << resp;
                    } else {
                        ret = mRemoteHaveCtl.RemoteHavcClose();
                        LOG(INFO) << "eHVACControl:close, and ret = " << ret;
                        if(ret == 1){
                            resp = gecko_response_ok;
                        } else {
                            if(mRemoteHaveCtl.mHavcCtl.air_conditioning_switch == 0x00 && mRemoteHaveCtl.mHavcCtl.defrosting_switch == 0xFF){
                                LOG(INFO) << "eHVACControl : close AC";
                                for (auto i = 0; i < 200; i++) {
                                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                                    mVehicleCtl.getPropertybyPropId((int)VehicleProperty::GECKO_AC_REMTCNTLFB, 0, refValue, refStatus);
                                    if(refValue.value.int32Values[0] == 0x01){
                                        resp = gecko_response_ok;
                                        LOG(INFO) << "eHVACControl : close got AC_RemtCtrlFb success";
                                        break;
                                    }
                                }
                            }
                            if(mRemoteHaveCtl.mHavcCtl.air_conditioning_switch == 0x00 && mRemoteHaveCtl.mHavcCtl.defrosting_switch == 0x00){
                                LOG(INFO) << "eHVACControl : close defrost";
                                for (auto i = 0; i < 200; i++) {
                                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                                    mVehicleCtl.getPropertybyPropId((int)VehicleProperty::GECKO_AC_REMOTEDFROSTCTLFB, 0, refValue, refStatus);
                                    if(refValue.value.int32Values[0] == 0x01){
                                        resp = gecko_response_ok;
                                        LOG(INFO) << "eHVACControl : close got AC_RemtCtrlFb success";
                                        break;
                                    }
                                }
                            }
                            LOG(INFO) << "eHVACControl:CLOSE" << "resp=" << resp;
                        }
                    }
                }
                LOG(INFO) << "eHVACControl: " << "resp=" << resp;
                responce.push_back(resp);
                paramreportmsg =
                    mMessageData.controlMessageResponse(eHVACControl, responce);
                mNetworkComm.SendQueueEnqueue(paramreportmsg);
                if(ret == 1 || ret == 2){
                    setSTRWorkReq(gecko_tbox_no_work_req);
                    LOG(INFO) << "eHVACControl: release the keep awake flag immediately and ret = " << ret;
                }
            } break;
            case eRemoteDiagAuth: {  // 0xa9 Remote diagnosis authentication
                LOG(INFO) << "eRemoteDiagAuth 0xA9";
                setSTRWorkReq(gecko_tbox_work_req);
                validate = mMessageParser.validateRemoteControleCMD(mMessageData.mUniqueID, message);
                if(validate == 1){
                    LOG(INFO) << "eRemoteDiagAuth validate failed";
                    break;
                }
                waitingForVehicleAwake();
                std::vector<uint8_t> remoteDiagAuth =
                        std::vector<uint8_t>(message.begin() + 30, message.end() - 1);
                std::vector<uint8_t> responce;
                responce = mMessageParser.diagAuthHandle(remoteDiagAuth);
                std::vector<uint8_t> paramreportmsg =
                        mMessageData.controlMessageResponse(eRemoteDiagAuth, responce);
                mNetworkComm.SendQueueEnqueue(paramreportmsg);
                setSTRWorkReq(gecko_tbox_no_work_req);
            } break;
            case eRemoteDiagTrans: {  // 0xaa Remote diagnostic transmission
                LOG(INFO) << "eRemoteDiagTrans 0xAA";
                setSTRWorkReq(gecko_tbox_work_req);
                validate = mMessageParser.validateRemoteControleCMD(mMessageData.mUniqueID, message);
                if(validate == 1){
                    LOG(INFO) << "eRemoteDiagTrans validate failed";
                    break;
                }
                waitingForVehicleAwake();
                std::vector<uint8_t> remoteDiagTrans =
                        std::vector<uint8_t>(message.begin() + 30, message.end() - 1);
                std::vector<uint8_t> responce;
                responce = mMessageParser.diagTransHandle(remoteDiagTrans);
                std::vector<uint8_t> paramreportmsg =
                        mMessageData.controlMessageResponse(eRemoteDiagTrans, responce);
                mNetworkComm.SendQueueEnqueue(paramreportmsg);
                setSTRWorkReq(gecko_tbox_no_work_req);
            } break;
            case eRealNameAuthState: {
                mRealNameAuthState = static_cast<int>(message[30]);
                std::string authState = std::to_string(mRealNameAuthState);
                property_set("persist.tbox.authstate", authState.c_str());
                LOG(INFO) << "eRealNameAuthState = " << mRealNameAuthState;
            } break;
            case eRemtMaiDrSeatHeat: {
                LOG(INFO) << "eRemtMaiDrSeatHeat RemoteSeatHeatCtl 0xB0";
                setSTRWorkReq(gecko_tbox_work_req);
                validate = mMessageParser.validateRemoteControleCMD(mMessageData.mUniqueID, message);
                uint8_t resp = gecko_response_fail;
                if(validate != 0x01){
                    waitingForVehicleAwake();
                    mMessageParser.parseMsgSeatHeatCtl(mMessageParser.data, mMessageParser.mMsgSeatHeatCtl);
                    mRemoteSeatHeatCtl.setRemoteSeatHeatCtl(mMessageParser.mMsgSeatHeatCtl);
                    mRemoteSeatHeatCtl.startRemoteSeatHeatCtlCMD();
                    mRemoteSeatHeatCtl.waitingForState();
                    StatusCode refStatus;
                    VehiclePropValue refValue;
                    for (auto i = 0; i < 200; i++) {
                        std::this_thread::sleep_for(std::chrono::milliseconds(10));
                        mVehicleCtl.getPropertybyPropId((int)VehicleProperty::GECKO_CCU_REMTMAIDRSEATHEATFB, 0, refValue, refStatus);
                        if(refValue.value.int32Values[0] == 0x01){
                            resp = gecko_response_ok;
                            LOG(INFO) << "eRemtMaiDrSeatHeat : RemoteSeatHeatCtl got CCU_RemtMaiDrSeatHeatFb success";
                            break;
                        }
                    }
                }
                std::vector<uint8_t> responce;
                responce.push_back(resp);
                std::vector<uint8_t> paramreportmsg =
                        mMessageData.controlMessageResponse(eRemtMaiDrSeatHeat, responce);
                mNetworkComm.SendQueueEnqueue(paramreportmsg);
                LOG(INFO) << "eRemtMaiDrSeatHeat : RemoteSeatHeatCtl end and resp = " <<  static_cast<int>(resp);
                setSTRWorkReq(gecko_tbox_no_work_req);
            } break;
            case eRemtSteerWhlHeat: {
                LOG(INFO) << "eRemtSteerWhlHeat 0xB1";
                setSTRWorkReq(gecko_tbox_work_req);
                validate = mMessageParser.validateRemoteControleCMD(mMessageData.mUniqueID, message);
                uint8_t resp = gecko_response_fail;
                if(validate != 0x01){
                    waitingForVehicleAwake();
                    mRemoteSteerWheelHeatCtl.setRemoteSteerWheelHeatCtl(static_cast<int>(message[30]));
                    mRemoteSteerWheelHeatCtl.startRemoteSteerWheelHeatCtlCMD();
                    mRemoteSteerWheelHeatCtl.waitingForState();
                    StatusCode refStatus;
                    VehiclePropValue refValue;
                    for (auto i = 0; i < 200; i++) {
                        std::this_thread::sleep_for(std::chrono::milliseconds(10));
                        mVehicleCtl.getPropertybyPropId((int)VehicleProperty::GECKO_CCU_REMTSTEERWHLHEATFB, 0, refValue, refStatus);
                        if(refValue.value.int32Values[0] == 0x01){
                            resp = gecko_response_ok;
                            LOG(INFO) << "eRemtSteerWhlHeat : RemoteSteerWheelHeatCtl got CCU_RemtSteerWhlHeatFb success";
                            break;
                        }
                    }
                }
                std::vector<uint8_t> responce;
                responce.push_back(resp);
                std::vector<uint8_t> paramreportmsg =
                        mMessageData.controlMessageResponse(eRemtSteerWhlHeat, responce);
                mNetworkComm.SendQueueEnqueue(paramreportmsg);
                LOG(INFO) << "eRemtSteerWhlHeat : RemoteSteerWheelHeatCtl end and resp = " << static_cast<int>(resp);
                setSTRWorkReq(gecko_tbox_no_work_req);
            } break;
            case eRemotePowerLock: {
                LOG(INFO) << "eRemotePowerLock 0xB2";
                setSTRWorkReq(gecko_tbox_work_req);
                validate = mMessageParser.validateRemoteControleCMD(mMessageData.mUniqueID, message);
                uint8_t resp = gecko_response_fail;
                if(validate != 0x01){
                    waitingForVehicleAwake();
                    VehiclePropValue powerLock;
                    powerLock.prop = static_cast<int>(VehicleProperty::GECKO_CCU_REMOTEPOWERLOCKFB);
                    powerLock.areaId = 0;
                    powerLock.value.int32Values.resize(1);
                    if(message[30] == 0x01){
                        powerLock.value.int32Values[0] = 0x01;
                    } else {
                        powerLock.value.int32Values[0] = 0x02;
                    }
                    mVehicleCtl.setProperty(powerLock);
                    LOG(INFO) << "eRemotePowerLock : set power lock = " << powerLock.value.int32Values[0];
                    StatusCode refStatus;
                    VehiclePropValue refValue;
                    //CCU_RemotePowerLockFb信号周期为20ms，当前版本会发送5帧，后续修改为发送10帧，为能读取到该信号，缩短对该属性的读取间隔为20ms
                    for (auto i = 0; i < 75; i++) {
                        std::this_thread::sleep_for(std::chrono::milliseconds(20));
                        mVehicleCtl.getPropertybyPropId((int)VehicleProperty::GECKO_CCU_REMOTEPOWERLOCKFB, 0, refValue, refStatus);
                        if(refValue.value.int32Values[0] == 0x01){
                            resp = gecko_response_ok;
                            LOG(INFO) << "eRemotePowerLock : got CCU_RemotePowerLock_Req" << static_cast<int>(resp);
                            break;
                        }
                    }
                }
                std::vector<uint8_t> responce;
                responce.push_back(resp);
                std::vector<uint8_t> paramreportmsg =
                        mMessageData.controlMessageResponse(eRemotePowerLock, responce);
                mNetworkComm.SendQueueEnqueue(paramreportmsg);
                LOG(INFO) << "eRemotePowerLock : end and resp = " << static_cast<int>(resp);
                setSTRWorkReq(gecko_tbox_no_work_req);
            } break;
            default:
                LOG(INFO) << "Cannot handle msg. TO DEFAULT";
                break;
        }
    }
}
void Tbox::RemoteCtl(msg82pack& pack) {
    std::vector<uint8_t> logoutmsg;
    std::vector<uint8_t> logoutmsgsample;
    switch (pack.commandId) {
        case REMOTE_UPGRADE:
            logoutmsg = mMessageData.generateLogoutMessage();
            mNetworkComm.SendQueueEnqueue(logoutmsg);
            if(mXmlConfig.vehicleConfig.isSampling == 1){
                logoutmsgsample = mMessageData.generateLogoutMessageSample();
                mNetworkCommSample.SendQueueEnqueue(logoutmsgsample);
            }
            break;
        case SHUTDOWN:
            logoutmsg = mMessageData.generateLogoutMessage();
            mNetworkComm.SendQueueEnqueue(logoutmsg);
            if(mXmlConfig.vehicleConfig.isSampling == 1){
                logoutmsgsample = mMessageData.generateLogoutMessageSample();
                mNetworkCommSample.SendQueueEnqueue(logoutmsgsample);
            }
            //shutdown();
            break;
        case RESET:
            logoutmsg = mMessageData.generateLogoutMessage();
            mNetworkComm.SendQueueEnqueue(logoutmsg);
            if(mXmlConfig.vehicleConfig.isSampling == 1){
                logoutmsgsample = mMessageData.generateLogoutMessageSample();
                mNetworkCommSample.SendQueueEnqueue(logoutmsgsample);
            }
            //reboot();
            break;
        case FACTORY_RESET:
            logoutmsg = mMessageData.generateLogoutMessage();
            mNetworkComm.SendQueueEnqueue(logoutmsg);
            if(mXmlConfig.vehicleConfig.isSampling == 1){
                logoutmsgsample = mMessageData.generateLogoutMessageSample();
                mNetworkCommSample.SendQueueEnqueue(logoutmsgsample);
            }
            ResetConfigfile();
            break;
        case DISCONNECT:
            logoutmsg = mMessageData.generateLogoutMessage();
            mNetworkComm.SendQueueEnqueue(logoutmsg);
            if(mXmlConfig.vehicleConfig.isSampling == 1){
                logoutmsgsample = mMessageData.generateLogoutMessageSample();
                mNetworkCommSample.SendQueueEnqueue(logoutmsgsample);
            }
            break;
        case ALARM:
            break;
        case START_SAMPLING:
            mXmlConfig.LoadParmConfig(mMessageData.mParmWMsg);
            mMessageData.mParmWMsg.misSampling = 1;
            mXmlConfig.UpdateParmConfig(mMessageData.mParmWMsg);
            mXmlConfig.vehicleConfigInit();
#ifdef EV48
            mCustomInfoFlags.updateConfig(mXmlConfig.vehicleConfig.isSampling);
#endif
            break;
        case OFFLINE_MODE:
            logoutmsg = mMessageData.generateLogoutMessage();
            mNetworkComm.SendQueueEnqueue(logoutmsg);
            if(mXmlConfig.vehicleConfig.isSampling == 1){
                logoutmsgsample = mMessageData.generateLogoutMessageSample();
                mNetworkCommSample.SendQueueEnqueue(logoutmsgsample);
            }
            break;
        case REMOTE_CAN_COLLECTION:
            break;
        default:
            break;
    }
}

void Tbox::shutdown() {
    LOG(INFO) << "shutdown successfully.";
    //android::base::SetProperty("sys.powerctl", "shutdown");  // or "reboot" for a reboot
}
void Tbox::reboot() {
    LOG(INFO) << "reboot successfully.";
    //android::base::SetProperty("sys.powerctl", "reboot");  // or "reboot" for a reboot
}
int Tbox::ResetConfigfile() {
    std::string sourceFileName = "/vendor/etc/gecko-tbox-config.xml";        // 源文件名
    std::string targetFileName = "/data/vendor/tbox/gecko-tbox-config.xml";  // 目标文件名

    // 使用文件系统库检查目标文件是否存在
    std::ifstream file(targetFileName);

    // 打开源文件和目标文件
    std::ifstream sourceFile(sourceFileName);
    std::ofstream targetFile(targetFileName);

    if (!sourceFile.is_open() || !targetFile.is_open()) {
        //std::cerr << "Failed to open files." << std::endl;
        LOG(INFO) << "Failed to open files.";
        return 1;
    }
    targetFile.clear();

    // 从源文件读取内容并写入目标文件
    std::string line;
    while (std::getline(sourceFile, line)) {
        targetFile << line << std::endl;
    }

    // 关闭文件
    sourceFile.close();
    targetFile.close();
    LOG(INFO) << "File copied successfully.";
    return 0;
}

int Tbox::getSOCPowerState(){
    StatusCode refStatus;
    VehiclePropValue refValue;
    mVehicleCtl.getPropertybyPropId((int)VehicleProperty::GECKO_SOC_POWER_STATUS, 0, refValue, refStatus);
    LOG(INFO) << __func__ << refValue.value.int32Values[0];
    return refValue.value.int32Values[0];
}

void Tbox::sendLogoutMessage(){
    if(mState == MachineState::Realtime || mState == MachineState::UpReport || mState == MachineState::Login) {
        LOG(INFO) << __func__ << __LINE__ << "send logout";
        std::vector<uint8_t> logoutmsg = mMessageData.generateLogoutMessage();
        mNetworkComm.SendQueueEnqueue(logoutmsg);
    } else {
        LOG(INFO) << __func__ << __LINE__ << "do not send logout as tbox wasnot login";
    }
    if (mXmlConfig.vehicleConfig.isSampling == 1
        && (mStateSample == MachineState::Realtime || mStateSample == MachineState::UpReport || mStateSample == MachineState::Login)) {
        std::vector<uint8_t> logoutmsgsample = mMessageData.generateLogoutMessageSample();
        mNetworkCommSample.SendQueueEnqueue(logoutmsgsample);
        LOG(INFO) << __func__ << __LINE__ << "send sampling logout";
    } else {
        LOG(INFO) << __func__ << __LINE__ << "do not send logout sample as tbox sample wasnot login";
    }
}

void Tbox::saveSpeedLimitLevelReq(uint8_t chargingCondition, uint8_t level){
    std::string requestLevel = "0";
    int charging = chargingCondition;
    std::string chargingCon = std::to_string(charging);
    switch(level){
        case 0x55:
            requestLevel = "1";
            break;
        case 0x56:
            requestLevel = "2";
            break;
        case 0x57:
            requestLevel = "3";
            break;
        case 0x58:
            requestLevel = "4";
            break;
        case 0x59:
            requestLevel = "5";
            break;
        case 0x60:
            requestLevel = "6";
            break;
        case 0xaa:
            requestLevel = "0";
            break;
        default:
            LOG(INFO) << "VehicleStatusMessage cannot handle SpeedLimitLevel msg, do nothing ";
            break;
    }
    property_set("persist.tbox.speedlimitlevel", requestLevel.c_str());
    property_set("persist.tbox.chargingcondition", chargingCon.c_str());
}
/*
 * @param workReq STR_Work_Req 1:有工作需求, 0:无工作需求
*/
void Tbox::setSTRWorkReq(int workReq){
    //0x0:No Node hasn’t woke up network(Passvie wakeup); 0x1:Node has woke up network(Active wakeup)
    LOG(INFO) << "setSTRWorkReq: " << workReq;
    mRemoteHaveCtl.vehicleKeepAwake(workReq);
}

void Tbox::revertIPConfig(){
    LOG(INFO) << __func__ << __LINE__ << "start revert IP config";
    ParmReportMessage parm;
    mXmlConfig.LoadParmConfig(parm);
    parm.mdomainName = mXmlConfig.ChangeNetParmSave.ip;
    parm.mdomainNameLen = mXmlConfig.ChangeNetParmSave.iplen;
    parm.mport = mXmlConfig.ChangeNetParmSave.port;
    parm.mpublicDomain = mXmlConfig.ChangeNetParmSave.pip;
    parm.mpublicDomainLen = mXmlConfig.ChangeNetParmSave.piplen;
    parm.mpublicPort = mXmlConfig.ChangeNetParmSave.pport;
    mXmlConfig.UpdateParmConfig(parm);
    mXmlConfig.vehicleConfigInit();
    mXmlConfig.ChangeNetParm = 0;
    mNetworkComm.mRetryCount = 0;
}

uint32_t Tbox::validateBCMPowerSt(){
    StatusCode refStatus;
    VehiclePropValue refValue;
    mVehicleCtl.getPropertybyPropId((int)VehicleProperty::IGNITION_STATE, 0, refValue, refStatus);
    uint32_t powerSt = refValue.value.int32Values[0];
    LOG(INFO) << __func__ << refValue.value.int32Values[0];
    if( powerSt == 0) {
        return 0;
    } else {
        return 1;
    }
}

uint8_t Tbox::getBCMPowerSt(){
    StatusCode refStatus;
    VehiclePropValue refValue;
    mVehicleCtl.getPropertybyPropId((int)VehicleProperty::GECKO_BCM_SYSPOWERSTS, 0, refValue, refStatus);
    uint8_t powerSt = static_cast<uint8_t>(refValue.value.int32Values[0]);
    LOG(INFO) << __func__ << static_cast<int>(powerSt);
    return powerSt;
}

void Tbox::waitingForVehicleAwake(){
    StatusCode refStatus;
    VehiclePropValue refValue;
    for(auto i = 0; i<10; i++){
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        mVehicleCtl.getPropertybyPropId((int)VehicleProperty::IGNITION_STATE, 0, refValue, refStatus);
        int ignitionSt = refValue.value.int32Values[0];
        LOG(INFO) << __func__ << ignitionSt;
        if(ignitionSt != -1024){
            break;
        }
    }
}