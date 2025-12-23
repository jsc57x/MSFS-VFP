#pragma once

#include "datatypes.h"
#include <winsock2.h>
#include <Windows.h>
#include <string>

class UDPClient{
public:
    uint connect(std::string targetIPAddress, ushort port);
    void sendPlaneStatus(AircraftState* aircraftState);
    void disconnect();

private:
    SOCKET sock;

    sockaddr_in addr;
};