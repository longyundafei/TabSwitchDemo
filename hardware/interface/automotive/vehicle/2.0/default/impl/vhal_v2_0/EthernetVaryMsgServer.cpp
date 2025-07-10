// EthernetVaryMsgServer.cpp
// 以太网服务端，周期性发送 processVaryMsg 可解析的 0x01 类型报文
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <cstring>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <mutex>
#include <set>

#define SERVER_PORT 12345
#define MAX_CLIENTS 10

std::set<int> client_fds;
std::mutex client_mutex;

std::vector<uint8_t> make_vary_msg() {
    // 用户指定的原文报文
    uint8_t raw_msg[] = {
//        0x02,0x01,0x01,0x64,0x00,0x02,0x01,0xa0,0x08,0x00,0x00,0x0f,0x0c,0x23,0x04,0x08,0x00,0x02,0x01,0x06,0x30,0x00,0x00,0xff,0xf0,0x00,0xff,0xc0,0x00,0x00,0x00,0xff,0xb2,0xc0,0x00,0x00,0x00,0x00,0x00,0x62,0x00,0x07,0x00,0x84,0xfe,0x00,0x00,0x82,0x16,0x15,0x00,0x00,0x00,0x00,0x00,0x80,0x00,0x07,0x00,0x00,0x00,0x00,0x00,0x14,0x00,0x00,0x00,0x00,0x00,0x02,0x02,0x6e,0x08,0xff,0xff,0xff,0x00,0x91,0x00,0x01,0x00,0x04,0x00,0xf1,0x08,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x00,0x04,0x00,0xf2,0x08,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x00,0x6a
    0x02, 0x01, 0x01, 0xb4, 0x00, 0x02, 0x00, 0x71,
    0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x02, 0x00, 0x82, 0x08, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x02, 0x00, 0xa1, 0x10, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02,
    0x00, 0xa2, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x02, 0x00, 0xf1, 0x08, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0xf2, 0x08,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00,
    0xf4, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x02, 0x01, 0x06, 0x30, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x02, 0x01, 0x1d, 0x08, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xa8
    };
    return std::vector<uint8_t>(raw_msg, raw_msg + sizeof(raw_msg)/sizeof(raw_msg[0]));
}


std::atomic<int> g_send_period_ms(1000);

void recv_loop() {
    const size_t BUF_SIZE = 4096;
    uint8_t buf[BUF_SIZE];
    while (true) {
        std::set<int> fds_copy;
        {
            std::lock_guard<std::mutex> lock(client_mutex);
            fds_copy = client_fds;
        }
        for (int fd : fds_copy) {
            ssize_t ret = recv(fd, buf, BUF_SIZE, MSG_DONTWAIT);
            if (ret > 0) {
                printf("[EthernetVaryMsgServer] Recv from fd=%d, len=%zd, head=", fd, ret);
                size_t print_len = ret < 10 ? ret : 10;
                for (size_t i = 0; i < print_len; ++i) {
                    printf("%02X ", buf[i]);
                }
                printf("...\n");
                fflush(stdout);
            } else if (ret == 0 || (ret < 0 && errno != EAGAIN && errno != EWOULDBLOCK)) {
                std::lock_guard<std::mutex> lock(client_mutex);
                client_fds.erase(fd);
                close(fd);
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}


void send_loop() {
    while (true) {
        std::vector<uint8_t> msg = make_vary_msg();
        std::set<int> fds_copy;
        {
            std::lock_guard<std::mutex> lock(client_mutex);
            fds_copy = client_fds;
        }
        for (int fd : fds_copy) {
            ssize_t ret = send(fd, msg.data(), msg.size(), 0);
            if (ret <= 0) {
                std::lock_guard<std::mutex> lock(client_mutex);
                client_fds.erase(fd);
                close(fd);
            }
        }
        std::cout << "[EthernetVaryMsgServer] Send msg, len=" << msg.size() << ", head=";
        for (size_t i = 0; i < std::min<size_t>(8, msg.size()); ++i) {
            printf("%02X ", msg[i]);
        }
        std::cout << "..." << std::endl;
        std::cout.flush();
        std::this_thread::sleep_for(std::chrono::milliseconds(g_send_period_ms.load()));
    }
}



void server_main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        std::cerr << "Failed to create socket\n";
        return;
    }
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(SERVER_PORT);
    if (bind(server_fd, (sockaddr*)&addr, sizeof(addr)) < 0) {
        std::cerr << "Bind failed, errno=" << errno << std::endl;
        close(server_fd);
        return;
    }
    listen(server_fd, MAX_CLIENTS);
    std::cout << "Server listening on port " << SERVER_PORT << std::endl;
    std::cout.flush();

    std::thread(send_loop).detach();
    std::thread(recv_loop).detach();

    std::cout << "输入新的周期(毫秒)并回车可随时调整发送周期。例如输入500回车设置为0.5秒：" << std::endl;
    std::cout.flush();

    std::thread([&](){
        while (true) {
            int period;
            std::cin >> period;
            if(std::cin.fail()) {
                std::cin.clear();
                std::string dummy;
                std::cin >> dummy;
                std::cout << "输入无效，请输入整数毫秒周期！" << std::endl;
                continue;
            }
            if (period < 10) period = 10;
            g_send_period_ms.store(period);
            std::cout << "已设置发送周期为 " << period << " ms" << std::endl;
        }
    }).detach();

    while (true) {
        sockaddr_in client_addr{};
        socklen_t client_len = sizeof(client_addr);
        int client_fd = accept(server_fd, (sockaddr*)&client_addr, &client_len);
        if (client_fd >= 0) {
            char ipstr[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &client_addr.sin_addr, ipstr, sizeof(ipstr));
            int cport = ntohs(client_addr.sin_port);
            {
                std::lock_guard<std::mutex> lock(client_mutex);
                client_fds.insert(client_fd);
            }
            std::cout << "New client: fd=" << client_fd << ", IP=" << ipstr << ", port=" << cport << std::endl;
            std::cout.flush();
        } else {
            std::cerr << "accept() failed, errno=" << errno << std::endl;
            std::cerr.flush();
        }
    }
}



int main() {
    server_main();
    return 0;
}
