#include <queue>
#include <vector>
#include <mutex>
#include <map>
#include <condition_variable>

#include "MessageDefine.h"
#include "FunctionMap.h"
#include "property_manager.h"

class FunctionMap;
class HandlerCanData {
private:
    static HandlerCanData* instance;
    android::hardware::automotive::vehicle::V2_0::impl::FunctionMap* pfunctionMap = nullptr;
    android::hardware::automotive::vehicle::V2_0::impl::PropertyManager* pPropManager = nullptr;
    HandlerCanData(){}
    HandlerCanData(const HandlerCanData&) = delete;
    HandlerCanData& operator=(const HandlerCanData&) = delete;

    void Handler_unpack();
public:
    static HandlerCanData* getInstance();
    static void deleteInstance();

    //定时解析报文
    void HandlerData();

    // 超时处理接口
    void onCanTimeout(uint16_t messageID, BusIdentifier busIdentifier);
    // 恢复数据时调用，清除超时标记
    void clearTimeoutFlag(uint16_t messageID, BusIdentifier busIdentifier);

    void initFunctionMap(android::hardware::automotive::vehicle::V2_0::impl::FunctionMap* functionMap){
        if(functionMap){
            pfunctionMap = functionMap;
        }
    };

    void initPropManager(android::hardware::automotive::vehicle::V2_0::impl::PropertyManager* propManager){
        if(propManager){
            pPropManager = propManager;
        }
    };

};

