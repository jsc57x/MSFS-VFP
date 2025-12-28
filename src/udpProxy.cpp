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

uint UDPServer::startUDPServer(ushort udpPort, UDPMessageCallback* callback, std::string targetIPAddress, ushort targetPort)
{
    targetAddr.sin_family = AF_INET;
    targetAddr.sin_port = htons(targetPort);
    inet_pton(AF_INET, targetIPAddress.c_str(), &targetAddr.sin_addr);

    sock = openUDPServerSocket(udpPort);
    serverThread = std::thread(&UDPServer::handleSocket, this, callback);
    return 0;
}

void UDPServer::stopUDPServer()
{
    isRunning = false;
    closeUDPServerSocket();

    // Waiting for thread to finish
    serverThread.join();
}

SOCKET UDPServer::openUDPServerSocket(ushort port)
{
    WSADATA wsaData;
    SOCKET sock = INVALID_SOCKET;
    struct sockaddr_in serverAddr;

    Logger::logInfo(("Connecting to UDP port " + std::to_string(port) + "...").c_str());

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        Logger::logError("WSAStartup failed.");
        return 1;
    }

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock == INVALID_SOCKET) {
        Logger::logError("Create socket failed. WSA Error: " + WSAGetLastError());
        WSACleanup();
        return 1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        Logger::logError("Failed to bind to socket. WSA Error: " + WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    Logger::logInfo("UDP Port connected");

    return sock;
}

void UDPServer::sendPlaneStatus(AircraftState* aircraftState)
{
    int contentLength = 56;
    char* rawContent = new char[contentLength] {};

    double latitude = aircraftState->getLatitude();
    double longitude = aircraftState->getLongitude();
    double altitude = aircraftState->getAltitude();
    double heading = aircraftState->getHeading();
    double bank = aircraftState->getBank();
    double pitch = aircraftState->getPitch();
    double speed = aircraftState->getSpeed();

    memcpy(rawContent, &latitude, sizeof(latitude));
    memcpy(rawContent + 8, &longitude, sizeof(longitude));
    memcpy(rawContent + 16, &altitude, sizeof(altitude));
    memcpy(rawContent + 24, &heading, sizeof(heading));
    memcpy(rawContent + 32, &bank, sizeof(bank));
    memcpy(rawContent + 40, &pitch, sizeof(pitch));
    memcpy(rawContent + 48, &speed, sizeof(speed));

    int res = sendto(sock, rawContent, contentLength, 0, (sockaddr*)&targetAddr, sizeof(targetAddr));

    if (res == SOCKET_ERROR)
    {
        Logger::logError("Failed to send aircraft information: " + std::to_string(WSAGetLastError()));
    }
}

void UDPServer::closeUDPServerSocket()
{
    closesocket(sock);
    WSACleanup();
}

void UDPServer::handleSocket(UDPMessageCallback* callback)
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
            Logger::logError("Failed to handle incoming UDP message. WSA Error: " + std::to_string(WSAGetLastError()));
            break;
        }

        callback->handleMessage(buffer, recvLen);
    }
}