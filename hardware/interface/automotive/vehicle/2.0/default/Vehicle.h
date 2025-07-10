// FIXME: your file license if you have one

#pragma once

#include <android/hardware/automotive/vehicle/2.0/IVehicle.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>

namespace android::hardware::automotive::vehicle::implementation {

using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::sp;

struct Vehicle : public V2_0::IVehicle {
    // Methods from ::android::hardware::automotive::vehicle::V2_0::IVehicle follow.
    Return<void> getAllPropConfigs(getAllPropConfigs_cb _hidl_cb) override;
    Return<void> getPropConfigs(const hidl_vec<int32_t>& props, getPropConfigs_cb _hidl_cb) override;
    Return<void> get(const ::android::hardware::automotive::vehicle::V2_0::VehiclePropValue& requestedPropValue, get_cb _hidl_cb) override;
    Return<::android::hardware::automotive::vehicle::V2_0::StatusCode> set(const ::android::hardware::automotive::vehicle::V2_0::VehiclePropValue& propValue) override;
    Return<::android::hardware::automotive::vehicle::V2_0::StatusCode> subscribe(const sp<::android::hardware::automotive::vehicle::V2_0::IVehicleCallback>& callback, const hidl_vec<::android::hardware::automotive::vehicle::V2_0::SubscribeOptions>& options) override;
    Return<::android::hardware::automotive::vehicle::V2_0::StatusCode> unsubscribe(const sp<::android::hardware::automotive::vehicle::V2_0::IVehicleCallback>& callback, int32_t propId) override;
    Return<void> debugDump(debugDump_cb _hidl_cb) override;
    Return<::android::hardware::automotive::vehicle::V2_0::StatusCode> udsSendBack(const hidl_vec<uint8_t>& data) override;
    Return<::android::hardware::automotive::vehicle::V2_0::StatusCode> sendOtaData(uint32_t addr, const hidl_vec<uint8_t>& data, uint32_t len) override;
    Return<::android::hardware::automotive::vehicle::V2_0::StatusCode> sendOtaCtlCmd(uint8_t ctlCmd) override;
    Return<::android::hardware::automotive::vehicle::V2_0::StatusCode> setOtaCallBack(const sp<::android::hardware::automotive::vehicle::V2_0::IVehicleOtaCallback>& callback) override;

    // Methods from ::android::hidl::base::V1_0::IBase follow.

};

// FIXME: most likely delete, this is only for passthrough implementations
// extern "C" IVehicle* HIDL_FETCH_IVehicle(const char* name);

}  // namespace android::hardware::automotive::vehicle::implementation
