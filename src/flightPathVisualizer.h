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
#pragma once

#include "datatypes.h"
#include "udpProxy.h"
#include "udpCommand.h"
#include "simConnectProxy.h"

#include <string>

/// <summary>
/// Main class which controls and processes the data flow between SimConnectProxy and UDPProxy.
/// </summary>
class FlightPathVisualizer : public UDPMessageCallback, public SimConnectCallback{
public:
    /// <summary>
    /// Starts the procssing of incoming UDP data traffic and SimConnect status updates.
    /// </summary>
    /// <param name="serverPort">The IP port for incoming data</param>
    /// <param name="targetIP">The IP address for outgoing data</param>
    /// <param name="targetPort">The IP port for outgoing data</param>
    void start(ushort serverPort, std::string targetIP, ushort targetPort);

    /// <summary>
    /// Stops the processing.
    /// </summary>
    void shutdown();

    void handleMessage(char* message, uint length) override;
    void handleAircraftStateUpdate(AircraftState aircraftState) override;

    /// <summary>
    /// Advises the SimConnectProxy to clear the cached indicator type mappings.
    /// </summary>
    void clearIndicatorMappings();

    /// <summary>
    /// Advises the SimConnectProxy to delete all by this instance placed indicators.
    /// </summary>
    void removeAllIndicators();

private:
    /// <summary>
    /// The UDP Proxy for receiving and sending data over a UDP socket.
    /// </summary>
    UDPProxy* udpServer;

    /// <summary>
    /// The SimConnect Proxy for data exchange with a running SimConnect application.
    /// </summary>
    SimConnectProxy* simConnectProxy;

    /// <summary>
    /// Parses the incoming UDP datagram and creates the command configuration.
    /// </summary>
    /// <param name="array">UDP data</param>
    /// <param name="length">Length of UDP data</param>
    /// <returns>Abstract command configuration</returns>
    std::unique_ptr<AbstractCommandConfiguration> parseIncomingMessage(char* array, uint length);

    /// <summary>
    /// Parses the incoming UDP datagram and creates a command configuration to place a SimObject.
    /// </summary>
    /// <param name="array">UDP data</param>
    /// <param name="length">Length of UDP data</param>
    /// <returns>Set indicator command configuration</returns>
    std::unique_ptr<SetIndicatorCommandConfiguration> parseSetCommand(char* array, uint length);
    
    /// <summary>
    /// Parses the incoming UDP datagram and creates a command configuration to remove an existing SimObject.
    /// </summary>
    /// <param name="array">UDP data</param>
    /// <param name="length">Length of UDP data</param>
    /// <returns>Remove indicator command configuration</returns>
    std::unique_ptr<RemoveIndicatorsCommandConfiguration> parseRemoveCommand(char* array, uint length);
};

