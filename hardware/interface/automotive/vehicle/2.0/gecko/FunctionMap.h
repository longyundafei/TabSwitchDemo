#ifndef FUNCTION_MAP_H
#define FUNCTION_MAP_H

#include <cstdint>
#include <functional>
#include <vector>
#include <unordered_map>
#include <iostream>
#include "PropertyManager.h"
// 定义函数指针类型
//typedef void (*FunctionPtr)(std::vector<uint8_t>& dataContent);

// 定义映射类
class FunctionMap {
public:
    // 构造函数，在这里初始化映射表
  FunctionMap() {
    functionMap_ = {
      {0x024f, std::bind(&FunctionMap::a_ev_can_ccm_4_0_gw_vcu_250_unpack, this, std::placeholders::_1)},
      {0xABCC, std::bind(&FunctionMap::function2, this, std::placeholders::_1)}  
    };
  }
    // 根据ID调用相应的函数
    static void callFunction(uint16_t id,std::vector<uint8_t>& dataContent);
    void addparam();
public:
    PropertyManager propmanager;
private:
    typedef std::function<void(std::vector<uint8_t>&)> FunctionType;
    // 静态映射表
  std::unordered_map<uint16_t, FunctionType> functionMap_;
private:
    void a_ev_can_ccm_4_0_gw_vcu_250_unpack(std::vector<uint8_t>& message);
    void function2(std::vector<uint8_t>& dataContent);
};

#endif  // FUNCTION_MAP_H

