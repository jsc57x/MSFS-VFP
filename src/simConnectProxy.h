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
    virtual void handlePlaneUpdate(/*WorldPosition planePos*/) = 0;
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

    // FIXME currently the map potentially grow to 2^32 as this is the size of RequestIDs, this could be much smaller...
    std::unordered_map<u32, u32> requestToIndicator;
    std::unordered_map<u32, u32> indicatorToSimObject;

    std::mutex requestToIndicatorMutex;
    std::mutex indicatorToSimObjectMutex;

    std::thread recvDataThread;

    bool isSimulationActive();

    void initIndicatorTypeMapping();
    const std::unordered_map<u64, std::string>& getIndicatorTypeMapping();
    std::string getIndicatorTypeName(u64 indicatorTypeID);

    void setRequestToIndicator(u32 requestID, u32 indicatorID);
    void setIndicatorToSimObject(u32 requestID, u32 simObjectID);
    void removeIndicatorMapping(u32 indicatorID);

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
};