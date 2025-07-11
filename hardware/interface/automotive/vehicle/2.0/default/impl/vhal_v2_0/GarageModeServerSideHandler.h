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
#ifndef android_hardware_automotive_vehicle_V2_0_GarageModeServerSideHandler_H_
#define android_hardware_automotive_vehicle_V2_0_GarageModeServerSideHandler_H_
//#pragma once
#include <memory>
#include "vhal_v2_0/VehicleObjectPool.h"
#include "vhal_v2_0/VehicleServer.h"
#include "property_manager.h"
#include "VehicleEmulator.h"

namespace android::hardware::automotive::vehicle::V2_0::impl {

class GarageModeServerSideHandler {
  public:
    virtual ~GarageModeServerSideHandler() = default;
    virtual void HandleHeartbeat() = 0;
    char mVehicleInfoVin[17];
    char mVehicleInfoSn[20];
    char mVehicleInfoVersion[13];
};

std::unique_ptr<GarageModeServerSideHandler> makeGarageModeServerSideHandler(
        VehiclePropValuePool* valueObjectPool,PropertyManager* pm, EmulatedVehicleHalIface* hal);

}  // namespace android::hardware::automotive::vehicle::V2_0::impl
#endif
