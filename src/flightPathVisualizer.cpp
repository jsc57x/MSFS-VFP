#include "datatypes.h"
#include "flightPathVisualizer.h"
#include "log.h"
#include "udpCommand.h"

#include <string>
#include <iostream>
#include <memory>

void FlightPathVisualizer::start(u16 serverPort, std::string targetIP, u16 targetPort)
{
    udpServer = new UDPServer();
    int startUDPServRes = udpServer->startUDPServer(serverPort, this);
    //TODO Result auswerten

    simConnectProxy = new SimConnectProxy(this);
}

void FlightPathVisualizer::handleMessage(char* message, u32 length)
{
    std::unique_ptr<UDPCommandConfiguration> command = parseIncomingMessage(message, length);

    // make sure command was parsed and is not null
    if (command == NULL) return;

    UDPCommandConfiguration* commandConfig = command.get();

    Logger::logInfo(commandConfig->toString());
    simConnectProxy->handleCommand(commandConfig);
}

std::unique_ptr<UDPCommandConfiguration> FlightPathVisualizer::parseIncomingMessage(char* messageContent, u32 length)
{
    if (length < sizeof(u16))
    {
        handleInvalidMessage("missing command", messageContent, length);
    }
    u16 commandID;
    memcpy(&commandID, messageContent, sizeof(u16));

    // this has to be improved if there are more than two commands
    if (commandID != 1 && commandID != 2)
    {
        handleInvalidMessage("invalid command id " + std::to_string(commandID), messageContent, length);
    }

    // these should be constants
    if (commandID == 1)
    {
        return parseSetCommand(messageContent, length);
    }
    else if (commandID == 2)
    {
        return parseRemoveCommand(messageContent, length);
    }

    return NULL;
}

std::unique_ptr<SetIndicatorCommandConfiguration> FlightPathVisualizer::parseSetCommand(char* message, u32 length)
{
    if (length != 56)
    {
        handleInvalidMessage("Invalid message length for Set command", message, length);
    }
    return SetIndicatorCommandConfiguration::parse(message);
}

std::unique_ptr<RemoveIndicatorsCommandConfiguration> FlightPathVisualizer::parseRemoveCommand(char* message, u32 length)
{
    return RemoveIndicatorsCommandConfiguration::parse(message, length);
}

void FlightPathVisualizer::handleInvalidMessage(std::string errorMsg, char* message, u32 length)
{
    Logger::logError("Received invalid message (" + errorMsg + "): " + std::string(message, length));
}

void FlightPathVisualizer::handlePlaneUpdate()
{
    Logger::logInfo("Plane update received");
}

void FlightPathVisualizer::shutdown()
{
    udpServer->stopUDPServer();
    simConnectProxy->disconnect();
}

