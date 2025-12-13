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

    if (command->getCommand() == UDPCommand::SET)
    {
        SetIndicatorCommandConfiguration* setCommand = static_cast<SetIndicatorCommandConfiguration*>(command);
        SIMCONNECT_DATA_INITPOSITION pos;
        pos.Latitude = setCommand->getLatitude();
        pos.Longitude = setCommand->getLongitude();
        pos.Altitude = setCommand->getHeight();
        pos.Heading = setCommand->getPitch(); // FIXME Anpassen der Werte 
        pos.Bank = setCommand->getRoll();
        pos.Pitch = setCommand->getYaw();
        pos.Airspeed = 0;
        pos.OnGround = 0;

        SimConnect_AICreateSimulatedObject_EX1(hSimConnect, "VFP_Circle_S", nullptr, pos, 1);
    }
    else if (command->getCommand() == UDPCommand::REMOVE)
    {
    // TODO implement
    }
    else {
        Logger::logError("Unknown command.");
    }
}

bool SimConnectProxy::isConnectionOpen()
{
    return this->hSimConnect != NULL;
}

bool SimConnectProxy::isSimulationActive()
{
    return simulationIsRunning;
}

void SimConnectProxy::disconnect()
{
    SimConnect_Close(this->hSimConnect);
}