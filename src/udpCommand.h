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

enum Command { SET, REMOVE };

class AbstractCommandConfiguration {
public:
    virtual Command getCommand() = 0;
    virtual std::string toString() = 0;
};

class SetIndicatorCommandConfiguration : public AbstractCommandConfiguration
{
public:
    static std::unique_ptr<SetIndicatorCommandConfiguration> parse(char* array);

    Command getCommand() override {
        return Command::SET;
    }

    ushort getID();
    uint getIndicatorTypeID();
    WorldPosition getPosition();

    std::string toString() override;

private:
    ushort id;
    uint indicatorTypeID;
    WorldPosition position;

    SetIndicatorCommandConfiguration(ushort id, 
                                     uint indicatorTypeID, 
                                     WorldPosition worldPosition) 
    : id(id), indicatorTypeID(indicatorTypeID), position(worldPosition) {};
    
    bool validate();
};

class RemoveIndicatorsCommandConfiguration : public AbstractCommandConfiguration
{
public:
    Command getCommand() override {
        return Command::REMOVE;
    }
    std::string toString() override;
    
    static std::unique_ptr<RemoveIndicatorsCommandConfiguration> parse(char* array, uint length);
    std::vector<ushort> getIDsToRemove();

private:
    RemoveIndicatorsCommandConfiguration() {};
    std::vector<ushort> idsToRemove;
};