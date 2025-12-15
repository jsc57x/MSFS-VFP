#include "udpServer.h"
#include "datatypes.h"
#include "log.h"
#include <iostream>
#include <thread>
#include <winsock2.h>
#include <Windows.h>
#include <chrono>

#pragma comment(lib, "ws2_32.lib")

u32 UDPServer::startUDPServer(u16 udpPort, UDPMessageCallback* callback)
{
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

SOCKET UDPServer::openUDPServerSocket(u16 port)
{
    WSADATA wsaData;
    SOCKET sock = INVALID_SOCKET;
    struct sockaddr_in serverAddr;

    Logger::logInfo(("Connecting to port " + std::to_string(port) + "...").c_str());

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        Logger::logError("WSAStartup failed.");
        return 1;
    }

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock == INVALID_SOCKET) {
        Logger::logError("Create socket failed: " + WSAGetLastError());
        WSACleanup();
        return 1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        Logger::logError("Failed to bind to socket: " + WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    Logger::logInfo("Connected");

    return sock;
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
            printf("recvfrom() Fehler: %d\n", WSAGetLastError());
            break;
        }

        callback->handleMessage(buffer, recvLen);
    }
}

void UDPServer::handleUDPMessage(std::string message)
{
    printMessage(message);
}

//TODO: Übergebe ich hier einen String oder ein struct mit den Details?
void UDPServer::printMessage(std::string message)
{
    //FIXME: Alleine ein String reicht hier vermutlich nicht aus, mindestens ein Validierungsstatus der Message wäre noch gut, die Validierung findet aber nicht unbedingt hier statt
    std::cout << message << std::endl;
}