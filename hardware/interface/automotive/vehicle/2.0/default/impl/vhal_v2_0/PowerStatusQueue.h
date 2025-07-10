
#ifndef android_hardware_automotive_vehicle_V2_0_impl_PowerStatusQueue_H_
#define android_hardware_automotive_vehicle_V2_0_impl_PowerStatusQueue_H_

#include <queue>

namespace android {
namespace hardware {
namespace automotive {
namespace vehicle {
namespace V2_0 {

namespace impl {

  extern  std::queue<int> powerStatusQueue;
  extern  std::queue<int> powerOnStatusQueue;
}  // namespace impl

}  // namespace V2_0
}  // namespace vehicle
}  // namespace automotive
}  // namespace hardware
}  // namespace android

#endif  // android_hardware_automotive_vehicle_V2_0_impl_PowerStatusQueue_H_
