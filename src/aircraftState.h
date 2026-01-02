/*
 * Visual Flight Path for Microsoft Flight Simulator 2024
 * Copyright (c) 2026 Jens Scharmann / Fernuniversität in Hagen
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#pragma once
#include "worldPosition.h"
#include <memory>


/// <summary>
/// Data structure representing the position, orientation, and speed of an simulated aircraft.
/// </summary>
struct AircraftStateStruct : WorldPositionStruct
{
    /// <summary>
    /// Aircraft speed in knots (kts)
    /// </summary>
    double speed;
};

/// <summary>
/// Represents the position, orientation and speed of an simulated aircraft.
/// </summary>
class AircraftState : public WorldPosition
{
public:
    /// <summary>
    /// Constructs an aircraft state from an aircraft state structure.
    /// </summary>
    /// <param name="s">Aircraft state structure containing position, orientation, and speed</param>
    explicit AircraftState(const AircraftStateStruct& s)
        : WorldPosition(static_cast<const WorldPositionStruct&>(s))
        , state(s) {}


    /// <summary>
    /// Returns the aircraft speed in knots (kts)
    /// </summary>
    /// <returns>Aircraft speed</returns>
    double getSpeed() const;

private:
    /// <summary>
    /// Encapsulated struct for communication with SimConnect-API
    /// </summary>
    AircraftStateStruct state;
};