#ifndef MACHINE_H
#define MACHINE_H

#include <android-base/logging.h>

enum class MachineState {
    OffLine,
    OnLine,
    Login,
    UpReport,
    Realtime,
    Logout,
};

class Machine {
  public:
    Machine();  // 构造函数

    void setState(MachineState state);  // 设置状态
    MachineState getState() const;      // 获取当前状态

    void process();  // 处理状态

  private:
    MachineState currentState;  // 当前状态
};

#endif  // MACHINE_H
