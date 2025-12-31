#pragma once

#include<string>
#include<sstream>
#include<vector>

inline std::vector<std::string> splitString(const std::string& s, char delimiter)
{
    std::vector<std::string> tokens;
    std::stringstream ss(s);
    std::string part;

    while (std::getline(ss, part, delimiter)) {
        tokens.push_back(part);
    }

    return tokens;
}