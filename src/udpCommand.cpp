#include "udpCommand.h"
#include "flightPathVisualizer.h"
#include "indicator.h"

SetIndicatorCommandConfiguration::SetIndicatorCommandConfiguration() {
    this->id = 0;
    this->latitude = 0;
    this->longitude = 0;
    this->height = 0;
    this->roll = 0;
    this->pitch = 0;
    this->yaw = 0;
}

void SetIndicatorCommandConfiguration::setID(u32 id)
{
    this->id = id;
}

void SetIndicatorCommandConfiguration::setIndicator(Indicator indicator)
{
    this->indicator = indicator;
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

void SetIndicatorCommandConfiguration::setFromByteArray(char* array, u32 length)
{
    this->indicator = Indicator::parse(array + 8, length - 8);

    memcpy(&id, array + 4, sizeof(u32));
    memcpy(&latitude, array + 16, sizeof(f64));
    memcpy(&longitude, array + 24, sizeof(f64));
    memcpy(&height, array + 32, sizeof(f64));
    memcpy(&roll, array + 40, sizeof(f64));
    memcpy(&pitch, array + 48, sizeof(f64));
    memcpy(&yaw, array + 56, sizeof(f64));
}

u32 SetIndicatorCommandConfiguration::getID()
{
    return this->id;
}

Indicator SetIndicatorCommandConfiguration::getIndicator()
{
    return this->indicator;
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

RemoveIndicatorsCommandConfiguration::RemoveIndicatorsCommandConfiguration()
{
    // empty
}

void RemoveIndicatorsCommandConfiguration::addIDToRemove(u32 id)
{
    this->idsToRemove.push_back(id);
}

std::vector<int> RemoveIndicatorsCommandConfiguration::getIDsToRemove()
{
    return this->idsToRemove;
}