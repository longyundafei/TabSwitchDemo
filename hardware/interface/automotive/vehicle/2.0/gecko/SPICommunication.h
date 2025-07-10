#ifndef SPI_COMMUNICATION_H
#define SPI_COMMUNICATION_H

#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <string>

class SPICommunication {
public:
    void openDevice();
    void closeDevice();
    void receiveDataThread();
    void sendDataThread();
    void setPortname(std::string portname);

private:
    bool isReceivingData = false;
    bool isSendingData = false;
    std::string portname;
    std::thread receiveThread;
    std::thread sendThread;
    std::mutex receiveQueueMutex;
    std::mutex sendQueueMutex;
    std::queue<std::string> receiveQueue;
    std::queue<std::string> sendQueue;
};

#endif  // SPI_COMMUNICATION_H

