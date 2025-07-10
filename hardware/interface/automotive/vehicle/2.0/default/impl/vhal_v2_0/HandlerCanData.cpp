#include "HandlerCanData.h"
#include <log/log.h>
#include <thread>
#include <sstream>
#include <iomanip>
#include "CanMap.h"
#include "DebugManager.h"
#include <android-base/logging.h>
#include <set>
#include <utility>

using namespace std;

HandlerCanData* HandlerCanData::instance = new (std::nothrow) HandlerCanData();

HandlerCanData* HandlerCanData::getInstance()
{
    return instance;
}

void HandlerCanData::deleteInstance()
{
    if (instance)
    {
        delete instance;
        instance = nullptr;
    }
}


// 只处理一次超时，直到数据恢复
static std::set<std::pair<uint16_t, int>> timeout_set;

void HandlerCanData::onCanTimeout(uint16_t messageID, BusIdentifier busIdentifier) {
    auto key = std::make_pair(messageID, static_cast<int>(busIdentifier));
    if (timeout_set.find(key) != timeout_set.end()) {
        // 已经超时过，忽略
        return;
    }
    // 首次超时，处理并记录
    timeout_set.insert(key);
    ALOGW("CAN Message Timeout: messageID=0x%x, busIdentifier=%d", messageID, busIdentifier);
    // TODO: Add further timeout handling logic as needed, e.g., call special unpack/parse/notify functions
}

// 恢复数据时调用此函数清除超时标记
void HandlerCanData::clearTimeoutFlag(uint16_t messageID, BusIdentifier busIdentifier) {
    auto key = std::make_pair(messageID, static_cast<int>(busIdentifier));
    timeout_set.erase(key);
}

void HandlerCanData::HandlerData()
{
    while(true){
        Handler_unpack();
    }
}

void HandlerCanData::Handler_unpack()
{ 
    if(nullptr == pPropManager){
        ALOGI("Handler_unpack in sleeping ");
        std::this_thread::sleep_for(std::chrono::milliseconds(48));
        return;
    }
    //去map取数据
    can_data candata;
    bool ret = CanMap::getInstance()->get_data(candata);
    if(ret)
    {
        std::string buf = "";
	int len = candata.dataContent.size();
        for (int i = 0; i < len; i++) {
            std::ostringstream ss;
            ss.clear();
            int data = candata.dataContent[i];
            ss << std::setw(2) << std::setfill('0') << std::hex << data;
            buf += "0x";
            buf += ss.str();
            buf += " ";
        }
        if (DebugManager::getInstance().isOpenDebug()) {
            ALOGI("Handler_unpack messageID:%x  busid %d buffer:%s", candata.messageID, candata.busIdentifier, buf.c_str());
        }
        //解包
        char data[1024];
        pfunctionMap->unpackcallFunction(candata.messageID, candata.dataContent, static_cast<void *>(data), candata.busIdentifier, *pPropManager);
        // 报文已恢复，清除超时标记
        clearTimeoutFlag(candata.messageID, candata.busIdentifier);
    }else{
        std::this_thread::sleep_for(std::chrono::milliseconds(48));
    }

}

