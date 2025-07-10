// FIXME: your file license if you have one

#include "VehicleCallback.h"

namespace android::hardware::automotive::vehicle::implementation {

// Methods from ::android::hardware::automotive::vehicle::V2_0::IVehicleCallback follow.
Return<void> VehicleCallback::onPropertyEvent(const hidl_vec<::android::hardware::automotive::vehicle::V2_0::VehiclePropValue>& propValues) {
    // TODO implement
    return Void();
}

Return<void> VehicleCallback::onPropertySet(const ::android::hardware::automotive::vehicle::V2_0::VehiclePropValue& propValue) {
    // TODO implement
    return Void();
}

Return<void> VehicleCallback::onPropertySetError(::android::hardware::automotive::vehicle::V2_0::StatusCode errorCode, int32_t propId, int32_t areaId) {
    // TODO implement
    return Void();
}


// Methods from ::android::hidl::base::V1_0::IBase follow.

//IVehicleCallback* HIDL_FETCH_IVehicleCallback(const char* /* name */) {
    //return new VehicleCallback();
//}
//
}  // namespace android::hardware::automotive::vehicle::implementation
