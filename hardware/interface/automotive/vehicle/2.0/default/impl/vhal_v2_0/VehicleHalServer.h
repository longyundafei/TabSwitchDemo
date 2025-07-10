/*
 * Copyright (C) 2020 The Android Open Source Project
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

#pragma once

#include <vhal_v2_0/VehicleObjectPool.h>
#include <vhal_v2_0/VehiclePropertyStore.h>
#include <vhal_v2_0/VehicleServer.h>

#include "GeneratorHub.h"

#include "MessageParser.h"
#include "config_parser.h"
#define GECKO_ENGINEER_MODE_ENTER -1
#define GECKO_ENGINEER_MODE_EXIT 0
#define GECKO_ENGINEER_MODE_E2PROM_TEST 1
#define GECKO_ENGINEER_MODE_KEYCODE_TEST 2
#define GECKO_ENGINEER_MODE_QUERY_LOWBAT_VOLT 3
#define GECKO_ENGINEER_MODE_CURRENT_TEST 4
#define VENDOR_REQ_TAG      0x56524551
#define VENDOR_READ_IO      _IOW('v', 0x01, unsigned int)
#define VENDOR_WRITE_IO     _IOW('v', 0x02, unsigned int)
#define VENDOR_VEHICLE_FEATURE_ID             (16)
#define VENDOR_VEHICLE_VIN_ID                 (17)
#define VENDOR_ECU_SN_ID                      (18)
#define VENDOR_VEHICLE_TSP_ID                 (19)
#define VENDOR_VEHICLE_TYPE_ID                (20)
#define VENDOR_VEHICLE_OEM_DEV_ID             (21)
#define VENDOR_VEHICLE_HAL_FEATURE_ID         (100)

#define VEHICLE_FEATURE_MSG_LEN               (22)
#define VEHICLE_VIN_MSG_LEN                   (17)
#define VEHICLE_ECU_MSG_LEN                   (15)
#define VEHICLE_TSP_MSG_LEN                   (4)
#define VEHICLE_TYPE_MSG_LEN                  (2)
#define VEHICLE_OEM_MSG_LEN                   (16)
#define VEHICLE_HAL_FEATURE_LEN               (7)
struct gecko_vendor_req {
    uint32_t tag;
    uint16_t id;
    uint16_t len;
    uint8_t data[1024];
};

class MessageParser;

namespace android::hardware::automotive::vehicle::V2_0::impl {

// This contains the common server operations that will be used by
// both native and virtualized VHAL server. Notice that in the virtualized
// scenario, the server may be run on a different OS than Android.
class VehicleHalServer : public IVehicleServer {
  public:
    VehicleHalServer();

    void sendAllValuesToClient();

    // Methods from IVehicleServer

    std::vector<VehiclePropConfig> onGetAllPropertyConfig() const override;
    float getActualFloatValue(int32_t raw_value, float factor, int offset);
    StatusCode onSetProperty(const VehiclePropValue& value, bool updateStatus) override;
    int vehicle_vendor_storage_write(uint16_t id, uint16_t len, const std::vector<uint8_t>& write_value);
    int vehicle_vendor_storage_read(uint16_t id, uint16_t len, std::vector<uint8_t>& out_data);
    // Set the Property Value Pool used in this server
    void setValuePool(VehiclePropValuePool* valuePool);
    void setPropertyManager(PropertyManager* mp) {
        mPropertyManager = mp;
    }
    ConfigParser* GetPropConfig();
  private:
    using VehiclePropValuePtr = recyclable_ptr<VehiclePropValue>;

    GeneratorHub* getGenerator();

    VehiclePropValuePool* getValuePool() const;

    void onFakeValueGenerated(const VehiclePropValue& value);

    void setFMCommandToMCU(std::string fm_ctrl, std::string mode, std::string value);
    void setappointmentChragingToMCU(int soc_value, uint32_t week);
    StatusCode handleGenerateFakeDataRequest(const VehiclePropValue& request);
    void sendMultipleJson(std::string root, std::string child, std::string value);

    VehiclePropValuePtr createApPowerStateReq(VehicleApPowerStateReq req, int32_t param);

    VehiclePropValuePtr createHwInputKeyProp(VehicleHwKeyInputAction action, int32_t keyCode,
                                             int32_t targetDisplay);
    uint32_t getScheChargeWakedata();
  private:
    GeneratorHub mGeneratorHub{
            std::bind(&VehicleHalServer::onFakeValueGenerated, this, std::placeholders::_1)};

    VehiclePropValuePool* mValuePool{nullptr};
    VehiclePropertyStore mServerSidePropStore;
//    MessageParser mprase;
    PropertyManager* mPropertyManager;
    ConfigParser mConfigParser{"/vendor/etc/gecko-prop-init.json"};
};

}  // namespace android::hardware::automotive::vehicle::V2_0::impl
