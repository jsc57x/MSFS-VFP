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

class SimConnectCallback {
public:
    virtual ~SimConnectCallback() = default;
    virtual void handlePlaneUpdate(AircraftState aircraftState) = 0;
};

class SimConnectProxy {
public:
    bool startSimConnectProxy(SimConnectCallback* callback);
    void stopSimConnectProxy();

    void handleCommand(AbstractCommandConfiguration* command);

    void removeAllIndicators();

    void resetIndicatorTypeMapping();

private:
    SimConnectCallback* callback;
    HANDLE hSimConnect = NULL;
    
    std::unordered_map<ulong, std::string> indicatorTypeMapping;

    std::atomic_bool isRunning{ false };
    std::atomic_bool simulationIsActive{ false };
    std::atomic_int nextRequestID{ 1000 }; // < 1000 will be reserved for system events that are actively polled by this application

    std::unordered_map<uint, ushort> requestToIndicator;
    std::unordered_map<ushort, uint> indicatorToSimObject;

    std::mutex requestToIndicatorMutex;
    std::mutex indicatorToSimObjectMutex;

    std::thread recvDataThread;

    bool isSimulationActive();

    int getNextRequestID();

    void initIndicatorTypeMapping();
    const std::unordered_map<ulong, std::string>& getIndicatorTypeMapping();
    std::string getIndicatorTypeName(ulong indicatorTypeID);

    void setRequestToIndicator(uint requestID, ushort indicatorID);
    void setIndicatorToSimObject(uint requestID, uint simObjectID);
    uint getSimObjectByIndicator(ushort indicatorID);
    std::vector<ushort> getAllExistingIndicators();
    void removeIndicators(std::vector<ushort> indicatorsToRemove);
    void removeIndicatorMapping(ushort indicatorID);

    void connectCore();
    void runSimConnectMessageLoop();
    void subscribeToEvents();
    static void CALLBACK handleSimConnectMessage(SIMCONNECT_RECV* pData, DWORD cbData, void* pContext);
    void handleSimConnectMessageCore(SIMCONNECT_RECV* pData, DWORD cbData);

    std::vector<std::string> splitString(const std::string& s, char delimiter);
};

enum EventIDs : uint {
    SIM_START = 1,
    SIM_STOP = 2,
};

enum ReservedRequestIDs : uint {
    SIM_STATE = 100,
    AIRCRAFT_STATE = 200,
};

enum DATA_DEFINE_ID {
    AIRCRAFT_STATE_DEFINITION,
};