#include "LockVehicle.h"
#include <android-base/properties.h>

LockVehicle::LockVehicle(VehicleController& vehicle) {
    mVehicle = vehicle;
    mSpeedLimitLevel = 0xAA;
}

void LockVehicle::setLockVehicleMsg(LockVehicleMessage &msg){
    mLockVehicleMsg = msg;
}

/*
   0x55，一级限速（5km/h）
   0x56，二级限速（60km/h）
   0x57，三级限速（80km/h）
   0x58，四级限速（预留）
   0x59，五级限速（预留）
   0x60，库存融限速（5km/h）
   0xAA，不限速（解除限速）
*/

uint8_t LockVehicle::setSpeedLimitLevel(uint8_t chargingConditions, uint8_t level){
    LOG(INFO) << "LockVehicle::setSpeedLimitLevel, and chargingConditions is " << std::hex << static_cast<int>(chargingConditions) << std::endl;
    LOG(INFO) << "LockVehicle::setSpeedLimitLevel, and level is " << std::hex << static_cast<int>(level) << std::endl;
	VehiclePropValue charging_conditions;
    charging_conditions.prop = (int)VehicleProperty::GECKO_CCM_CHARGING_CONDIONS;
    charging_conditions.areaId = 0;
    charging_conditions.value.int32Values.resize(1);
    charging_conditions.value.int32Values[0] = chargingConditions;
    VehiclePropValue operat_licence;
    operat_licence.prop = (int)VehicleProperty::GECKO_CCM_OPERAT_LICENCE;
    operat_licence.areaId = 0;
    operat_licence.value.int32Values.resize(1);
    VehiclePropValue limit_level;
    limit_level.prop = (int)VehicleProperty::GECKO_CCM_SPEED_LIMIT_LEVEL;
    limit_level.areaId = 0;
    limit_level.value.int32Values.resize(1);

    switch(level){
        case 0x55:
            //一级限速
            limit_level.value.int32Values[0] = 1;
            operat_licence.value.int32Values[0] = 1;
            break;
        case 0x56:
            //二级限速
            limit_level.value.int32Values[0] = 2;
            operat_licence.value.int32Values[0] = 1;
            break;
        case 0x57:
            //三级限速
            limit_level.value.int32Values[0] = 3;
            operat_licence.value.int32Values[0] = 1;
            break;
        case 0x60:
            //库存融限速
            charging_conditions.value.int32Values[0] = 0;
            limit_level.value.int32Values[0] = 1;
            operat_licence.value.int32Values[0] = 1;
			break;
        case 0xaa:
            //不限速
            limit_level.value.int32Values[0] = 0;
            operat_licence.value.int32Values[0] = 0;
            break;
        case 0x58:
            //四级限速（预留）
		FALLTHROUGH_INTENDED;
        case 0x59:
            //五级限速（预留）
		FALLTHROUGH_INTENDED;
        default:
            LOG(INFO) << "LockVehicle: cannot handle SpeedLimitLevel msg, do nothing ";
            return 0;
            break;
    }
    LOG(INFO) << __func__ << "LockVehicle:  charging_conditions: " << std::hex << charging_conditions.value.int32Values[0];
    mVehicle.setProperty(charging_conditions);
    LOG(INFO) << __func__ << "LockVehicle:  operat_licence: " << std::hex << operat_licence.value.int32Values[0];
    mVehicle.setProperty(operat_licence);
    LOG(INFO) << __func__ << "LockVehicle:  limit_level: " << std::hex << limit_level.value.int32Values[0];
    mVehicle.setProperty(limit_level);
    StatusCode refStatus;
    VehiclePropValue refValue;
    /*mVehicle.getPropertybyPropId((int)VehicleProperty::GECKO_MCU_OPERAT_LICENCE, 0, refValue, refStatus);
    int vcuOperatlicence = refValue.value.int32Values[0];
    if (vcuOperatlicence == operat_licence.prop){
        LOG(INFO) << "LockVehicle::setSpeedLimitLevel, return OK;";
        return 1;
    } else{
        LOG(INFO) << "LockVehicle::setSpeedLimitLevel, return NOK;";
        return 0;
    }*/
    uint8_t resp = 1;
    for(auto i = 0; i<15; i++){
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        mVehicle.getPropertybyPropId((int)VehicleProperty::GECKO_VCU_SPEEDLIMITLEVELSETFB, 0, refValue, refStatus);
        if(refValue.value.int32Values[0] == 0x01){
            resp = 0;
            LOG(INFO) << "LockVehicle: got response from VCU_SpeedLimitLevelSetFb : " << static_cast<int>(resp);
            break;
        }
    }
    return resp;
}