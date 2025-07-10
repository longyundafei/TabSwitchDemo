#include "config_parser.h"
#include <log/log.h>
namespace android {
namespace hardware {
namespace automotive {
namespace vehicle {
namespace V2_0 {

namespace impl {
int addconfigfile() {
    std::string sourceFileName = "/vendor/etc/gecko-prop-init.json";        // 源文件名
    std::string targetFileName = "/data/vendor/etc/gecko-prop-init.json";  // 目标文件名

    // 使用文件系统库检查目标文件是否存在
    std::ifstream file(targetFileName);
    if (file.good()) {
        std::cout << "Target file exists.";
        file.close();
        return 0;
    } else {
        std::cout << "Target file does not exist.";
    }

    // 打开源文件和目标文件
    std::ifstream sourceFile(sourceFileName);
    std::ofstream targetFile(targetFileName);

    if (!sourceFile.is_open() || !targetFile.is_open()) {
        std::cerr << "Failed to open files." << std::endl;
        return 1;
    }

    // 从源文件读取内容并写入目标文件
    std::string line;
    while (std::getline(sourceFile, line)) {
        targetFile << line << std::endl;
    }

    // 关闭文件
    sourceFile.close();
    targetFile.close();
    std::cout << "File copied successfully." << std::endl;
    return 0;
}

ConfigParser::ConfigParser(const std::string& filename) {
    json_ = LoadJson(filename);
    addconfigfile();
}
std::string ConfigParser::LoadJson(const std::string& filename) {
    ALOGE("%s %d %s", __func__, __LINE__, filename.c_str());
    std::ifstream ifs(filename);
    if (!ifs) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        ALOGE("%s %d", __func__, __LINE__);
        return "";
    }
    std::string json;
    ifs.seekg(0, std::ios::end);
    json.reserve(ifs.tellg());
    ifs.seekg(0, std::ios::beg);

    json.assign((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

    return json;
}

void ConfigParser::Parse() {
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
        return;
    }
    // int counter = 0;
    for (auto& el : root) {
        ConfigDeclaration config;

        ParseConfig(el["config"], config);

        // ALOGE("%s %d %d %d %d %d", __func__, __LINE__,counter++,config.config.prop,
        // el["initialValue"].isNull(),el["initialAreaValues"].isNull());
        if (!el["initialValue"].isNull()) {
            //            ALOGE("%s %d", __func__, __LINE__);
            ParseInitialValue(el["initialValue"], config.initialValue);
        }

        //...
        if (!el["initialAreaValues"].isNull()) {
            //            ALOGE("%s %d", __func__, __LINE__);
            for (auto& areaEl : el["initialAreaValues"]) {
                int32_t areaId = areaEl["areaId"].asInt();
                VehiclePropValue::RawValue value;
                ParseInitialValue(areaEl, value);
                config.initialAreaValues[areaId] = value;
            }
        }
        if (!el["canmsg"].isNull()) {
            for(auto& val : el["canmsg"]){
              ConfigDBC conf;
              conf.msgname = val["msgname"].asString();
              conf.msgtype = val["msgtype"].asString();
              conf.value = val["value"].asInt();
              if(!val["value_map"].isNull()){
                for (auto& value : val["value_map"]) {
                    uint32_t id = value["can_value"].asInt();
                    std::string val = value["android_value"].asString();
                    conf.valueMap[id] = val;
//                    ALOGE("%s %d %d %s", __func__, __LINE__, id, val.c_str());
                }
              }
              ALOGE("%s %d %s %s %d", __func__, __LINE__, conf.msgname.c_str(),
                  conf.msgtype.c_str(), conf.value);
              config.canmsg.push_back(conf);
//              mcanConfMap[conf.msgname] = &(config.canmsg.back());
            }
        }
        mConfigs.push_back(config);
    }
    for (auto& p : mConfigs) {
        for (auto& pp : p.canmsg) {
            if (pp.msgname.size() != 0) {
                mcanConfMap[pp.msgname] = &pp;
            }
        }
    }
}

void ConfigParser::ParseConfig(const Json::Value& value, ConfigDeclaration& config) {
    config.config.prop = value["prop"].asInt();
    // 解析access
    std::string accessStr = value["access"].asString();
    if (accessStr == "READ") {
        config.config.access = VehiclePropertyAccess::READ;
    } else if (accessStr == "WRITE") {
        config.config.access = VehiclePropertyAccess::WRITE;
    } else if (accessStr == "READ_WRITE") {
        config.config.access = VehiclePropertyAccess::READ_WRITE;
    } else if (accessStr == "NONE") {
        config.config.access = VehiclePropertyAccess::NONE;
    }

    // 解析changeMode
    std::string changeModeStr = value["changeMode"].asString();
    if (changeModeStr == "STATIC") {
        config.config.changeMode = VehiclePropertyChangeMode::STATIC;
    } else if (changeModeStr == "CONTINUOUS") {
        config.config.changeMode = VehiclePropertyChangeMode::CONTINUOUS;
    } else if (changeModeStr == "ON_CHANGE") {
        config.config.changeMode = VehiclePropertyChangeMode::ON_CHANGE;
    }
    if (changeModeStr == "CONTINUOUS") {
        config.config.maxSampleRate = value["maxSampleRate"].asFloat();
        config.config.minSampleRate = value["minSampleRate"].asFloat();
    }

    // 解析areaConfigs
    int counter = 0;
    for (auto& areaConfigVal : value["areaConfigs"]) {
        VehicleAreaConfig areaConfig;
        areaConfig.areaId = areaConfigVal["areaId"].asInt();

        areaConfig.minInt32Value = areaConfigVal["minInt32Value"].asInt();
        areaConfig.maxInt32Value = areaConfigVal["maxInt32Value"].asInt();
        areaConfig.minInt64Value = areaConfigVal["minInt64Value"].asInt64();
        areaConfig.maxInt64Value = areaConfigVal["maxInt64Value"].asInt64();
        areaConfig.minFloatValue = areaConfigVal["minFloatValue"].asFloat();
        areaConfig.maxFloatValue = areaConfigVal["maxFloatValue"].asFloat();
        // 解析areaConfig其他字段

        config.config.areaConfigs.resize(counter + 1);
        config.config.areaConfigs[counter] = areaConfig;
        counter++;
    }

    counter = 0;
    for (auto& v : value["configArray"]) {
        config.config.configArray.resize(counter + 1);  // hidl vector增长
        config.config.configArray[counter] = v.asInt();
        counter++;
    }
}

void ConfigParser::ParseInitialValue(const Json::Value& value,
                                     VehiclePropValue::RawValue& initialValue) {
    if (!value["int32Values"].isNull()) {
        for (auto& v : value["int32Values"]) {
            initialValue.int32Values.resize(initialValue.int32Values.size() + 1);
            initialValue.int32Values[initialValue.int32Values.size() - 1] = v.asInt();
        }
    }
    if (!value["floatValues"].isNull()) {
        for (auto& v : value["floatValues"]) {
            initialValue.floatValues.resize(initialValue.floatValues.size() + 1);
            initialValue.floatValues[initialValue.floatValues.size() - 1] = v.asFloat();
        }
    }
    // 解析bytes
    if (!value["bytes"].isNull()) {
        for (auto& v : value["bytes"]) {
            initialValue.bytes.resize(initialValue.bytes.size() + 1);
            initialValue.bytes[initialValue.bytes.size() - 1] = v.asUInt();
        }
    }
    // 解析int64Values
    if (!value["int64Values"].isNull()) {
        for (auto& v : value["int64Values"]) {
            initialValue.int64Values.resize(initialValue.int64Values.size() + 1);
            initialValue.int64Values[initialValue.int64Values.size() - 1] = v.asInt64();
        }
    }
    // 解析stringValue
    if (!value["stringValue"].isNull()) {
        initialValue.stringValue = value["stringValue"].asString();
    }
}

std::string ConfigParser::configToJson(const ConfigDeclaration& config) {
    Json::Value root;

    root["config"]["prop"] = config.config.prop;
    std::string accessStr;
    switch (config.config.access) {
        case VehiclePropertyAccess::READ:
            accessStr = "READ";
            break;
        case VehiclePropertyAccess::WRITE:
            accessStr = "WRITE";
            break;
        case VehiclePropertyAccess::READ_WRITE:
            accessStr = "READ_WRITE";
            break;
        case VehiclePropertyAccess::NONE:
            accessStr = "NONE";
            break;
    }

    root["config"]["access"] = accessStr;

    std::string changeModeStr;
    switch (config.config.changeMode) {
        case VehiclePropertyChangeMode::STATIC:
            changeModeStr = "STATIC";
            break;
        case VehiclePropertyChangeMode::CONTINUOUS:
            changeModeStr = "CONTINUOUS";
            break;
        case VehiclePropertyChangeMode::ON_CHANGE:
            changeModeStr = "ON_CHANGE";
            break;
    }

    root["config"]["changeMode"] = changeModeStr;
    if (changeModeStr == "CONTINUOUS") {
        root["config"]["minSampleRate"] = config.config.minSampleRate;
        root["config"]["maxSampleRate"] = config.config.maxSampleRate;
    }

    if (config.config.configArray.size() != 0) {
        Json::Value configArrayJson;
        for (const auto& ca : config.config.configArray) {
            configArrayJson.append(ca);
        }
        root["config"]["configArray"] = configArrayJson;
    }
    if (config.config.areaConfigs.size() != 0) {
        Json::Value areaConfigs(Json::arrayValue);

        for (const auto& areaConfig : config.config.areaConfigs) {
            Json::Value areaConfigJson;
            areaConfigJson["areaId"] = areaConfig.areaId;
            areaConfigJson["minFloatValue"] = areaConfig.minFloatValue;
            areaConfigJson["maxFloatValue"] = areaConfig.maxFloatValue;
            areaConfigJson["minInt32Value"] = areaConfig.minInt32Value;
            areaConfigJson["maxInt32Value"] = areaConfig.maxInt32Value;
            areaConfigJson["minInt64Value"] = areaConfig.minInt64Value;
            areaConfigJson["maxInt64Value"] = areaConfig.maxInt64Value;
            areaConfigs.append(areaConfigJson);
            // 处理areaConfig的其他字段
        }
        root["config"]["areaConfigs"] = areaConfigs;
    }
    // 处理其他config字段
    Json::Value initialValue;
    if (config.initialValue.int32Values.size() != 0) {
        initialValue["int32Values"] = Json::Value(Json::arrayValue);
        for (int32_t v : config.initialValue.int32Values) {
            initialValue["int32Values"].append(v);
        }
    }

    if (config.initialValue.floatValues.size() != 0) {
        initialValue["floatValues"] = Json::Value(Json::arrayValue);
        for (float v : config.initialValue.floatValues) {
            initialValue["floatValues"].append(v);
        }
    }

    if (config.initialValue.int64Values.size() != 0) {
        initialValue["int64Values"] = Json::Value(Json::arrayValue);
        for (int64_t v : config.initialValue.int64Values) {
            initialValue["int64Values"].append(v);
        }
    }

    if (config.initialValue.bytes.size() != 0) {
        initialValue["bytes"] = Json::Value(Json::arrayValue);
        for (uint8_t v : config.initialValue.bytes) {
            initialValue["bytes"].append(v);
        }
    }

    if (!config.initialValue.stringValue.empty()) {
        std::string stringValue = config.initialValue.stringValue;
        initialValue["stringValue"] = stringValue;
    }
    if (!initialValue.empty()) {
        root["initialValue"] = initialValue;
    }
    // 处理initialValue其他字段

    Json::Value initialAreaValues;
    Json::Value AreaId;
    for (auto& v : config.initialAreaValues) {
        // 处理每个area的initial value

        AreaId["areaId"] = v.first;
        if (v.second.int32Values.size() != 0) {
            AreaId["int32Values"] = Json::Value(Json::arrayValue);
            for (int32_t v1 : v.second.int32Values) {
                AreaId["int32Values"].append(v1);
            }
        }

        if (v.second.floatValues.size() != 0) {
            AreaId["floatValues"] = Json::Value(Json::arrayValue);
            for (float v1 : v.second.floatValues) {
                AreaId["floatValues"].append(v1);
            }
        }

        if (v.second.int64Values.size() != 0) {
            AreaId["int64Values"] = Json::Value(Json::arrayValue);
            for (int64_t v1 : v.second.int64Values) {
                AreaId["int64Values"].append(v1);
            }
        }

        if (v.second.bytes.size() != 0) {
            AreaId["bytes"] = Json::Value(Json::arrayValue);
            for (uint8_t v1 : v.second.bytes) {
                AreaId["bytes"].append(v1);
            }
        }

        if (!v.second.stringValue.empty()) {
            std::string stringValue = v.second.stringValue;
            AreaId["stringValue"] = stringValue;
        }
        initialAreaValues.append(AreaId);
    }
    if (!initialAreaValues.empty()) {
        root["initialAreaValues"] = initialAreaValues;
    }
    std::ofstream file("/data/data/output.json", std::ios::app);
    Json::StreamWriterBuilder builder;
    std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
    writer->write(root, &file);
    file.close();
    return "";
}



}  // namespace impl
}  // namespace V2_0
}  // namespace vehicle
}  // namespace automotive
}  // namespace hardware
}  // namespace android
