#include <sstream>
#include <fstream>
#include <string>
#include <linux/timerfd.h>
#include <thread>
#include <log/log.h>
#include <sys/stat.h> 
#include <unistd.h> 
#include <iostream>
#include <stdio.h>
#include <sys/time.h>
#include <string.h>
#include <signal.h>
#include <time.h>

#include "RecordData.h"

RecordData* RecordData::instance = new (std::nothrow) RecordData();

RecordData* RecordData::getInstance()
{
    return instance;
}

void RecordData::deleteInstance()
{
    instance->stop();
    if (instance)
    {
        delete instance;
        instance = nullptr;
    }

    
}

void RecordData::init()
{
    initSigaction();
    mFileThread = std::make_unique<std::thread>(std::bind(&RecordData::handleFile, this));
}

void RecordData::stop()
{
    if (mFileThread->joinable()) {
        mFileThread->join();
    }
}

void RecordData::timeUp(int signo){
    ALOGD("RecordData timer fired %d\n",signo);
    instance->stopRecord();
}

void RecordData::initSigaction(){
    struct sigaction act;
    act.sa_handler = timeUp;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask); 
    sigaction(SIGPROF,&act,NULL); 
}


void RecordData::setRecordTimer(int32_t sec, int32_t intervalSec) { 
    struct itimerval value; 
    value.it_value.tv_sec= sec;
    value.it_value.tv_usec= 0; 
    value.it_interval.tv_sec = intervalSec;
    value.it_interval.tv_usec = 0; 
    setitimer(ITIMER_PROF,&value,NULL);
}

void RecordData::startRecord(int32_t recordSeconds, int32_t intervalSec){
    mRecord = true;
    ALOGD("RecordData startRecord = %d\n", recordSeconds);
    setRecordTimer(recordSeconds, intervalSec); 

}

void RecordData::stopRecord(){
    mRecord = false;
    cv_.notify_one();
}

void RecordData::handleFile()
{
    while(true){
        std::unique_lock<std::mutex> lock(mutex_);
        cv_.wait(lock);
        writeCanData("BODY_CAN.txt", BusIdentifier::BODY_CAN);
        writeCanData("EV_CAN.txt", BusIdentifier::EV_CAN);
        writeCanData("CH_CAN.txt", BusIdentifier::CH_CAN);
        writeCanData("MCU.txt", BusIdentifier::MCU);
        tarFile();
        cleanData();
    }
}

void RecordData::recordEVCAN(const std::vector<uint8_t> message){
    if(mRecord){
        EVCAN_data_.push_back(message);
    }
}

void RecordData::recordChassis_CAN(const std::vector<uint8_t> message){
    if(mRecord){
        Chassis_CAN_data_.push_back(message);
    }
}

void RecordData::recordBody_CAN(const std::vector<uint8_t> message){
    if(mRecord){
        Body_CAN_data_.push_back(message);
    }
}

void RecordData::recordMCU(const std::vector<uint8_t> message) {
    if(mRecord){
        MCU_data_.push_back(message);
    }
}

void RecordData::writeCanData(std::string fileName, BusIdentifier busIdentifier){
    
    std::vector<std::vector<uint8_t>> * tmp = nullptr;
    switch(busIdentifier){
        case BusIdentifier::BODY_CAN:
            tmp = &Body_CAN_data_;
            //ALOGD("RecordData writeCanData Body_CAN_data_ size%d", static_cast<int>(Body_CAN_data_.size()));
            break;
        case BusIdentifier::EV_CAN:
            tmp = &EVCAN_data_;
            //ALOGD("RecordData writeCanData EVCAN_data_ size%d", static_cast<int>(EVCAN_data_.size()));
            break;
        case BusIdentifier::CH_CAN:
            tmp = &Chassis_CAN_data_;
            //ALOGD("RecordData writeCanData Chassis_CAN_data_ size%d", static_cast<int>(Chassis_CAN_data_.size()));
            break;
        case BusIdentifier::MCU:
            tmp = &MCU_data_;
            //ALOGD("RecordData writeCanData MCU_data_ size%d", static_cast<int>(MCU_data_.size()));
            break;
        default:  ALOGE("writeCanData err, Wrong  BUS id:%d", static_cast<int>(busIdentifier));;
    }

    std::string buf;
    for(auto iter : *tmp){
        std::vector<uint8_t> buffer = iter;
        for(int i = 0; i < static_cast<int>(buffer.size()); i++){
            std::ostringstream ss;
            ss.clear();
            int data = buffer[i];
            ss << std::setw(2) << std::setfill('0') << std::hex << data;
            buf += ss.str();
            buf += " ";
        }
        buf += "\n";
    }

    std::string ofstreamName = "/data/";
    ofstreamName += fileName;
    std::ofstream istrm(ofstreamName, std::ios::out | std::ios::ate | std::ios::in | std::ios::app);
    istrm << buf <<std::endl;
}

void RecordData::tarFile()
{
    system("tar -zcvf /data/log.tar.gz /data/BODY_CAN.txt /data/EV_CAN.txt /data/CH_CAN.txt /data/MCU.txt");
    system("rm -rf /data/BODY_CAN.txt /data/EV_CAN.txt /data/CH_CAN.txt /data/MCU.txt");
}

void RecordData::cleanData()
{
    Body_CAN_data_.clear();
    EVCAN_data_.clear();
    Chassis_CAN_data_.clear();
    MCU_data_.clear();
}


