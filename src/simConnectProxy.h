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
#pragma once

#include "datatypes.h"
#include "udpCommand.h"
#include "aircraftState.h"

#include "windows.h"
#include "SimConnect.h"
#include <unordered_map>
#include <vector>
#include <thread>
#include <mutex>
#include <optional>

/// <summary>
/// Callback for status updates from the SimConnect-API
/// </summary>
class SimConnectCallback {
public:
    /// <summary>
    /// Virtual destructor
    /// </summary>
    virtual ~SimConnectCallback() = default;

    /// <summary>
    /// Handles a new plane status update.
    /// </summary>
    /// <param name="aircraftState">The current position, orientation, and speed of the simulated aircraft</param>
    virtual void handleAircraftStateUpdate(AircraftState aircraftState) = 0;
};

/// <summary>
/// Proxy class to communicate with the SimConnect-API
/// </summary>
class SimConnectProxy {
public:
    /// <summary>
    /// Starts a new thread to connect with the SimConnect-API. If the API is not available the thread tries until it is stopped.
    /// Commands cannot be executed until the connection is established.
    /// The new thread then handles the SimConnect events using the SimConnectCallback.
    /// </summary>
    /// <param name="callback">Callback to handle aircraft status updates</param>
    void startSimConnectProxy(SimConnectCallback* callback);

    /// <summary>
    /// Stops the SimConnectProxy. That means it stops the thread and if the connection to SimConnect has been established it is
    /// disconnected.
    /// </summary>
    void stopSimConnectProxy();

    /// <summary>
    /// Executes a command based on the given command configuration.
    /// </summary>
    /// <param name="command">Command configuration</param>
    void handleCommand(AbstractCommandConfiguration* command);

    /// <summary>
    /// Removes all indicators which are known by this instance.
    /// </summary>
    void removeAllIndicators();

    /// <summary>
    /// Resets the indicator type mapping. The mapping will be updated before the next indicator is placed in the simulation.
    /// </summary>
    void resetIndicatorTypeMapping();

private:
    /// <summary>
    /// Callback for aircraft status updates.
    /// </summary>
    SimConnectCallback* callback;

    /// <summary>
    /// Handle for the connection to SimConnect
    /// </summary>
    HANDLE hSimConnect = NULL;
    

    /// <summary>
    /// Mapping: indicator type id -> model name
    /// </summary>
    std::unordered_map<ulong, std::string> indicatorTypeMapping;

    /// <summary>
    /// Indicates the status of the thread.
    /// </summary>
    std::atomic_bool isRunning{ false };

    /// <summary>
    /// Indicates if the SimConnect connection is established and the simulation is running
    /// </summary>
    std::atomic_bool simulationIsActive{ false };

    /// <summary>
    /// Next request id which should be used for SimConnect commands. The next id should be polled by using
    /// getNextRequestID to prevent collisions with reserved request ids.
    /// </summary>
    std::atomic_int nextRequestID{ 1000 }; // < 1000 will be reserved for system events that are actively polled by this application

    
    /// <summary>
    /// Mapping: SimConnect request id -> external indicator id
    /// </summary>
    std::unordered_map<uint, ushort> requestToIndicator;
    
    /// <summary>
    /// Mapping: external indicator id -> SimConnect handle of the SimObject
    /// </summary>
    std::unordered_map<ushort, uint> indicatorToSimObject;


    /// <summary>
    /// Mutex for accessing the request id to external indicator id mapping.
    /// </summary>
    std::mutex requestToIndicatorMutex;

    /// <summary>
    /// Mutex for accessing the external indicator id to SimObject id mapping.
    /// </summary>
    std::mutex indicatorToSimObjectMutex;


    /// <summary>
    /// Thread for polling the SimConnect message queue.
    /// </summary>
    std::thread recvDataThread;

    /// <summary>
    /// Returns true if the simulation is active.
    /// </summary>
    /// <returns>Status of the simulation</returns>
    bool isSimulationActive();


    /// <summary>
    /// Gets the next request id and increase the counter. If the counter overflows it is resetted to the first not reserved values.
    /// </summary>
    /// <returns>Id for the next request</returns>
    int getNextRequestID();


    /// <summary>
    /// Initializes the mapping indicator type id -> model name
    /// </summary>
    void initIndicatorTypeMapping();

    /// <summary>
    /// Gets the indicator type mapping and initializes it if necessary.
    /// </summary>
    /// <returns>Mapping: indicator type id -> model name</returns>
    const std::unordered_map<ulong, std::string>& getIndicatorTypeMapping();
    
    /// <summary>
    /// Returns the model name for the give indicator type id or an empty string if the indicator type id is not mapped.
    /// </summary>
    /// <param name="indicatorTypeID">Requested indicator type id</param>
    /// <returns>Model name or empty string</returns>
    std::string getIndicatorTypeName(ulong indicatorTypeID);

    /// <summary>
    /// Sets the mapping request id -> external indicator id
    /// </summary>
    /// <param name="requestID">The request id which is used for the SimConnect request</param>
    /// <param name="indicatorID">The external indicator id</param>
    void setRequestToIndicator(uint requestID, ushort indicatorID);

    /// <summary>
    /// Sets the mapping indicator id -> SimObject id. The indicator id is determined by the used SimConnect request id.
    /// </summary>
    /// <param name="requestID">The SimConnect request id which was used to create the SimObject</param>
    /// <param name="simObjectID">The SimObject id which was created</param>
    void setIndicatorToSimObject(uint requestID, uint simObjectID);
    
    /// <summary>
    /// Retuns the SimObject id for the external indicator id or 0 if the external id is unknown.
    /// </summary>
    /// <param name="indicatorID">The external indicator id</param>
    /// <returns>SimObject id</returns>
    uint getSimObjectByIndicator(ushort indicatorID);

    /// <summary>
    /// Returns the list of all known external indicator ids.
    /// </summary>
    /// <returns>List with external indicator ids</returns>
    std::vector<ushort> getAllExistingIndicators();

    /// <summary>
    /// Removes the indicators for the given list of external indicator ids.
    /// </summary>
    /// <param name="indicatorsToRemove">List with external indicator ids to remove</param>
    void removeIndicators(std::vector<ushort> indicatorsToRemove);
    
    /// <summary>
    /// Removes the mapping external indicator id -> SimObject id for the given external indicator id.
    /// </summary>
    /// <param name="indicatorID">External indicator id to be removed</param>
    void removeIndicatorMapping(ushort indicatorID);


    /// <summary>
    /// Opens the connection to SimConnect. If SimConnect is not available the method tries again.
    /// The method returns when the connection was established.
    /// </summary>
    void connectCore();

    /// <summary>
    /// Connects to SimConnect and handles the SimConnect message queue.
    /// </summary>
    void runSimConnectMessageLoop();

    /// <summary>
    /// Subscribes to SimConnect system events and updated aircraft information.
    /// </summary>
    void subscribeToEvents();
    
    /// <summary>
    /// Proxy method to send SimConnect messages to the SimConnectProxy instance.
    /// </summary>
    /// <param name="pData">Pointer to the data buffer</param>
    /// <param name="cbData">Pointer to the size of the data buffer</param>
    /// <param name="pContext">The "context" which is the instance of SimConnectProxy</param>
    static void CALLBACK handleSimConnectMessage(SIMCONNECT_RECV* pData, DWORD cbData, void* pContext);

    /// <summary>
    /// Core method to handle the SimConnect message.
    /// </summary>
    /// <param name="pData">Pointer to the data buffer</param>
    void handleSimConnectMessageCore(SIMCONNECT_RECV* pData);
};

/// <summary>
/// Well-known event ids which occur while communicating with the SimConnect-API.
/// </summary>
enum EventIDs : uint {
    SIM_START = 1,
    SIM_STOP = 2,
};

/// <summary>
/// Well-known request ids which are used to poll status updates from the SimConnect-API.
/// </summary>
enum ReservedRequestIDs : uint {
    SIM_STATE = 100,
    AIRCRAFT_STATE = 200,
};

/// <summary>
/// Well-known data definition ids to exchange data with die SimConnect-API.
/// </summary>
enum DATA_DEFINE_ID {
    AIRCRAFT_STATE_DEFINITION,
};