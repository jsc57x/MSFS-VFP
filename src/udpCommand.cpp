#include "udpCommand.h"
#include "flightPathVisualizer.h"
#include "log.h"
#include "doubleHelper.h"

#include <string>
#include <memory>

#define LATITUDE_MIN -90.0
#define LATITUDE_MAX 90.0

#define LONGITUDE_MIN -180.0
#define LONGITUDE_MAX 180.0

//////////////
///   SET  ///
//////////////

std::unique_ptr<SetIndicatorCommandConfiguration> SetIndicatorCommandConfiguration::parse(char* array)
{
    ushort id;
    uint indicatorTypeID;
    double latitude;
    double longitude;
    double altitude;
    double heading;
    double bank;
    double pitch;

    memcpy(&id, array + 2, sizeof(id));
    memcpy(&indicatorTypeID, array + 4, sizeof(indicatorTypeID));
    memcpy(&latitude, array + 8, sizeof(latitude));
    memcpy(&longitude, array + 16, sizeof(longitude));
    memcpy(&altitude, array + 24, sizeof(altitude));
    memcpy(&heading, array + 32, sizeof(heading));
    memcpy(&bank, array + 40, sizeof(bank));
    memcpy(&pitch, array + 48, sizeof(pitch));

    WorldPosition worldPos = WorldPosition(latitude, longitude, altitude, heading, bank, pitch);
    SetIndicatorCommandConfiguration commandConfig = SetIndicatorCommandConfiguration(id, indicatorTypeID, worldPos);


    if (commandConfig.validate())
    {
    return std::make_unique<SetIndicatorCommandConfiguration>(commandConfig);
    }
    else {
        return nullptr;
    }

}

bool SetIndicatorCommandConfiguration::validate()
{
    if (!isDoubleInRange(position.getLatitude(), LATITUDE_MIN, LATITUDE_MAX))
    {
        Logger::logError("Latitude is out of range: " + std::to_string(position.getLatitude()));
        return false;
    }

    if (!isDoubleInRange(position.getLongitude(), LONGITUDE_MIN, LONGITUDE_MAX))
    {
        Logger::logError("Longitude is out of range: " + std::to_string(position.getLongitude()));
        return false;
    }

    return true;
}

ushort SetIndicatorCommandConfiguration::getID()
{
    return this->id;
}

uint SetIndicatorCommandConfiguration::getIndicatorTypeID()
{
    return this->indicatorTypeID;
}

WorldPosition SetIndicatorCommandConfiguration::getPosition()
{
    return this->position;
}

std::string SetIndicatorCommandConfiguration::toString()
{
    return "Set Indicator: Indicator " + std::to_string(id) + 
        " of type " + std::to_string(indicatorTypeID) + 
        " Lat: " + std::to_string(position.getLatitude()) + 
        ", Long: " + std::to_string(position.getLongitude()) +
        ", Height: " + std::to_string(position.getAltitude()) + 
        ", Roll: " + std::to_string(position.getHeading()) + 
        ", Pitch: " + std::to_string(position.getBank()) +
        ", Yaw: " + std::to_string(position.getPitch());
}

//////////////
/// REMOVE ///
//////////////
std::unique_ptr<RemoveIndicatorsCommandConfiguration> RemoveIndicatorsCommandConfiguration::parse(char* array, uint length)
{
    RemoveIndicatorsCommandConfiguration command;

    for (uint curMemOffset = 2; curMemOffset < length; curMemOffset = curMemOffset + sizeof(ushort))
    {
        ushort id;
        memcpy(&id, array + curMemOffset, sizeof(ushort));
        command.idsToRemove.push_back(id);
    }

    return std::make_unique<RemoveIndicatorsCommandConfiguration>(command);
}

std::vector<ushort> RemoveIndicatorsCommandConfiguration::getIDsToRemove()
{
    return this->idsToRemove;
}

std::string RemoveIndicatorsCommandConfiguration::toString()
{
    if (idsToRemove.size() == 0)
    {
        return "Delete all indicators.";
    }

    std::string msg = "Delete indicators: ";

    for (uint i = 0; i < idsToRemove.size(); i++)
    {
        msg = msg + std::to_string(idsToRemove.at(i));
        if(i + 1 != idsToRemove.size()) {
            msg = msg + ", ";
        }
        
    }

    return msg;
}