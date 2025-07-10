// SerialCommunication.cpp
#include <log/log.h>
#include <iostream>
#include <sstream>
#include <android-base/logging.h>
#include "SerialCommunication.h"

namespace android {
namespace hardware {
namespace automotive {
namespace vehicle {
namespace V2_0 {

namespace impl {

SerialCommunication* SerialCommunication::instance = new (std::nothrow) SerialCommunication();

SerialCommunication* SerialCommunication::getInstance()
{
    return instance;
}

void SerialCommunication::deleteInstance()
{
    if (instance)
    {
        delete instance;
        instance = nullptr;
    }
}


speed_t SerialCommunication::getBaudRate(int baudrate)
{
	switch(baudrate) {
        case 0: return B0;
        case 50: return B50;
        case 75: return B75;
        case 110: return B110;
        case 134: return B134;
        case 150: return B150;
        case 200: return B200;
        case 300: return B300;
        case 600: return B600;
        case 1200: return B1200;
        case 1800: return B1800;
        case 2400: return B2400;
        case 4800: return B4800;
        case 9600: return B9600;
        case 19200: return B19200;
        case 38400: return B38400;
        case 57600: return B57600;
        case 115200: return B115200;
        case 230400: return B230400;
        case 460800: return B460800;
        case 500000: return B500000;
        case 576000: return B576000;
        case 921600: return B921600;
        case 1000000: return B1000000;
        case 1152000: return B1152000;
        case 1500000: return B1500000;
        case 2000000: return B2000000;
        case 2500000: return B2500000;
        case 3000000: return B3000000;
        case 3500000: return B3500000;
        case 4000000: return B4000000;
        default: return B9600;
	}
}

int SerialCommunication::UART_Set(int fd,int baudrate,int flow_ctrl,int databits,int stopbits,int parity)  
{  
	struct termios options;  
	/*tcgetattr(fd,&options)得到与fd指向对象的相关参数，并将它们保存于options,该函数还可以测试配置是否正确，该串口是否可用等。若调用成功，函数返回值为0，若调用失败，函数返回值为1. 
    */  
	if( tcgetattr( fd,&options)  !=  0)  
	{  
		perror("SetupSerial 1");      
		return 0;   
	}  
    
    //设置串口输入波特率和输出波特率              
	cfsetispeed(&options, getBaudRate(baudrate));   
	cfsetospeed(&options, getBaudRate(baudrate));     
     
    //修改控制模式，保证程序不会占用串口  
    options.c_cflag |= CLOCAL;  
    //修改控制模式，使得能够从串口中读取输入数据  
    options.c_cflag |= CREAD;  

    options.c_iflag &= ~ICRNL; //禁止将输入的CR转换为NL

    options.c_iflag &= ~(IXON); //清bit位 关闭流控字符
    
    //设置数据流控制  
    switch(flow_ctrl)  
    {  
        
		case 0 ://不使用流控制  
              options.c_cflag &= ~CRTSCTS;  
              break;     
        
		case 1 ://使用硬件流控制  
              options.c_cflag |= CRTSCTS;  
              break;  
		case 2 ://使用软件流控制  
              options.c_cflag |= IXON | IXOFF | IXANY;  
              break;  
    }  
    //设置数据位  
    //屏蔽其他标志位  
    options.c_cflag &= ~CSIZE;  
    switch (databits)  
    {    
		case 5    :  
                     options.c_cflag |= CS5;  
                     break;  
		case 6    :  
                     options.c_cflag |= CS6;  
                     break;  
		case 7    :      
                 options.c_cflag |= CS7;  
                 break;  
		case 8:      
                 options.c_cflag |= CS8;  
                 break;    
		default:     
                 fprintf(stderr,"Unsupported data size\n");  
                 return 0;   
    }  
    //设置校验位  
    switch (parity)  
    {    
		case 'n':  
		case 'N': //无奇偶校验位。  
                 options.c_cflag &= ~PARENB;   
                 options.c_iflag &= ~INPCK;      
                 break;   
		case 'o':    
		case 'O'://设置为奇校验      
                 options.c_cflag |= (PARODD | PARENB);   
                 options.c_iflag |= INPCK;               
                 break;   
		case 'e':   
		case 'E'://设置为偶校验    
                 options.c_cflag |= PARENB;         
                 options.c_cflag &= ~PARODD;         
                 options.c_iflag |= INPCK;        
                 break;  
		case 's':  
		case 'S': //设置为空格   
                 options.c_cflag &= ~PARENB;  
                 options.c_cflag &= ~CSTOPB;  
                 break;   
        default:    
                 fprintf(stderr,"Unsupported parity\n");      
                 return 0;   
    }   
    // 设置停止位   
    switch (stopbits)  
    {    
		case 1:     
                 options.c_cflag &= ~CSTOPB; break;   
		case 2:     
                 options.c_cflag |= CSTOPB; break;  
		default:     
                       fprintf(stderr,"Unsupported stop bits\n");   
                       return 0;  
    }  
     
	//修改输出模式，原始数据输出  
	options.c_oflag &= ~OPOST;  
    
	options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);  
	//options.c_lflag &= ~(ISIG | ICANON);  
     
    //设置等待时间和最小接收字符  
    options.c_cc[VTIME] = 1; /* 读取一个字符等待1*(1/10)s */    
    options.c_cc[VMIN] = 1; /* 读取字符的最少个数为1 */  
     
    //如果发生数据溢出，接收数据，但是不再读取 刷新收到的数据但是不读  
    tcflush(fd,TCIFLUSH);  
     
    //激活配置 (将修改后的termios数据设置到串口中）  
    if (tcsetattr(fd,TCSANOW,&options) != 0)    
	{  
		perror("com set error!\n");    
		return 0;   
	}  
    return 1;   
}

void SerialCommunication::setPortname(std::string portname){
    this->portName = portname;
	std::cout << "Opening Serial device..." << this->portName.c_str() << std::endl;
}


int SerialCommunication::openDevice(int baud) {
    std::cout << "Opening Serial device..." << portName.c_str() << std::endl;
    // 具体的打开串口设备的代码
    fd = open(portName.c_str(), O_RDWR | O_NOCTTY | O_NONBLOCK);
    if(fd == -1) {
        printf("deviceTtysRead open err.\n");
        return -1;
    }

    if (UART_Set(fd,baud, 0,8,1,'N') == 0)  
	{ 
        printf("serialAttr fail.\n");                                                          
		return -1;  
	}

    std::cout << "Serial device " << portName << " opened." << std::endl;
    return 0;
}

int SerialCommunication::openDevice() {
    LOG(INFO) << "Opening Serial device..." << portName.c_str();
    // 具体的打开串口设备的代码
    fd = open(portName.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd == -1) {
        std::cerr << "Failed to open serial device: " << portName << std::endl;
        return false;
    }

    struct termios options;
    tcgetattr(fd, &options);

    // 配置串口通信参数
    cfsetispeed(&options, B1000000);
    cfsetospeed(&options, B1000000);
    options.c_cflag |= (CLOCAL | CREAD);
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    options.c_iflag &= ~(IXON | IXOFF | IXANY);
    options.c_oflag &= ~OPOST;

    tcsetattr(fd, TCSANOW, &options);

    std::cout << "Serial device " << portName << " opened." << std::endl;
    return 0;
}

void SerialCommunication::closeDevice() {
    std::cout << "Closing Serial device..." << std::endl;
    // 具体的关闭串口设备的代码
    close(fd);
}

int SerialCommunication::receiveData(std::vector<uint8_t>& buffer) {
    if (fd == -1) {
        ALOGE("Failed to open serial device");
        return -1;
    }

    int bytesRead = 0;
    std::vector<uint8_t> temp(MAX_BUFFER_SIZE);
    bytesRead = read(fd, temp.data(), MAX_BUFFER_SIZE);

    if(bytesRead > 0){
        ALOGI("receiveData size:%d", bytesRead);
        printf("deviceTtysRead  bytes_read = %d\n", bytesRead);
        buffer.assign(temp.begin(), temp.begin() + bytesRead);
    }
    return bytesRead;
}

int SerialCommunication::sendData(const std::vector<uint8_t> buffer) {
    if (fd == -1) {
        ALOGE("Failed to open serial device");
        return -1;
    }

    int bytesWrite;
    //每条数据发送3次
    for(int i = 0; i < 3; i++){
        bytesWrite = write(fd, buffer.data(), buffer.size() );
        if( bytesWrite == -1) {
            ALOGE("sendData fail size:%d", bytesWrite);
        }
        //间隔100ms
        usleep(100 * 1000);
    }
	
    return bytesWrite;
}

int SerialCommunication::SendMessage(const std::vector<uint8_t> buffer) {
    if (fd == -1) {
        ALOGE("Failed to open serial device");
        return -1;
    }

    int bytesWrite;
    bytesWrite = write(fd, buffer.data(), buffer.size() );
    if( bytesWrite == -1) {
        ALOGE("sendData fail size:%d", bytesWrite);
    }
    return bytesWrite;
}
}  // impl

}  // namespace V2_0
}  // namespace vehicle
}  // namespace automotive
}  // namespace hardware
}  // namespace android


