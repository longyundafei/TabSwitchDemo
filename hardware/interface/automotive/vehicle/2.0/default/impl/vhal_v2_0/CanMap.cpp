#include "CanMap.h"
#include <log/log.h>
#include <sstream>
#include <iomanip>

#include <android-base/logging.h>

using namespace std;

CanMap* CanMap::instance = new (std::nothrow) CanMap();

CanMap* CanMap::getInstance()
{
    return instance;
}

void CanMap::deleteInstance()
{
    if (instance)
    {
        delete instance;
        instance = nullptr;
    }
}

void CanMap::insert_data(uint16_t messageID, std::vector<uint8_t>& dataContent, BusIdentifier busIdentifier) {
    size_t idx = getBucketIndex(messageID);
    auto& bucket = buckets[idx];

    std::lock_guard<std::mutex> lock(bucket.mt);
    can_data tempData;
    tempData.messageID = messageID;
    tempData.dataContent = dataContent;
    tempData.busIdentifier = busIdentifier;
    tempData.updateFlag = 1;
    tempData.lastRecvTime = std::chrono::steady_clock::now();

    bucket.can_map[messageID] = tempData;
}

bool CanMap::get_data(can_data& data) {
    for (auto& bucket : buckets) {
        std::lock_guard<std::mutex> lock(bucket.mt);
        for (auto& it : bucket.can_map) {
            if (it.second.updateFlag == 1) {
                it.second.updateFlag = 0;
                data = it.second;
                return true;
            }
        }
    }
    return false;
}

void CanMap::checkTimeouts(std::function<void(uint16_t, BusIdentifier)> onTimeout, std::chrono::milliseconds timeoutMs) {
    auto now = std::chrono::steady_clock::now();
    for (auto& bucket : buckets) {
        std::lock_guard<std::mutex> lock(bucket.mt);
        for (auto& it : bucket.can_map) {
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - it.second.lastRecvTime);
            if (elapsed > timeoutMs) {
                onTimeout(it.first, it.second.busIdentifier);
            }
        }
    }
}