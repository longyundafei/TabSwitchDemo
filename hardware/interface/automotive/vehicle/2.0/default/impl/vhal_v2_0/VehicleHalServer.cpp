/*
 * Copyright (C) 2020 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "VehicleHalServer"

#include "VehicleHalServer.h"

#include <fstream>

#include <android-base/logging.h>
#include <json/json.h>
#include <utils/SystemClock.h>
#include "DefaultConfig.h"
#include "JsonFakeValueGenerator.h"
#include "LinearFakeValueGenerator.h"
#include "MessageDefine.h"
#include "Obd2SensorStore.h"
#include "SendQueue.h"
#include "SendCanMap.h"
#include "SerialCommunication.h"
#include "body_can_ccm_4_0.h"
#include "chassise_can_ccm_4_0.h"
#include "ev_can_ccm_4_0.h"
#include "mcu.h"
#include <android-base/properties.h>
#include "PowerStatusQueue.h"

namespace android::hardware::automotive::vehicle::V2_0::impl {

static bool isDiagnosticProperty(VehiclePropConfig propConfig) {
    switch (propConfig.prop) {
        case OBD2_LIVE_FRAME:
        case OBD2_FREEZE_FRAME:
        case OBD2_FREEZE_FRAME_CLEAR:
        case OBD2_FREEZE_FRAME_INFO:
            return true;
    }
    return false;
}
std::string configToJson(const ConfigDeclaration& config) {
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
    Json::Value AreaValues;
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
    LOG(WARNING) << __func__ << &file << "=-" << writer;
    writer->write(root, &file);
    file.close();
    return "";
}

ConfigParser* VehicleHalServer::GetPropConfig(){
  return &mConfigParser;
}


int VehicleHalServer::vehicle_vendor_storage_read(uint16_t id, uint16_t len, std::vector<uint8_t>& out_data)
{
    int ret = 0;
    uint32_t i = 0;
    uint8_t p_buf[1124];
    struct gecko_vendor_req *req;

    req = (struct gecko_vendor_req *)p_buf;
    int sys_fd = open("/dev/vendor_storage", O_RDWR, 0);
    if (sys_fd < 0) {
        ALOGE("vendor_storage open fail\n");
        return -1;
    }

    req->tag = VENDOR_REQ_TAG;
    req->id = id;
    req->len = 512;

    memset(req->data, 0xAA, len);
    ret = ioctl(sys_fd, VENDOR_READ_IO, req);
    if (ret == 0) {
        out_data.assign(req->data, req->data + len);
        if (!out_data.empty()) {
            std::string hex_string;
            for (i=0;i<len;i++) {
                char buf[4];
                snprintf(buf, sizeof(buf), "%02X ", out_data[i]);
                hex_string += buf;
            }
            ALOGE("vehicle_vendor_storage_read: %s", hex_string.c_str());
        }
        ALOGE("read vendor storage success\n");
        close(sys_fd);
        return 0;
    }
    else {
        ALOGE("read vendor storage error ret %d\n", ret);
        close(sys_fd);
        return -2;  // IO 操作失败
    }
}

int VehicleHalServer::vehicle_vendor_storage_write(uint16_t id, uint16_t len, const std::vector<uint8_t>& write_value)
{
    int ret = 0;
    uint32_t i = 0;
    uint8_t p_buf[1124];
    struct gecko_vendor_req *req;

    if (write_value.empty()) {
        ALOGE("ITEM process_cmd value is empty");
        return -1;
    }

    req = (struct gecko_vendor_req *)p_buf;
    int sys_fd = open("/dev/vendor_storage", O_RDWR, 0);
    if (sys_fd < 0) {
        ALOGE("vendor_storage open fail\n");
        return -1;  // 打开设备失败
    }

    req->tag = VENDOR_REQ_TAG;
    req->id = id;
    req->len = len;

    if (write_value.size() < len) {
        ALOGE("Error: write_value length (%zu) is less than expected (%u)\n", write_value.size(), len);
        close(sys_fd);
        return -3;  // 数据长度不足
    }
    memcpy(req->data, write_value.data(), len);
    std::string hex_string;
    for (i=0;i<len;i++) {
        char buf[4];
        snprintf(buf, sizeof(buf), "%02X ", req->data[i]);
        hex_string += buf;
    }
    ALOGE("vehicle_vendor_storage_write: %s", hex_string.c_str());
    ret = ioctl(sys_fd, VENDOR_WRITE_IO, req);
    if (ret) {
        ALOGE("write vendor storage error %d \n", ret);
        close(sys_fd);
        return -2;
    }

    ALOGE("write vendor storage success\n");
    close(sys_fd);
    return 0;
}

VehicleHalServer::VehicleHalServer() {
    constexpr bool shouldUpdateStatus = true;
        mConfigParser.Parse();
//        for(auto& it : mConfigParser.mConfigs){
//          mConfigParser.configToJson(it);
//        }

//    for (auto& it : kVehicleProperties) {
    for (auto& it : mConfigParser.mConfigs) {
        VehiclePropConfig cfg = it.config;

//        configToJson(it);
        mServerSidePropStore.registerProperty(cfg);

        if (isDiagnosticProperty(cfg)) {
            continue;
        }

        // A global property will have only a single area
        int32_t numAreas = isGlobalProp(cfg.prop) ? 1 : cfg.areaConfigs.size();

        for (int i = 0; i < numAreas; i++) {
            int32_t curArea = isGlobalProp(cfg.prop) ? 0 : cfg.areaConfigs[i].areaId;

            // Create a separate instance for each individual zone
            VehiclePropValue prop = {
                    .areaId = curArea,
                    .prop = cfg.prop,
            };

            if (it.initialAreaValues.empty()) {
                prop.value = it.initialValue;
            } else if (auto valueForAreaIt = it.initialAreaValues.find(curArea);
                       valueForAreaIt != it.initialAreaValues.end()) {
                prop.value = valueForAreaIt->second;
            } else {
                LOG(WARNING) << __func__ << " failed to get default value for"
                             << " prop 0x" << std::hex << cfg.prop << " area 0x" << std::hex
                             << curArea;
                prop.status = VehiclePropertyStatus::UNAVAILABLE;
            }

            mServerSidePropStore.writeValue(prop, shouldUpdateStatus);
        }
    }
}

void VehicleHalServer::sendAllValuesToClient() {
    constexpr bool update_status = true;
    auto values = mServerSidePropStore.readAllValues();
    for (const auto& value : values) {
        onPropertyValueFromCar(value, update_status);
    }
}

GeneratorHub* VehicleHalServer::getGenerator() {
    return &mGeneratorHub;
}

VehiclePropValuePool* VehicleHalServer::getValuePool() const {
    if (!mValuePool) {
        LOG(WARNING) << __func__ << ": Value pool not set!";
    }
    return mValuePool;
}

void VehicleHalServer::setValuePool(VehiclePropValuePool* valuePool) {
    if (!valuePool) {
        LOG(WARNING) << __func__ << ": Setting value pool to nullptr!";
    }
    mValuePool = valuePool;
}

void VehicleHalServer::onFakeValueGenerated(const VehiclePropValue& value) {
    constexpr bool updateStatus = true;
    LOG(DEBUG) << __func__ << ": " << toString(value);
    auto updatedPropValue = getValuePool()->obtain(value);
    if (updatedPropValue) {
        updatedPropValue->timestamp = value.timestamp;
        updatedPropValue->status = VehiclePropertyStatus::AVAILABLE;
        mServerSidePropStore.writeValue(*updatedPropValue, updateStatus);
        onPropertyValueFromCar(*updatedPropValue, updateStatus);
    }
}

std::vector<VehiclePropConfig> VehicleHalServer::onGetAllPropertyConfig() const {
    return mServerSidePropStore.getAllConfigs();
}

StatusCode VehicleHalServer::handleGenerateFakeDataRequest(const VehiclePropValue& request) {
    constexpr bool updateStatus = true;

    LOG(INFO) << __func__;
    const auto& v = request.value;
    if (!v.int32Values.size()) {
        LOG(ERROR) << __func__ << ": expected at least \"command\" field in int32Values";
        return StatusCode::INVALID_ARG;
    }

    FakeDataCommand command = static_cast<FakeDataCommand>(v.int32Values[0]);

    switch (command) {
        case FakeDataCommand::StartLinear: {
            LOG(INFO) << __func__ << ", FakeDataCommand::StartLinear";
            if (v.int32Values.size() < 2) {
                LOG(ERROR) << __func__ << ": expected property ID in int32Values";
                return StatusCode::INVALID_ARG;
            }
            if (!v.int64Values.size()) {
                LOG(ERROR) << __func__ << ": interval is not provided in int64Values";
                return StatusCode::INVALID_ARG;
            }
            if (v.floatValues.size() < 3) {
                LOG(ERROR) << __func__ << ": expected at least 3 elements in floatValues, got: "
                           << v.floatValues.size();
                return StatusCode::INVALID_ARG;
            }
            int32_t cookie = v.int32Values[1];
            getGenerator()->registerGenerator(cookie,
                                              std::make_unique<LinearFakeValueGenerator>(request));
            break;
        }
        case FakeDataCommand::StartJson: {
            LOG(INFO) << __func__ << ", FakeDataCommand::StartJson";
            if (v.stringValue.empty()) {
                LOG(ERROR) << __func__ << ": path to JSON file is missing";
                return StatusCode::INVALID_ARG;
            }
            int32_t cookie = std::hash<std::string>()(v.stringValue);
            getGenerator()->registerGenerator(cookie,
                                              std::make_unique<JsonFakeValueGenerator>(request));
            break;
        }
        case FakeDataCommand::StopLinear: {
            LOG(INFO) << __func__ << ", FakeDataCommand::StopLinear";
            if (v.int32Values.size() < 2) {
                LOG(ERROR) << __func__ << ": expected property ID in int32Values";
                return StatusCode::INVALID_ARG;
            }
            int32_t cookie = v.int32Values[1];
            getGenerator()->unregisterGenerator(cookie);
            break;
        }
        case FakeDataCommand::StopJson: {
            LOG(INFO) << __func__ << ", FakeDataCommand::StopJson";
            if (v.stringValue.empty()) {
                LOG(ERROR) << __func__ << ": path to JSON file is missing";
                return StatusCode::INVALID_ARG;
            }
            int32_t cookie = std::hash<std::string>()(v.stringValue);
            getGenerator()->unregisterGenerator(cookie);
            break;
        }
        case FakeDataCommand::KeyPress: {
            LOG(INFO) << __func__ << ", FakeDataCommand::KeyPress";
            int32_t keyCode = request.value.int32Values[2];
            int32_t display = request.value.int32Values[3];
            // Send back to HAL
            onPropertyValueFromCar(
                    *createHwInputKeyProp(VehicleHwKeyInputAction::ACTION_DOWN, keyCode, display),
                    updateStatus);
            onPropertyValueFromCar(
                    *createHwInputKeyProp(VehicleHwKeyInputAction::ACTION_UP, keyCode, display),
                    updateStatus);
            break;
        }
        default: {
            LOG(ERROR) << __func__ << ": unexpected command: " << toInt(command);
            return StatusCode::INVALID_ARG;
        }
    }
    return StatusCode::OK;
}

VehicleHalServer::VehiclePropValuePtr VehicleHalServer::createApPowerStateReq(
        VehicleApPowerStateReq state, int32_t param) {
    auto req = getValuePool()->obtain(VehiclePropertyType::INT32_VEC, 2);
    req->prop = toInt(VehicleProperty::AP_POWER_STATE_REQ);
    req->areaId = 0;
    req->timestamp = elapsedRealtimeNano();
    req->status = VehiclePropertyStatus::AVAILABLE;
    req->value.int32Values[0] = toInt(state);
    req->value.int32Values[1] = param;
    return req;
}

VehicleHalServer::VehiclePropValuePtr VehicleHalServer::createHwInputKeyProp(
        VehicleHwKeyInputAction action, int32_t keyCode, int32_t targetDisplay) {
    auto keyEvent = getValuePool()->obtain(VehiclePropertyType::INT32_VEC, 3);
    keyEvent->prop = toInt(VehicleProperty::HW_KEY_INPUT);
    keyEvent->areaId = 0;
    keyEvent->timestamp = elapsedRealtimeNano();
    keyEvent->status = VehiclePropertyStatus::AVAILABLE;
    keyEvent->value.int32Values[0] = toInt(action);
    keyEvent->value.int32Values[1] = keyCode;
    keyEvent->value.int32Values[2] = targetDisplay;
    return keyEvent;
}

void VehicleHalServer::setFMCommandToMCU(std::string fm_ctrl, std::string mode, std::string value){
    Json::Value second;
    second["fm_ctrl"] = fm_ctrl;
    second["mode"] = mode;
    second["value"] = value;
    Json::StreamWriterBuilder builder;
    builder["indentation"] = ""; // 设置缩进为空字符串，即不缩进
    //std::string json_str = Json::writeString(builder, second);
    Json::Value final_root;
    final_root["FM"] = second;
    std::string final_json_str = Json::writeString(builder, final_root);
    LOG(INFO) <<"setFMCommandToMCU -- str:"<<final_json_str;
    std::vector<uint8_t> vec(final_json_str.begin(), final_json_str.end());
    mPropertyManager->jsonQueueEnqueue(vec);
}
uint32_t VehicleHalServer::getScheChargeWakedata()
{
    uint32_t value = 0;
    uint32_t sunday_loop = mPropertyManager->getCProValue(mPropertyManager->findCProperty("sunday_loop"));
    uint32_t monday_loop = mPropertyManager->getCProValue(mPropertyManager->findCProperty("monday_loop"));
    uint32_t tuesday_loop = mPropertyManager->getCProValue(mPropertyManager->findCProperty("tuesday_loop"));
    uint32_t wednesday_loop = mPropertyManager->getCProValue(mPropertyManager->findCProperty("wednesday_loop"));
    uint32_t thursday_loop = mPropertyManager->getCProValue(mPropertyManager->findCProperty("thursday_loop"));
    uint32_t friday_loop = mPropertyManager->getCProValue(mPropertyManager->findCProperty("friday_loop"));
    uint32_t saturday_loop = mPropertyManager->getCProValue(mPropertyManager->findCProperty("saturday_loop"));

    value |= sunday_loop << 0;
    value |= monday_loop << 1;
    value |= tuesday_loop << 2;
    value |= wednesday_loop << 3;
    value |= thursday_loop << 4;
    value |= friday_loop << 5;
    value |= saturday_loop << 6;

    return value;
}
void VehicleHalServer::setappointmentChragingToMCU(int soc_value, uint32_t week)
{
    Json::Value second;
    if (0 == soc_value) {
        uint32_t appointment_switch = mPropertyManager->getCProValue(mPropertyManager->findCProperty("sc_chraging_mode_req"));
        second["ON"] = std::to_string(appointment_switch);
        uint32_t appointment_mode = mPropertyManager->getCProValue(mPropertyManager->findCProperty("appointment_mode"));
        second["CIRC"] = std::to_string(appointment_mode);

        uint32_t chraging_begin_time_hour = mPropertyManager->getCProValue(mPropertyManager->findCProperty("chraging_begin_time_hour"));
        uint32_t chraging_begin_time_minute = mPropertyManager->getCProValue(mPropertyManager->findCProperty("chraging_begin_time_minute"));
        std::string beginTime;
        beginTime += std::to_string(chraging_begin_time_hour);
        beginTime += ":";
        beginTime += std::to_string(chraging_begin_time_minute);
        second["Start"] =  beginTime;

        uint32_t chraging_end_time_hour = mPropertyManager->getCProValue(mPropertyManager->findCProperty("chraging_end_time_hour"));
        uint32_t chraging_end_time_minute = mPropertyManager->getCProValue(mPropertyManager->findCProperty("chraging_end_time_minute"));
        std::string endTime;
        endTime += std::to_string(chraging_end_time_hour);
        endTime += ":";
        endTime += std::to_string(chraging_end_time_minute);
        second["End"] =  endTime;
        #if 0
        uint32_t sunday_loop = mPropertyManager->getCProValue(mPropertyManager->findCProperty("sunday_loop"));
        uint32_t monday_loop = mPropertyManager->getCProValue(mPropertyManager->findCProperty("monday_loop"));
        uint32_t tuesday_loop = mPropertyManager->getCProValue(mPropertyManager->findCProperty("tuesday_loop"));
        uint32_t wednesday_loop = mPropertyManager->getCProValue(mPropertyManager->findCProperty("wednesday_loop"));
        uint32_t thursday_loop = mPropertyManager->getCProValue(mPropertyManager->findCProperty("thursday_loop"));
        uint32_t friday_loop = mPropertyManager->getCProValue(mPropertyManager->findCProperty("friday_loop"));
        uint32_t saturday_loop = mPropertyManager->getCProValue(mPropertyManager->findCProperty("saturday_loop"));

        uint32_t value = 0;
        value |= sunday_loop << 0;
        value |= monday_loop << 1;
        value |= tuesday_loop << 2;
        value |= wednesday_loop << 3;
        value |= thursday_loop << 4;
        value |= friday_loop << 5;
        value |= saturday_loop << 6;
        #endif
        second["WEEK"] = std::to_string(week);

        uint32_t target_soc = mPropertyManager->getCProValue(mPropertyManager->findCProperty("chraging_target_soc"));
        second["TGTSoc"] = std::to_string(target_soc);
    }
    else {
        uint32_t target_soc = mPropertyManager->getCProValue(mPropertyManager->findCProperty("chraging_target_soc"));
        second["TGTSoc"] = std::to_string(target_soc);
    }

    Json::StreamWriterBuilder builder;
    builder["indentation"] = ""; // 设置缩进为空字符串，即不缩进
    std::string json_str = Json::writeString(builder, second);

    Json::Value final_root;
    final_root["SchCHG"] = json_str;

    std::string final_json_str = Json::writeString(builder, final_root);
    LOG(INFO) <<"setappointmentChragingToMCU -- str:"<<final_json_str;
    std::vector<uint8_t> vec(final_json_str.begin(), final_json_str.end());
    mPropertyManager->jsonQueueEnqueue(vec);
}

void VehicleHalServer::sendMultipleJson(std::string root, std::string child, std::string value)
{
    Json::Value second;
    second[child.c_str()] = value;

    Json::StreamWriterBuilder builder;
    builder["indentation"] = ""; // 设置缩进为空字符串，即不缩进
    //std::string json_str = Json::writeString(builder, second);

    Json::Value final_root;
    final_root[root.c_str()] = second;

    std::string final_json_str = Json::writeString(builder, final_root);

    LOG(INFO) <<"sendMultipleJson -- str:"<<final_json_str;
    std::vector<uint8_t> vec(final_json_str.begin(), final_json_str.end());
    mPropertyManager->jsonQueueEnqueue(vec);
}

float VehicleHalServer::getActualFloatValue(int32_t raw_value, float factor, int offset) {
    return static_cast<float>(raw_value * factor + offset);
}

static std::string intToHexString(int num) {
    std::stringstream ss;
    ss << std::setw(2) << std::setfill('0') <<std::hex << num;
    std::string result = ss.str();
    return result;
}
static Json::Value mSecond;
static bool filledStrength = false;
static bool wifiStrengthChanged = false;
static int32_t oldWifiStatus = -1;
static int32_t oldWifiStrength = -1;
static int32_t oldSimCardSts = -1;
StatusCode VehicleHalServer::onSetProperty(const VehiclePropValue& propValue, bool updateStatus) {
    // �յ�
    int32_t propId = propValue.prop;
    LOG(INFO) << "dxy----propId=" << propId;
    switch (propId) {      
        case toInt(VehicleProperty::GECKO_CCM_FM_SEARCH): //seek搜索
        {
            LOG(INFO) << "GECKO_CCM_FM_SEARCH:" << propValue.value.int32Values[0];
            std::string key = "fm_ctrl";
            std::string value = "02";
            value += intToHexString(propValue.value.int32Values[0]);
            mPropertyManager->singleJsonEnQueue(key, value);
        }break;
        //case toInt(VehicleProperty::GECKO_CCM_PREVIOUS_CHANNEL): 
        case toInt(VehicleProperty::GECKO_CCM_FM_VOLUME): //FM音量
        {
            LOG(INFO) << "GECKO_CCM_FM_VOLUME:" << propValue.value.int32Values[0];
            std::string key = "fm_ctrl";
            std::string value = "01";
            value += intToHexString(propValue.value.int32Values[0]);
            mPropertyManager->singleJsonEnQueue(key, value);
            mPropertyManager->updateCPropertyValue("fm_volume", propValue.value.int32Values[0]);
        } break;
        case toInt(VehicleProperty::GECKO_CCM_PLAY_CHANNEL): 
        {
            LOG(INFO) << "GECKO_CCM_PLAY_CHANNEL:" << propValue.value.int32Values[0];
            std::string key = "fm_ctrl";
            std::string value = "02";
            value += intToHexString(propValue.value.int32Values[0]);
            mPropertyManager->singleJsonEnQueue(key, value);
        } break;
        //case toInt(VehicleProperty::GECKO_CCM_PAUSE_CHANNEL): 
        //case toInt(VehicleProperty::GECKO_CCM_COLLECT_CHANNEL): 
        case toInt(VehicleProperty::GECKO_CCM_ADJUST_CHANNELS): { //指定频率下发
            LOG(INFO) << "GECKO_CCM_ADJUST_CHANNELS:" << propValue.value.int32Values[0];
            std::string key = "fm_ctrl";
            std::string value = "03";
            value += intToHexString(propValue.value.int32Values[0]);
            mPropertyManager->singleJsonEnQueue(key, value);
        } break;
        case toInt(VehicleProperty::GECKO_FM_AUTO_SEEK): {  //auto搜索
            LOG(INFO) << "GECKO_FM_AUTO_SEEK:" << propValue.value.int32Values[0];
            std::string key = "fm_ctrl";
            std::string value = "04";
            value += intToHexString(propValue.value.int32Values[0]);
            mPropertyManager->singleJsonEnQueue(key, value);
        } break;
        case toInt(VehicleProperty::GECKO_SOC_POWER_STATUS): {  // 557850898
            uint16_t messageID = 0x0D;
            BusIdentifier busIdentifier = BusIdentifier::MCU;
            uint8_t command = 0x02;
            uint8_t responseFlag = 0x00;
            mcu_soc_power_status mcuSocPowerStatus;
            mcuSocPowerStatus.mcu_power_status = 0;
            mcuSocPowerStatus.soc_power_status = propValue.value.int32Values[0];
            MessageParser mparse;
            LOG(INFO) << "dxy----GECKO_SOC_POWER_STATUS makePack done";
            std::vector<uint8_t> message =
                    mparse.makePack(messageID, static_cast<void*>(&mcuSocPowerStatus), busIdentifier,
                                    command, responseFlag);
            //SendQueue::getInstance()->enqueue(message);
            SendCanMap::getInstance()->insert_data(messageID, message, busIdentifier);
        } break;
        case AP_POWER_STATE_REQ:{
            LOG(INFO) << "dxy---- to set AP_POWER_STATE_REQ";
            auto updatedPropValue = getValuePool()->obtain(propValue);
            updatedPropValue->timestamp = elapsedRealtimeNano();
            mServerSidePropStore.writeValue(*updatedPropValue, updateStatus);
            onPropertyValueFromCar(*updatedPropValue, updateStatus);
        }break;
        case AP_POWER_STATE_REPORT:{
            switch (propValue.value.int32Values[0]) {
                case toInt(VehicleApPowerStateReport::DEEP_SLEEP_EXIT):{
                    LOG(INFO) << "dxy---- to set AP_POWER_STATE_REPORT DEEP_SLEEP_EXIT " << propValue.value.int32Values[0];
                }
                break;
                case toInt(VehicleApPowerStateReport::SHUTDOWN_CANCELLED):{
                    LOG(INFO) << "dxy---- to set AP_POWER_STATE_REPORT SHUTDOWN_CANCELLED " << propValue.value.int32Values[0];
                }
                break;
                case toInt(VehicleApPowerStateReport::WAIT_FOR_VHAL):
                    LOG(INFO) << "dxy---- to set AP_POWER_STATE_REPORT WAIT_FOR_VHAL " << propValue.value.int32Values[0];
                    onPropertyValueFromCar(*createApPowerStateReq(VehicleApPowerStateReq::ON, 0),
                                           true /* updateStatus */);
                break;
                case toInt(VehicleApPowerStateReport::DEEP_SLEEP_ENTRY):
                {
                    LOG(INFO) << "dxy---- to set AP_POWER_STATE_REPORT DEEP_SLEEP_ENTRY " << propValue.value.int32Values[0];
                }
                break;
                case toInt(VehicleApPowerStateReport::SHUTDOWN_START):
                {
                    LOG(INFO) << "dxy---- to set AP_POWER_STATE_REPORT SHUTDOWN_START" << propValue.value.int32Values[0];
                }
                break;
                case toInt(VehicleApPowerStateReport::ON):{
                    LOG(INFO) << "dxy---- to set AP_POWER_STATE_REPORT ON " << propValue.value.int32Values[0];
                    mPropertyManager->set_soc_power_on_ready(1);
                }break;
                case toInt(VehicleApPowerStateReport::SHUTDOWN_POSTPONE):{
                    LOG(INFO) << "dxy---- to set AP_POWER_STATE_REPORT SHUTDOWN_POSTPONE value = " << propValue.value.int32Values[0];
		    mPropertyManager->set_postpone_counter(propValue.value.int32Values[1]);
                }break;
                case toInt(VehicleApPowerStateReport::SHUTDOWN_PREPARE):{
                    LOG(INFO) << "dxy---- to set AP_POWER_STATE_REPORT SHUTDOWN_PREPARE " << propValue.value.int32Values[0];
                }
                break;
                case toInt(VehicleApPowerStateReport::SYS_SHUTDOWN_REBOOT):{
                    mPropertyManager->set_soc_power_on_ready(0);
                    LOG(INFO) << "dxy---- to set SYS_SHUTDOWN_REBOOT";
                }
                break;
                default:
                    // Unknown state
                    break;
            }
            auto updatedPropValue = getValuePool()->obtain(propValue);
            updatedPropValue->timestamp = elapsedRealtimeNano();
            mServerSidePropStore.writeValue(*updatedPropValue, updateStatus);
            onPropertyValueFromCar(*updatedPropValue, updateStatus);
        } break;                                               


        case toInt(VehicleProperty::HVAC_TEMPERATURE_SET): {  // 358614275: {
            LOG(INFO) << "dxy----ccm_left_set_temp makePack done";
            LOG(INFO) << "zcy----temp="
                      << std::to_string(static_cast<int>(propValue.value.floatValues[0]));
            mPropertyManager->updateCPropertyValue("sc_ccm_left_set_temp", static_cast<int>(propValue.value.floatValues[0]));
            mConfigParser.saveInitialValueToJson(propId, propValue.value.floatValues[0]);  //save prop value to json demo
        } break;
        case toInt(VehicleProperty::GECKO_REMA_RANGE_DISPLAY_MODE): {
            LOG(INFO) << "dxy---GECKO_REMA_RANGE_DISPLAY_MODE value is = " << std::to_string(propValue.value.int32Values[0]);
/*            auto updatedPropValue = getValuePool()->obtain(propValue);
            updatedPropValue->timestamp = elapsedRealtimeNano();
            mServerSidePropStore.writeValue(*updatedPropValue, updateStatus);
            onPropertyValueFromCar(*updatedPropValue, updateStatus);
            uint16_t messageID = 0x20;
            BusIdentifier busIdentifier = BusIdentifier::MCU;
            uint8_t command = 0x02;
            uint8_t responseFlag = 0x00;
            mcu_headlight_control_003_t light_height;
            light_height.light_status = propValue.value.int32Values[0];
            MessageParser mparse;
            LOG(INFO) << "dxy----REMA_DISPSET makePack done";
            std::vector<uint8_t> message =
                    mparse.makePack(messageID, static_cast<void*>(&light_height), busIdentifier,
                                    command, responseFlag);
            SendCanMap::getInstance()->insert_data(messageID, message, busIdentifier);
*/
            //singleJsonEnQueue(std::string& key, std::string& value)
            std::string key = "RangeMode";
            std::string value = std::to_string(propValue.value.int32Values[0]);
            mPropertyManager->singleJsonEnQueue(key, value);

            mPropertyManager->updateCPropertyValue("sc_rema_range_display_mode_down", value);
            // mPropertyManager->updateCAPropertyValue("sc_ccm_remt_ctrl_flag", std::to_string(propValue.value.int32Values[0]));
        }break;
        case toInt(VehicleProperty::GECKO_HVAC_PTC_HEAT_ON): {
            LOG(INFO) << "dxy----ccm_heat_key makePack done";
            mPropertyManager->updateCPropertyValue("sc_ccm_heat_key", propValue.value.int32Values[0]);
        } break;
        case toInt(VehicleProperty::HVAC_FAN_DIRECTION): {  // 356517121: {
            LOG(INFO) << "zcy----ccm_air_distribution_mode_req[1111]="
                      << std::to_string(static_cast<int>(propValue.value.int32Values[0]));
            mPropertyManager->updateCPropertyValue("sc_ccm_air_distribution_mode_req", propValue.value.int32Values[0]);
        } break;
        case toInt(VehicleProperty::HVAC_AC_ON): {  // 354419973: {
            LOG(INFO) << "dxy----ccm_a_ckey makePack done";
            mPropertyManager->updateCPropertyValue("sc_ccm_a_ckey", (propValue.value.int32Values[0] + 1));
        } break;
        case toInt(VehicleProperty::GECKO_HVAC_AC_STA): {  // 354419973: {
            LOG(INFO) << "dxy----sc_hu_ac_state makePack done";
            mPropertyManager->updateCPropertyValue("sc_hu_ac_state", propValue.value.int32Values[0]);
        } break;
        case toInt(VehicleProperty::HVAC_POWER_ON): {  // 354419984:
            LOG(INFO) << "dxy----ccm_ac_havc_state makePack done";
            mPropertyManager->updateCPropertyValue("sc_ccm_ac_havc_state", (propValue.value.int32Values[0] + 1));
        } break;
        case toInt(VehicleProperty::HVAC_DEFROSTER): {  // 320865540: {
            LOG(INFO) << "dxy----ccm_front_defrost_key makePack done";
            mPropertyManager->updateCPropertyValue("sc_ccm_front_defrost_key", (propValue.value.int32Values[0] + 1));
        } break;
        case toInt(VehicleProperty::HVAC_RECIRC_ON): {  // 354419976: {
            LOG(INFO) << "dxy----ccm_fresh_reci_key makePack done" << propValue.value.int32Values[0];
            int32_t value = 0;
            if(propValue.value.int32Values[0]==0) {
                value = 2;
            } else {
                value = 1;
            }

            mPropertyManager->updateCPropertyValue("sc_ccm_fresh_reci_key", value);
        } break;
        case toInt(VehicleProperty::HVAC_AUTO_RECIRC_ON): {  // 354419986: {
            LOG(INFO) << "dxy----ccm_fresh_reci_key makePack done";
            //mPropertyManager->updateCPropertyValue("sc_ccm_fresh_reci_key", propValue.value.int32Values[0]);
        } break;
        case toInt(VehicleProperty::HVAC_FAN_SPEED): {  // 356517120: {
            LOG(INFO) << "dxy----ccm_blower_level makePack done";
            mPropertyManager->updateCPropertyValue("sc_ccm_blower_level", propValue.value.int32Values[0]);
        } break;
        case toInt(VehicleProperty::GECKO_HVAC_HYBIRD_ECO_MODE): { //: {
            LOG(INFO) << "dxy----hybird_eco_mode makePack done";
            mPropertyManager->updateCPropertyValue("sc_hybird_eco_mode", propValue.value.int32Values[0]);
        } break;
        case toInt(VehicleProperty::FOG_LIGHTS_SWITCH): { //289410578
        //case toInt(VehicleProperty::FOG_LIGHTS_STATE): {  289410562
            LOG(INFO) << "dxy----ccm_rear_fog_lamp_set makePack done";
            LOG(INFO) << "zcy----ccm_rear_fog_lamp_set[000]=" << std::to_string(propValue.value.int32Values[0]);
            mPropertyManager->updateCAPropertyValue("sc_ccm_rear_fog_lamp_set", std::to_string(propValue.value.int32Values[0]));
        } break;
        case toInt(VehicleProperty::GECKO_VEHICLE_DRIVE_MODE):
        case toInt(VehicleProperty::GECKO_RIGHT_MIRROR_SET):
        case toInt(VehicleProperty::GECKO_LEFT_MIRROR_SET):
        case toInt(VehicleProperty::GECKO_AUTO_UNLOCK_SET):
        case toInt(VehicleProperty::GECKO_AUTO_LOCK_SET):
        case toInt(VehicleProperty::GECKO_PARK_UNLOCK_SET):
        case toInt(VehicleProperty::GECKO_SERVICE_REMIND_SET):
        case toInt(VehicleProperty::GECKO_SERVICE_REMIND_TIME_SET):
        case toInt(VehicleProperty::GECKO_SERVICE_REMIND_DISTANCE_SET):
        case toInt(VehicleProperty::GECKO_SERVICE_REMIND_START_ODO):
        case toInt(VehicleProperty::GECKO_SINGLE_PED_STATE):
        case toInt(VehicleProperty::GECKO_CREEP_STATE):
        case toInt(VehicleProperty::GECKO_LOW_SPEED_PEDESTRIAN_WARNING):
        case toInt(VehicleProperty::GECKO_BCM_DRIVELOCKST):
        case toInt(VehicleProperty::GECKO_CCM_BCM_OPENDOOR_WARN_SET):
        case toInt(VehicleProperty::GECKO_VEHICLE_PAY_WARNING_ST):
        case toInt(VehicleProperty::GECKO_VEHICLE_ANNUAL_REVIEW_WARNING_ST):
        case toInt(VehicleProperty::GECKO_VEHICLE_REMIND_WARNING_ST):
        {
            LOG(INFO) << "zcy----GECKO_SERVICE_REMIND_START_ODO propid=" << toInt(VehicleProperty::GECKO_SERVICE_REMIND_START_ODO);
            if(propId == toInt(VehicleProperty::GECKO_LEFT_MIRROR_SET)) {
                 mPropertyManager->updateCPropertyValue("sc_ccm_left_mirror_set", propValue.value.int32Values[0]);
            }else if(propId == toInt(VehicleProperty::GECKO_RIGHT_MIRROR_SET)) {
                 mPropertyManager->updateCPropertyValue("sc_ccm_right_mirror_set", propValue.value.int32Values[0]);
            }else if(propId == toInt(VehicleProperty::GECKO_VEHICLE_DRIVE_MODE)) {
                LOG(INFO) << "zcy----sc_ccm_drive_mode_req[000]=" << std::to_string(propValue.value.int32Values[0]);
                mPropertyManager->updateCPropertyValue("sc_ccm_drive_mode_req", propValue.value.int32Values[0]);
            }else if(propId == toInt(VehicleProperty::GECKO_AUTO_UNLOCK_SET)){
                mPropertyManager->updateCPropertyValue("sc_ccm_auto_unlock_req", propValue.value.int32Values[0]);
            }else if(propId == toInt(VehicleProperty::GECKO_AUTO_LOCK_SET)){
                mPropertyManager->updateCPropertyValue("sc_ccm_auto_lock_req", propValue.value.int32Values[0]);
            }else if(propId == toInt(VehicleProperty::GECKO_PARK_UNLOCK_SET)){
                int32_t value = 0;
                if(propValue.value.int32Values[0]==0) {
                    value = 2;
                } else {
                    value = 1;
                }
                mPropertyManager->updateCPropertyValue("sc_ccm_park_un_lock_req", value);
            }else if(propId == toInt(VehicleProperty::GECKO_SINGLE_PED_STATE)){
                //mPropertyManager->updateCPropertyValue("sc_ccm_single_ped_ctr_cmd", propValue.value.int32Values[0]);
            }else if(propId == toInt(VehicleProperty::GECKO_CREEP_STATE)){
                int32_t value = 0;
                if(propValue.value.int32Values[0]==0) {
                    value = 2;
                } else {
                    value = 1;
                }
                mPropertyManager->updateCPropertyValue("sc_ccm_creep_ctr_cmd", value);
            }else if(propId == toInt(VehicleProperty::GECKO_LOW_SPEED_PEDESTRIAN_WARNING)){
                LOG(INFO) << "dxy----debug GECKO_LOW_SPEED_PEDESTRIAN_WARNING value is" << std::to_string(propValue.value.int32Values[0]);
                mPropertyManager->updateCPropertyValue("sc_ccm_avasset", propValue.value.int32Values[0]);
            }else if(propId == toInt(VehicleProperty::GECKO_BCM_DRIVELOCKST)) {
                LOG(INFO) << "zcy----debug GECKO_BCM_DRIVELOCKST value is" << std::to_string(propValue.value.int32Values[0]);
                //mPropertyManager->updateCPropertyValue("sc_ccm_central_lock_switch", propValue.value.int32Values[0]);
                std::string key = "CCMLock";
                std::string value = std::to_string(propValue.value.int32Values[0]);
                mPropertyManager->singleJsonEnQueue(key, value);
            } else if(propId == toInt(VehicleProperty::GECKO_CCM_BCM_OPENDOOR_WARN_SET)) {
                LOG(INFO) << "zcy----debug GECKO_CCM_BCM_OPENDOOR_WARN_SET value is" << std::to_string(propValue.value.int32Values[0]);
                mPropertyManager->updateCPropertyValue("sc_ccm_bcm_open_door_warn_set", propValue.value.int32Values[0]);
            } else {
                if(propId == toInt(VehicleProperty::GECKO_SERVICE_REMIND_TIME_SET)) {
                    std::chrono::seconds s_start = std::chrono::duration_cast< std::chrono::seconds >(
                          std::chrono::system_clock::now().time_since_epoch());
                    int32_t times = s_start.count();
                    sendMultipleJson("Mtce", "INR", std::to_string(propValue.value.int32Values[0]));
                    sendMultipleJson("Mtce", "StartTime", std::to_string(times));
                    mPropertyManager->updateCPropertyValue("mc_remind_start_time", times);
		            LOG(INFO) <<"do GECKO_SERVICE_REMIND_TIME_SET -- starttime:"<<std::to_string(times)<<";INR:"<<std::to_string(propValue.value.int32Values[0]);
                } else if(propId == toInt(VehicleProperty::GECKO_SERVICE_REMIND_DISTANCE_SET)) {
                    sendMultipleJson("Mtce", "DIS", std::to_string(propValue.value.int32Values[0]));
                    LOG(INFO) <<"do GECKO_SERVICE_REMIND_DISTANCE_SET -- DIS:"<<std::to_string(propValue.value.int32Values[0]);
                } else if(propId == toInt(VehicleProperty::GECKO_SERVICE_REMIND_START_ODO)) {
                    sendMultipleJson("Mtce", "StartODO", std::to_string(static_cast<int>(propValue.value.floatValues[0])));
                    LOG(INFO) <<"do GECKO_SERVICE_REMIND_START_ODO -- startodo:"<<std::to_string(propValue.value.floatValues[0]);
                } else if((propId == toInt(VehicleProperty::GECKO_SERVICE_REMIND_SET)) && (propValue.value.int32Values[0] == 2)) { // for 维修保养查询使用
                    Json::Value root;
                    root["Mtce"] = "?";
                    // 输出JSON字符串
                    Json::StreamWriterBuilder builder;
                    std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
                    std::ostringstream oss;
                    writer->write(root, &oss);
                    // 获取字符串
                    std::string str = oss.str();
                    // 将字符串转换为 std::vector<uint8_t>
                    LOG(INFO) <<"do SERVICE_REMIND_QUERY -- str:"<<str;
                    std::vector<uint8_t> vec(str.begin(), str.end());
                    mPropertyManager->jsonQueueEnqueue(vec);
                    return StatusCode::OK;
                }
                auto updatedPropValue = getValuePool()->obtain(propValue);
                updatedPropValue->timestamp = elapsedRealtimeNano();
                mServerSidePropStore.writeValue(*updatedPropValue, updateStatus);
                onPropertyValueFromCar(*updatedPropValue, updateStatus);
                return StatusCode::OK;
            }
        } break;
        case toInt(VehicleProperty::INFO_VIN): {
                //LOG(INFO) << "zcy----save INFO_VIN To Json[BEGIN]=" << propValue.value.stringValue << "----propId=" << propId;
                //mConfigParser.saveInitialValueToJson(propId, propValue.value.stringValue.c_str());  //save prop value to json
                //LOG(INFO) << "zcy----save INFO_VIN To Json[END]=" << propValue.value.stringValue << "----propId=" << propId;
            } break;
        case toInt(VehicleProperty::GECKO_CCM_REMT_CTR_FLAG):{
                LOG(INFO) << "dxy---GECKO_CCM_REMT_CTR_FLAG value is = " << std::to_string(propValue.value.int32Values[0]);
                mPropertyManager->updateCAPropertyValue("sc_ccm_remt_ctrl_flag", std::to_string(propValue.value.int32Values[0]));
            }
            break;
        case toInt(VehicleProperty::GECKO_CCM_TARGET_CHRG_CURR):{
                mPropertyManager->updateCAPropertyValue("sc_ec_ccm_target_chrg_curr", std::to_string(propValue.value.int32Values[0]));
        } break;    
        case toInt(VehicleProperty::GECKO_HIGH_BEAM_LIGHTS_LEVEL): {
/*            
            uint16_t messageID = 0x03;
            BusIdentifier busIdentifier = BusIdentifier::MCU;
            uint8_t command = 0x02;
            uint8_t responseFlag = 0x00;
            mcu_headlight_control_003_t light_height;
            light_height.light_status = propValue.value.int32Values[0];
            MessageParser mparse;
            LOG(INFO) << "dxy----HIGH_BEAM_LIGHTS_LEVEL makePack done";
            std::vector<uint8_t> message =
                    mparse.makePack(messageID, static_cast<void*>(&light_height), busIdentifier,
                                    command, responseFlag);
            //SendQueue::getInstance()->enqueue(message);
            SendCanMap::getInstance()->insert_data(messageID, message, busIdentifier);
*/
            std::string key = "HeadlightH";
            std::string value = std::to_string(propValue.value.int32Values[0]);
            mPropertyManager->singleJsonEnQueue(key, value);

            auto updatedPropValue = getValuePool()->obtain(propValue);
            updatedPropValue->timestamp = elapsedRealtimeNano();
            mServerSidePropStore.writeValue(*updatedPropValue, updateStatus);
            onPropertyValueFromCar(*updatedPropValue, updateStatus);

        } break;
        case toInt(VehicleProperty::GECKO_CCM_REMOTE_CAR_SEARCH):{              //远程寻车
                LOG(INFO) << "GECKO_CCM_REMOTE_CAR_SEARCH: "<<propValue.value.int32Values[0];
                mPropertyManager->updateCPropertyValue("sc_ccm_remote_car_search", propValue.value.int32Values[0]);
            }
            break;
        case toInt(VehicleProperty::GECKO_CCM_REMOTE_LOCK):{              //远程锁车功能 提供给tbox 下设用的
           mPropertyManager->updateCPropertyValue("sc_ccm_tbox_remote_lock", propValue.value.int32Values[0]);
        }break;
        case toInt(VehicleProperty::GECKO_CCM_SPEED_LIMIT_LEVEL): {      //远控CCM下发限速等级
           LOG(INFO) << "wjj----GECKO_CCM_SPEED_LIMIT_LEVEL makePack done";
           mPropertyManager->updateCPropertyValue("sc_ec_ccm_speed_limit_level", propValue.value.int32Values[0]);
           mPropertyManager->updateCPropertyValue("set_value_flag", 1);
            LOG(INFO) << "GECKO_CCM_SPEED_LIMIT_LEVEL:" << propValue.value.int32Values[0];
            sendMultipleJson("SL", "SLLevel", std::to_string(propValue.value.int32Values[0]));
            std::vector<uint8_t> out_value(7, 0);
            int ret = 0;
            vehicle_vendor_storage_read(VENDOR_VEHICLE_HAL_FEATURE_ID, VEHICLE_HAL_FEATURE_LEN, out_value);
            LOG(INFO) << "GECKO_CCM_SPEED_LIMIT_LEVEL old value:" << out_value[3]; 
            if(out_value.size() > 6) {
                out_value[3] = (out_value[3] & 0x3F) | (static_cast<uint8_t> (propValue.value.int32Values[0]) <<6);
            }
            ret = vehicle_vendor_storage_write(VENDOR_VEHICLE_HAL_FEATURE_ID, VEHICLE_HAL_FEATURE_LEN, out_value);
            if(ret != 0) {
                ALOGE("write SLLevel vendor storage fail\n");
            }
        }break;
         case toInt(VehicleProperty::GECKO_CCM_VCU_VERSION): {      //远控下发限速锁车版本 0x0: 非锁车版本 0x1: 锁车版本
            LOG(INFO) << "wjj----GECKO_CCM_VCU_VERSION makePack done";
            mPropertyManager->updateCPropertyValue("sc_ccm_version", propValue.value.int32Values[0]);
            mPropertyManager->updateCPropertyValue("set_value_flag", 1);
            LOG(INFO) << "GECKO_CCM_VCU_VERSION:" << propValue.value.int32Values[0];
            sendMultipleJson("SL", "SLVersion", std::to_string(propValue.value.int32Values[0]));
            std::vector<uint8_t> out_value(7, 0);
            int ret = 0;
            vehicle_vendor_storage_read(VENDOR_VEHICLE_HAL_FEATURE_ID, VEHICLE_HAL_FEATURE_LEN, out_value);
            if(out_value.size() > 6) {
               out_value[3] = (out_value[3] & 0xDF) | (static_cast<uint8_t> (propValue.value.int32Values[0]) << 5);
            }
            ret = vehicle_vendor_storage_write(VENDOR_VEHICLE_HAL_FEATURE_ID, VEHICLE_HAL_FEATURE_LEN, out_value);
            if(ret != 0) {
                ALOGE("write SLVersion vendor storage fail\n");
            }
        }break;
        case toInt(VehicleProperty::GECKO_CCM_CHARGING_CONDIONS): {     //远控CCM下发充电条件 0x0: 判断充电条件 0x1: 不判断充电条件
            mPropertyManager->updateCPropertyValue("sc_ccm_charging_conditions", propValue.value.int32Values[0]);
            auto updatedPropValue = getValuePool()->obtain(propValue);
            updatedPropValue->timestamp = elapsedRealtimeNano();
            mServerSidePropStore.writeValue(*updatedPropValue, updateStatus);
            onPropertyValueFromCar(*updatedPropValue, updateStatus);
            LOG(INFO) << "GECKO_CCM_CHARGING_CONDIONS:" << propValue.value.int32Values[0];
            sendMultipleJson("SL", "CHGCOND", std::to_string(propValue.value.int32Values[0]));
            std::vector<uint8_t> out_value(7, 0);
            int ret = 0;
            vehicle_vendor_storage_read(VENDOR_VEHICLE_HAL_FEATURE_ID, VEHICLE_HAL_FEATURE_LEN, out_value);
            if(out_value.size() > 6) {
               out_value[3] = (out_value[3] & 0xF7) | (static_cast<uint8_t> (propValue.value.int32Values[0]) << 3);
            }
            ret = vehicle_vendor_storage_write(VENDOR_VEHICLE_HAL_FEATURE_ID, VEHICLE_HAL_FEATURE_LEN, out_value);
            if(ret != 0) {
                ALOGE("write CHGCOND vendor storage fail\n");
            }
        }break;
       case toInt(VehicleProperty::GECKO_CCM_OPERAT_LICENCE): {     //远控CCM下发锁车指令许可 0x0: 允许运营 0x1: 禁止运营
            mPropertyManager->updateCPropertyValue("sc_ccm_operat_licence", propValue.value.int32Values[0]);
            auto updatedPropValue = getValuePool()->obtain(propValue);
            updatedPropValue->timestamp = elapsedRealtimeNano();
            mServerSidePropStore.writeValue(*updatedPropValue, updateStatus);
            onPropertyValueFromCar(*updatedPropValue, updateStatus);
            LOG(INFO) << "GECKO_CCM_OPERAT_LICENCE:" << propValue.value.int32Values[0];
            sendMultipleJson("SL", "SLOL", std::to_string(propValue.value.int32Values[0]));
            std::vector<uint8_t> out_value(7, 0);
            int ret = 0;
            vehicle_vendor_storage_read(VENDOR_VEHICLE_HAL_FEATURE_ID, VEHICLE_HAL_FEATURE_LEN, out_value);
            if(out_value.size() > 6) {
               out_value[3] = (out_value[3] & 0xEF) | (static_cast<uint8_t> (propValue.value.int32Values[0]) << 4);
            }
            ret = vehicle_vendor_storage_write(VENDOR_VEHICLE_HAL_FEATURE_ID, VEHICLE_HAL_FEATURE_LEN, out_value);
            if(ret != 0) {
                ALOGE("write SLOL vendor storage fail\n");
            }
        }break;

        case toInt(VehicleProperty::GECKO_CHRAGINGMODESP): {      //预约充电 充电模式 0：立即充电，01：预约充电
            mPropertyManager->updateCPropertyValue("sc_chraging_mode_req", propValue.value.int32Values[0]);
            LOG(INFO)<<"SchCHG sc_chraging_mode_req:"<<propValue.value.int32Values[0];
            if(propValue.value.int32Values[0] == 0){
                //立即充电
                sendMultipleJson("SchCHG", "ON", std::to_string(propValue.value.int32Values[0]));
            }
            else {
                uint32_t week = getScheChargeWakedata();
                if (week == 0) {
                    sendMultipleJson("SchCHG", "ON", "1");
                }
                else {
                    //预约充电
                    setappointmentChragingToMCU(0, week);
                }
            }
        }
        break;

        case toInt(VehicleProperty::GECKO_CCM_APPOINTMENT_MODE): {      //（预约充电方式）00：单次，01：循环
            mPropertyManager->updateCPropertyValue("appointment_mode", propValue.value.int32Values[0]);
            LOG(INFO)<<"SchCHG appointment_mode:"<<propValue.value.int32Values[0];
            // appointmentChragingCount++;
            // setappointmentChragingToMCU();
        }
        break;
        case toInt(VehicleProperty::GECKO_CCM_CHRAGING_BEGIN_TIME_HOUR): {      //预约充电开始时间（时）范围：00 ~ 23
            mPropertyManager->updateCPropertyValueNoCheck("chraging_begin_time_hour", propValue.value.int32Values[0]);
            LOG(INFO)<<"SchCHG chraging_begin_time_hour:"<<propValue.value.int32Values[0];

            // appointmentChragingCount++;
            // setappointmentChragingToMCU();
        }
        break;

        case toInt(VehicleProperty::GECKO_CCM_CHRAGING_BEGIN_TIME_MINUTE): {      //预约充电开始时间（分）范围：00 ~ 59
            mPropertyManager->updateCPropertyValueNoCheck("chraging_begin_time_minute", propValue.value.int32Values[0]);
            LOG(INFO)<<"SchCHG chraging_begin_time_minute:"<<propValue.value.int32Values[0];

            // appointmentChragingCount++;
            // setappointmentChragingToMCU();
        }
        break;
        case toInt(VehicleProperty::GECKO_CCM_CHRAGING_END_TIME_HOUR): {      //预约充电结束时间（时）	范围：00 ~ 23
            mPropertyManager->updateCPropertyValueNoCheck("chraging_end_time_hour", propValue.value.int32Values[0]);

            LOG(INFO)<<"SchCHG chraging_end_time_hour:"<<propValue.value.int32Values[0];
            // appointmentChragingCount++;
            // setappointmentChragingToMCU();
        }
        break;
        case toInt(VehicleProperty::GECKO_CCM_CHRAGING_END_TIME_MINUTE): {      //预约充电结束时间（分）	范围：00 ~ 59
            mPropertyManager->updateCPropertyValueNoCheck("chraging_end_time_minute", propValue.value.int32Values[0]);

            LOG(INFO)<<"SchCHG chraging_end_time_minute:"<<propValue.value.int32Values[0];
            // appointmentChragingCount++;
            // setappointmentChragingToMCU();
        }
        break;
        case toInt(VehicleProperty::GECKO_CCM_CHRAGING_LOOP): {      //预约充电 循环启用标志	按如下顺序定义：周日、周一、周二、周三、周四、周五、周六
            mPropertyManager->updateCPropertyValue("sunday_loop", propValue.value.int32Values[0]);
            mPropertyManager->updateCPropertyValue("monday_loop", propValue.value.int32Values[1]);
            mPropertyManager->updateCPropertyValue("tuesday_loop", propValue.value.int32Values[2]);
            mPropertyManager->updateCPropertyValue("wednesday_loop", propValue.value.int32Values[3]);
            mPropertyManager->updateCPropertyValue("thursday_loop", propValue.value.int32Values[4]);
            mPropertyManager->updateCPropertyValue("friday_loop", propValue.value.int32Values[5]);
            mPropertyManager->updateCPropertyValue("saturday_loop", propValue.value.int32Values[6]);
            
            LOG(INFO)<<"SchCHG sunday_loop:"<<propValue.value.int32Values[0];
            // appointmentChragingCount++;
            // setappointmentChragingToMCU();
        }
        break;

        case toInt(VehicleProperty::GECKO_CCM_CHRAGING_TARGET_SOC): {      //充电目标电量	"范围：0 ~ 100 
            mPropertyManager->updateCPropertyValue("sc_charging_full_soc", propValue.value.int32Values[0]);

            LOG(INFO)<<"SchCHG sc_charging_full_soc:"<<propValue.value.int32Values[0];
            std::vector<uint8_t> out_value(7, 0);
            int ret = 0;
            vehicle_vendor_storage_read(VENDOR_VEHICLE_HAL_FEATURE_ID, VEHICLE_HAL_FEATURE_LEN, out_value);
            if(out_value.size() > 6) {
               out_value[6] =  static_cast<uint8_t> (propValue.value.int32Values[0]);
            }
            ret = vehicle_vendor_storage_write(VENDOR_VEHICLE_HAL_FEATURE_ID, VEHICLE_HAL_FEATURE_LEN, out_value);
            if(ret != 0) {
                ALOGE("write BRKDISWIP vendor storage fail\n");
            }
            //appointmentChragingCount++;
            //setappointmentChragingToMCU(propValue.value.int32Values[0]);
        }
        break;
        case toInt(VehicleProperty::GECKO_VCU_SMART_DC_ST):      //远控下发智能补电配置
        case toInt(VehicleProperty::GECKO_SMART_DC_QUERY): 
        {
            Json::Value final_root;
            Json::Value second;
            if(propId == toInt(VehicleProperty::GECKO_VCU_SMART_DC_ST)) {
                second["StartV"] = std::to_string(propValue.value.int32Values[0]);
                second["EndV"] = std::to_string(propValue.value.int32Values[1]);
                second["RTime"] = std::to_string(propValue.value.int32Values[2]);
                final_root["Recharge"] = second;
                LOG(INFO) <<"do SMART_DC -- second[StartV]="<<second["StartV"] << ";second[EndV]="<<second["EndV"] << ";second[RTime]=" << second["RTime"];
            } else {
                final_root["Recharge"] = "?";
                LOG(INFO) <<"do SMART_DC_QUERY";
            }

            Json::StreamWriterBuilder builder;
            builder["indentation"] = ""; // 设置缩进为空字符串，即不缩进
            //std::string json_str = Json::writeString(builder, second);

            std::string final_json_str = Json::writeString(builder, final_root);

            LOG(INFO) <<"do SMART_DC -- str:"<<final_json_str;
            std::vector<uint8_t> vec(final_json_str.begin(), final_json_str.end());
            mPropertyManager->jsonQueueEnqueue(vec);
        } break;
        case toInt(VehicleProperty::GECKO_CCM_SCREEN_BRIGHTNESS_ADJUST):
        case toInt(VehicleProperty::GECKO_CCM_SCREEN_BRIGHTNESS_AUTO_ADJUST):
        {
            /*
            uint16_t messageID = 0x0009;
            BusIdentifier busIdentifier = BusIdentifier::MCU;
            uint8_t command = 0x02;
            uint8_t responseFlag = 0x00;
            MessageParser mparse;
            mcu_screen_brightness_adjust  screen_brightness_adjust;
            if(propId == toInt(VehicleProperty::GECKO_CCM_SCREEN_BRIGHTNESS_ADJUST)) {
                mPropertyManager->updateCPropertyValue("sc_ccm_light_level", propValue.value.int32Values[0]);
                screen_brightness_adjust.light_level = propValue.value.int32Values[0];
                screen_brightness_adjust.auto_adjust_state = 0;
            }else if(propId == toInt(VehicleProperty::GECKO_CCM_SCREEN_BRIGHTNESS_AUTO_ADJUST)) {
                screen_brightness_adjust.light_level = mPropertyManager->getCProValue(mPropertyManager->findCProperty("sc_ccm_light_level"));
                screen_brightness_adjust.auto_adjust_state = propValue.value.int32Values[0];
	    }
            LOG(INFO) << "dxy----gotoseting screenlight=" << std::to_string(propValue.value.int32Values[0]) << "----propId=" << propId;
            std::vector<uint8_t> message =
                    mparse.makePack(messageID, static_cast<void*>(&screen_brightness_adjust), busIdentifier,
                                    command, responseFlag);
            std::string buf;
            buf.clear();
            for (int i = 0; i < (int)message.size(); i++) {
                std::ostringstream ss;
                ss.clear();
                int data = message[i];
                ss << std::hex << data;
                buf += "0x";
                buf += ss.str();
                buf += ", ";
            }
            LOG(INFO) << "dxy----gotoseting screenlight buf=" << buf ;
            //SendQueue::getInstance()->enqueue(message);
            SendCanMap::getInstance()->insert_data(messageID, message, busIdentifier);
            */
	    if(propId == toInt(VehicleProperty::GECKO_CCM_SCREEN_BRIGHTNESS_ADJUST)) {
                mPropertyManager->updateCPropertyValue("sc_ccm_screen_brightness_adjust", propValue.value.int32Values[0]);
                sendMultipleJson("BackLight", "Value", std::to_string(propValue.value.int32Values[0]));
                LOG(INFO) << "zcy----gotoseting screenlight=" << std::to_string(propValue.value.int32Values[0]);
            } else if(propId == toInt(VehicleProperty::GECKO_CCM_SCREEN_BRIGHTNESS_AUTO_ADJUST)) {
                if(propValue.value.int32Values[0] == 0) {
                    std::string value = std::to_string(mPropertyManager->getCProValue(mPropertyManager->findCProperty("sc_ccm_screen_brightness_adjust")));
                    sendMultipleJson("BackLight", "Value", value);
                    LOG(INFO) << "zcy----gotoseting screenlight=" << value;
                } else {
                    sendMultipleJson("BackLight", "Value", std::to_string(propValue.value.int32Values[0]));
                }
            }
        } break;

        case toInt(VehicleProperty::GECKO_MCU_VOIUME_NAVIGATION):
        {
            sendMultipleJson("Vol", "Navi", std::to_string(propValue.value.int32Values[0]));

        } break;
        case toInt(VehicleProperty::GECKO_MCU_VOIUME_VOIC):
        {
            sendMultipleJson("Vol", "Voice", std::to_string(propValue.value.int32Values[0]));

        } break;
        case toInt(VehicleProperty::GECKO_MCU_VOIUME_MULTIMEDIA):
        {
            sendMultipleJson("Vol", "Multi", std::to_string(propValue.value.int32Values[0]));

        } break;
        case toInt(VehicleProperty::GECKO_MCU_VOIUME_PHONE):
        {
            sendMultipleJson("Vol", "Tel", std::to_string(propValue.value.int32Values[0]));

        } break;
        case toInt(VehicleProperty::GECKO_MCU_VOIUME_ALARM):
        {
            sendMultipleJson("Vol", "Alarm", std::to_string(propValue.value.int32Values[0]));

        } break;
        case toInt(VehicleProperty::GECKO_MCU_VOIUME_KEY):
        {
            sendMultipleJson("Vol", "Key", std::to_string(propValue.value.int32Values[0]));
        } break;
        case toInt(VehicleProperty::GECKO_CCM_ENERGY_RECOV_LEVEL_CMD): {
            LOG(INFO) << "dxy----GECKO_CCM_ENERGY_RECOV_LEVEL=" << std::to_string(propValue.value.int32Values[0]) << "----propId=" << propId;
            mPropertyManager->updateCPropertyValue("sc_ccm_energy_recov_level_cmd", propValue.value.int32Values[0]);
        } break;
        case toInt(VehicleProperty::GECKO_MCU_TRAVEL_INFO_RESET): {
            LOG(INFO) << "dxy----GECKO_MCU_TRAVEL_INFO_RESET=" << std::to_string(propValue.value.int32Values[0]) << "----propId=" << propId;
            Json::Value second;
            second["Maleage"] = "0";
            second["TRTime"] = "0";
            second["AvgEngC"] = "0";

            Json::StreamWriterBuilder builder;
            builder["indentation"] = ""; // 设置缩进为空字符串，即不缩进
            //std::string json_str = Json::writeString(builder, second);

            Json::Value final_root;
            final_root["TRAV"] = second;

            std::string final_json_str = Json::writeString(builder, final_root);

            LOG(INFO) <<"do GECKO_MCU_TRAVEL_INFO -- str:"<<final_json_str;
            std::vector<uint8_t> vec(final_json_str.begin(), final_json_str.end());
            mPropertyManager->jsonQueueEnqueue(vec);
        } break;
        case toInt(VehicleProperty::IGNITION_STATE):
        {
            mPropertyManager->updateCPropertyValue("sc_pwr_off_req", propValue.value.int32Values[0]);
        }break;
        case toInt(VehicleProperty::GECKO_WIPER_INTERVAL_TIME):
        {
            mPropertyManager->updateCPropertyValue("sc_wiper_interval_time", propValue.value.int32Values[0]);
        }break;
        case toInt(VehicleProperty::GECKO_ACCOMPANY_BACK_HOME_DURATION):
        {
            mPropertyManager->updateCPropertyValue("sc_flwr_me_hm_ctrl", propValue.value.int32Values[0]);
        }break;
        case toInt(VehicleProperty::GECKO_WELCOME_UNLOCK_STATUS):
        {
            mPropertyManager->updateCPropertyValue("sc_illmnd_unlock_sts", propValue.value.int32Values[0]);
        }break;
        case toInt(VehicleProperty::GECKO_ILLMND_LOCK_STATUS):
        {
            mPropertyManager->updateCPropertyValue("sc_illmnd_lock_sts", propValue.value.int32Values[0]);
        }break;
        case toInt(VehicleProperty::GECKO_POWER_STEERING_MODE):
        {
            mPropertyManager->updateCPropertyValue("sc_elec_steer_mode_set", propValue.value.int32Values[0]);
        }break;
        case toInt(VehicleProperty::GECKO_LOCK_HORN_ENABLE):
        {
            mPropertyManager->updateCPropertyValue("sc_lock_horn_on_st", propValue.value.int32Values[0]);
        }break;
        case toInt(VehicleProperty::GECKO_VEHICLE_STABILITY_ON):
        {
            mPropertyManager->updateCPropertyValue("sc_esc_func_ena_sta_resvd", propValue.value.int32Values[0]);
        }break;
        case toInt(VehicleProperty::GECKO_AUTO_PARKING_ON):
        {
            mPropertyManager->updateCPropertyValue("sc_avh_enb", propValue.value.int32Values[0]);
        }break;
        case toInt(VehicleProperty::GECKO_ELECTRONIC_PARKING_MODE):
        {
            mPropertyManager->updateCPropertyValue("sc_epb_modal", propValue.value.int32Values[0]);
        }break;
        case toInt(VehicleProperty::GECKO_BRAKE_DISC_WIPING_ON):
        {
            mPropertyManager->updateCPropertyValue("sc_brk_dis_wip_sw", propValue.value.int32Values[0]);
            LOG(INFO) << "set GECKO_BRAKE_DISC_WIPING_ON to:" << propValue.value.int32Values[0];
            std::vector<uint8_t> out_value(7, 0);
            int ret = 0;
            vehicle_vendor_storage_read(VENDOR_VEHICLE_HAL_FEATURE_ID, VEHICLE_HAL_FEATURE_LEN, out_value);
            if(out_value.size() > 6) {
               out_value[3] = (out_value[3] & 0xFD) | (static_cast<uint8_t> (propValue.value.int32Values[0]) << 1);
            }
            ret = vehicle_vendor_storage_write(VENDOR_VEHICLE_HAL_FEATURE_ID, VEHICLE_HAL_FEATURE_LEN, out_value);
            if(ret != 0) {
                ALOGE("write BRKDISWIP vendor storage fail\n");
            }
            auto updatedPropValue = getValuePool()->obtain(propValue);
            updatedPropValue->timestamp = elapsedRealtimeNano();
            mServerSidePropStore.writeValue(*updatedPropValue, updateStatus);
            onPropertyValueFromCar(*updatedPropValue, updateStatus);
            return StatusCode::OK;
        }break;
        case toInt(VehicleProperty::GECKO_DRIVING_POWER_ST):
        {
            mPropertyManager->updateCPropertyValue("sc_driving_power_set", propValue.value.int32Values[0]);
        }break;
        case toInt(VehicleProperty::GECKO_FIVE_YARDS_ICCID):
        {
            LOG(INFO) << "GECKO_FIVE_YARDS_ICCID:" << propValue.value.stringValue;

            sendMultipleJson("FiveYards","ICCID",propValue.value.stringValue);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));

        } break;
        case toInt(VehicleProperty::GECKO_FIVE_YARDS_IMSI):
        {
            LOG(INFO) << "GECKO_FIVE_YARDS_IMSI:" << propValue.value.stringValue;
            sendMultipleJson("FiveYards","IMSI",propValue.value.stringValue);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));

        } break;
        case toInt(VehicleProperty::GECKO_FIVE_YARDS_MSISDN):
        {
            LOG(INFO) << "GECKO_FIVE_YARDS_MSISDN:" << propValue.value.stringValue;
            sendMultipleJson("FiveYards","MSISDN",propValue.value.stringValue);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));

        } break;
        case toInt(VehicleProperty::GECKO_FIVE_YARDS_IMEI):
        {
            LOG(INFO) << "GECKO_FIVE_YARDS_IMEI:" << propValue.value.stringValue;
            sendMultipleJson("FiveYards","IMEI",propValue.value.stringValue);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));

        } break;
        case toInt(VehicleProperty::GECKO_SOC_FACTORY_RESET):
        {
            LOG(INFO) << "GECKO_SOC_FACTORY_RESET:" << propValue.value.int32Values[0];
            std::string key = "SocFactory";
            std::string value = std::to_string(propValue.value.int32Values[0]);
            mPropertyManager->singleJsonEnQueue(key, value);
            //恢复出厂设置需要下发制动盘擦拭为OFF并将相应数据存放在vendor storage中 begin
            mPropertyManager->updateCPropertyValue("sc_brk_dis_wip_sw", 0);
            std::vector<uint8_t> out_value(7, 0);
            int ret = 0;
            vehicle_vendor_storage_read(VENDOR_VEHICLE_HAL_FEATURE_ID, VEHICLE_HAL_FEATURE_LEN, out_value); //100 -> VENDOR_VEHICLE_HAL_FEATURE_ID; 0x06 -> VEHICLE_HAL_FEATURE_LEN
            if(out_value.size() > 6) {
               out_value[3] = (out_value[3] & 0xFD) | (0);
            }
            ret = vehicle_vendor_storage_write(VENDOR_VEHICLE_HAL_FEATURE_ID, VEHICLE_HAL_FEATURE_LEN, out_value);
            if(ret != 0) {
                ALOGE("write BRKDISWIP vendor storage fail\n");
            }
            //恢复出厂设置需要下发制动盘擦拭为OFF并将相应数据存放在vendor storage中 end
        } break;
        case toInt(VehicleProperty::GECKO_FCM_ACCTAKEOVERREQ):
        {
            LOG(INFO) << "GECKO_FCM_ACCTAKEOVERREQ:" << propValue.value.int32Values[0];
            mPropertyManager->updateCPropertyValue("sc_fcm_acc_take_over_req", propValue.value.int32Values[0]);
        } break;
        case toInt(VehicleProperty::GECKO_FCM_ADASDRVRRREQ):
        {
            LOG(INFO) << "GECKO_FCM_ADASDRVRRREQ:" << propValue.value.int32Values[0];
            mPropertyManager->updateCPropertyValue("sc_fcm_adas_drvr_req", propValue.value.int32Values[0]);
        } break;
        case toInt(VehicleProperty::GECKO_HU_CHARGINGMODE):
        {
            LOG(INFO) << "GECKO_HU_CHARGINGMODE:" << propValue.value.int32Values[0];
            mPropertyManager->updateCPropertyValue("sc_hu_chargingmode", propValue.value.int32Values[0]);
        } break;
        case toInt(VehicleProperty::GECKO_HU_CHARGINGREQUEST):
        {
            LOG(INFO) << "GECKO_HU_CHARGINGREQUEST:" << propValue.value.int32Values[0];
            mPropertyManager->updateCPropertyValue("sc_hu_charging_request", propValue.value.int32Values[0]);
        } break;
        case toInt(VehicleProperty::GECKO_HU_ILLMNDENTRYSTS):
        {
            LOG(INFO) << "GECKO_HU_ILLMNDENTRYSTS:" << propValue.value.int32Values[0];
            mPropertyManager->updateCPropertyValue("sc_hu_illmnd_entry_sts", propValue.value.int32Values[0]);
        } break;
        case toInt(VehicleProperty::GECKO_HU_OVERSPDCNTRLLOCKSET):
        {
            LOG(INFO) << "GECKO_HU_OVERSPDCNTRLLOCKSET:" << propValue.value.int32Values[0];
            mPropertyManager->updateCPropertyValue("sc_hu_overspd_cntrl_lock_set", propValue.value.int32Values[0]);
        } break;
        case toInt(VehicleProperty::GECKO_HU_TRUNKLOCKSWCTRL):
        {
            LOG(INFO) << "GECKO_HU_TRUNKLOCKSWCTRL:" << propValue.value.int32Values[0];
            mPropertyManager->updateCPropertyValue("sc_hu_trunk_lock_sw_ctrl", propValue.value.int32Values[0]);
        } break;
        case toInt(VehicleProperty::GECKO_HU_CHGWIPERMTRMDSWSET):
        {
            LOG(INFO) << "GECKO_HU_CHGWIPERMTRMDSWSET:" << propValue.value.int32Values[0];
            mPropertyManager->updateCPropertyValue("sc_hu_chg_wiper_mt_md_sw_set", propValue.value.int32Values[0]);
        } break;
        case toInt(VehicleProperty::GECKO_HU_TPMSRESETREQ):
        {
            LOG(INFO) << "GECKO_HU_TPMSRESETREQ:" << propValue.value.int32Values[0];
            mPropertyManager->updateCPropertyValue("sc_hu_tpms_reset_req", propValue.value.int32Values[0]);
        } break;
        case toInt(VehicleProperty::GECKO_BCM_MAIDRVARSEATSTS):
        {
            LOG(INFO) << "set MAIDRVARSEATSTS:" << propValue.value.int32Values[0];
            mPropertyManager->updateCPropertyValue("sc_hu_mai_drvr_seat_heat", propValue.value.int32Values[0]);
        } break;
        case toInt(VehicleProperty::GECKO_BCM_STEERWHLHEATSTS):
        {
            LOG(INFO) << "set STEERWHLHEATSTS:" << propValue.value.int32Values[0];
            mPropertyManager->updateCPropertyValue("sc_hu_steer_whl_heat", propValue.value.int32Values[0]);
        } break;
        case toInt(VehicleProperty::GECKO_HU_PWROFFREQ):
        {
            LOG(INFO) << "GECKO_HU_PWROFFREQ:" << propValue.value.int32Values[0];
            mPropertyManager->updateCPropertyValue("sc_hu_pwr_off_req", propValue.value.int32Values[0]);
        } break;
        case toInt(VehicleProperty::GECKO_BCM_BACKLADJVALLFB):
        {
            LOG(INFO) << "GECKO_CDC_BACKLADJVALL:" << propValue.value.int32Values[0];
            mPropertyManager->updateCPropertyValue("sc_cdc_back_ladjval", propValue.value.int32Values[0]);
        } break;
	case toInt(VehicleProperty::GECKO_AC_ACUNLOCKVENTSETFB):
        {
            LOG(INFO) << "GECKO_AC_ACUNLOCKVENTSET:" << propValue.value.int32Values[0];
            mPropertyManager->updateCPropertyValue("sc_hu_ac_unlock_vent_set", propValue.value.int32Values[0]);
        } break;
        case toInt(VehicleProperty::GECKO_HU_SPDDOWNINCORENABLESW):
        {
            LOG(INFO) << "GECKO_HU_SPDDOWNINCORENABLESW:" << propValue.value.int32Values[0];
            mPropertyManager->updateCPropertyValue("sc_hu_spd_downin_cor_enable_sw", propValue.value.int32Values[0]);
        } break;
        case toInt(VehicleProperty::GECKO_VCU_ACCHRGMAXCURR):
        {
            LOG(INFO) << "GECKO_VCU_ACCHRGMAXCURR:" << propValue.value.int32Values[0];
            mPropertyManager->updateCPropertyValue("sc_hu_veh_max_current", propValue.value.int32Values[0]);
        } break;
        // case toInt(VehicleProperty::GECKO_CCM_CHRAGING_TARGET_SOC):
        // {
        //     LOG(INFO) << "GECKO_CCM_CHRAGING_TARGET_SOC:" << propValue.value.int32Values[0];
        //     mPropertyManager->updateCPropertyValue("chraging_target_soc", propValue.value.int32Values[0]);
        // } break;
        case toInt(VehicleProperty::GECKO_FCM_LKASETFB):
        {
            LOG(INFO) << "GECKO_FCM_LKASET:" << propValue.value.int32Values[0];
            mPropertyManager->updateCPropertyValue("sc_hu_lka_enable_sw", propValue.value.int32Values[0]);
        } break;
        /*
        case toInt(VehicleProperty::GECKO_FCM_LDWSETFB):
        {
            LOG(INFO) << "GECKO_FCM_LDWSET:" << propValue.value.int32Values[0];
            mPropertyManager->updateCPropertyValue("sc_hu_ldw_enable_sw", propValue.value.int32Values[0]);
        } break;
        */
        case toInt(VehicleProperty::GECKO_FCM_ELKSETFB):
        {
            LOG(INFO) << "GECKO_FCM_ELKSET:" << propValue.value.int32Values[0];
            mPropertyManager->updateCPropertyValue("sc_hu_elk_enb_sw", propValue.value.int32Values[0]);
        } break;
        case toInt(VehicleProperty::GECKO_CRRR_BSD_SETFB):
        {
            LOG(INFO) << "GECKO_CRRR_BSD_SET:" << propValue.value.int32Values[0];
            mPropertyManager->updateCPropertyValue("sc_hu_bsd_enable_sw", propValue.value.int32Values[0]);
        } break;
        case toInt(VehicleProperty::GECKO_CRRR_DOWSETFB):
        {
            LOG(INFO) << "GECKO_CRRR_DOWSET:" << propValue.value.int32Values[0];
            mPropertyManager->updateCPropertyValue("sc_hu_dow_ena_sw", propValue.value.int32Values[0]);
        } break;
        /*
        case toInt(VehicleProperty::GECKO_FCM_AEBSETFB):
        {
            LOG(INFO) << "GECKO_FCM_AEBSET:" << propValue.value.int32Values[0];
            mPropertyManager->updateCPropertyValue("sc_hu_aeb_enable_sw", propValue.value.int32Values[0]);
            std::string aeb = std::to_string(propValue.value.int32Values[0]);
            android::base::SetProperty("persist.aeb.enable.sw", aeb);
        } break;
        */
        case toInt(VehicleProperty::GECKO_CRRR_RCW_SETFB):
        {
            LOG(INFO) << "GECKO_CRRR_RCW_SET:" << propValue.value.int32Values[0];
            mPropertyManager->updateCPropertyValue("sc_hu_rcw_enable_sw", propValue.value.int32Values[0]);
        } break;
        case toInt(VehicleProperty::GECKO_CRRR_RCTA_SETFB):
        {
            LOG(INFO) << "GECKO_CRRR_RCTA_SET:" << propValue.value.int32Values[0];
            mPropertyManager->updateCPropertyValue("sc_hu_rcta_enable_sw", propValue.value.int32Values[0]);
        } break;
        case toInt(VehicleProperty::GECKO_HU_TPMSLEARNINGREQ):
        {
            LOG(INFO) << "GECKO_HU_TPMSLEARNINGREQ:" << propValue.value.int32Values[0];
            mPropertyManager->updateCPropertyValue("sc_hu_tpms_learning_req", propValue.value.int32Values[0]);
        } break;
        case toInt(VehicleProperty::GECKO_BCM_OTA_MODE_SET):
        {
            LOG(INFO) << "GECKO_BCM_OTA_MODE_SET:" << propValue.value.int32Values[0];
            mPropertyManager->updateCPropertyValue("hu_bcmota_mode_set", propValue.value.int32Values[0]);
        } break;
        case toInt(VehicleProperty::GECKO_OTA_RTC):
        {
            LOG(INFO) << "GECKO_OTA_RTC:" << propValue.value.int32Values[0];
            std::string key = "OTARTC";
            std::string value = std::to_string(propValue.value.int32Values[0]);
            mPropertyManager->singleJsonEnQueue(key, value);
        } break;
        case toInt(VehicleProperty::GECKO_FCM_HMASETFB):
        {
            LOG(INFO) << "GECKO_FCM_HMASETFB:" << propValue.value.int32Values[0];
            mPropertyManager->updateCPropertyValue("sc_hu_hma_enable_sw", propValue.value.int32Values[0]);
        } break;
        case toInt(VehicleProperty::GECKO_FCM_ICASETFB):
        {
            LOG(INFO) << "GECKO_FCM_ICASETFB:" << propValue.value.int32Values[0];
            mPropertyManager->updateCPropertyValue("sc_hu_ica_ena_swt", propValue.value.int32Values[0]);
        } break;
        case toInt(VehicleProperty::GECKO_FCM_ICATRUCKSETFB):
        {
            LOG(INFO) << "GECKO_FCM_ICATRUCKSETFB:" << propValue.value.int32Values[0];
            mPropertyManager->updateCPropertyValue("sc_hu_ica_avd_truck_set", propValue.value.int32Values[0]);
        } break;
        case toInt(VehicleProperty::GECKO_FCM_LDW_AND_ALERT_SET):
        {
            LOG(INFO) << "GECKO_FCM_LDW_AND_ALERT_SET:value[0]:" << propValue.value.int32Values[0] << ";value[1]:" << propValue.value.int32Values[1];
            mPropertyManager->updateCPropertyValue("sc_hu_ldw_enable_sw", propValue.value.int32Values[0]);
            mPropertyManager->updateCPropertyValue("sc_hu_ldw_alert_method_sw", propValue.value.int32Values[1]);
        } break;
        case toInt(VehicleProperty::GECKO_FCM_AEBFCW_SETFB):
        {
            LOG(INFO) << "GECKO_FCM_AEBFCW_SETFB:value[0]:" << propValue.value.int32Values[0] << ";value[1]:" << propValue.value.int32Values[1];
            mPropertyManager->updateCPropertyValue("sc_hu_aeb_enable_sw", propValue.value.int32Values[0]);
            mPropertyManager->updateCPropertyValue("sc_hu_fcw_enable_sw", propValue.value.int32Values[1]);
            std::string code0 = std::to_string(propValue.value.int32Values[0]);
            std::string code1 = std::to_string(propValue.value.int32Values[1]);
            android::base::SetProperty("persist.aeb.enable.sw", code0);
            android::base::SetProperty("persist.fcw.enable.sw", code1);
        } break;
        case toInt(VehicleProperty::GECKO_FCM_SLIFSETFB):
        {
            LOG(INFO) << "GECKO_FCM_SLIFSETFB:" << propValue.value.int32Values[0];
            mPropertyManager->updateCPropertyValue("sc_hu_slif_enable_sw", propValue.value.int32Values[0]);
        } break;
        case toInt(VehicleProperty::GECKO_FCM_SLWFSETFB):
        {
            LOG(INFO) << "GECKO_FCM_SLWFSETFB:" << propValue.value.int32Values[0];
            mPropertyManager->updateCPropertyValue("sc_hu_slwf_enable_sw", propValue.value.int32Values[0]);
        } break;
        case toInt(VehicleProperty::GECKO_CCU_REMTSTEERWHLHEATFB):
        {
            LOG(INFO) << "GECKO_CCU_REMTSTEERWHLHEAT:" << propValue.value.int32Values[0];
            mPropertyManager->updateCPropertyValue("sc_tbox_remt_steer_whl_heat_req", propValue.value.int32Values[0]);
        } break;
        case toInt(VehicleProperty::GECKO_CCU_REMTPOWERCTRLFB):
        {
            LOG(INFO) << "GECKO_CCU_REMTPOWERCTRL:" << propValue.value.int32Values[0];
            mPropertyManager->updateCPropertyValue("sc_tbox_remt_power_ctrl_req", propValue.value.int32Values[0]);
        } break;
        case toInt(VehicleProperty::GECKO_CCU_REMOTEPOWERLOCKFB):
        {
            LOG(INFO) << "GECKO_CCU_REMOTEPOWERLOCK:" << propValue.value.int32Values[0];
            mPropertyManager->updateCPropertyValue("sc_tbox_remote_power_lock", propValue.value.int32Values[0]);
            mPropertyManager->updateCPropertyValue("sc_tbox_remote_power_lock_v", 0x1);
        } break;
        case toInt(VehicleProperty::GECKO_AC_REMOTEDFROSTCTLFB):
        {
            LOG(INFO) << "GECKO_AC_REMOTEDFROSTCTL:" << propValue.value.int32Values[0];
            mPropertyManager->updateCPropertyValue("sc_tbox_remote_defrost_ctl", propValue.value.int32Values[0]);
        } break;
        case toInt(VehicleProperty::GECKO_CCU_REMTMAIDRSEATHEATFB):
        {
            LOG(INFO) << "GECKO_CCU_REMTMAIDRSEATHEAT:" << propValue.value.int32Values[0];
            mPropertyManager->updateCPropertyValue("sc_tbox_remt_mai_dr_seat_heat_req", propValue.value.int32Values[0]);
        } break;
        case toInt(VehicleProperty::GECKO_HU_NAV_COUNTRYID):
        {
            LOG(INFO) << "GECKO_HU_NAV_COUNTRYID:" << propValue.value.int32Values[0];
            mPropertyManager->updateCPropertyValue("hu_nav_country_id", propValue.value.int32Values[0]);
            std::string code = std::to_string(propValue.value.int32Values[0]);
            android::base::SetProperty("persist.navi.country.id", code);
            auto updatedPropValue = getValuePool()->obtain(propValue);
            updatedPropValue->timestamp = elapsedRealtimeNano();
            mServerSidePropStore.writeValue(*updatedPropValue, updateStatus);
            onPropertyValueFromCar(*updatedPropValue, updateStatus);
            std::vector<uint8_t> out_value(7, 0);
            int ret = 0;
            vehicle_vendor_storage_read(VENDOR_VEHICLE_HAL_FEATURE_ID, VEHICLE_HAL_FEATURE_LEN, out_value);
            if(out_value.size() > 6) {
               out_value[5] = static_cast<uint8_t> (propValue.value.int32Values[0]);
            }
            ret = vehicle_vendor_storage_write(VENDOR_VEHICLE_HAL_FEATURE_ID, VEHICLE_HAL_FEATURE_LEN, out_value);
            if(ret != 0) {
                ALOGE("write COUNTRYID vendor storage fail\n");
            }
            return StatusCode::OK;
        } break;
        case toInt(VehicleProperty::GECKO_HU_NAVICOUNTRYCODE):
        {
            LOG(INFO) << "GECKO_HU_NAVICOUNTRYCODE:" << propValue.value.int32Values[0];
            mPropertyManager->updateCPropertyValue("hu_navi_country_code", propValue.value.int32Values[0]);
            std::string code = std::to_string(propValue.value.int32Values[0]);
            android::base::SetProperty("persist.navi.country.code", code);
        } break;
        case toInt(VehicleProperty::GECKO_CDC_ACTIVEWAKEUP):
        {
            LOG(INFO) << "GECKO_CDC_ACTIVEWAKEUP:" << propValue.value.int32Values[0];
            mPropertyManager->updateCPropertyValue("sc_cdc_active_wakeup", propValue.value.int32Values[0]);
            if(propValue.value.int32Values.size() > 1) {
                mPropertyManager->updateCPropertyValue("sc_cdc_active_wakeup_t", propValue.value.int32Values[1]);
            }
        } break;
        case toInt(VehicleProperty::GECKO_CDC_WAKEUPREAS):
        {
            LOG(INFO) << "GECKO_CDC_WAKEUPREAS:" << propValue.value.int32Values[0];
            mPropertyManager->updateCPropertyValue("sc_cdc_wakeup_reas", propValue.value.int32Values[0]);
            if(propValue.value.int32Values.size() > 1) {
                mPropertyManager->updateCPropertyValue("sc_cdc_wakeup_reas_t", propValue.value.int32Values[1]);
            }
        } break;
        case toInt(VehicleProperty::GECKO_CDC_KEEPAWAKEREASREMREQ):
        {
            LOG(INFO) << "GECKO_CDC_KEEPAWAKEREASREMREQ:" << propValue.value.int32Values[0];
            mPropertyManager->updateCPropertyValue("sc_cdc_keep_awake_reas_rem_req", propValue.value.int32Values[0]);
            if(propValue.value.int32Values.size() > 1) {
                mPropertyManager->updateCPropertyValue("sc_cdc_keep_awake_reas_rem_req_t", propValue.value.int32Values[1]);
            }
        } break;
        case toInt(VehicleProperty::GECKO_CDC_KEEPAWAKEREASFACRESET):
        {
            LOG(INFO) << "GECKO_CDC_KEEPAWAKEREASFACRESET:" << propValue.value.int32Values[0];
            mPropertyManager->updateCPropertyValue("sc_cdc_keep_awake_reas_fac_reset", propValue.value.int32Values[0]);
            if(propValue.value.int32Values.size() > 1) {
                mPropertyManager->updateCPropertyValue("sc_cdc_keep_awake_reas_fac_reset_t", propValue.value.int32Values[1]);
            }
        } break;
        case toInt(VehicleProperty::GECKO_CDC_KEEPAWAKEREASRESTART):
        {
            LOG(INFO) << "GECKO_CDC_KEEPAWAKEREASRESTART:" << propValue.value.int32Values[0];
            mPropertyManager->updateCPropertyValue("sc_cdc_keep_awake_reas_restart", propValue.value.int32Values[0]);
            if(propValue.value.int32Values.size() > 1) {
                mPropertyManager->updateCPropertyValue("sc_cdc_keep_awake_reas_restart_t", propValue.value.int32Values[1]);
            }
        } break;
        case toInt(VehicleProperty::GECKO_CDC_KEEPAWAKEREASDO):
        {
            LOG(INFO) << "GECKO_CDC_KEEPAWAKEREASDO:" << propValue.value.int32Values[0];
            mPropertyManager->updateCPropertyValue("sc_cdc_keep_awake_reas_do", propValue.value.int32Values[0]);
            if(propValue.value.int32Values.size() > 1) {
                mPropertyManager->updateCPropertyValue("sc_cdc_keep_awake_reas_do_t", propValue.value.int32Values[1]);
            }
        } break;
        case toInt(VehicleProperty::GECKO_CDC_KEEPAWAKEREASLOCUP):
        {
            LOG(INFO) << "GECKO_CDC_KEEPAWAKEREASLOCUP:" << propValue.value.int32Values[0];
            mPropertyManager->updateCPropertyValue("sc_cdc_keep_awake_reas_loc_up", propValue.value.int32Values[0]);
            if(propValue.value.int32Values.size() > 1) {
                mPropertyManager->updateCPropertyValue("sc_cdc_keep_awake_reas_loc_up_t", propValue.value.int32Values[1]);
            }
        } break;
        case toInt(VehicleProperty::GECKO_CDC_KEEPAWAKESTA):
        {
            LOG(INFO) << "GECKO_CDC_KEEPAWAKESTA:" << propValue.value.int32Values[0];
            mPropertyManager->updateCPropertyValue("sc_cdc_keep_awake_sta", propValue.value.int32Values[0]);
            if(propValue.value.int32Values.size() > 1) {
                mPropertyManager->updateCPropertyValue("sc_cdc_keep_awake_sta_t", propValue.value.int32Values[1]);
            }
        } break;
        case toInt(VehicleProperty::GECKO_BCM_CARRIAGELIGHTSTA):
        {
            LOG(INFO) << "GECKO_BCM_CARRIAGELIGHTSTA:" << propValue.value.int32Values[0];
            mPropertyManager->updateCPropertyValue("sc_hu_bcm_cargo_light_set", propValue.value.int32Values[0]);
        } break;
        case toInt(VehicleProperty::GECKO_SOC_SYNC_TIME_MCU): {      //soc 同步时间到MCU
            std::string key = "NetworkTime";
            std::string value = std::to_string(propValue.value.int32Values[0]);
            mPropertyManager->singleJsonEnQueue(key, value);
        } break;
        case toInt(VehicleProperty::GECKO_FM_PLAY_STATUS): {
            LOG(INFO) << "set GECKO_FM_PLAY_STATUS to:" << propValue.value.int32Values[0];
            auto updatedPropValue = getValuePool()->obtain(propValue);
            updatedPropValue->timestamp = elapsedRealtimeNano();
            mServerSidePropStore.writeValue(*updatedPropValue, updateStatus);
            onPropertyValueFromCar(*updatedPropValue, updateStatus);
            return StatusCode::OK;
        } break;
        case toInt(VehicleProperty::GECKO_SIM_CARD_STS):
        {
            LOG(INFO) << "GECKO_SIM_CARD_STS:" << propValue.value.int32Values[0];
            std::string key = "SIMCardSts";
            std::string value = std::to_string(propValue.value.int32Values[0]);
            if(propValue.value.int32Values[0] != oldSimCardSts ) {
                oldSimCardSts = propValue.value.int32Values[0];
                mPropertyManager->singleJsonEnQueue(key, value);
                mPropertyManager->updateCPropertyValue("gecko_sim_card_sts", propValue.value.int32Values[0]);
            }
        } break;
        case toInt(VehicleProperty::GECKO_TBOX_LINK_TSP):
        {
            LOG(INFO) << "GECKO_TBOX_LINK_TSP:" << propValue.value.int32Values[0];
            std::string key = "TBOXLINKTSP";
            std::string value = std::to_string(propValue.value.int32Values[0]);
            mPropertyManager->singleJsonEnQueue(key, value);
            mPropertyManager->updateCPropertyValue("gecko_tbox_link_tsp", propValue.value.int32Values[0]);

        } break;
        case toInt(VehicleProperty::GECKO_4G_STRENGTH):
        {
            LOG(INFO) << "GECKO_4G_STRENGTH:" << propValue.value.int32Values[0];
            std::string key = "4GStrength";
            std::string value = std::to_string(propValue.value.int32Values[0]);
            mPropertyManager->singleJsonEnQueue(key, value);
            mPropertyManager->updateCPropertyValue("gecko_4g_strength", propValue.value.int32Values[0]);

        } break;
        case toInt(VehicleProperty::GECKO_WIFI_STRENGTH):
        case toInt(VehicleProperty::GECKO_WIFI_STATUS):
        {
            if(propId == toInt(VehicleProperty::GECKO_WIFI_STRENGTH)) {
                if(oldWifiStrength != propValue.value.int32Values[0]) {
                    oldWifiStrength = propValue.value.int32Values[0];
                    wifiStrengthChanged = true;
                }
                mSecond["Strength"] = std::to_string(propValue.value.int32Values[0]);
                mPropertyManager->updateCPropertyValue("gecko_wifi_strength", propValue.value.int32Values[0]);
                filledStrength = true;
            } else {
                if(filledStrength && (wifiStrengthChanged || (oldWifiStatus != propValue.value.int32Values[0]))) {
                    LOG(INFO) << "GECKO_WIFI_STATUS newStatus:" << propValue.value.int32Values[0] << "oldStatus:" << oldWifiStatus;
                    oldWifiStatus = propValue.value.int32Values[0];
                    mSecond["Status"] = std::to_string(propValue.value.int32Values[0]);
                    Json::StreamWriterBuilder builder;
                    builder["indentation"] = ""; // 设置缩进为空字符串，即不缩进
                    Json::Value final_root;
                    final_root["WIFI"] = mSecond;
                    std::string final_json_str = Json::writeString(builder, final_root);
                    LOG(INFO) <<"send GECKO_WIFI INFO to mcu -- str:"<<final_json_str;
                    std::vector<uint8_t> vec(final_json_str.begin(), final_json_str.end());
                    mPropertyManager->jsonQueueEnqueue(vec);
                    mPropertyManager->updateCPropertyValue("gecko_wifi_status", propValue.value.int32Values[0]);
                    filledStrength = false;
                    wifiStrengthChanged = false;
                    mSecond.clear();
                }
            }
        } break;
        case toInt(VehicleProperty::GECKO_GPS_LON):
        {
            LOG(INFO) << "GECKO_GPS_LON:" << propValue.value.stringValue;
            sendMultipleJson("GPS","Lon",propValue.value.stringValue);
            std::string Value = propValue.value.stringValue;
            mPropertyManager->updateCPropertyValue("gecko_gps_lon", Value);

        } break;
        case toInt(VehicleProperty::GECKO_GPS_LAT):
        {
            LOG(INFO) << "GECKO_GPS_LAT:" << propValue.value.stringValue;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            sendMultipleJson("GPS","lat",propValue.value.stringValue);
            std::string Value = propValue.value.stringValue;
            mPropertyManager->updateCPropertyValue("gecko_gps_lat", Value);

        } break;
        case toInt(VehicleProperty::GECKO_ENGINEER_MODE_TEST):
        {
            std::string value;
            int32_t testItem = propValue.value.int32Values[0];
            LOG(INFO) << "GECKO_ENGINEER_MODE_TEST:" << testItem;
            if((testItem==GECKO_ENGINEER_MODE_ENTER) || (testItem==GECKO_ENGINEER_MODE_EXIT)) {
                std::string key = "INEng";
                value = std::to_string(testItem + 1);
                mPropertyManager->singleJsonEnQueue(key, value);
            } else if(testItem == GECKO_ENGINEER_MODE_E2PROM_TEST) {
                std::string key = "E2PRom";
                value = "?";
                mPropertyManager->singleJsonEnQueue(key, value);
            } else if(testItem == GECKO_ENGINEER_MODE_KEYCODE_TEST) {
                std::string key = "KEYCode";
                value = "?";
                mPropertyManager->singleJsonEnQueue(key, value);
            } else if(testItem == GECKO_ENGINEER_MODE_QUERY_LOWBAT_VOLT) {
                std::string key = "LBATVol";
                value = "?";
                mPropertyManager->singleJsonEnQueue(key, value);
            } else if(testItem == GECKO_ENGINEER_MODE_CURRENT_TEST) {
                std::string key = "CURTest";
                value = "1";
                mPropertyManager->singleJsonEnQueue(key, value);
            }
        } break;
        default:
            break;
    }

    std::string buildType = android::base::GetProperty("GECKO.car.application.debug", "0"); 
    int32_t appDebugMode = 0;
    appDebugMode = static_cast<int32_t> (std::stoi(buildType)); // 尝试将字符串转换为整数
    LOG(INFO) << "dxy--- app debug mode = " << appDebugMode;
    if(1 == appDebugMode){
        LOG(INFO) << "dxy--- 1 == appDebugMode running to onSetProperty = ";
        auto updatedPropValue = getValuePool()->obtain(propValue);
        updatedPropValue->timestamp = elapsedRealtimeNano();
        mServerSidePropStore.writeValue(*updatedPropValue, updateStatus);
        onPropertyValueFromCar(*updatedPropValue, updateStatus);
    }

    return StatusCode::OK;

    // Some properties need to be treated non-trivially
    //    switch (value.prop) {
    //        case kGenerateFakeDataControllingProperty:
    //            return handleGenerateFakeDataRequest(value);

    // set the value from vehicle side, used in end to end test.
    //        case kSetIntPropertyFromVehicleForTest: {
    //            auto updatedPropValue = createVehiclePropValue(VehiclePropertyType::INT32, 1);
    //            updatedPropValue->prop = value.value.int32Values[0];
    //            updatedPropValue->value.int32Values[0] = value.value.int32Values[1];
    //            updatedPropValue->timestamp = value.value.int64Values[0];
    //            updatedPropValue->areaId = value.areaId;
    //            onPropertyValueFromCar(*updatedPropValue, updateStatus);
    //            return StatusCode::OK;
    //        }
    //        case kSetFloatPropertyFromVehicleForTest: {
    //            auto updatedPropValue = createVehiclePropValue(VehiclePropertyType::FLOAT, 1);
    //            updatedPropValue->prop = value.value.int32Values[0];
    //            updatedPropValue->value.floatValues[0] = value.value.floatValues[0];
    //            updatedPropValue->timestamp = value.value.int64Values[0];
    //            updatedPropValue->areaId = value.areaId;
    //            onPropertyValueFromCar(*updatedPropValue, updateStatus);
    //            return StatusCode::OK;
    //        }
    //        case kSetBooleanPropertyFromVehicleForTest: {
    //            auto updatedPropValue = createVehiclePropValue(VehiclePropertyType::BOOLEAN, 1);
    //            updatedPropValue->prop = value.value.int32Values[1];
    //            updatedPropValue->value.int32Values[0] = value.value.int32Values[0];
    //            updatedPropValue->timestamp = value.value.int64Values[0];
    //            updatedPropValue->areaId = value.areaId;
    //            onPropertyValueFromCar(*updatedPropValue, updateStatus);
    //            return StatusCode::OK;
    //        }

    //        case AP_POWER_STATE_REPORT:
    //            switch (value.value.int32Values[0]) {
    //                case toInt(VehicleApPowerStateReport::DEEP_SLEEP_EXIT):
    //                case toInt(VehicleApPowerStateReport::SHUTDOWN_CANCELLED):
    //                case toInt(VehicleApPowerStateReport::WAIT_FOR_VHAL):
    // CPMS is in WAIT_FOR_VHAL state, simply move to ON
    // Send back to HAL
    // ALWAYS update status for generated property value
    //                    onPropertyValueFromCar(*createApPowerStateReq(VehicleApPowerStateReq::ON,
    //                    0),
    //                                           true /* updateStatus */);
    //                    break;
    //                case toInt(VehicleApPowerStateReport::DEEP_SLEEP_ENTRY):
    //                case toInt(VehicleApPowerStateReport::SHUTDOWN_START):
    // CPMS is in WAIT_FOR_FINISH state, send the FINISHED command
    // Send back to HAL
    // ALWAYS update status for generated property value
    //                    onPropertyValueFromCar(
    //                            *createApPowerStateReq(VehicleApPowerStateReq::FINISHED, 0),
    //                            true /* updateStatus */);
    //                    break;
    //                case toInt(VehicleApPowerStateReport::ON):
    //                case toInt(VehicleApPowerStateReport::SHUTDOWN_POSTPONE):
    //                case toInt(VehicleApPowerStateReport::SHUTDOWN_PREPARE):
    // Do nothing
    //                    break;
    //                default:
    // Unknown state
    //                    break;
    //            }
    //            break;

    /*
    #ifdef ENABLE_VENDOR_CLUSTER_PROPERTY_FOR_TESTING
            case toInt(VehicleProperty::CLUSTER_REPORT_STATE):
            case toInt(VehicleProperty::CLUSTER_REQUEST_DISPLAY):
            case toInt(VehicleProperty::CLUSTER_NAVIGATION_STATE):
            case VENDOR_CLUSTER_SWITCH_UI:
            case VENDOR_CLUSTER_DISPLAY_STATE: {
                auto updatedPropValue = createVehiclePropValue(getPropType(value.prop), 0);
                updatedPropValue->prop = value.prop & ~toInt(VehiclePropertyGroup::MASK);
                if (isSystemProperty(value.prop)) {
                    updatedPropValue->prop |= toInt(VehiclePropertyGroup::VENDOR);
                } else {
                    updatedPropValue->prop |= toInt(VehiclePropertyGroup::SYSTEM);
                }
                updatedPropValue->value = value.value;
                updatedPropValue->timestamp = elapsedRealtimeNano();
                updatedPropValue->areaId = value.areaId;
                onPropertyValueFromCar(*updatedPropValue, updateStatus);
                return StatusCode::OK;
            }
    #endif  // ENABLE_VENDOR_CLUSTER_PROPERTY_FOR_TESTING

            default:
                break;
        }
    */

    // In the real vhal, the value will be sent to Car ECU.
    // We just pretend it is done here and send back to HAL
    //    auto updatedPropValue = getValuePool()->obtain(value);
    //    updatedPropValue->timestamp = elapsedRealtimeNano();

    //    mServerSidePropStore.writeValue(*updatedPropValue, updateStatus);
    //    onPropertyValueFromCar(*updatedPropValue, updateStatus);
    //    return StatusCode::INVALID_ARG;
}
}  // namespace android::hardware::automotive::vehicle::V2_0::impl
