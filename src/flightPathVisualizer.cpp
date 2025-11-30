#include <string>
#include <iostream>
#include "datatypes.h"
#include "flightPathVisualizer.h"
#include "console.h"

void FlightPathVisualizer::start(u16 serverPort, std::string targetIP, u16 targetPort)
{
    udpServer = UDPServer();
    int startUDPServRes = udpServer.startUDPServer(serverPort, this);

}

void FlightPathVisualizer::handleMessage(char* message, int length)
{
    std::cout << message << std::endl;
}

void FlightPathVisualizer::shutdown()
{
    udpServer.stopUDPServer();
}
