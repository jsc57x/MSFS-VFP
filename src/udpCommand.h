#pragma once

#include "datatypes.h"
#include "indicator.h"
#include <string>
#include <vector>

enum UDPCommand { SET, REMOVE };

class UDPCommandConfiguration {
public:
    virtual UDPCommand getCommand() = 0;
    virtual ~UDPCommandConfiguration() = default;
};

class SetIndicatorCommandConfiguration : public UDPCommandConfiguration
{
public:
    SetIndicatorCommandConfiguration();

    UDPCommand getCommand() override {
        return UDPCommand::SET;
    }
    void setID(u32 id);
    void setIndicator(Indicator indicator);

    void setLatitude(f64 latitude);
    void setLongitude(f64 longitude);
    void setHeight(f64 height);
    void setRoll(f64 roll);
    void setPitch(f64 pitch);
    void setYaw(f64 yaw);

    void setFromByteArray(char* array, u32 length);

    u32 getID();
    Indicator getIndicator();
         
    f64 getLatitude();
    f64 getLongitude();
    f64 getHeight();
    f64 getRoll();
    f64 getPitch();
    f64 getYaw();

private:
    u32 id;
    Indicator indicator;
    f64 latitude;
    f64 longitude;
    f64 height;
    f64 roll;
    f64 pitch;
    f64 yaw;
};

class RemoveIndicatorsCommandConfiguration : public UDPCommandConfiguration
{
public:
    RemoveIndicatorsCommandConfiguration();

    UDPCommand getCommand() override {
        return UDPCommand::REMOVE;
    }

    void addIDToRemove(u32 id);
    std::vector<int> getIDsToRemove();

private:
    std::vector<int> idsToRemove;
};