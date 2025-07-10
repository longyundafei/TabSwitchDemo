#ifndef BINARYMESSAGE_H
#define BINARYMESSAGE_H

#include <chrono>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <dirent.h>

class BinaryMessage {
  public:
    void appendMessage(const std::vector<uint8_t>& message,bool sample);
    void appendSaveMessage(const std::vector<uint8_t>& message);
    std::vector<std::vector<uint8_t>> readMessages(bool sample);
    std::string convertToHex(const std::vector<uint8_t>& data);
    std::vector<uint8_t> convertToBinary(const std::string& hexString);
    std::string generateFilenameWithDate();
    std::string generateFilenameWithDate7Ago();
    void checkAndDeleteFile();
    void checkAndCreateDir();
    void deleteMessageFileBefor7Days();
    int sumDays(int year, int mon, int day);
    int checkMsgDateOutof7Days(uint8_t s_year, uint8_t s_mon, uint8_t s_day);
};

#endif