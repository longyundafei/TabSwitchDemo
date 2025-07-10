/*
 * Copyright (C) 2017 The Android Open Source Project
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
#define LOG_TAG "VehicleEmulator_v2_0"
#include <android/log.h>

#include <android-base/properties.h>
#include <log/log.h>
#include <utils/SystemClock.h>
#include <algorithm>

#include <vhal_v2_0/VehicleUtils.h>

#include "PipeComm.h"
#include "ProtoMessageConverter.h"
#include "SocketComm.h"
#include "body_can_ccm_4_0.h"
#include "chassise_can_ccm_4_0.h"
#include "ev_can_ccm_4_0.h"
#include "mcu.h"
#include "SerialCommunication.h"
#include "CanMap.h"
#include "HandlerCanData.h"
#include "VehicleEmulator.h"
#include "PropertyUtils.h"
#include <random>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include "me11_cmx_baccanfd_ots_car_v02.h"
namespace android {
namespace hardware {
namespace automotive {
namespace vehicle {
namespace V2_0 {

namespace impl {

bool is_valid_message(std::vector<uint8_t> message)
{
    if (message.size() < 8) {
        return false;
    }
    int size  = static_cast<int> (message.size());
    for (int i = 0; i < size; i++) {
        if (message[i] != 0xff) {
            return true;
        }
    }

    return false;
}

VehicleEmulator::VehicleEmulator(EmulatedVehicleHalIface* hal, PropertyManager* pm) : mHal{hal}, mPropManager{pm} {
    mHal->registerEmulator(this);

      //  mPropManager.importPropertiesFromFile("/etc/properties.txt",mPropManager);
      ALOGI("Starting SocketComm");
//    mSocketComm = std::make_unique<SocketComm>(this);
//    mSocketComm->start();

//    mGarageModeHandler = makeGarageModeServerSideHandler(&mValueObjectPool, &mPropManager, mHal);
//    if (isInEmulator()) {
        ALOGI("Starting PipeComm");
        mPipeComm = new PipeComm(this);
        mPipeComm->setPropertyManager(pm);
        mPipeComm->start();
//    }

      //lihy get uds server
      uds_binder = ::ndk::SpAIBinder(AServiceManager_getService("android.hardware.gecko_uds.IGeckoUds/default"));
      if (uds_binder.get() == nullptr) {
          uds_success = false;
          ALOGE("Failed to get GeckoUds daemon");
      }
      else {
          udsserver = IGeckoUds::fromBinder(uds_binder);
          if (udsserver == nullptr) {
              uds_success = false;
              ALOGE("Failed to connect to GeckoUds daemon");
          }
          else {
              ALOGE("connect to GeckoUds daemon successfully");
			  uds_success = true;
              msg.addr_flag = 0;
              msg.payload = {0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
              msg.timestamp = 0;
              Result rst = Result::OK;
              ndk::ScopedAStatus Status;
              Status = udsserver->resend(msg, &rst);
          }
      }

      HandlerCanData::getInstance()->initFunctionMap(&functionMap);
      HandlerCanData::getInstance()->initPropManager(mPropManager);

      // Start CAN timeout checking thread
      std::thread([]() {
          constexpr std::chrono::milliseconds checkInterval(100); // check every 100ms
          constexpr std::chrono::milliseconds timeoutMs(500); // timeout threshold
          while (true) {
              CanMap::getInstance()->checkTimeouts(
                  [](uint16_t messageID, BusIdentifier busIdentifier) {
                      HandlerCanData::getInstance()->onCanTimeout(messageID, busIdentifier);
                  },
                  timeoutMs
              );
              std::this_thread::sleep_for(checkInterval);
          }
      }).detach();

      turnSigEventHandleThread();
}

VehicleEmulator::~VehicleEmulator() {
    mSocketComm->stop();
    if (mPipeComm) {
        mPipeComm->stop();
    }
}

PipeComm* VehicleEmulator::getComm(){
	return mPipeComm;
}

float VehicleEmulator::getActualFloatValue(int32_t raw_value, float factor, int offset) {
     return static_cast<float>(raw_value * factor + offset);
}

/**
 * This is called by the HAL when a property changes. We need to notify our clients that it has
 * changed.
 */
void VehicleEmulator::doSetValueFromClient(const VehiclePropValue& propValue) {
//    ALOGW("dxy---- After the Application changes the value, it will receive a message here. You can send command to the MCU  ");

//    打印propValue的值
//    ALOGI("dxy----Property: prop=%d, areaId=%d, status=%d",
//          propValue.prop, propValue.areaId, static_cast<int32_t>(propValue.status));
    //  ALOGI("Timestamp: %lld", propValue.timestamp);
//     int32_t int32Value;
//     float floatValue;
       int32_t propId = propValue.prop;
       if(1 < 0 )
            ALOGI("dxy----propId= %d", propId);
//     if (propValue.value.int32Values.size() > 0) {
//         for (int32_t value : propValue.value.int32Values) {
//             ALOGI("dxy----dxyint32value------%d", value);
//             int32Value = value;
//         }
//     }

//     if (propValue.value.floatValues.size() > 0) {
// //        ALOGI("FloatValues:");
//         for (float value : propValue.value.floatValues) {
//             ALOGI("dxy----dxyfloatvalue------%f", value);
//             floatValue = value;
//         }
//     }
//     //空调
//     ALOGI("dxy----propId= %d", propId);
//     uint16_t messageID;
//     BusIdentifier busIdentifier;
//     uint8_t command = 0x02;
//     uint8_t responseFlag;
//     switch(propId){
//         case toInt(VehicleProperty::HVAC_TEMPERATURE_SET): {//358614275: {
//                 struct body_can_ccm_4_0_ccm_310_t ac;
//                 memset(&ac, 0, sizeof(body_can_ccm_4_0_ccm_310_t));
//                 ac.ccm_left_set_temp = static_cast<int32_t> (propValue.value.floatValues[0]);
//                 messageID = 0x310;
//                 busIdentifier = BODY_CAN;
//                 responseFlag = 0x00;
//                 std::vector<uint8_t> message = makePack(messageID, static_cast<void*>(&ac), busIdentifier, command, responseFlag);
//                 std::string buf;
//                 for(int i = 0; i < static_cast<int> (message.size()); i++){
//                     std::ostringstream ss;
//                     ss.clear();
//                     int data = message[i];
//                     ss << std::hex << data;
//                     buf += "0x";
//                     buf += ss.str();
//                     buf += ", ";
//                 }
//                 ALOGI("dxy buffer:%s", buf.c_str());
//                 ALOGI("dxy----ccm_left_set_temp makePack done %d", message[0]);
// 		SerialCommunication::getInstance()->sendData(message);
//                 ALOGI("dxy----ccm_left_set_temp makePack done");
//             }
//             break;
//         case toInt(VehicleProperty::HVAC_FAN_DIRECTION): { // 356517121: {
//                 struct body_can_ccm_4_0_ccm_310_t ac;
//                 memset(&ac, 0, sizeof(body_can_ccm_4_0_ccm_310_t));
//                 ac.ccm_air_distribution_mode_req = propValue.value.int32Values[0];
//                 messageID = 0x310;
//                 busIdentifier = BODY_CAN;
//                 responseFlag = 0x00;
//                 std::vector<uint8_t> message = makePack(messageID, static_cast<void*>(&ac), busIdentifier, command, responseFlag);
//                 SerialCommunication::getInstance()->sendData(message);
//                 ALOGI("dxy----ccm_air_distribution_mode_req makePack done");
//             }
//             break;
// 	case toInt(VehicleProperty::HVAC_AC_ON): {//354419973: {
//                 struct body_can_ccm_4_0_ccm_310_t ac;
//                 memset(&ac, 0, sizeof(body_can_ccm_4_0_ccm_310_t));
//                 if(propValue.value.int32Values[0]) {
//                     ac.ccm_a_ckey = 2;
//                 } else {
//                     ac.ccm_a_ckey = 1;
//                 }
//                 messageID = 0x310;
//                 busIdentifier = BODY_CAN;
//                 responseFlag = 0x00;
//                 std::vector<uint8_t> message = makePack(messageID, static_cast<void*>(&ac), busIdentifier, command, responseFlag);
//                 SerialCommunication::getInstance()->sendData(message);
//                 ALOGI("dxy----ccm_a_ckey makePack done");
//             }
//             break;
// 	case toInt(VehicleProperty::HVAC_POWER_ON): {// 354419984: {
//                 struct body_can_ccm_4_0_ccm_310_t ac;
//                 memset(&ac, 0, sizeof(body_can_ccm_4_0_ccm_310_t));
//                 if(propValue.value.int32Values[0]) {
//                     ac.ccm_ac_havc_state = 2;
//                 } else {
//                     ac.ccm_ac_havc_state = 1;
//                 }
//                 messageID = 0x310;
//                 busIdentifier = BODY_CAN;
//                 responseFlag = 0x00;
//                 std::vector<uint8_t> message = makePack(messageID, static_cast<void*>(&ac), busIdentifier, command, responseFlag);
//                 SerialCommunication::getInstance()->sendData(message);
//                 ALOGI("dxy----ccm_ac_havc_state makePack done");
//             }
//             break;
// 	case toInt(VehicleProperty::HVAC_DEFROSTER): {//320865540: {
//                 struct body_can_ccm_4_0_ccm_310_t ac;
//                 memset(&ac, 0, sizeof(body_can_ccm_4_0_ccm_310_t));
//                 if (propValue.value.int32Values[0]) {
//                     ac.ccm_front_defrost_key = 2;
//                 } else {
//                     ac.ccm_front_defrost_key = 1;
//                 }
//                 messageID = 0x310;
//                 busIdentifier = BODY_CAN;
//                 responseFlag = 0x00;
//                 std::vector<uint8_t> message = makePack(messageID, static_cast<void*>(&ac), busIdentifier, command, responseFlag);
//                 SerialCommunication::getInstance()->sendData(message);
//                 ALOGI("dxy----ccm_front_defrost_key makePack done");
//             }
//             break;
// 	case toInt(VehicleProperty::HVAC_RECIRC_ON): {//354419976: {
//                 struct body_can_ccm_4_0_ccm_310_t ac;
//                 memset(&ac, 0, sizeof(body_can_ccm_4_0_ccm_310_t));
//                 if (propValue.value.int32Values[0]) {
//                     ac.ccm_fresh_reci_key = 1;
//                 } else {
//                     ac.ccm_fresh_reci_key = 2;
//                 }
//                 messageID = 0x310;
//                 busIdentifier = BODY_CAN;
//                 responseFlag = 0x00;
//                 std::vector<uint8_t> message = makePack(messageID, static_cast<void*>(&ac), busIdentifier, command, responseFlag);
//                 SerialCommunication::getInstance()->sendData(message);
//                 ALOGI("dxy----ccm_fresh_reci_key makePack done");
//             }
//             break;
// 	case toInt(VehicleProperty::HVAC_AUTO_RECIRC_ON): {//354419986: {
//                 struct body_can_ccm_4_0_ccm_310_t ac;
//                 memset(&ac, 0, sizeof(body_can_ccm_4_0_ccm_310_t));
//                 if (propValue.value.int32Values[0]) {
//                     ac.ccm_fresh_reci_key = 2;
//                 } else {
//                     ac.ccm_fresh_reci_key = 1;
//                 }
//                 messageID = 0x310;
//                 busIdentifier = BODY_CAN;
//                 responseFlag = 0x00;
//                 std::vector<uint8_t> message = makePack(messageID, static_cast<void*>(&ac), busIdentifier, command, responseFlag);
//                 SerialCommunication::getInstance()->sendData(message);
//                 ALOGI("dxy----ccm_fresh_reci_key makePack done");
//             }
//             break;
// 	case toInt(VehicleProperty::HVAC_FAN_SPEED): {//356517120: {
//                 struct body_can_ccm_4_0_ccm_310_t ac;
//                 memset(&ac, 0, sizeof(body_can_ccm_4_0_ccm_310_t));
//                 ac.ccm_blower_level = propValue.value.int32Values[0];
//                 messageID = 0x310;
//                 busIdentifier = BODY_CAN;
//                 responseFlag = 0x00;
//                 std::vector<uint8_t> message = makePack(messageID, static_cast<void*>(&ac), busIdentifier, command, responseFlag);
//                 SerialCommunication::getInstance()->sendData(message);
//                 ALOGI("dxy----ccm_blower_level makePack done");
//             }
//             break;
// 	case toInt(VehicleProperty::CURRENT_GEAR): { //289408001: {
//                 struct ev_can_ccm_4_0_gw_vcu_0_x350_t gear;
//                 memset(&gear, 0, sizeof(ev_can_ccm_4_0_gw_vcu_0_x350_t));
//                 gear.vcu_actual_gear = propValue.value.int32Values[0];
//                 messageID = 0x350;
//                 busIdentifier = EV_CAN;
//                 responseFlag = 0x00;
//                 std::vector<uint8_t> message = makePack(messageID, static_cast<void*>(&gear), busIdentifier, command, responseFlag);
// 		SerialCommunication::getInstance()->sendData(message);
//                 ALOGI("dxy----vcu_actual_gear makePack done");
//             }
//             break;
// 	case toInt(VehicleProperty::HAZARD_LIGHTS_SWITCH): { //289410579: {
//                 struct body_can_ccm_4_0_gw_bcm_330_t lightsop;
//                 memset(&lightsop, 0, sizeof(body_can_ccm_4_0_gw_bcm_330_t));
//                 lightsop.bcm_hazard_light_status = propValue.value.int32Values[0];
//                 messageID = 0x330;
//                 busIdentifier = BODY_CAN;
//                 responseFlag = 0x00;
//                 std::vector<uint8_t> message = makePack(messageID, static_cast<void*>(&lightsop), busIdentifier, command, responseFlag);
// 		SerialCommunication::getInstance()->sendData(message);
//                 ALOGI("dxy----bcm_hazard_light_status makePack done");
//             }
//             break;
//         case toInt(VehicleProperty::HEADLIGHTS_SWITCH): { //289410576
//                 struct body_can_ccm_4_0_gw_bcm_330_t lightsop;
//                 memset(&lightsop, 0, sizeof(body_can_ccm_4_0_gw_bcm_330_t));
//                 lightsop.bcm_low_beam_status = propValue.value.int32Values[0];
//                 messageID = 0x330;
//                 busIdentifier = BODY_CAN;
//                 responseFlag = 0x00;
//                 std::vector<uint8_t> message = makePack(messageID, static_cast<void*>(&lightsop), busIdentifier, command, responseFlag);
// 		SerialCommunication::getInstance()->sendData(message);
//                 ALOGI("dxy----bcm_low_beam_status makePack done");
//             }
//             break;
//         case toInt(VehicleProperty::HIGH_BEAM_LIGHTS_SWITCH): {//289410577
//                 struct body_can_ccm_4_0_gw_bcm_330_t lightsop;
//                 memset(&lightsop, 0, sizeof(body_can_ccm_4_0_gw_bcm_330_t));
//                 lightsop.bcm_high_beam_status = propValue.value.int32Values[0];
//                 messageID = 0x330;
//                 busIdentifier = BODY_CAN;
//                 responseFlag = 0x00;
//                 std::vector<uint8_t> message = makePack(messageID, static_cast<void*>(&lightsop), busIdentifier, command, responseFlag);
//                 SerialCommunication::getInstance()->sendData(message);
//                 ALOGI("dxy----bcm_high_beam_status makePack done");
//             }
//             break;
//         case toInt(VehicleProperty::FOG_LIGHTS_SWITCH): { //289410578 ,
//                 struct body_can_ccm_4_0_gw_bcm_330_t lightsop;
//                 memset(&lightsop, 0, sizeof(body_can_ccm_4_0_gw_bcm_330_t));
//                 lightsop.bcm_rear_fog_light_status = propValue.value.int32Values[0];
//                 messageID = 0x330;
//                 busIdentifier = BODY_CAN;
//                 responseFlag = 0x00;
//                 std::vector<uint8_t> message = makePack(messageID, static_cast<void*>(&lightsop), busIdentifier, command, responseFlag);
//                 SerialCommunication::getInstance()->sendData(message);
//                 ALOGI("dxy----bcm_rear_fog_light_status makePack done");
//             }
//             break;
//         case toInt(VehicleProperty::TURN_SIGNAL_STATE): { //289408008: {
//                 struct body_can_ccm_4_0_gw_bcm_330_t lightsop;
//                 memset(&lightsop, 0, sizeof(body_can_ccm_4_0_gw_bcm_330_t));
//                 if (propValue.value.int32Values[0] == 1) {
//                     lightsop.bcm_turn_indcr_ri = 1;
//                     lightsop.bcm_turn_indcr_le = 0;
//                 } else if (propValue.value.int32Values[0] == 2) {
//                     lightsop.bcm_turn_indcr_ri = 0;
//                     lightsop.bcm_turn_indcr_le = 1;
//                 } else {
//                     lightsop.bcm_turn_indcr_ri = 0;
//                     lightsop.bcm_turn_indcr_le = 0;
//                 }
//                 messageID = 0x330;
//                 busIdentifier = BODY_CAN;
//                 responseFlag = 0x00;
//                 std::vector<uint8_t> message = makePack(messageID, static_cast<void*>(&lightsop), busIdentifier, command, responseFlag);
// 		SerialCommunication::getInstance()->sendData(message);
//                 ALOGI("dxy----bcm_turn_indcr_ri bcm_turn_indcr_le makePack done");
//             }
//             break;
//         case toInt(VehicleProperty::READING_LIGHTS_SWITCH): { //356519684: {
//                 struct mcu_info_001_t lightsop;
//                 memset(&lightsop, 0, sizeof(mcu_info_001_t));
//                 lightsop.lamplet_status = propValue.value.int32Values[0];
//                 messageID = 0x001;
//                 busIdentifier = MCU;
//                 responseFlag = 0x00;
//                 std::vector<uint8_t> message = makePack(messageID, static_cast<void*>(&lightsop), busIdentifier, command, responseFlag);
//                 SerialCommunication::getInstance()->sendData(message);
//                 ALOGI("dxy----lamplet_status makePack done");
//             }
//             break;
//         default:
//             break;
//     }

//     vhal_proto::EmulatorMessage msg;
//     vhal_proto::VehiclePropValue* val = msg.add_value();
//     populateProtoVehiclePropValue(val, &propValue);
//     msg.set_status(vhal_proto::RESULT_OK);
//     msg.set_msg_type(vhal_proto::SET_PROPERTY_ASYNC);

//     mSocketComm->sendMessage(msg);
//     if (mPipeComm) {
//         mPipeComm->sendMessage(msg);
//     }
}

void VehicleEmulator::doGetConfig(VehicleEmulator::EmulatorMessage const& rxMsg,
                                  VehicleEmulator::EmulatorMessage& respMsg) {
    std::vector<VehiclePropConfig> configs = mHal->listProperties();
    vhal_proto::VehiclePropGet getProp = rxMsg.prop(0);

    respMsg.set_msg_type(vhal_proto::GET_CONFIG_RESP);
    respMsg.set_status(vhal_proto::ERROR_INVALID_PROPERTY);

    for (auto& config : configs) {
        // Find the config we are looking for
        if (config.prop == getProp.prop()) {
            vhal_proto::VehiclePropConfig* protoCfg = respMsg.add_config();
            populateProtoVehicleConfig(protoCfg, config);
            respMsg.set_status(vhal_proto::RESULT_OK);
            break;
        }
    }
}

void VehicleEmulator::doGetConfigAll(VehicleEmulator::EmulatorMessage const& /* rxMsg */,
                                     VehicleEmulator::EmulatorMessage& respMsg) {
    std::vector<VehiclePropConfig> configs = mHal->listProperties();

    respMsg.set_msg_type(vhal_proto::GET_CONFIG_ALL_RESP);
    respMsg.set_status(vhal_proto::RESULT_OK);

    for (auto& config : configs) {
        vhal_proto::VehiclePropConfig* protoCfg = respMsg.add_config();
        populateProtoVehicleConfig(protoCfg, config);
    }
}

void VehicleEmulator::doGetProperty(VehicleEmulator::EmulatorMessage const& rxMsg,
                                    VehicleEmulator::EmulatorMessage& respMsg) {
    int32_t areaId = 0;
    vhal_proto::VehiclePropGet getProp = rxMsg.prop(0);
    int32_t propId = getProp.prop();
    vhal_proto::Status status = vhal_proto::ERROR_INVALID_PROPERTY;

    respMsg.set_msg_type(vhal_proto::GET_PROPERTY_RESP);

    if (getProp.has_area_id()) {
        areaId = getProp.area_id();
    }

    {
        VehiclePropValue request = {
                .areaId = areaId,
                .prop = propId,
        };
        StatusCode halStatus;
        auto val = mHal->get(request, &halStatus);
        if (val != nullptr) {
            vhal_proto::VehiclePropValue* protoVal = respMsg.add_value();
            populateProtoVehiclePropValue(protoVal, val.get());
            status = vhal_proto::RESULT_OK;
        }
    }

    respMsg.set_status(status);
}

void VehicleEmulator::doGetPropertyAll(VehicleEmulator::EmulatorMessage const& /* rxMsg */,
                                       VehicleEmulator::EmulatorMessage& respMsg) {
    respMsg.set_msg_type(vhal_proto::GET_PROPERTY_ALL_RESP);
    respMsg.set_status(vhal_proto::RESULT_OK);

    {
        for (const auto& prop : mHal->getAllProperties()) {
            vhal_proto::VehiclePropValue* protoVal = respMsg.add_value();
            populateProtoVehiclePropValue(protoVal, &prop);
        }
    }
}

void VehicleEmulator::doParseDataToPropertyStore(int type, void *data){
      ALOGW("dxy----doSetProperty%d,%p ",type,data);
}

void VehicleEmulator::doSetProperty() {

    ALOGW("dxy----doSetProperty ");
    // 使用随机设备作为种子
    std::random_device rd;
    std::mt19937 gen(rd());
    // 定义浮点数范围
    float min_value = 16.0;
    float max_value = 28.0;
    // 定义浮点数分布
    std::uniform_real_distribution<float> distribution(min_value, max_value);
    // 生成随机浮点数
    float random_left_float = distribution(gen);
    VehiclePropValue left_values = {
            .timestamp = elapsedRealtimeNano(),
            .areaId = 49,
            .prop = 358614275,
            .status = VehiclePropertyStatus::AVAILABLE,
            //.value.floatValues = [19.000000],
            .value.floatValues = {random_left_float},
    };
// 68    
    bool halRes = mHal->setPropertyFromVehicle(left_values);
    float random_right_float = distribution(gen);
    VehiclePropValue right_values = {
            .timestamp = elapsedRealtimeNano(),
            .areaId = 68,
            .prop = 358614275,
            .status = VehiclePropertyStatus::AVAILABLE,
            //.value.floatValues = [19.000000],
            .value.floatValues = {random_right_float},
    };
    mHal->setPropertyFromVehicle(right_values);
        VehiclePropValue fanSpeed = {
            .timestamp = elapsedRealtimeNano(),
            .areaId = 117,
            .prop = 356517120,
            .status = VehiclePropertyStatus::AVAILABLE,
            //.value.floatValues = [19.000000],
            .value.int32Values = {2},
    };
    mHal->setPropertyFromVehicle(fanSpeed);

    // 空调自动模式    VehicleProperty::HVAC_AUTO_ON
    VehiclePropValue hvac_auto_mode = {
            .timestamp = elapsedRealtimeNano(),
//            .areaId = HVAC_ALL,
            .areaId = 117,
            .prop = (int)VehicleProperty::HVAC_AUTO_ON,
            .status = VehiclePropertyStatus::AVAILABLE,
            //.value.floatValues = [19.000000],
            .value.int32Values = {3},
    };

    mHal->setPropertyFromVehicle(hvac_auto_mode);

    ALOGW("dxy----set hac property result  = %d ",halRes);
    }
// {.timestamp = 2116231334387, .areaId = 117, .prop = 356517120, .status = AVAILABLE, .value = {.int32Values = [1]{3}, 
//.floatValues = [0]{}, .int64Values = [0]{}, .bytes = [0]{}, .stringValue = ""}}

void VehicleEmulator::doSetProperty(VehicleEmulator::EmulatorMessage const& rxMsg,
                                    VehicleEmulator::EmulatorMessage& respMsg) {
    vhal_proto::VehiclePropValue protoVal = rxMsg.value(0);
    VehiclePropValue val = {
            .timestamp = elapsedRealtimeNano(),
            .areaId = protoVal.area_id(),
            .prop = protoVal.prop(),
            .status = (VehiclePropertyStatus)protoVal.status(),
    };

    respMsg.set_msg_type(vhal_proto::SET_PROPERTY_RESP);

    // Copy value data if it is set.  This automatically handles complex data types if needed.
    if (protoVal.has_string_value()) {
        val.value.stringValue = protoVal.string_value().c_str();
    }

    if (protoVal.has_bytes_value()) {
        val.value.bytes = std::vector<uint8_t> { protoVal.bytes_value().begin(),
                                                 protoVal.bytes_value().end() };
    }

    if (protoVal.int32_values_size() > 0) {
        val.value.int32Values = std::vector<int32_t> { protoVal.int32_values().begin(),
                                                       protoVal.int32_values().end() };
    }

    if (protoVal.int64_values_size() > 0) {
        val.value.int64Values = std::vector<int64_t> { protoVal.int64_values().begin(),
                                                       protoVal.int64_values().end() };
    }

    if (protoVal.float_values_size() > 0) {
        val.value.floatValues = std::vector<float> { protoVal.float_values().begin(),
                                                     protoVal.float_values().end() };
    }

    bool halRes = mHal->setPropertyFromVehicle(val);
    respMsg.set_status(halRes ? vhal_proto::RESULT_OK : vhal_proto::ERROR_INVALID_PROPERTY);
}

void VehicleEmulator::processMessage(vhal_proto::EmulatorMessage const& rxMsg,
                                     vhal_proto::EmulatorMessage& respMsg) {
    switch (rxMsg.msg_type()) {
        case vhal_proto::GET_CONFIG_CMD:
            doGetConfig(rxMsg, respMsg);
            break;
        case vhal_proto::GET_CONFIG_ALL_CMD:
            doGetConfigAll(rxMsg, respMsg);
            break;
        case vhal_proto::GET_PROPERTY_CMD:
            doGetProperty(rxMsg, respMsg);
            break;
        case vhal_proto::GET_PROPERTY_ALL_CMD:
            doGetPropertyAll(rxMsg, respMsg);
            break;
        case vhal_proto::SET_PROPERTY_CMD:
            doSetProperty(rxMsg, respMsg);
            break;
        default:
            ALOGW("%s: Unknown message received, type = %d", __func__, rxMsg.msg_type());
            respMsg.set_status(vhal_proto::ERROR_UNIMPLEMENTED_CMD);
            break;
    }
}
int VehicleEmulator::checkpack(const std::vector<uint8_t>& message) {
    std::vector<uint8_t> data;
    uint8_t command, responseFlag;
    return mprase.parseDataPacket(message, data, command, responseFlag);
}


// static int32_t curTurnIndcrLe = 0;
// static int32_t curTurnIndcrRi = 0;
// static bool curTurnIndcrHa = false;
// void VehicleEmulator::updateTurnLightStatus(std::vector<uint8_t> message){
//     VehiclePropValue LeftTurnIndrOFF = {
//             .timestamp = elapsedRealtimeNano(),
//             .areaId = 0,
//             .prop = 557850895,
//             .status = VehiclePropertyStatus::AVAILABLE,
//             .value.int32Values = {0},
//     };
//     VehiclePropValue RightTurnIndrOFF = {
//             .timestamp = elapsedRealtimeNano(),
//             .areaId = 0,
//             .prop = 557850894,
//             .status = VehiclePropertyStatus::AVAILABLE,
//             .value.int32Values = {0},
//     };
//     VehiclePropValue HazardTurnIndrOFF = {
//             .timestamp = elapsedRealtimeNano(),
//             .areaId = 0,
//             .prop = 289410563,
//             .status = VehiclePropertyStatus::AVAILABLE,
//             .value.int32Values = {0},
//     };
//     if(!is_valid_message(message)) {
//         curTurnIndcrHa = false;
//         curTurnIndcrLe = 0;
//         curTurnIndcrRi = 0;
//         mHal->onPropertyValue(RightTurnIndrOFF, true);
//         mHal->onPropertyValue(LeftTurnIndrOFF, true);
//         mHal->onPropertyValue(HazardTurnIndrOFF, true);
//         return;
//     }
//     struct me11_cmx_baccanfd_ots_car_v02_07_20240520_bcm_b_package_bac_t dst_p;
//     std::string value;
//     std::string turnsingal_status_int;
//         me11_cmx_baccanfd_ots_car_v02_07_20240520_bcm_b_package_bac_unpack(&dst_p,message.data(),message.size());
//     VehiclePropValue LeftTurnIndr = {
//             .timestamp = elapsedRealtimeNano(),
//             .areaId = 0,
//             .prop = 557850895,
//             .status = VehiclePropertyStatus::AVAILABLE,
//             .value.int32Values = {dst_p.bcm_turn_lamp_sta_left},
//     };
//     VehiclePropValue RightTurnIndr = {
//             .timestamp = elapsedRealtimeNano(),
//             .areaId = 0,
//             .prop = 557850894,
//             .status = VehiclePropertyStatus::AVAILABLE,
//             .value.int32Values = {dst_p.bcm_turn_lamp_sta_right},
//     };
//     VehiclePropValue HazardTurnIndr = {
//             .timestamp = elapsedRealtimeNano(),
//             .areaId = 0,
//             .prop = 289410563,
//             .status = VehiclePropertyStatus::AVAILABLE,
//             .value.int32Values = {(dst_p.bcm_turn_lamp_sta_left == 1) & (dst_p.bcm_turn_lamp_sta_right == 1)},
//     };
//     bool isHazardStatus = (dst_p.bcm_turn_lamp_sta_left == 1) & (dst_p.bcm_turn_lamp_sta_right == 1);
//     if(isHazardStatus) {
//         if(curTurnIndcrHa != isHazardStatus) {
//             ALOGE("turn_indcr_ha status change from %d to %d", curTurnIndcrHa, isHazardStatus);
//             curTurnIndcrHa = isHazardStatus;
//             mHal->onPropertyValue(HazardTurnIndr, true);
//         }
//     } else {
//         if(curTurnIndcrHa != isHazardStatus) {
//             ALOGE("turn_indcr_ha status change from %d to %d", curTurnIndcrHa, isHazardStatus);
//             curTurnIndcrHa = isHazardStatus;
//             mHal->onPropertyValue(HazardTurnIndr, true);
//         } else {
//             if(curTurnIndcrRi != dst_p.bcm_turn_lamp_sta_right) {
//                 ALOGE("turn_indcr_ri status change from %d to %d", curTurnIndcrRi, dst_p.bcm_turn_lamp_sta_right);
//                 curTurnIndcrRi = dst_p.bcm_turn_lamp_sta_right;
//                 mHal->onPropertyValue(RightTurnIndr, true);
//             }
//             if(curTurnIndcrLe != dst_p.bcm_turn_lamp_sta_left) {
//                 ALOGE("turn_indcr_le status change from %d to %d", curTurnIndcrLe, dst_p.bcm_turn_lamp_sta_left);
//                 curTurnIndcrLe = dst_p.bcm_turn_lamp_sta_left;
//                 mHal->onPropertyValue(LeftTurnIndr, true);
//             }
//         }
//     }
// }

#define TAG "turn_sig_sync"

void VehicleEmulator::turnSigEventHandleThread(){
    std::thread([this](){
        int fd =  -1;
        // 打开输入设备
        while(true){
            const char* devPath = "/sys/class/gpio_turn_sig/gpio_turn_sig/turnsig_value"; 
            fd = open(devPath, O_RDONLY);
            if (fd < 0) {
                ALOGE("%s: Failed to open input device %s: %s", TAG, devPath, strerror(errno));
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            }else{
                ALOGI("%s: open input device %s succeed", TAG, devPath);
                break;
            }
        }

        char databuf[8] = {0};
        int32_t value = 0;
        // 循环读取输入事件
        int32_t preValue = 0;
        while (true) {
            std::this_thread::sleep_for(std::chrono::milliseconds(30));
            lseek(fd, 0, SEEK_SET);
            ssize_t n = read(fd, databuf, sizeof(databuf));
            if (n < 0) {
                ALOGE("%s: read data size error n:%ld", TAG, n);
                continue;
            }
            value = atoi(databuf);

            if(preValue != value){
                ALOGI("%s: preValue:%d, value:%d", TAG, preValue, value);
                preValue = value;

                if(value == TURN_SIG_LEFT_OFF_RIGHT_OFF){
                    //左右都灭
                    this->updateTurnLightStatus(0, 0);
                }else if(value == TURN_SIG_LEFT_ON_RIGHT_OFF){
                    //左亮右灭
                    this->updateTurnLightStatus(1, 0);
                }else if(value == TURN_SIG_LEFT_OFF_RIGHT_ON){
                    //左灭右亮
                    this->updateTurnLightStatus(0, 1);
                }else if(value == TURN_SIG_LEFT_ON_RIGHT_ON){
                    //左右都亮
                    this->updateTurnLightStatus(1, 1);
                }else{
                    ALOGW("%s: data error vlaue:%d line:%d", TAG, value, __LINE__);
                }
            }
        }
        close(fd);
    }).detach();
}

static int32_t curTurnIndcrLe = 0;
static int32_t curTurnIndcrRi = 0;
static bool curTurnIndcrHa = false;
void VehicleEmulator::updateTurnLightStatus(int32_t leftLampStatus, int32_t rightLampStatus){
    VehiclePropValue LeftTurnIndr = {
            .timestamp = elapsedRealtimeNano(),
            .areaId = 0,
            .prop = 557850895,
            .status = VehiclePropertyStatus::AVAILABLE,
            .value.int32Values = {leftLampStatus},
    };
    VehiclePropValue RightTurnIndr = {
            .timestamp = elapsedRealtimeNano(),
            .areaId = 0,
            .prop = 557850894,
            .status = VehiclePropertyStatus::AVAILABLE,
            .value.int32Values =  {rightLampStatus},
    };
    VehiclePropValue HazardTurnIndr = {
            .timestamp = elapsedRealtimeNano(),
            .areaId = 0,
            .prop = 289410563,
            .status = VehiclePropertyStatus::AVAILABLE,
            .value.int32Values = {(leftLampStatus == 1) & (rightLampStatus == 1)},
    };

    bool isHazardStatus = (leftLampStatus == 1) & (rightLampStatus == 1);
    if(isHazardStatus) {
        if(curTurnIndcrHa != isHazardStatus) {
            ALOGI("%s, turn_indcr_ha status change from %d to %d, line:%d", TAG, curTurnIndcrHa, isHazardStatus, __LINE__);
            curTurnIndcrHa = isHazardStatus;
            mHal->onPropertyValue(HazardTurnIndr, true);
        }
    } else {
        if(curTurnIndcrHa != isHazardStatus) {
            ALOGI("%s turn_indcr_ha status change from %d to %d, line:%d", TAG, curTurnIndcrHa, isHazardStatus, __LINE__);
            curTurnIndcrHa = isHazardStatus;
            mHal->onPropertyValue(HazardTurnIndr, true);
        } else {
            if(curTurnIndcrRi != rightLampStatus) {
                ALOGI("%s, turn_indcr_ri status change from %d to %d, line:%d", TAG, curTurnIndcrRi, rightLampStatus, __LINE__);
                curTurnIndcrRi = rightLampStatus;
                mHal->onPropertyValue(RightTurnIndr, true);
            }
            if(curTurnIndcrLe != leftLampStatus) {
                ALOGI("%s, turn_indcr_le status change from %d to %d, line:%d", TAG, curTurnIndcrLe, leftLampStatus, __LINE__);
                curTurnIndcrLe = leftLampStatus;
                mHal->onPropertyValue(LeftTurnIndr, true);
            }
        }
    }
}

void VehicleEmulator::processCanReadMessage1(uint16_t messageID, std::vector<uint8_t>& dataContent, uint8_t busIdentifier)
{
    BusIdentifier busId;
    //std::vector<uint8_t> messageIdentifier;
    busId = static_cast<BusIdentifier>(busIdentifier);
//    messageIdentifier = {message[1], message[2]};
    //ALOGE("zhugang %s %d %02x %02x",__func__,__LINE__,message[1],message[2]);
//     uint16_t messageID = 0;
//     messageID = (message[1] << 8) | message[2];
// //    ALOGE("zhugang %d %d",messageIdentifier[1],messageIdentifier[0]);
//     std::vector<uint8_t> dataContent;
//     dataContent.assign(message.begin() + 3, message.end());
    //ALOGE("spi1028 VehicleEmulator %04x %d",messageID,busIdentifier);
    // if (messageID == 0x021A) {
        // updateTurnLightStatus(dataContent);
    // }
    CanMap::getInstance()->insert_data(messageID,dataContent, busId);
}

void VehicleEmulator::processCanReadMessage(const std::vector<uint8_t>& message)
{
    // 处理接收到的二进制信息
    std::vector<uint8_t> data;
    uint8_t command, responseFlag;
    BusIdentifier busIdentifier;
    std::vector<uint8_t> messageIdentifier;
    std::vector<uint8_t> dataContent;
    uint16_t messageID = 0;
    mprase.parseDataPacket(message, data, command, responseFlag);
    MessageType messageType = static_cast<MessageType>(command);
    ALOGI("hu0506 processCanReadMessage command:%x", command);
    switch(messageType){
        case MessageType::CAN_REPORT:
        {
            mprase.parseCANMessage(data,busIdentifier,messageIdentifier,dataContent);
            if (messageIdentifier.size() == 2) {
//                hexValue = (messageIdentifier[0] << 8) | messageIdentifier[1];
                messageID = (messageIdentifier[1] << 8) | messageIdentifier[0];
            }
            std::cout << " can id is: " << messageID << std::endl;
            ALOGI("hu0506 processCanReadMessage messageID:%x", messageID);
            //lihy
            if ((messageID == 0x700 || messageID == 0x7DF)) //phy addr or fun addr
            {
                //resend_uds_msg[8] = (messageID>>8) & 0xff;
                //resend_uds_msg[9] =  messageID & 0xff;
                //memcpy(resend_uds_msg, &data[0], 8);
                if (uds_binder.get() == nullptr) {
                    uds_binder = ::ndk::SpAIBinder(AServiceManager_getService("android.hardware.gecko_uds.IGeckoUds/default"));
                    if (uds_binder.get() == nullptr) {
                        ALOGE("Failed to get GeckoUds daemon, pls check GeckUds service");
                    }
                    else {
                        udsserver = IGeckoUds::fromBinder(uds_binder);
                        if (udsserver == nullptr) {
                            ALOGE("Failed to connect to GeckoUds daemon, pls check GeckUds service");
                        }
                        else {
                            uds_success = true;
                        }
                    }
                }
                if ( uds_success == true) {
                    msg.addr_flag = 0; //备用
                    msg.timestamp = 0; //备用
                    msg.payload[8] = (messageID>>8) & 0xff;
                    msg.payload[9] =  messageID & 0xff;
                    memcpy(&msg.payload[0], &dataContent[0], 8);
                    Result rst = Result::OK;
                    ndk::ScopedAStatus Status;
                    Status = udsserver->resend(msg, &rst);
                }
            }
            else {
            //end
                CanMap::getInstance()->insert_data(messageID,dataContent, busIdentifier);
                // char data[1024] = {0};
                // functionMap.unpackcallFunction(messageID,dataContent, static_cast<void *>(data), busIdentifier,*mPropManager);
//              doParseDataToPropertyStore(messageID,data);
            }
        }
            break;
        case MessageType::CAN_DOWNLINK:
            break;
        case MessageType::MCU_UPGRADE_DATA:
            break;
        case MessageType::MCU_UPGRADE_FINISH:
            break;
        default:
            break;
    }
    // 调用MessageProducer的方法示例
//    mp.sendMessage("Processed message");
}

std::vector<uint8_t> VehicleEmulator::makePack(uint16_t messageID, void* canData, BusIdentifier busIdentifier, uint8_t command, uint8_t responseFlag)
{
    //合成报文
    
    std::vector<uint8_t> data = functionMap.packcallFunction(messageID, static_cast<void*>(canData), busIdentifier);

    std::cout << "data:   ";
    for (size_t i = 0; i < data.size(); i++) {
        std::cout << std::hex << "0x" << static_cast<int>(data[i]) << " ";
    }
    std::cout << std::endl;

    //合成包头
    std::vector<uint8_t> packet;
    std::vector<uint8_t> canMessage;

    canMessage = mprase.constructCANMessage(busIdentifier, messageID, data);
    mprase.constructDataPacket(packet, canMessage, command, responseFlag);

    //std::vector<uint8_t> data = {0x03,0x01,0x02,0x00,0x0B,0x02,0x50,0x02,0x08,0x00,0x3A,0x98,0x00,0x00,0x00,0x00,0xf0};

    packet.push_back(0x0a);
    std::cout << "message:   ";
    for (size_t i = 0; i < packet.size(); i++) {
        std::cout << std::hex << static_cast<int>(packet[i]) << " ";
    }
    std::cout << std::endl;
    return packet;
}

void VehicleEmulator::processCanWriteMessage()
{
    // uint16_t messageID = 0;
    // std::vector<uint8_t> message;
    // BusIdentifier busIdentifier;
    // uint8_t command;
    // uint8_t responseFlag;

    // struct ev_can_ccm_4_0_ccm_0_x353_t total_odometer;
    // memset(&total_odometer, 0, sizeof(ev_can_ccm_4_0_ccm_0_x353_t));
    // total_odometer.total_odometer = 4000;
    // messageID = 0x353;

    // busIdentifier = EV_CAN;
    // command = 0x01;
    // responseFlag = 0x02;
    // message = makePack(messageID, static_cast<void*>(&total_odometer), busIdentifier, command, responseFlag);
    // SerialCommunication::getInstance()->sendData(message);
    // std::this_thread::sleep_for(std::chrono::milliseconds(500));


    // struct ev_can_ccm_4_0_gw_vcu_0_x350_t actual_gear;
    // memset(&actual_gear, 0, sizeof(ev_can_ccm_4_0_gw_vcu_0_x350_t));
    // actual_gear.vcu_actual_gear = 2;
    // messageID = 0x350;

    // busIdentifier = EV_CAN;
    // command = 0x01;
    // responseFlag = 0x02;
    // message = makePack(messageID, static_cast<void*>(&actual_gear), busIdentifier, command, responseFlag);
    // serialCommunication.sendData(message);
    // std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // struct ev_can_ccm_4_0_mcu_status_1_200_t engine_rpm_values;
    // memset(&engine_rpm_values, 0, sizeof(ev_can_ccm_4_0_mcu_status_1_200_t));
    // engine_rpm_values.mcu_motorspd = 2500;
    // messageID = 0x200;

    // busIdentifier = EV_CAN;
    // command = 0x01;
    // responseFlag = 0x02;
    // message = makePack(messageID, static_cast<void*>(&engine_rpm_values), busIdentifier, command, responseFlag);
    // serialCommunication.sendData(message);
    // std::this_thread::sleep_for(std::chrono::milliseconds(500));


    // struct ev_can_ccm_4_0_bms_0_x125_t batterycurrent;
    // memset(&batterycurrent, 0, sizeof(ev_can_ccm_4_0_bms_0_x125_t));
    // batterycurrent.bms_batterycurrent = 200;
    // messageID = 0x125;

    // busIdentifier = EV_CAN;
    // command = 0x01;
    // responseFlag = 0x02;
    // message = makePack(messageID, static_cast<void*>(&batterycurrent), busIdentifier, command, responseFlag);
    // serialCommunication.sendData(message);
    // std::this_thread::sleep_for(std::chrono::milliseconds(500));


    // struct ev_can_ccm_4_0_bms_0_x135_t batterysoc;
    // memset(&batterysoc, 0, sizeof(ev_can_ccm_4_0_bms_0_x135_t));
    // batterysoc.bms_batterysoc = 200;
    // messageID = 0x135;

    // busIdentifier = EV_CAN;
    // command = 0x01;
    // responseFlag = 0x02;
    // message = makePack(messageID, static_cast<void*>(&batterycurrent), busIdentifier, command, responseFlag);
    // serialCommunication.sendData(message);
    // std::this_thread::sleep_for(std::chrono::milliseconds(500));


    // struct body_can_ccm_4_0_gw_bcm_330_t values;
    // memset(&values, 0, sizeof(body_can_ccm_4_0_gw_bcm_330_t));
    // values.bcm_hazard_light_status = 0;
    // values.bcm_low_beam_status = 0;
    // values.bcm_high_beam_status = 0;
    // values.bcm_rear_fog_light_status = 1;
    // values.bcm_turn_indcr_ri = 0;
    // values.bcm_turn_indcr_le = 1;
    // values.bcm_brakelight_st = 1;
    // values.bcm_fl_door_ajar_status = 0;
    // values.bcm_fr_door_ajar_status = 1;
    // values.bcm_rr_door_ajar_status = 1;
    // messageID = 0x330;

    // busIdentifier = BODY_CAN;
    // command = 0x01;
    // responseFlag = 0x02;
    // message = makePack(messageID, static_cast<void*>(&values), busIdentifier, command, responseFlag);
    // //serialCommunication.sendData(message);
    // std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // struct body_can_ccm_4_0_tpms_332_t values;
    // memset(&values, 0, sizeof(body_can_ccm_4_0_tpms_332_t));
    // values.tpms_tyrepressure_fl = 200;
    // values.tpms_tyrepressure_fr = 250;
    // values.tpms_tyrepressure_rl = 250;
    // values.tpms_tyrepressure_rr = 200;
    // messageID = 0x332;

    // busIdentifier = BODY_CAN;
    // command = 0x01;
    // responseFlag = 0x02;
    // message = makePack(messageID, static_cast<void*>(&values), busIdentifier, command, responseFlag);
    // serialCommunication.sendData(message);
    // std::this_thread::sleep_for(std::chrono::milliseconds(500));
 
    //车速
    // struct ev_can_ccm_4_0_gw_vcu_250_t speed;
    // memset(&speed, 0, sizeof(ev_can_ccm_4_0_gw_vcu_250_t));
    // speed.vcu_vehicle_spd = 200;
    // messageID = 0x250;

    // busIdentifier = EV_CAN;
    // command = 0x01;
    // responseFlag = 0x02;
    // message = makePack(messageID, static_cast<void*>(&speed), busIdentifier, command, responseFlag);
    // serialCommunication.sendData(message);
    // std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // //空调
    // message.clear();
    // struct body_can_ccm_4_0_ccm_310_t ac;
    // memset(&ac, 0, sizeof(body_can_ccm_4_0_ccm_310_t));
    // ac.ccm_left_set_temp = 26;
    // messageID = 0x310;

    // busIdentifier = BODY_CAN;
    // command = 0x01;
    // responseFlag = 0x02;
    // message = makePack(messageID, static_cast<void*>(&ac), busIdentifier, command, responseFlag);
    // serialCommunication.sendData(message);
    // std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // message.clear();
    // struct ev_can_ccm_4_0_gw_ac_232_t ac232;
    // memset(&ac232, 0, sizeof(ev_can_ccm_4_0_gw_ac_232_t));
    // ac232.ac_left_set_temperature = 26;
    // ac232.ac_blower_level = 5;                //吹风档位
    // ac232.ac_fresh_reci_state = 1;            //内外循环进气
    // ac232.ac_front_defrost_state = 1;         //除霜工作状态
    // ac232.ac_heat_state =1;                  //PTC加热状态
    // ac232.ac_havc_state =1;                 //空调快速开闭(ON/OFF）
    // ac232.ac_ai_rdistribution_mode = 2;       //吹风模式
    // ac232.ac_a_cstate =1;                    //AC制冷状态
    // ac232.ac_left_set_temperature = 20;        //设定温度
    // messageID = 0x232;

    // busIdentifier = EV_CAN;
    // command = 0x01;
    // responseFlag = 0x02;
    // message = makePack(messageID, static_cast<void*>(&ac232), busIdentifier, command, responseFlag);
    // serialCommunication.sendData(message);
    // std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

void VehicleEmulator::populateProtoVehicleConfig(vhal_proto::VehiclePropConfig* protoCfg,
                                                 const VehiclePropConfig& cfg) {
    return proto_msg_converter::toProto(protoCfg, cfg);
}

void VehicleEmulator::populateProtoVehiclePropValue(vhal_proto::VehiclePropValue* protoVal,
                                                    const VehiclePropValue* val) {
    return proto_msg_converter::toProto(protoVal, *val);
}

void VehicleEmulator::createUdsSendBackMsg(const hidl_vec<uint8_t>& data) {
    mPipeComm->reback_uds_message(data);
}

bool isInEmulator() {
    return android::base::GetBoolProperty("ro.boot.qemu", false);
}
}  // impl

}  // namespace V2_0
}  // namespace vehicle
}  // namespace automotive
}  // namespace hardware
}  // namespace android
