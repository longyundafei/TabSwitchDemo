// CommunicationManager.cpp

#include "CommunicationManager.h"
#include <log/log.h>

namespace android {
namespace hardware {
namespace automotive {
namespace vehicle {
namespace V2_0 {
namespace impl {
CommunicationManager::CommunicationManager(CommunicationType type)
    : communicationType(type) {
}

CommunicationManager::~CommunicationManager() {
    closeDevice();
}
void CommunicationManager::setType(int type){
    switch(type){
        case 0:
             communicationType = SERIAL;
        break;
        case 1:
             communicationType = SPI;
        break;
        case 2:
             communicationType = ETHERNET;
        break;
        default:
             communicationType = SERIAL;
        break;
    }
}

int CommunicationManager::getType(void){
    return communicationType;
}

int CommunicationManager::openDevice(VehicleConfigParser& mconf) {
    switch (communicationType) {
        case SERIAL:
            mSerialCommunication.setPortname(mconf.mPortName);
            mSerialCommunication.openDevice();
            break;
        case SPI:
            mSpiCommunication.setPortname(mconf.mPortName);
            mSpiCommunication.openDevice();
            break;
        case ETHERNET:
            mEthernetCommunication.setPortname(mconf.mIPServer,mconf.mPort);
            mEthernetCommunication.openDevice();
            break;
    }
    return 0;
}

void CommunicationManager::closeDevice() {
    switch (communicationType) {
        case SERIAL:
            mSerialCommunication.closeDevice();
            break;
        case SPI:
            mSpiCommunication.closeDevice();
            break;
        case ETHERNET:
            mEthernetCommunication.closeDevice();
            break;
    }
}
int CommunicationManager::receiveData(std::vector<uint8_t>& buffer) {
    int bytesRead = 0;
    switch (communicationType) {
        case SERIAL:
            bytesRead = mSerialCommunication.receiveData(buffer);
            break;
        case SPI:
            bytesRead = mSpiCommunication.receiveData(buffer);
            break;
        case ETHERNET:
            bytesRead = mEthernetCommunication.receiveData(buffer);
            break;
    }
    return bytesRead;
}

int CommunicationManager::transferData(std::vector<uint8_t>& rbuffer, const std::vector<uint8_t> tbuffer) {
    int bytesRead = 0;
    switch (communicationType) {
        case SERIAL:
            // 如有需要可实现串口转发
            break;
        case SPI:
            bytesRead = mSpiCommunication.transferData(rbuffer, tbuffer);
            break;
        case ETHERNET:
            // 发送数据
            if (!tbuffer.empty()) {
                mEthernetCommunication.SendMessage(tbuffer);
            }
            // 接收数据
            if (!rbuffer.empty()) {
                bytesRead = mEthernetCommunication.receiveData(rbuffer);
            }
            break;
    }
    return bytesRead;
}


int CommunicationManager::transferData_V1_2(uint8_t *sendBuf, uint8_t *recvBuf, uint16_t len) {
    int ret = 0;
    switch (communicationType) {
        case SERIAL:
            break;
        case SPI:
            ret = mSpiCommunication.transferData_V1_2(sendBuf, recvBuf, len);
            break;
        case ETHERNET: {
            ALOGI("ETHERNET branch: sendBuf[0..3]=%02x %02x %02x %02x, len=%d", 
                  sendBuf ? sendBuf[0] : 0, sendBuf ? sendBuf[1] : 0, sendBuf ? sendBuf[2] : 0, sendBuf ? sendBuf[3] : 0, (int)len);
            std::vector<uint8_t> sendVec(sendBuf, sendBuf + len);
            ret = mEthernetCommunication.SendMessage(sendVec);
            ALOGI("SendMessage ret: %d", ret);
            if (ret > 0 && recvBuf != nullptr) {
                std::vector<uint8_t> recvVec;
                ALOGI("before receiveData");
                ret = mEthernetCommunication.receiveData(recvVec);
                ALOGI("receiveData ret: %d", ret);
                if (ret > 0 && !recvVec.empty()) {
                    memcpy(recvBuf, recvVec.data(), std::min((size_t)ret, recvVec.size()));
                    ALOGI("recvVec[0..3]=%02x %02x %02x %02x, size=%d", 
                          recvVec[0], recvVec[1], recvVec[2], recvVec[3], (int)recvVec.size());
                } else {
                    ALOGI("No data received or receiveData ret <= 0");
                }
            } else {
                ALOGI("SendMessage failed or recvBuf is null");
            }
            break;
        }
    }
    return ret;
}


int android::hardware::automotive::vehicle::V2_0::impl::CommunicationManager::writeData_V1_2(uint8_t *sendBuf, uint16_t len) {
    int ret = 0;
    switch (communicationType) {
        case SERIAL:
            break;
        case SPI:
            ret = mSpiCommunication.spiWrite_V1_2(sendBuf,len);
            break;
        case ETHERNET:
            break;
    }
    return ret;
}

int android::hardware::automotive::vehicle::V2_0::impl::CommunicationManager::readData_V1_2(uint8_t *recvBuf,uint16_t len) {
    int ret = 0;
    switch (communicationType) {
        case SERIAL:
            break;
        case SPI:
            ret = mSpiCommunication.spiRead_V1_2(recvBuf,len);
            break;
        case ETHERNET:
            break;
    }
    return ret;
}




int android::hardware::automotive::vehicle::V2_0::impl::CommunicationManager::SendMessage(const std::vector<uint8_t> buffer) {
    int bytesRead = 0;
    switch (communicationType) {
        case SERIAL:
            bytesRead = mSerialCommunication.SendMessage(buffer);
            break;
        case SPI:
            bytesRead = mSpiCommunication.SendMessage(buffer);
            break;
        case ETHERNET:
            bytesRead = mEthernetCommunication.SendMessage(buffer);
            break;
    }
    return bytesRead;
}
void android::hardware::automotive::vehicle::V2_0::impl::CommunicationManager::startReceiveDataThread() {
    // switch (communicationType) {
    //     case SERIAL:
    //         receiver = std::thread(&SerialCommunication::receiveDataThread, &serialCommunication, std::ref(receiveQueue));
    //         receiver.detach();
    //         break;
    //     case SPI:
    //         break;
    //     case ETHERNET:
    //         receiver = std::thread(&EthernetCommunication::receiveDataThread, &ethernetCommunication, std::ref(receiveQueue));
    //         receiver.detach();
    //         break;
    // }
}
void android::hardware::automotive::vehicle::V2_0::impl::CommunicationManager::startSendDataThread() {
    // switch (communicationType) {
    //     case SERIAL:
    //         sender = std::thread(&SerialCommunication::sendDataThread, &serialCommunication, std::ref(sendQueue));
    //         sender.detach();
    //         break;
    //     case SPI:
    //         break;
    //     case ETHERNET:
    //         sender = std::thread(&EthernetCommunication::sendDataThread, &ethernetCommunication, std::ref(sendQueue));
    //         sender.detach();
    //         break;
    // }
}
void android::hardware::automotive::vehicle::V2_0::impl::CommunicationManager::stopSendDataThread() {
	// sender.join();
}
void android::hardware::automotive::vehicle::V2_0::impl::CommunicationManager::stopReceiveDataThread() {
    // receiver.join();
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
}  // namespace impl

}  // namespace V2_0
}  // namespace vehicle
}  // namespace automotive
}  // namespace hardware
}  // namespace android
