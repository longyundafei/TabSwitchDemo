#include <arpa/inet.h>
#include <sys/socket.h>
#include <iostream>
#include <cstring>
#include <vector>
#include <unistd.h>

int main(int argc, char *argv[]) {

  if (argc < 3) {
    std::cout << "Usage: client <ip> <data>" << std::endl;
    return 1;
  }

  // 创建socket
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  
  // 连接服务器
  sockaddr_in servaddr;
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = inet_addr(argv[1]);
  servaddr.sin_port = htons(8888);

  connect(sockfd, (sockaddr*)&servaddr, sizeof(servaddr));

  // 将十六进制字符串转换为二进制
  std::string hex = argv[2];
  std::vector<uint8_t> data;

  for (size_t i = 0; i < hex.size(); i += 2) {
    std::string byte = hex.substr(i, 2);
    data.push_back(std::stoul(byte, nullptr, 16)); 
  }

  // 发送数据
  send(sockfd, data.data(), data.size(), 0);

  close(sockfd);

  return 0;
}
