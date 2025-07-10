#ifndef ETHERNET_COMMUNICATION_H
#define ETHERNET_COMMUNICATION_H

#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <string>
#include <fcntl.h>
#include "ReceiveQueue.h"
#include "SendQueue.h"

class EthernetCommunication {
public:
    void openDevice();
    void closeDevice();
    void receiveDataThread(ReceiveQueue& receiveQueue);
    void sendDataThread(SendQueue& sendQueue);

private:
    bool isReceivingData = false;
    bool isSendingData = false;
    int socketFd;
    std::thread receiveThread;
    std::thread sendThread;
    std::mutex receiveQueueMutex;
    std::mutex sendQueueMutex;
//    std::queue<std::string> receiveQueue;
//    std::queue<std::string> sendQueue;
//    const int MAX_BUFFER_SIZE = 1024;
};

#endif  // ETHERNET_COMMUNICATION_H

