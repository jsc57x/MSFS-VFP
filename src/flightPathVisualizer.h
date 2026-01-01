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

class FlightPathVisualizer : public UDPMessageCallback, public SimConnectCallback{
public:
    void start(ushort serverPort, std::string targetIP, ushort targetPort);
    void shutdown();

    void handleMessage(char* message, uint length) override;
    void handlePlaneUpdate(AircraftState aircraftState) override;

    void clearIndicatorMappings();
    void removeAllIndicators();

private:
    UDPServer* udpServer;
    SimConnectProxy* simConnectProxy;

    std::unique_ptr<AbstractCommandConfiguration> parseIncomingMessage(char* array, uint length);
    std::unique_ptr<SetIndicatorCommandConfiguration> parseSetCommand(char* array, uint length);
    std::unique_ptr<RemoveIndicatorsCommandConfiguration> parseRemoveCommand(char* array, uint length);

    void handleInvalidMessage(std::string errorMsg, char* message, uint length);
};

