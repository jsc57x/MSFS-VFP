/*
 * Visual Flight Path for Microsoft Flight Simulator 2024
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
#include "console.h"
#include "log.h"
#include "datatypes.h"
#include <iostream>
#include <string>

const char* COLOR_NORMAL = "\033[0m";
const char* COLOR_BLUE = "\033[34m";
const char* COLOR_YELLOW = "\033[33m";
const char* COLOR_RED = "\033[31m";

void printHelp(ushort defaultReceivingPort, std::string defaultTargetIP, ushort defaultTargetPort)
{
    std::cout << "Syntax: VisualFlightPathExtension [-p port] [-t ip address] [-tp target port]" << std::endl;
    std::cout << std::endl << "Options:" << std::endl;
    std::cout << "\t-p\tReceiving UDP port ([1-65535], default: " << (int)defaultReceivingPort << ")" << std::endl;
    std::cout << "\t-t\tTarget IP address for flight status informations (default: " << defaultTargetIP << ")" << std::endl;
    std::cout << "\t-tp\tTarget UDP port ([1-65535], default: " << (int)defaultTargetPort << ")" << std::endl;
}

void Logger::logMessage(std::string message)
{
    printMessage(message);
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

void printMessage(std::string message)
{
    std::cout << message << std::endl;
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