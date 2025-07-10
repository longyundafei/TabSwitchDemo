#include "Machine.h"
#include <iostream>

Machine::Machine() : currentState(MachineState::Login) {}

void Machine::setState(MachineState state) {
    currentState = state;
}

MachineState Machine::getState() const {
    return currentState;
}

void Machine::process() {
    switch (currentState) {
        case MachineState::Login:
            //std::cout << "Processing Login state." << std::endl;
            LOG(INFO)<< "Processing Login state.";
            // 处理登录状态
            break;
        case MachineState::Realtime:
            //std::cout << "Processing Realtime state." << std::endl;
            LOG(INFO) << "Processing Realtime state.";
            // 处理实时状态
            break;
        case MachineState::Logout:
            //std::cout << "Processing Logout state." << std::endl;
            LOG(INFO) << "Processing Logout state.";
            // 处理注销状态
            break;
        default:
            //std::cout << "Invalid state." << std::endl;
            LOG(INFO) << "Invalid state.";
            break;
    }
}
