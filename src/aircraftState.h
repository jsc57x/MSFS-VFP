#pragma once
#include "worldPosition.h"
#include <memory>

struct AircraftStateStruct : WorldPositionStruct
{
    double speed;
};

class AircraftState : public WorldPosition
{
public:
    explicit AircraftState(const AircraftStateStruct& s)
        : WorldPosition(static_cast<const WorldPositionStruct&>(s))
        , state(s) {
    }

    double getSpeed() const;
private:
    AircraftStateStruct state;
};