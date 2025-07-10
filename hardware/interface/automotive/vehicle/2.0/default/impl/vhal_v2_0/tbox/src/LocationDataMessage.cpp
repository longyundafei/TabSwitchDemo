#include "LocationDataMessage.h"
#include <bitset>
#include <cmath>
#include <iostream>
#include <vector>
#define PROPERTY_VALUE_MAX_LEN 250

LocationDataMessage::LocationDataMessage(uint8_t locationStatus, double longitude, double latitude)
    : locationStatus(locationStatus), longitude(longitude), latitude(latitude) {}
LocationDataMessage::LocationDataMessage() {}

std::vector<uint8_t> LocationDataMessage::generateDataPacket() {
    std::vector<uint8_t> dataPacket;
    dataPacket.push_back(locationdataid);
    // 添加定位状态
    char prop[PROPERTY_VALUE_MAX_LEN] = "\0";
    std::string statusLoc;
    if(property_get("persist.location.validity", prop, nullptr)!=0){
        statusLoc = prop;
        locationStatus = static_cast<uint8_t>(stoi(statusLoc));
        //LOG(INFO) << "locationStatus = " << std::dec << static_cast<int>(locationStatus);
    } else {
        locationStatus = 1;
        LOG(INFO) << "ERROR : can not get locationStatus!!!";
    }

    memset(prop, 0, sizeof(prop));
    double temp = 0;
    std::string latitude;
    int32_t latitudeValue = 0;
    if(property_get("persist.latitude", prop, nullptr)!=0){
        latitude = prop;
        temp = stod(latitude);
        if(temp < 0){
            locationStatus = locationStatus | 2;
        }
        latitudeValue = fabs(temp)*1000000;
        //LOG(INFO) << "latitudeValue = " << latitudeValue;
    } else {
        LOG(INFO) << "ERROR : can not get latitude!!!";
    }

    memset(prop, 0, sizeof(prop));
    std::string longitude;
    int32_t longitudeValue = 0;
    if(property_get("persist.longitude", prop, nullptr)!=0){
        longitude = prop;
        temp = stod(longitude);
        if(temp < 0){
            locationStatus = locationStatus | 4;
        }
        longitudeValue = fabs(temp)*1000000;
        //LOG(INFO) << "longitudeValue = " << longitudeValue;
    } else {
        LOG(INFO) << "ERROR : can not get longitude!!!";
    }
    //LOG(INFO) << "locationStatus is " <<  std::dec << static_cast<int>(locationStatus);
    dataPacket.push_back(locationStatus);

    // 添加经度
    dataPacket.push_back((longitudeValue >> 24) & 0xFF);
    dataPacket.push_back((longitudeValue >> 16) & 0xFF);
    dataPacket.push_back((longitudeValue >> 8) & 0xFF);
    dataPacket.push_back(longitudeValue & 0xFF);

    // 添加纬度
    dataPacket.push_back((latitudeValue >> 24) & 0xFF);
    dataPacket.push_back((latitudeValue >> 16) & 0xFF);
    dataPacket.push_back((latitudeValue >> 8) & 0xFF);
    dataPacket.push_back(latitudeValue & 0xFF);

    return dataPacket;
}

void LocationDataMessage::printMessage() {
    /*std::cout << "信息ID: " << static_cast<int>(locationdataid) << std::endl;
    std::cout << "定位状态: " << std::bitset<8>(locationStatus) << std::endl;
    std::cout << "经度: " << longitude << "°" << std::endl;
    std::cout << "纬度: " << latitude << "°" << std::endl;*/
    LOG(INFO) << "信息ID: " << static_cast<int>(locationdataid);
    LOG(INFO) << "定位状态: " << std::bitset<8>(locationStatus);
    LOG(INFO) << "经度: " << longitude << "°";
    LOG(INFO) << "纬度: " << latitude << "°";
}
