#pragma once

#include "datatypes.h"
#include <string>
#include <vector>
#include <memory>

enum UDPCommand { SET, REMOVE };

class UDPCommandConfiguration {
public:
    virtual UDPCommand getCommand() = 0;
    virtual std::string toString() = 0;
    virtual ~UDPCommandConfiguration() = default;
};

class SetIndicatorCommandConfiguration : public UDPCommandConfiguration
{
public:
    static std::unique_ptr<SetIndicatorCommandConfiguration> parse(char* array);

    UDPCommand getCommand() override {
        return UDPCommand::SET;
    }

    ushort getID();
    uint getIndicatorTypeID();
         
    double getLatitude();
    double getLongitude();
    double getAltitude();
    double getHeading();
    double getBank();
    double getPitch();

    std::string toString() override;

private:
    ushort id;
    uint indicatorTypeID;
    double latitude;
    double longitude;
    double altitude;
    double heading;
    double bank;
    double pitch;

    bool validate();
};

class RemoveIndicatorsCommandConfiguration : public UDPCommandConfiguration
{
public:
    static std::unique_ptr<RemoveIndicatorsCommandConfiguration> parse(char* array, uint length);

    UDPCommand getCommand() override {
        return UDPCommand::REMOVE;
    }

    void addIDToRemove(ushort id);
    std::vector<ushort> getIDsToRemove();

    std::string toString() override;

private:
    std::vector<ushort> idsToRemove;

    bool validate();
};