This repository is part of my Bachelor Thesis. 
It containts the Microsoft Flight Simulator Visual Flight Path Extension which allows the placement and removal of SimObjects within MSFS 2024 by sending commands via an UDP connection.
The repo contains three products
* The Visual Flight Path Extension itself
* A MSFS Add-on with 15 SimObjects 
* A test system which read files, converts them to commands and sends them over an UDP socket

### Prerequisites
The extension and the test system are Visual Studio 2022 projects. They were development and can be compiled with version 17.14.
In order to compile the extension, the MSFS 2024 SDK in version 1.2.4. Later versions are incompatible due to a memory leak when trying to connect to SimConnect.

#### MSFS Add-on
The MSFS Add-on can be manipulated and compiled with the MSFS Developer Mode.
