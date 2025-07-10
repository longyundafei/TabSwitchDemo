// CommunicationManager.h

#ifndef COMMUNICATION_MANAGER_H
#define COMMUNICATION_MANAGER_H

#include "SerialCommunication.h"
#include "SPICommunication.h"
#include "EthernetCommunication.h"
#include "SendQueue.h"
#include "ReceiveQueue.h"

enum CommunicationType {
    SERIAL,
    SPI,
    ETHERNET
};

class CommunicationManager {
public:
    CommunicationManager(CommunicationType type);
    ~CommunicationManager();

    int openDevice(std::string portname);
    void closeDevice();
    void sendData(const std::string& message);
    void startReceiveDataThread();
    void startSendDataThread();
    void stopReceiveDataThread();
    void stopSendDataThread();

public:
    SendQueue sendQueue;
    ReceiveQueue receiveQueue;


private:
    std::thread receiver;
    std::thread sender;
    CommunicationType communicationType;
    SerialCommunication serialCommunication;
    SPICommunication spiCommunication;
    EthernetCommunication ethernetCommunication;
};

#endif  // COMMUNICATION_MANAGER_H

