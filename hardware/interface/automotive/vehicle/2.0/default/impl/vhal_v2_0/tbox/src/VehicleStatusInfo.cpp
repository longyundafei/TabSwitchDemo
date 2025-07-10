#include "VehicleStatusInfo.h"
#include <android-base/properties.h>

VehicleStatusInfo::VehicleStatusInfo(VehicleController& vehicle) {
    mVehicle = vehicle;
}
int VehicleStatusInfo::getpacktotalcellnumValue(){
    //LOG(INFO) << __func__;
    StatusCode refStatus;
    VehiclePropValue refValue;
    mVehicle.getPropertybyPropId((int)VehicleProperty::GECKO_BMS_PACKTOTALCELLNUM, 0, refValue, refStatus);
    int totalCellNum = refValue.value.int32Values[0];
    //LOG(INFO) << __func__ << " totalCellNum = " << totalCellNum;
    return totalCellNum;
}
int VehicleStatusInfo::getpacktotaltempnumValue(){
    //LOG(INFO) << __func__;
    StatusCode refStatus;
    VehiclePropValue refValue;
    mVehicle.getPropertybyPropId((int)VehicleProperty::GECKO_BMS_PACKTOTALTEMPNUM, 0, refValue, refStatus);
    int totalTempNum = refValue.value.int32Values[0];
    //LOG(INFO) << __func__ << " cellCount = " << cellCount;
    return totalTempNum;
}
int VehicleStatusInfo::getframeCellCountValue() {
    //LOG(INFO) << __func__;
    StatusCode refStatus;
    VehiclePropValue refValue;
    mVehicle.getPropertybyPropId((int)VehicleProperty::GECKO_B2V_V_FRAMENO, 0, refValue, refStatus);
    int cellCount = refValue.value.int32Values[0];
    //LOG(INFO) << __func__ << " cellCount = " << cellCount;
    return cellCount;
}
int VehicleStatusInfo::getvoltage1Value() {
    //LOG(INFO) << __func__;
    StatusCode refStatus;
    VehiclePropValue refValue;
    mVehicle.getPropertybyPropId((int)VehicleProperty::GECKO_B2V_V_CELL_VOLTN1, 0, refValue, refStatus);
    int voltage1 = refValue.value.int32Values[0];
    //LOG(INFO) << __func__ << " voltage1 = " << voltage1;
    return voltage1;
}
int VehicleStatusInfo::getvoltage2Value() {
    //LOG(INFO) << __func__;
    StatusCode refStatus;
    VehiclePropValue refValue;
    mVehicle.getPropertybyPropId((int)VehicleProperty::GECKO_B2V_V_CELL_VOLTN2, 0, refValue, refStatus);
    int voltage2 = refValue.value.int32Values[0];
    //LOG(INFO) << __func__ << " voltage2 = " << voltage2;
    return voltage2;
}
int VehicleStatusInfo::getvoltage3Value() {
    //LOG(INFO) << __func__;
    StatusCode refStatus;
    VehiclePropValue refValue;
    mVehicle.getPropertybyPropId((int)VehicleProperty::GECKO_B2V_V_CELL_VOLTN3, 0, refValue, refStatus);
    int voltage3 = refValue.value.int32Values[0];
    //LOG(INFO) << __func__ << " voltage3 = " << voltage3;
    return voltage3;
}
int VehicleStatusInfo::getrechargeableFaultCountValue() {
    //LOG(INFO) << __func__;
    StatusCode refStatus;
    VehiclePropValue refValue;
    mVehicle.getPropertybyPropId((int)VehicleProperty::GECKO_BMS_BATTERYDTC_NUM, 0, refValue, refStatus);
    int rechargeableFaultCount = refValue.value.int32Values[0];
    //LOG(INFO) << __func__ << " rechargeableFaultCount = " << rechargeableFaultCount;
    return rechargeableFaultCount;
}
int VehicleStatusInfo::getrechargeableFaultCodesValue() {
    //LOG(INFO) << __func__;
    StatusCode refStatus;
    VehiclePropValue refValue;
    mVehicle.getPropertybyPropId((int)VehicleProperty::GECKO_BMS_BATTERYDTC_LIST, 0, refValue, refStatus);
    int rechargeableFaultCode = refValue.value.int32Values[0];
    //LOG(INFO) << __func__ << " rechargeableFaultCode = " << rechargeableFaultCode;
    return rechargeableFaultCode;
}
#ifdef EV48
int VehicleStatusInfo::getdriveMotorFaultCountValue() {
    //LOG(INFO) << __func__;
    StatusCode refStatus;
    VehiclePropValue refValue;
    mVehicle.getPropertybyPropId((int)VehicleProperty::GECKO_MCU_MOTORFAULTTOTALNUM, 0, refValue, refStatus);
    int motorFaultCount = refValue.value.int32Values[0];
    //LOG(INFO) << __func__ << " motorFaultCount = " << motorFaultCount;
    return motorFaultCount;
}
#endif
int VehicleStatusInfo::getOtherFaultCountValue() {
    //LOG(INFO) << __func__;
    StatusCode refStatus;
    VehiclePropValue refValue;
    mVehicle.getPropertybyPropId((int)VehicleProperty::GECKO_VCU_ERRAMOUNT, 0, refValue, refStatus);
    int otherFaultCount = refValue.value.int32Values[0];
    //LOG(INFO) << __func__ << " motorFaultCount = " << motorFaultCount;
    return otherFaultCount;
}
int VehicleStatusInfo::getdriveMotorFaultCodesValue() {
    //LOG(INFO) << __func__;
    StatusCode refStatus;
    VehiclePropValue refValue;
    mVehicle.getPropertybyPropId((int)VehicleProperty::GECKO_INV_4_ERRBITS, 0, refValue, refStatus);
    int motorFaultCode = refValue.value.int32Values[0];
    //LOG(INFO) << __func__ << " motorFaultCode = " << motorFaultCode;
    return motorFaultCode;
}
int VehicleStatusInfo::getOtherFaultCodesValue() {
    //LOG(INFO) << __func__;
    StatusCode refStatus;
    VehiclePropValue refValue;
    mVehicle.getPropertybyPropId((int)VehicleProperty::GECKO_VCU_ERRBIT, 0, refValue, refStatus);
    int otherFaultCode = refValue.value.int32Values[0];
    //LOG(INFO) << __func__ << " otherFaultCode = " << otherFaultCode;
    return otherFaultCode;
}

int VehicleStatusInfo::gettemperatureFrameNo() {
    //LOG(INFO) << __func__;
    StatusCode refStatus;
    VehiclePropValue refValue;
    mVehicle.getPropertybyPropId((int)VehicleProperty::GECKO_B2V_T_FRAMENO, 0, refValue, refStatus);
    int temperatureFrameNo = refValue.value.int32Values[0];
    //LOG(INFO) << __func__ << " temperatureFrameNo = " << temperatureFrameNo;
    return temperatureFrameNo;
}
int VehicleStatusInfo::gettemperature1Value() {
    //LOG(INFO) << __func__;
    StatusCode refStatus;
    VehiclePropValue refValue;
    mVehicle.getPropertybyPropId((int)VehicleProperty::GECKO_B2V_T_CELL_TEMPN1, 0, refValue, refStatus);
    int temperature1 = refValue.value.int32Values[0];
    //LOG(INFO) << __func__ << " temperature1 = " << temperature1;
    return temperature1;
}
int VehicleStatusInfo::gettemperature2Value() {
    //LOG(INFO) << __func__;
    StatusCode refStatus;
    VehiclePropValue refValue;
    mVehicle.getPropertybyPropId((int)VehicleProperty::GECKO_B2V_T_CELL_TEMPN2, 0, refValue, refStatus);
    int temperature2 = refValue.value.int32Values[0];
    //LOG(INFO) << __func__ << " temperature2 = " << temperature2;
    return temperature2;
}
int VehicleStatusInfo::gettemperature3Value() {
    //LOG(INFO) << __func__;
    StatusCode refStatus;
    VehiclePropValue refValue;
    mVehicle.getPropertybyPropId((int)VehicleProperty::GECKO_B2V_T_CELL_TEMPN3, 0, refValue, refStatus);
    int temperature3 = refValue.value.int32Values[0];
    //LOG(INFO) << __func__ << " temperature3 = " << temperature3;
    return temperature3;
}
int VehicleStatusInfo::gettemperature4Value() {
    //LOG(INFO) << __func__;
    StatusCode refStatus;
    VehiclePropValue refValue;
    mVehicle.getPropertybyPropId((int)VehicleProperty::GECKO_B2V_T_CELL_TEMPN4, 0, refValue, refStatus);
    int temperature4 = refValue.value.int32Values[0];
    //LOG(INFO) << __func__ << " temperature4 = " << temperature4;
    return temperature4;
}
int VehicleStatusInfo::gettemperature5Value() {
    //LOG(INFO) << __func__;
    StatusCode refStatus;
    VehiclePropValue refValue;
    mVehicle.getPropertybyPropId((int)VehicleProperty::GECKO_B2V_T_CELL_TEMPN5, 0, refValue, refStatus);
    int temperature5 = refValue.value.int32Values[0];
    //LOG(INFO) << __func__ << " temperature5 = " << temperature5;
    return temperature5;
}
int VehicleStatusInfo::gettemperature6Value() {
    //LOG(INFO) << __func__;
    StatusCode refStatus;
    VehiclePropValue refValue;
    mVehicle.getPropertybyPropId((int)VehicleProperty::GECKO_B2V_T_CELL_TEMPN6, 0, refValue, refStatus);
    int temperature6 = refValue.value.int32Values[0];
    //LOG(INFO) << __func__ << " temperature6 = " << temperature6;
    return temperature6;
}
#ifdef EV48
int VehicleStatusInfo::gettemperature7Value() {
    //LOG(INFO) << __func__;
    StatusCode refStatus;
    VehiclePropValue refValue;
    mVehicle.getPropertybyPropId((int)VehicleProperty::GECKO_B2V_T_CELL_TEMPN7, 0, refValue, refStatus);
    int temperature7 = refValue.value.int32Values[0];
    //LOG(INFO) << __func__ << " temperature7 = " << temperature7;
    return temperature7;
}
#endif
int VehicleStatusInfo::getBMSErrTabIndexValue() {
    //LOG(INFO) << __func__;
    StatusCode refStatus;
    VehiclePropValue refValue;
    mVehicle.getPropertybyPropId((int)VehicleProperty::GECKO_BMS_ERR_TAB_INDEX, 0, refValue, refStatus);
    int errTabIndex = refValue.value.int32Values[0];
    //LOG(INFO) << __func__ << " temperature7 = " << temperature7;
    return errTabIndex;
}

int VehicleStatusInfo::initVehicleStatusInfoTask() {
    if(mThreads.size() == 0){
        LOG(INFO) << __func__ << "mThreads.size() == 0";
        //mThreads.emplace_back(std::bind(&VehicleStatusInfo::updateVehicleStatusInfoThread, this));
        mThreads.emplace_back(std::bind(&VehicleStatusInfo::updateCellVoltageThread, this));
        mThreads.emplace_back(std::bind(&VehicleStatusInfo::updateProbTemperatureThread, this));
        mThreads.emplace_back(std::bind(&VehicleStatusInfo::updateRechargeableFaultCodesThread, this));
        mThreads.emplace_back(std::bind(&VehicleStatusInfo::updatedriveMotorFaultCodesThread, this));
        mThreads.emplace_back(std::bind(&VehicleStatusInfo::updateOtherFaultCodesThread, this));
        mThreads.emplace_back(std::bind(&VehicleStatusInfo::updateBMSErrorTabThread, this));
        for (auto& thread : mThreads) {
            thread.detach();
        }
        return 0;
    } else {
        LOG(INFO) << __func__ << "mThreads.size() != 0";
        return 0;
    }
}
void VehicleStatusInfo::updateCellVoltageThread(){
    while(totalCellNum == 0){
        totalCellNum = getpacktotalcellnumValue();
        LOG(INFO) << "getting totalCellNum" << totalCellNum;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    LOG(INFO) << __func__ << " totalCellNum = " << totalCellNum;
    cellVotageList.resize(totalCellNum);
    int iterateCellVotage = 0;
    if(totalCellNum%3){
        iterateCellVotage = totalCellNum/3 + 1;
    } else {
        iterateCellVotage = totalCellNum/3;
    }
    while(true) {
        int cellFrameNo = 0;
        for(auto i = 0; i<iterateCellVotage; i++) {
            cellFrameNo = getframeCellCountValue();
            if (cellFrameNo<0 || cellFrameNo>iterateCellVotage) {
                cellFrameNo = 0;
                LOG(INFO) << "BatteryVoltageMessage error: get wrong cellFrameNo";
            }
            int voltage1 = getvoltage1Value();
            if(voltage1 <= 5000){
                cellVotageList[cellFrameNo*3] = voltage1;
            }
            if((cellFrameNo*3+1) < totalCellNum){
                int voltage2 = getvoltage2Value();
                if(voltage2 <= 5000){
                    cellVotageList[cellFrameNo*3+1] = voltage2;
                }
            }
            if((cellFrameNo*3+2) < totalCellNum){
                int voltage3 = getvoltage3Value();
                if(voltage3 <= 5000){
                    cellVotageList[cellFrameNo*3+2] = voltage3;
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
}

void VehicleStatusInfo::updateProbTemperatureThread(){
    while(probNum == 0){
        probNum = getpacktotaltempnumValue();
        LOG(INFO) << "getting probNum";
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    probeTemperatureList.resize(probNum);
    int iterateProbTemperature = 0;
    if (probNum%6){
        iterateProbTemperature = probNum/6+1;
    } else {
        iterateProbTemperature = probNum/6;
    }
    while(true) {
       int temperatureFrameNo = 0;
       for (auto i=0; i<iterateProbTemperature; i++) {
            temperatureFrameNo = gettemperatureFrameNo();
            if (temperatureFrameNo<0 || temperatureFrameNo>iterateProbTemperature) {
                temperatureFrameNo = 0;
                LOG(INFO) << "ProbeTemperatureMessage error: get wrong temperatureFrameNo";
            }
            probeTemperatureList[temperatureFrameNo*6] = gettemperature1Value();
            if((temperatureFrameNo*6+1) < probNum){
                probeTemperatureList[temperatureFrameNo*6+1] = gettemperature2Value();
            }
            if((temperatureFrameNo*6+2) < probNum){
                probeTemperatureList[temperatureFrameNo*6+2] = gettemperature3Value();
            }
            if((temperatureFrameNo*6+3) < probNum){
                probeTemperatureList[temperatureFrameNo*6+3] = gettemperature4Value();
            }
            if((temperatureFrameNo*6+4) < probNum){
                probeTemperatureList[temperatureFrameNo*6+4] = gettemperature5Value();
            }
            if((temperatureFrameNo*6+5) < probNum){
                probeTemperatureList[temperatureFrameNo*6+5] = gettemperature6Value();
            }
            /*if((temperatureFrameNo+6) < probNum){
                probeTemperatureList[temperatureFrameNo+6] = gettemperature7Value();
            }*/
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(800));
    }

}

/*bms_faultcode cycle: 100ms,each error code report 1 time,100ms
 * rechargeable fault codes collect
 */
void VehicleStatusInfo::updateRechargeableFaultCodesThread(){
    uint32_t rechargeableFaultCode = 0;
    std::vector<int> tempRechargeableFaultList;
    while(true) {
        tempRechargeableFaultList.resize(0);
        int count = getrechargeableFaultCountValue();
        int size = 0;
        long long start = 0;
        long long end = 0;
        struct timeval tv;
        gettimeofday(&tv, NULL);
        start = tv.tv_sec*1000LL+tv.tv_usec/1000;
        for(auto i = 0; size<count; i++){
            rechargeableFaultCode = getrechargeableFaultCodesValue();
            auto ret = find(tempRechargeableFaultList.begin(), tempRechargeableFaultList.end(), rechargeableFaultCode);
            if ((rechargeableFaultCode != 0) && (ret == tempRechargeableFaultList.end())) {
                LOG(INFO) << "Got a new rechargeableFaultCode, add it to templist : " << rechargeableFaultCode;
                tempRechargeableFaultList.push_back(rechargeableFaultCode);
            }
            size = static_cast<int>(tempRechargeableFaultList.size());
            gettimeofday(&tv, NULL);
            end = tv.tv_sec*1000LL+tv.tv_usec/1000;
            int delay = static_cast<int>(end - start);
            if(delay > (mSleepTime*1000-25)){
                LOG(INFO) << "into next cycle:tempRechargeableFaultList, break; size = " << size;
                break;
            }
            count = getrechargeableFaultCountValue();
            std::this_thread::sleep_for(std::chrono::milliseconds(25));
        }
        mutexUpdateRechargeableFault.lock();
        rechargeableFaultList.resize(0);
        rechargeableFaultList.assign(tempRechargeableFaultList.begin(), tempRechargeableFaultList.end());
        mutexUpdateRechargeableFault.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(25));
    }
}

/*mcu_error_code cycle:10ms, each error code report 50 times, 500ms.
 * motor fault codes collect
 */
void VehicleStatusInfo::updatedriveMotorFaultCodesThread(){
    uint32_t driveMotorFaultCode = 0;
    std::vector<int> tempDriveMotorFaultList;
    while(true) {
        tempDriveMotorFaultList.resize(0);
        //int count = getdriveMotorFaultCountValue();
        int count = mSleepTime*20;
        int size = 0;
        /*long long start = 0;
        long long end = 0;
        struct timeval tv;
        gettimeofday(&tv, NULL);
        start = tv.tv_sec*1000LL+tv.tv_usec/1000;*/
        for(auto i = 0; i<count; i++){
            driveMotorFaultCode = getdriveMotorFaultCodesValue();
            auto ret = find(tempDriveMotorFaultList.begin(), tempDriveMotorFaultList.end(), driveMotorFaultCode);
            if ((driveMotorFaultCode != 0) && (ret == tempDriveMotorFaultList.end())){
                LOG(INFO) << "Got a new driveMotorFaultCode, add it to templist : " << driveMotorFaultCode;
                tempDriveMotorFaultList.push_back(driveMotorFaultCode);
            }
            /*size = static_cast<int>(tempDriveMotorFaultList.size());
            gettimeofday(&tv, NULL);
            end = tv.tv_sec*1000LL+tv.tv_usec/1000;
            int delay = static_cast<int>(end - start);
            if (delay > (mSleepTime*1000-50)){
                LOG(INFO) << "into next cycle:tempDriveMotorFaultList, break; size = " << size;
                break;
            }*/
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
        size = static_cast<int>(tempDriveMotorFaultList.size());
        LOG(INFO) << "into next cycle:tempDriveMotorFaultList, break; size = " << size;
        mutexUpdateDriveMotorFault.lock();
        driveMotorFaultList.resize(0);
        driveMotorFaultList.assign(tempDriveMotorFaultList.begin(), tempDriveMotorFaultList.end());
        mutexUpdateDriveMotorFault.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

/*vcu_fault_code, cycle: 100ms, each error code report 20 times ,2s
 * other fault codes collect
 */
void VehicleStatusInfo::updateOtherFaultCodesThread(){
    uint32_t otherFaultCode = 0;
    std::vector<int> tempOtherFaultList;
    while(true) {
        tempOtherFaultList.resize(0);
        int count = getOtherFaultCountValue();
        int size = 0;
        long long start = 0;
        long long end = 0;
        struct timeval tv;
        gettimeofday(&tv, NULL);
        start = tv.tv_sec*1000LL+tv.tv_usec/1000;
        for(auto i = 0; i<count; i++){
            otherFaultCode = getOtherFaultCodesValue();
            auto ret = find(tempOtherFaultList.begin(), tempOtherFaultList.end(), otherFaultCode);
            if ((otherFaultCode != 0) && (ret == tempOtherFaultList.end())) {
                LOG(INFO) << "Got a new otherFaultCode, add it to templist : " << otherFaultCode;
                tempOtherFaultList.push_back(otherFaultCode);
            }
            size = tempOtherFaultList.size();
            gettimeofday(&tv, NULL);
            end = tv.tv_sec*1000LL+tv.tv_usec/1000;
            int delay = static_cast<int>(end - start);
            if (delay > (mSleepTime*1000-500)){
                LOG(INFO) << "into next cycle:tempOtherFaultList break; size = " << size;
                break;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
        mutexUpdateOtherFault.lock();
        otherFaultList.resize(0);
        otherFaultList.assign(tempOtherFaultList.begin(), tempOtherFaultList.end());
        mutexUpdateOtherFault.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}


void VehicleStatusInfo::updateBMSErrorTabThread(){
    uint16_t errorTabIndex = 0;
    std::vector<uint16_t> tempErrTabIndexList;
    while(true) {
        tempErrTabIndexList.resize(0);
        int count = mSleepTime*10;
        int size = 0;
        long long start = 0;
        long long end = 0;
        struct timeval tv;
        gettimeofday(&tv, NULL);
        start = tv.tv_sec*1000LL+tv.tv_usec/1000;
        for(auto i = 0; i<count; i++){
            errorTabIndex = getBMSErrTabIndexValue();
            auto ret = find(tempErrTabIndexList.begin(), tempErrTabIndexList.end(), errorTabIndex);
            if ((errorTabIndex != 0) && (ret == tempErrTabIndexList.end())) {
                LOG(INFO) << "Got a new errorTabIndex, add it to errTabIndexList : " << errorTabIndex;
                tempErrTabIndexList.push_back(errorTabIndex);
            }
            size = tempErrTabIndexList.size();
            gettimeofday(&tv, NULL);
            end = tv.tv_sec*1000LL+tv.tv_usec/1000;
            int delay = static_cast<int>(end - start);
            if (delay > (mSleepTime*1000-100)){
                LOG(INFO) << "into next cycle:errTabIndexList break; size = " << size;
                break;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        mutexUpdateErrTabIndex.lock();
        errTabIndexList.resize(0);
        errTabIndexList.assign(tempErrTabIndexList.begin(), tempErrTabIndexList.end());
        mutexUpdateErrTabIndex.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}