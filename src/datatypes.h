#pragma once

//general
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;

// subject-specific
struct AircraftState
{
    double latitude;
    double longitude;
    double altitude;
    double heading;
    double bank;
    double pitch;
    double speed;
};