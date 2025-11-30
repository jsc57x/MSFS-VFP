// TestFlightPathProvider.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <string>
#include <iostream>
#include <fstream>
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include "TestFlightPathProvider.h"

#define PROG_NAME "MSFS Flight Path Visualizer Test Provider"
#define VERSION "0.0.1"

#define DEFAULT_SEND_IP_ADDR "127.0.0.1"
#define DEFAULT_SEND_UDP_PORT 10388


int main(int argc, char* argv[])
{
    std::cout << PROG_NAME << std::endl;
    std::cout << "Version " << VERSION << std::endl;
    
int udpPort = DEFAULT_SEND_UDP_PORT;
    if (argc == 1)
    {
        std::cout << "Invalid syntax" << std::endl << std::endl;
        printHelp();
    }
    if (argc == 2)
    {
        if (strcmp(argv[1], "-help") == 0 || strcmp(argv[1], "-h") == 0)
        {
            printHelp();
        }
        else {
            // Parameter has to be a file path
            std::string filePath = argv[1];
            processFile(udpPort, filePath);
        }
    }
    else if (argc == 4)
    {
        if (strcmp(argv[1], "-p") != 0)
        {
            std::cout << "Invalid syntax" << std::endl << std::endl;
            printHelp();
        }
        else {
            int port = atoi(argv[2]);
            if (port <= 0 || port > 65535)
            {
                std::cout << "Invalid UDP port" << std::endl << std::endl;
                printHelp();
            }
            else {
                udpPort = port;

                std::string filePath = argv[3];
                processFile(udpPort, filePath);
            }
        }
    }
    else {
        std::cout << "Invalid syntax" << std::endl << std::endl;
        printHelp();
    }
}

void printHelp()
{
    std::cout << "Syntax: TestFlightPathProvider [-p port] filename" << std::endl;
    std::cout << std::endl << "Options:" << std::endl;
    std::cout << "\t-p\tUDP-Port to use ([1-65535], default: " << DEFAULT_SEND_UDP_PORT << ")" << std::endl;
}

void processFile(int targetUPDPort, std::string filePath)
{
    WSADATA wsaData;
    SOCKET udpTarget = INVALID_SOCKET;

    std::ifstream testDataFile(filePath);

    if (!testDataFile.good())
    {
        std::cout << "File does not exist or could not be read." << std::endl;
        return;
    }

    // startup WinSocket
    int res = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (res != 0) {
        std::cerr << "WSAStartup failed :" << res << std::endl;
        return;
    }

   

    udpTarget = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (udpTarget == INVALID_SOCKET)
    {
        std::cerr << "Error creating UDP socket: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return;
    }

    sockaddr_in localAddr;
    localAddr.sin_family = AF_INET;
    localAddr.sin_port = htons(targetUPDPort);
    inet_pton(AF_INET, DEFAULT_SEND_IP_ADDR, &localAddr.sin_addr);

    //// 4. Bind the socket to the local address and port
    //res = bind(udpTarget, (sockaddr*)&localAddr, sizeof(localAddr));
    //if (res == SOCKET_ERROR) {
    //    std::cerr << "bind() failed, error: " << WSAGetLastError() << "\n";
    //    closesocket(udpTarget);
    //    WSACleanup();
    //    return;
    //}

    while (testDataFile.good())
    {
        std::string row;
        std::getline(testDataFile, row);
        std::cout << row << std::endl;

        int lineLength = (int)strlen(row.c_str());

        res = sendto(udpTarget, row.c_str(), lineLength, 0, (sockaddr*)&localAddr, sizeof(localAddr));

        if (res == SOCKET_ERROR)
        {
            std::cerr << "Failed to send row :\"" << row << " with error " << WSAGetLastError() << std::endl;
        }
    }

    closesocket(udpTarget);
    WSACleanup();
    testDataFile.close();
}
