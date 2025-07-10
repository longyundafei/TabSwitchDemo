#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H

#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <iostream>
#include "DefaultConfig.h"
#include <json/json.h>
#include <log/log.h>

namespace android::hardware::automotive::vehicle::V2_0::impl {
class ConfigParser {
public:
//  ConfigParser(const std::string& json);
  ConfigParser(const std::string& filename);
  void Parse();
  std::string configToJson(const ConfigDeclaration& config);

public:
  std::map<std::string, ConfigDBC*> mcanConfMap;
  std::vector<ConfigDeclaration> mConfigs;
  std::mutex mtx;  //lock
  template<typename T>
inline std::string vecToString(const hidl_vec<T>& vec) {
    std::stringstream ss("[");
    for (size_t i = 0; i < vec.size(); i++) {
        if (i != 0) ss << ",";
        ss << vec[i];
    }
    ss << "]";
    return ss.str();
}

  template<typename T>
bool saveInitialValueToJson(int prop, T& value) {
//bool ConfigParser::saveInitialValueToJson(int prop, ::android::hardware::hidl_vec<float> value) {
   	ALOGI("%s %d %d", __func__, __LINE__,prop);
	//mtx.lock();
	std::lock_guard<std::mutex> guard(mtx);
    std::vector<ConfigDeclaration> configs;
    Json::CharReaderBuilder builder;
    std::unique_ptr<Json::CharReader> reader(builder.newCharReader());

    Json::Value root;
    std::string errors;
    bool parsingSuccessful =
            reader->parse(json_.c_str(), json_.c_str() + json_.size(), &root, &errors);
    if (!parsingSuccessful) {
        std::cout << "Error parsing json: " << errors << std::endl;
        //        ALOGE("%s %d %s", __func__, __LINE__, errors.c_str());
        return false;
    }
    
     
      ALOGI("%s %d rootsize:%d", __func__, __LINE__,root.size());
     if(root.isArray()) {
     for(int i = 0; i < (int)root.size();i++) {
     
         ALOGI("%s %d rootsize[i]:%d",__func__, __LINE__,i);
         Json::Value& mConfig = root[i]["config"];
         Json::Value& mInitialValue = root[i]["initialValue"];
         if(!mConfig["prop"].isNull() && mConfig["prop"] == prop) {
           ALOGI("%s %d prop isnull and prop equal id",__func__, __LINE__);
           Json::Value initialValue;
           if(!mInitialValue["int32Values"].isNull() && std::is_integral<T>::value) {
               ALOGI("%s %d int32Values int32_t:%d",__func__, __LINE__, (int32_t) value);
               initialValue["int32Values"].append((int32_t) value);
           }else if(!mInitialValue["floatValues"].isNull()) {
               ALOGI("%s %d floatValues value",__func__, __LINE__);
               for (float v : (::android::hardware::hidl_vec<float>)value) {
               ALOGI("%s %d floatValues float:%f",__func__, __LINE__,v);
               initialValue["floatValues"].append(v);
                 }  
           }else if(!mInitialValue["int64Values"].isNull()) {
               std::stringstream ss;
               ss << "----commInt64VecData----prop=0x" << std::hex << prop;
               ss << vecToString<int32_t>(value);
               ALOGI("%s %d int64Values ss:%s",__func__, __LINE__,ss.str().c_str());
               for (int64_t v : (::android::hardware::hidl_vec<int64_t>)value) {
               //ALOGI("%s %d int64Values int64_t:%llu",__func__, __LINE__,v);
               initialValue["int64Values"].append(v);
                 }
           }else if(!mInitialValue["bytes"].isNull()) {
               ALOGI("%s %d bytes value",__func__, __LINE__);
               for (uint8_t v : (::android::hardware::hidl_vec<uint8_t>)value) {
               ALOGI("%s %d bytes bytes:%d",__func__, __LINE__,static_cast<int>(v));
               initialValue["bytes"].append(v);
                 }
           } 
          
           if (!initialValue.empty()) {
               ALOGI("%s %d initialValue is not null",__func__, __LINE__);
               root[i]["initialValue"] = initialValue;
           }
           ALOGI("%s %d file save to json",__func__, __LINE__);
           std::ofstream file("/data/vendor/etc/gecko-prop-init.json", std::ios::trunc);
           Json::StreamWriterBuilder builder1;
           std::unique_ptr<Json::StreamWriter> writer(builder1.newStreamWriter());
           writer->write(root, &file);
           file.close();
           return true;
          
        }
      
      }

    }
    
    return false;
}


bool saveInitialValueToJson(int prop, std::string value) {
//bool ConfigParser::saveInitialValueToJson(int prop, ::android::hardware::hidl_vec<float> value) {
   	ALOGI("%s %d %d", __func__, __LINE__,prop);
	//mtx.lock();
	std::lock_guard<std::mutex> guard(mtx);
    std::vector<ConfigDeclaration> configs;
    Json::CharReaderBuilder builder;
    std::unique_ptr<Json::CharReader> reader(builder.newCharReader());

    Json::Value root;
    std::string errors;
    bool parsingSuccessful =
            reader->parse(json_.c_str(), json_.c_str() + json_.size(), &root, &errors);
    if (!parsingSuccessful) {
        std::cout << "Error parsing json: " << errors << std::endl;
        //        ALOGE("%s %d %s", __func__, __LINE__, errors.c_str());
        return false;
    }
    
     ALOGI("%s %d rootsize:%d", __func__, __LINE__,root.size());
     if(root.isArray()) {
     for(int i = 0; i < (int)root.size();i++) {
         ALOGI("%s %d rootsize[i]:%d",__func__, __LINE__,i);
         Json::Value& mConfig = root[i]["config"];
         Json::Value& mInitialValue = root[i]["initialValue"];
         if(!mConfig["prop"].isNull() && mConfig["prop"] == prop) {
           ALOGI("%s %d prop isnull and prop equal id",__func__, __LINE__);
           std::string stringValue(value);
           Json::Value initialValue;
           if(!mInitialValue["stringValue"].isNull()){
               ALOGI("%s %d stringValue value:%s",__func__, __LINE__, stringValue.c_str());
               initialValue["stringValue"] = stringValue;
            }
           if (!initialValue.empty()) {
               ALOGI("%s %d initialValue is not null",__func__, __LINE__);
               root[i]["initialValue"] = initialValue;
           }
           ALOGI("%s %d file save to json",__func__, __LINE__);
           std::ofstream file("/data/vendor/etc/gecko-prop-init.json", std::ios::trunc);
           Json::StreamWriterBuilder builder1;
           std::unique_ptr<Json::StreamWriter> writer(builder1.newStreamWriter());
           writer->write(root, &file);
           file.close();
           return true;
          
        }
      
      }

    }
    
    return false;
}

private:
  std::string LoadJson(const std::string& filename);
  void ParseConfig(const Json::Value& value, ConfigDeclaration& config);
  void ParseInitialValue(const Json::Value& value, VehiclePropValue::RawValue& initialValue);
  
  std::string json_;
};
}
#endif // CONFIG_PARSER_H
