#pragma once
#include "worldPosition.h"

struct AircraftStateStruct : WorldPositionStruct
{
    double speed;
};

class AircraftState : public WorldPosition
{
public:
    AircraftState(AircraftStateStruct* stateStruct) : WorldPosition(static_cast<WorldPositionStruct*>(stateStruct)), state(stateStruct) {};

    double getSpeed();
private:
    AircraftStateStruct* state;
};