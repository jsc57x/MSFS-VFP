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

    u16 getID();
    u32 getIndicatorTypeID();
         
    f64 getLatitude();
    f64 getLongitude();
    f64 getAltitude();
    f64 getHeading();
    f64 getBank();
    f64 getPitch();

    std::string toString() override;

private:
    u16 id;
    u32 indicatorTypeID;
    f64 latitude;
    f64 longitude;
    f64 altitude;
    f64 heading;
    f64 bank;
    f64 pitch;

    bool validate();
};

class RemoveIndicatorsCommandConfiguration : public UDPCommandConfiguration
{
public:
    static std::unique_ptr<RemoveIndicatorsCommandConfiguration> parse(char* array, u32 length);

    UDPCommand getCommand() override {
        return UDPCommand::REMOVE;
    }

    void addIDToRemove(u16 id);
    std::vector<u16> getIDsToRemove();

    std::string toString() override;

private:
    std::vector<u16> idsToRemove;

    bool validate();
};