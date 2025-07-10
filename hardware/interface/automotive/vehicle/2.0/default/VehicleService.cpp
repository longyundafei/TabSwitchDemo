/*
 * Copyright (C) 2016 The Android Open Source Project
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

#define LOG_TAG "automotive.vehicle@2.0-service"
#include <android/log.h>
#include <hidl/HidlTransportSupport.h>

#include <iostream>

#include <vhal_v2_0/EmulatedUserHal.h>
#include <vhal_v2_0/EmulatedVehicleConnector.h>
#include <vhal_v2_0/EmulatedVehicleHal.h>
#include <vhal_v2_0/VehicleHalManager.h>
#include <vhal_v2_0/property_manager.h>
#include <vhal_v2_0/GarageModeServerSideHandler.h>
//#include <vhal_v2_0/SendQueue.h>

using namespace android;
using namespace android::hardware;
using namespace android::hardware::automotive::vehicle::V2_0;
using namespace android::hardware::automotive::vehicle::V2_0::impl;
int main(int /* argc */, char* /* argv */ []) {
//    SendQueue mSendQueue;
    PropertyManager mPropManager;
    mPropManager.importPropertiesFromFile("/vendor/etc/properties.txt",mPropManager);           // 将所有属性添加到 mPropManager
    VehiclePropValuePool mValueObjectPool;
    auto store = std::make_unique<VehiclePropertyStore>();
    auto connector = std::make_unique<impl::EmulatedVehicleConnector>();
    auto mconf = connector->GetPropConfig();
    mPropManager.setmpConfigParser(mconf);
    connector->setPropertyManager(&mPropManager);
    auto userHal = connector->getEmulatedUserHal();
    auto hal = std::make_unique<impl::EmulatedVehicleHal>(store.get(), connector.get(), userHal);
    std::unique_ptr<android::hardware::automotive::vehicle::V2_0::impl::GarageModeServerSideHandler> mGarageModeHandler;
    mGarageModeHandler = makeGarageModeServerSideHandler(&mValueObjectPool, &mPropManager, hal.get());
    auto emulator = std::make_unique<impl::VehicleEmulator>(hal.get(),&mPropManager);
	PipeComm* pipeComm= emulator->getComm();
    auto service = std::make_unique<VehicleHalManager>(hal.get(),pipeComm);
    connector->setValuePool(hal->getValuePool());
    configureRpcThreadpool(4, true /* callerWillJoin */);
    status_t status = service->registerAsService();
    if (status != OK) {
        ALOGE("main Unable to register vehicle service (%d)", status);
        return 1;
    }
    joinRpcThreadpool();
    return 1;
}
