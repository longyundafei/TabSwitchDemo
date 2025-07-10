#include "property_manager.h"
#include <log/log.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <android-base/logging.h>
#include "DebugManager.h"
namespace android::hardware::automotive::vehicle::V2_0::impl {
//PropertyManager::PropertyManager() {}
PropertyManager::PropertyManager()
    : can071_cv(new std::condition_variable()), can30A_cv(new std::condition_variable()), \
      can30B_cv(new std::condition_variable()), can30C_cv(new std::condition_variable()) {
}

void PropertyManager::addProperty(const Property& prop) {
    std::lock_guard<std::mutex> lock(mutex);
    //  properties[prop.name] = prop;
    //  properties.push_back(prop);
    properties.emplace_back(std::move(prop));
}

Property* PropertyManager::findProperty(const std::string& name) {
    std::lock_guard<std::mutex> lock(mutex);
    for (auto& p : properties) {
        if (p.name == name) {
            return &p;
        }
    }
    return nullptr;
}
std::string PropertyManager::getProValue(Property* mp) {
    if(!mp)
    {
        ALOGE("%s %d mp is null",__func__,__LINE__);
        return "";
    }

    std::lock_guard<std::mutex> lock(mutex);
    return mp->value;
}
void PropertyManager::updatePropertyValue(const std::string& name, const std::string& newValue) {
    for (auto& p : properties) {
        if (p.name == name) {
            std::unique_lock<std::mutex> lock(mutex);
            if (p.value != newValue) {
                p.value = newValue;
                p.cv->notify_one();
                return;
            }
        }
    }
}
ConfigDBC* PropertyManager::findCProperty(const std::string& name) {
    std::lock_guard<std::mutex> lock(mutex);
    for (auto& p : mpConfigParser->mConfigs) {
        for (auto& pp : p.canmsg) {
            if (pp.msgname == name) {
                return &(pp);
            }
        }
    }
    return nullptr;
}
uint32_t PropertyManager::getCProValue(ConfigDBC* mp) {
    if(!mp)
    {
        //ALOGE("%s %d mp is null",__func__,__LINE__);
        return  0;
    }

    std::lock_guard<std::mutex> lock(mutex);
    mp->updated = 0;
    return mp->value;
}
bool PropertyManager::getCProValue(ConfigDBC* mp,int32_t* value) {
    if(!mp)
    {
        *value = 0;
        return  false;
    }

    std::lock_guard<std::mutex> lock(mutex);
    if(1==mp->updated){
        mp->updated = 0;
        *value = mp->value;
        return true;
    }else{
        *value = 0;
        return false;
    }
}
std::string PropertyManager::getCProStrValue(ConfigDBC* mp) {
    std::lock_guard<std::mutex> lock(mutex);
    if(!mp)
    {
        ALOGE("%s %d mp is null",__func__,__LINE__);
        return  "";
    }
    return mp->str_value;
}

uint64_t PropertyManager::getTProValue(ConfigDBC* mp) {
    if(!mp)
    {
        ALOGE("%s %d mp is null",__func__,__LINE__);
        return  0;
    }

    std::lock_guard<std::mutex> lock(mutex);
    return mp->time_value;
}
/*
void PropertyManager::updateCPropertyValue(const std::string& name, uint32_t newValue) {
    for (auto& p : mpConfigParser->mConfigs) {
        for (auto& pp : p.canmsg) {
            if (pp.msgname.size() != 0) {
                //        ALOGE("%s %d %s %s",__func__,__LINE__, p.canmsg.msgname.c_str(),
                //        name.c_str());
                if (pp.msgname == name) {
//                    mpConfigParser->mcanConfMap[name]->value;
                    ALOGE("%s %d %p %p %s %s", __func__,__LINE__,mpConfigParser->mcanConfMap[name],&pp,mpConfigParser->mcanConfMap[name]->msgname.c_str(),pp.msgname.c_str());
                    std::unique_lock<std::mutex> lock(mutex);
                    if (pp.value != newValue) {
                        pp.value = newValue;
                        pp.cv->notify_one();
                        ALOGE("%s %d %s %s %d %d",__func__,__LINE__, pp.msgname.c_str(),
                            name.c_str(),pp.value,newValue);
                        //                ALOGE("%s %d %s", __func__,__LINE__,name.c_str());
                        return;
                    }
                }
            }
        }
    }
}*/
void PropertyManager::updateCPropertyValue(const std::string& name, uint32_t newValue) {
    //ALOGE("%s %d %p %s", __func__,__LINE__,mpConfigParser->mcanConfMap[name],mpConfigParser->mcanConfMap[name].msgname.c_str(),);
    auto pp = mpConfigParser->mcanConfMap[name];
    std::unique_lock<std::mutex> lock(mutex);
    if (pp->value != newValue) {
        if (DebugManager::getInstance().isOpenDebug()) {
            ALOGE("dxy------ noequal %s %d %s %s %d %d",__func__,__LINE__, pp->msgname.c_str(),
                name.c_str(),pp->value,newValue);
        }
        pp->value = newValue;
        pp->updated = 1;
        pp->counter++;
        if(pp->counter %100 == 0){
          ALOGE("%s %d %p %s %d", __func__,__LINE__,mpConfigParser->mcanConfMap[name],mpConfigParser->mcanConfMap[name]->msgname.c_str(),pp->counter);
        }
        pp->cv->notify_one();
        return;
    }
    // ALOGE("dxy----- equeal %s %d %s %s %d %d",__func__,__LINE__, pp->msgname.c_str(),
    //         name.c_str(),pp->value,newValue);
}

void PropertyManager::updateCPropertyValueNoUpdate(const std::string& name, uint32_t newValue) {
    //ALOGE("%s %d %p %s", __func__,__LINE__,mpConfigParser->mcanConfMap[name],mpConfigParser->mcanConfMap[name].msgname.c_str(),);
    auto pp = mpConfigParser->mcanConfMap[name];
    std::unique_lock<std::mutex> lock(mutex);
    if (pp->value != newValue) {
        ALOGE("dxy------ noequal %s %d %s %s %d %d",__func__,__LINE__, pp->msgname.c_str(),
            name.c_str(),pp->value,newValue);
        pp->value = newValue;
        pp->updated = 1;
        return;
    }
    // ALOGE("dxy----- equeal %s %d %s %s %d %d",__func__,__LINE__, pp->msgname.c_str(),
    //         name.c_str(),pp->value,newValue);
}

void PropertyManager::updateCPropertyValue(const std::string& name, std::string& newValue) {
    auto pp = mpConfigParser->mcanConfMap[name];
    std::unique_lock<std::mutex> lock(mutex);
    if (pp->str_value != newValue) {
        pp->str_value = newValue;
        pp->updated = 1;
        pp->counter++;
        if(pp->counter %100 == 0){
          ALOGE("%s %d %p %s %d", __func__,__LINE__,mpConfigParser->mcanConfMap[name],mpConfigParser->mcanConfMap[name]->msgname.c_str(),pp->counter);
        }
        pp->cv->notify_one();
        return;
    }
}

void PropertyManager::updateCPropertyValueNoCheck(const std::string& name, std::string& newValue) {
    auto pp = mpConfigParser->mcanConfMap[name];
    std::unique_lock<std::mutex> lock(mutex);
    //if (pp->str_value != newValue) {
        pp->str_value = newValue;
        pp->updated = 1;
        pp->counter++;
        if(pp->counter %100 == 0){
          ALOGE("%s %d %p %s %d", __func__,__LINE__,mpConfigParser->mcanConfMap[name],mpConfigParser->mcanConfMap[name]->msgname.c_str(),pp->counter);
        }
        pp->cv->notify_one();
        return;
    //}
}

void PropertyManager::updateTPropertyValue(const std::string& name, int64_t newValue) {
    //ALOGE("%s %d %p %s", __func__,__LINE__,mpConfigParser->mcanConfMap[name],mpConfigParser->mcanConfMap[name].msgname.c_str(),);
    auto pp = mpConfigParser->mcanConfMap[name];
    std::unique_lock<std::mutex> lock(mutex);
    if (pp->time_value != newValue) {
        pp->time_value = newValue;
        pp->cv->notify_one();
        LOG(INFO) << "time_value:"<< pp->time_value << ";newValue=" << newValue;
        return;
    }
}
void PropertyManager::updateCPropertyValueNoCheck(const std::string& name, int32_t newValue){
   // ALOGE("%s %d %p %s", __func__,__LINE__,mpConfigParser->mcanConfMap[name],mpConfigParser->mcanConfMap[name].msgname.c_str(),);
    auto pp = mpConfigParser->mcanConfMap[name];
    std::unique_lock<std::mutex> lock(mutex);
   // if (pp->value != newValue) {
        pp->value = newValue;
        pp->updated = 1;
        pp->cv->notify_one();
    //    ALOGE("%s %d %s %s %d %d",__func__,__LINE__, pp->msgname.c_str(),
    //        name.c_str(),pp->value,newValue);
        return;
   // }
}
/*
void PropertyManager::updateCAPropertyValue(const std::string& name, const std::string& newValue) {
    for (auto& p : mpConfigParser->mConfigs) {
        for (auto& pp : p.canmsg) {
            if (pp.msgname.size() != 0) {
                //        ALOGE("%s %d %s %s",__func__,__LINE__, p.canmsg.msgname.c_str(),
                //        name.c_str());
                if (pp.msgname == name) {
                    //        ALOGE("%s %d %s %s %d %d",__func__,__LINE__, p.canmsg.msgname.c_str(),
                    //        name.c_str(),p.canmsg.value,newValue);
                    for(auto&ppp : pp.valueMap){
                        ALOGE("%s %d ",__func__,__LINE__);
                        if(ppp.second == newValue){
                            if(ppp.first!=pp.value){
                                pp.value = ppp.first;
                                pp.cv->notify_one();
                                ALOGE("%s %d %s %s %d %s",__func__,__LINE__, pp.msgname.c_str(),
                                    name.c_str(),pp.value,newValue.c_str());

                                return;
                            }
                        }
                    }
                }
            }
        }
    }
}
*/
void PropertyManager::updateCAPropertyValue(const std::string& name, const std::string& newValue) {
    auto pp = mpConfigParser->mcanConfMap[name];
    //ALOGE("%s %d %s %s %d %d",__func__,__LINE__, p.canmsg.msgname.c_str(),
    //    name.c_str(),p.canmsg.value,newValue);
    for(auto&ppp : pp->valueMap){
        if(ppp.second == newValue){
            if(ppp.first!=pp->value){
                pp->value = ppp.first;
                pp->cv->notify_one();
                ALOGI("%s %d %s %s %d %s",__func__,__LINE__, pp->msgname.c_str(),
                    name.c_str(),pp->value,newValue.c_str());
                return;
            }
        }
    }
}

void PropertyManager::updateCPropertyValue(ConfigDBC* mp, uint32_t newValue) {
    if(!mp)
    {
        ALOGE("%s %d mp is null",__func__,__LINE__);
        return;
    }

    std::unique_lock<std::mutex> lock(mutex);
    if (mp->value != newValue) {
        mp->value = newValue;
        mp->updated = 1;
        mp->cv->notify_one();
        return;
    }
}

void PropertyManager::updateTPropertyValue(ConfigDBC* mp, int64_t newValue) {
    if(!mp)
    {
        ALOGE("%s %d mp is null",__func__,__LINE__);
        return;
    }

    std::unique_lock<std::mutex> lock(mutex);
    if (mp->time_value != newValue) {
        mp->time_value = newValue;
        mp->cv->notify_one();
        return;
    }
}

void PropertyManager::getUpdateCState(ConfigDBC* mp) {
    if(!mp)
    {
        //ALOGE("%s %d mp is null",__func__,__LINE__);
        return;
    }
    std::unique_lock<std::mutex> lock(mutex);
    mp->cv->wait(lock);
}

void PropertyManager::getCan071State() {
    std::unique_lock<std::mutex> lock(mutex);
    can071_cv->wait(lock);
}
void PropertyManager::setCan071update() {
    can071_cv->notify_one();
}
void PropertyManager::getCan30AState() {
    std::unique_lock<std::mutex> lock(mutex);
    can30A_cv->wait(lock);
}
void PropertyManager::setCan30Aupdate() {
    can30A_cv->notify_one();
}

void PropertyManager::getCan30BState() {
    std::unique_lock<std::mutex> lock(mutex);
    can30B_cv->wait(lock);
}
void PropertyManager::setCan30Bupdate() {
    can30B_cv->notify_one();
}

void PropertyManager::getCan30CState() {
    std::unique_lock<std::mutex> lock(mutex);
    can30C_cv->wait(lock);
}
void PropertyManager::setCan30Cupdate() {
    can30C_cv->notify_one();
}


void PropertyManager::getUpdateTState(ConfigDBC* mp) {
    if(!mp)
    {
        ALOGE("%s %d mp is null",__func__,__LINE__);
        return;
    }

    std::unique_lock<std::mutex> lock(mutex);
    mp->cv->wait(lock);
}
void PropertyManager::updatePropertyValue(Property* mp, const std::string& newValue) {
    if(!mp)
    {
        ALOGE("%s %d mp is null",__func__,__LINE__);
        return;
    }

    std::unique_lock<std::mutex> lock(mutex);
    if (mp->value != newValue) {
        mp->value = newValue;
        mp->cv->notify_one();
        return;
    }
}
void PropertyManager::getUpdateState(Property* mp) {
    if(!mp)
    {
        ALOGE("%s %d mp is null",__func__,__LINE__);
        return;
    }

    std::unique_lock<std::mutex> lock(mutex);
    mp->cv->wait(lock);
}
void PropertyManager::updatePropertyValues() {
    // 更新属性值实现
}

size_t PropertyManager::getPropertyCount() {
    std::lock_guard<std::mutex> lock(mutex);
    return properties.size();
}

void PropertyManager::printProperties() {
    std::lock_guard<std::mutex> lock(mutex);

    for (const auto& prop : properties) {
        //    const Property& prop = pair.second;
        std::cout << "Name: " << prop.name;
        std::cout << " Value: " << prop.value;
        std::cout << " Original Type: " << prop.originalType;
        std::cout << " Updated: " << prop.updated << std::endl;
    }
}
void PropertyManager::setmpConfigParser(ConfigParser* mp) {
    mpConfigParser = mp;
}

void PropertyManager::importPropertiesFromFile(const std::string& filename,
                                               PropertyManager& manager) {
    std::ifstream file(filename);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string name;
            std::string originalType;
            if (std::getline(ss, name, ';') && std::getline(ss, originalType)) {
                // 移除字段中的空格
                name.erase(std::remove_if(name.begin(), name.end(), ::isspace), name.end());
                originalType.erase(
                        std::remove_if(originalType.begin(), originalType.end(), ::isspace),
                        originalType.end());

                // 创建属性并添加到管理器
                Property prop;
                prop.name = name;
                prop.originalType = originalType;
                manager.addProperty(prop);
            }
        }
        file.close();
    }
}
void PropertyManager::setspiQueue(SendQueueV1_1* p){
    mpSpi = p;
}

void PropertyManager::stopJsonQueueEnqueue(){
	jsonQueueEnqueueFlag = 1;
	return;
}

void PropertyManager::resumeJsonQueueEnqueue(){
	jsonQueueEnqueueFlag = 0;
	return;
}

void PropertyManager::jsonQueueEnqueue(const std::vector<uint8_t>& message){
	if(jsonQueueEnqueueFlag == 0)
	{
		jsonQueue.enqueue(message);
	}else{
		//stop json msg in spi transfer;
	}
}
std::vector<uint8_t> PropertyManager::jsonQueuedequeue(){
    return jsonQueue.dequeue();
}

void PropertyManager::singleJsonEnQueue(std::string& key, std::string& value){
     Json::Value root;
     root[key] = value;
     // 输出JSON字符串
     Json::StreamWriterBuilder builder;
     builder["indentation"] = ""; // 设置缩进为空字符串，即不缩进
     std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
     std::ostringstream oss;
     writer->write(root, &oss);
     // 获取字符串
     std::string str = oss.str();
     // 将字符串转换为 std::vector<uint8_t>
     std::vector<uint8_t> vec(str.begin(), str.end());
     ALOGD("singjson -- %s",str.c_str());
	 if(jsonQueueEnqueueFlag == 0)
	 {
     	jsonQueue.enqueue(vec);
	 }else{
		//stop json msg in spi transfer;
	 }
}


void PropertyManager::multipleJsonEnQueue(Json::Value root)
{
    Json::StreamWriterBuilder builder;
    builder["indentation"] = ""; // 设置缩进为空字符串，即不缩进
     std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
     std::ostringstream oss;
     writer->write(root, &oss);
     // 获取字符串
     std::string str = oss.str();
     // 将字符串转换为 std::vector<uint8_t>
     std::vector<uint8_t> vec(str.begin(), str.end());
      ALOGD("multiple 1 -- %s",str.c_str());
	 if(jsonQueueEnqueueFlag == 0)
	 {
     	jsonQueue.enqueue(vec);
	 }else{
		//stop json msg in spi transfer;
	 }
}
void PropertyManager::PowerStausEnqueue(const std::vector<uint8_t>& message){
    PowerStatusQueue.enqueue(message);
}
std::vector<uint8_t> PropertyManager::Powerstatusdequeue(){
    return PowerStatusQueue.dequeue();
}

int PropertyManager::get_old_power_status() {
    int o_status = 0;
    pthread_mutex_lock(&power_mutex); // 加锁
    o_status = old_power_status;
    pthread_mutex_unlock(&power_mutex); // 解锁
    return o_status;
}
void PropertyManager::set_old_power_status(int status) {
    pthread_mutex_lock(&power_mutex); // 加锁
    old_power_status = status;
    pthread_mutex_unlock(&power_mutex); // 解锁
}

void PropertyManager::set_soc_power_on_ready(uint8_t value)
{
    mSocPowerOnReady = value;
}

uint8_t  PropertyManager::get_soc_power_on_ready()
{
   return mSocPowerOnReady;
}
void PropertyManager::set_postpone_counter(int num)
{
    postpone_counter = num+100;
}
 
void PropertyManager::ponstpone_counter_dec(int num)
{
    postpone_counter = postpone_counter - num;
}
int  PropertyManager::get_postpone_counter()
{
    return postpone_counter;
}
// bool PropertyManager::isUpdated(const std::string& name) {
//
//   std::lock_guard<std::mutex> lock(mutex);
//
//   auto it = properties.find(name);
//   if (it != properties.end()) {
//     return it->second.updated == "yes";
//   }
//
//   return false;
//
// }
//
// void PropertyManager::changeUpdate(const std::string& name) {
//
//   std::lock_guard<std::mutex> lock(mutex);
//   auto it = properties.find(name);
//   if (it != properties.end()) {
//     if(it->second.updated == "yes")
//         it->second.updated = "no";
//     else
//         it->second.updated = "yes";
//   }
// }
}  // namespace android::hardware::automotive::vehicle::V2_0::impl
