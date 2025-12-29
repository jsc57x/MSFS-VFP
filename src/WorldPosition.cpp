#include "worldPosition.h"

WorldPosition::WorldPosition(double latitude, double longitude, double altitude, double heading, double bank, double pitch)
{
    this->worldPositionStruct = WorldPositionStruct();
    this->worldPositionStruct.latitude = latitude;
    this->worldPositionStruct.longitude = longitude;
    this->worldPositionStruct.altitude = altitude;
    this->worldPositionStruct.heading = heading;
    this->worldPositionStruct.bank = bank;
    this->worldPositionStruct.pitch = pitch;
}

double WorldPosition::getLatitude() const
{
    return worldPositionStruct.latitude;
}

double WorldPosition::getLongitude() const
{
    return worldPositionStruct.longitude;
}

double WorldPosition::getAltitude() const
{
    return worldPositionStruct.altitude;
}

double WorldPosition::getHeading() const
{
    return worldPositionStruct.heading;
}

double WorldPosition::getBank() const
{
    return worldPositionStruct.bank;
}

double WorldPosition::getPitch() const
{
    return worldPositionStruct.pitch;
}