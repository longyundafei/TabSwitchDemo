#ifndef REALTIMEMESSAGEBASE_H 
#define REALTIMEMESSAGEBASE_H

#include <cstdint>
#include <vector>
#include "TboxDataDeclaration.h"
#include "LockMap.h"
#include "vehicle_controller.h"
#include <android/hardware/automotive/vehicle/2.0/IVehicle.h>
#include <cmath>
using namespace android::hardware::automotive::vehicle::V2_0;

class RealTimeMessageBase {
  public:
    RealTimeMessageBase(LockMap<int32_t, TboxDataDeclaration>& tboxdecl,VehicleController& vehicle);
    uint32_t getValue(int propId);
    uint32_t getRawValue(int propId);
    uint32_t getRawValue(int propId,int areaId);
    uint32_t getRawPropValue(int propId);
    uint32_t tboxReturnValue(int propId);
    uint32_t getRawPropFloatToIntValue(int propId,int areaId);
    VehicleController mVehicle;
    LockMap<int32_t, TboxDataDeclaration>* mTboxdecl;
    /** Returns underlying (integer) value for given enum. */
    template<typename ENUM, typename U = typename std::underlying_type<ENUM>::type>
    inline constexpr U toInt(ENUM const value) {
        return static_cast<U>(value);
    }
};

#endif  // VEHICLEDATAMESSAGE_H
