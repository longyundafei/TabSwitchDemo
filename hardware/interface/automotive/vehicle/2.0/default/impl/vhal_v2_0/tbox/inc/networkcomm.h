#ifndef NETWORKCOMM_H
#define NETWORKCOMM_H

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <ostream>
#include <string>
#include <thread>
#include "GB32960MessageData.h"
#include "Machine.h"
#include "messagequeue.h"
#include "xml_config.h"
#include "parm_report_message.h"
#include <netdb.h>
#include <arpa/inet.h>

#define MAX_BUFFER_SIZE 1024
#define FEATURE_TBOX_DUAL_APN 0
class NetworkComm {
  public:
    NetworkComm(GB32960MessageData *pgbdata);
    ~NetworkComm();

    void sendData(const std::string& data);
    void stop();
    void onConnectionChanged(bool connected);
    int ETHconnect(std::string& ip, int& port);
    //  void Start(std::string& ip, int& port);
    void Start(XMLConfig* conf,bool sample);
    void getUpdateNetState();
    void setNetLose();
    void setStopState();
    void getStopState();
    void getNetStateChange();
    int nonBlockingRead(unsigned char* buffer, int bufferSize);
    int selectNonBlockingRead(unsigned char* buffer, int bufferSize, int timeoutSec,
                              int timeoutUsec);
    int nonBlockingWrite(const unsigned char* buffer, int bufferSize);
    int selectNonBlockingWrite(const unsigned char* buffer, int bufferSize, int timeoutSec,
                               int timeoutUsec);
    std::vector<uint8_t> RecvQueueDequeue();
    void SendQueueEnqueue(const std::vector<uint8_t>& message);
    void SendCacheQueueEnqueue(const std::vector<uint8_t>& message);
    std::vector<uint8_t> SendCacheQueuedequeue();
    void setMachineState(MachineState* state);
    size_t GetSendQueueEnqueueSize();
    size_t GetCacheSendQueueEnqueueSize();
    int checkNetwork();
    GB32960MessageData* pmMessageData;
    MessageQueue* pMsgQueue = nullptr;

  public:
    int mRetryCount = 0;
    XMLConfig* mconf;
    enum class NetState {
        NoConnect,
        Connected,
        ConnectedRetry,
    };
    NetState mNetState;
    int state = 0;      //0: default
                        //1: DNS resolving;
                        //2: socket connecting;
                        //3: logining
    long long startTime = 0;
    bool running = true;
    int mSample = 0;
  private:
    void recvFunc();
    void sendFunc();
    void endConnectFunc();
    MessageQueue mSendQueue;
    MessageQueue mCacheSendQueue;
    MessageQueue mRecvQueue;
    int mSocketFD;
    std::thread mRecvThread;
    std::thread mSendThread;
    std::thread mEndConnectThread;
    std::condition_variable cv;
    std::condition_variable stopcv;
    std::mutex mutex;
    std::mutex stopmutex;
    MachineState* pMachineState;
};

#endif