#ifndef FUNCTION_MAP_H
#define FUNCTION_MAP_H

#include <cstdint>
#include <functional>
#include <vector>
#include <unordered_map>
#include <iostream>
#include "property_manager.h"
#include "MessageDefine.h"
namespace android::hardware::automotive::vehicle::V2_0::impl {
// 定义函数指针类型
typedef std::vector<uint8_t> (*packFunctionPtr)(void *buf);
typedef void (*unpackFunctionPtr)(std::vector<uint8_t>& dataContent, void *data,PropertyManager& pm);

// 定义映射类
class FunctionMap {
public:
    // 根据ID调用相应的函数
	static std::vector<uint8_t> packcallFunction(uint16_t id,void *buf, BusIdentifier busIdentifier);
    static void unpackcallFunction(uint16_t id,std::vector<uint8_t>& dataContent, void *data, BusIdentifier busIdentifier,PropertyManager& pm);

private:
    // 静态映射表
    //ev_can
    static const std::unordered_map<uint16_t, packFunctionPtr> pack_ev_can_;
    static const std::unordered_map<uint16_t, unpackFunctionPtr> unpack_ev_can_;

    //body_can
    static const std::unordered_map<uint16_t, packFunctionPtr> pack_body_can_;
    static const std::unordered_map<uint16_t, unpackFunctionPtr> unpack_body_can_;

    //chassise_can
    static const std::unordered_map<uint16_t, packFunctionPtr> pack_chassise_can_;
    static const std::unordered_map<uint16_t, unpackFunctionPtr> unpack_chassise_can_;

    //mcu
    static const std::unordered_map<uint16_t, packFunctionPtr> pack_mcu_;
    static const std::unordered_map<uint16_t, unpackFunctionPtr> unpack_mcu_;

    // ME11 canfd pack and unpack
    static const std::unordered_map<uint16_t, packFunctionPtr> pack_me11_can_;
    static const std::unordered_map<uint16_t, unpackFunctionPtr> unpack_me11_can_;    
};
}
#endif  // FUNCTION_MAP_H

