#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <json/json.h>
#include <log/log.h>
#include <android-base/logging.h>
#include <android-base/properties.h>
#include "ConfigurationWordParse.h"

namespace android {
namespace hardware {
namespace automotive {
namespace vehicle {
namespace V2_0 {

namespace impl {
    ConfigurationWordParse::ConfigurationWordParse() {}
    ConfigurationWordParse::~ConfigurationWordParse() {}
    // Function to extract specific bit range from a byte
    uint8_t ConfigurationWordParse::extract_bit_range(uint8_t byte, int start_bit, int end_bit) {
        // Create a mask for the specified bit range
        uint8_t mask = ((1 << (end_bit - start_bit + 1)) - 1) << (start_bit);
        
        // Shift and mask to get the value
        return (byte & mask) >> (start_bit);
    }

    // Function to find description for a specific value
    std::string ConfigurationWordParse::find_description(const Json::Value& descriptions, uint8_t value) {
        // Convert value to hex string
        char hex_value[10];
        snprintf(hex_value, sizeof(hex_value), "0x%X", value);

        // Iterate through description keys
        for (const auto& key : descriptions.getMemberNames()) {
            // Check exact match
            if (key == hex_value) {
                return descriptions[key].asString();
            }

            // Check range match
            if (key.find('~') != std::string::npos && key.substr(0, 2) == "0x") {
                size_t pos = key.find('~');
                std::string range_start = key.substr(2, pos - 2);
                std::string range_end = key.substr(pos + 1);

                // Convert hex strings to integers
                uint8_t start = std::stoi(range_start, nullptr, 16);
                uint8_t end = std::stoi(range_end, nullptr, 16);

                // Check if value is in range
                if (value >= start && value <= end) {
                    return descriptions[key].asString();
                }
            }
        }
        
        return "Unknown";
    }

    std::string ConfigurationWordParse::decToHex(int num) {
        std::ostringstream ss;
        ss << "0x" << std::hex << num; // 设置输出格式为十六进制
        return ss.str();
    }

    void ConfigurationWordParse::parseFieldFromBytes(const Json::Value& fields, const std::vector<uint8_t>& data, const std::string& fieldName) {
        std::ostringstream result;
        // Iterate through fields
        for (const auto& field : fields) {
            // Check if field name matches
            if (field["Name"].asString() != fieldName) {
                continue;
            }
            // Extract byte and bit range
            int byteIndex = field["Byte"].asInt();
            std::string bitRange = field["BitRange"].asString();
            // Parse bit range
            int startBit, endBit;
            sscanf(bitRange.c_str(), "%d~%d", &startBit, &endBit);
            // Extract value from the specified byte
            uint8_t value = extract_bit_range(data[byteIndex], startBit, endBit);
            // Find description
            std::string desc = find_description(field["Description"], value);
            // Build result string
            LOG(INFO) << "Field:" << fieldName
               << ";Byte:" << byteIndex
               << ";Bit Range:" << bitRange
               << ";Raw Value:0x" << std::hex << static_cast<int>(value)
               << ";Description:" << desc;
            if(fieldName.compare("VehCfg1")==0) {
                ALOGI("Field:%s value:%c", fieldName.c_str(), value);
                return;
            } else if(fieldName.compare("VehCfg0")==0) {
                ALOGI("Field:%s value:%c", fieldName.c_str(), value);
                return;
            } else if (fieldName.compare("SLLevel")==0) {
                android::base::SetProperty("persist.tbox.speedlimitlevel", std::to_string(value));
                mPropertyManager->updateCPropertyValueNoCheck("sc_ec_ccm_speed_limit_level", static_cast<uint32_t>(value));
                ALOGI("Field:%s value:%d", fieldName.c_str(), value);
                break;
            } else if(fieldName.compare("SLVersion")==0) {
                android::base::SetProperty("persist.tbox.versionreq", std::to_string(value));
                mPropertyManager->updateCPropertyValueNoCheck("sc_ccm_version", static_cast<uint32_t>(value));
                ALOGI("Field:%s value:%d", fieldName.c_str(), value);
                break;
            } else if(fieldName.compare("SLOL")==0) {
                android::base::SetProperty("persist.tbox.operatlicense", std::to_string(value));
                mPropertyManager->updateCPropertyValueNoCheck("sc_ccm_operat_licence", static_cast<uint32_t>(value));
                ALOGI("Field:%s value:%d", fieldName.c_str(), value);
                break;
            } else if(fieldName.compare("CHGCOND")==0) {
                mPropertyManager->updateCPropertyValueNoCheck("sc_ccm_charging_conditions", static_cast<uint32_t>(value));
                android::base::SetProperty("persist.tbox.chargingcondition", std::to_string(value));
                ALOGI("Field:%s value:%d", fieldName.c_str(), value);
                break;
            } else if(fieldName == "CountryId") {
                android::base::SetProperty("persist.navi.country.id", std::to_string(value));
                mPropertyManager->updateCPropertyValueNoCheck("hu_nav_country_id", static_cast<uint32_t>(value));
                ALOGI("Field:%s value:%d", fieldName.c_str(), value);
            }/** else if(fieldName == "ChrgTarSoc") {
                if(value <= 100) {
                    mPropertyManager->updateCPropertyValueNoCheck("chraging_target_soc", static_cast<uint32_t>(value));
                }
                ALOGI("Field:%s value:%d", fieldName.c_str(), value);
            }**/ else if(fieldName.compare("BRKDISWIP")==0) {
                mPropertyManager->updateCPropertyValueNoCheck("ec_ehb_brk_dis_wip_ac", static_cast<uint32_t>(value));
                mPropertyManager->updateCPropertyValueNoCheck("sc_brk_dis_wip_sw", static_cast<uint32_t>(value));
                ALOGI("Field:%s value:%d", fieldName.c_str(), value);
                return;
            } else if(fieldName.compare("Platform")==0) {
                android::base::SetProperty("persist.gecko.variant.platform", decToHex(value));
                ALOGI("Field:%s value:%d", fieldName.c_str(), value);
                return;
            } else if(fieldName.compare("Model Series")==0) {
                android::base::SetProperty("persist.gecko.variant.model_series", decToHex(value));
                ALOGI("Field:%s value:%d", fieldName.c_str(), value);
                return;
            } else if(fieldName.compare("Model Year")==0) {
                android::base::SetProperty("persist.gecko.variant.model_year", decToHex(value));
                ALOGI("Field:%s value:%d", fieldName.c_str(), value);
                return;
            } else if(fieldName.compare("Vehicle Usage")==0) {
                android::base::SetProperty("persist.gecko.variant.vehicle_use", decToHex(value));
                ALOGI("Field:%s value:%d", fieldName.c_str(), value);
                return;
            } else if(fieldName.compare("Vehicle Series Subdivision")==0) {
                android::base::SetProperty("persist.gecko.variant.series_subdiv", decToHex(value));
                ALOGI("Field:%s value:%d", fieldName.c_str(), value);
                return;
            } else if(fieldName.compare("Driving Type")==0) {
                android::base::SetProperty("persist.gecko.variant.driving_type", decToHex(value));
                ALOGI("Field:%s value:%d", fieldName.c_str(), value);
                return;
            } else if(fieldName.compare("LH/RH Drive+Sliding Door Type")==0) {
                android::base::SetProperty("persist.gecko.variant.drive_dir_cfg", decToHex(value));
                ALOGI("Field:%s value:%d", fieldName.c_str(), value);
                return;
            } else if(fieldName.compare("Brand")==0) {
                android::base::SetProperty("persist.gecko.brand", decToHex(value));
                ALOGI("Field:%s value:%d", fieldName.c_str(), value);
                return;
            } else if(fieldName.compare("Country/Region")==0) {
                android::base::SetProperty("persist.gecko.variant.country", decToHex(value));
                ALOGI("Field:%s value:%d", fieldName.c_str(), value);
                return;
            } else if(fieldName.compare("Language")==0) {
                android::base::SetProperty("persist.gecko.variant.language", decToHex(value));
                ALOGI("Field:%s value:%d", fieldName.c_str(), value);
                return;
            } else if(fieldName.compare("Ext Color")==0) {
                android::base::SetProperty("persist.gecko.variant.ext_color", decToHex(value));
                ALOGI("Field:%s value:%d", fieldName.c_str(), value);
                return;
            } else if(fieldName.compare("Driver's Seat Heating&Ventilate")==0) {
                android::base::SetProperty("persist.gecko.variant.seat_climate", decToHex(value));
                ALOGI("Field:%s value:%d", fieldName.c_str(), value);
                return;
            } else if(fieldName.compare("Longitudinal Active Safety")==0) {
                android::base::SetProperty("persist.gecko.variant.longi_safety", decToHex(value));
                ALOGI("Field:%s value:%d", fieldName.c_str(), value);
                return;
            } else if(fieldName.compare("Lateral Active Safety")==0) {
                android::base::SetProperty("persist.gecko.variant.lat_safety", decToHex(value));
                ALOGI("Field:%s value:%d", fieldName.c_str(), value);
                return;
            } else if(fieldName.compare("Adaptive Cruise Control")==0) {
                android::base::SetProperty("persist.gecko.variant.adaptive_cruise", decToHex(value));
                ALOGI("Field:%s value:%d", fieldName.c_str(), value);
                return;
            } else if(fieldName.compare("Intelligent Speed Assistance")==0) {
                android::base::SetProperty("persist.gecko.variant.speed_assist", decToHex(value));
                ALOGI("Field:%s value:%d", fieldName.c_str(), value);
                return;
            } else if(fieldName.compare("Intelligent High Beam Control")==0) {
                android::base::SetProperty("persist.gecko.variant.auto_high_beam", decToHex(value));
                ALOGI("Field:%s value:%d", fieldName.c_str(), value);
                return;
            } else if(fieldName.compare("Lane Centering Control")==0) {
                android::base::SetProperty("persist.gecko.variant.lane_assist", decToHex(value));
                ALOGI("Field:%s value:%d", fieldName.c_str(), value);
                return;
            } else if(fieldName.compare("Emergency Lane Maintenance")==0) {
                android::base::SetProperty("persist.gecko.variant.emergency_lane", decToHex(value));
                ALOGI("Field:%s value:%d", fieldName.c_str(), value);
                return;
            } else if(fieldName.compare("Lateral Blind Area Assistance")==0) {
                android::base::SetProperty("persist.gecko.variant.blindspot_asst", decToHex(value));
                ALOGI("Field:%s value:%d", fieldName.c_str(), value);
                return;
            } else if(fieldName.compare("Backward Blind Area Assistance")==0) {
                android::base::SetProperty("persist.gecko.variant.rear_x_alert", decToHex(value));
                ALOGI("Field:%s value:%d", fieldName.c_str(), value);
                return;
            } else if(fieldName.compare("Meter")==0) {
                android::base::SetProperty("persist.gecko.variant.instr_cluster", decToHex(value));
                ALOGI("Field:%s value:%d", fieldName.c_str(), value);
                return;
            } else if(fieldName.compare("Energy Type")==0) {
                android::base::SetProperty("persist.gecko.variant.powertrain", decToHex(value));
                ALOGI("Field:%s value:%d", fieldName.c_str(), value);
                return;
            }  else if(fieldName.compare("Steering Wheel")==0) {
                android::base::SetProperty("persist.gecko.variant.steering_wheel", decToHex(value));
                ALOGI("Field:%s value:%d", fieldName.c_str(), value);
                return;
            } else if(fieldName.compare("Steering Wheel Heating")==0) {
                android::base::SetProperty("persist.gecko.variant.steering_heat", decToHex(value));
                ALOGI("Field:%s value:%d", fieldName.c_str(), value);
                return;
            } else if(fieldName.compare("Tyre")==0) {
                android::base::SetProperty("persist.gecko.variant.tire_cfg", decToHex(value));
                ALOGI("Field:%s value:%d", fieldName.c_str(), value);
                return;
            } else if(fieldName.compare("Tyre Pressure Monitoring System")==0) {
                android::base::SetProperty("persist.gecko.variant.tpms", decToHex(value));
                ALOGI("Field:%s value:%d", fieldName.c_str(), value);
                return;
            } else if(fieldName.compare("Reverse Image")==0) {
                android::base::SetProperty("persist.gecko.variant.rearview_cam", decToHex(value));
                ALOGI("Field:%s value:%d", fieldName.c_str(), value);
                return;
            } else if(fieldName.compare("Parking Assist Radar")==0) {
                android::base::SetProperty("persist.gecko.variant.park_radar", decToHex(value));
                ALOGI("Field:%s value:%d", fieldName.c_str(), value);
                return;
            } else if(fieldName.compare("Cockpit Air Conditioning")==0) {
                android::base::SetProperty("persist.gecko.variant.climate_ctrl", decToHex(value));
                ALOGI("Field:%s value:%d", fieldName.c_str(), value);
                return;
            } else if(fieldName.compare("Large Screen Assembly")==0) {
                android::base::SetProperty("persist.gecko.variant.info_screen", decToHex(value));
                ALOGI("Field:%s value:%d", fieldName.c_str(), value);
                return;
            } else if(fieldName.compare("Navigation")==0) {
                android::base::SetProperty("persist.gecko.variant.nav_system", decToHex(value));
                ALOGI("Field:%s value:%d", fieldName.c_str(), value);
                return;
            } else if(fieldName.compare("Driving Video Recorder")==0) {
                android::base::SetProperty("persist.gecko.variant.dashcam", decToHex(value));
                ALOGI("Field:%s value:%d", fieldName.c_str(), value);
                return;
            } else if(fieldName.compare("Intelligent Vehicle System")==0) {
                android::base::SetProperty("persist.gecko.variant.ivis", decToHex(value));
                ALOGI("Field:%s value:%d", fieldName.c_str(), value);
                return;
            } else if(fieldName.compare("Intelligent Cargo Hold System")==0) {
                android::base::SetProperty("persist.gecko.variant.smart_cargo", decToHex(value));
                ALOGI("Field:%s value:%d", fieldName.c_str(), value);
                return;
            } else if(fieldName.compare("DMS Driver Monitoring System")==0) {
                android::base::SetProperty("persist.gecko.variant.dms", decToHex(value));
                ALOGI("Field:%s value:%d", fieldName.c_str(), value);
                return;
            } else if(fieldName.compare("Portable Power Source V2L")==0) {
                android::base::SetProperty("persist.gecko.variant.v2l", decToHex(value));
                ALOGI("Field:%s value:%d", fieldName.c_str(), value);
                return;
            } else if(fieldName.compare("Cruise Control")==0) {
                android::base::SetProperty("persist.gecko.variant.adapt_cruise", decToHex(value));
                ALOGI("Field:%s value:%d", fieldName.c_str(), value);
                return;
            } else if(fieldName.compare("Hill Descent Control")==0) {
                android::base::SetProperty("persist.gecko.variant.hill_desc", decToHex(value));
                ALOGI("Field:%s value:%d", fieldName.c_str(), value);
                return;
            } else if(fieldName.compare("Illuminated Entry System")==0) {
                android::base::SetProperty("persist.gecko.variant.ambient_light", decToHex(value));
                ALOGI("Field:%s value:%d", fieldName.c_str(), value);
                return;
            } else if(fieldName.compare("Peristalsis Control")==0) {
                android::base::SetProperty("persist.gecko.variant.traction_ctrl", decToHex(value));
                ALOGI("Field:%s value:%d", fieldName.c_str(), value);
                return;
            } else if(fieldName.compare("YEAR1")==0) {
                mDate += static_cast<char>(value);
                ALOGI("Field:%s value:%d", fieldName.c_str(), value);
                return;
            } else if(fieldName.compare("YEAR2")==0) {
                mDate += static_cast<char>(value);
                ALOGI("Field:%s value:%d", fieldName.c_str(), value);
                return;
            } else if(fieldName.compare("MONTH1")==0) {
                mDate += static_cast<char>(value);
                ALOGI("Field:%s value:%d", fieldName.c_str(), value);
                return;
            } else if(fieldName.compare("MONTH2")==0) {
                mDate += static_cast<char>(value);
                ALOGI("Field:%s value:%d", fieldName.c_str(), value);
                return;
            } else if(fieldName.compare("DAY1")==0) {
                mDate += static_cast<char>(value);
                ALOGI("Field:%s value:%d", fieldName.c_str(), value);
                return;
            } else if(fieldName.compare("DAY2")==0) {
                mDate += static_cast<char>(value);
                android::base::SetProperty("persist.gecko.product.date", mDate.c_str());
                ALOGI("Field:%s value:%d", fieldName.c_str(), value);
                return;
            } else if(fieldName.compare("SN1")==0) {
                mVehSn += static_cast<char>(value);
                ALOGI("Field:%s value:%d", fieldName.c_str(), value);
                return;
            } else if(fieldName.compare("SN2")==0) {
                mVehSn += static_cast<char>(value);
                ALOGI("Field:%s value:%d", fieldName.c_str(), value);
                return;
            } else if(fieldName.compare("SN3")==0) {
                mVehSn += static_cast<char>(value);
                ALOGI("Field:%s value:%d", fieldName.c_str(), value);
                return;
            } else if(fieldName.compare("SN4")==0) {
                mVehSn += static_cast<char>(value);
                ALOGI("Field:%s value:%d", fieldName.c_str(), value);
                return;
            } else if(fieldName.compare("SN5")==0) {
                mVehSn += static_cast<char>(value);
                ALOGI("Field:%s value:%d", fieldName.c_str(), value);
                return;
            } else if(fieldName.compare("SN6")==0) {
                mVehSn += static_cast<char>(value);
                android::base::SetProperty("gecko.serial.number", mVehSn.c_str());
                ALOGI("Field:%s value:%d", fieldName.c_str(), value);
                return;
            } else {
                ALOGI("Field %s not found!!!", fieldName.c_str());
                return;
            }
            //return static_cast<int32_t>(value);
        }
        ALOGI("Field %s not found!!!", fieldName.c_str());
        return;
    }
    // Method to load JSON from file
    Json::Value ConfigurationWordParse::loadJsonFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            //throw std::runtime_error("Could not open file: " + filename);
            ALOGI("loadJsonFromFile file %s could not open!!!", filename.c_str());
        }

        Json::Value root;
        Json::CharReaderBuilder reader;
        std::string errs;

        if (!parseFromStream(reader, file, &root, &errs)) {
            //throw std::runtime_error("Error parsing JSON: " + errs);
            ALOGI("loadJsonFromFile error parsing JSON:%s!!!", errs.c_str());
        }

        return root;
    }


    int ConfigurationWordParse::vehicle_vendor_storage_read(uint16_t id, uint16_t len, std::vector<uint8_t>& out_data)
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
    void ConfigurationWordParse::parseEcuConfig(uint16_t config_id, std::string name,const Json::Value& root) {
    int ret = 0;
    std::vector<uint8_t> out_value;
    uint16_t hex_len = 0;
    uint16_t storage_id = 0;
    uint8_t length_err = 0;
    uint8_t config_id_err = 0;
    std::string area;
    std::vector<std::string> nodes;  
    switch (config_id)
    {
        case 0xf101:
            storage_id = VENDOR_VEHICLE_FEATURE_ID;
            hex_len = VEHICLE_FEATURE_MSG_LEN;
            area = "0xF101";
            nodes = f101Nodes;
            if (hex_len != VEHICLE_FEATURE_MSG_LEN) {
                ALOGE("hex_len %d err, need %d",hex_len, VEHICLE_FEATURE_MSG_LEN);
                length_err = 1;
            }
            break;
        case 0xf011:
            storage_id = VENDOR_VEHICLE_TSP_ID;
            hex_len = VEHICLE_TSP_MSG_LEN;
            area = "0xF011";
            nodes = {};
            if (hex_len != VEHICLE_TSP_MSG_LEN) {
                ALOGE("hex_len %d err, need %d",hex_len, VEHICLE_TSP_MSG_LEN);
                length_err = 1;
            }
            break;
        case 0xf016:
            storage_id = VENDOR_VEHICLE_TYPE_ID;
            hex_len = VEHICLE_TYPE_MSG_LEN;
            area = "0xF016";
            nodes = {};
            if (hex_len != VEHICLE_TYPE_MSG_LEN) {
                ALOGE("hex_len %d err, need %d",hex_len, VEHICLE_TYPE_MSG_LEN);
                length_err = 1;
            }
            break;
        case 0xf187:
            storage_id = VENDOR_VEHICLE_OEM_DEV_ID;
            hex_len = VEHICLE_OEM_MSG_LEN;
            area = "0xF187";
            nodes = {}; 
            if (hex_len != VEHICLE_OEM_MSG_LEN) {
                ALOGE("hex_len %d err, need %d",hex_len, VEHICLE_OEM_MSG_LEN);
                length_err = 1;
            }
            break;
        case 0xf190:
            storage_id = VENDOR_VEHICLE_VIN_ID;
            hex_len = VEHICLE_VIN_MSG_LEN;
            area = "0xF190";
            nodes = {};
            if (hex_len != VEHICLE_VIN_MSG_LEN) {
                ALOGE("hex_len %d err, need %d",hex_len, VEHICLE_VIN_MSG_LEN);
                length_err = 1;
            }
            break;
        case 0xf18C:
            storage_id = VENDOR_ECU_SN_ID;
            hex_len = VEHICLE_ECU_MSG_LEN;
            area = "0xF18C";
            nodes = f18CNodes;
            if (hex_len != VEHICLE_ECU_MSG_LEN) {
                ALOGE("hex_len %d err, need %d",hex_len, VEHICLE_ECU_MSG_LEN);
                length_err = 1;
            }
            break;
        case 0xff01:
            storage_id = VENDOR_MCU_VERSION_ID;
            hex_len = MCU_VERSION_MSG_LEN;
            area = "0xFF01";
            nodes = {};
            if (hex_len != MCU_VERSION_MSG_LEN) {
                ALOGE("hex_len %d err, need %d",hex_len, MCU_VERSION_MSG_LEN);
                length_err = 1;
            }
            break;
        case 0xff02:
            storage_id = VENDOR_MCU_AB_PT_ID;
            hex_len = MCU_AB_PT_MSG_LEN;
            area = "0xFF02";
            nodes = {};
            if (hex_len != MCU_AB_PT_MSG_LEN) {
                ALOGE("hex_len %d err, need %d",hex_len, MCU_AB_PT_MSG_LEN);
                length_err = 1;
            }
            break;
        case 0xffff: //add for hal
            storage_id = VENDOR_VEHICLE_HAL_FEATURE_ID;
            hex_len = VEHICLE_HAL_FEATURE_LEN;
            area = "0xFFFF";
            nodes = ffffNodes;
            if (hex_len != VEHICLE_HAL_FEATURE_LEN) {
                ALOGE("hex_len %d err, need %d",hex_len, VEHICLE_HAL_FEATURE_LEN);
                length_err = 1;
            }
            break;
        default:
            ALOGE("Unknown config_id: 0x%x", config_id);
            config_id_err = 1;
            length_err = 1;
            area = "UNKNOW";
            break;
    }
    if (length_err) {
        ALOGE("hex_len err do nothing...");
        return;
    }
    if (config_id_err) {
        ALOGE("config_id err do nothing...");
        return;
    }
    ret = vehicle_vendor_storage_read(storage_id, hex_len, out_value);
    if (0 != ret) {
         ALOGE("vendor storage read err, storage_id:%d", storage_id);
    } else {
        Json::Value fields;
        std::string result;
        bool isF187 = (name.compare("F187")==0);
        bool isF190 = (name.compare("F190")==0);
        bool isMcuVersion = (name.compare("McuVersion")==0);
        bool isFF02 = (name.compare("FF02")==0);
        if(isF187 || isF190 || isMcuVersion) {
            for(int i = 0; i < hex_len; i++){
                result += static_cast<char> (out_value[i]);
            }
            ALOGE("%s vendor storage result:%s", name.c_str(), result.c_str());
            if(isF187) {
                android::base::SetProperty("persist.gecko.oem.part.no", result.c_str());
            } else if(isF190) {
                android::base::SetProperty("gecko.vin", result.c_str());
            } else if(isMcuVersion) {
                android::base::SetProperty("gecko.mcu.version", result.c_str());
            }
            return;
        }
        if(isFF02) {
            ALOGE("%s vendor storage result:%d", "ABStatus", out_value[0]);
            if((out_value[0] & 0x80) == 0x80) {
                ALOGI("cmd get to set ABstatus to B");
                android::base::SetProperty("gecko.mcu.abstatus", "B");
            } else {
                ALOGI("cmd get to set ABstatus to A");
                android::base::SetProperty("gecko.mcu.abstatus", "A");
            }
            return;
        }
        // Get field
        if(root.isArray()) {
           for(unsigned int index = 0; index < root.size(); ++index) {
               Json::Value subroot = root[index];
               ALOGE("subroot['DID'] value:%s; area:%s", subroot["DID"].asString().c_str(), area.c_str() );
               bool isArea = (subroot["DID"].asString().compare(area)==0);
               if(isArea) {
                   fields = subroot["Fields"];
                   for(int i = 0; i < (int (nodes.size())); i++) {
                       parseFieldFromBytes(fields, out_value, nodes[i]);
                   }
               }
           }
        }
    }
    return;
    }

    void ConfigurationWordParse::initConfigurationWordData(PropertyManager* propertyManager) {
         mRoot = loadJsonFromFile("/vendor/etc/ecu-config.json");
         mPropertyManager = propertyManager;
         parseEcuConfig(0xf101, "F101", mRoot);
         parseEcuConfig(0xffff, "FFFF", mRoot);
         parseEcuConfig(0xf18C, "F18C", mRoot);
         parseEcuConfig(0xf187, "F187", mRoot);
         parseEcuConfig(0xf190, "F190", mRoot);
         parseEcuConfig(0xff01, "McuVersion", mRoot);
         parseEcuConfig(0xff02, "FF02", mRoot);
         return;
    }
}  // namespace impl

}  // namespace V2_0
}  // namespace vehicle
}  // namespace automotive
}  // namespace hardware
}  // namespace android

