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

#include<string>
#include<sstream>
#include<vector>

/// Contains necessary helper methods for string operations

/// <summary>
/// Splits the given string on every occurence of the given delimiter.
/// </summary>
/// <param name="s">The string to be split</param>
/// <param name="delimiter">The delimiter</param>
/// <returns>The seperated string values</returns>
inline std::vector<std::string> splitString(const std::string& s, char delimiter)
{
    std::vector<std::string> tokens;
    std::stringstream ss(s);
    std::string part;

    while (std::getline(ss, part, delimiter)) {
        tokens.push_back(part);
    }

    return tokens;
}