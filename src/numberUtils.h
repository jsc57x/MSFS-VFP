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

#include <math.h>
#include "datatypes.h"

/// The precision threshold for double values
#define DOUBLE_PRECISION 0.0000000001

/// <summary>
/// Compares two double values if they are equals according to the used double precision.
/// </summary>
/// <param name="a">First value</param>
/// <param name="b">Second value</param>
/// <returns>true if the difference between the given values is smaller then the double precision</returns>
inline bool areDoubleEqual(double a, double b) {
    return fabs(a - b) < DOUBLE_PRECISION;
}

/// <summary>
/// Checks if the given value is within the given min and max values.
/// </summary>
/// <param name="value">The value to be tested</param>
/// <param name="min">Lower border</param>
/// <param name="max">Upper border</param>
/// <returns>true if the value is within min and max with respect to the double precision</returns>
inline bool isDoubleInRange(double value, double min, double max)
{
    if (min > max) std::swap(min, max);

    return !(value < min - DOUBLE_PRECISION || value > max + DOUBLE_PRECISION);
}

inline void writeDoubleInNetworkByteOrder(double value, char* dst)
{
    char tmp[8];
    std::memcpy(tmp, &value, 8);
    for (int i = 0; i < 8; ++i)
    {
        dst[i] = tmp[7 - i];
    }
}

inline void writeUshortInNetworkByteOrder(ushort value, char* dst)
{
    char tmp[2];
    std::memcpy(tmp, &value, 2);
    for (int i = 0; i < 2; ++i)
    {
        dst[i] = tmp[1 - i];
    }
}

inline ushort readUShortNetworkByteOrder(const char* src)
{
    char tmp[2];

    for (int i = 0; i < 2; ++i)
    {
        tmp[i] = src[1 - i];
    }

    ushort value;
    std::memcpy(&value, tmp, sizeof(value));
    return value;
}

inline uint readUintNetworkByteOrder(const char* src)
{
    char tmp[4];

    for (int i = 0; i < 4; ++i)
    {
        tmp[i] = src[3 - i];
    }

    uint value;
    std::memcpy(&value, tmp, sizeof(value));
    return value;
}

inline double readDoubleinNetworkByteOrder(const char* src)
{
    char tmp[8];

    for (int i = 0; i < 8; ++i)
    {
        tmp[i] = src[7 - i];
    }

    double value;
    std::memcpy(&value, tmp, sizeof(value));
    return value;
}