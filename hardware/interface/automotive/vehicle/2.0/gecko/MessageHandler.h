#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H

#include <iostream>
#include <vector>
#include "MessageParser.h"
#include "FunctionMap.h"

class MessageHandler {
public:
    MessageHandler();
    void handleMessage(const std::vector<uint8_t>& message);
    FunctionMap& getfunctionMap();
public:
    FunctionMap functionMap;
};

#endif  // MESSAGEHANDLER_H

