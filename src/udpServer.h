#pragma once

#include "datatypes.h"
#include <string>
#include <winsock2.h>
#include <thread>

class UDPMessageCallback {
public:
    virtual ~UDPMessageCallback() = default;
    virtual void handleMessage(char* message, u32 length) = 0;
};

class UDPServer {
public: 
    int startUDPServer(u16 udpPort, UDPMessageCallback* callback);
    void stopUDPServer();

private:
    SOCKET openUDPServerSocket(u16 port);
    void closeUDPServerSocket();

    void handleSocket(UDPMessageCallback* callback);

    void printMessage(std::string message);
    void handleUDPMessage(std::string message);

    bool isRunning;
    std::thread serverThread;
    SOCKET sock;
};