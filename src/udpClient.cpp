#include "udpClient.h"
#include "log.h"
#include "aircraftState.h"

#include <WinSock2.h>
#include <ws2tcpip.h>

uint UDPClient::connect(std::string targetIPAddress, ushort port)
{
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, targetIPAddress.c_str(), &addr.sin_addr);

    WSADATA wsaData;
    //sock = INVALID_SOCKET;

    // startup WinSocket
    int res = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (res != 0) {

        return -1;
    }

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (sock == INVALID_SOCKET)
    {
        WSACleanup();
        return -1;
    }

    return 0;
}

void UDPClient::sendPlaneStatus(AircraftState* aircraftState)
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

    int res = sendto(sock, rawContent, contentLength, 0, (sockaddr*)&addr, sizeof(addr));

    if (res == SOCKET_ERROR)
    {
        Logger::logError("Failed to send aircraft information: " + std::to_string(WSAGetLastError()));
    }
}

void UDPClient::disconnect()
{
    closesocket(sock);
    WSACleanup();
}