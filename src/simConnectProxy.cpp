#include "simConnectProxy.h"
#include "log.h"

#include "SimConnect.h"

SimConnectProxy::SimConnectProxy(SimConnectCallback* callback)
{
    this->callback = callback;
}

int SimConnectProxy::connect()
{
    HRESULT hr;
    hr = SimConnect_Open(&this->hSimConnect, "Visual Flight Path", NULL, 0, 0, SIMCONNECT_OPEN_CONFIGINDEX_LOCAL);

    return 0;
}

void SimConnectProxy::handleCommand(UDPCommandConfiguration* command)
{
    // open connection lazy (and prevent problems wenn connection is closed unexpectedly)
    if (!isConnectionOpen())
    {
        if (!SUCCEEDED(connect()))
        {
            Logger::logError("Command cannot be execute: Connection is closed.");
            return;
        }
    }

    if (!isSimulationActive())
    {
        Logger::logError("Command cannot be execute: Simulation is not running.");
        return;
    }

    // TODO implement
}

bool SimConnectProxy::isConnectionOpen()
{
    return this->hSimConnect != NULL;
}

bool SimConnectProxy::isSimulationActive()
{
    const char* szState = 0;
    SimConnect_RequestSystemState(this->hSimConnect, 0, szState);

    return false;
}

void SimConnectProxy::disconnect()
{
    SimConnect_Close(this->hSimConnect);
}