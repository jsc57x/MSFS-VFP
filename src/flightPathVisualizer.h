#pragma once

#include "datatypes.h"
#include "udpServer.h"
#include "udpCommand.h"
#include "simConnectProxy.h"

#include <string>

class FlightPathVisualizer : public UDPMessageCallback, public SimConnectCallback{
public:
    void start(u16 serverPort, std::string targetIP, u16 targetPort);
    void shutdown();

    void handleMessage(char* message, u32 length) override;
    void handlePlaneUpdate(/*WorldPosition planePos*/) override;

    void handleInvalidMessage(std::string errorMsg, char* message, u32 length);

private:
    UDPServer* udpServer;
    SimConnectProxy* simConnectProxy;

    UDPCommandConfiguration* parseIncomingMessage(char* array, u32 length);
    SetIndicatorCommandConfiguration* parseSetCommand(char* array, u32 length);
    RemoveIndicatorsCommandConfiguration* parseRemoveCommand(char* array, u32 length);
};

