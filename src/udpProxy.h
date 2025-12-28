#pragma once

#include "datatypes.h"
#include "aircraftState.h"
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
    uint startUDPServer(ushort udpPort, UDPMessageCallback* callback, std::string targetIPAddress, ushort targetPort);
    void sendPlaneStatus(AircraftState* aircraftState);
    void stopUDPServer();

private:
    bool isRunning;
    std::thread serverThread;
    SOCKET sock;

    sockaddr_in targetAddr;

    SOCKET openUDPServerSocket(ushort port);
    void closeUDPServerSocket();

    void handleSocket(UDPMessageCallback* callback);
};