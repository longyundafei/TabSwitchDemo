#include <queue>
#include <vector>
#include <mutex>
#include <map>
#include <condition_variable>

#include "MessageDefine.h"
#include <unordered_map>
#include <chrono>
#include <array>
constexpr size_t BUCKET_COUNT = 32; // 可根据实际情况调整
struct can_data{
    uint16_t messageID {0};
    std::vector<uint8_t> dataContent {0};
    BusIdentifier busIdentifier;
    int32_t updateFlag {0};  //更新标识
    std::chrono::steady_clock::time_point lastRecvTime;
};

class CanMap {
private:
    std::map<uint16_t, struct can_data> can_map;  
    std::mutex mt;
    std::condition_variable cv_;
    static CanMap* instance;
    CanMap(){}
    CanMap(const CanMap&) = delete;
    CanMap& operator=(const CanMap&) = delete;
    struct Bucket {
        std::mutex mt;
        std::unordered_map<uint16_t, can_data> can_map;
    };
    std::array<Bucket, BUCKET_COUNT> buckets;

    size_t getBucketIndex(uint16_t messageID) const {
        return std::hash<uint16_t>{}(messageID) % BUCKET_COUNT;
    }
    
public:
    static CanMap* getInstance();
    static void deleteInstance();
    //存入map
    void insert_data(uint16_t messageID, std::vector<uint8_t>& dataContent, BusIdentifier busIdentifier);

    bool get_data(can_data& data);

    // 检查超时并回调
    void checkTimeouts(std::function<void(uint16_t, BusIdentifier)> onTimeout, std::chrono::milliseconds timeoutMs);
};

