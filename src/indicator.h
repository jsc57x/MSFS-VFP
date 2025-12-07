#pragma once

#include "datatypes.h"
#include <string>

class Indicator {
public:
    std::string getFileName();
    static Indicator parse(char* array, u32 length);

private:
    std::string filename;
};