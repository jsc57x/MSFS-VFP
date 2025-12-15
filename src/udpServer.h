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
    u32 startUDPServer(u16 udpPort, UDPMessageCallback* callback);
    void stopUDPServer();

private:
    bool isRunning;
    std::thread serverThread;
    SOCKET sock;

    SOCKET openUDPServerSocket(u16 port);
    void closeUDPServerSocket();

    void handleSocket(UDPMessageCallback* callback);

    void printMessage(std::string message);
    void handleUDPMessage(std::string message);
};