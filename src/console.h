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
#include <string>

/// Contains helper methods for printing on the console

/// <summary>
/// Prints the help text.
/// </summary>
/// <param name="defaultReceivingPort">Default port for incoming requests</param>
/// <param name="defaultTargetIP">Default target IP address for outgoing requests</param>
/// <param name="defaultTargetPort">Default port for outgoing requests</param>
void printHelp(ushort defaultReceivingPort, std::string defaultTargetIP, ushort defaultTargetPort);

/// <summary>
/// Prints a "normal" message on the console.
/// </summary>
/// <param name="message">The message to show on console</param>
void printMessage(std::string message);

/// <summary>
/// Prints a message in info color on the console.
/// </summary>
/// <param name="message">The message to show on console</param>
void printInfo(std::string message);

/// <summary>
/// Prints a message in warning color on the console.
/// </summary>
/// <param name="message">The message to show on console</param>
void printWarning(std::string message);

/// <summary>
/// Prints a message in error color on the console.
/// </summary>
/// <param name="message">The message to show on console</param>
void printError(std::string message);