#include "udpClient.h"
#include "log.h"

#include <WinSock2.h>
#include <ws2tcpip.h>

u32 UDPClient::connect(std::string targetIPAddress, u16 port)
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

    memcpy(rawContent, &aircraftState->latitude, sizeof(&aircraftState->latitude));
    memcpy(rawContent + 8, &aircraftState->longitude, sizeof(&aircraftState->longitude));
    memcpy(rawContent + 16, &aircraftState->altitude, sizeof(&aircraftState->altitude));
    memcpy(rawContent + 24, &aircraftState->heading, sizeof(&aircraftState->heading));
    memcpy(rawContent + 32, &aircraftState->bank, sizeof(&aircraftState->bank));
    memcpy(rawContent + 40, &aircraftState->pitch, sizeof(&aircraftState->pitch));
    memcpy(rawContent + 48, &aircraftState->speed, sizeof(&aircraftState->speed));

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