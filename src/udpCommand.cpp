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
#include "udpCommand.h"
#include "flightPathVisualizer.h"
#include "log.h"
#include "numberUtils.h"

#include <string>

#define LATITUDE_MIN -90.0
#define LATITUDE_MAX 90.0

#define LONGITUDE_MIN -180.0
#define LONGITUDE_MAX 180.0

 //////////////
 /// PARSER ///
 //////////////
std::unique_ptr<AbstractCommandConfiguration> CommandConfigurationParser::parse(char* raw, uint length)
{
    std::unique_ptr<AbstractCommandConfiguration> commandConfiguration = nullptr;

    if (length < sizeof(ushort))
    {
        throw std::invalid_argument("missing_command");
    }

    ushort commandID = readUShortNetworkByteOrder(raw);

    // this has to be improved if there are more than two commands
    if (commandID != 1 && commandID != 2)
    {
        throw std::invalid_argument("unknown_command");
    }

    if (commandID == 1)
    {
        if (length != 56)
        {
            throw std::invalid_argument("set_invalid_length");
        }
        commandConfiguration = SetIndicatorCommandConfiguration::parse(raw);
    }
    else if (commandID == 2)
    {
        if (length % 2 != 0)
        {
            throw std::invalid_argument("remove_invalid_length");
        }
        commandConfiguration = RemoveIndicatorsCommandConfiguration::parse(raw, length);
    }

    return commandConfiguration;
}



//////////////
///   SET  ///
//////////////

std::unique_ptr<SetIndicatorCommandConfiguration> SetIndicatorCommandConfiguration::parse(char* array)
{
    ushort id = readUShortNetworkByteOrder(array + 2);
    uint indicatorTypeID = readUintNetworkByteOrder(array + 4);
    double latitude = readDoubleinNetworkByteOrder(array + 8);
    double longitude = readDoubleinNetworkByteOrder(array + 16);
    double altitude = readDoubleinNetworkByteOrder(array + 24);
    double heading = readDoubleinNetworkByteOrder(array + 32);
    double bank = readDoubleinNetworkByteOrder(array + 40);
    double pitch = readDoubleinNetworkByteOrder(array + 48);

    WorldPosition worldPos = WorldPosition(latitude, longitude, altitude, heading, bank, pitch);
    SetIndicatorCommandConfiguration commandConfig = SetIndicatorCommandConfiguration(id, indicatorTypeID, worldPos);


    if (commandConfig.validate())
    {
    return std::make_unique<SetIndicatorCommandConfiguration>(commandConfig);
    }
    else {
        return nullptr;
    }

}

bool SetIndicatorCommandConfiguration::validate()
{
    if (!isDoubleInRange(position.getLatitude(), LATITUDE_MIN, LATITUDE_MAX))
    {
        Logger::logError("Latitude is out of range: " + std::to_string(position.getLatitude()));
        return false;
    }

    if (!isDoubleInRange(position.getLongitude(), LONGITUDE_MIN, LONGITUDE_MAX))
    {
        Logger::logError("Longitude is out of range: " + std::to_string(position.getLongitude()));
        return false;
    }

    return true;
}

ushort SetIndicatorCommandConfiguration::getID()
{
    return this->id;
}

uint SetIndicatorCommandConfiguration::getIndicatorTypeID()
{
    return this->indicatorTypeID;
}

WorldPosition SetIndicatorCommandConfiguration::getPosition()
{
    return this->position;
}

std::string SetIndicatorCommandConfiguration::toString()
{
    return "Set Indicator: Indicator " + std::to_string(id) + 
        " of type " + std::to_string(indicatorTypeID) + 
        " Lat: " + std::to_string(position.getLatitude()) + 
        ", Long: " + std::to_string(position.getLongitude()) +
        ", Height: " + std::to_string(position.getAltitude()) + 
        ", Roll: " + std::to_string(position.getHeading()) + 
        ", Pitch: " + std::to_string(position.getBank()) +
        ", Yaw: " + std::to_string(position.getPitch());
}

//////////////
/// REMOVE ///
//////////////
std::unique_ptr<RemoveIndicatorsCommandConfiguration> RemoveIndicatorsCommandConfiguration::parse(char* array, uint length)
{
    RemoveIndicatorsCommandConfiguration command;

    for (uint curMemOffset = 2; curMemOffset < length; curMemOffset = curMemOffset + sizeof(ushort))
    {
        ushort id = readUShortNetworkByteOrder(array + curMemOffset);
        command.idsToRemove.push_back(id);
    }

    return std::make_unique<RemoveIndicatorsCommandConfiguration>(command);
}

std::vector<ushort> RemoveIndicatorsCommandConfiguration::getIDsToRemove()
{
    return this->idsToRemove;
}

std::string RemoveIndicatorsCommandConfiguration::toString()
{
    if (idsToRemove.size() == 0)
    {
        return "Delete all indicators.";
    }

    std::string msg = "Delete indicators: ";

    for (uint i = 0; i < idsToRemove.size(); i++)
    {
        msg = msg + std::to_string(idsToRemove.at(i));
        if(i + 1 != idsToRemove.size()) {
            msg = msg + ", ";
        }
        
    }

    return msg;
}