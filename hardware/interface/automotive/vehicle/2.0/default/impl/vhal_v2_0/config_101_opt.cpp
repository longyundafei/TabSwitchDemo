#include <string>
#include <vector>
#include <log/log.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <iostream>
#include <sstream>
#include <sys/ioctl.h>

#define VENDOR_REQ_TAG                  0x56524551
#define VENDOR_READ_IO                  _IOW('v', 0x01, unsigned int)
#define VENDOR_WRITE_IO                 _IOW('v', 0x02, unsigned int)

#define VENDOR_VEHICLE_FEATURE_ID             (16)
#define VEHICLE_FEATURE_MSG_LEN               (22)

#define VENDOR_SET_FLAG_ID                    (50)
#define VENDOR_SET_FLAG_LEN                   (8)

struct gecko_vendor_req {
    uint32_t tag;
    uint16_t id;
    uint16_t len;
    uint8_t data[1024];
};

int vehicle_vendor_storage_read(uint16_t id, uint16_t len, std::vector<uint8_t>& out_data)
{
    int ret = 0;
    uint32_t i = 0;
    uint8_t p_buf[1124];
    struct gecko_vendor_req *req;

    req = (struct gecko_vendor_req *)p_buf;
    int sys_fd = open("/dev/vendor_storage", O_RDWR, 0);
    if (sys_fd < 0) {
        std::cout << "vendor_storage open fail " << std::endl;
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
            std::cout << "vehicle_vendor_storage_read " << hex_string.c_str() << std::endl;
        }
        std::cout << "read vendor storage success" <<std::endl;
        close(sys_fd);
        return 0;
    }
    else {
        std::cout << "read vendor storage error ret" <<std::endl;
        close(sys_fd);
        return -2;  // IO 操作失败
    }
}

int vehicle_vendor_storage_write(uint16_t id, uint16_t len, const std::vector<uint8_t>& write_value)
{
    int ret = 0;
    uint32_t i = 0;
    uint8_t p_buf[1124];
    struct gecko_vendor_req *req;

    if (write_value.empty()) {
        std::cout << "ITEM process_cmd value is empty" <<std::endl;
        return -1;
    }

    req = (struct gecko_vendor_req *)p_buf;
    int sys_fd = open("/dev/vendor_storage", O_RDWR, 0);
    if (sys_fd < 0) {
        std::cout << "vendor_storage open fail" <<std::endl;
        return -1;  // 打开设备失败
    }

    req->tag = VENDOR_REQ_TAG;
    req->id = id;
    req->len = len;

    if (write_value.size() < len) {
        std::cout << "Error: write_value length " << write_value.size() << " is less than expected " << len << std::endl;
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
    std::cout << "vehicle_vendor_storage_write:" << hex_string.c_str() << std::endl;
    ret = ioctl(sys_fd, VENDOR_WRITE_IO, req);
    if (ret) {
        ALOGE("write vendor storage error %d \n", ret);
        std::cout << "read vendor storage error ret" <<std::endl;
        close(sys_fd);
        return -2;
    }
    std::cout << "write vendor storage success\n" <<std::endl;
    close(sys_fd);
    return 0;
}

typedef struct config_item {
    uint8_t byte;
    uint8_t bitmask;
    const char* item_name;
}CONFIG_ITME_T;
const CONFIG_ITME_T config_item_s [41] {
    {0, 0x0f, "PLT"}, {0, 0xf0, "ModS"}, {1, 0xff, "ModY"},
    {2, 0x0f, "VehU"}, {2, 0x0f, "VehSS"}, {3, 0x0f, "DriT"},
    {3, 0xf0,"DriSDT"}, {4, 0x0f,"BRAND"}, {3, 0xff,"ConR"},
    {6, 0x0f,"LANG"}, {6, 0xf0,"COLOR"}, {7, 0x0f,"DriSH"},
    {7, 0xf0,"LonAS"}, {8, 0x0f,"LaAS"}, {8, 0xf0,"AdapCC"},
    {9, 0x0f, "IntSA"}, {9, 0xf0,"IntHBC"}, {10, 0x0f,"LanCC"},
    {10, 0xf0,"EmerLM"}, {11, 0x0f,"LatBAA"}, {11, 0xf0,"BackBAA"},
    {12, 0x0f,"METER"}, {12, 0xf0,"EnT"}, {13, 0x0f,"StW"},
    {13, 0xf0,"StWH"}, {14, 0x0f,"TYRE"}, {14, 0xf0,"TyrePMS"}, 
    {15, 0x0f,"RevI"}, {15, 0xf0,"ParAR"}, {16, 0x0f,"CoAC"},
    {16, 0xf0,"LaSA"}, {17, 0x0f,"Nav"}, {17, 0xf0,"DriVR"},
    {18, 0x0f,"IntVS"}, {18, 0xf0,"IntCHS"}, {19, 0x0f,"DMS"},
    {19, 0xf0,"PPS"}, {20, 0x0f,"CrC"}, {20, 0xf0,"HiDC"},
    {21, 0x0f,"IllES"}, {21, 0xf0,"PsC"}
};
int main(int argc, char* argv[])
{   int i = 0, j = 0, k = 0;
    int ret = 0;
    uint8_t mb  = 0;
    std::vector<uint8_t> config_value (22, 0x00);
    if (argc < 2 || 0 == strcmp("help", argv[1]) || 0 == strcmp("h", argv[1])) {
        fprintf(stderr, "        set F101 item config ...\n");
        fprintf(stderr, "        PLT      : Platform \n");
        fprintf(stderr, "        ModS     : Model Series \n");
        fprintf(stderr, "        ModY     : Model Year \n");
        fprintf(stderr, "        VehU     : Vehicle Usage \n");
        fprintf(stderr, "        VehSS    : Vehicle Series Subdivision \n");
        fprintf(stderr, "        DriT     : Driving Type \n");
        fprintf(stderr, "        DriSDT   : LH/RH Drive+Sliding Door Type \n");
        fprintf(stderr, "        BRAND    : Brand \n");
        fprintf(stderr, "        ConR     : Country/Region \n");
        fprintf(stderr, "        LANG     : Language \n");
        fprintf(stderr, "        COLOR    : Ext Color \n");
        fprintf(stderr, "        DriSH    : Driver's Seat Heating&Ventilate \n");
        fprintf(stderr, "        LonAS    : Longitudinal Active Safety \n");
        fprintf(stderr, "        LaAS     : Lateral Active Safety \n");
        fprintf(stderr, "        AdapCC   : Adaptive Cruise Control \n");
        fprintf(stderr, "        IntSA    : Intelligent Speed Assistance \n");
        fprintf(stderr, "        IntHBC   : Intelligent High Beam Control \n");
        fprintf(stderr, "        LanCC    : Lane Centering Control \n");
        fprintf(stderr, "        EmerLM   : Emergency Lane Maintenance \n");
        fprintf(stderr, "        LatBAA   : Lateral Blind Area Assistance \n");
        fprintf(stderr, "        BackBAA  : Backward Blind Area Assistance \n");
        fprintf(stderr, "        METER    : Meter \n");
        fprintf(stderr, "        EnT      : Energy Type \n");
        fprintf(stderr, "        StW      : Steering Wheel \n");
        fprintf(stderr, "        StWH     : Steering Wheel Heating \n");
        fprintf(stderr, "        TYRE     : Tyre \n");
        fprintf(stderr, "        TyrePMS  : Tyre Pressure Monitoring System  \n");
        fprintf(stderr, "        RevI     : Reverse Image \n");
        fprintf(stderr, "        ParAR    : Parking Assist Radar \n");
        fprintf(stderr, "        CoAC     : Cockpit Air Conditioning \n");
        fprintf(stderr, "        LaSA     : Large Screen Assembly \n");
        fprintf(stderr, "        Nav      : Navigation \n");
        fprintf(stderr, "        DriVR    : Driving Video Recorder \n");
        fprintf(stderr, "        IntVS    : Intelligent Vehicle System \n");
        fprintf(stderr, "        IntCHS   : Intelligent Cargo Hold System \n");
        fprintf(stderr, "        DMS      : DMS Driver Monitoring System \n");
        fprintf(stderr, "        PPS      : Portable Power SourceV2L \n");
        fprintf(stderr, "        CrC      : Cruise Control \n");
        fprintf(stderr, "        HiDC     : Hill Descent Control \n");
        fprintf(stderr, "        IllES    : Illuminated Entry System \n");
        fprintf(stderr, "        PsC      : Peristalsis Control \n");
        fprintf(stderr, "=======================================\n");
        fprintf(stderr, " enable or disable one feature, Usage: %s xxx 1 xxx 0 xxx 1 ...\n", argv[0]);
        fprintf(stderr, "=======================================\n");

        fprintf(stderr, "=======================================\n");
        fprintf(stderr, " clean f101 config, Usage: %s clean\n", argv[0]);
        fprintf(stderr, "=======================================\n");

        fprintf(stderr, "=======================================\n");
        fprintf(stderr, " set f101 config, Usage: %s set xx xx..\n", argv[0]);
        fprintf(stderr, "=======================================\n");

        fprintf(stderr, "=======================================\n");
        fprintf(stderr, " get f101 config, Usage: %s get\n", argv[0]);
        fprintf(stderr, "=======================================\n");
        return 0;
    }
    if (argc == 2) {
        if (0 == strcmp(argv[1], "clean")) {
            ret = vehicle_vendor_storage_write(VENDOR_VEHICLE_FEATURE_ID, VEHICLE_FEATURE_MSG_LEN, config_value);
            if (ret == 0){
                std::cout << "clean f101 success..." <<std::endl;
            }
            return 0;
        }
        if (0 == strcmp(argv[1], "get")) {
            ret = vehicle_vendor_storage_read(VENDOR_VEHICLE_FEATURE_ID, VEHICLE_FEATURE_MSG_LEN, config_value);
            if (ret == 0){
                std::cout << "read f101 success..." <<std::endl;
            }
            return 0;
        }
    }
    if (0 == strcmp(argv[1], "set")) {
        if (argc > 24) {
            std::cout << "too many argc " << argc <<std::endl;
            return 0;
        }
        k = 0;
        for (i=2; i<argc; i++) {
            config_value[k++] = std::stoi(std::string(argv[i]), nullptr, 16);
        }
        ret = vehicle_vendor_storage_write(VENDOR_VEHICLE_FEATURE_ID, VEHICLE_FEATURE_MSG_LEN, config_value);
        if (ret == 0){
            std::cout << "set f101 success..." <<std::endl;
        }
        return 0;
    }

    ret = vehicle_vendor_storage_read(VENDOR_VEHICLE_FEATURE_ID, VEHICLE_FEATURE_MSG_LEN, config_value);
	if (ret == 0){
        std::cout << "read VENDOR_VEHICLE_FEATURE_ID success..." <<std::endl;
	}
    else {
        std::cout << "read VENDOR_VEHICLE_FEATURE_ID err..." <<std::endl;
        return 0;
    }
    for (i = 1; i<argc; i+=2) {
        for(j = 0; j<41; j++) {
            if (0 == strcmp(argv[i], config_item_s[j].item_name)) {
                fprintf(stderr, "%s : %x\n", argv[i], std::stoi(std::string(argv[i+1]), nullptr, 16));
                mb = (config_item_s[j].bitmask == 0xf0) ? 4:0;
                k = config_item_s[j].byte;
                config_value[k] &= !config_item_s[j].bitmask;
                config_value[k] |= ((uint8_t)std::stoi(std::string(argv[i+1]), nullptr, 16))<<mb;
            }
        }
    }
    ret = vehicle_vendor_storage_write(VENDOR_VEHICLE_FEATURE_ID, VEHICLE_FEATURE_MSG_LEN, config_value);
	if (ret == 0){
        std::cout << "re write VENDOR_VEHICLE_FEATURE_ID success..." <<std::endl;
	}
    return 0;
}

