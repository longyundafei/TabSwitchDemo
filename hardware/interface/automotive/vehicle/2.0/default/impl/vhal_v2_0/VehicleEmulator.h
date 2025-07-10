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

#ifndef android_hardware_automotive_vehicle_V2_0_impl_VehicleHalEmulator_H_
#define android_hardware_automotive_vehicle_V2_0_impl_VehicleHalEmulator_H_

#include <log/log.h>
#include <memory>
#include <thread>
#include <vector>

#include "vhal_v2_0/VehicleHal.h"
//#include "GarageModeServerSideHandler.h"
#include "MessageParser.h"
#include "FunctionMap.h"
#include "CommConn.h"
#include "PipeComm.h"
#include "SocketComm.h"
#include "VehicleHalProto.pb.h"
#include "property_manager.h"
#include <aidl/android/hardware/gecko_uds/IGeckoUds.h>
#include <android/binder_manager.h>

class FunctionMap;
class MessageParser;

using ::aidl::android::hardware::gecko_uds::IGeckoUds;
using ::aidl::android::hardware::gecko_uds::UdsMessage;
using ::aidl::android::hardware::gecko_uds::Result;

namespace android {
namespace hardware {
namespace automotive {
namespace vehicle {
namespace V2_0 {
namespace impl {

class VehicleEmulator;  // Forward declaration.

/** Extension of VehicleHal that used by VehicleEmulator. */
class EmulatedVehicleHalIface : public VehicleHal {
public:
    virtual bool setPropertyFromVehicle(const VehiclePropValue& propValue) = 0;
    virtual void onPropertyValue(const VehiclePropValue& value, bool updateStatus) = 0;
    virtual std::vector<VehiclePropValue> getAllProperties() const = 0;

    void registerEmulator(VehicleEmulator* emulator) {
        ALOGI("%s, emulator: %p", __func__, emulator);
        std::lock_guard<std::mutex> g(mEmulatorLock);
        mEmulator = emulator;
    }
    void ForTest() {
    }

protected:
    VehicleEmulator* getEmulatorOrDie() {
        std::lock_guard<std::mutex> g(mEmulatorLock);
        if (mEmulator == nullptr) abort();
        return mEmulator;
    }

private:
    mutable std::mutex mEmulatorLock;
    VehicleEmulator* mEmulator;
    
};

/**
 * Emulates vehicle by providing controlling interface from host side either through ADB or Pipe.
 */
class VehicleEmulator : public MessageProcessor {
   public:
    VehicleEmulator(EmulatedVehicleHalIface* hal, PropertyManager* pm);
    virtual ~VehicleEmulator();

	PipeComm* getComm();

    void doSetValueFromClient(const VehiclePropValue& propValue);
    void processMessage(vhal_proto::EmulatorMessage const& rxMsg,
                        vhal_proto::EmulatorMessage& respMsg) override;

    int checkpack(const std::vector<uint8_t>& message) override;
    void processCanReadMessage(const std::vector<uint8_t>& message) override;
    void processCanReadMessage1(uint16_t messageID, std::vector<uint8_t>& dataContent, uint8_t busIdentifier) override;
    void processCanWriteMessage() override;
    void createUdsSendBackMsg(const hidl_vec<uint8_t>& data) override;
    ndk::SpAIBinder uds_binder;
    std::shared_ptr<IGeckoUds> udsserver;
    UdsMessage  msg;
    bool uds_success;

   private:
    friend class ConnectionThread;
    using EmulatorMessage = vhal_proto::EmulatorMessage;

    void doGetConfig(EmulatorMessage const& rxMsg, EmulatorMessage& respMsg);
    void doGetConfigAll(EmulatorMessage const& rxMsg, EmulatorMessage& respMsg);
    void doGetProperty(EmulatorMessage const& rxMsg, EmulatorMessage& respMsg);
    void doGetPropertyAll(EmulatorMessage const& rxMsg, EmulatorMessage& respMsg);
    void doSetProperty(EmulatorMessage const& rxMsg, EmulatorMessage& respMsg);
    void doSetProperty();
    void doParseDataToPropertyStore(int type,void *data);
    void populateProtoVehicleConfig(vhal_proto::VehiclePropConfig* protoCfg,
                                    const VehiclePropConfig& cfg);
    void populateProtoVehiclePropValue(vhal_proto::VehiclePropValue* protoVal,
                                       const VehiclePropValue* val);
    float getActualFloatValue(int32_t raw_value, float factor, int offset);
    //void updateTurnLightStatus(std::vector<uint8_t> message);
    void updateTurnLightStatus(int32_t leftLampStatus, int32_t rightLampStatus);
    void turnSigEventHandleThread();
    std::vector<uint8_t> makePack(uint16_t messageID, void* canData, BusIdentifier busIdentifier, uint8_t command, uint8_t responseFlag);

private:
    EmulatedVehicleHalIface* mHal;

    std::unique_ptr<SocketComm> mSocketComm;
    PipeComm* mPipeComm;
    uint16_t my_vcu_vehicle_spd;
    MessageParser mprase;
    FunctionMap functionMap;
    PropertyManager* mPropManager;
//    VehiclePropValuePool mValueObjectPool;
//    std::unique_ptr<GarageModeServerSideHandler> mGarageModeHandler;
//    ObserverThreadPool mObjThreadPool;

   uint8_t resend_uds_msg[10] = {0};

enum class MessageType : uint8_t {
    CAN_REPORT = 0x01,         // CAN报文上报，上行
    CAN_DOWNLINK = 0x02,       // SOC下发CAN报文，下发
    MCU_UPGRADE_ENTER = 0x03,  // MCU升级进入，下行
    MCU_UPGRADE_DATA = 0x04,   // MCU升级数据包，下行
    MCU_UPGRADE_FINISH = 0x05  // MCU升级结束，下行
};

// 从驱动收到的转向灯事件类型
typedef enum {
    TURN_SIG_LEFT_OFF_RIGHT_OFF = 0,  //左右都灭
    TURN_SIG_LEFT_ON_RIGHT_OFF = 1,   //左亮右灭
    TURN_SIG_LEFT_OFF_RIGHT_ON = 2,   //左灭右亮
    TURN_SIG_LEFT_ON_RIGHT_ON = 3,    //左右都亮
}TurnSigType;
};

// determine if it's running inside Android Emulator
bool isInEmulator();

}  // impl

}  // namespace V2_0
}  // namespace vehicle
}  // namespace automotive
}  // namespace hardware
}  // namespace android

#endif // android_hardware_automotive_vehicle_V2_0_impl_VehicleHalEmulator_H_
