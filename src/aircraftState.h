#pragma once

struct AircraftStateStruct
{
    double latitude;
    double longitude;
    double altitude;
    double heading;
    double bank;
    double pitch;
    double speed;
};

class AircraftState
{
public:
    AircraftState(AircraftStateStruct* stateStruct);

    double getLatitude();
    double getLongitude();
    double getAltitude();
    double getHeading();
    double getBank();
    double getPitch();
    double getSpeed();
private:
    AircraftStateStruct* state;
};