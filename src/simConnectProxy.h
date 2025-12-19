#pragma once

#include "datatypes.h"
#include "udpCommand.h"

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
    virtual void handlePlaneUpdate(AircraftState* aircraftState) = 0;
};

class SimConnectProxy {
public:
    bool startSimConnectProxy(SimConnectCallback* callback);
    void stopSimConnectProxy();

    void handleCommand(UDPCommandConfiguration* command);
    void resetIndicatorTypeMapping();

private:
    SimConnectCallback* callback;
    HANDLE hSimConnect = NULL;
    
    std::unordered_map<u64, std::string> indicatorTypeMapping;

    //FIXME: Value will be set (and maybe requested) with the plane information (via message dispatch)
    std::atomic_bool isRunning{ false };
    std::atomic_bool connected{ false };
    std::atomic_bool simulationIsActive{ false };
    // FIXME if there are more then ~2^32 requests this will finally become 1 and interfere with the reserved IDs
    std::atomic_int nextRequestID{ 1000 }; // < 1000 will be reserved for system events that are actively polled by this application

    std::unordered_map<u32, u16> requestToIndicator;
    std::unordered_map<u16, u32> indicatorToSimObject;

    std::mutex requestToIndicatorMutex;
    std::mutex indicatorToSimObjectMutex;

    std::thread recvDataThread;

    bool isSimulationActive();

    void initIndicatorTypeMapping();
    const std::unordered_map<u64, std::string>& getIndicatorTypeMapping();
    std::string getIndicatorTypeName(u64 indicatorTypeID);

    void setRequestToIndicator(u32 requestID, u16 indicatorID);
    void setIndicatorToSimObject(u32 requestID, u32 simObjectID);
    u32 getSimObjectByIndicator(u16 indicatorID);
    std::vector<u16> getAllExistingIndicators();
    void removeIndicatorMapping(u16 indicatorID);

    void runSimConnectMessageLoop();
    void subscribeToEvents();
    static void CALLBACK handleSimConnectMessage(SIMCONNECT_RECV* pData, DWORD cbData, void* pContext);
    void handleSimConnectMessageCore(SIMCONNECT_RECV* pData, DWORD cbData);

    std::vector<std::string> splitString(const std::string& s, char delimiter);
};

enum EventIDs : u32 {
    SIM_START = 1,
    SIM_STOP = 2,
    PAUSE = 3,
};

enum ReservedRequestIDs : u32 {
    SIM_STATE = 100,
    AIRCRAFT_STATE = 200,
};

enum DATA_DEFINE_ID {
    AIRCRAFT_STATE_DEFINITION,
};