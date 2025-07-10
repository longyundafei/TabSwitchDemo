// FIXME: your file license if you have one

#pragma once

#include <android/hardware/automotive/vehicle/2.0/IVehicleOtaCallback.h>
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

struct VehicleOtaCallback : public V2_0::IVehicleOtaCallback {
    // Methods from ::android::hardware::automotive::vehicle::V2_0::IVehicleOtaCallback follow.
    Return<void> otaEvent(::android::hardware::automotive::vehicle::V2_0::StatusCode status, uint32_t addr, const hidl_vec<uint8_t>& data, uint32_t len) override;

    // Methods from ::android::hidl::base::V1_0::IBase follow.

};

// FIXME: most likely delete, this is only for passthrough implementations
// extern "C" IVehicleOtaCallback* HIDL_FETCH_IVehicleOtaCallback(const char* name);

}  // namespace android::hardware::automotive::vehicle::implementation
