// CommunicationManager.h

#ifndef COMMUNICATION_MANAGER_H
#define COMMUNICATION_MANAGER_H


#include "SPICommunication.h"
#include "EthernetCommunication.h"
#include "SerialCommunication.h"
#include "vehicle_config_parser.h"
// #include "ReceiveQueue.h"

namespace android {
namespace hardware {
namespace automotive {
namespace vehicle {
namespace V2_0 {
namespace impl {

enum CommunicationType {
    SERIAL,
    SPI,
    ETHERNET,
};

class CommunicationManager {
public:
    CommunicationManager(CommunicationType type);
    ~CommunicationManager();

    int openDevice(VehicleConfigParser& mconf);
    void closeDevice();
    void setType(int type);
    int getType(void);
    void sendData(const std::string& message);
    void startReceiveDataThread();
    void startSendDataThread();
    void stopReceiveDataThread();
    void stopSendDataThread();
    int receiveData(std::vector<uint8_t>& buffer);
    int SendMessage(const std::vector<uint8_t> buffer);
    int transferData(std::vector<uint8_t>& rbuffer, const std::vector<uint8_t> tbuffer);
	int transferData_V1_2(uint8_t *sendBuf, uint8_t *recvBuf,uint16_t len);
	int writeData_V1_2(uint8_t *sendBuf, uint16_t len);
	int readData_V1_2(uint8_t *recvBuf,uint16_t len);

// public:
//     SendQueue sendQueue;
//     ReceiveQueue receiveQueue;


private:
    std::thread receiver;
    std::thread sender;
    CommunicationType communicationType;
    SerialCommunication mSerialCommunication;
    SPICommunication mSpiCommunication;
    EthernetCommunication mEthernetCommunication;
};

}  // namespace impl

}  // namespace V2_0
}  // namespace vehicle
}  // namespace automotive
}  // namespace hardware
}  // namespace android

#endif  // COMMUNICATION_MANAGER_H

