#include "PowerStatusQueue.h"

namespace android {
namespace hardware {
namespace automotive {
namespace vehicle {
namespace V2_0 {

namespace impl {

  std::queue<int> powerStatusQueue;
  std::queue<int> powerOnStatusQueue;
}  // namespace impl

}  // namespace V2_0
}  // namespace vehicle
}  // namespace automotive
}  // namespace hardware
}  // namespace android
