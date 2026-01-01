/*
 * Visual Flight Path for Microsoft Flight Simulator 2024
 * Copyright (c) 2026 Jens Scharmann / Fernuniversität in Hagen
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "datatypes.h"
#include "flightPathVisualizer.h"
#include "log.h"
#include "udpCommand.h"

#include <string>
#include <iostream>
#include <memory>

void FlightPathVisualizer::start(ushort serverPort, std::string targetIP, ushort targetPort)
{
    udpServer = new UDPServer();
    bool startUDPServRes = udpServer->startUDPServer(serverPort, this, targetIP, targetPort);

    if (!startUDPServRes)
    {
        Logger::logError("Failed to start UDP Connection. Abort.");
        return;
    }

    simConnectProxy = new SimConnectProxy();
    simConnectProxy->startSimConnectProxy(this);
}

void FlightPathVisualizer::handleMessage(char* message, uint length)
{
    std::unique_ptr<AbstractCommandConfiguration> command = parseIncomingMessage(message, length);

    // make sure command was parsed and is not null
    if (command == NULL) return;

    AbstractCommandConfiguration* commandConfig = command.get();

    Logger::logInfo(commandConfig->toString());
    simConnectProxy->handleCommand(commandConfig);
}

std::unique_ptr<AbstractCommandConfiguration> FlightPathVisualizer::parseIncomingMessage(char* messageContent, uint length)
{
    if (length < sizeof(ushort))
    {
        handleInvalidMessage("missing command", messageContent, length);
    }
    ushort commandID;
    memcpy(&commandID, messageContent, sizeof(ushort));

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

std::unique_ptr<SetIndicatorCommandConfiguration> FlightPathVisualizer::parseSetCommand(char* message, uint length)
{
    if (length != 56)
    {
        handleInvalidMessage("Invalid message length for Set command", message, length);
    }
    return SetIndicatorCommandConfiguration::parse(message);
}

std::unique_ptr<RemoveIndicatorsCommandConfiguration> FlightPathVisualizer::parseRemoveCommand(char* message, uint length)
{
    return RemoveIndicatorsCommandConfiguration::parse(message, length);
}

void FlightPathVisualizer::handleInvalidMessage(std::string errorMsg, char* message, uint length)
{
    Logger::logError("Received invalid message (" + errorMsg + "): " + std::string(message, length));
}

void FlightPathVisualizer::handlePlaneUpdate(AircraftState aircraftState)
{
    Logger::logInfo("Aircraft state received: Latitude: " + std::to_string(aircraftState.getLatitude()) +
        " Longitude: " + std::to_string(aircraftState.getLongitude()) +
        " Altitude: " + std::to_string(aircraftState.getAltitude()) +
        " Heading: " + std::to_string(aircraftState.getHeading()) +
        " Bank: " + std::to_string(aircraftState.getBank()) +
        " Pitch: " + std::to_string(aircraftState.getPitch()) +
        " Speed: " + std::to_string(aircraftState.getSpeed()));

    udpServer->sendPlaneStatus(aircraftState);
}

void FlightPathVisualizer::clearIndicatorMappings()
{
    simConnectProxy->resetIndicatorTypeMapping();
}

void FlightPathVisualizer::removeAllIndicators()
{
    simConnectProxy->removeAllIndicators();
}

void FlightPathVisualizer::shutdown()
{
    udpServer->stopUDPServer();
    simConnectProxy->stopSimConnectProxy();
}

