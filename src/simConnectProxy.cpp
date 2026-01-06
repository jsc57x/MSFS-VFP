/*
 * Visual Flight Path for Microsoft Flight Simulator 2024
 * Copyright (c) 2026 Jens Scharmann / Fernuniversität in Hagen
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "simConnectProxy.h"
#include "log.h"
#include "stringHelper.h"

#include "SimConnect.h"
#include <map>
#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>
#include <memory>


void SimConnectProxy::startSimConnectProxy(SimConnectCallback* callback)
{
    this->callback = callback;

    recvDataThread = std::thread(&SimConnectProxy::runSimConnectMessageLoop, this);
}

void SimConnectProxy::subscribeToEvents()
{
    if (FAILED(SimConnect_SubscribeToSystemEvent(hSimConnect, SIM_START, "SimStart")))
    {
        Logger::logError("Failed to register for SIM_START");
    }
    if (FAILED(SimConnect_SubscribeToSystemEvent(hSimConnect, SIM_STOP, "SimStop")))
    {
        Logger::logError("Failed to register for SIM_STOP");
    }

    // the simulation might be already running, so we have to poll once for the current state
    SimConnect_RequestSystemState(hSimConnect, SIM_STATE, "Sim");

    // Aircraft State
    SimConnect_AddToDataDefinition(hSimConnect, AIRCRAFT_STATE_DEFINITION, "PLANE LATITUDE", "degrees");
    SimConnect_AddToDataDefinition(hSimConnect, AIRCRAFT_STATE_DEFINITION, "PLANE LONGITUDE", "degrees");
    SimConnect_AddToDataDefinition(hSimConnect, AIRCRAFT_STATE_DEFINITION, "PLANE ALTITUDE", "feet");
    SimConnect_AddToDataDefinition(hSimConnect, AIRCRAFT_STATE_DEFINITION, "PLANE HEADING DEGREES TRUE", "degrees");
    SimConnect_AddToDataDefinition(hSimConnect, AIRCRAFT_STATE_DEFINITION, "PLANE BANK DEGREES", "degrees");
    SimConnect_AddToDataDefinition(hSimConnect, AIRCRAFT_STATE_DEFINITION, "PLANE PITCH DEGREES", "degrees");
    SimConnect_AddToDataDefinition(hSimConnect, AIRCRAFT_STATE_DEFINITION, "GROUND VELOCITY", "knots");
    
    SimConnect_RequestDataOnSimObject(hSimConnect, AIRCRAFT_STATE, AIRCRAFT_STATE_DEFINITION, SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_SECOND);
}

void SimConnectProxy::handleCommand(AbstractCommandConfiguration* command)
{
    if (!isSimulationActive())
    {
        Logger::logError("Command cannot be execute: Simulation is not running.");
        return;
    }

    if (command->getCommand() == Command::SET)
    {
        SetIndicatorCommandConfiguration* setCommand = static_cast<SetIndicatorCommandConfiguration*>(command);

        std::string indicatorType = getIndicatorTypeName(setCommand->getIndicatorTypeID());

        if (indicatorType.empty())
        {
            Logger::logError("Indicator type with id " + std::to_string(setCommand->getIndicatorTypeID()) + " does not exist.");
            return;
        }

        WorldPosition worldPosition = setCommand->getPosition();

        SIMCONNECT_DATA_INITPOSITION pos;
        pos.Latitude = worldPosition.getLatitude();
        pos.Longitude = worldPosition.getLongitude();
        pos.Altitude = worldPosition.getAltitude();
        pos.Heading = worldPosition.getHeading();
        pos.Bank = worldPosition.getBank();
        pos.Pitch = worldPosition.getPitch();
        pos.Airspeed = 0;
        pos.OnGround = 0;

        uint requestID = getNextRequestID();
        setRequestToIndicator(requestID, setCommand->getID());

        uint existingObjectID = getSimObjectByIndicator(setCommand->getID());
        if (existingObjectID != 0)
        {
            SimConnect_AIRemoveObject(hSimConnect, existingObjectID, getNextRequestID());
        }

        SimConnect_AICreateSimulatedObject_EX1(hSimConnect, indicatorType.c_str(), nullptr, pos, requestID);
    }
    else if (command->getCommand() == Command::REMOVE)
    {
        RemoveIndicatorsCommandConfiguration* removeCommand = static_cast<RemoveIndicatorsCommandConfiguration*>(command);
        std::vector<ushort> idsToRemove = removeCommand->getIDsToRemove();

        if (idsToRemove.size() == 0)
        {
            // remove all
            idsToRemove = getAllExistingIndicators();
        }
        removeIndicators(idsToRemove);
    }
    else {
        Logger::logError("Unknown command.");
    }
}

void SimConnectProxy::removeIndicators(std::vector<ushort> indicatorsToRemove)
{
    for (ushort id : indicatorsToRemove)
    {
        uint existingObjectID = getSimObjectByIndicator(id);
        if (existingObjectID != 0)
        {
            SimConnect_AIRemoveObject(hSimConnect, existingObjectID, getNextRequestID());
            removeIndicatorMapping(id);
        }
        else {
            Logger::logWarning("The indicator with ID " + std::to_string(id) + " cannot be removed because it is unknown.");
        }
    }
}

void SimConnectProxy::removeAllIndicators()
{
    removeIndicators(getAllExistingIndicators());
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

        ulong key = std::stoi(parts[0]);
        indicatorTypeMapping.insert({ key, parts[1] });
    }
}

const std::unordered_map<ulong, std::string>& SimConnectProxy::getIndicatorTypeMapping()
{
    if (this->indicatorTypeMapping.size() == 0)
    {
        this->initIndicatorTypeMapping();
    }

    return this->indicatorTypeMapping;
}

std::string SimConnectProxy::getIndicatorTypeName(ulong indicatorTypeID)
{
    std::unordered_map<ulong, std::string> typeMapping = getIndicatorTypeMapping();
    std::unordered_map<ulong, std::string>::const_iterator it = typeMapping.find(indicatorTypeID);
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

int SimConnectProxy::getNextRequestID()
{
    int nextID = nextRequestID.fetch_add(1);
    if (nextID <= 0)
    {
        nextID = 1000;
        nextRequestID.store(nextID);

    }
    return nextID;
}

void SimConnectProxy::stopSimConnectProxy()
{
    isRunning = false;
    SimConnect_Close(this->hSimConnect);
}

void SimConnectProxy::setRequestToIndicator(uint requestID, ushort indicatorID)
{
    std::scoped_lock lk(requestToIndicatorMutex);
    requestToIndicator[requestID] = indicatorID;
}

void SimConnectProxy::setIndicatorToSimObject(uint requestID, uint simObjectID)
{
    std::scoped_lock lk(requestToIndicatorMutex, indicatorToSimObjectMutex);
    ushort indicatorID = requestToIndicator[requestID];

    if (indicatorID == 0)
    {
        Logger::logWarning("Unknown indicator ID detected.");
        return;
    }

    requestToIndicator.erase(requestID);
    indicatorToSimObject[indicatorID] = simObjectID;
}

uint SimConnectProxy::getSimObjectByIndicator(ushort indicatorID)
{
    std::scoped_lock lk(indicatorToSimObjectMutex);
    try {
        return indicatorToSimObject.at(indicatorID);
    }
    catch (std::out_of_range e)
    {
        return 0;
    }
}

std::vector<ushort> SimConnectProxy::getAllExistingIndicators()
{
    std::scoped_lock lk(indicatorToSimObjectMutex);
    
    std::vector<ushort> keys(indicatorToSimObject.size());

    for (std::pair<ushort, uint> entry : indicatorToSimObject)
    {
        keys.push_back(entry.first);
    }

    return keys;
}

void SimConnectProxy::removeIndicatorMapping(ushort indicatorID)
{
    std::scoped_lock lk(indicatorToSimObjectMutex);
    indicatorToSimObject.erase(indicatorID);
}

void SimConnectProxy::connectCore()
{
    // as of dec 2025 there is a bug in SimConnect >= 1.4.5 (https://devsupport.flightsimulator.com/t/memory-leak-in-simconnect-open-function-sdk-1-4-5/17043)
    // so this only works with SimConnect SDK 1.2.4
    while (FAILED(SimConnect_Open(&this->hSimConnect, "Visual Flight Path", NULL, 0, 0, SIMCONNECT_OPEN_CONFIGINDEX_LOCAL)))
    {
        Sleep(10);
    }

    subscribeToEvents();
}

void SimConnectProxy::runSimConnectMessageLoop()
{
    connectCore();

    isRunning = true;
    
    SIMCONNECT_RECV* pData;
    DWORD cbData;
    HRESULT res;

    while (isRunning)
    {
        res = SimConnect_GetNextDispatch(hSimConnect, &pData, &cbData);

        if (res == E_FAIL)
        {
            //no Message in the queue -> wait and continue
            Sleep(1);
            continue;
        }

        if (SUCCEEDED(res))
        {
            handleSimConnectMessageCore(pData);
        }
    }
}

void SimConnectProxy::handleSimConnectMessageCore(SIMCONNECT_RECV* pData)
{
    switch (pData->dwID)
    {
        // Simulation started and stopped events do not occur as expected
        // According to https://docs.flightsimulator.com/msfs2024/html/6_Programming_APIs/SimConnect/API_Reference/Events_And_Data/SimConnect_SubscribeToSystemEvent.htm
        // the system event SimStart should be send if the user is in control of an aircraft but it is also send when MSFS starts and is in main menu
        // -> its currently seems impossible to determine the active running state of the simulation
       case SIMCONNECT_RECV_ID_EVENT : // Simulation started or stopped
       {
           SIMCONNECT_RECV_EVENT* evt = reinterpret_cast<SIMCONNECT_RECV_EVENT*>(pData);
           switch (evt->uEventID)
           {
                case SIM_START:
                {
                    Logger::logInfo("Simulation started");
                    simulationIsActive.store(true, std::memory_order_release);
                    break;
                }
                case SIM_STOP:
                {
                    Logger::logInfo("Simulation stopped");
                    simulationIsActive.store(false, std::memory_order_release);
                    removeAllIndicators();
                    break;
                }
           }
           break;
       }
       case SIMCONNECT_RECV_ID_ASSIGNED_OBJECT_ID: // object created with given id
       {
           SIMCONNECT_RECV_ASSIGNED_OBJECT_ID* aoi = reinterpret_cast<SIMCONNECT_RECV_ASSIGNED_OBJECT_ID*>(pData);

           this->setIndicatorToSimObject(aoi->dwRequestID, aoi->dwObjectID);
           
           break;
       }
       case SIMCONNECT_RECV_ID_SYSTEM_STATE: // current state of the simulation
       {
           SIMCONNECT_RECV_SYSTEM_STATE* st = reinterpret_cast<SIMCONNECT_RECV_SYSTEM_STATE*>(pData);
           if (st->dwRequestID == SIM_STATE)
           {
               simulationIsActive.store(st->dwInteger == 1, std::memory_order_release);
           }
           break;
       }
       case SIMCONNECT_RECV_ID_SIMOBJECT_DATA: // polled aircraft information
       {
           if (!isSimulationActive())
           {
               // The position will also be provided if the simulation is in menus :(
               return;
           }
           SIMCONNECT_RECV_SIMOBJECT_DATA* pObjData = (SIMCONNECT_RECV_SIMOBJECT_DATA*)pData;
           switch (pObjData->dwRequestID)
           {
           case AIRCRAFT_STATE:
               AircraftStateStruct tmp{};
               std::memcpy(&tmp, &pObjData->dwData, sizeof(tmp));

               this->callback->handleAircraftStateUpdate(AircraftState{ tmp });
               break;
           }
           break;
       }
       case SIMCONNECT_RECV_ID_QUIT:
       {
           simulationIsActive.store(false, std::memory_order_release);

           Logger::logInfo("SimConnect connection closed. Waiting for new connection.");
           // clear all mappings

           { // section for scoped lock
               std::scoped_lock lk(indicatorToSimObjectMutex);
               indicatorToSimObject.clear();
           }

           // waiting for new connection
           connectCore();

           Logger::logInfo("SimConnect connection reestablished.");
           break;
       }
    }
}

