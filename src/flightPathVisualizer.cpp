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
#include "numberUtils.h"

#include <string>
#include <iostream>
#include <memory>

void FlightPathVisualizer::start(ushort serverPort, std::string targetIP, ushort targetPort)
{
    udpProxy = new UDPProxy();
    bool startUDPServRes = udpProxy->startUDPProxy(serverPort, this, targetIP, targetPort);

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
    std::unique_ptr<AbstractCommandConfiguration> command = nullptr;
    try {
        command = CommandConfigurationParser::parse(message, length);
    }
    catch (std::invalid_argument e)
    {
        if (strcmp(e.what(), "missing_command") == 0)
        {
            Logger::logError("Received invalid message (missing command): " + std::string(message, length));
        } else if (strcmp(e.what(), "unknown_command") == 0)
        {
            Logger::logError("Received invalid message (invalid command id " + std::to_string(readUShortNetworkByteOrder(message)) + "): " + std::string(message, length));
        } else if (strcmp(e.what(), "set_invalid_length") == 0)
        {
            Logger::logError("Received invalid message (Invalid message length for Set command): " + std::string(message, length));
        } else if (strcmp(e.what(), "remove_invalid_length") == 0)
        {
            Logger::logError("Received invalid message (Invalid message length for Remove command): " + std::string(message, length));
        }
    }

    // make sure command was parsed and is not null
    if (command == nullptr) return;

    AbstractCommandConfiguration* commandConfig = command.get();

    Logger::logInfo(commandConfig->toString());
    simConnectProxy->handleCommand(commandConfig);
}

void FlightPathVisualizer::handleAircraftStateUpdate(AircraftState aircraftState)
{
    Logger::logInfo("Aircraft state received: Latitude: " + std::to_string(aircraftState.getLatitude()) +
        " Longitude: " + std::to_string(aircraftState.getLongitude()) +
        " Altitude: " + std::to_string(aircraftState.getAltitude()) +
        " Heading: " + std::to_string(aircraftState.getHeading()) +
        " Bank: " + std::to_string(aircraftState.getBank()) +
        " Pitch: " + std::to_string(aircraftState.getPitch()) +
        " Speed: " + std::to_string(aircraftState.getSpeed()));

    int contentLength = 56;
    char* rawContent = new char[contentLength] {};

    writeDoubleInNetworkByteOrder(aircraftState.getLatitude(), rawContent);
    writeDoubleInNetworkByteOrder(aircraftState.getLongitude(), rawContent + 8);
    writeDoubleInNetworkByteOrder(aircraftState.getAltitude(), rawContent + 16);
    writeDoubleInNetworkByteOrder(aircraftState.getHeading(), rawContent + 24);
    writeDoubleInNetworkByteOrder(aircraftState.getBank(), rawContent + 32);
    writeDoubleInNetworkByteOrder(aircraftState.getPitch(), rawContent + 40);
    writeDoubleInNetworkByteOrder(aircraftState.getSpeed(), rawContent + 48);

    udpProxy->sendData(rawContent, contentLength);

    delete[] rawContent;
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
    udpProxy->stopUDPProxy();
    simConnectProxy->stopSimConnectProxy();
}

