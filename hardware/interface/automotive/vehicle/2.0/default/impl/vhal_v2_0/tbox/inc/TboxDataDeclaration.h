#ifndef TBOXDATADECLARATION_H
#define TBOXDATADECLARATION_H
#include <android/hardware/automotive/vehicle/2.0/IVehicle.h>

using namespace android::hardware::automotive::vehicle::V2_0;

struct ConfigDeclaration {
    uint32_t prop;
    std::string name;
    uint32_t areaid;
    std::string type;
    float prop_res;
    int prop_offset;
    uint32_t prop_muls;
    uint32_t tbox_value_size;
    uint32_t tbox_value_min;
    uint32_t tbox_value_max;
    int tbox_value_offset;
    uint32_t tbox_value_muls;
    std::map<int32_t, std::string> TboxRflex;
};
struct TboxData {
    int type = 0;
    uint8_t bytevalue = 0;
    uint16_t uint16value = 0;
    uint32_t uint32value = 0;
    float floatvalue = 0.0;
    std::string stringvalue;
};
struct TboxDataDeclaration {
    ConfigDeclaration conf;
    VehiclePropValue value;
    TboxData tvalue;
};
struct Messagestruct{
    std::string type;
    TboxData datavalue;
    std::vector<SubscribeOptions> opt;
    std::map<int32_t, std::string> TboxRflex;
};
struct vehicleMessage{
    uint8_t id;
    std::vector<Messagestruct> msg;
};
struct TboxRealMessage{
    vehicleMessage vehiclemsg;
};
#endif