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

#include <string>
/// <summary>
/// Contains static methods to log messages.
/// </summary>
class Logger {
public:
    /// <summary>
    /// Logs messages in "normal" style.
    /// </summary>
    /// <param name="message">The message to be logged</param>
    static void logMessage(std::string message);

    /// <summary>
    /// Logs messages with log level info.
    /// </summary>
    /// <param name="message">The message to be logged</param>
    static void logInfo(std::string message);

    /// <summary>
    /// Logs messages with log level warning.
    /// </summary>
    /// <param name="message">The message to be logged</param>
    static void logWarning(std::string message);

    /// <summary>
    /// Logs messages with log level error.
    /// </summary>
    /// <param name="message">The message to be logged</param>
    static void logError(std::string message);
};
