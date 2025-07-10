#include "SPICommunication.h"

void SPICommunication::setPortname(std::string portname){
    this->portname = portname;
}
void SPICommunication::openDevice() {
    std::cout << "Opening SPI device..." << std::endl;
    // 具体的打开 SPI 设备的代码
}

void SPICommunication::closeDevice() {
    std::cout << "Closing SPI device..." << std::endl;
    // 具体的关闭 SPI 设备的代码
}

void SPICommunication::receiveDataThread() {
    std::cout << "Starting SPI data receiving thread..." << std::endl;
    while (isReceivingData) {
        // 读取数据逻辑
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    std::cout << "SPI data receiving thread stopped." << std::endl;
}

void SPICommunication::sendDataThread() {
    std::cout << "Starting SPI data sending thread..." << std::endl;
    while (isSendingData) {
        // 从发送消息队列获取数据并发送
        std::string message;
        {
            std::lock_guard<std::mutex> lock(sendQueueMutex);
            if (!sendQueue.empty()) {
                message = sendQueue.front();
                sendQueue.pop();
            }
        }

        if (!message.empty()) {
            // 发送数据逻辑
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    std::cout << "SPI data sending thread stopped." << std::endl;
}

