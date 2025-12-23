#include "aircraftState.h"

AircraftState::AircraftState(AircraftStateStruct* stateStruct)
{
    this->state = stateStruct;
}

double AircraftState::getLatitude()
{
    return state->latitude;
}

double AircraftState::getLongitude()
{
    return state->longitude;
}

double AircraftState::getAltitude()
{
    return state->altitude;
}

double AircraftState::getHeading()
{
    return state->heading;
}

double AircraftState::getBank()
{
    return state->bank;
}

double AircraftState::getPitch()
{
    return state->pitch;
}

double AircraftState::getSpeed()
{
    return state->speed;
}