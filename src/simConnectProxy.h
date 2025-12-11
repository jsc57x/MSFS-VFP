#pragma once

#include "datatypes.h"
#include "udpCommand.h"

#include "windows.h"
#include "SimConnect.h"

class SimConnectCallback {
public:
    virtual ~SimConnectCallback() = default;
    virtual void handlePlaneUpdate(/*WorldPosition planePos*/) = 0;
};

class SimConnectProxy {
public:
    SimConnectProxy(SimConnectCallback* callback);
    void handleCommand(UDPCommandConfiguration* command);
    void disconnect();

private:
    SimConnectCallback* callback;
    HANDLE hSimConnect = NULL;

    int connect();
    bool isConnectionOpen();
    bool isSimulationActive();
};