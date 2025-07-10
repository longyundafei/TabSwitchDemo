// CommunicationManager.cpp

#include "CommunicationManager.h"

CommunicationManager::CommunicationManager(CommunicationType type)
    : communicationType(type) {
}

CommunicationManager::~CommunicationManager() {
    closeDevice();
}

int CommunicationManager::openDevice(std::string portname) {
    switch (communicationType) {
        case SERIAL:
	    serialCommunication.setPortname(portname);
            serialCommunication.openDevice();
            break;
        case SPI:
	    spiCommunication.setPortname(portname);
            spiCommunication.openDevice();
            break;
        case ETHERNET:
            ethernetCommunication.openDevice();
            break;
    }
    return 0;
}

void CommunicationManager::closeDevice() {
    switch (communicationType) {
        case SERIAL:
            serialCommunication.closeDevice();
            break;
        case SPI:
            spiCommunication.closeDevice();
            break;
        case ETHERNET:
            ethernetCommunication.closeDevice();
            break;
    }
}
void CommunicationManager::startReceiveDataThread() {
    switch (communicationType) {
        case SERIAL:
            receiver = std::thread(&SerialCommunication::receiveDataThread, &serialCommunication, std::ref(receiveQueue));
            receiver.detach();
            break;
        case SPI:
            break;
        case ETHERNET:
            receiver = std::thread(&EthernetCommunication::receiveDataThread, &ethernetCommunication, std::ref(receiveQueue));
            receiver.detach();
            break;
    }
}
void CommunicationManager::startSendDataThread() {
    switch (communicationType) {
        case SERIAL:
            sender = std::thread(&SerialCommunication::sendDataThread, &serialCommunication, std::ref(sendQueue));
            sender.detach();
            break;
        case SPI:
            break;
        case ETHERNET:
            sender = std::thread(&EthernetCommunication::sendDataThread, &ethernetCommunication, std::ref(sendQueue));
            sender.detach();
            break;
    }
}
void CommunicationManager::stopSendDataThread() {
	sender.join();
}
void CommunicationManager::stopReceiveDataThread() {
    receiver.join();
}
/*
void CommunicationManager::sendData(const std::string& message) {
    switch (communicationType) {
        case SERIAL:
            serialCommunication.sendData(message);
            break;
        case SPI:
            spiCommunication.sendData(message);
            break;
        case ETHERNET:
            ethernetCommunication.sendData(message);
            break;
    }
}
*/
