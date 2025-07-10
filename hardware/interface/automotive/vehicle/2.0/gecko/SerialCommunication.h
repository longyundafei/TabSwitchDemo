// SerialCommunication.h

#ifndef SERIAL_COMMUNICATION_H
#define SERIAL_COMMUNICATION_H

#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <string>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include "SendQueue.h"
#include "ReceiveQueue.h"

class SerialCommunication {
public:
    int openDevice();
    void closeDevice();
    void receiveDataThread(ReceiveQueue& receiveQueue);
    void sendDataThread(SendQueue& sendQueue);
    void setPortname(std::string portname);

//public:
//    SendQueue sendQueue;
//    ReceiveQueue receiveQueue;

private:
    bool isReceivingData = false;
    bool isSendingData = false;
    int serialPort;
    std::string portName;
    std::thread receiveThread;
    std::thread sendThread;
};

#endif  // SERIAL_COMMUNICATION_H

