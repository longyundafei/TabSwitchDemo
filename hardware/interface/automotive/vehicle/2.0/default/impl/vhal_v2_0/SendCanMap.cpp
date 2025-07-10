#include "SendCanMap.h"
#include <log/log.h>
#include <sstream>
#include <iomanip>

#include <android-base/logging.h>

using namespace std;

SendCanMap* SendCanMap::instance = new (std::nothrow) SendCanMap();

SendCanMap* SendCanMap::getInstance()
{
    return instance;
}

void SendCanMap::deleteInstance()
{
    if (instance)
    {
        delete instance;
        instance = nullptr;
    }
}

void SendCanMap::insert_data(uint16_t messageID, std::vector<uint8_t> dataContent, BusIdentifier busIdentifier)
{
    std::lock_guard<std::mutex> lock(mt);
    struct can_data tempData;    
    tempData.messageID = messageID;
    tempData.dataContent = dataContent;
    tempData.busIdentifier = busIdentifier;
    tempData.updateFlag = 1;
    if (((messageID == 0x353) || (messageID == 0x354))&& busIdentifier == BusIdentifier::BODY_CAN) {
        can_map[0x100 + messageID] = tempData;
    } else {
        can_map[messageID] = tempData;
    }
}

bool SendCanMap::get_data(can_data& data)
{
    std::lock_guard<std::mutex> lock(mt);

    for(auto& it : can_map)
    {
        if(it.second.updateFlag == 1)
        {
            it.second.updateFlag = 0;
            data = it.second;
            return true;
        }
    }
    return false;
}



std::vector<uint8_t> SendCanMap::demap_to_spi() {
     std::lock_guard<std::mutex> lock(mt);
     std::vector<uint8_t> msg;
     std::vector<uint8_t> msg_it;
     std::vector<uint8_t> temp_msg = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
     std::string buf;
     for(auto& it : can_map) {
         if(it.second.updateFlag == 1) {
             it.second.updateFlag = 0;
             msg_it = it.second.dataContent;
             msg.insert(msg.end(), msg_it.begin(), msg_it.end());
             buf = "";
             for(auto& content : msg_it){
                 std::ostringstream ss;
                 ss.clear();
                 int data = static_cast<int>(content);
                 ss << std::hex << data;
                 buf += "0x";
                 buf += ss.str();
                 buf += ", ";   
             }

             std::stringstream meId;
             meId << std::hex << it.second.messageID;
             std::string str = meId.str();
             ALOGI("demap_to_spibuffer %s = %s", str.c_str(), buf.c_str());   
	    }
     }
     int size = msg.size()/17;
     ALOGI("demap_to_spi msg.size %d ", size);
     for (int i = size; i< 60; i++) {
         msg.insert(msg.end(), temp_msg.begin(), temp_msg.end());
     }
     return msg;
}

