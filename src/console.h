#pragma once

#include <string>

#include "datatypes.h"

void printHelp(ushort defaultReceivingPort, std::string defaultTargetIP, ushort defaultTargetPort);

void printMessage(std::string message);
void printInfo(std::string message);
void printWarning(std::string message);
void printError(std::string message);