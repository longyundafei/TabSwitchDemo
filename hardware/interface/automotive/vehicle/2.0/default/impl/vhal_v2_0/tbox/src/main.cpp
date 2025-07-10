#include <android-base/logging.h>
#include <sys/stat.h>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <ostream>
#include <thread>
#include <vector>
#include "GB32960MessageParser.h"
#include "Machine.h"
#include "Tbox.h"
#include "messageproductstate.h"
#include "networkcomm.h"
#include <signal.h>
#include <unistd.h>
#define PROPERTY_VALUE_MAX_LEN 250

Tbox* p_gTbox;
void handle_sigterm(int sig)
{
    // 注意：在信号处理器中调用任何非异步安全函数都可能会导致未定义行为
    LOG(INFO) << __func__ << ":" << sig << ":" << "in";
    if(p_gTbox->mState == MachineState::Realtime){
//        p_gTbox->mState = MachineState::Logout;
        //生成登出报文，压入队列
    }
//    write(STDOUT_FILENO, "Caught SIGTERM\n", 15);
    // 进行资源清理等工作...
//    _exit(0); // 使用_exit 而不是 exit，因为 exit 不是异步安全的
}
void StateTask() {
    LOG(INFO) << __func__ << "in";
    while (true) {
        // 切换对应状态
    }
}
void HeartTask(Tbox* pBox) {
    LOG(INFO) << __func__ << "in";
    std::vector<uint8_t> heartmessage;
    int hearttime;
    while (true) {
        // 切换对应状态
        hearttime = pBox->mXmlConfig.vehicleConfig.heartbeatCycle;
        std::this_thread::sleep_for(std::chrono::seconds(hearttime));
        if (pBox->mState == MachineState::Realtime) {
            LOG(INFO) << __func__ << "in" << hearttime;
            heartmessage = pBox->mMessageData.generateHeartbeatMessage();
            pBox->mNetworkComm.SendQueueEnqueue(heartmessage);
        }
    }
}
void HeartTaskSample(Tbox* pBox) {
    LOG(INFO) << __func__ << "in";
    std::vector<uint8_t> heartmessage;
    int hearttime;
    while (true) {
        // 切换对应状态
        hearttime = pBox->mXmlConfig.vehicleConfig.heartbeatCycle;
        std::this_thread::sleep_for(std::chrono::seconds(hearttime));
        if(pBox->mXmlConfig.vehicleConfig.isSampling == 1){
            if (pBox->mStateSample == MachineState::Realtime) {
                LOG(INFO) << __func__ << "in" << hearttime;
                heartmessage = pBox->mMessageData.generateHeartbeatMessage();
                pBox->mNetworkCommSample.SendQueueEnqueue(heartmessage);
            }
        }
    }
}
void CommunicationTask(Tbox* pTbox) {
    LOG(INFO) << __func__ << "in";
    // pNetworkComm->Start();
    while (true) {
        LOG(INFO) << "waitfor updatenetstate " << __func__ << __LINE__;
        pTbox->mNetworkComm.getUpdateNetState();
        LOG(INFO) << __func__ << __LINE__;
        pTbox->mState = MachineState::Logout;
        pTbox->mNetworkComm.stop();
        pTbox->mNetworkComm.setStopState();
    }
}


void SendTask(Tbox* pTbox) {
    std::vector<std::vector<uint8_t>> result;
    while (true) {
        if (pTbox->mState == MachineState::Realtime) {
            if (pTbox->mMsgProductState == MessageProductState::Normal) {
                std::this_thread::sleep_for(
                        std::chrono::seconds(pTbox->mXmlConfig.vehicleConfig.normalReportCycle));
            } else if (pTbox->mMsgProductState == MessageProductState::Warn) {
                std::this_thread::sleep_for(
                        std::chrono::seconds(pTbox->mXmlConfig.vehicleConfig.alarmReportCycle));
            }
            // Cache中无数据时从文件取数据
            if (pTbox->mNetworkComm.GetCacheSendQueueEnqueueSize() == 0) {
                if (pTbox->mCacheResult.size() == 0) {
                    pTbox->mCacheResult = pTbox->mBinaryMessage.readMessages(false);
                } else {
                    for (const auto& message : pTbox->mCacheResult) {
                        // 遍历每个消息（每个 std::vector<uint8_t>）
                        int ret = 0;
                        ret = pTbox->mBinaryMessage.checkMsgDateOutof7Days(message[24], message[25], message[26]);
                        LOG(INFO) << __func__ << __LINE__ << static_cast<int>(message[24]) << "."<< static_cast<int>(message[25])  << "."<< static_cast<int>(message[26]);
                        if (ret == 0){
                            pTbox->mNetworkComm.SendCacheQueueEnqueue(message);
                            LOG(INFO) << __func__ << __LINE__ << "send message in Cache";
                        } else {
                            LOG(INFO) << __func__ << __LINE__ << "drop message in Cache";
                        }
                    }
                    pTbox->mCacheResult.clear();
                }
            }
        } else if (pTbox->mState == MachineState::UpReport) {
            std::vector<uint8_t> timesyncmessage = pTbox->mMessageData.generatetimeSynchronizationMessage();
            pTbox->mNetworkComm.SendQueueEnqueue(timesyncmessage);
            std::this_thread::sleep_for(std::chrono::seconds(2));

            //校时操作 08
            //1B VIN上传
            //1C 电量信息上传
            //20 自定义报警数据

        } else {
            std::this_thread::sleep_for(std::chrono::seconds(10));
        }
    }
}
void SendTaskSample(Tbox* pTbox) {
    std::vector<std::vector<uint8_t>> result;
    while (true) {
        if (pTbox->mMsgProductState == MessageProductState::Normal) {
            std::this_thread::sleep_for(
                    std::chrono::seconds(pTbox->mXmlConfig.vehicleConfig.normalReportCycle));
        } else if (pTbox->mMsgProductState == MessageProductState::Warn) {
            std::this_thread::sleep_for(
                    std::chrono::seconds(pTbox->mXmlConfig.vehicleConfig.alarmReportCycle));
        }
        if (pTbox->mStateSample == MachineState::Realtime) {
            // Cache中无数据时从文件取数据
            if (pTbox->mNetworkCommSample.GetCacheSendQueueEnqueueSize() == 0) {
                if (pTbox->mCacheResultSample.size() == 0) {
                    pTbox->mCacheResultSample = pTbox->mBinaryMessage.readMessages(true);
                } else {
                    for (const auto& message : pTbox->mCacheResultSample) {
                        // 遍历每个消息（每个 std::vector<uint8_t>）
                        int ret = 0;
                        ret = pTbox->mBinaryMessage.checkMsgDateOutof7Days(message[24], message[25], message[26]);
                        if (ret == 0) {
                            pTbox->mNetworkCommSample.SendCacheQueueEnqueue(message);
                            LOG(INFO) << __func__ << __LINE__ << "send message in Sample Cache";
                        } else {
                            LOG(INFO) << __func__ << __LINE__ << "drop message in Sample Cache";
                        }
                    }
                    pTbox->mCacheResultSample.clear();
                }
            }
        }
    }
}
void RecvTask(Tbox* pTbox) {
    std::vector<uint8_t> receivedMessage;
    while (true) {
        receivedMessage = pTbox->mNetworkComm.RecvQueueDequeue();
        //        LOG(INFO) << "RecvTask Get a Message";
        pTbox->HandleMessage(receivedMessage);
    }
}
void RecvTaskSample(Tbox* pTbox) {
    std::vector<uint8_t> receivedMessage;
    while (true) {
        receivedMessage = pTbox->mNetworkCommSample.RecvQueueDequeue();
        //        LOG(INFO) << "RecvTask Get a Message";
        pTbox->HandleMessageSample(receivedMessage);
    }
}
void WatchTask(Tbox* pTbox) {
    pTbox->mNetworkComm.mNetState = NetworkComm::NetState::NoConnect;
    //pTbox->setSTRWorkReq(1);
    pTbox->mNetworkComm.Start(&(pTbox->mXmlConfig),false);
    struct timeval tv;
    long long endTime = 0;
    int delay = 0;
    char propValue[PROPERTY_VALUE_MAX_LEN] = "\0";
    std::string testMode = "0";
    memset(propValue, 0, sizeof(propValue));
    if(property_get("persist.gecko.tbox.testmode", propValue, nullptr)!=0){
        testMode = propValue;
        if(testMode == "1"){
            LOG(INFO) << __func__ <<"GB32960_3_TEST";
        }
    }
    LOG(INFO) << __func__ << __LINE__;
    while(true) {
        switch(pTbox->mNetworkComm.mNetState){
            case NetworkComm::NetState::NoConnect:
                LOG(INFO) << __func__ << __LINE__ << "no connect";
                //pTbox->setSTRWorkReq(2);
                if(testMode == "0"){
                    //pTbox->mLogoutReason = 0;
                    pTbox->mNetworkComm.mRetryCount = 0;
                    if(pTbox->mSTR == 1){
                        LOG(INFO) << __func__ << __LINE__ << "detect no connect entering STR mode, donot connect";
                        std::this_thread::sleep_for(std::chrono::seconds(1));
                        break;
                    } else {
                        LOG(INFO) << __func__ << __LINE__ << "detect no connect after exit STR mode, connect TSP in 1s";
                        std::this_thread::sleep_for(std::chrono::seconds(1));
                    }
                } else {
                    if (pTbox->mNetworkComm.mRetryCount < 3) {
                        LOG(INFO) << __func__ << __LINE__ << "mRetryCount = " << pTbox->mNetworkComm.mRetryCount;
                        if(pTbox->mNetworkComm.state ==3){
                            delay = static_cast<int>(endTime - pTbox->mNetworkComm.startTime);
                            if(delay < 0 || delay > 60000){
                                delay = 0;
                            }
                            std::this_thread::sleep_for(std::chrono::milliseconds(60000-delay));
                        } else {
                            std::this_thread::sleep_for(std::chrono::seconds(60));
                        }
                    } else {
                        LOG(INFO) << __func__ << __LINE__ << "mRetryCount >= 3";
                        /*std::this_thread::sleep_for(
                                std::chrono::seconds((pTbox->mXmlConfig.vehicleConfig.loginRetryInterval) * 60));*/
                        for(auto sleepIterate= 0; sleepIterate < pTbox->mXmlConfig.vehicleConfig.loginRetryInterval; sleepIterate++){
                            if(pTbox->mNetworkComm.state ==3 && sleepIterate == 0){
                                delay = static_cast<int>(endTime - pTbox->mNetworkComm.startTime);
                                if(delay < 0 || delay > 60000){
                                    delay = 0;
                                }
                            std::this_thread::sleep_for(std::chrono::milliseconds(60000-delay));
                            } else {
                                std::this_thread::sleep_for(std::chrono::seconds(60));
                            }
                            //check net
                            if(pTbox->mNetworkComm.state !=3){
                                LOG(INFO) << __func__ << __LINE__ << "start detect network";
                                int ret = pTbox->mNetworkComm.checkNetwork();
                                if(ret ==  0){
                                    LOG(INFO) << __func__ << __LINE__ << "detect network restore";
                                    pTbox->mNetworkComm.mRetryCount = 0;
                                    break;
                                }
                                LOG(INFO) << __func__ << __LINE__ << "end detect network";
                            } else {
                                LOG(INFO) << __func__ << __LINE__ << "no need to detect network";
                            }

                        }
                    }
                }
                //pTbox->setSTRWorkReq(1);
                pTbox->mNetworkComm.Start(&(pTbox->mXmlConfig),false);
            break;
            case NetworkComm::NetState::Connected:
                pTbox->mNetworkComm.getUpdateNetState();
                //pTbox->setSTRWorkReq(2);
                LOG(INFO) << __func__ << __LINE__ << "disable net!";
                if(pTbox->mNetworkComm.state ==3){
                    LOG(INFO) << __func__ << __LINE__ << "login failed, we need record this login failure";
                    pTbox->mNetworkComm.mRetryCount++;
                }
                gettimeofday(&tv, NULL);
                endTime = tv.tv_sec*1000LL+tv.tv_usec/1000;
                pTbox->mState = MachineState::Logout;
                if(pTbox->mXmlConfig.ChangeNetParm == 1 && pTbox->mNetworkComm.running == false){
                    pTbox->revertIPConfig();
                    LOG(INFO) << __func__ << __LINE__ << "login no response,but socket is s revert ip and set ChangeNetParm to 0";
                }
                pTbox->mNetworkComm.stop();
                pTbox->mNetworkComm.setStopState();
                pTbox->mNetworkComm.mNetState = NetworkComm::NetState::NoConnect;
            break;
            default:
            break;
        }
    }
//    while (true) {
//        LOG(INFO) << __func__ << __LINE__;
//        pNetworkComm->getStopState();
//        LOG(INFO) << __func__ << __LINE__;
//        if (conf->ChangeNetParm == 0) {
//            if (pNetworkComm->mRetryCount < 3) {
//                LOG(INFO) << __func__ << __LINE__;
//                std::this_thread::sleep_for(std::chrono::seconds(60));
//            } else {
//                LOG(INFO) << __func__ << __LINE__;
//                std::this_thread::sleep_for(
//                        std::chrono::seconds((conf->vehicleConfig.loginRetryInterval) * 60));
//            }
//        } else {
//            std::this_thread::sleep_for(std::chrono::seconds(15));
//        }
//        pNetworkComm->Start(conf);
//    }
}
void WatchTaskSample(Tbox* pTbox) {
    pTbox->mNetworkCommSample.mNetState = NetworkComm::NetState::NoConnect;
    if(pTbox->mXmlConfig.vehicleConfig.isSampling == 1){
        pTbox->mNetworkCommSample.Start(&(pTbox->mXmlConfig),true);
    }
    LOG(INFO) << __func__ << __LINE__;
    while(true) {
        switch(pTbox->mNetworkCommSample.mNetState){
            case NetworkComm::NetState::NoConnect:
                if (pTbox->mNetworkCommSample.mRetryCount < 3) {
                    LOG(INFO) << __func__ << __LINE__;
                    std::this_thread::sleep_for(std::chrono::seconds(60));
                } else {
                    LOG(INFO) << __func__ << __LINE__;
                    std::this_thread::sleep_for(
                            std::chrono::seconds((pTbox->mXmlConfig.vehicleConfig.loginRetryInterval) * 60));
                }
                if(pTbox->mXmlConfig.vehicleConfig.isSampling == 1){
                    pTbox->mNetworkCommSample.Start(&(pTbox->mXmlConfig),true);
                }
            break;
            case NetworkComm::NetState::Connected:
                pTbox->mNetworkCommSample.getUpdateNetState();
                LOG(INFO) << __func__ << __LINE__ << "disable net!";
                pTbox->mStateSample = MachineState::Logout;
                pTbox->mNetworkCommSample.stop();
                pTbox->mNetworkCommSample.setStopState();
                pTbox->mNetworkCommSample.mNetState = NetworkComm::NetState::NoConnect;
            break;
            default:
            break;
        }
    }
}

void WatchSOCPowerStateTask(Tbox* pTbox){
    LOG(INFO) << __func__ << __LINE__;
    pTbox->mVehicleCtl.subscribeToProperty((int)VehicleProperty::GECKO_SOC_POWER_STATUS);
    while(true){
        if((pTbox != nullptr) && (pTbox->mVehicleCtl.mCallback != nullptr)){
            pTbox->mVehicleCtl.mCallback->getUpdateSOCPowerState();
            LOG(INFO) << __func__ << __LINE__ << "enter ! SOC CHANGE";
            //std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            switch(pTbox->getSOCPowerState()){
                case 3:
                case 4:
                //do nothing
                LOG(INFO) << __func__ << __LINE__ << "enter ! SOC wakeup";
                pTbox->mSTR = 0;
                break;
                case 1:
                case 5:
                //send logout message
                LOG(INFO) << __func__ << __LINE__ << "enter ! SOC sleep";
                pTbox->sendLogoutMessage();
                pTbox->mSTR = 1;
                break;
                default:
                break;
            }
        } else {
            LOG(INFO) << __func__ << __LINE__ << "error ! can not get callback function";
            std::this_thread::sleep_for(std::chrono::seconds(50));
        }
    }
}

void signalWatchTask(Tbox* pTbox) {
    uint32_t mileage = 0;
    uint32_t start = 0;
    uint32_t limitReq = 0;
    std::string iccid;
    std::string signalStrength;
    std::string speedlimitlevel;
    int signal = 0;
    while(true) {
        LOG(INFO) << __func__ << __LINE__;
        std::this_thread::sleep_for(std::chrono::seconds(60));
        char propertyValue[PROPERTY_VALUE_MAX_LEN] = "\0";
        uint32_t version = pTbox->mRemoteVersionSwitch.getVCUVersion();
        if (version == 0x01) {
            int level = 0;
            memset(propertyValue, 0, sizeof(propertyValue));
            property_get("persist.tbox.speedlimitlevel", propertyValue, nullptr);
            speedlimitlevel = propertyValue;
            if(speedlimitlevel.size()!=0) {
                level = std::stoi(speedlimitlevel);
            }
            if (level == 0 || level == 2 || level == 3){
                memset(propertyValue, 0, sizeof(propertyValue));
                //获取iccid，获取signal strength
                property_get("persist.iccid", propertyValue, nullptr);
                iccid = propertyValue;
                memset(propertyValue, 0, sizeof(propertyValue));
                property_get("persist.tbox.signalStrength", propertyValue, nullptr);
                signalStrength = propertyValue;
                if (signalStrength.size()!=0){
                    signal = std::stoi(signalStrength);
                }
                if(iccid.size() == 0 || (signal > 99)) {
                    if (start == 0) {
                        LOG(INFO) << __func__ << __LINE__ << " start accumulate mileage";
                        start = pTbox->mVehiclemessage.getaccumulatedMileageValue();
                    }
                    uint32_t end = pTbox->mVehiclemessage.getaccumulatedMileageValue();
                    mileage = end - start;
                    LOG(INFO) << __func__ << __LINE__ << " end = " << end << ", start = " << start;
                    if(mileage >= 500 && limitReq == 0){
                        LOG(INFO) << __func__ << __LINE__ << " accumulate mileage over 50, send speed limit level cmd";
                        pTbox->mLockVehicle.setSpeedLimitLevel(0x0, 0x55);
                        limitReq = 1;
                    }
                }
            }
        }
        memset(propertyValue, 0, sizeof(propertyValue));
        property_get("persist.iccid", propertyValue, nullptr);
        iccid = propertyValue;
        memset(propertyValue, 0, sizeof(propertyValue));
        property_get("persist.tbox.signalStrength", propertyValue, nullptr);
        signalStrength = propertyValue;
        if (signalStrength.size()!=0){
            signal = std::stoi(signalStrength);
        }else {
            signal = 0;
        }
        LOG(INFO) << __func__ << __LINE__ << " signal = " << signal;
        if ((iccid.size()!=0) && (signal <99)){
            LOG(INFO) << __func__ << __LINE__ << " start to recover";
            if (limitReq == 1){
                LOG(INFO) << __func__ << __LINE__ << " recover speed limit";
                //解除限速
                int level = 0;
                int chargingCon = 0;
                memset(propertyValue, 0, sizeof(propertyValue));
                if(property_get("persist.tbox.speedlimitlevel", propertyValue, nullptr) !=0){
                    std::string speedlimitlevel = propertyValue;
                    level = std::stoi(speedlimitlevel);
                }
                memset(propertyValue, 0, sizeof(propertyValue));
                if(property_get("persist.tbox.chargingcondition", propertyValue, nullptr)!=0){
                    std::string chargingcondition = propertyValue;
                    chargingCon = std::stoi(chargingcondition);
                }
                pTbox->mLockVehicle.setSpeedLimitLevel(chargingCon, level);
                limitReq = 0;
                start = 0;
            } else if (start != 0){
                LOG(INFO) << __func__ << __LINE__ << " only set start to 0";
                start = 0;
            }
        }
    }
}

void realNameAuthStateWatchTask(Tbox* pTbox) {
    int i = 0;
    int req = 0;
    int st = 0;
    char propertyValue[PROPERTY_VALUE_MAX_LEN] = "\0";
    while(true) {
        LOG(INFO) << __func__ << __LINE__;
        std::this_thread::sleep_for(std::chrono::seconds(10));
        i++;
        memset(propertyValue, 0, sizeof(propertyValue));
        if(property_get("persist.tbox.authstate", propertyValue, nullptr)!=0){
            std::string authSt = propertyValue;
            st = std::stoi(authSt);
        }
        if(i >= 30){
            i = 0;
            if((pTbox->mState == MachineState::Realtime) && (st != 1)){
                LOG(INFO) << __func__ << "st is not 1, start query";
                std::vector<uint8_t> RealNameAuthMessage = pTbox->mMessageData.generateRealNameAuthMessage();
                pTbox->mNetworkComm.SendQueueEnqueue(RealNameAuthMessage);
            }
        }
        memset(propertyValue, 0, sizeof(propertyValue));
        if(property_get("gecko.tbox.authreq", propertyValue, nullptr)!=0){
            std::string authReq = propertyValue;
            req = std::stoi(authReq);
        }
        if((pTbox->mState == MachineState::Realtime) && (req == 1)){
            LOG(INFO) << __func__ << "got auth query request";
            std::vector<uint8_t> RealNameAuthMessage = pTbox->mMessageData.generateRealNameAuthMessage();
            pTbox->mNetworkComm.SendQueueEnqueue(RealNameAuthMessage);
            property_set("gecko.tbox.authreq", "0");
        }
    }
}

void updateMessageData(Tbox* pTbox, int sleep_time) {
    pTbox->mProbeTemperatureMessage.probeTemperatures.resize(pTbox->mVehicleStatusInfo.probNum);
    pTbox->mProbeTemperatureMessage.probeTemperatures.assign(pTbox->mVehicleStatusInfo.probeTemperatureList.begin(), pTbox->mVehicleStatusInfo.probeTemperatureList.end());
    pTbox->mBatteryVoltageMessage.mCellVoltages.resize(pTbox->mVehicleStatusInfo.totalCellNum);
    pTbox->mBatteryVoltageMessage.mCellVoltages.assign(pTbox->mVehicleStatusInfo.cellVotageList.begin(), pTbox->mVehicleStatusInfo.cellVotageList.end());
    pTbox->mVehicleStatusInfo.mutexUpdateRechargeableFault.lock();
    pTbox->mFaultdatamessage.rechargeableFaultCodes.resize(0);
    pTbox->mFaultdatamessage.rechargeableFaultCodes.assign(pTbox->mVehicleStatusInfo.rechargeableFaultList.begin(), pTbox->mVehicleStatusInfo.rechargeableFaultList.end());
    pTbox->mVehicleStatusInfo.mutexUpdateRechargeableFault.unlock();
    pTbox->mVehicleStatusInfo.mutexUpdateDriveMotorFault.lock();
    pTbox->mFaultdatamessage.driveMotorFaultCodes.resize(0);
    pTbox->mFaultdatamessage.driveMotorFaultCodes.assign(pTbox->mVehicleStatusInfo.driveMotorFaultList.begin(), pTbox->mVehicleStatusInfo.driveMotorFaultList.end());
    pTbox->mVehicleStatusInfo.mutexUpdateDriveMotorFault.unlock();
    /*faultdatamessage.engineFaultCount = vehicleStatusInfo.rechargeableFaultList.size();
    faultdatamessage.engineFaultCodes.resize(0);
    faultdatamessage.engineFaultCodes.assign(vehicleStatusInfo.rechargeableFaultList.begin(), vehicleStatusInfo.rechargeableFaultList.end());*/
    pTbox->mVehicleStatusInfo.mutexUpdateOtherFault.lock();
    pTbox->mFaultdatamessage.otherFaultCodes.resize(0);
    pTbox->mFaultdatamessage.otherFaultCodes.assign(pTbox->mVehicleStatusInfo.otherFaultList.begin(), pTbox->mVehicleStatusInfo.otherFaultList.end());
    pTbox->mVehicleStatusInfo.mutexUpdateOtherFault.unlock();
    pTbox->mVehicleStatusInfo.mutexUpdateErrTabIndex.lock();
    int errTabIndexCount = pTbox->mVehicleStatusInfo.errTabIndexList.size();
    for(auto i = 0; i < 20; i++){
        if(i < errTabIndexCount){
            pTbox->mBatterySysInfoMessage.bms_err_tab_index[i] = pTbox->mVehicleStatusInfo.errTabIndexList[i];
        } else {
            pTbox->mBatterySysInfoMessage.bms_err_tab_index[i] = 0;
        }
    }
    pTbox->mVehicleStatusInfo.mutexUpdateErrTabIndex.unlock();
    pTbox->mVehicleStatusInfo.mMsgProductState = pTbox->mMsgProductState;
    pTbox->mVehicleStatusInfo.mSleepTime = sleep_time;
    //update remotePower ctrl\ seat heat\ steer wheel heat req
    if(pTbox->mRemoteHaveCtl.mRemotePowerCtrlReq == 0x02 || pTbox->mRemoteSeatHeatCtl.mRemotePowerCtrlReq == 0x02 || pTbox->mRemoteSteerWheelHeatCtl.mRemotePowerCtrlReq == 0x02){
        pTbox->mTerminalSysInfoMessage.remt_power_ctrl_req = 0x02;
    } else if (pTbox->mRemoteHaveCtl.mRemotePowerCtrlReq == 0x00 && pTbox->mRemoteSeatHeatCtl.mRemotePowerCtrlReq == 0x00 && pTbox->mRemoteSteerWheelHeatCtl.mRemotePowerCtrlReq == 0x00){
        pTbox->mTerminalSysInfoMessage.remt_power_ctrl_req = 0x00;
    } else {
        pTbox->mTerminalSysInfoMessage.remt_power_ctrl_req = 0x01;
    }
    pTbox->mTerminalSysInfoMessage.remt_mai_dr_seat_heat_req = pTbox->mRemoteSeatHeatCtl.mSeatHeatReq;
    pTbox->mTerminalSysInfoMessage.remt_steer_whl_heat_req = pTbox->mRemoteSteerWheelHeatCtl.mSteerWheelHeatReq;
}
void MessageCreateTask(Tbox* pTbox) {
    std::vector<uint8_t> realDataMessage;
    int sleep_time;
    int saveInterval;
    int alarmModeDuration = 0;
    int duration = 0;
    int saveDuration = 0;
    struct timeval tv;
    long long start = 0;
    long long end = 0;
    while (true) {
        //LOG(INFO) <<  __func__ << "while start" ;
        if (!pTbox->mMessageData.mUniqueID.empty()){
            saveInterval = pTbox->mXmlConfig.vehicleConfig.localStorageCycle/1000;
            if (pTbox->mMsgProductState == MessageProductState::Normal) {
                sleep_time = pTbox->mXmlConfig.vehicleConfig.normalReportCycle;
            } else if (pTbox->mMsgProductState == MessageProductState::Warn) {
                sleep_time = pTbox->mXmlConfig.vehicleConfig.alarmReportCycle;
                alarmModeDuration++;
            }
            //LOG(INFO) << __func__ << "--" << sleep_time;
            int delay = static_cast<int>(end - start);
            if(delay >= 1000){
                delay = 0;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1000-delay));
            gettimeofday(&tv, NULL);
            start = tv.tv_sec*1000LL+tv.tv_usec/1000;
            updateMessageData(pTbox, sleep_time);
            /*realDataMessage = pTbox->mMessageData.generateRealtimeMessage(
                    pTbox->mVehiclemessage, pTbox->mDrivemotormessage,
                    pTbox->mLocationDataMessage, pTbox->mExtremedatamessage,
                    pTbox->mFaultdatamessage, pTbox->mBatteryVoltageMessage,
                    pTbox->mProbeTemperatureMessage, pTbox->mCustomInfoFlags,
                    pTbox->mDataCollectionFlags, pTbox->mVehicleStatusMessage,
                    pTbox->mDoorLockStatus, pTbox->mHavcStatus);*/
            //modify for ME11
            realDataMessage = pTbox->mMessageData.generateME11RealtimeMessage(
                    pTbox->mVehiclemessage, pTbox->mDrivemotormessage,
                    pTbox->mLocationDataMessage, pTbox->mExtremedatamessage,
                    pTbox->mFaultdatamessage, pTbox->mBatteryVoltageMessage,
                    pTbox->mProbeTemperatureMessage, pTbox->mFrontCamInfoMessage,
                    pTbox->mIntgSwitchInfoMessage, pTbox->mTerminalSysInfoMessage,
                    pTbox->mRadioSysInfoMessage, pTbox->mReverseAssistantSysInfoMessage,
                    pTbox->mChassisSysInfoMessage, pTbox->mPowerSysInfoMessage,
                    pTbox->mBatterySysInfoMessage, pTbox->mBatteryChargingInfoMessage,
                    pTbox->mACSysInfoMessage, pTbox->mVehicleBodySysInfoMessage,
                    pTbox->mVehicleControllerInfoMessage);
            LOG(INFO) <<  __func__ << "ME11generateRealtimeMessage end" ;
            if (pTbox->mFaultdatamessage.maxAlarmLevel == 0x03
                && pTbox->mLastAlarmLevel != 0x03) {
                pTbox->mMsgProductState = MessageProductState::Warn;
                alarmModeDuration = 0;
                duration = 0;
                std::vector<uint8_t> addtionalMessage;
                int addtionalMessageCount = pTbox->mMsg30SecondsQueue.getQueueSize();
                LOG(INFO) <<  __func__ << "addtionalMessageCount = " << addtionalMessageCount;
                for (auto i = 0; pTbox->mMsg30SecondsQueue.getQueueSize() > 0; i++){
                    addtionalMessage = pTbox->mMsg30SecondsQueue.dequeue();
                    pTbox->mMsgQueue.enqueue(addtionalMessage);
                }
                if(pTbox->mXmlConfig.vehicleConfig.isSampling == 1){
                    std::vector<uint8_t> addtionalSampleMessage;
                    int addtionalSampleMessageCount = pTbox->mMsg30SecondsSampleQueue.getQueueSize();
                    LOG(INFO) <<  __func__ << "addtionalSampleMessageCount = " << addtionalSampleMessageCount;
                    for (auto i = 0; pTbox->mMsg30SecondsSampleQueue.getQueueSize() > 0; i++){
                        addtionalSampleMessage = pTbox->mMsg30SecondsSampleQueue.dequeue();
                        pTbox->mMsgQueueSample.enqueue(addtionalSampleMessage);
                    }
                }
            }
            size_t addtionalQueueSize = static_cast<size_t>(30/pTbox->mXmlConfig.vehicleConfig.alarmReportCycle);

            pTbox->mLastAlarmLevel = pTbox->mFaultdatamessage.maxAlarmLevel;
            //LOG(INFO) << __func__ << "-- mMsgProductState : " << (int)pTbox->mMsgProductState;
            //LOG(INFO) << __func__ << "-- duration : " << duration;
            if (duration%sleep_time == 0){
                duration = 0;
                LOG(INFO) << __func__ << "-- send realtimemessage : ";
                if(pTbox->mState == MachineState::Realtime) {
                    pTbox->mMsgQueue.enqueue(realDataMessage);
                    /*while (pTbox->mSave7DayQueue.getQueueSize() != 0) {
                        pTbox->mSave7DayQueue.dequeue();
                    }*/
                    //pTbox->mSave7DayQueue.enqueue(realDataMessage);
                } else {
                    pTbox->mMsgQueue.enqueue(pTbox->mMessageData.mAddtionMessage);
                    /*while (pTbox->mSave7DayQueue.getQueueSize() != 0) {
                        pTbox->mSave7DayQueue.dequeue();
                    }*/
                    //pTbox->mSave7DayQueue.enqueue(pTbox->mMessageData.mAddtionMessage);
                }
                if(pTbox->mXmlConfig.vehicleConfig.isSampling == 1) {
                    if(pTbox->mStateSample == MachineState::Realtime) {
                        pTbox->mMsgQueueSample.enqueue(realDataMessage);
                    } else {
                        pTbox->mMsgQueueSample.enqueue(pTbox->mMessageData.mAddtionMessage);
                    }
                }
            }
            //LOG(INFO) << __func__ << "save msg -- saveDuration : " << saveDuration;
            if (saveDuration%saveInterval == 0){
                //LOG(INFO) << __func__ << "save msg";
                saveDuration = 0;
                if(pTbox->mState == MachineState::Realtime){
                    pTbox->mSave7DayQueue.enqueue(realDataMessage);
                } else {
                    pTbox->mSave7DayQueue.enqueue(pTbox->mMessageData.mAddtionMessage);
                }
            }
            duration++;
            saveDuration++;
            //LOG(INFO) << __func__ << "-- duration : " << duration;
            //LOG(INFO) << __func__ << "-- alarmModeDuration : " << alarmModeDuration;
            if(duration%pTbox->mXmlConfig.vehicleConfig.alarmReportCycle == 0) {
                while(pTbox->mMsg30SecondsQueue.getQueueSize()>=addtionalQueueSize){
                    pTbox->mMsg30SecondsQueue.dequeue();
                }
                pTbox->mMsg30SecondsQueue.enqueue(pTbox->mMessageData.mAddtionMessage);
                if(pTbox->mXmlConfig.vehicleConfig.isSampling == 1){
                    while (pTbox->mMsg30SecondsSampleQueue.getQueueSize()>=addtionalQueueSize)
                    {
                        pTbox->mMsg30SecondsSampleQueue.dequeue();
                    }
                    pTbox->mMsg30SecondsSampleQueue.enqueue(pTbox->mMessageData.mAddtionMessage);
                }
            }
            if (pTbox->mMsgProductState == MessageProductState::Warn && alarmModeDuration == (30+pTbox->mXmlConfig.vehicleConfig.alarmReportCycle)){
                pTbox->mMsgProductState = MessageProductState::Normal;
                alarmModeDuration = 0;
                duration = 0;
                LOG(INFO) << __func__ << "-- mMsgProductState : Normal";
            }
            gettimeofday(&tv, NULL);
            end = tv.tv_sec*1000LL+tv.tv_usec/1000;
        } else {
            std::this_thread::sleep_for(std::chrono::seconds(20));
        }
        //LOG(INFO) <<  __func__ << "while end" ;
    }
}
void MessageconsumerTask(Tbox* pBox) {
    std::vector<uint8_t> msg;
    int i = 0;
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        // 判断实时队列是否超过了100条记录
        if (pBox->mState != MachineState::Realtime || pBox->mMsgQueue.getQueueSize() > 10) {
            // 超过就离线保存(离线保存前需要手动变更标识02到03并更新校验值)，并清空实时队列
            while (pBox->mMsgQueue.getQueueSize() != 0) {
                msg = pBox->mMsgQueue.dequeue();
                std::vector<uint8_t> saveMsg = pBox->mMessageData.changeMessageID(0x03, msg);
                pBox->mSaveQueue.enqueue(saveMsg);
            }
        } else {
            if (pBox->mState == MachineState::Realtime) {
                if (pBox->mNetworkComm.GetSendQueueEnqueueSize() == 0) {
                    if (pBox->mMsgQueue.getQueueSize() != 0) {
                        msg = pBox->mMsgQueue.dequeue();
                        pBox->mNetworkComm.SendQueueEnqueue(msg);
                    } else if (pBox->mNetworkComm.GetCacheSendQueueEnqueueSize() != 0) {
                        // 此处可以判定为闲置时间
                        msg = pBox->mNetworkComm.SendCacheQueuedequeue();
                        pBox->mNetworkComm.SendQueueEnqueue(msg);
                        LOG(INFO) << "send cache queue " << i++;
                    }
                }
            }
        }
    }
}
void MessageconsumerTaskSample(Tbox* pBox) {
    std::vector<uint8_t> msg;
    int i = 0;
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        // 判断实时队列是否超过了100条记录
        if (pBox->mMsgQueueSample.getQueueSize() > 10) {
            // 超过就离线保存(离线保存前需要手动变更标识02到03并更新校验值)，并清空实时队列
            while (pBox->mMsgQueueSample.getQueueSize() != 0) {
                msg = pBox->mMsgQueueSample.dequeue();
                std::vector<uint8_t> saveMsg = pBox->mMessageData.changeMessageID(0x03, msg);
                pBox->mSaveQueueSample.enqueue(saveMsg);
            }
        } else {
            if (pBox->mStateSample == MachineState::Realtime) {
                if (pBox->mNetworkCommSample.GetSendQueueEnqueueSize() == 0) {
                    if (pBox->mMsgQueueSample.getQueueSize() != 0) {
                        msg = pBox->mMsgQueueSample.dequeue();
                        pBox->mNetworkCommSample.SendQueueEnqueue(msg);
                    } else if (pBox->mNetworkCommSample.GetCacheSendQueueEnqueueSize() != 0) {
                        // 此处可以判定为闲置时间
                        msg = pBox->mNetworkCommSample.SendCacheQueuedequeue();
                        pBox->mNetworkCommSample.SendQueueEnqueue(msg);
                        LOG(INFO) << "send Sample cache queue " << i++;
                    }
                }
            }
        }
    }
}
void MessageSaveTask(Tbox* pBox) {
    std::vector<uint8_t> saveDataMessage;
    while (true) {
        saveDataMessage = pBox->mSaveQueue.dequeue();
        pBox->mBinaryMessage.appendMessage(saveDataMessage,false);
        // 离线保存消息
    }
}
void MessageSaveTaskSample(Tbox* pBox) {
    std::vector<uint8_t> saveDataMessage;
    while (true) {
        saveDataMessage = pBox->mSaveQueueSample.dequeue();
        pBox->mBinaryMessage.appendMessage(saveDataMessage,true);
        // 离线保存消息
    }
}
void MessageSave7DayTask(Tbox* pBox) {
    std::vector<uint8_t> saveDataMessage;
    int localtime = 0;
    while (true) {
        localtime = pBox->mXmlConfig.vehicleConfig.localStorageCycle;
        std::this_thread::sleep_for(std::chrono::milliseconds(localtime));
        if (pBox->mSave7DayQueue.getQueueSize() != 0) {
            //saveDataMessage = pBox->mSave7DayQueue.getMsgNoPop();
            saveDataMessage = pBox->mSave7DayQueue.dequeue();
            pBox->mBinaryMessage.appendSaveMessage(saveDataMessage);
            //pBox->mBinaryMessage.checkAndDeleteFile();
            if(pBox->network == 1){
                pBox->mBinaryMessage.deleteMessageFileBefor7Days();
            }
        }
        //        saveDataMessage = pBox->mSaveQueue.dequeue();
        //        pBox->mBinaryMessage.appendMessage(saveDataMessage);
        // 离线保存消息
    }
}
void VehicleTask(Tbox* pBox) {
    //    pBox->mVehicleCtl.Start();
    while (true) {
        LOG(INFO) << "VehicleTask: " << pBox;
        for (auto pair : pBox->mTboxDecl) {
        }
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }
}
int addconfigfile() {
    std::string sourceFileName = "/vendor/etc/gecko-tbox-config.xml";        // 源文件名
    std::string targetFileName = "/data/vendor/tbox/gecko-tbox-config.xml";  // 目标文件名
    char propV[PROPERTY_VALUE_MAX_LEN] = "\0";
    if(property_get("gecko.variant.tspsite", propV, nullptr)!=0){
        std::string value = propV;
        if(value == "0"){
            sourceFileName = "/vendor/etc/gecko-tbox-config-bjt.xml";
            LOG(INFO) << "XMLConfig : using bjt config, value = 00";
        } else if(value == "1"){
            sourceFileName = "/vendor/etc/gecko-tbox-config-dst.xml";
            LOG(INFO) << "XMLConfig :using DST config, value = 01";
        } else if (value == "2"){
            sourceFileName = "/vendor/etc/gecko-tbox-config-dst.xml";
            LOG(INFO) << "XMLConfig :using DST config, value = 02";
        } else {
            sourceFileName = "/vendor/etc/gecko-tbox-config-dst.xml";
            LOG(INFO) << "XMLConfig :using DST config, value = " << value;
        }
    } else {
        LOG(INFO) << "XMLConfig ERROR : can not get config: gecko.variant.tspsite, using default!!!";
    }

    // 使用文件系统库检查目标文件是否存在
    std::ifstream file(targetFileName);
    if (file.good()) {
        LOG(INFO) << "Target file exists.";
        file.close();
        return 0;
    } else {
        LOG(INFO) << "Target file does not exist.";
    }

    // 打开源文件和目标文件
    std::ifstream sourceFile(sourceFileName);
    std::ofstream targetFile(targetFileName);

    if (!sourceFile.is_open() || !targetFile.is_open()) {
        //std::cerr << "Failed to open files." << std::endl;
        LOG(INFO) << "Failed to open files.";
        return 1;
    }

    // 从源文件读取内容并写入目标文件
    std::string line;
    while (std::getline(sourceFile, line)) {
        targetFile << line << std::endl;
    }

    // 关闭文件
    sourceFile.close();
    targetFile.close();
    //std::cout << "File copied successfully." << std::endl;
    LOG(INFO) << "File copied successfully.";
    return 0;
}

// int main(int argc, char* argv[]) {
int main() {
    struct sigaction sa;
    sa.sa_handler = &handle_sigterm;
    /*char propValue[PROPERTY_VALUE_MAX_LEN] = "\0";
    std::string vehcfg = "00";
    memset(propValue, 0, sizeof(propValue));
    if(property_get("persist.gecko.vehcfg", propValue, nullptr)!=0){
        vehcfg = propValue;
        LOG(INFO) << __func__ << "vehcfg is "<< vehcfg;
        if(vehcfg == "01" || vehcfg == "02"){
            LOG(INFO) << __func__ <<"aboard version, donot start TBOX process";
            std::this_thread::sleep_for(std::chrono::seconds(120));
            return 0;
        }
    } else {
        LOG(INFO) << __func__ <<" cannot get vehcfg, donot start TBOX process";
        std::this_thread::sleep_for(std::chrono::seconds(120));
        return 0;
    }*/

    Tbox mTbox;
    mTbox.init();
    /*if (mTbox.mMessageData.mUniqueID.empty()) {
        std::this_thread::sleep_for(std::chrono::seconds(10));
        LOG(INFO) << "ERROR : NO VIN, Tbox init config fail, return!!!!!!!!!!!!!!!!!!!!!";
        return 0;
    }*/
    mTbox.mVehicleStatusInfo.initVehicleStatusInfoTask();
    addconfigfile();
    int ret = mTbox.mXmlConfig.vehicleConfigInit();
#ifdef EV48
    mTbox.mCustomInfoFlags.updateConfig(mTbox.mXmlConfig.vehicleConfig.isSampling);
#endif
    if (ret == 0) {
        std::string filename = "/data/vendor/tbox/gecko-tbox-config.xml";
        std::remove(filename.c_str());
        std::this_thread::sleep_for(std::chrono::seconds(10));
        LOG(INFO) << "ERROR : Tbox init config fail, return!!!!!!!!!!!!!!!!!!!!!";
        return 0;
    }
//    std::this_thread::sleep_for(std::chrono::seconds(
//            mTbox.mXmlConfig.vehicleConfig.tboxservicestartwait));  // 等待整体服务ok后再启动功能
    //    std::this_thread::sleep_for(std::chrono::seconds(30));//等待整体服务ok后再启动功能
    std::vector<std::thread> mThreads;
    // 创建车机通讯任务
    mThreads.emplace_back(&VehicleTask, &mTbox);

    mTbox.mNetworkComm.setMachineState(&mTbox.mState);
    mTbox.mNetworkCommSample.setMachineState(&mTbox.mState);
    // 创建状态维护任务
    //    mThreads.emplace_back(std::bind(&StateTask));
    // 创建心跳任务
    mThreads.emplace_back(&HeartTask, &mTbox);
    // 创建采样链路心跳任务
    mThreads.emplace_back(&HeartTaskSample, &mTbox);
    // 创建通讯链路任务
//    mThreads.emplace_back(&CommunicationTask, &mTbox);
    // 创建监视任务
//    mTbox.mNetworkComm.pMsgQueue = &mTbox.mMsgQueue;
    mThreads.emplace_back(&WatchTask, &mTbox);
    mThreads.emplace_back(&WatchTaskSample, &mTbox);
    mThreads.emplace_back(&WatchSOCPowerStateTask, &mTbox);
    //mThreads.emplace_back(&signalWatchTask, &mTbox);
    mThreads.emplace_back(&realNameAuthStateWatchTask, &mTbox);
//    std::this_thread::sleep_for(std::chrono::seconds(1));
//    mTbox.mNetworkComm.pMsgQueue = &mTbox.mMsgQueue;
//    mTbox.mNetworkComm.Start(&(mTbox.mXmlConfig));
    // 创建发送任务
    mThreads.emplace_back(&SendTask, &mTbox);
    mThreads.emplace_back(&SendTaskSample, &mTbox);
    // 创建接收任务
    mThreads.emplace_back(&RecvTask, &mTbox);
    mThreads.emplace_back(&RecvTaskSample, &mTbox);
    // 报文生成任务
    mThreads.emplace_back(&MessageCreateTask, &mTbox);
    //
    mThreads.emplace_back(&MessageconsumerTask, &mTbox);
    mThreads.emplace_back(&MessageconsumerTaskSample, &mTbox);
    // 创建报文存储任务
    mThreads.emplace_back(&MessageSaveTask, &mTbox);
    mThreads.emplace_back(&MessageSaveTaskSample, &mTbox);
    mThreads.emplace_back(&MessageSave7DayTask, &mTbox);
    p_gTbox = &mTbox;
    sigaction(SIGUSR1, &sa, NULL);
    // 等待所有线程执行完毕
    for (auto& thread : mThreads) {
        thread.join();
    }
    return 0;
}
