#ifndef DATA_SAVER_H
#define DATA_SAVER_H
#include <string>
#include <vector>

class DataSaver {
public:
    void saveToFile(BusIdentifier& busIdentifier, const std::vector<uint8_t>& data);
};
#endif