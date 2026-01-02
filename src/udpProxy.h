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
#include "aircraftState.h"
#include <string>
#include <winsock2.h>
#include <thread>

/// <summary>
/// Callback for incoming messages to show or remove indicators.
/// </summary>
class UDPMessageCallback {
public:
    /// <summary>
    /// Virtual destructor
    /// </summary>
    virtual ~UDPMessageCallback() = default;

    /// <summary>
    /// Handles an incoming message.
    /// </summary>
    /// <param name="message">The message as char array</param>
    /// <param name="length">The length of the array</param>
    virtual void handleMessage(char* message, uint length) = 0;
};

/// <summary>
/// Proxy class for incoming and outgoing UDP data traffic.
/// </summary>
class UDPProxy {
public: 
    /// <summary>
    /// Starts the UDPProxy. This means it opens an UDP socket connection and launches an additional thread 
    /// that handles incoming UDP data traffic.
    /// </summary>
    /// <param name="udpPort">Port number for incoming data</param>
    /// <param name="callback">Callback to handle incoming data</param>
    /// <param name="targetIPAddress">Target IP address for outgoing data</param>
    /// <param name="targetPort">Target port number for outgoing data</param>
    /// <returns></returns>
    bool startUDPProxy(ushort udpPort, UDPMessageCallback* callback, std::string targetIPAddress, ushort targetPort);

    /// <summary>
    /// Sends the given data to the target for outgoing data.
    /// </summary>
    /// <param name="rawData">Data to be send</param>
    /// <param name="length">Data length</param>
    void sendData(char* rawData, uint length);

    /// <summary>
    /// Closes the UDP socket, stops the created thread and let it run dry
    /// </summary>
    void stopUDPProxy();

private:
    /// <summary>
    /// Flag for the running state of the serverThread.
    /// </summary>
    bool isRunning;

    /// <summary>
    /// Thread for receiving and handling of messages.
    /// </summary>
    std::thread serverThread;

    /// <summary>
    /// UDP socket
    /// </summary>
    SOCKET sock;

    /// <summary>
    /// Information about the target of outgoing messages.
    /// </summary>
    sockaddr_in targetAddr;

    /// <summary>
    /// Opens the necessary socket for incoming and outgoing UDP traffic.
    /// </summary>
    /// <param name="port">The port number for incoming UDP traffic</param>
    void openUDPSocket(ushort port);
    
    /// <summary>
    /// Closes the UDP socket.
    /// </summary>
    void closeUDPSocket();

    /// <summary>
    /// Handles the loop for incoming messages. Returns after closeUDPSocket was called.
    /// </summary>
    /// <param name="callback">The callback to handle incoming messages</param>
    void handleSocket(UDPMessageCallback* callback);
};