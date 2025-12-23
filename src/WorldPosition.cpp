#include "worldPosition.h"

WorldPosition::WorldPosition(double latitude, double longitude, double altitude, double heading, double bank, double pitch)
{
    this->worldPositionStruct = new WorldPositionStruct();
    this->worldPositionStruct->latitude = latitude;
    this->worldPositionStruct->longitude = longitude;
    this->worldPositionStruct->altitude = altitude;
    this->worldPositionStruct->heading = heading;
    this->worldPositionStruct->bank = bank;
    this->worldPositionStruct->pitch = pitch;
}

double WorldPosition::getLatitude()
{
    return worldPositionStruct->latitude;
}

double WorldPosition::getLongitude()
{
    return worldPositionStruct->longitude;
}

double WorldPosition::getAltitude()
{
    return worldPositionStruct->altitude;
}

double WorldPosition::getHeading()
{
    return worldPositionStruct->heading;
}

double WorldPosition::getBank()
{
    return worldPositionStruct->bank;
}

double WorldPosition::getPitch()
{
    return worldPositionStruct->pitch;
}