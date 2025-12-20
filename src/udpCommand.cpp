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

    memcpy(&command.id, array + 2, sizeof(u16));
    memcpy(&command.indicatorTypeID, array + 4, sizeof(u32));
    memcpy(&command.latitude, array + 8, sizeof(f64));
    memcpy(&command.longitude, array + 16, sizeof(f64));
    memcpy(&command.altitude, array + 24, sizeof(f64));
    memcpy(&command.heading, array + 32, sizeof(f64));
    memcpy(&command.bank, array + 40, sizeof(f64));
    memcpy(&command.pitch, array + 48, sizeof(f64));
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

u16 SetIndicatorCommandConfiguration::getID()
{
    return this->id;
}

u32 SetIndicatorCommandConfiguration::getIndicatorTypeID()
{
    return this->indicatorTypeID;
}

f64 SetIndicatorCommandConfiguration::getLatitude()
{
    return this->latitude;
}

f64 SetIndicatorCommandConfiguration::getLongitude()
{
    return this->longitude;
}

f64 SetIndicatorCommandConfiguration::getAltitude()
{
    return this->altitude;
}

f64 SetIndicatorCommandConfiguration::getHeading()
{
    return this->heading;
}

f64 SetIndicatorCommandConfiguration::getBank()
{
    return this->bank;
}

f64 SetIndicatorCommandConfiguration::getPitch()
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
std::unique_ptr<RemoveIndicatorsCommandConfiguration> RemoveIndicatorsCommandConfiguration::parse(char* array, u32 length)
{
    RemoveIndicatorsCommandConfiguration command;

    for (u32 curMemOffset = 2; curMemOffset < length; curMemOffset = curMemOffset + sizeof(u16))
    {
        u16 id;
        memcpy(&id, array + curMemOffset, sizeof(u16));
        command.addIDToRemove(id);
    }


    return std::make_unique<RemoveIndicatorsCommandConfiguration>(command);
}

void RemoveIndicatorsCommandConfiguration::addIDToRemove(u16 id)
{
    this->idsToRemove.push_back(id);
}

std::vector<u16> RemoveIndicatorsCommandConfiguration::getIDsToRemove()
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