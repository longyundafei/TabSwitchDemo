#include "EthernetCommunication.h"
#include <arpa/inet.h>
#include <sys/socket.h>
#include <vector>
#include <unistd.h>

void EthernetCommunication::openDevice() {
    std::cout << "Opening Ethernet device..." << std::endl;
    // 具体的打开以太网设备的代码
  socketFd = socket(AF_INET, SOCK_STREAM, 0);

  // 将socket和端口绑定
  sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(8585);

  bind(socketFd, (struct sockaddr*)&addr, sizeof(addr));

  // 开始监听
  listen(socketFd, 10); 

  std::cout << "Server listening on port 8585\n";
}

void EthernetCommunication::closeDevice() {
    std::cout << "Closing Ethernet device..." << std::endl;
    // 具体的关闭以太网设备的代码
    close(socketFd);
}

void EthernetCommunication::receiveDataThread(ReceiveQueue& receiveQueue) {
    std::cout << "Starting Ethernet data receiving thread..." << std::endl;
    int clientFd;
//    char buffer[1024];
    ssize_t bytesRead = 0;
    std::vector<uint8_t> buffer(MAX_BUFFER_SIZE);
    isReceivingData = true;
    while (isReceivingData) {
        // 读取数据逻辑
        // 接受客户端连接
        clientFd = accept(socketFd, NULL, NULL);
        // 接收客户端数据
        bytesRead = read(clientFd, buffer.data(), MAX_BUFFER_SIZE );
        if (bytesRead > 0) {
            // 将接收到的数据插入接收消息队列
            std::vector<uint8_t> message(buffer.begin(), buffer.begin() + bytesRead);
            receiveQueue.enqueue(message);
        }
        close(clientFd);
        //std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    std::cout << "Ethernet data receiving thread stopped." << std::endl;
}

void EthernetCommunication::sendDataThread(SendQueue& sendQueue) {
    std::cout << "Starting Ethernet data sending thread..." << std::endl;
    while (isSendingData) {
        // 从发送消息队列获取数据并发送
//        std::string message;
//        {
//            std::lock_guard<std::mutex> lock(sendQueueMutex);
//            if (!sendQueue.empty()) {
//                message = sendQueue.front();
//                sendQueue.pop();
//            }
//        }
//
//        if (!message.empty()) {
//            // 发送数据逻辑
//        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    std::cout << "Ethernet data sending thread stopped." << std::endl;
}

