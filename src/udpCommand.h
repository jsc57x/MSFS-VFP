#pragma once

#include "datatypes.h"
#include "worldPosition.h"
#include <string>
#include <vector>
#include <memory>

enum Command { SET, REMOVE };

class AbstractCommandConfiguration {
public:
    virtual Command getCommand() = 0;
    virtual std::string toString() = 0;
};

class SetIndicatorCommandConfiguration : public AbstractCommandConfiguration
{
public:
    static std::unique_ptr<SetIndicatorCommandConfiguration> parse(char* array);

    Command getCommand() override {
        return Command::SET;
    }

    ushort getID();
    uint getIndicatorTypeID();
    WorldPosition getPosition();

    std::string toString() override;

private:
    ushort id;
    uint indicatorTypeID;
    WorldPosition position;

    SetIndicatorCommandConfiguration(ushort id, 
                                     uint indicatorTypeID, 
                                     WorldPosition worldPosition) 
    : id(id), indicatorTypeID(indicatorTypeID), position(worldPosition) {};
    
    bool validate();
};

class RemoveIndicatorsCommandConfiguration : public AbstractCommandConfiguration
{
public:
    Command getCommand() override {
        return Command::REMOVE;
    }
    std::string toString() override;
    
    static std::unique_ptr<RemoveIndicatorsCommandConfiguration> parse(char* array, uint length);
    std::vector<ushort> getIDsToRemove();

private:
    RemoveIndicatorsCommandConfiguration();
    std::vector<ushort> idsToRemove;
};