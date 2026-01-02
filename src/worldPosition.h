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

/// <summary>
/// Plain data structure representing a position and orientation in world coordinates (for usage with SimConnect).
/// </summary>
struct WorldPositionStruct
{
    /// <summary>
    /// Geographic latitude in degrees
    /// </summary>
    double latitude;

    /// <summary>
    /// Geographic longitude in degrees
    /// </summary>
    double longitude;

    /// <summary>
    /// Altitude above MSL (Mean Sea Level) in feet
    /// </summary>
    double altitude;

    /// <summary>
    /// Heading (yaw) angle in degrees
    /// </summary>
    double heading;

    /// <summary>
    /// Bank (roll) angle in degrees
    /// </summary>
    double bank;

    /// <summary>
    /// Pitch angle in degrees
    /// </summary>
    double pitch;
};

///<summary>
/// Represents the position and orientation of an object in world coordinates to be used by SimConnect.
/// </summary>
class WorldPosition
{
public:

    /// <summary>
    /// Constructs a world position by explicit values.
    /// </summary>
    /// <param name="latitude">Geographic latitude in degrees</param>
    /// <param name="longitude">Geographic longitude in degrees</param>
    /// <param name="altitude">Altitude above the reference level</param>
    /// <param name="heading">Heading(yaw) angle in degrees</param>
    /// <param name="bank">Bank(roll) angle in degrees</param>
    /// <param name="pitch">Pitch angle in degrees</param>
    WorldPosition(double latitude,
        double longitude,
        double altitude,
        double heading,
        double bank,
        double pitch);

    /// <summary>
    /// Constructs a world position from an existing struct (e.g. from SimConnect).
    /// </summary>
    /// <param name="s">World position structure containing position and orientation data</param>
    explicit WorldPosition(const WorldPositionStruct& s) : worldPositionStruct(s) {}

    /// <summary>
    /// Virtual destructor
    /// </summary>
    virtual ~WorldPosition() = default;

    /// <summary>
    /// Returns the geographic latitude.
    /// </summary>
    /// <returns>Latitude in degrees</returns>
    double getLatitude() const;

    /// <summary>
    /// Returns the geographic longitude.
    /// </summary>
    /// <returns>Longitude in degrees</returns>
    double getLongitude() const;

    /// <summary>
    /// Returns the altitude above MSL.
    /// </summary>
    /// <returns>Altitude in feet</returns>
    double getAltitude() const;

    /// <summary>
    /// Returns the heading (yaw) of the object.
    /// </summary>
    /// <returns>Heading angle in degrees</returns>
    double getHeading() const;

    /// <summary>
    /// Returns the bank (roll) angle of the object.
    /// </summary>
    /// <returns>Bank angle in degrees</returns>
    double getBank() const;

    /// <summary>
    /// Returns the pitch angle of the object.
    /// </summary>
    /// <returns>Pitch angle in degrees</returns>
    double getPitch() const;

protected:
    /// <summary>
    /// Encapsulated struct for communication with SimConnect-API
    /// </summary>
    WorldPositionStruct worldPositionStruct;
};