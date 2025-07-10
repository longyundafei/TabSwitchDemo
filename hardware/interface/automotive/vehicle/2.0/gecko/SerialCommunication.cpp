// SerialCommunication.cpp

#include "SerialCommunication.h"

void SerialCommunication::setPortname(std::string portname){
    this->portName = portname;
	std::cout << "Opening Serial device..." << this->portName.c_str() << std::endl;
}
int SerialCommunication::openDevice() {
    std::cout << "Opening Serial device..." << portName.c_str() << std::endl;
    // 具体的打开串口设备的代码
    serialPort = open(portName.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
    if (serialPort == -1) {
        std::cerr << "Failed to open serial device: " << portName << std::endl;
        return false;
    }

    struct termios options;
    tcgetattr(serialPort, &options);

    // 配置串口通信参数
    cfsetispeed(&options, B115200);
    cfsetospeed(&options, B115200);
    options.c_cflag |= (CLOCAL | CREAD);
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    options.c_iflag &= ~(IXON | IXOFF | IXANY);
    options.c_oflag &= ~OPOST;

    tcsetattr(serialPort, TCSANOW, &options);

    std::cout << "Serial device " << portName << " opened." << std::endl;
    return 0;
}

void SerialCommunication::closeDevice() {
    std::cout << "Closing Serial device..." << std::endl;
    // 具体的关闭串口设备的代码
    close(serialPort);
}

void SerialCommunication::receiveDataThread(ReceiveQueue& receiveQueue) {
    std::vector<uint8_t> buffer(MAX_BUFFER_SIZE);
    ssize_t bytesRead = 0;
	isReceivingData = true;
    std::cout << "Starting Serial data receiving thread..." << std::endl;
    while (isReceivingData) {
        // 读取数据逻辑
        bytesRead = read(serialPort, buffer.data(), MAX_BUFFER_SIZE );
        if (bytesRead > 0) {
            // 将接收到的数据插入接收消息队列
            std::vector<uint8_t> message(buffer.begin(), buffer.begin() + bytesRead);
            receiveQueue.enqueue(message);
        }
//        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    std::cout << "Serial data receiving thread stopped." << std::endl;
}

void SerialCommunication::sendDataThread(SendQueue& sendQueue) {
    ssize_t bytesWrite;
	isSendingData = true;
    std::cout << "Starting Serial data sending thread..." << std::endl;
    while (isSendingData) {
        // 从发送消息队列获取数据并发送
        std::vector<uint8_t> message = sendQueue.dequeue();

        if (!message.empty()) {
            // 发送数据逻辑
	    bytesWrite = write(serialPort, message.data(), message.size() );
	    if( bytesWrite == -1) {
                std::cerr << "Failed to write serial data" << std::endl;
                break;
	    }
        }
    }
    std::cout << "Serial data sending thread stopped." << std::endl;
}


