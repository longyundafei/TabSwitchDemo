#include "CommunicationThread.h"
#include <thread>
#include <chrono>

CommunicationThread::CommunicationThread(const std::string& port) 
    : mPortname(port), mManager(ETHERNET) {
    //: mPortname(port), mManager(SERIAL) {

  // 启动线程
  this->m_thread = std::thread(&CommunicationThread::ThreadFunc, this); 

}

CommunicationThread::~CommunicationThread() {

  // 等待线程结束
  this->m_thread.join();

}

MessageHandler* CommunicationThread::getHandler() {
  return &mHandler;
}

void CommunicationThread::ThreadFunc() {

  // 线程执行函数实现
  // ...
    mManager.openDevice(mPortname);
    mManager.startReceiveDataThread();
    mManager.startSendDataThread();
    
    while (true) {
        // Get and handle message from the receive queue
        std::vector<uint8_t> receivedMessage = mManager.receiveQueue.dequeue();
        mHandler.handleMessage(receivedMessage);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

	mManager.stopSendDataThread();
	mManager.stopReceiveDataThread();
	mManager.closeDevice();

}