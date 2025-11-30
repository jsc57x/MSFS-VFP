#pragma once
#include "flightPathVisualizer.h"

class SimConnectProxy {
public:
    bool connect(u64 timeout, SimConnectCallback callback);
    void disconnect();
};

class SimConnectCallback {
public:
    void handlePlaneUpdate(WorldPosition planePos);
};