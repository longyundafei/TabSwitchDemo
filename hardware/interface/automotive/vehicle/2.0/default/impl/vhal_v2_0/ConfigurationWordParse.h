// ConfigurationWordParse.h
#ifndef CONFIGURATION_WORD_PARSE_H
#define CONFIGURATION_WORD_PARSE_H

#include <iostream>
#include <string>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <termios.h>
#include <string>
#include <vector>
#include <json/json.h>
#include "property_manager.h"
namespace android {
namespace hardware {
namespace automotive {
namespace vehicle {
namespace V2_0 {

namespace impl {

#define VENDOR_REQ_TAG      0x56524551
#define VENDOR_READ_IO      _IOW('v', 0x01, unsigned int)
#define VENDOR_WRITE_IO     _IOW('v', 0x02, unsigned int)
#define VENDOR_VEHICLE_FEATURE_ID             (16)
#define VENDOR_VEHICLE_VIN_ID                 (17)
#define VENDOR_ECU_SN_ID                      (18)
#define VENDOR_VEHICLE_TSP_ID                 (19)
#define VENDOR_VEHICLE_TYPE_ID                (20)
#define VENDOR_VEHICLE_OEM_DEV_ID             (21)
#define VENDOR_MCU_VERSION_ID                 (22)
#define VENDOR_MCU_AB_PT_ID                   (23)

#define VENDOR_VEHICLE_HAL_FEATURE_ID         (100)

#define VEHICLE_FEATURE_MSG_LEN               (22)
#define VEHICLE_VIN_MSG_LEN                   (17)
#define VEHICLE_ECU_MSG_LEN                   (15)
#define VEHICLE_TSP_MSG_LEN                   (4)
#define VEHICLE_TYPE_MSG_LEN                  (2)
#define VEHICLE_OEM_MSG_LEN                   (16)
#define MCU_VERSION_MSG_LEN                   (11)
#define MCU_AB_PT_MSG_LEN                     (1)

#define VEHICLE_HAL_FEATURE_LEN               (7)
struct gecko_vendor_req {
        uint32_t tag;
        uint16_t id;
        uint16_t len;
        uint8_t data[1024];
    };
class ConfigurationWordParse {
    public:
        Json::Value mRoot;
        std::string mVehSn;
        std::string mDate;
        PropertyManager* mPropertyManager; 
        std::vector<std::string> ffffNodes = {"SLLevel", "SLVersion", "SLOL", "CHGCOND", "CountryId", "BRKDISWIP", "ChrgTarSoc"};
        std::vector<std::string> f101Nodes = {"Platform","Model Series", "Model Year", "Vehicle Usage", "Vehicle Series Subdivision", "Driving Type", "LH/RH Drive+Sliding Door Type", "Brand", "Country/Region", "Language", "Ext Color", "Driver's Seat Heating&Ventilate", "Longitudinal Active Safety", "Lateral Active Safety", "Adaptive Cruise Control", "Intelligent Speed Assistance", "Intelligent High Beam Control", "Lane Centering Control", "Emergency Lane Maintenance", "Lateral Blind Area Assistance", "Backward Blind Area Assistance", "Meter", "Energy Type", "Steering Wheel", "Steering Wheel Heating", "Tyre", "Tyre Pressure Monitoring System", "Reverse Image", "Parking Assist Radar", "Cockpit Air Conditioning", "Large Screen Assembly", "Navigation", "Driving Video Recorder", "Intelligent Vehicle System", "Intelligent Cargo Hold System", "DMS Driver Monitoring System", "Portable Power Source V2L", "Cruise Control", "Hill Descent Control", "Illuminated Entry System", "Peristalsis Control"};
        std::vector<std::string> f18CNodes = {"YEAR1", "YEAR2", "MONTH1", "MONTH2", "DAY1", "DAY2", "SN1","SN2", "SN3", "SN4", "SN5","SN6"};
        ConfigurationWordParse();
        ~ConfigurationWordParse();
        uint8_t extract_bit_range(uint8_t byte, int start_bit, int end_bit);
        std::string find_description(const Json::Value& descriptions, uint8_t value);
        std::string decToHex(int num);
        void parseFieldFromBytes(const Json::Value& fields, const std::vector<uint8_t>& data, const std::string& fieldName);
        Json::Value loadJsonFromFile(const std::string& filename);
        int vehicle_vendor_storage_read(uint16_t id, uint16_t len, std::vector<uint8_t>& out_data);
        void parseEcuConfig(uint16_t config_id, std::string name, const Json::Value& fields);
        void initConfigurationWordData(PropertyManager* propertyManager);
    };
}  // impl

}  // namespace V2_0
}  // namespace vehicle
}  // namespace automotive
}  // namespace hardware
}  // namespace android
#endif
