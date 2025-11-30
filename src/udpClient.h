#pragma once

#include "datatypes.h"
#include "flightPathVisualizer.h"
#include <string>

class UDPClient{
public:
    void sendPlaneStatus(std::string ipAddress, u16 port, PlaneInformation info);
};

class PlaneInformation : public WorldPosition{
public:
    std::string getPlaneName();
};