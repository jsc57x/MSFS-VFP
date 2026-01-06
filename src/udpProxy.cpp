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
#include "udpProxy.h"
#include "aircraftState.h"
#include "datatypes.h"
#include "log.h"
#include <iostream>
#include <thread>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <Windows.h>
#include <chrono>

#pragma comment(lib, "ws2_32.lib")

bool UDPProxy::startUDPProxy(ushort udpPort, UDPProxyCallback* callback, std::string targetIPAddress, ushort targetPort)
{
    targetAddr.sin_family = AF_INET;
    targetAddr.sin_port = htons(targetPort);
    inet_pton(AF_INET, targetIPAddress.c_str(), &targetAddr.sin_addr);

    openUDPSocket(udpPort);

    if (sock == INVALID_SOCKET)
    {
        return false;
    }

    serverThread = std::thread(&UDPProxy::handleSocket, this, callback);
    return true;
}

void UDPProxy::stopUDPProxy()
{
    isRunning = false;
    closeUDPSocket();

    // Waiting for thread to finish
    serverThread.join();
}

void UDPProxy::openUDPSocket(ushort port)
{
    WSADATA wsaData;
    sock = INVALID_SOCKET;
    struct sockaddr_in serverAddr;

    Logger::logInfo(("Connecting to UDP port " + std::to_string(port) + "...").c_str());

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        Logger::logError("WSAStartup failed.");
        return;
    }

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock == INVALID_SOCKET) {
        Logger::logError("Create socket failed. WSA Error: " + WSAGetLastError());
        WSACleanup();
        return;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        Logger::logError("Failed to bind to socket. WSA Error: " + WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return;
    }

    Logger::logInfo("UDP Port connected");
}

void UDPProxy::sendData(char* rawData, uint length)
{
    int res = sendto(sock, rawData, length, 0, (sockaddr*)&targetAddr, sizeof(targetAddr));

    if (res == SOCKET_ERROR)
    {
        Logger::logError("Failed to send aircraft information: " + std::to_string(WSAGetLastError()));
    }
}

void UDPProxy::closeUDPSocket()
{
    closesocket(sock);
    WSACleanup();
}

void UDPProxy::handleSocket(UDPProxyCallback* callback)
{
    struct sockaddr_in clientAddr;
    char buffer[1024];
    int recvLen;
    int clientAddrLen = sizeof(clientAddr);

    isRunning = true;

    while (isRunning)
    {
        recvLen = recvfrom(sock, buffer, sizeof(buffer) - 1, 0, (struct sockaddr*)&clientAddr, &clientAddrLen);

        // is UDP server stopped?
        if (!isRunning)
        {
            return;
        }

        if (recvLen == SOCKET_ERROR) { 
            uint lastErrorCode = WSAGetLastError();
            if (lastErrorCode == WSAECONNRESET) {
                // this error code occurs if sendto could not reach the target host/port
                // minor inconvenience in Windows Sockets
                continue;
            }
            else if (lastErrorCode == WSAEMSGSIZE) {
                Logger::logError("Message size is more than 1024 Bytes. Message ignored!");
                continue;
            }
            Logger::logError("Failed to handle incoming UDP message. WSA Error: " + std::to_string(lastErrorCode));
            continue;
        }

        callback->handleMessage(buffer, recvLen);
    }
}