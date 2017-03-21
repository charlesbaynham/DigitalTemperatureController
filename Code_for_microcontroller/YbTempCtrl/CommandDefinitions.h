/** @file
 *
 * @brief Defines all the commands that the device listens for
 *
 * */
#pragma once

#include "CommandHandler\CommandHandler.h"

 /** @file

 This file contains definitions of all the commands that can be issued to the device. Command processing is done via the CommandHandler class which has its own documentation.

 All functions to be called by CommandHandler take exactly the same form:
 they return void and take a const ParameterLookup object as an argument. This object dispenses c strings as the params, e.g. `params[1]` is the
 first parameter, etc. (`params[0]` is the command itself). See the definition of #commandFunction for the required spec.

 All commands are parsed and stored using a hash function for memory efficiency, and are case-insensitive.

 The following is a summary of the available commands. If query form of a command with different functionality exists is it listed separately. If it's identical, this is shown as e.g. `*TST(?)`. If no query / non-query form exists, it is not shown in this table; calling it will result in an error.

| Command | Description                                                                                                                                                              | Params                                                                                      | Output                                                                                             | 
|---------|--------------------------------------------------------------------------------------------------------------------------------------------------------------------------|---------------------------------------------------------------------------------------------|----------------------------------------------------------------------------------------------------| 
| *TST    | Test comms                                                                                                                                                               | 0                                                                                           | "Loud and clear!"                                                                                  | 
| *TST?   | Test comms                                                                                                                                                               | 0                                                                                           | "Query received"                                                                                   | 
| *IDN(?) | Identify                                                                                                                                                                 | 0                                                                                           | "ARDUINO PID"                                                                                      | 
| *VER(?) | Output version string                                                                                                                                                    | 0                                                                                           | e.g. "v4.1"                                                                                        | 
| *RST    | Reset the device (N.B. does not clear EEPROM)                                                                                                                            | 0                                                                                           | -                                                                                                  | 
| STOR    | Store a command in EEPROM to be executed on startup                                                                                                                      | "command to be called on startup"                                                           | "Done"                                                                                             | 
|         |                                                                                                                                                                          | Multiple commands can be stored by separating them with a ';'                               | "#StoreCommand error: command too long"                                                            | 
| RETR    | Retrieve any stored commands from EEPROM                                                                                                                                 | 0                                                                                           | Stored command or "None"                                                                           | 
| WIPE    | Erase any stored commands in EEPROM                                                                                                                                      | 0                                                                                           | "Done"                                                                                             | 
| ERRO(?) | Query the error signal on given channel.                                                                                                                                 | [channel = 1t,2t,1v,2v] for signals 1 or 2 from thermistor input or voltage input           | Error signal float                                                                                 | 
|         |                                                                                                                                                                          |                                                                                             | ·         If thermistor reading, voltage read from -2.5 -] +2.5                                    | 
|         | If this channel is being controlled, return the previously measured value. Else, measure it now and then return.                                                         |                                                                                             | ·         If voltage reading, return voltage from -5 -] +5 V                                       | 
|         |                                                                                                                                                                          |                                                                                             | e.g. "-0.00151"                                                                                    | 
| STAT(?) | Return a string describing the status of the controller                                                                                                                  | 0                                                                                           | t.b.c.                                                                                             | 
| CONT    | Set the control signal.                                                                                                                                                  | [output channel = 1, 2, 3, 4, BPA, BPB] [voltage = 0 - max for single, -max  - +max for BP] | e.g. "#SetControl BP 2.500"                                                                        | 
|         | N.B. If this is called during a lock this will set the control signal to the given value, but this may subsequently be changed by the locking algorithm                  |                                                                                             |                                                                                                    | 
| CONT?   | Query the control signal                                                                                                                                                 | [output channel = 1, 2, 3, 4, BPA, BPB]                                                     | e.g. "2.500"                                                                                       | 
| THER?   | Check the thermal state of the given channel. For the bipolar channel, return "BAD" if either OPA is overheated                                                          | [output channel = 1, 2, 3, 4, BPA, BPB]                                                     | e.g. "GOOD" / "BAD"                                                                                | 
| LIMI    | Set software limits on the output voltage                                                                                                                                | [output channel = 1, 2, 3, 4, BPA, BPB]                                                     | e.g. "#SetLimits 1 0.000 3.000"                                                                    | 
|         |                                                                                                                                                                          | [min voltage]                                                                               |                                                                                                    | 
|         |                                                                                                                                                                          | [max voltage]                                                                               |                                                                                                    | 
| LIMI?   | Query limits                                                                                                                                                             | [output channel = 1, 2, 3, 4, BPA, BPB]                                                     | e.g. "0.000 3.000"                                                                                 | 
| THRE    | Set thresholds at which the LEDs will start flashing.                                                                                                                    | [thresholdHigh] [thresholdLow]                                                              | e.g. "#setThresholds 0.1 0.01"                                                                     | 
|         | LEDs will flash fast when the absolute error signal is ] thresholdHigh, slow when it's between thresholdHigh and thresholdLow and light solidly when it's [ thresholdLow |                                                                                             |                                                                                                    | 
| THRE?   | Query current LED thresholds                                                                                                                                             | 0                                                                                           | e.g. "0.1, 0.01"                                                                                   | 
| CLIM    | Set hardware limits on the output current                                                                                                                                | [output channel = 1, 2, 3, 4, BPA, BPB]                                                     | e.g. "#SetCurrentLimit 1 1.000"                                                                    | 
|         |                                                                                                                                                                          | [max current in amps]                                                                       |                                                                                                    | 
| CLIM?   | Get current hardware on output current                                                                                                                                   | [output channel = 1, 2, 3, 4, BPA, BPB]                                                     | e.g. "1.000"                                                                                       | 
| SETP    | Set setpoint for lock                                                                                                                                                    | [output channel = 1, 2, 3, 4, BPA, BPB]                                                     | ·         If no lock running on this channel: "#SetSetpoint error: no lock running on channel xxx" | 
|         |                                                                                                                                                                          |                                                                                             | ·         Else, e.g. "#SetSetpoint 1 0.000"                                                        | 
| SETP?   | Query lock setpoint                                                                                                                                                      | [output channel = 1, 2, 3, 4, BPA, BPB]                                                     | ·         If no lock running on this channel: "#SetSetpoint error: no lock running on channel xxx" | 
|         |                                                                                                                                                                          |                                                                                             | ·         Else, e.g. "0.000"                                                                       | 
| LOCK    | Start PID lock                                                                                                                                                           | [input channel = 1t,2t,1v,2v]                                                               | e.g. "#StartLock 1t BP 0.000 1 0.5 0 10"                                                           | 
|         |                                                                                                                                                                          | [output channel = 1, 2, 3, 4, BPA, BPB]                                                     |                                                                                                    | 
|         |                                                                                                                                                                          | [setpoint]                                                                                  |                                                                                                    | 
|         |                                                                                                                                                                          | [Kp] [Ki] [Kd] [N=10]                                                                       |                                                                                                    | 
| VOLT    | Output a constant voltage                                                                                                                                                | [output channel = 1, 2, 3, 4, BPA, BPB] [voltage = 0 - max for single, -max - +max for BP]  | e.g. "#ConstVoltage 1 2.500"                                                                       | 
|         | N.B. Unlike "CONT", this function will output a constant voltage, disabling any PID lock that was previously running on this output channel                              |                                                                                             |                                                                                                    | 

 */

 // Declare all functions:

commandFunction testComms;
commandFunction testCommsQ;
commandFunction resetDevice;
commandFunction identity;
commandFunction version;
commandFunction availableMemory;

commandFunction storeCommand;
commandFunction retreiveCommand;
commandFunction wipeCommand;

commandFunction recallError;
commandFunction measureError;

commandFunction getControlSignal;
commandFunction setControlSignal;

commandFunction getThermalState;

commandFunction getControlLimits;
commandFunction setControlLimits;

commandFunction getCurrentLimit;
commandFunction setCurrentLimit;

commandFunction getThresholds;
commandFunction setThresholds;

commandFunction getSetpoint;
commandFunction setSetpoint;

commandFunction voltageLock;
commandFunction constVoltage;

/**
 * @brief      Register the commands
 *
 *             Register the commandFunction functions with the CommandHandler
 *             object, defining:
 *     * The command used to call them
 *     * The number of parameters they must be called with
 *
 * @param      h     Pointer to a CommandHandler object
 *
 * @tparam     size  Number of commands this CommandHandler can contain. This
 *                   will be inferred by the number specified in its definition.
 *
 * @return     A CommandHandlerReturn enum detailing any errors that occured
 *             during the execution of this method.
 */
template <size_t size>
CommandHandlerReturn registerCommands(CommandHandler<size> * h) {
	// N.B. commands are not case sensitive

	h->registerCommand(COMMANDHANDLER_HASH("*TST"), 0, *testComms);
	h->registerCommand(COMMANDHANDLER_HASH("*TST?"), 0, *testCommsQ);
	h->registerCommand(COMMANDHANDLER_HASH("*RST"), 0, *resetDevice);
	h->registerCommand(COMMANDHANDLER_HASH("*IDN"), 0, *identity);
	h->registerCommand(COMMANDHANDLER_HASH("*IDN?"), 0, *identity);
	h->registerCommand(COMMANDHANDLER_HASH("*VER"), 0, *version);
	h->registerCommand(COMMANDHANDLER_HASH("*VER?"), 0, *version);
	h->registerCommand(COMMANDHANDLER_HASH("*MEM"), 0, *availableMemory);
	h->registerCommand(COMMANDHANDLER_HASH("*MEM?"), 0, *availableMemory);

	h->registerCommand(COMMANDHANDLER_HASH("STOR"), -1, *storeCommand);
	h->registerCommand(COMMANDHANDLER_HASH("RETR"), 0, *retreiveCommand);
	h->registerCommand(COMMANDHANDLER_HASH("RETR?"), 0, *retreiveCommand);
	h->registerCommand(COMMANDHANDLER_HASH("WIPE"), 0, *wipeCommand);

	h->registerCommand(COMMANDHANDLER_HASH("ERRO"), 1, *measureError);
	h->registerCommand(COMMANDHANDLER_HASH("ERRO?"), 1, *measureError);

	h->registerCommand(COMMANDHANDLER_HASH("CONT"), 2, *setControlSignal);
	h->registerCommand(COMMANDHANDLER_HASH("CONT?"), 1, *getControlSignal);

	h->registerCommand(COMMANDHANDLER_HASH("CURR"), 2, *setCurrentLimit);
	h->registerCommand(COMMANDHANDLER_HASH("CURR?"), 1, *getCurrentLimit);

	h->registerCommand(COMMANDHANDLER_HASH("THER?"), 1, *getThermalState);

	h->registerCommand(COMMANDHANDLER_HASH("THRE"), 2, *setThresholds);
	h->registerCommand(COMMANDHANDLER_HASH("THRE?"), 0, *getThresholds);

	h->registerCommand(COMMANDHANDLER_HASH("LIMI"), 3, *setControlLimits);
	h->registerCommand(COMMANDHANDLER_HASH("LIMI?"), 1, *getControlLimits);

	h->registerCommand(COMMANDHANDLER_HASH("SETP"), 2, *setSetpoint);
	h->registerCommand(COMMANDHANDLER_HASH("SETP?"), 1, *getSetpoint);

	h->registerCommand(COMMANDHANDLER_HASH("LOCK"), 7, *voltageLock);

	CommandHandlerReturn lastReturn = h->registerCommand(COMMANDHANDLER_HASH("VOLT"), 2, *constVoltage);

	return lastReturn;
}
