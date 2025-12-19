#pragma once

#include "datatypes.h"
#include "udpServer.h"
#include "udpCommand.h"
#include "udpClient.h"
#include "simConnectProxy.h"

#include <string>

class FlightPathVisualizer : public UDPMessageCallback, public SimConnectCallback{
public:
    void start(u16 serverPort, std::string targetIP, u16 targetPort);
    void shutdown();

    void handleMessage(char* message, u32 length) override;
    void handlePlaneUpdate(AircraftState* aircraftState) override;

    void handleInvalidMessage(std::string errorMsg, char* message, u32 length);

private:
    UDPServer* udpServer;
    UDPClient* udpClient;
    SimConnectProxy* simConnectProxy;

    std::unique_ptr<UDPCommandConfiguration> parseIncomingMessage(char* array, u32 length);
    std::unique_ptr<SetIndicatorCommandConfiguration> parseSetCommand(char* array, u32 length);
    std::unique_ptr<RemoveIndicatorsCommandConfiguration> parseRemoveCommand(char* array, u32 length);
};

