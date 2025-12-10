// TestFlightPathProvider.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <string>
#include <iostream>
#include <fstream>
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <vector>
#include <sstream>

#define PROG_NAME "MSFS Flight Path Visualizer Test Provider"
#define VERSION "0.0.1"

#define DEFAULT_SEND_IP_ADDR "127.0.0.1"
#define DEFAULT_SEND_UDP_PORT 10388

#define SOURCE_FILE_DELIMITER ';'


void processFile(int targetUPDPort, std::string filePath);

void printHelp();

int sendData(SOCKET sock, sockaddr_in addr, const char* row, int length);

char* convertRowToRaw(std::string row, int* out_len);

char* convertSetIndicatorToRaw(std::vector<std::string> splittedRow, int* out_len);
char* convertRemoveIndicatorToRaw(std::vector<std::string> splittedRow, int* out_len);

std::vector<std::string> split(const std::string& s);


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

    bool sendRaw = false;
    while (testDataFile.good())
    {
        std::string row;
        std::getline(testDataFile, row);

        if (row == "<raw>")
        {
            sendRaw = true;
            continue;
        }

        if (sendRaw)
        {
            sendData(udpTarget, localAddr, row.c_str(), (int)strlen(row.c_str()));
        }
        else {
            int length;
            char* rawContent = convertRowToRaw(row, &length);
            sendData(udpTarget, localAddr, rawContent, length);
        }
    }

    closesocket(udpTarget);
    WSACleanup();
    testDataFile.close();
}


int sendData(SOCKET sock, sockaddr_in addr, const char* row, int length)
{
    int res;

    std::cout << "Sending to " << addr.sin_port << std::endl;

    res = sendto(sock, row, length, 0, (sockaddr*)&addr, sizeof(addr));

    if (res == SOCKET_ERROR)
    {
        std::cerr << "Failed to send row :\"" << row << " with error " << WSAGetLastError() << std::endl;
    }

    return res;
}

char* convertRowToRaw(std::string row, int* out_len)
{
    std::vector<std::string> parts = split(row);

    if (parts.at(0) == "<set>")
    {
        return convertSetIndicatorToRaw(parts, out_len);
    }
    else if (parts.at(0) == "<rem>")
    {
        return convertRemoveIndicatorToRaw(parts, out_len);
    }

    return 0;
}

char* convertSetIndicatorToRaw(std::vector<std::string> splittedRow, int* out_len)
{
    int commandID = 1;
    int indicatorID = std::stoi(splittedRow.at(1));
    double latitude = std::stod(splittedRow.at(3));
    double longitude = std::stod(splittedRow.at(4));
    double height = std::stod(splittedRow.at(5));
    double roll = std::stod(splittedRow.at(6));
    double pitch = std::stod(splittedRow.at(7));
    double yaw = std::stod(splittedRow.at(8));


    *out_len = 64;
    char* rawContent = new char[*out_len] {};
    
    memcpy(rawContent, &commandID, sizeof(commandID));
    memcpy(rawContent + 4, &indicatorID, sizeof(indicatorID));
    memcpy(rawContent + 16, &latitude, sizeof(latitude));
    memcpy(rawContent + 24, &longitude, sizeof(longitude));
    memcpy(rawContent + 32, &height, sizeof(height));
    memcpy(rawContent + 40, &roll, sizeof(roll));
    memcpy(rawContent + 48, &pitch, sizeof(pitch));
    memcpy(rawContent + 56, &yaw, sizeof(yaw));

    return rawContent;
}


char* convertRemoveIndicatorToRaw(std::vector<std::string> splittedRow, int* out_len)
{
    int commandID = 2;

    *out_len = sizeof(int) * (splittedRow.size() - 1);
    
    char rawContent[1024];


    memcpy(rawContent, &commandID, sizeof(commandID));

    return rawContent;
}


std::vector<std::string> split(const std::string& s)
{
    std::vector<std::string> tokens;
    std::stringstream ss(s);
    std::string part;

    while (std::getline(ss, part, SOURCE_FILE_DELIMITER)) {
        tokens.push_back(part);
    }

    return tokens;
}