## Test System
The test system supports the validation of the Visual Flight Path extension. It reads files and sends the content over an UDP connection. Three types of files are supported. They are defined by the first row:
* raw files (first row: <raw>) are send by reading and sending the bytes for each row (this is limited due to line break characters are not allowed as content)
* static flight paths (first row: <static>) contain set, remove and delay commands (see below)
* dynamic flight paths (first row: <dynamic>) conatin set, remove and delay commands (see below)

### Commands
Static and dynamic flight path files support three commands: set, remove and delay. Each command is introduced by its keyword at the beginning of the row. Separated by semicolon they are followed by their parameters.
#### Set
Command: **\<set\>**;indicator id; indidcator type id;latitude;longitude;altitude;heading;yaw;pitch

Sets or replaces an indicator. 

#### Remove
Command: **\<rem\>**;optional list of indicator ids separated by ;

Removes an indicator.

#### Delay
Command: **\<delay\>**;Delay in Milliseconds

Delays the execution of the next row by the given amount of milliseconds.
