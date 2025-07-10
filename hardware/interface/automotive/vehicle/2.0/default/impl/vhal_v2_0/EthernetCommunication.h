#ifndef ETHERNET_COMMUNICATION_H
#define ETHERNET_COMMUNICATION_H

#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <deque>
#include <atomic>

class EthernetCommunication {
public:
    EthernetCommunication() : socketFd(-1), mExitThreads(false), mConnectionActive(false) {}
    ~EthernetCommunication() { closeDevice(); }
    
    void setPortname(std::string portname, int port);
    void openDevice();
    void closeDevice();
    int receiveData(std::vector<uint8_t>& buffer);
    int SendMessage(const std::vector<uint8_t> buffer);

private:
    // 网络连接相关
    std::string mportName;
    int mport;
    int socketFd;
    
    // 线程控制
    std::atomic<bool> mExitThreads;
    std::atomic<bool> mConnectionActive;
    
    // 线程对象
    std::thread mReadThread;
    std::thread mWriteThread;
    std::thread mConnectionMonitorThread;
    
    // 读取相关
    std::mutex mReadMutex;
    std::condition_variable mReadCondVar;
    std::deque<std::vector<uint8_t>> mReadBuffer;
    std::vector<uint8_t> mPacketBuffer;
    
    // 写入相关
    std::mutex mWriteMutex;
    std::condition_variable mWriteCondVar;
    std::deque<std::vector<uint8_t>> mWriteQueue;
    
    // 连接监控相关
    std::mutex mConnectionMutex;
    std::condition_variable mConnectionCondVar;
    
    // 私有方法
    void readThreadFunc();
    void writeThreadFunc();
    void connectionMonitorThreadFunc();
    
    bool createConnection();
    void startReadWriteThreads();
    void stopReadWriteThreads();
    bool isConnectionAlive();
};

#endif // ETHERNET_COMMUNICATION_H