#include "vehicle_config_parser.h"
#include <log/log.h>
#include <json/json.h> // 引入jsoncpp头文件
#include <fstream>     // 用于文件操作
#include <iostream>    // 用于标准输入输出
#include <string>      // 用于字符串操作
#include <filesystem>
namespace android {
namespace hardware {
namespace automotive {
namespace vehicle {
namespace V2_0 {
namespace impl {

VehicleConfigParser::VehicleConfigParser(const std::string& filename) {
    json_ = LoadJson(filename);
    ParseGeckoConfig();
}
std::string VehicleConfigParser::LoadJson(const std::string& filename) {
    std::ifstream ifs(filename);
    if (!ifs) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return "";
    }

    std::string json;
    ifs.seekg(0, std::ios::end);
    json.reserve(ifs.tellg());
    ifs.seekg(0, std::ios::beg);

    json.assign((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

    return json;
}
void VehicleConfigParser::ParseGeckoConfig() {
    Json::CharReaderBuilder builder;
    std::unique_ptr<Json::CharReader> reader(builder.newCharReader());

    Json::Value root;
    std::string errors;
    bool parsingSuccessful =
            reader->parse(json_.c_str(), json_.c_str() + json_.size(), &root, &errors);
    if (!parsingSuccessful) {
        std::cout << "Error parsing json: " << errors << std::endl;
        return;
    }
    mComunication = root["CanCommunictionDevice"].asInt();
    mPortName = root["PortName"].asString();
    mIPServer = root["IPServer"].asString();
    mPort = root["Port"].asInt();
    mVersion = root["Version"].asString();
    ALOGE("%s %d %d", __func__, __LINE__, mComunication);
}

void VehicleConfigParser::updateJsonValue(const std::string& filePath, const std::string& key, const std::string& newValue) {
    // 1. 读取文件中的JSON数据
    std::ifstream file(filePath);
 
    std::string fileContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();
    // 2. 使用Json::CharReader解析JSON数据
    Json::CharReaderBuilder builder;
    Json::Value root;
    std::string errors;
    std::istringstream iss(fileContent); // 将字符串转换为输入流
    Json::parseFromStream(builder, iss, &root, &errors);
    // 3. 修改特定键的值或新增键值对
    bool keyFound = false;
    for (auto& item : root) {
        if (item.isObject() && item.isMember(key)) {
            item[key] = newValue;
            keyFound = true;
            break;
        }
    }
    if (!keyFound) {
        // 键不存在，新增键值对
        Json::Value newItem;
        newItem[key] = newValue;
        root.append(newItem);
    }
    // 4. 将更新后的数据写回文件
    std::ofstream outFile(filePath);
    if (!outFile.is_open()) {
        ALOGI(" saveConfig 无法打开文件进行写入。");
        return;
    }
    Json::StreamWriterBuilder writerBuilder;
    writerBuilder["commentStyle"] = "None";
    writerBuilder["indentation"] = "   ";
    std::unique_ptr<Json::StreamWriter> writer(writerBuilder.newStreamWriter());
    writer->write(root, &outFile);
    std::string oug = root.toStyledString();
    outFile.close();
    //ALOGI(" saveConfig root！%s", oug.c_str());
    ALOGI(" saveConfig value update！%s", key.c_str());
}

void VehicleConfigParser::createFile(const std::string& filename)
{
    std::ifstream file(filename);
    if(!file.good()){
        // Json::Value root;
        // std::ofstream file(filePath, std::ios::app);
        // Json::StreamWriterBuilder builder;
        // std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
        // writer->write(root, &file);
        // file.close();
        std::ofstream file(filename);
        if (file.is_open()) {
            ALOGI(" saveConfig 文件创建成功:%s", filename.c_str());
            file.close();
        } else {
            ALOGE(" saveConfig 无法创建文件:%s", filename.c_str());
        }
    }
}
void VehicleConfigParser::saveConfig(const std::string& key, const std::string& value)
{
    
    createFile(mFilePath);
    ALOGI("%s  key:%s, value:%s", __func__, key.c_str(), value.c_str());
    updateJsonValue(mFilePath, key, value);
    // Json::Value root;
    // root[key] = value;
    // std::ofstream file("/data/vendor/vehicle/config.json", std::ios::app);
    // Json::StreamWriterBuilder builder;
    // std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
    // writer->write(root, &file);
    // file.close();
}

}  // namespace impl
}  // namespace V2_0
}  // namespace vehicle
}  // namespace automotive
}  // namespace hardware
}  // namespace android