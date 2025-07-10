#include <queue>
#include <vector>
#include <mutex>
#include <map>
#include <condition_variable>

#include "MessageDefine.h"

struct can_data{
    uint16_t messageID {0};
    std::vector<uint8_t> dataContent {0};
    BusIdentifier busIdentifier;
    int32_t updateFlag {0};  //更新标识
};

class SendCanMap {
private:
    std::map<uint16_t, struct can_data> can_map;  
    std::mutex mt;
    std::condition_variable cv_;
    static SendCanMap* instance;
    SendCanMap(){}
    SendCanMap(const SendCanMap&) = delete;
    SendCanMap& operator=(const SendCanMap&) = delete;

    
public:
    static SendCanMap* getInstance();
    static void deleteInstance();
    //存入map
    void insert_data(uint16_t messageID, std::vector<uint8_t> dataContent, BusIdentifier busIdentifier);

    bool get_data(can_data& data);
    std::vector<uint8_t> demap_to_spi();
};

