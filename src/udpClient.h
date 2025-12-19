#pragma once

#include "datatypes.h"
#include <winsock2.h>
#include <Windows.h>
#include <string>

class UDPClient{
public:
    u32 connect(std::string targetIPAddress, u16 port);
    void sendPlaneStatus(AircraftState* aircraftState);
    void disconnect();

private:
    SOCKET sock;

    sockaddr_in addr;
};