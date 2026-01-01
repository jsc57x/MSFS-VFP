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
#include "worldPosition.h"

WorldPosition::WorldPosition(double latitude, double longitude, double altitude, double heading, double bank, double pitch)
{
    this->worldPositionStruct = WorldPositionStruct();
    this->worldPositionStruct.latitude = latitude;
    this->worldPositionStruct.longitude = longitude;
    this->worldPositionStruct.altitude = altitude;
    this->worldPositionStruct.heading = heading;
    this->worldPositionStruct.bank = bank;
    this->worldPositionStruct.pitch = pitch;
}

double WorldPosition::getLatitude() const
{
    return worldPositionStruct.latitude;
}

double WorldPosition::getLongitude() const
{
    return worldPositionStruct.longitude;
}

double WorldPosition::getAltitude() const
{
    return worldPositionStruct.altitude;
}

double WorldPosition::getHeading() const
{
    return worldPositionStruct.heading;
}

double WorldPosition::getBank() const
{
    return worldPositionStruct.bank;
}

double WorldPosition::getPitch() const
{
    return worldPositionStruct.pitch;
}