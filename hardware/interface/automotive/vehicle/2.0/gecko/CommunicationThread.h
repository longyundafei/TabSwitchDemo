#ifndef COMMUNICATION_THREAD_H
#define COMMUNICATION_THREAD_H

#include "CommunicationManager.h"
#include "MessageHandler.h"
#include <thread>

class CommunicationThread {

public:

  CommunicationThread(const std::string& port);

  ~CommunicationThread();

  MessageHandler* getHandler(); 


private:

  void ThreadFunc();

  std::thread m_thread;

  std::string mPortname;
  CommunicationManager mManager;
  MessageHandler mHandler;

};

#endif // COMMUNICATION_THREAD_H

