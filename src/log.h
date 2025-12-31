#pragma once

#include <string>

class Logger {
public:
    static void logMessage(std::string message);
    static void logInfo(std::string message);
    static void logWarning(std::string message);
    static void logError(std::string message);
};
