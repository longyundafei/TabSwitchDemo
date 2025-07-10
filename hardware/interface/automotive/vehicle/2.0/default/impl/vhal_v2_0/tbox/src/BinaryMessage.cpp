#include "BinaryMessage.h"
#include <android-base/logging.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstdio>
#include <fstream>
#define MESSAGE_FILE "/data/vendor/tbox/message.txt"
#define MESSAGE_FILE_SAMPLE "/data/vendor/tbox/messageSample.txt"
std::string BinaryMessage::convertToHex(const std::vector<uint8_t>& data) {
    std::ostringstream oss;
    oss << std::hex << std::setfill('0');
    for (uint8_t byte : data) {
        oss << std::setw(2) << static_cast<int>(byte);
    }
    return oss.str();
}
void BinaryMessage::appendMessage(const std::vector<uint8_t>& message,bool sample) {
    LOG(INFO) << __func__;
    std::string hex_str;
    // 二进制转十六进制
    hex_str = convertToHex(message);
    // std::cout << hex_str << std::endl;
    if(!sample){
        std::ofstream file(MESSAGE_FILE, std::ios::app);
        file << hex_str << "\r\n";
    }else{
        std::ofstream file(MESSAGE_FILE_SAMPLE, std::ios::app);
        file << hex_str << "\r\n";
    }
}
std::vector<std::vector<uint8_t>> BinaryMessage::readMessages(bool sample) {
    std::vector<std::vector<uint8_t>> result;
    if(!sample){
        std::ifstream file(MESSAGE_FILE);
        std::string line;
        //    int count = 0; // 计数器
        while (std::getline(file, line)) {
            int ret = line.compare(0, 4, "2323");
            LOG(INFO) << "readMessages----" << line;
            if (ret == 0){
                std::vector<uint8_t> data;
                // 十六进制字符串转二进制
                data = convertToBinary(line);
                //std::cout << line << "----" << data.size() << std::endl;
                LOG(INFO) << "readMessages----" << line << "----" << data.size();
                result.push_back(data);
            } else {
                LOG(INFO) << "readMessages---- got null" << line;
            }
        }
    }else{
        std::ifstream file(MESSAGE_FILE_SAMPLE);
        std::string line;
        //    int count = 0; // 计数器
        while (std::getline(file, line)) {
            std::vector<uint8_t> data;
            // 十六进制字符串转二进制
            data = convertToBinary(line);
            //std::cout << line << "----" << data.size() << std::endl;
            LOG(INFO) << line << "----" << data.size();
            result.push_back(data);
        }
    }

    int res = 0;
    if(!sample){
        res = std::remove(MESSAGE_FILE);
    }else{
        res = std::remove(MESSAGE_FILE_SAMPLE);
    }

    if (res != 0) {
    } else {
        //std::cout << "Failed to delete the file" << std::endl;
        LOG(INFO) << "Failed to delete the file";
    }
    return result;
}
std::vector<uint8_t> BinaryMessage::convertToBinary(const std::string& hexString) {
    std::vector<uint8_t> binaryData;
    std::string byteString;
    for (size_t i = 0; i < hexString.length() - 2; i += 2) {
        byteString = hexString.substr(i, 2);  // 获取每两个字符组成的字节
        // std::cout << "--" << byteString << std::endl;
        uint8_t byte =
                static_cast<uint8_t>(std::stoi(byteString, nullptr, 16));  // 将字节转换为整数
        binaryData.push_back(byte);  // 添加到二进制数据中
    }
    return binaryData;
}

std::string BinaryMessage::generateFilenameWithDate() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::ostringstream ss;
    ss << std::put_time(std::localtime(&in_time_t),
                        "%Y%m%d");  // Use %Y for year, %m for month, %d for day

    return "/data/vendor/tbox/message" + ss.str() + ".txt";  // Append file extension
}
std::string BinaryMessage::generateFilenameWithDate7Ago() {
    auto now = std::chrono::system_clock::now();
    // Subtract 7 days from the current time
    auto sevenDaysAgo = now - std::chrono::hours(24 * 7);

    auto in_time_t = std::chrono::system_clock::to_time_t(sevenDaysAgo);

    std::ostringstream ss;
    ss << std::put_time(std::localtime(&in_time_t),
                        "%Y%m%d");  // Use %Y for year, %m for month, %d for day

    return "/data/vendor/tbox/message" + ss.str() + ".txt";  // Append file extension
}
void BinaryMessage::checkAndDeleteFile() {
    std::string filename = generateFilenameWithDate7Ago();
    std::ifstream file(filename);

    if (file.good()) {                  // If the file exists
        file.close();                   // Close the file stream before deleting
        std::remove(filename.c_str());  // Convert string to C-string for remove()
    }
}
void BinaryMessage::appendSaveMessage(const std::vector<uint8_t>& message) {
    using namespace std::chrono;
    LOG(INFO) << __func__;
    std::string hex_str;
    // 二进制转十六进制
    hex_str = convertToHex(message);
    // std::cout << hex_str << std::endl;
    std::string filename = generateFilenameWithDate();
    std::ofstream file(filename.c_str(), std::ios::app);
    // 获取当前时间
    system_clock::time_point now = system_clock::now();
    // 转换为系统时间
    std::time_t t = system_clock::to_time_t(now);
    std::tm tm = *std::localtime(&t);
    // 提取毫秒
    milliseconds ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;
    // 创建输出流并设置格式
    std::ostringstream oss;
    oss << std::put_time(&tm, "%m-%d %H:%M:%S");
    oss << '.' << std::setfill('0') << std::setw(3) << ms.count();
    file << oss.str() << " " << hex_str << "\r\n";
}

void BinaryMessage::checkAndCreateDir() {
    std::string dirPath = "/data/vendor/tbox/";
    struct stat info;

    if (stat(dirPath.c_str(), &info) != 0) {      // Check if directory exists
        if (mkdir(dirPath.c_str(), 0755) == 0) {  // Create new directory
            LOG(INFO) << __func__;                // Directory created successfully
        }
    }
}

void BinaryMessage::deleteMessageFileBefor7Days(){
    DIR* dir = opendir("/data/vendor/tbox");
    if(dir == nullptr){
        LOG(INFO) << "ERROR : deleteMessageFileBefor7Days /data/vendor/tbox donot exist";
    }
    struct dirent* ptr = nullptr;
    std::string filename;
    while((ptr=readdir(dir))!=nullptr){
        filename = ptr->d_name;
        LOG(INFO) << "d_name: " <<filename;
        std::string::size_type idx;
        idx=filename.find("message");
        if((idx != std::string::npos) && (filename.size() == 19)){
            int year = std::stoi(std::string(1, ptr->d_name[9])) * 10
                       + std::stoi(std::string(1, ptr->d_name[10]));
            int mon = std::stoi(std::string(1, ptr->d_name[11])) * 10
                      + std::stoi(std::string(1, ptr->d_name[12]));
            int day = std::stoi(std::string(1, ptr->d_name[13])) * 10
                      + std::stoi(std::string(1, ptr->d_name[14]));
            int ret = checkMsgDateOutof7Days(static_cast<uint8_t>(year), static_cast<uint8_t>(mon), static_cast<uint8_t>(day));
            if(ret == 1){
                filename = "/data/vendor/tbox/"+filename;
                std::remove(filename.c_str());
                LOG(INFO) <<"remove "<< filename.c_str() ;
            }
        }
    }
    closedir(dir);
}

int BinaryMessage::sumDays(int year, int mon, int day) {
    int sumDays = 0;
    switch(mon) {
        case 1:
            sumDays = day;
        break;
        case 2:
            sumDays = day+31;
        break;
        case 3:
            //统一按非闰年计算
            sumDays = day+59;
        break;
        case 4:
            sumDays = day+90;
        break;
        case 5:
            sumDays = day+120;
        break;
        case 6:
            sumDays = day+151;
        break;
        case 7:
            sumDays = day+181;
        break;
        case 8:
            sumDays = day+212;
        break;
        case 9:
            sumDays = day+243;
        break;
        case 10:
            sumDays = day+273;
        break;
        case 11:
            sumDays = day+304;
        break;
        case 12:
            sumDays = day+334;
        break;
    }
    if (year%4 == 0){
        sumDays++;
    }
    return sumDays;
}

int BinaryMessage::checkMsgDateOutof7Days(uint8_t s_year, uint8_t s_mon, uint8_t s_day) {
    // 获取当前时间
    std::time_t now = std::time(nullptr);
    std::tm* timeinfo = std::localtime(&now);

    int sYear = static_cast<int>(s_year)+2000;
    int sMon = static_cast<int>(s_mon);
    int sDay = static_cast<int>(s_day);
    int eYear = static_cast<int>(timeinfo->tm_year % 100)+2000;
    //LOG(INFO) << "timeinfo->tm_year" << timeinfo->tm_year;
    int eMon = static_cast<int>(timeinfo->tm_mon + 1);
    //LOG(INFO) << "timeinfo->tm_mon" << timeinfo->tm_mon;
    int eDay = static_cast<int>(timeinfo->tm_mday);
    int sSumDays = 0;
    int eSumDays = 0;
    if (sYear == eYear){
        sSumDays = sumDays(sYear, sMon, sDay);
        eSumDays = sumDays(eYear, eMon, eDay);
        int diff = eSumDays-sSumDays;
        if ((diff < 0) || (diff > 7)) {
            return 1;
        } else {
            return 0;
        }
    } else if ((sYear+1) == eYear) {
        if((sMon != 12) || (eMon != 1)){
            return 1;
        }
        int diff = eDay + 32 - sDay;
        if(diff > 7){
            return 1;
        } else {
            return 0;
        }

    } else {
       return 1;
    }
}