/*
 * Copyright (C) 2019 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <android-base/logging.h>
#include "GrpcVehicleServer.h"
#include "CommunicationThread.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: ./communication_program <portname>" << std::endl;
        return 1;
    }

    namespace vhal_impl = android::hardware::automotive::vehicle::V2_0::impl;
    std::string portname = argv[1];
    CommunicationThread* comthread = new CommunicationThread(portname);
    //CommunicationThread communicationThread(portname);
//    std::thread thread(std::ref(communicationThread));
//    std::thread thread(std::ref(pThread));
//    std::thread thread(pThread);
    auto server = vhal_impl::makeGrpcVehicleServer("127.0.0.1:50051",comthread);
//    server->setmCommThread(comthread);
    server->Start().Wait();
//    thread.join();  // Wait for the communication thread to finish
    return 0;
}
