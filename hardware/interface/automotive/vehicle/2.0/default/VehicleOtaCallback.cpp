// FIXME: your file license if you have one

#include "VehicleOtaCallback.h"

namespace android::hardware::automotive::vehicle::implementation {

// Methods from ::android::hardware::automotive::vehicle::V2_0::IVehicleOtaCallback follow.
Return<void> VehicleOtaCallback::otaEvent(::android::hardware::automotive::vehicle::V2_0::StatusCode status, uint32_t addr, const hidl_vec<uint8_t>& data, uint32_t len) {
    // TODO implement
    return Void();
}


// Methods from ::android::hidl::base::V1_0::IBase follow.

//IVehicleOtaCallback* HIDL_FETCH_IVehicleOtaCallback(const char* /* name */) {
    //return new VehicleOtaCallback();
//}
//
}  // namespace android::hardware::automotive::vehicle::implementation
