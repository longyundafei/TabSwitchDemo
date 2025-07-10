#include "EthernetCommunication.h"
#include <arpa/inet.h>
#include <sys/socket.h>
#include <vector>
#include <unistd.h>
#include <android-base/logging.h>
#include <fstream>
#include <chrono>
#define MAX_BUFFER_SIZE 1024

void EthernetCommunication::setPortname(std::string portname,int port){
    this->mportName = portname;
    this->mport = port;
    LOG(INFO) << "Opening Serial device..." << this->mportName.c_str();
}

void EthernetCommunication::openDevice() {
    LOG(INFO) << "[EthernetCommunication] Try to open device: IP=" << mportName << ", Port=" << mport;
    
    mExitThreads = false;
    mConnectionActive = false;
    
    // 创建连接监控线程
    mConnectionMonitorThread = std::thread(&EthernetCommunication::connectionMonitorThreadFunc, this);
}

void EthernetCommunication::closeDevice() {
    LOG(INFO) << "Closing Ethernet device...";
    mExitThreads = true;
    mConnectionActive = false;
    
    // 通知所有条件变量
    mReadCondVar.notify_all();
    mWriteCondVar.notify_all();
    mConnectionCondVar.notify_all();
    
    // 等待连接监控线程结束
    if (mConnectionMonitorThread.joinable()) {
        mConnectionMonitorThread.join();
    }
    
    // 关闭socket
    if (socketFd != -1) {
        close(socketFd);
        socketFd = -1;
    }
}

bool EthernetCommunication::createConnection() {
    if (socketFd != -1) {
        close(socketFd);
        socketFd = -1;
    }
    
    socketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFd == -1) {
        LOG(ERROR) << "[EthernetCommunication] can't create socket";
        return false;
    }
    
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(mport);
    serverAddr.sin_addr.s_addr = inet_addr(mportName.c_str());
    
    LOG(INFO) << "[EthernetCommunication] Connecting to " << mportName << ":" << mport << " ...";
    if (connect(socketFd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        LOG(ERROR) << "[EthernetCommunication] can't connect server " << mportName << ":" << mport << ", errno=" << errno;
        close(socketFd);
        socketFd = -1;
        return false;
    }
    
    LOG(INFO) << "[EthernetCommunication] connected to " << mportName << ":" << mport;
    return true;
}

void EthernetCommunication::startReadWriteThreads() {
    // 确保之前的线程已经结束
    stopReadWriteThreads();
    
    // 创建新的读写线程
    mReadThread = std::thread(&EthernetCommunication::readThreadFunc, this);
    mWriteThread = std::thread(&EthernetCommunication::writeThreadFunc, this);
    
    LOG(INFO) << "[EthernetCommunication] Read/Write threads started";
}

void EthernetCommunication::stopReadWriteThreads() {
    // 等待读写线程结束
    if (mReadThread.joinable()) {
        mReadThread.join();
    }
    if (mWriteThread.joinable()) {
        mWriteThread.join();
    }
    
    LOG(INFO) << "[EthernetCommunication] Read/Write threads stopped";
}

bool EthernetCommunication::isConnectionAlive() {
    if (socketFd == -1) return false;
    
    // 使用send发送0字节数据来检测连接状态
    int result = send(socketFd, nullptr, 0, MSG_NOSIGNAL);
    if (result == -1) {
        if (errno == EPIPE || errno == ECONNRESET || errno == ENOTCONN) {
            return false;
        }
    }
    return true;
}

void EthernetCommunication::connectionMonitorThreadFunc() {
    const int RECONNECT_INTERVAL_MS = 5000; // 5秒重连间隔
    const int CONNECTION_CHECK_INTERVAL_MS = 3000; // 3秒检查间隔
    
    while (!mExitThreads) {
        if (!mConnectionActive) {
            // 尝试建立连接
            if (createConnection()) {
                mConnectionActive = true;
                startReadWriteThreads();
                LOG(INFO) << "[EthernetCommunication] Connection established";
            } else {
                LOG(ERROR) << "[EthernetCommunication] Failed to establish connection, retrying in " 
                          << RECONNECT_INTERVAL_MS << "ms";
                
                // 等待重连间隔
                std::unique_lock<std::mutex> lock(mConnectionMutex);
                mConnectionCondVar.wait_for(lock, std::chrono::milliseconds(RECONNECT_INTERVAL_MS));
                continue;
            }
        }
        
        // 检查连接是否仍然活跃
        if (mConnectionActive) {
            if (!isConnectionAlive()) {
                LOG(ERROR) << "[EthernetCommunication] Connection lost, attempting to reconnect";
                
                mConnectionActive = false;
                
                // 停止读写线程
                stopReadWriteThreads();
                
                // 关闭socket
                if (socketFd != -1) {
                    close(socketFd);
                    socketFd = -1;
                }
                
                // 清理缓冲区
                {
                    std::lock_guard<std::mutex> readLock(mReadMutex);
                    mReadBuffer.clear();
                    mPacketBuffer.clear();
                }
                {
                    std::lock_guard<std::mutex> writeLock(mWriteMutex);
                    mWriteQueue.clear();
                }
                
                continue; // 立即尝试重连
            }
        }
        
        // 等待下次检查
        std::unique_lock<std::mutex> lock(mConnectionMutex);
        mConnectionCondVar.wait_for(lock, std::chrono::milliseconds(CONNECTION_CHECK_INTERVAL_MS));
    }
    
    // 清理工作
    if (mConnectionActive) {
        stopReadWriteThreads();
        mConnectionActive = false;
    }
}

int EthernetCommunication::receiveData(std::vector<uint8_t>& buffer) {
    std::unique_lock<std::mutex> lock(mReadMutex);

    // 只有当mPacketBuffer和mReadBuffer都为空时才等待数据到来
    while (mPacketBuffer.empty() && mReadBuffer.empty() && !mExitThreads && mConnectionActive) {
        mReadCondVar.wait(lock);
    }
    if (mExitThreads || !mConnectionActive) return 0;

    // 把新数据追加到缓冲区
    while (!mReadBuffer.empty()) {
        const auto& newData = mReadBuffer.front();
        mPacketBuffer.insert(mPacketBuffer.end(), newData.begin(), newData.end());
        mReadBuffer.pop_front();
    }

    // 输出当前二进制缓冲区剩余字节数
    LOG(INFO) << "mPacketBuffer remaining bytes: " << mPacketBuffer.size();

    // 查找完整的报文
    while (mPacketBuffer.size() >= 4) {  // 至少需要4个字节来读取长度
        // 查找报文头 02 01 01
        size_t headerPos = std::string::npos;
        for (size_t i = 0; i <= mPacketBuffer.size() - 3; ++i) {
            if (mPacketBuffer[i] == 0x02 && 
                mPacketBuffer[i+1] == 0x01 && 
                mPacketBuffer[i+2] == 0x01) {
                headerPos = i;
                break;
            }
        }
        
        if (headerPos == std::string::npos) {
            // 没有找到报文头，清空缓冲区（或保留最后2个字节以防分包）
            if (mPacketBuffer.size() > 2) {
                mPacketBuffer.erase(mPacketBuffer.begin(), mPacketBuffer.end() - 2);
            }
            break;
        }
        
        // 如果报文头不在开始位置，删除头部无效数据
        if (headerPos > 0) {
            mPacketBuffer.erase(mPacketBuffer.begin(), mPacketBuffer.begin() + headerPos);
        }
        
        // 检查是否有足够的数据读取长度字段
        if (mPacketBuffer.size() < 4) {
            break;  // 等待更多数据
        }
        
        // 读取报文长度（第4个字节）
        uint8_t packetLength = mPacketBuffer[3];
        size_t totalPacketSize = 4 + packetLength + 2;  // 头部(4) + 数据(length，已包含校验)
        
        // 检查是否有完整的报文
        if (mPacketBuffer.size() < totalPacketSize) {
            break;  // 等待更多数据
        }
         
        LOG(ERROR) << "totalPacketSize: " << totalPacketSize;
        // 提取完整报文
        buffer.assign(mPacketBuffer.begin(), mPacketBuffer.begin() + totalPacketSize);
        mPacketBuffer.erase(mPacketBuffer.begin(), mPacketBuffer.begin() + totalPacketSize);

        return buffer.size();
    }

    // 没有找到完整的报文
    return 0;
}

int EthernetCommunication::SendMessage(const std::vector<uint8_t> buffer) {
    if (!mConnectionActive) {
        LOG(ERROR) << "[EthernetCommunication] Cannot send message, connection not active";
        return -1;
    }
    
    std::lock_guard<std::mutex> lock(mWriteMutex);
    mWriteQueue.push_back(buffer);
    mWriteCondVar.notify_one();
    return buffer.size(); // 仅表示入队长度
}

void EthernetCommunication::readThreadFunc() {
    while (!mExitThreads && mConnectionActive) {
        std::vector<uint8_t> temp(MAX_BUFFER_SIZE);
        int bytesRead = recv(socketFd, temp.data(), MAX_BUFFER_SIZE, 0);
        if (bytesRead > 0) {
            temp.resize(bytesRead);
            {
                std::lock_guard<std::mutex> lock(mReadMutex);
                mReadBuffer.push_back(temp);
            }
            mReadCondVar.notify_one();
        } else if (bytesRead == 0) {
            LOG(ERROR) << "[EthernetCommunication] Connection closed by peer";
            break;
        } else {
            if (errno == EINTR) continue;
            LOG(ERROR) << "[EthernetCommunication] recv error: " << strerror(errno);
            break;
        }
    }
    mReadCondVar.notify_all();
}

void EthernetCommunication::writeThreadFunc() {
    while (!mExitThreads && mConnectionActive) {
        std::vector<uint8_t> toSend;
        {
            std::unique_lock<std::mutex> lock(mWriteMutex);
            while (mWriteQueue.empty() && !mExitThreads && mConnectionActive) {
                mWriteCondVar.wait(lock);
            }
            if (mExitThreads || !mConnectionActive) break;
            toSend = mWriteQueue.front();
            mWriteQueue.pop_front();
        }
        if (!toSend.empty()) {
            ssize_t sent = send(socketFd, toSend.data(), toSend.size(), MSG_NOSIGNAL);
            if (sent < 0) {
                LOG(ERROR) << "send error: " << strerror(errno);
                if (errno == EPIPE || errno == ECONNRESET || errno == ENOTCONN) {
                    LOG(ERROR) << "[EthernetCommunication] Connection error detected in write thread";
                    break;
                }
            }
        }
    }
}