/*
 * Visual Flight Path for Microsoft Flight Simulator 2024 (Test Mock)
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

#define DEFAULT_RECEIVE_UDP_PORT 10988

#define SOURCE_FILE_DELIMITER ';'

SOCKET openOutgoingPort();
SOCKET openIngoingPort(int port);

void processFile(int targetUPDPort, int sourceUDPPort, std::string filePath);

void handleRaw( std::ifstream& inFile, sockaddr_in addr, SOCKET target);
void handleStatic( std::ifstream& inFile, sockaddr_in addr, SOCKET target);
void handleDynamic( std::ifstream& inFile, sockaddr_in addr, SOCKET target, int ingoingPort);

void handleIngoingPosition(SOCKET target, sockaddr_in addr, char* rawPosition, int messageLength, std::vector<std::vector<std::string>> commands);

void printHelp();

int sendData(SOCKET sock, sockaddr_in addr, const char* row, int length);

char* convertRowToRaw(std::vector<std::string> rowParts, int* out_len);

char* convertSetIndicatorToRaw(std::vector<std::string> splittedRow, int* out_len);
char* createSetIndicator(unsigned short indicatorID, unsigned int indicatorTypeID, double latitude, double longitude, double altitude, double heading, double bank, double pitch, int* out_len);
char* convertRemoveIndicatorToRaw(std::vector<std::string> splittedRow, int* out_len);

std::vector<std::string> split(const std::string& s);

inline void writeUshortInNetworkByteOrder(unsigned short value, char* dst)
{
    char tmp[2];
    std::memcpy(tmp, &value, 2);
    for (int i = 0; i < 2; ++i)
    {
        dst[i] = tmp[1 - i];
    }
}

inline void writeUintInNetworkByteOrder(unsigned int value, char* dst)
{
    char tmp[4];
    std::memcpy(tmp, &value, 4);
    for (int i = 0; i < 4; ++i)
    {
        dst[i] = tmp[3 - i];
    }
}

inline void writeDoubleInNetworkByteOrder(double value, char* dst)
{
    char tmp[8];
    std::memcpy(tmp, &value, 8);
    for (int i = 0; i < 8; ++i)
    {
        dst[i] = tmp[7 - i];
    }
}

inline double readDoubleinNetworkByteOrder(const char* src)
{
    char tmp[8];

    for (int i = 0; i < 8; ++i)
    {
        tmp[i] = src[7 - i];
    }

    double value;
    std::memcpy(&value, tmp, sizeof(value));
    return value;
}


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
            processFile(udpPort, DEFAULT_RECEIVE_UDP_PORT, filePath);
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
                processFile(udpPort, DEFAULT_RECEIVE_UDP_PORT, filePath);
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

void processFile(int targetUPDPort, int sourceUDPPort, std::string filePath)
{
    SOCKET udpTarget = openOutgoingPort();

    if (udpTarget == INVALID_SOCKET)
    {
        return;
    }

    std::ifstream testDataFile(filePath);
    if (!testDataFile.good())
    {
        std::cout << "File does not exist or could not be read." << std::endl;
        return;
    }

    sockaddr_in localAddr;
    localAddr.sin_family = AF_INET;
    localAddr.sin_port = htons(targetUPDPort);
    inet_pton(AF_INET, DEFAULT_SEND_IP_ADDR, &localAddr.sin_addr);

    if (testDataFile.good())
    {
        std::string firstRow;
        std::getline(testDataFile, firstRow);

        if (firstRow == "<raw>")
        {
            handleRaw(testDataFile, localAddr, udpTarget);
        }
        else if (firstRow == "<static>")
        {
            handleStatic(testDataFile, localAddr, udpTarget);
        }
        else if (firstRow == "<dynamic>")
        {
            handleDynamic(testDataFile, localAddr, udpTarget, sourceUDPPort);
        }
    }

    closesocket(udpTarget);
    WSACleanup();
    testDataFile.close();
}

SOCKET openOutgoingPort()
{
    WSADATA wsaData;
    SOCKET udpTarget = INVALID_SOCKET;

    // startup WinSocket
    int res = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (res != 0) {
        std::cerr << "WSAStartup failed :" << res << std::endl;
        return udpTarget;
    }

    udpTarget = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (udpTarget == INVALID_SOCKET)
    {
        std::cerr << "Error creating UDP socket: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return udpTarget;
    }

    return udpTarget;
}

SOCKET openIngoingPort(int port)
{
    WSADATA wsaData;
    SOCKET sock = INVALID_SOCKET;
    struct sockaddr_in serverAddr;

    int res = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (res != 0) {
        std::cerr << "WSAStartup failed :" << res << std::endl;
        return INVALID_SOCKET;
    }

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock == INVALID_SOCKET) {
        std::cerr << "Error creating UDP socket: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return INVALID_SOCKET;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Failed to bind to socket: " << WSAGetLastError() << std::endl;
        closesocket(sock);
        WSACleanup();
        return INVALID_SOCKET;
    }

    return sock;
}

void handleRaw( std::ifstream& inFile, sockaddr_in addr, SOCKET target)
{
    while (inFile.good())
    {
        std::string row;
        std::getline(inFile, row);
        sendData(target, addr, row.c_str(), (int)row.size());
    }
}

void handleStatic(std::ifstream& inFile, sockaddr_in addr, SOCKET target)
{
    while (inFile.good())
    {
        std::string row;
        std::getline(inFile, row);
        std::vector<std::string> parts = split(row);

        if (parts.at(0) == "<delay>")
        {
            int delay = std::stoi(parts.at(1));
            Sleep(delay);
        }
        else {
            int length;
            char* rawContent = convertRowToRaw(parts, &length);
            sendData(target, addr, rawContent, length);
        }
    }
}

void handleDynamic(std::ifstream& inFile, sockaddr_in addr, SOCKET target, int ingoingPort)
{
    std::vector<std::vector<std::string>> commands;
    // read file in to vector (and split rows)
    while (inFile.good())
    {
        std::string row;
        std::getline(inFile, row);
        std::vector<std::string> parts = split(row);

        commands.push_back(parts);
    }

    // open udp port
    SOCKET sock = openIngoingPort(ingoingPort);

    // listen to udp port
    struct sockaddr_in clientAddr;
    char buffer[1024];
    int recvLen;
    int clientAddrLen = sizeof(clientAddr);

    while (true)
    {
        recvLen = recvfrom(sock, buffer, sizeof(buffer) - 1, 0, (struct sockaddr*)&clientAddr, &clientAddrLen);

        if (recvLen == SOCKET_ERROR) {
            std::cerr << "Failed to receive data: " << WSAGetLastError() << std::endl;
            break;
        }

        handleIngoingPosition(target, addr, buffer, recvLen, commands);
    }
}

void handleIngoingPosition(SOCKET target, sockaddr_in addr, char* rawPosition, int messageLength, std::vector<std::vector<std::string>> commands)
{
    if (messageLength != 56)
    {
        std::cerr << "Message received has an invalid length." << std::endl;
    }

    double latitude = readDoubleinNetworkByteOrder(rawPosition);
    double longitude = readDoubleinNetworkByteOrder(rawPosition + 8);
    double altitude = readDoubleinNetworkByteOrder(rawPosition + 16);
    double heading = readDoubleinNetworkByteOrder(rawPosition + 24);
    double bank = readDoubleinNetworkByteOrder(rawPosition + 32);
    double pitch = readDoubleinNetworkByteOrder(rawPosition + 40);
    double speed = readDoubleinNetworkByteOrder(rawPosition + 48);

    std::cout << "Received plane position: " <<
        "Latitude: " << latitude <<
        ", Longitude " << longitude <<
        ", Altitude " << altitude <<
        ", Heading " << heading <<
        ", Bank " << bank <<
        ", Pitch " << pitch <<
        ", Speed " << speed << std::endl;

    for (std::vector<std::string> command : commands)
    {
        if(command.at(0) == "<set>")
        {
            unsigned short indicatorID = static_cast<unsigned short>(std::stoi(command.at(1))); // not very clean but ok for test code
            unsigned int indicatorTypeID = std::stoi(command.at(2));

            // values with offset
            double setLatitude = std::stod(command.at(3)) + latitude;
            double setLongitude = std::stod(command.at(4)) + longitude;
            double setAltitude = std::stod(command.at(5)) + altitude;
            double setHeading = std::stod(command.at(6)) + heading;
            double setBank = std::stod(command.at(7)) + bank;
            double setPitch = std::stod(command.at(8)) + pitch;

            int length;
            char* rawContent = createSetIndicator(indicatorID, indicatorTypeID, setLatitude, setLongitude, setAltitude, setHeading, setBank, setPitch, &length);
            sendData(target, addr, rawContent, length);

            delete[] rawContent;
        }
        else if (command.at(0) == "<rem>")
        {
            int length;
            char* rawContent = convertRemoveIndicatorToRaw(command, &length);
            sendData(target, addr, rawContent, length);

        }
        else if (command.at(0) == "<delay>")
        {
            int delay = std::stoi(command.at(1));
            Sleep(delay);
        }
    }
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

char* convertRowToRaw(std::vector<std::string> rowParts, int* out_len)
{
    if (rowParts.at(0) == "<set>")
    {
        return convertSetIndicatorToRaw(rowParts, out_len);
    }
    else if (rowParts.at(0) == "<rem>")
    {
        return convertRemoveIndicatorToRaw(rowParts, out_len);
    }

    return 0;
}

char* convertSetIndicatorToRaw(std::vector<std::string> splittedRow, int* out_len)
{
    unsigned short indicatorID = static_cast<short>(std::stoi(splittedRow.at(1))); // not very clean but ok for test code
    unsigned int indicatorTypeID = std::stoi(splittedRow.at(2));
    double latitude = std::stod(splittedRow.at(3));
    double longitude = std::stod(splittedRow.at(4));
    double altitude = std::stod(splittedRow.at(5));
    double heading = std::stod(splittedRow.at(6));
    double bank = std::stod(splittedRow.at(7));
    double pitch = std::stod(splittedRow.at(8));

    return createSetIndicator(indicatorID, indicatorTypeID, latitude, longitude, altitude, heading, bank, pitch, out_len);
}

char* createSetIndicator(unsigned short indicatorID, unsigned int indicatorTypeID, double latitude, double longitude, double altitude, double heading, double bank, double pitch, int* out_len)
{
    *out_len = 56;
    char* rawContent = new char[*out_len] {};

    short commandID = 1;
    writeUshortInNetworkByteOrder(commandID, rawContent);
    writeUshortInNetworkByteOrder(indicatorID, rawContent + 2);
    writeUintInNetworkByteOrder(indicatorTypeID, rawContent + 4);
    writeDoubleInNetworkByteOrder(latitude, rawContent + 8);
    writeDoubleInNetworkByteOrder(longitude, rawContent + 16);
    writeDoubleInNetworkByteOrder(altitude, rawContent + 24);
    writeDoubleInNetworkByteOrder(heading, rawContent + 32);
    writeDoubleInNetworkByteOrder(bank, rawContent + 40);
    writeDoubleInNetworkByteOrder(pitch, rawContent + 48);

    return rawContent;
}


char* convertRemoveIndicatorToRaw(std::vector<std::string> splittedRow, int* out_len)
{
    short commandID = 2;

    *out_len = sizeof(short) * (splittedRow.size() - 1) + 2;
    
    char* rawContent = new char[2048] {}; // for tests this buffer has to be larger than the input buffer in VFP

    writeUshortInNetworkByteOrder(commandID, rawContent);

    int curPos = 2;

    for (int i = 1; i < splittedRow.size(); i++)
    {
        unsigned short indicatorID = static_cast<unsigned short>(std::stoi(splittedRow.at(i))); // not very clean but ok for test code
        writeUshortInNetworkByteOrder(indicatorID, rawContent + curPos);
        curPos += sizeof(indicatorID);
    }

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