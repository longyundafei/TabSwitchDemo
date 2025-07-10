#include "networkcomm.h"
#include <fcntl.h>   // 包含 fcntl 函数所需的头文件
#include <unistd.h>  // 可能需要包含此头文件以获取其他常量
#include <cstring>

NetworkComm::NetworkComm(GB32960MessageData *pgbdata) {
    pmMessageData = pgbdata;
}

NetworkComm::~NetworkComm() {}

void NetworkComm::Start(XMLConfig* conf,bool sample) {
    std::string ip;
    std::string hostname;
    int port;
    mconf = conf;
    state = 0;
    if(sample) {
//    if (conf->vehicleConfig.isSampling == 1) {
        hostname = conf->vehicleConfig.publicDomain;
        port = conf->vehicleConfig.publicPort;
        mSample = 1;

    } else {
        hostname = conf->vehicleConfig.domainName;
        port = conf->vehicleConfig.port;
    }
    struct hostent *hptr;
    char* name = (char*)hostname.c_str();
    unsigned long add = inet_addr(name);
    if(add == INADDR_NONE){
        LOG(INFO) << __func__ << "sample = " << mSample <<" GOT a domain name not ip";
        state = 1;
        hptr = gethostbyname(name);
        char **pptr;
        if (hptr!=nullptr && hptr->h_addr_list != nullptr){
            pptr = hptr->h_addr_list;
            char str[INET_ADDRSTRLEN];
            ip = inet_ntop(hptr->h_addrtype, hptr->h_addr, str, sizeof(str));
        } else {
            LOG(INFO) << __func__ << "sample = " << mSample << " DNS resolve failed";
            mRetryCount++;
            return;
        }
    } else {
        LOG(INFO) << __func__ << "sample = " << mSample << " GOT a ip addr";
        ip = hostname;
    }
    LOG(INFO) << __func__ << "sample = " << mSample << " -ip:" << ip << "port:" << port << "samp:" << conf->vehicleConfig.isSampling;
    state = 2;
    startTime = 0;
    struct timeval tv;
    gettimeofday(&tv, NULL);
    startTime = tv.tv_sec*1000LL+tv.tv_usec/1000;
    if (0 == ETHconnect(ip, port) && !pmMessageData->mUniqueID.empty()) {
        //if(!sample){
        //    conf->ChangeNetParm = 0;
        //}
        std::vector<uint8_t> message;
        if(!sample){
            message = pmMessageData->generateLoginMessage();
        }else{
            message = pmMessageData->generateLoginMessageSample();
        }
        state = 3;
        mSendQueue.clear();
        mRecvQueue.clear();
        mSendQueue.enqueue(message);
        running = true;
        mRecvThread = std::thread(&NetworkComm::recvFunc, this);
        mSendThread = std::thread(&NetworkComm::sendFunc, this);
        //mRetryCount = 0;
        LOG(INFO) << __func__ << "sample = " << mSample << " create recv:" << mRecvThread.get_id() << " send:" << mSendThread.get_id();
        mNetState = NetState::Connected;
    } else {
//        setNetLose();
//        std::this_thread::sleep_for(std::chrono::seconds(5));
//        LOG(INFO) << __func__ << __LINE__;
//        mEndConnectThread = std::thread(&NetworkComm::endConnectFunc, this);
        if(mSocketFD != -1){
            close(mSocketFD);
        }
        if(conf->ChangeNetParm==1){
            mRetryCount = 0;
            ParmReportMessage parm;
            conf->LoadParmConfig(parm);
            parm.mdomainName = conf->ChangeNetParmSave.ip;
            parm.mdomainNameLen = conf->ChangeNetParmSave.iplen;
            parm.mport = conf->ChangeNetParmSave.port;
            parm.mpublicDomain = conf->ChangeNetParmSave.pip;
            parm.mpublicDomainLen = conf->ChangeNetParmSave.piplen;
            parm.mpublicPort = conf->ChangeNetParmSave.pport;
            conf->UpdateParmConfig(parm);
            conf->vehicleConfigInit();
            conf->ChangeNetParm = 0;
            LOG(INFO) << __func__ << "sample = " << mSample << " change ip revert:";
        }else{
            mRetryCount++;
            LOG(INFO) << __func__ << "sample = " << mSample << " mRetryCount" << mRetryCount;
        }
    }
    LOG(INFO) << __func__ << "sample = " << mSample << " out";
}
void NetworkComm::sendData(const std::string& data) {
    //...
    //std::cout << data << std::endl;
    LOG(INFO) << "sample = " << mSample << data;

}

void NetworkComm::stop() {
    //...
    //std::cout << __func__ << "in" << std::endl;
    LOG(INFO) << __func__ << "sample = " << mSample << "in";
    running = false;
    if (mRecvThread.joinable()) {
        LOG(INFO) << __func__ << "sample = " << mSample << __LINE__;
        mRecvThread.join();
    }
    if (mSendThread.joinable()) {
        LOG(INFO) << __func__ << __LINE__<< "sample = " << mSample;
        mSendThread.join();
    }
    LOG(INFO) << __func__ << __LINE__ << "sample = " << mSample << "sockfd:" << mSocketFD;
    close(mSocketFD);
    LOG(INFO) << __func__ << "sample = " << mSample << " out"
              << "Send: " << mSendQueue.getQueueSize() << "Recv: " << mRecvQueue.getQueueSize();
    //  std::cout << __func__ << std::endl;
}
int NetworkComm::ETHconnect(std::string& ip, int& port) {
    mSocketFD = socket(AF_INET, SOCK_STREAM, 0);
    if (mSocketFD == -1) {
        LOG(INFO) << "sample = " << mSample << " Failed to create socket";
        return 1;
    }
#if FEATURE_TBOX_DUAL_APN
    LOG(INFO) << __func__ << "sample = " << mSample << " dual apn is on, try to bind ppp0";
    const char *device_name = "ppp0";
    int ret = setsockopt(mSocketFD, SOL_SOCKET, SO_BINDTODEVICE, device_name, strlen(device_name) + 1);
    if (ret < 0) {
        LOG(INFO) << "sample = " << mSample << " Failed to bind socket to device";
        close(mSocketFD);
        return 1;
    }
#endif
    LOG(INFO) << __func__ << "sample = " << mSample << "try to bind usb0";
    const char *device_name = "usb0";
    int ret = setsockopt(mSocketFD, SOL_SOCKET, SO_BINDTODEVICE, device_name, strlen(device_name) + 1);
    if (ret < 0) {
        LOG(INFO) << "sample = " << mSample << " Failed to bind socket to device";
        close(mSocketFD);
        return 1;
    }
    // 将套接字设置为非阻塞模式
    int flags = fcntl(mSocketFD, F_GETFL, 0);
    if (flags == -1) {
        LOG(INFO) << "sample = " << mSample << " Failed to get socket flags";
        close(mSocketFD);
        return 1;
    }
    if (fcntl(mSocketFD, F_SETFL, flags | O_NONBLOCK) == -1) {
        LOG(INFO) << "sample = " << mSample << " Failed to set socket to non-blocking mode";
        close(mSocketFD);
        return 1;
    }

    // 准备连接的目标地址和端口
    const char* serverIP = ip.c_str();
    int serverPort = port;

    LOG(INFO) << __func__ << "sample = " << mSample << " -ip:" << serverIP << "port:" << port;
    struct sockaddr_in serverAddress {};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(serverPort);
    if (inet_pton(AF_INET, serverIP, &(serverAddress.sin_addr)) <= 0) {
        LOG(INFO) << "sample = " << mSample << " Failed to convert IP address";
        close(mSocketFD);
        return 1;
    }

    // 非阻塞连接
    int connectResult = connect(mSocketFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
    if (connectResult == -1 && errno != EINPROGRESS) {
        LOG(INFO) << "sample = " << mSample << " Failed to connect to the server";
        close(mSocketFD);
        return 1;
    }

    // 等待连接完成
    fd_set writeSet;
    FD_ZERO(&writeSet);
    FD_SET(mSocketFD, &writeSet);

    struct timeval timeout {};
    timeout.tv_sec = 5;  // 设置超时时间为5秒

    int selectResult = select(mSocketFD + 1, nullptr, &writeSet, nullptr, &timeout);
    if (selectResult == -1) {
        LOG(INFO) << "sample = " << mSample << " Connection timeout";
        close(mSocketFD);
        return 1;
    } else if (selectResult == 0) {
        LOG(INFO) << "sample = " << mSample << " Connection timeout";
        close(mSocketFD);
        return 1;
    } else {
        int socketError = 0;
        socklen_t errorLength = sizeof(socketError);
        if (getsockopt(mSocketFD, SOL_SOCKET, SO_ERROR, &socketError, &errorLength) == -1) {
            LOG(INFO) << "sample = " << mSample << " Failed to get socket error";
            close(mSocketFD);
            return 1;
        }
        if (socketError != 0) {
            LOG(INFO) << "sample = " << mSample << " Error in socket connection: " << strerror(socketError);
            close(mSocketFD);
            return 1;
        }
    }

    LOG(INFO) << "sample = " << mSample << " Connected successfully";
    return 0;
}

void NetworkComm::getNetStateChange() {

}
void NetworkComm::onConnectionChanged(bool connected) {
    //..._
    //std::cout << connected << std::endl;
    LOG(INFO) << "sample = " << mSample << connected;
}
void NetworkComm::setNetLose() {
    //std::cout << __func__ << std::endl;
    LOG(INFO) << __func__ << "sample = " << mSample ;
    std::unique_lock<std::mutex> lock(mutex);
    cv.notify_one();
}
void NetworkComm::setStopState() {
    //std::cout << __func__ << std::endl;
    LOG(INFO) << __func__ << "sample = " << mSample ;
    std::unique_lock<std::mutex> lock(stopmutex);
    stopcv.notify_one();
}
void NetworkComm::getUpdateNetState() {
    std::unique_lock<std::mutex> lock(mutex);
    //std::cout << __func__ << std::endl;
    LOG(INFO) << __func__ << "sample = " << mSample ;
    cv.wait(lock);
}
void NetworkComm::getStopState() {
    std::unique_lock<std::mutex> lock(stopmutex);
    //std::cout << __func__ << std::endl;
    LOG(INFO) << __func__ << "sample = " << mSample ;
    stopcv.wait(lock);
}

// 非阻塞读取数据函数
int NetworkComm::nonBlockingRead(unsigned char* buffer, int bufferSize) {
    ssize_t bytesRead = read(mSocketFD, buffer, bufferSize - 1);
    if (bytesRead == -1) {
        if (errno != EWOULDBLOCK && errno != EAGAIN) {
            //std::cerr << "Error in read" << std::endl;
            LOG(INFO) << "sample = " << mSample << " Error in read";
            return -1;
        }
        // 在非阻塞模式下，如果没有数据可读，read() 可能返回 EWOULDBLOCK 或 EAGAIN 错误
        return 0;
    } else if (bytesRead == 0) {
        // 读取到 EOF，连接已关闭
        //std::cout << "Connection closed by the remote peer" << std::endl;
        LOG(INFO) << "sample = " << mSample << " Connection closed by the remote peer";
        return 0;
    } else {
        // 成功读取数据
        buffer[bytesRead] = '\0';
        // std::cout << "Received data: " << buffer << std::endl;
        //std::cout << "Received data: ";

        std::ostringstream ss;
        for (auto i = 0;i<bytesRead;i++) {
            ss << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(buffer[i]) << " ";
        }
        LOG(INFO) << "sample = " << mSample << " Received data: " << ss.str();
/*        LOG(INFO) << "sample = " << mSample << " Received data: ";

        for (int i = 0; i < bytesRead; i++) {
            //std::cout << std::hex << std::setw(2) << std::setfill('0')
                      //<< static_cast<int>(buffer[i]);
            LOG(INFO) << std::hex << std::setw(2) << std::setfill('0')
                      << static_cast<int>(buffer[i]);
        }*/
        //std::cout << std::endl;
        return bytesRead;
    }
}

// 使用 select 进行非阻塞读取数据函数
int NetworkComm::selectNonBlockingRead(unsigned char* buffer, int bufferSize, int timeoutSec,
                                       int timeoutUsec) {
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(mSocketFD, &readfds);

    struct timeval timeout;
    timeout.tv_sec = timeoutSec;
    timeout.tv_usec = timeoutUsec;

    int selectResult = select(mSocketFD + 1, &readfds, nullptr, nullptr, &timeout);
    if (selectResult == -1) {
        //std::cerr << "Error in select" << std::endl;
        LOG(INFO) << "sample = " << mSample << " Error in select";
        return -1;
    } else if (selectResult == 0) {
        // 超时
        //std::cout << "Read timeout" << std::endl;
        LOG(INFO) << "sample = " << mSample << " Read timeout";
        return 0;
    } else {
        if (FD_ISSET(mSocketFD, &readfds)) {
            // 套接字可读
            return nonBlockingRead(buffer, bufferSize);
        }
    }

    return -1;  // 出现未知错误
}
void NetworkComm::endConnectFunc() {

    LOG(INFO) << __func__ << __LINE__ << " sample = " << mSample ;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    LOG(INFO) << __func__ << __LINE__ << " sample = " << mSample ;
    setNetLose();
    LOG(INFO) << __func__ << __LINE__ << " sample = " << mSample ;
}
void NetworkComm::recvFunc() {
    //...
    //std::cout << __func__ << "in" << std::endl;
    LOG(INFO) << __func__ << "sample = " << mSample << " in";
    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(mSocketFD, &rfds);
    std::vector<uint8_t> buffer(MAX_BUFFER_SIZE);
    ssize_t receivedBytes;
    int readtimeout = 0;
    while (running) {
        // 接收数据
        // 判断连接断开
        // 接收数据
        receivedBytes = selectNonBlockingRead(buffer.data(), MAX_BUFFER_SIZE, 1, 0);

        if (receivedBytes <= 0) {
            readtimeout++;
            if (readtimeout >= mconf->vehicleConfig.platformRespTimeout) {
                LOG(ERROR) << "sample = " << mSample << " platform resp timeout";
                setNetLose();
                running = false;
            }
        } else {
            readtimeout = 0;
            std::vector<uint8_t> message(buffer.begin(), buffer.begin() + receivedBytes);
            mRecvQueue.enqueue(message);
        }
    }
    LOG(INFO) << __func__ << __LINE__ << " sample = " << mSample ;
}
// 非阻塞写入数据函数
int NetworkComm::nonBlockingWrite(const unsigned char* buffer, int bufferSize) {
    std::ostringstream ss;
    for (auto i = 0;i<bufferSize;i++) {
        ss << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(buffer[i]) << " ";
    }
    LOG(INFO) << "sample = " << mSample << " Write byte: " << ss.str();
    //LOG(INFO) << ss.str();
    ssize_t bytesWritten = write(mSocketFD, buffer, bufferSize);
    if (bytesWritten == -1) {
        if (errno != EWOULDBLOCK && errno != EAGAIN) {
            //std::cerr << "Error in write" << std::endl;
            LOG(INFO) << "sample = " << mSample << " Error in write";
            return -1;
        }
        // 在非阻塞模式下，如果无法立即写入数据，write() 可能返回 EWOULDBLOCK 或 EAGAIN 错误
        return 0;
    } else {
        // 成功写入数据
        LOG(INFO) << "sample = " << mSample << " write data length: " << bytesWritten << ":" << mSocketFD;
        return bytesWritten;
    }
}

// 使用 select 进行非阻塞写入数据函数
int NetworkComm::selectNonBlockingWrite(const unsigned char* buffer, int bufferSize, int timeoutSec,
                                        int timeoutUsec) {
    fd_set writefds;
    FD_ZERO(&writefds);
    FD_SET(mSocketFD, &writefds);

    struct timeval timeout;
    timeout.tv_sec = timeoutSec;
    timeout.tv_usec = timeoutUsec;

    int selectResult = select(mSocketFD + 1, nullptr, &writefds, nullptr, &timeout);
    if (selectResult == -1) {
        //std::cerr << "Error in select" << std::endl;
        LOG(INFO) << "sample = " << mSample << " Error in select";
        return -1;
    } else if (selectResult == 0) {
        // 超时
        //std::cout << "Write timeout" << std::endl;
        LOG(INFO) << "sample = " << mSample << " Write timeout";
        return 0;
    } else {
        if (FD_ISSET(mSocketFD, &writefds)) {
            // 套接字可写
            return nonBlockingWrite(buffer, bufferSize);
        }
    }

    return -1;  // 出现未知错误
}
void NetworkComm::sendFunc() {
    //...
    //std::cout << __func__ << "in" << std::endl;
    LOG(INFO) << __func__ << "sample = " << mSample << " in";
    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(mSocketFD, &rfds);
    std::vector<uint8_t> buffer(MAX_BUFFER_SIZE);
    std::vector<uint8_t> message;
    int writetimeout = 0;
    while (running) {
        // 发送数据
        message = mSendQueue.dequeue(200);
        if (message.empty()) {
        } else {
            // 处理非空消息
            // 发送数据
            ssize_t sentBytes = selectNonBlockingWrite(message.data(), message.size(), 1, 0);
            if (sentBytes <= 0) {
                writetimeout++;
                if (writetimeout > mconf->vehicleConfig.terminalRespTimeout) {
                    LOG(ERROR) << "sample = " << mSample << " terminal resp timeout";
                    setNetLose();
                    running = false;
                }
            } else {
                writetimeout = 0;
            }
        }
    }
    LOG(INFO) << __func__ << __LINE__ << "sample = " << mSample ;
}

std::vector<uint8_t> NetworkComm::RecvQueueDequeue() {
    return mRecvQueue.dequeue();
}
void NetworkComm::SendQueueEnqueue(const std::vector<uint8_t>& message) {
    mSendQueue.enqueue(message);
}
void NetworkComm::SendCacheQueueEnqueue(const std::vector<uint8_t>& message) {
    mCacheSendQueue.enqueue(message);
}
std::vector<uint8_t> NetworkComm::SendCacheQueuedequeue() {
    return mCacheSendQueue.dequeue();
}
size_t NetworkComm::GetSendQueueEnqueueSize() {
    return mSendQueue.getQueueSize();
}
size_t NetworkComm::GetCacheSendQueueEnqueueSize() {
    return mCacheSendQueue.getQueueSize();
}
void NetworkComm::setMachineState(MachineState* state) {
    pMachineState = state;
}

int NetworkComm::checkNetwork(){
    mSocketFD = socket(AF_INET, SOCK_STREAM, 0);
    if (mSocketFD == -1) {
        LOG(INFO) << __func__ << "sample = " << mSample << " Failed to create socket";
        return 1;
    }
#if FEATURE_TBOX_DUAL_APN
    LOG(INFO) << __func__ << "sample = " << mSample << " dual apn is on, try to bind ppp0";
    const char *device_name = "ppp0";
    int ret = setsockopt(mSocketFD, SOL_SOCKET, SO_BINDTODEVICE, device_name, strlen(device_name) + 1);
    if (ret < 0) {
        LOG(INFO) << __func__ << "sample = " << mSample << " Failed to bind socket to device";
        close(mSocketFD);
        return 1;
    }
#endif
    int flags = fcntl(mSocketFD, F_GETFL, 0);
    if (flags == -1) {
        LOG(INFO) << __func__ << "sample = " << mSample << " Failed to get socket flags";
        close(mSocketFD);
        return 1;
    }
    if (fcntl(mSocketFD, F_SETFL, flags | O_NONBLOCK) == -1) {
        LOG(INFO) << __func__ << "sample = " << mSample << " Failed to set socket to non-blocking mode";
        close(mSocketFD);
        return 1;
    }

    // 准备连接的目标地址和端口
    std::string ip = "122.9.138.193";
    const char* serverIP = ip.c_str();
    int serverPort = 6638;

    LOG(INFO) << __func__ << "sample = " << mSample << " -ip:" << serverIP << "port: 6638";
    struct sockaddr_in serverAddress {};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(serverPort);
    if (inet_pton(AF_INET, serverIP, &(serverAddress.sin_addr)) <= 0) {
        LOG(INFO) << __func__ << "sample = " << mSample << " Failed to convert IP address";
        close(mSocketFD);
        return 1;
    }

    // 非阻塞连接
    int connectResult = connect(mSocketFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
    if (connectResult == -1 && errno != EINPROGRESS) {
        LOG(INFO) << __func__ << "sample = " << mSample << " Failed to connect to the server";
        close(mSocketFD);
        return 1;
    }
    LOG(INFO) << "sample = " << mSample << " ppp0 exist";
    close(mSocketFD);
    return 0;
}