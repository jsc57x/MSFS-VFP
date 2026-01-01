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