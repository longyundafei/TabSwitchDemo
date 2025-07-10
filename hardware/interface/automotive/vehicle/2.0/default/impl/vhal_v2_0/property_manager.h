#ifndef PROPERTY_MANAGER_H
#define PROPERTY_MANAGER_H

#include <unordered_map>
#include <string>
#include <random>
#include <mutex>
#include "config_parser.h"
#include "SendQueueV1_1.h"
namespace android::hardware::automotive::vehicle::V2_0::impl {
//struct Property {
//  std::string name;
//  std::string value = "0";
//  std::string originalType;
//  std::string updated = "no";    
////  std::mutex mutex;
//  std::condition_variable cv;
//};
struct Property {
  std::string name;
  std::string value = "0";
  std::string originalType;
  std::string updated = "no";    
  std::shared_ptr<std::condition_variable> cv;

  Property() : cv(std::make_shared<std::condition_variable>()) {}
};
class PropertyManager {
public:
  PropertyManager();
  
  void addProperty(const Property& prop);
  
  Property* findProperty(const std::string& name);

  ConfigDBC* findCProperty(const std::string& name);
  
  void updatePropertyValue(const std::string& name, const std::string& newValue);

  void updateCPropertyValue(const std::string& name, uint32_t newValue);
  void updateCPropertyValueNoUpdate(const std::string& name, uint32_t newValue);
  void updateCPropertyValueNoCheck(const std::string& name, int32_t newValue);
  void updateCAPropertyValue(const std::string& name, const std::string&  newValue);
  void updateTPropertyValue(const std::string& name, int64_t newValue);

  void updatePropertyValue(Property* mp, const std::string& newValue);
  void updateCPropertyValue(ConfigDBC* mp, uint32_t newValue);
  void updateCPropertyValue(const std::string& name, std::string& newValue);
  void updateCPropertyValueNoCheck(const std::string& name, std::string& newValue);
  void updateTPropertyValue(ConfigDBC* mp, int64_t newValue); 
  void updatePropertyValues();
  void setspiQueue(SendQueueV1_1 *p);
  void getCan071State();
  void setCan071update();
  void getCan30AState();
  void setCan30Aupdate();
  void getCan30BState();
  void setCan30Bupdate();
  void getCan30CState();
  void setCan30Cupdate();

  size_t getPropertyCount();
  
  void printProperties();

  void importPropertiesFromFile(const std::string& filename, PropertyManager& manager);

//  bool isUpdated(const std::string& name); 
  
//  void changeUpdate(const std::string& name);

  std::string getProValue(Property* mp); 
  uint32_t getCProValue(ConfigDBC* mp);
  bool getCProValue(ConfigDBC* mp,int32_t* value);
  uint64_t getTProValue(ConfigDBC* mp);
  std::string getCProStrValue(ConfigDBC* mp);
  void getUpdateState(Property* mp);
  void getUpdateCState(ConfigDBC* mp);
  void getUpdateTState(ConfigDBC* mp);
  void setmpConfigParser(ConfigParser* mp);
  SendQueueV1_1* mpSpi;
  void stopJsonQueueEnqueue();
  void resumeJsonQueueEnqueue();
  void jsonQueueEnqueue(const std::vector<uint8_t>& message);
  void singleJsonEnQueue(std::string& key, std::string& value);
  void multipleJsonEnQueue(Json::Value root);
  void PowerStausEnqueue(const std::vector<uint8_t>& message);
  std::vector<uint8_t> Powerstatusdequeue();
  std::vector<uint8_t> jsonQueuedequeue();
  SendQueueV1_1 PowerStatusQueue;
  SendQueueV1_1 jsonQueue;
  int get_old_power_status();
  void set_old_power_status(int status);
  void set_soc_power_on_ready(uint8_t value);
  uint8_t get_soc_power_on_ready();
  void set_postpone_counter(int num);
  void ponstpone_counter_dec(int num);
  int  get_postpone_counter();
private:
  int postpone_counter = 0;
  std::mutex mutex; 
  int old_power_status = 0;
  pthread_mutex_t power_mutex = PTHREAD_MUTEX_INITIALIZER;
  ConfigParser* mpConfigParser;
  uint8_t jsonQueueEnqueueFlag = 0;
  uint8_t mSocPowerOnReady = 0;
//  std::unordered_map<std::string, Property> properties;
  std::vector<Property> properties;
  std::unique_ptr<std::condition_variable> can071_cv;
  std::unique_ptr<std::condition_variable> can30A_cv;
  std::unique_ptr<std::condition_variable> can30B_cv;
  std::unique_ptr<std::condition_variable> can30C_cv;
};
}
#endif // PROPERTY_MANAGER_H
