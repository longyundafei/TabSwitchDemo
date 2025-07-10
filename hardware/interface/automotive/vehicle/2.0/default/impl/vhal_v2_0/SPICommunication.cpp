#include <log/log.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sstream>
#include <iomanip>

#include <linux/types.h>
#include <linux/spi/spidev.h>


#include "SPICommunication.h"

static uint32_t speed = 5000000;
static uint8_t bits = 8;
static uint16_t delay = 0;

void SPICommunication::setPortname(std::string portname){
    this->portname = portname;
}

void SPICommunication::openDevice() {
    system("echo 90 >/sys/class/gpio/export");
    system("echo out >/sys/class/gpio/gpio90/direction");
    system("echo  0 >/sys/class/gpio/gpio90/value");
    std::cout << "Opening SPI device..." << std::endl;
    fd = open(portname.c_str(), O_RDWR);
	if (fd < 0) {
		ALOGE("Opening SPI device fail portname:%s", portname.c_str());
        return;
	}

    int mode = SPI_MODE_0;
    int bits_per_word = 8;
    int speed_hz = 5000000;

    if (ioctl(fd, SPI_IOC_WR_MODE, &mode) == -1) {
        ALOGE("Failed to set SPI mode");
		return;
	}
	if (ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits_per_word) == -1) {
        ALOGE("Failed to set SPI bits per word");
		return;
	}
	if (ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed_hz) == -1) {
        ALOGE("Failed to set SPI speed");
		return;
	}

    ALOGI("Opening SPI device succed portname:%s", portname.c_str());
}

void SPICommunication::closeDevice() {
    std::cout << "Closing SPI device..." << std::endl;
    close(fd);
    system("echo  1 >/sys/class/gpio/gpio90/value");
    ALOGI("Closing SPI device succed portname:%s", portname.c_str());
}

void SPICommunication::receiveDataThread() {
    std::cout << "Starting SPI data receiving thread..." << std::endl;
    while (isReceivingData) {
        // 读取数据逻辑
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    std::cout << "SPI data receiving thread stopped." << std::endl;
}

void SPICommunication::sendDataThread() {
    std::cout << "Starting SPI data sending thread..." << std::endl;
    while (isSendingData) {
        // 从发送消息队列获取数据并发送
        std::string message;
        {
            std::lock_guard<std::mutex> lock(sendQueueMutex);
            if (!sendQueue.empty()) {
                message = sendQueue.front();
                sendQueue.pop();
            }
        }

        if (!message.empty()) {
            // 发送数据逻辑
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    std::cout << "SPI data sending thread stopped." << std::endl;
}
int SPICommunication::receiveData(std::vector<uint8_t>& buffer) {
    // if (fd == -1) {
    //     ALOGE("Failed to open spi device");
    //     return -1;
    // }

    // ALOGI("SPICommunication::receiveData");

    int bytesRead = 0;
    buffer.clear();
    // std::vector<uint8_t> temp(MAX_BUFFER_SIZE);
    // bytesRead = read(fd, temp.data(), MAX_BUFFER_SIZE);

    // unsigned char buf[32];
    // memset(buf, 0, sizeof(buf));
    // bytesRead = read(fd, buf, 32);

    // if(bytesRead > 0){
    //     ALOGI("SPICommunication::receiveData size:%d", bytesRead);
    //     printf("deviceTtysRead  bytes_read = %d\n", bytesRead);
    //     //buffer.assign(temp.begin(), temp.begin() + bytesRead);
    // }
    return bytesRead;
}
int SPICommunication::SendMessage(const std::vector<uint8_t> buffer) {
    int bytesWrite=0;
    //std::cout << "SPI send data" << buffer.size() << std::endl;
    struct spi_ioc_transfer	xfer;
    memset(&xfer, 0, sizeof(xfer));
    xfer.tx_buf = (unsigned long)buffer.data();
    xfer.len = buffer.size();

    // ALOGI("SPICommunication::SendMessage size:%d",(int)buffer.size());

    // std::string buf;
    // for (int i = 0; i < (int)buffer.size(); i++) {
    //     std::ostringstream ss;
    //     ss.clear();
    //     int data = buffer[i];
    //     ss << std::hex << data;
    //     buf += "0x";
    //     buf += ss.str();
    //     buf += ", ";
    // }
    // ALOGI("spi SendMessage buf:%s", buf.c_str());
    // buf.clear();    
    // ALOGI("spi SendMessage running here");
    int status = ioctl(fd, SPI_IOC_MESSAGE(1), xfer);
	if (status < 0) {
        ALOGI("SPI_IOC_MESSAGE");
		return bytesWrite;
	}
    return bytesWrite;
}
int SPICommunication::transferData(std::vector<uint8_t>& rbuffer , const std::vector<uint8_t> tbuffer) {
    int bytesRead  =0;
    if (fd == -1) {
        ALOGE("Failed to open spi device");
        return -1;
    }

    //uint8_t rx[25] = {0,};
    uint8_t rx[1024] = {0,}; //60*17
    rbuffer.clear();
    struct spi_ioc_transfer	xfer;
    memset(&xfer, 0, sizeof(xfer));
    xfer.tx_buf = (unsigned long)tbuffer.data();
    xfer.rx_buf = (unsigned long)rx;
    xfer.len = tbuffer.size();
    xfer.delay_usecs = delay;
    xfer.speed_hz = speed;
    xfer.bits_per_word = bits;

    // ALOGI("SPICommunication::SendMessage size:%d",(int)tbuffer.size());
    std::string tbuf;
    tbuf.clear();
    for (int i = 0; i < (int)tbuffer.size(); i++) {
        std::ostringstream ss;
        ss.clear();
        int data = tbuffer[i];
        ss << std::hex << data;
        tbuf += "0x";
        tbuf += ss.str();
        tbuf += ", ";
    }
     ALOGI("spi transfer SendMessage len:%d buffer:%s ", (int)tbuffer.size(),tbuf.c_str());

    int status = ioctl(fd, SPI_IOC_MESSAGE(1), &xfer); 
    if (status < 0) {
        ALOGI("spi transfer SPI_IOC_MESSAGE error");
        return bytesRead;
    }

    bytesRead = xfer.len;
    std::string rbuf;
    rbuf.clear();
    for (int i = 0; i < bytesRead; i++) {
        std::ostringstream ss;
        ss.clear();
        int data = rx[i];
        ss << std::hex << data;
        rbuf += "0x";
        rbuf += ss.str();
        rbuf += ", ";

        rbuffer.insert(rbuffer.end(),1,data);
    }
    ALOGI("spi transfer receiveData len:%d, buf:%s", (int)rbuffer.size(), rbuf.c_str());

#if 0 // for debug
    std::string xbuf;
    xbuf.clear();
    for (int i = 0; i < (int)rbuffer.size(); i++) {
        std::ostringstream ss;
        ss.clear();
        int data = rbuffer[i];
        ss << std::hex << data;
        xbuf += "0x";
        xbuf += ss.str();
        xbuf += ", ";
    }
    ALOGI("spi transfer check  %d, buffer:%s ", bytesRead, xbuf.c_str());
#endif

    return bytesRead;
}

int SPICommunication::transferData_V1_2(uint8_t *sendBuf, uint8_t *recvBuf,uint16_t len) 
{
   	int ret = 0;
    struct spi_ioc_transfer	xfer;
    memset(&xfer, 0, sizeof(xfer));
    xfer.tx_buf = (unsigned long)sendBuf;
    xfer.rx_buf = (unsigned long)recvBuf;
    xfer.len = len;
    xfer.delay_usecs = delay;
    xfer.speed_hz = speed;
    xfer.bits_per_word = bits;

    ret = ioctl(fd, SPI_IOC_MESSAGE(1), &xfer); 

#if 0 // for debug
    std::string xbuf;
    xbuf.clear();
    for (int i = 0; i < len; i++) {
        std::ostringstream ss;
        ss.clear();
        int data = recvBuf[i];
        ss << std::hex<< std::setw(2)<<std::setfill('0') << data;
        //xbuf += "0x";
        xbuf += ss.str();
        xbuf += " ";
    }
    ALOGI("spi     transfer check buffer:%s ", xbuf.c_str());
#endif

    if (ret < 0) {
        ALOGI("spi transfer SPI_IOC_MESSAGE error");
        return ret;
    }

    return 0;
}

int SPICommunication::spiWrite_V1_2(uint8_t *sendBuf,uint16_t len) 
{
   	int ret;
	
	ret = write(fd, sendBuf, len);//spi写操作
	if (ret < 0)
	{
		ALOGI("zwt spi write error\r\n");
		return ret;
	}
	 
	return 0;
   	
}

int SPICommunication::spiRead_V1_2(uint8_t *recvBuf,uint16_t len) 
{
	int ret;
	ret = read(fd, recvBuf, len);//spi读操作
	if (ret < 0)
	{
		ALOGI("zwt spi read error\r\n");
		return ret;
	}
 
	return 0;
}



