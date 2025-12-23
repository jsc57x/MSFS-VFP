#include "udpCommand.h"
#include "flightPathVisualizer.h"
#include "indicator.h"
#include "log.h"
#include "doubleHelper.h"

#include <string>
#include <memory>

#define LATITUDE_MIN -90.0
#define LATITUDE_MAX 90.0

#define LONGITUDE_MIN -180.0
#define LONGITUDE_MAX 180.0

#define ALTITUDE_MIN 0.0
#define ALTITUDE_MAX 1000000 // TODO What is max height???

#define HEADING_MIN -360.0
#define HEADING_MAX 360.0

#define BANK_MIN -360.0
#define BANK_MAX 360.0

#define PITCH_MIN -360.0
#define PITCH_MAX 360.0

//////////////
///   SET  ///
//////////////
std::unique_ptr<SetIndicatorCommandConfiguration> SetIndicatorCommandConfiguration::parse(char* array)
{
    SetIndicatorCommandConfiguration command;

    memcpy(&command.id, array + 2, sizeof(ushort));
    memcpy(&command.indicatorTypeID, array + 4, sizeof(uint));
    memcpy(&command.latitude, array + 8, sizeof(double));
    memcpy(&command.longitude, array + 16, sizeof(double));
    memcpy(&command.altitude, array + 24, sizeof(double));
    memcpy(&command.heading, array + 32, sizeof(double));
    memcpy(&command.bank, array + 40, sizeof(double));
    memcpy(&command.pitch, array + 48, sizeof(double));
    if (command.validate())
    {
    return std::make_unique<SetIndicatorCommandConfiguration>(command);
    }
    else {
        return nullptr;
    }

}

bool SetIndicatorCommandConfiguration::validate()
{
    if (!isDoubleInRange(latitude, LATITUDE_MIN, LATITUDE_MAX))
    {
        Logger::logError("Latitude is out of range: " + std::to_string(latitude));
        return false;
    }

    if (!isDoubleInRange(longitude, LONGITUDE_MIN, LONGITUDE_MAX))
    {
        Logger::logError("Longitude is out of range: " + std::to_string(longitude));
        return false;
    }

/*    if (!isDoubleInRange(altitude, ALTITUDE_MIN, ALTITUDE_MAX))
    {
        Logger::logError("Altitude is out of range: " + std::to_string(altitude));
    }

    if (!isDoubleInRange(heading, HEADING_MIN, HEADING_MAX))
    {
        Logger::logError("Heading is out of range: " + std::to_string(heading));
    }

    if (!isDoubleInRange(bank, BANK_MIN, BANK_MAX))
    {
        Logger::logError("Bank is out of range: " + std::to_string(bank));
    }

    if (!isDoubleInRange(pitch, PITCH_MIN, PITCH_MAX))
    {
        Logger::logError("Pitch is out of range: " + std::to_string(pitch));
    }   */ 

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

double SetIndicatorCommandConfiguration::getLatitude()
{
    return this->latitude;
}

double SetIndicatorCommandConfiguration::getLongitude()
{
    return this->longitude;
}

double SetIndicatorCommandConfiguration::getAltitude()
{
    return this->altitude;
}

double SetIndicatorCommandConfiguration::getHeading()
{
    return this->heading;
}

double SetIndicatorCommandConfiguration::getBank()
{
    return this->bank;
}

double SetIndicatorCommandConfiguration::getPitch()
{
    return this->pitch;
}

std::string SetIndicatorCommandConfiguration::toString()
{
    return "Set Indicator: Indicator " + std::to_string(id) + " of type " + std::to_string(indicatorTypeID) + " Lat: " + std::to_string(latitude) + ", Long: " + std::to_string(longitude) +
        ", Height: " + std::to_string(altitude) + ", Roll: " + std::to_string(heading) + ", Pitch: " + std::to_string(bank) +
        ", Yaw: " + std::to_string(pitch);
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
        command.addIDToRemove(id);
    }


    return std::make_unique<RemoveIndicatorsCommandConfiguration>(command);
}

void RemoveIndicatorsCommandConfiguration::addIDToRemove(ushort id)
{
    this->idsToRemove.push_back(id);
}

std::vector<ushort> RemoveIndicatorsCommandConfiguration::getIDsToRemove()
{
    return this->idsToRemove;
}

bool RemoveIndicatorsCommandConfiguration::validate()
{
    bool isValid = true;

    return isValid;
}

std::string RemoveIndicatorsCommandConfiguration::toString()
{
    return "";
}