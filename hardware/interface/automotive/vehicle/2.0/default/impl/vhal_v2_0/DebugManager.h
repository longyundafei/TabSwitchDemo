#include <atomic>
#include <android-base/properties.h>
class DebugManager {
public:
    static DebugManager& getInstance() {
        static DebugManager instance;
        return instance;
    }

    void setOpenDebug(bool value) {
        openDebug.store(value, std::memory_order_relaxed); // 写入
    }

    bool isOpenDebug() {
        std::string buildType = android::base::GetProperty("persist.gecko.hal.debug.on", "0");
        int32_t value = static_cast<int32_t> (std::stoi(buildType)); 
        return (value == 1); // 读取
    }

private:
    DebugManager() : openDebug(false) {}
    ~DebugManager() = default;

    DebugManager(const DebugManager&) = delete;
    DebugManager& operator=(const DebugManager&) = delete;

    std::atomic<bool> openDebug;
};

