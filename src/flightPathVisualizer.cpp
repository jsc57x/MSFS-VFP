#include <string>
#include <iostream>
#include "datatypes.h"
#include "flightPathVisualizer.h"
#include "log.h"
#include "udpCommand.h"

void FlightPathVisualizer::start(u16 serverPort, std::string targetIP, u16 targetPort)
{
    udpServer = UDPServer();
    int startUDPServRes = udpServer.startUDPServer(serverPort, this);

}

void FlightPathVisualizer::handleMessage(char* message, u32 length)
{
    UDPCommandConfiguration* command = parseIncomingMessage(message, length);
}

UDPCommandConfiguration* FlightPathVisualizer::parseIncomingMessage(char* messageContent, u32 length)
{
    if (length < sizeof(u32))
    {
        handleInvalidMessage("missing command", messageContent, length);
    }
    u32 commandID;
    memcpy(&commandID, messageContent, sizeof(u32));

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

SetIndicatorCommandConfiguration* FlightPathVisualizer::parseSetCommand(char* message, u32 length)
{
    if (length != 64)
    {
        handleInvalidMessage("Invalid message length for Set command", message, length);
    }
    SetIndicatorCommandConfiguration command;
    command.setFromByteArray(message, length);

    return &command;
}

RemoveIndicatorsCommandConfiguration* FlightPathVisualizer::parseRemoveCommand(char* message, u32 length)
{
    return NULL;
}

void FlightPathVisualizer::handleInvalidMessage(std::string errorMsg, char* message, u32 length)
{
    Logger::logError("Received invalid message (" + errorMsg + "): " + std::string(message, length));
}

void FlightPathVisualizer::shutdown()
{
    udpServer.stopUDPServer();
}

