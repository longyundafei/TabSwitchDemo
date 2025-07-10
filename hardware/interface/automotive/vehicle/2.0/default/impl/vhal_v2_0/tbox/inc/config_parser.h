#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H

#include <json/json.h>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "TboxDataDeclaration.h"
#include "LockMap.h"

class ConfigParser {
  public:
    ConfigParser(const std::string& filename,LockMap<int32_t, TboxDataDeclaration>& tboxdecl);

    void Parse(LockMap<int32_t, TboxDataDeclaration>& tboxdecl);

  public:
    std::vector<ConfigDeclaration> mConfigs;
//    std::map<int32_t, TboxDataDeclaration> mTboxDecl;
  private:
    std::string LoadJson(const std::string& filename);
    void ParseConfig(const Json::Value& value, ConfigDeclaration& config);

    std::string json_;
};
#endif  // CONFIG_PARSER_H