#include "simConnectProxy.h"
#include "log.h"

#include "SimConnect.h"
#include <map>
#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>

bool SimConnectProxy::startSimConnectProxy(SimConnectCallback* callback)
{
    this->callback = callback;

    // as of dec 2025 there is a bug in SimConnect >= 1.4.5 (https://devsupport.flightsimulator.com/t/memory-leak-in-simconnect-open-function-sdk-1-4-5/17043)
    
    while (FAILED(SimConnect_Open(&this->hSimConnect, "Visual Flight Path", NULL, 0, 0, SIMCONNECT_OPEN_CONFIGINDEX_LOCAL)))
    {
        Sleep(10);
    }

    subscribeToEvents();

    recvDataThread = std::thread(&SimConnectProxy::runSimConnectMessageLoop, this);

    while (!connected.load(std::memory_order_acquire))
        Sleep(10);

    return 0;
}

void SimConnectProxy::subscribeToEvents()
{
    SimConnect_SubscribeToSystemEvent(hSimConnect, EVT_SIM_START, "SimStart");
    SimConnect_SubscribeToSystemEvent(hSimConnect, EVT_SIM_STOP, "SimStop");
}

void SimConnectProxy::handleCommand(UDPCommandConfiguration* command)
{
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
        pos.Altitude = setCommand->getAltitude();
        pos.Heading = setCommand->getHeading();
        pos.Bank = setCommand->getBank();
        pos.Pitch = setCommand->getPitch();
        pos.Airspeed = 0;
        pos.OnGround = 0;

        std::string indicatorType = getIndicatorTypeName(setCommand->getIndicatorTypeID());

        if (indicatorType.empty())
        {
            Logger::logError("Indicator type with id " + std::to_string(setCommand->getIndicatorTypeID()) + " does not exist.");
        }

        SimConnect_AICreateSimulatedObject_EX1(hSimConnect, indicatorType.c_str(), nullptr, pos, 1);
    }
    else if (command->getCommand() == UDPCommand::REMOVE)
    {
    // TODO implement
    }
    else {
        Logger::logError("Unknown command.");
    }
}

void SimConnectProxy::resetIndicatorTypeMapping()
{
    this->indicatorTypeMapping.clear();
}

void SimConnectProxy::initIndicatorTypeMapping()
{
    std::ifstream indicatorMappingFile("indicators.properties");

    if (!indicatorMappingFile.good())
    {
        Logger::logError("Indicator mappings could not be loaded.");
        return;
    }

    while (indicatorMappingFile.good())
    {
        std::string row;
        std::getline(indicatorMappingFile, row);

        std::vector<std::string> parts = splitString(row, '=');

        if (parts.size() != 2) // just a warning if row does not consist of two parts 
        {
            Logger::logWarning("Invalid indicator mapping: " + row);
            continue;
        }

        u64 key = std::stoi(parts[0]);
        indicatorTypeMapping.insert({ key, parts[1] });
    }
}

const std::map<u64, std::string>& SimConnectProxy::getIndicatorTypeMapping()
{
    if (this->indicatorTypeMapping.size() == 0)
    {
        this->initIndicatorTypeMapping();
    }

    return this->indicatorTypeMapping;
}

std::string SimConnectProxy::getIndicatorTypeName(u64 indicatorTypeID)
{
    std::map<u64, std::string> typeMapping = getIndicatorTypeMapping();
    std::map<u64, std::string>::const_iterator it = typeMapping.find(indicatorTypeID);
    if (it == typeMapping.end())
    {
        // nothing found -> return empty string
        return "";
    }

    return it->second; // return value
}

bool SimConnectProxy::isSimulationActive()
{
    return simulationIsActive.load(std::memory_order_acquire);
}

void SimConnectProxy::stopSimConnectProxy()
{
    isRunning = false;
    SimConnect_Close(this->hSimConnect);
}

void SimConnectProxy::runSimConnectMessageLoop()
{
    isRunning = true;
    while (isRunning)
    {
        SimConnect_CallDispatch(hSimConnect, SimConnectProxy::handleSimConnectMessage, this);
        Sleep(1);
    }
}

void CALLBACK SimConnectProxy::handleSimConnectMessage(SIMCONNECT_RECV* pData, DWORD cbData, void* pContext)
{
    auto* self = static_cast<SimConnectProxy*>(pContext);
    if (self) self->handleSimConnectMessageCore(pData, cbData);
}

void SimConnectProxy::handleSimConnectMessageCore(SIMCONNECT_RECV* pData, DWORD cbData)
{
    switch (pData->dwID)
    {
       case SIMCONNECT_RECV_ID_OPEN :
       {
           Logger::logInfo("SimConnect Client available");
           connected.store(true, std::memory_order_release);
           break;
       } 
       case SIMCONNECT_RECV_ID_EVENT :
       {
           SIMCONNECT_RECV_EVENT* evt = reinterpret_cast<SIMCONNECT_RECV_EVENT*>(pData);
           switch (evt->uEventID)
           {
                case EVT_SIM_START:
                {
                    Logger::logInfo("Simulation started");
                    simulationIsActive.store(true, std::memory_order_release);
                    break;
                }
                case EVT_SIM_STOP:
                {
                    Logger::logInfo("Simulation stopped");
                    simulationIsActive.store(false, std::memory_order_release);
                    break;
                }
           }
           break;
       }
    }
}

std::vector<std::string> SimConnectProxy::splitString(const std::string& s, char delimiter)
{
    std::vector<std::string> tokens;
    std::stringstream ss(s);
    std::string part;

    while (std::getline(ss, part, delimiter)) {
        tokens.push_back(part);
    }

    return tokens;
}