#ifndef CONFIG_VEHICLE_CONFIG_PARSER_H
#define CONFIG_VEHICLE_CONFIG_PARSER_H

#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <iostream>
#include <json/json.h>

namespace android::hardware::automotive::vehicle::V2_0::impl {
class VehicleConfigParser {
public:
  VehicleConfigParser(const std::string& filename);
  void saveConfig(const std::string& key, const std::string& value);
  int mComunication = 0;
  std::string mPortName;
  std::string mIPServer;
  int mPort = 0;
  std::string mVersion;

private:
void createFile(const std::string& filename);
  void updateJsonValue(const std::string& filePath, const std::string& key, const std::string& newValue);
  //void updateJsonValue(const std::string& filePath, const std::string& primaryKey, const std::string& secondaryKey, const std::string& newValue);
  std::string mFilePath = "/data/vendor/vehicle/config.json";
  std::string LoadJson(const std::string& filename);
  void ParseGeckoConfig();
  std::string json_;
};
}
#endif