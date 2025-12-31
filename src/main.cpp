#include "flightPathVisualizer.h"
#include "console.h"
#include "stringHelper.h"
#include "log.h"

#include <string>
#include <vector>
#include <iostream>

#define DEFAULT_SEND_IP_ADDR "127.0.0.1"
#define DEFAULT_SEND_UDP_PORT 10988

#define DEFAULT_RECV_UDP_PORT 10388

bool isIPAddressValid(std::string ipAddress)
{
    std::vector<std::string> ipAddressParts = splitString(ipAddress, '.');
    if (ipAddressParts.size() != 4)
    {
        return false;
    }

    for (std::string curPart : ipAddressParts)
    {
        try {
            int i = std::stoi(curPart);

            if (i < 0 || i > 255)
            {
                return false;
            }
        }
        catch (std::invalid_argument)
        {
            return false;
        }
    }

    return true;
}

int main(int argc, char* argv[])
{
    ushort serverPort = DEFAULT_RECV_UDP_PORT;
    std::string targetIP = DEFAULT_SEND_IP_ADDR;
    ushort targetPort = DEFAULT_SEND_UDP_PORT;
    FlightPathVisualizer fpv;

    Logger::logMessage("Flight Path Visualizer - MSFS Extension");
    Logger::logMessage("Version 1.0.0");
    
    bool cmdParamsValid = true;

    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-h") == 0)
        {
            printHelp(DEFAULT_RECV_UDP_PORT, DEFAULT_SEND_IP_ADDR, DEFAULT_SEND_UDP_PORT);
            return 0;
        }
        else if (strcmp(argv[i], "-p") == 0)
        {
            if (argc < ++i)
            {
                cmdParamsValid = false;
                break;
            }
            try {
                int serverPortRaw = std::stoi(argv[i]);
                if (serverPortRaw < 0 || serverPortRaw > 65535)
                {
                    cmdParamsValid = false;
                    break;
                }
                serverPort = static_cast<short>(serverPortRaw);
            }
            catch (std::invalid_argument)
            {
                cmdParamsValid = false;
                break;
            }
        }
        else if (strcmp(argv[i], "-t") == 0)
        {
            if (argc < ++i)
            {
                cmdParamsValid = false;
                break;
            }

            targetIP = argv[i];

            if(!isIPAddressValid(targetIP))
            {
                cmdParamsValid = false;
                break;
            }
        }
        else if (strcmp(argv[i], "-tp") == 0)
        {
            if (argc < ++i)
            {
                cmdParamsValid = false;
                break;
            }
            try {
                int targetPortRaw = std::stoi(argv[i]);
                if (targetPortRaw < 0 || targetPortRaw > 65535)
                {
                    cmdParamsValid = false;
                    break;
                }
                targetPort = static_cast<short>(targetPortRaw);
            }
            catch (std::invalid_argument)
            {
                cmdParamsValid = false;
                break;
            }
        }
    }

    if (!cmdParamsValid)
    {
        Logger::logMessage("Invalid syntax");
        printHelp(DEFAULT_RECV_UDP_PORT, DEFAULT_SEND_IP_ADDR, DEFAULT_SEND_UDP_PORT);
        return -1;
    }

    Logger::logMessage("Using ingoing port " + std::to_string(serverPort) + 
        ", target ip address " + targetIP + 
     ", target port " + std::to_string(targetPort));

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
        else if (command == "resetMappings")
        {
            fpv.clearIndicatorMappings();
        }
        else if (command == "clearIndicators")
        {
            fpv.removeAllIndicators();
        }
    }
}