#pragma once

#include "datatypes.h"
#include "udpCommand.h"

#include "windows.h"
#include "SimConnect.h"
#include <map>
#include <vector>
#include <thread>

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
    
    std::map<u64, std::string> indicatorTypeMapping;

    //FIXME: Value will be set (and maybe requested) with the plane information (via message dispatch)
    std::atomic_bool isRunning{ false };
    std::atomic_bool connected{ false };
    std::atomic_bool simulationIsActive{ false };

    bool isSimulationActive();
    

    std::thread recvDataThread;

    void initIndicatorTypeMapping();

    const std::map<u64, std::string>& getIndicatorTypeMapping();
    std::string getIndicatorTypeName(u64 indicatorTypeID);

    void runSimConnectMessageLoop();
    void subscribeToEvents();
    static void CALLBACK handleSimConnectMessage(SIMCONNECT_RECV* pData, DWORD cbData, void* pContext);
    void handleSimConnectMessageCore(SIMCONNECT_RECV* pData, DWORD cbData);

    std::vector<std::string> splitString(const std::string& s, char delimiter);
};

enum EVENT_ID : DWORD {
    EVT_SIM_START = 1,
    EVT_SIM_STOP = 2,
    EVT_PAUSE = 3,
};