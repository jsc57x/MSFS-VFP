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
    explicit WorldPosition(const WorldPositionStruct& s) : worldPositionStruct(s) {}
    virtual ~WorldPosition() = default;

    double getLatitude() const;
    double getLongitude() const;
    double getAltitude() const;
    double getHeading() const;
    double getBank() const;
    double getPitch() const;
protected:
    WorldPositionStruct worldPositionStruct;
};