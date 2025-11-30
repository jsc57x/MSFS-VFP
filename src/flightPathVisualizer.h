#pragma once

#include "datatypes.h"
#include "udpServer.h"

#include <string>

class FlightPathVisualizer : public UDPMessageCallback{
public:
    void start(u16 serverPort, std::string targetIP, u16 targetPort);
    void shutdown();

    void handleMessage(char* message, int length) override;

private:
    UDPServer udpServer;
};

