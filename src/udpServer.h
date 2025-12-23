#pragma once

#include "datatypes.h"
#include <string>
#include <winsock2.h>
#include <thread>

class UDPMessageCallback {
public:
    virtual ~UDPMessageCallback() = default;
    virtual void handleMessage(char* message, uint length) = 0;
};

class UDPServer {
public: 
    uint startUDPServer(ushort udpPort, UDPMessageCallback* callback);
    void stopUDPServer();

private:
    bool isRunning;
    std::thread serverThread;
    SOCKET sock;

    SOCKET openUDPServerSocket(ushort port);
    void closeUDPServerSocket();

    void handleSocket(UDPMessageCallback* callback);
};