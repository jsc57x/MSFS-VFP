#include "flightPathVisualizer.h"
#include "console.h"

#include <iostream>

#define PROG_NAME "Flight Path Visualizer - MSFS Extension"
#define VERSION "0.0.1"

#define DEFAULT_SEND_IP_ADDR "127.0.0.1"
#define DEFAULT_SEND_UDP_PORT 10988

#define DEFAULT_RECV_UDP_PORT 10388

int main(int argc, char* argv[])
{
    u16 serverPort = DEFAULT_RECV_UDP_PORT;
    std::string targetIP = DEFAULT_SEND_IP_ADDR;
    u16 targetPort = DEFAULT_SEND_UDP_PORT;
    FlightPathVisualizer fpv;

    std::cout << PROG_NAME << std::endl;
    std::cout << "Version " << VERSION << std::endl;

    if (argc == 1)
    {
        std::cout << "Invalid syntax" << std::endl << std::endl;
        printHelp(DEFAULT_RECV_UDP_PORT, DEFAULT_SEND_IP_ADDR, DEFAULT_SEND_UDP_PORT);
    }

    // TODO: Parameter parsen, für den Moment tun es die Default-Werte

    fpv = FlightPathVisualizer();
    fpv.start(serverPort, targetIP, targetPort);

    bool appRunning = true;
    std::string command;

    while (appRunning)
    {
        std::cin >> command;
        if (command == "exit")
        {
            appRunning = false;
            fpv.shutdown();
        }
    }
}