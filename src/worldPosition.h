#pragma once

struct WorldPositionStruct
{
    double latitude;
    double longitude;
    double altitude;
    double heading;
    double bank;
    double pitch;
};

class WorldPosition
{
public:
    WorldPosition(double latitude, double longitude, double altitude, double heading, double bank, double pitch);
    WorldPosition(WorldPositionStruct* worldPositionStruct) : worldPositionStruct(worldPositionStruct) {};

    double getLatitude();
    double getLongitude();
    double getAltitude();
    double getHeading();
    double getBank();
    double getPitch();
private:
    WorldPositionStruct* worldPositionStruct;
};