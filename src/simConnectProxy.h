#pragma once

#include "datatypes.h"
#include "udpCommand.h"

#include "windows.h"
#include "SimConnect.h"
#include <map>
#include <vector>

class SimConnectCallback {
public:
    virtual ~SimConnectCallback() = default;
    virtual void handlePlaneUpdate(/*WorldPosition planePos*/) = 0;
};

class SimConnectProxy {
public:
    SimConnectProxy(SimConnectCallback* callback);
    void handleCommand(UDPCommandConfiguration* command);
    void resetIndicatorTypeMapping();
    void disconnect();

private:
    SimConnectCallback* callback;
    HANDLE hSimConnect = NULL;
    
    std::map<u64, std::string> indicatorTypeMapping;

    //FIXME: Value will be set (and maybe requested) with the plane information (via message dispatch)
    bool simulationIsRunning = true;

    int connect();
    bool isConnectionOpen();
    bool isSimulationActive();

    void initIndicatorTypeMapping();
    const std::map<u64, std::string>& getIndicatorTypeMapping();
    std::string getIndicatorTypeName(u64 indicatorTypeID);

    std::vector<std::string> splitString(const std::string& s, char delimiter);
};