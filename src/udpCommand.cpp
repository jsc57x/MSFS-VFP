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

#define HEIGHT_MIN 0.0
#define HEIGHT_MAX 1000000 // TODO What is max height???

#define ROLL_MIN 0.0
#define ROLL_MAX 360.0

#define PITCH_MIN 0.0
#define PITCH_MAX 360.0

#define YAW_MIN 0.0
#define YAW_MAX 360.0

//////////////
///   SET  ///
//////////////
std::unique_ptr<SetIndicatorCommandConfiguration> SetIndicatorCommandConfiguration::parse(char* array, u32 length)
{
    SetIndicatorCommandConfiguration command;

    memcpy(&command.id, array + 4, sizeof(u32));
    memcpy(&command.indicatorTypeID, array + 8, sizeof(u32));
    memcpy(&command.latitude, array + 16, sizeof(f64));
    memcpy(&command.longitude, array + 24, sizeof(f64));
    memcpy(&command.height, array + 32, sizeof(f64));
    memcpy(&command.roll, array + 40, sizeof(f64));
    memcpy(&command.pitch, array + 48, sizeof(f64));
    memcpy(&command.yaw, array + 56, sizeof(f64));
    if (command.validate())
    {
    return std::make_unique<SetIndicatorCommandConfiguration>(command);
    }
    else {
        return nullptr;
    }

}

void SetIndicatorCommandConfiguration::setID(u32 id)
{
    this->id = id;
}

void SetIndicatorCommandConfiguration::setIndicator(u64 indicatorTypeID)
{
    this->indicatorTypeID = indicatorTypeID;
}

void SetIndicatorCommandConfiguration::setLatitude(f64 latitude)
{
    this->latitude = latitude;
}

void SetIndicatorCommandConfiguration::setLongitude(f64 longitude)
{
    this->longitude = longitude;
}

void SetIndicatorCommandConfiguration::setHeight(f64 height)
{
    this->height = height;
}

void SetIndicatorCommandConfiguration::setRoll(f64 roll)
{
    this->roll = roll;
}

void SetIndicatorCommandConfiguration::setPitch(f64 pitch)
{
    this->pitch = pitch;
}

void SetIndicatorCommandConfiguration::setYaw(f64 yaw)
{
    this->yaw = yaw;
}

bool SetIndicatorCommandConfiguration::validate()
{
    bool isValid = true;

    if (!isDoubleInRange(latitude, LATITUDE_MIN, LATITUDE_MAX))
    {
        Logger::logError("Latitude is out of range: " + std::to_string(latitude));
        isValid = false;
    }

    if (isValid && !isDoubleInRange(longitude, LONGITUDE_MIN, LONGITUDE_MAX))
    {
        Logger::logError("Longitude is out of range: " + std::to_string(longitude));
        isValid = false;
    }

    if (isValid && !isDoubleInRange(height, HEIGHT_MIN, HEIGHT_MAX))
    {
        Logger::logError("Height is out of range: " + std::to_string(height));
        isValid = false;
    }

    if (isValid && !isDoubleInRange(roll, ROLL_MIN, ROLL_MAX))
    {
        Logger::logError("Roll is out of range: " + std::to_string(roll));
        isValid = false;
    }

    if (isValid && !isDoubleInRange(pitch, PITCH_MIN, PITCH_MAX))
    {
        Logger::logError("Pitch is out of range: " + std::to_string(pitch));
        isValid = false;
    }

    if (isValid && !isDoubleInRange(yaw, YAW_MIN, YAW_MAX))
    {
        Logger::logError("Yaw is out of range: " + std::to_string(yaw));
        isValid = false;
    }    

    return isValid;
}

u32 SetIndicatorCommandConfiguration::getID()
{
    return this->id;
}

u64 SetIndicatorCommandConfiguration::getIndicatorTypeID()
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

f64 SetIndicatorCommandConfiguration::getHeight()
{
    return this->height;
}

f64 SetIndicatorCommandConfiguration::getRoll()
{
    return this->roll;
}

f64 SetIndicatorCommandConfiguration::getPitch()
{
    return this->pitch;
}

f64 SetIndicatorCommandConfiguration::getYaw()
{
    return this->yaw;
}

std::string SetIndicatorCommandConfiguration::toString()
{
    return "Set Indicator: Indicator <unknown> Lat: " + std::to_string(latitude) + ", Long: " + std::to_string(longitude) +
        ", Height: " + std::to_string(height) + ", Roll: " + std::to_string(roll) + ", Pitch: " + std::to_string(pitch) +
        ", Yaw: " + std::to_string(yaw);
}

//////////////
/// REMOVE ///
//////////////
std::unique_ptr<RemoveIndicatorsCommandConfiguration> RemoveIndicatorsCommandConfiguration::parse(char* array, u32 length)
{
    RemoveIndicatorsCommandConfiguration command;

    // TODO Parse

    return std::make_unique<RemoveIndicatorsCommandConfiguration>(command);
    ;
}

void RemoveIndicatorsCommandConfiguration::addIDToRemove(u32 id)
{
    this->idsToRemove.push_back(id);
}

std::vector<int> RemoveIndicatorsCommandConfiguration::getIDsToRemove()
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