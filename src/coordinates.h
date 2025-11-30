#pragma once

#include "datatypes.h"

class WorldPosition {
public:
    f64 getLongitude();
    f64 getLatitude();
    f64 getHeight();
    f64 getRoll();
    f64 getPitch();
    f64 getYaw();
};