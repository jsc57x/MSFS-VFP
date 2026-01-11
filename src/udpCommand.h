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

#include "datatypes.h"
#include "worldPosition.h"
#include <string>
#include <vector>
#include <memory>

/// <summary>
/// Command Types which can be executed.
/// </summary>
enum Command { SET, REMOVE };

/// <summary>
/// Abstract command configuration which at least provides the specified command.
/// </summary>
class AbstractCommandConfiguration {
public:
    /// <summary>
    /// Returns the command which is configured by this specific command configuration.
    /// </summary>
    /// <returns>Command ID</returns>
    virtual Command getCommand() = 0;

    /// <summary>
    /// Returns a human-readable string representation of the command configuration.
    /// </summary>
    /// <returns>Human-readable information about this command configuration</returns>
    virtual std::string toString() = 0;
};

/// <summary>
/// Configuration for the command to place a SimObject.
/// </summary>
class SetIndicatorCommandConfiguration : public AbstractCommandConfiguration
{
public:
    /// <summary>
    /// Parses the fiven data and creates a command configuration.
    /// </summary>
    /// <param name="array">Raw data</param>
    /// <returns>Command configuration to place a SimObject</returns>
    static std::unique_ptr<SetIndicatorCommandConfiguration> parse(char* array);

    Command getCommand() override {
        return Command::SET;
    }


    /// <summary>
    /// Returns the external identicator id.
    /// </summary>
    /// <returns>External indicator id</returns>
    ushort getID();

    /// <summary>
    /// Returns the numerical representation for a specific indicator model.
    /// </summary>
    /// <returns>id of indicator model</returns>
    uint getIndicatorTypeID();
    
    /// <summary>
    /// Returns position and orientation of the object.
    /// </summary>
    /// <returns>Position and orientation</returns>
    WorldPosition getPosition();

    std::string toString() override;

private:
    /// <summary>
    /// The external indicator id.
    /// </summary>
    ushort id;
    
    /// <summary>
    /// The numierical representation of the indicator model.
    /// </summary>
    uint indicatorTypeID;

    /// <summary>
    /// Position and orientation of the object
    /// </summary>
    WorldPosition position;

    /// <summary>
    /// Private constructor. Use the parse method.
    /// </summary>
    /// <param name="id">External indicator id</param>
    /// <param name="indicatorTypeID">Id of the indicator model</param>
    /// <param name="worldPosition">Position and orientation</param>
    SetIndicatorCommandConfiguration(ushort id, 
                                     uint indicatorTypeID, 
                                     WorldPosition worldPosition) 
    : id(id), indicatorTypeID(indicatorTypeID), position(worldPosition) {};
    
    /// <summary>
    /// Validates the settings of the command. This is mainly the latitude and longitude of the position.
    /// </summary>
    /// <returns>true if latitude and longitude are in bounds, false otherwise</returns>
    bool validate();
};

/// <summary>
/// Configuration for the command to remove a SimObject.
/// </summary>
class RemoveIndicatorsCommandConfiguration : public AbstractCommandConfiguration
{
public:
    Command getCommand() override {
        return Command::REMOVE;
    }
    std::string toString() override;
    
    /// <summary>
    /// Parses the fiven data and creates a command configuration.
    /// </summary>
    /// <param name="array">Raw data</param>
    /// <param name="length">Length of raw data</param>
    /// <returns>Command configuration to remove a SimObject</returns>
    static std::unique_ptr<RemoveIndicatorsCommandConfiguration> parse(char* array, uint length);

    /// <summary>
    /// Returns the external ids of the indicators which should be deleted.
    /// </summary>
    /// <returns>List of external indicator ids</returns>
    std::vector<ushort> getIDsToRemove();

private:
    /// <summary>
    /// Private constructor. Use the parse method.
    /// </summary>
    RemoveIndicatorsCommandConfiguration() {};

    /// <summary>
    /// List of external indicator ids which should be removed.
    /// </summary>
    std::vector<ushort> idsToRemove;
};

class CommandConfigurationParser
{
public:
    static std::unique_ptr<AbstractCommandConfiguration> parse(char* raw, uint length);
};