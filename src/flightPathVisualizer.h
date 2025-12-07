#pragma once

#include "datatypes.h"
#include "udpServer.h"
#include "udpCommand.h"

#include <string>

class FlightPathVisualizer : public UDPMessageCallback{
public:
    void start(u16 serverPort, std::string targetIP, u16 targetPort);
    void shutdown();

    void handleMessage(char* message, u32 length) override;

    void handleInvalidMessage(std::string errorMsg, char* message, u32 length);

private:
    UDPServer udpServer;

    UDPCommandConfiguration* parseIncomingMessage(char* array, u32 length);
    SetIndicatorCommandConfiguration* parseSetCommand(char* array, u32 length);
    RemoveIndicatorsCommandConfiguration* parseRemoveCommand(char* array, u32 length);
};

