#ifndef PROPERTY_MANAGER_H
#define PROPERTY_MANAGER_H

#include <unordered_map>
#include <string>
#include <mutex>
#include <random>

struct Property {
    std::string name;
    std::string value = "0";
    std::string originalType;
    std::string updated = "no";
};

class PropertyManager {
private:
    std::mt19937 rng; // 随机数生成器
    std::mutex mutex; // 互斥锁

public:
    std::unordered_map<std::string, Property> properties;

    PropertyManager();
   
    void addProperty(const Property& prop);

    Property* findProperty(const std::string& name);

    void updatePropertyValue(const std::string& name, const std::string& newValue);

    void updatePropertyValues();

    size_t getPropertyCount();

    void printProperties();

    void importPropertiesFromFile(const std::string& filename);

    bool isUpdated(const std::string& name);

};

#endif // PROPERTY_MANAGER_H

