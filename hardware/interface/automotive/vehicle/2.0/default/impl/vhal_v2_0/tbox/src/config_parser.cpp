#include "config_parser.h"
#include <log/log.h>
#include <android-base/logging.h>

ConfigParser::ConfigParser(const std::string& filename,LockMap<int32_t, TboxDataDeclaration>& tboxdecl) {
    json_ = LoadJson(filename);
    Parse(tboxdecl);
}
std::string ConfigParser::LoadJson(const std::string& filename) {
    ALOGE("%s %d %s", __func__, __LINE__, filename.c_str());
    std::ifstream ifs(filename);
    if (!ifs) {
        //std::cerr << "Failed to open file: " << filename << std::endl;
        LOG(INFO) << "Failed to open file: " << filename;
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

void ConfigParser::Parse(LockMap<int32_t, TboxDataDeclaration>& tboxdecl) {
    Json::CharReaderBuilder builder;
    std::unique_ptr<Json::CharReader> reader(builder.newCharReader());

    Json::Value root;
    std::string errors;
    bool parsingSuccessful =
            reader->parse(json_.c_str(), json_.c_str() + json_.size(), &root, &errors);
    if (!parsingSuccessful) {
        //std::cout << "Error parsing json: " << errors << std::endl;
        LOG(INFO) << "Error parsing json: " << errors;
        //        ALOGE("%s %d %s", __func__, __LINE__, errors.c_str());
        return;
    }
    for (auto& el : root) {
        ConfigDeclaration config;
        TboxDataDeclaration tboxdatadecl;

        ParseConfig(el["config"], tboxdatadecl.conf);
        mConfigs.push_back(tboxdatadecl.conf);
        tboxdecl[tboxdatadecl.conf.prop] = tboxdatadecl;
    }
}
void ConfigParser::ParseConfig(const Json::Value& value, ConfigDeclaration& config) {
    config.prop = value["prop"].asInt();
    config.name = value["name"].asString();
    config.areaid = value["areaId"].asInt();
    config.type = value["type"].asString();
    config.prop_res = value["prop_res"].asFloat();
    config.prop_offset = value["prop_offset"].asInt();
    config.prop_muls = value["prop_muls"].asInt();
    config.tbox_value_size = value["tbox_value_size"].asInt();
    config.tbox_value_min = value["tbox_value_min"].asInt();
    config.tbox_value_max = value["tbox_value_max"].asUInt();
    config.tbox_value_offset = value["tbox_value_offset"].asInt();
    config.tbox_value_muls = value["tbox_value_muls"].asInt();

    //LOG(INFO) << "prop:" << config.prop
              //<< "name:" << config.name
              //<< "areaid:" << config.areaid
              //<< "type:" << config.type;
    if(!value["value_map"].isNull()){
        for(auto&val:value["value_map"]) {
            int prop_value = val["prop_value"].asInt();
            std::string tbox_value = val["tbox_value"].asString();
            //LOG(INFO) << "prop_value:" << prop_value
                      //<< "tbox_value:" << tbox_value;
            config.TboxRflex[prop_value] = tbox_value;
        }
    }
}

