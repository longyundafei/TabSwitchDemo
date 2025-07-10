// FIXME: your file license if you have one

#pragma once

#include <android/hardware/automotive/vehicle/2.0/IVehicleCallback.h>
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

struct VehicleCallback : public V2_0::IVehicleCallback {
    // Methods from ::android::hardware::automotive::vehicle::V2_0::IVehicleCallback follow.
    Return<void> onPropertyEvent(const hidl_vec<::android::hardware::automotive::vehicle::V2_0::VehiclePropValue>& propValues) override;
    Return<void> onPropertySet(const ::android::hardware::automotive::vehicle::V2_0::VehiclePropValue& propValue) override;
    Return<void> onPropertySetError(::android::hardware::automotive::vehicle::V2_0::StatusCode errorCode, int32_t propId, int32_t areaId) override;

    // Methods from ::android::hidl::base::V1_0::IBase follow.

};

// FIXME: most likely delete, this is only for passthrough implementations
// extern "C" IVehicleCallback* HIDL_FETCH_IVehicleCallback(const char* name);

}  // namespace android::hardware::automotive::vehicle::implementation
