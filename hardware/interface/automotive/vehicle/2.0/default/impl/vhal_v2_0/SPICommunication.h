#ifndef SPI_COMMUNICATION_H
#define SPI_COMMUNICATION_H

#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <string>

#define MAX_BUFFER_SIZE 1024

class SPICommunication {
public:
    void openDevice();
    void closeDevice();
    void receiveDataThread();
    void sendDataThread();
    void setPortname(std::string portname);
    int receiveData(std::vector<uint8_t>& buffer);
    int SendMessage(const std::vector<uint8_t> buffer);
    int transferData(std::vector<uint8_t>& rbuffer, const std::vector<uint8_t> tbuffer);
	void dumpstat(const char *name, int fd);
	int transferData_V1_2(uint8_t *sendBuf, uint8_t *recvBuf,uint16_t len);
	int spiWrite_V1_2(uint8_t *sendBuf,uint16_t len);
	int spiRead_V1_2(uint8_t *recvBuf,uint16_t len);

private:
    int fd;
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

