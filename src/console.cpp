
#include "console.h"
#include "log.h"
#include "datatypes.h"
#include <iostream>
#include <string>

const char* COLOR_NORMAL = "\033[0m";
const char* COLOR_BLUE = "\033[34m";
const char* COLOR_YELLOW = "\033[33m";
const char* COLOR_RED = "\033[31m";

void printHelp(u16 defaultReceivingPort, std::string defaultTargetIP, u16 defaultTargetPort)
{
    std::cout << "Syntax: VisualFlightPathExtension [-p port] [-t ip address] [-tp target port]" << std::endl;
    std::cout << std::endl << "Options:" << std::endl;
    std::cout << "\t-p\tReceiving UDP port ([1-65535], default: " << (int)defaultReceivingPort << ")" << std::endl;
    std::cout << "\t-t\tTarget IP address for flight status informations (default: " << defaultTargetIP << ")" << std::endl;
    std::cout << "\t-tp\tTarget UDP port ([1-65535], default: " << (int)defaultTargetPort << ")" << std::endl;
}

void Logger::logInfo(std::string message)
{
    printInfo(message);
}

void Logger::logWarning(std::string message)
{
    printWarning(message);
}

void Logger::logError(std::string message)
{
    printError(message);
}

void printInfo(std::string message)
{
    std::cout << COLOR_BLUE << message << COLOR_NORMAL << std::endl;
}

void printWarning(std::string message)
{
    std::cout << COLOR_YELLOW << message << COLOR_NORMAL << std::endl;
}

void printError(std::string message)
{
    std::cout << COLOR_RED << message << COLOR_NORMAL << std::endl;
}