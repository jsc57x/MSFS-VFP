#pragma once

#include <string>
#include <WinSock2.h>
#include <Windows.h>
#include <vector>

void processFile(int targetUPDPort, std::string filePath);

void printHelp();

int sendData(SOCKET sock, sockaddr_in addr, std::string row);

char* convertRowToRaw(std::string row, size_t* out_len);

std::vector<std::string> split(const std::string& s);