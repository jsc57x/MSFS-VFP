#pragma once

#include "datatypes.h"
#include "udpServer.h"
#include "udpCommand.h"
#include "udpClient.h"
#include "simConnectProxy.h"

#include <string>

class FlightPathVisualizer : public UDPMessageCallback, public SimConnectCallback{
public:
    void start(ushort serverPort, std::string targetIP, ushort targetPort);
    void shutdown();

    void handleMessage(char* message, uint length) override;
    void handlePlaneUpdate(AircraftState* aircraftState) override;

    void handleInvalidMessage(std::string errorMsg, char* message, uint length);

private:
    UDPServer* udpServer;
    UDPClient* udpClient;
    SimConnectProxy* simConnectProxy;

    std::unique_ptr<UDPCommandConfiguration> parseIncomingMessage(char* array, uint length);
    std::unique_ptr<SetIndicatorCommandConfiguration> parseSetCommand(char* array, uint length);
    std::unique_ptr<RemoveIndicatorsCommandConfiguration> parseRemoveCommand(char* array, uint length);
};

