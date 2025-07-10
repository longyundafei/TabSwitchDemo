#ifndef RECORDDATA_H
#define RECORDDATA_H

#include <queue>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>

#include "MessageDefine.h"

class RecordData {
private:
    std::vector<std::vector<uint8_t>> EVCAN_data_;
    std::vector<std::vector<uint8_t>> Chassis_CAN_data_;
    std::vector<std::vector<uint8_t>> Body_CAN_data_;
    std::vector<std::vector<uint8_t>> MCU_data_;
    bool mRecord = false;
    static RecordData* instance;
    std::unique_ptr<std::thread> mFileThread;
    std::mutex mutex_;
    std::condition_variable cv_;

    RecordData(){
        init();
    }
    RecordData(const RecordData&) = delete;
    RecordData& operator=(const RecordData&) = delete;

    void init();
    void stop();
    void writeCanData(std::string fileName, BusIdentifier busIdentifier);
    void initSigaction();
    void setRecordTimer(int32_t sec, int32_t intervalSec);
    static void timeUp(int signo);
public:
    static RecordData* getInstance();
    static void deleteInstance();
    void startRecord(int32_t recordSeconds, int32_t intervalSec);
    void stopRecord();
    void recordEVCAN(const std::vector<uint8_t> message);
    void recordChassis_CAN(const std::vector<uint8_t> message);
    void recordBody_CAN(const std::vector<uint8_t> message);
    void recordMCU(const std::vector<uint8_t> message);
    void tarFile();
    void cleanData();
    void handleFile();
};

#endif  // RECORDDATA_H
