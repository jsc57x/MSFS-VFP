#pragma once

#include <string>

#include "datatypes.h"

void printHelp(u16 defaultReceivingPort, std::string defaultTargetIP, u16 defaultTargetPort);

void printInfo(std::string message);
void printWarning(std::string message);
void printError(std::string message);