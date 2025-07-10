// SerialCommunication.h

#ifndef SERIAL_COMMUNICATION_H
#define SERIAL_COMMUNICATION_H

#include <iostream>
#include <string>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <termios.h>

namespace android {
namespace hardware {
namespace automotive {
namespace vehicle {
namespace V2_0 {

namespace impl {

#define MAX_BUFFER_SIZE 1024

class SerialCommunication {
public:
    SerialCommunication(){}
    static SerialCommunication* getInstance();
    static void deleteInstance();

    int openDevice(int baud);
    int openDevice();
    void closeDevice();
    int receiveData(std::vector<uint8_t>& buffer);
    int sendData(const std::vector<uint8_t> buffer);
    int SendMessage(const std::vector<uint8_t> buffer);
    void setPortname(std::string portname);

;
private:
//    SerialCommunication(const SerialCommunication&) = delete;
//    SerialCommunication& operator=(const SerialCommunication&) = delete;

private:
    speed_t getBaudRate(int baudrate);
    int UART_Set(int fd,int baudrate,int flow_ctrl,int databits,int stopbits,int parity);

private:
    int fd;
    std::string portName;
    static SerialCommunication* instance;
};


}  // impl

}  // namespace V2_0
}  // namespace vehicle
}  // namespace automotive
}  // namespace hardware
}  // namespace android
#endif  // SERIAL_COMMUNICATION_H

