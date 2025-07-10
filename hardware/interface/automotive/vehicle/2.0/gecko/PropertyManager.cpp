#include "PropertyManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <chrono>

PropertyManager::PropertyManager() {
    std::random_device rd;
    rng.seed(rd()); 
}

void PropertyManager::addProperty(const Property& prop) {
        std::lock_guard<std::mutex> lock(mutex);
        properties[prop.name] = prop;
}

Property* PropertyManager::findProperty(const std::string& name) {
        std::lock_guard<std::mutex> lock(mutex);
        auto it = properties.find(name);
        if (it != properties.end()) {
            return &(it->second);
        }
        return nullptr;
}

void PropertyManager::updatePropertyValue(const std::string& name, const std::string& newValue) {
        std::lock_guard<std::mutex> lock(mutex);
        auto it = properties.find(name);
        if (it != properties.end()) {
            it->second.value = newValue;
            it->second.updated = "yes";
        }
}

void PropertyManager::updatePropertyValues() {
        std::uniform_int_distribution<size_t> numDistribution(3, 5);
        std::uniform_int_distribution<int> valueDistribution(1, 10);

        std::lock_guard<std::mutex> lock(mutex);
        size_t numPropertiesToUpdate = numDistribution(rng);
        size_t numPropertiesUpdated = 0;

        for (auto& pair : properties) {
            Property& prop = pair.second;
            if (numPropertiesUpdated >= numPropertiesToUpdate) {
                break;
            }
            if (prop.updated == "no") {
                prop.value = std::to_string(std::stoi(prop.value) + valueDistribution(rng));
                prop.updated = "yes";
                numPropertiesUpdated++;
            }
        }
}

size_t PropertyManager::getPropertyCount() {
        std::lock_guard<std::mutex> lock(mutex);
        return properties.size();
}

void PropertyManager::printProperties() {
        std::lock_guard<std::mutex> lock(mutex);
        for (const auto& pair : properties) {
            const Property& prop = pair.second;
            std::cout << "Name: " << prop.name; 
            std::cout << " Value: " << prop.value;
            std::cout << " Original Type: " << prop.originalType;
            std::cout << " Updated: " << prop.updated << std::endl;
        }
}

void PropertyManager::importPropertiesFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string name;
            std::string originalType;
            if (std::getline(ss, name, ';') && std::getline(ss, originalType)) {
                // 移除字段中的空格
                name.erase(std::remove_if(name.begin(), name.end(), ::isspace), name.end());
                originalType.erase(std::remove_if(originalType.begin(), originalType.end(), ::isspace), originalType.end());

                // 创建属性并添加到管理器
                Property prop;
                prop.name = name;
                prop.originalType = originalType;
                addProperty(prop);
            }
        }
        file.close();
    }
}

bool PropertyManager::isUpdated(const std::string& name) {

  std::lock_guard<std::mutex> lock(mutex);

  auto it = properties.find(name);
  if (it != properties.end()) {
    return it->second.updated == "yes"; 
  }

  return false;

}
