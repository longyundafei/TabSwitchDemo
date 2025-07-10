#ifndef LOCKMAP_H
#define LOCKMAP_H
#include <map>
#include <mutex>

template<typename Key, typename Value>
class LockMap {
private:
    std::map<Key, Value> internalMap;
    mutable std::mutex mapMutex;

public:
    LockMap& operator=(const LockMap& other);
    Value& operator[](const Key& key);
    const Value& operator[](const Key& key) const;
    std::size_t size() const;  // 添加 size 函数
    typename std::map<Key, Value>::iterator begin();  // 添加 begin 函数
    typename std::map<Key, Value>::const_iterator begin() const;  // 添加 const 版本的 begin 函数
    typename std::map<Key, Value>::iterator end();  // 添加 end 函数
    typename std::map<Key, Value>::const_iterator end() const;  // 添加 const 版本的 end 函数
};

template<typename Key, typename Value>
LockMap<Key, Value>& LockMap<Key, Value>::operator=(const LockMap& other) {
    if (&other != this) {
        std::lock_guard<std::mutex> lock(mapMutex);
        internalMap = other.internalMap;
    }
    return *this;
}

template<typename Key, typename Value>
Value& LockMap<Key, Value>::operator[](const Key& key) {
    std::lock_guard<std::mutex> lock(mapMutex);
    return internalMap[key];
}

template<typename Key, typename Value>
const Value& LockMap<Key, Value>::operator[](const Key& key) const {
    std::lock_guard<std::mutex> lock(mapMutex);
    return internalMap.at(key);
}

template<typename Key, typename Value>
std::size_t LockMap<Key, Value>::size() const {
    std::lock_guard<std::mutex> lock(mapMutex);
    return internalMap.size();
}
template<typename Key, typename Value>
typename std::map<Key, Value>::iterator LockMap<Key, Value>::begin() {
    std::lock_guard<std::mutex> lock(mapMutex);
    return internalMap.begin();
}

template<typename Key, typename Value>
typename std::map<Key, Value>::const_iterator LockMap<Key, Value>::begin() const {
    std::lock_guard<std::mutex> lock(mapMutex);
    return internalMap.begin();
}
template<typename Key, typename Value>
typename std::map<Key, Value>::iterator LockMap<Key, Value>::end() {
    std::lock_guard<std::mutex> lock(mapMutex);
    return internalMap.end();
}

template<typename Key, typename Value>
typename std::map<Key, Value>::const_iterator LockMap<Key, Value>::end() const {
    std::lock_guard<std::mutex> lock(mapMutex);
    return internalMap.end();
}
#endif