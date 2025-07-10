#ifndef MESSAGEQUEUE_H
#define MESSAGEQUEUE_H

#include <queue>
#include <vector>
#include <mutex>
#include <condition_variable>

class MessageQueue {
public:
  void enqueue(const std::vector<uint8_t>& message);
  std::vector<uint8_t> dequeue(); 
  std::vector<uint8_t> noCheckdequeue(); 
  std::vector<uint8_t> dequeue(int timeoutMs); 
  size_t getQueueSize();
  int WaitQueue(int timeoutMs);
  std::vector<uint8_t> getMsgNoPop();
  int cvdequeue(int timeoutMs);
  void notifydequeue();
  void nocvenqueue(const std::vector<uint8_t>& message);
  void clear();
private:
  std::queue<std::vector<uint8_t>> queue_;
  std::mutex mutex_;
  std::condition_variable cv_;  
  std::condition_variable decv_;  
};

#endif