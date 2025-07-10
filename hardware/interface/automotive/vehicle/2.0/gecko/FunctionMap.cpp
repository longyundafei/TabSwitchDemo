#include "FunctionMap.h"
#include "ev_can_ccm_4_0.h"
#include "body_can_ccm_4_0.h"
#include "chassise_can_ccm_4_0.h"
#include <iostream>


void FunctionMap::function2(std::vector<uint8_t>& dataContent) {
    // TODO: 实现函数2的逻辑
    std::cout << "Function 2 called" << std::endl;
}
void FunctionMap::a_ev_can_ccm_4_0_gw_vcu_250_unpack(std::vector<uint8_t>& message){
    struct ev_can_ccm_4_0_gw_vcu_250_t ev250;
	ev_can_ccm_4_0_gw_vcu_250_unpack(&ev250,message.data(),message.size());
        std::cout << "vcu_pump_state: " << ev250.vcu_pump_state << std::endl;
        std::cout << "vcu_mid_door_ajar_status: " << ev250.vcu_mid_door_ajar_status << std::endl;
        std::cout << "vcu_actual_rema_range_vd: " << ev250.vcu_actual_rema_range_vd << std::endl;
        std::cout << "vcu_cltc_rema_range_vd: " << ev250.vcu_cltc_rema_range_vd << std::endl;
        std::cout << "vcu_pt_cenable: " << ev250.vcu_pt_cenable << std::endl;
        std::cout << "vcu_accmenable: " << ev250.vcu_accmenable << std::endl;
        std::cout << "vcu_vehicle_spd: " << ev250.vcu_vehicle_spd << std::endl;
        std::cout << "vcu_actual_rema_range: " << ev250.vcu_actual_rema_range << std::endl;
        std::cout << "vcu_cltc_rema_range: " << ev250.vcu_cltc_rema_range << std::endl;
        std::cout << "vcu_livecounter_0x250: " << ev250.vcu_livecounter_0x250 << std::endl;
        std::cout << "vcu_checksum_0x250: " << ev250.vcu_checksum_0x250 << std::endl;
    propmanager.updatePropertyValue("ec_vcu_vehicle_spd",std::to_string(ev250.vcu_vehicle_spd));
}
// 静态映射表的初始化
//const std::unordered_map<uint16_t, FunctionPtr> FunctionMap::functionMap_ = {
//    {0x0250, &FunctionMap::a_ev_can_ccm_4_0_gw_vcu_250_unpack},  // 映射 ID 0x0250 到函数 function1
//    {0xABCD, &FunctionMap::function2}   // 映射 ID 0xABCD 到函数 function2
//};
//const std::unordered_map<uint16_t, FunctionMap::FunctionType> FunctionMap::functionMap_ = {
//    {0x0250, std::bind(&FunctionMap::a_ev_can_ccm_4_0_gw_vcu_250_unpack, this, std::placeholders::_1)},
//    {0xABCD, std::bind(&FunctionMap::function2, this, std::placeholders::_1)}
//};
void FunctionMap::addparam(){
    propmanager.importPropertiesFromFile("properties.txt");
}

// 根据ID调用相应的函数
void FunctionMap::callFunction(uint16_t id,std::vector<uint8_t>& dataContent) {
  FunctionMap obj; 
  auto it = obj.functionMap_.find(id);
//    auto it = functionMap_.find(id);
//    std::cout << " callFunction id " << it->second << std::endl;
    if (it != obj.functionMap_.end()) {
        auto function = it->second;
        function(dataContent);
    }
}

