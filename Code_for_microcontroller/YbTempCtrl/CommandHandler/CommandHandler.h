/** @file
 * Contains all the code for the Arduino CommandHandler library
 * 
 * @todo Write the CommandHandler documentation
 */

 #pragma once

#include <Arduino.h>

#include "Microprocessor_Debugging\debugging_disable.h"

#define COMMAND_SIZE_MAX 128 // num chars

// To disable EEPROM features, set this flag:
// #define EEPROM_DISABLED

#ifndef EEPROM_DISABLED
// Storage locations in EEPROM for commands
#include <EEPROM.h>
#define EEPROM_STORED_COMMAND_FLAG_LOCATION 0
#define EEPROM_STORED_COMMAND_LOCATION EEPROM_STORED_COMMAND_FLAG_LOCATION + sizeof(bool)
#endif

// Error messages
enum class CommandHandlerReturn {
	NO_ERROR = 0,
	COMMAND_NOT_FOUND,
	WRONG_NUM_OF_PARAMS,
	ERROR_PARSING_COMMAND,
	EMPTY_COMMAND_STRING,
	NO_COMMAND_WAITING,
	MALLOC_ERROR,
	OUT_OF_MEM,
	BUFFER_FULL,
	COMMAND_TOO_LONG,
	UNKNOWN_ERROR
};

//////////////////////  PARAMETER LOOKUP  //////////////////////

// This class handles the lookup of parameters from an internal string
// It stores an internal pointer to a string which will be invalidated
// and should not be used by other code after passing to this object.
// 
// Index it (e.g. "lookup[0]") to get a parameter out, starting with 0
// being the command itself. 
//
// This object is implemented entirely on the stack. Because of this, it must 
// use some odd internal conventions. Namely, it terminates `_theCommand` 
// with ETX (0x03) chars instead of NULL chars. This is because `_theCommand` 
// contains several NULL chars delimiting each parameter.
//
// After the Constuctor is called, `_theCommand` would be as shown
// for the command "HELO 1 2 3.3" :
//
// "HELO[0x00]1[0x00]2[0x00]3.3[0x00][0x03]"
//
// Thus pointers can be passed to the beginning of each required parameter
// and they form valid c strings for other functions to use.
//
// The flip-side is that _theCommand should not be handled using normal c string
// manipulation commands unless you're being very careful

class ParameterLookup {

public:

	// Constuctor.
	// Replace spaces in commandStr with NULL and terminate the whole lot with
	// a [0x03] char.
	// The buffer pointed to by commandStr must be at least COMMAND_SIZE_MAX+1 in length
	ParameterLookup(char * commandStr) :
		_theCommand(commandStr)
	{
		CONSOLE_LOG(F("ParameterLookup::Constuctor with command: "));
		CONSOLE_LOG_LN(commandStr);

		// Loop through _theCommand counting params and subbing out
		// spaces or tabs for NULLs
		char * loop = _theCommand;
		_size = 1;

		while (*loop) {

			if (' ' == *loop || '\t' == *loop) {

				CONSOLE_LOG(F("ParameterLookup::Replacing char '"));
				CONSOLE_LOG(*loop);
				CONSOLE_LOG(F("' at pos "));
				CONSOLE_LOG(loop - _theCommand);
				CONSOLE_LOG_LN(F(" with \\0"));

				// Replace spaces with NULL chars
				*loop = '\0';

				// If the preceeding char wasn't also a space,
				// increment the param count
				if (loop > _theCommand && // Don't look too far back!
					'\0' != *(loop - sizeof(char))) {
					_size++;
				}
			}

			loop++;
		}

		// We looped to the last char which is a NULL.
		// Leave it as a NULL and make the next char a [0x03] too
		loop++;
		*loop = 0x03;

		CONSOLE_LOG(F("ParameterLookup::[0x03] stored at pos "));
		CONSOLE_LOG_LN(loop - _theCommand);

	}

	// Get parameter indexed. Parameter 0 is the command itself
	// Requesting a non-existent parameter will return a NULL ptr
	const char * operator [] (int idx) const
	{

		// The string is setup as described in "CommandHandler.h", so now all we need
		// to do is get and return a pointer to the start of the requested param
		// `_theCommand` is terminated by a [0x03] char, so don't go past this

		CONSOLE_LOG(F("ParameterLookup::Looking for param "));
		CONSOLE_LOG_LN(idx);

		char * paramPtr = _theCommand;
		int count = idx;

		while (0x03 != *paramPtr) {

			if (count == 0 && *paramPtr) {
				// We found a non null char after passing the required 
				// number of nulls, so return a pointer to it

				CONSOLE_LOG(F("ParameterLookup::Returning ptr to pos "));
				CONSOLE_LOG(paramPtr - _theCommand);
				CONSOLE_LOG(F(", containing command: "));
				CONSOLE_LOG_LN(paramPtr);

				return paramPtr;
			}

			if ('\0' == *paramPtr && // We found a null...
									 // ... and the previous char wasn't a null
				paramPtr > _theCommand && '\0' != *(paramPtr - 1)) {
				// Decrement the count, as we've passed a delimiter
				count--;

				CONSOLE_LOG(F("ParameterLookup::Break found at pos "));
				CONSOLE_LOG_LN(paramPtr - _theCommand);
			}

			// Next char
			paramPtr++;
		}

		CONSOLE_LOG_LN(F("ParameterLookup::Not found"));

		// We got to the end without finding the requested param number
		// return a Null ptr
		return 0;
	}

	// Number of stored params, including the command itself
	unsigned int size() const { return _size; }

	// Dump the contents of _theCommand if in debug mode
	void dump() const {

		CONSOLE_LOG_LN(F("ParameterLookup::dump"));

#ifdef DEBUGGING_ENABLED

		if (!__serial_is_ready) return;

		Serial.print(F("*** loc ("));
		Serial.print((int)_theCommand, DEC);
		Serial.println(F(") ***"));

		for (int i = 0; i < COMMAND_SIZE_MAX; i++) {
			Serial.print(i);
			Serial.print('\t');
		}

		Serial.println("");

		for (int i = 0; i < COMMAND_SIZE_MAX; i++) {
			if (isprint(_theCommand[i]))
			{
				Serial.print(_theCommand[i]);
			}
			else {
				Serial.print('[');
				Serial.print((int)_theCommand[i]);
				Serial.print(']');
			}
			Serial.print('\t');
		}

		Serial.println(F("***"));

#endif
	}

private:

	// Pointer to the whole command
	char * _theCommand;
	unsigned int _size;

};

// Template for the functions to be called in response to a command
typedef void commandFunction(const ParameterLookup& params);

//////////////////////  COMMAND HANDLER  //////////////////////

// This class handle the receiving and executing of commands. It should be
// fed chars from the serial input by `addCommandChar()`
// When a command is ready it will flag `commandWaiting()`
// When convenient, `executeCommand()` should then be called. This will
// invoke the nested CommandLookup object in order to get the right command
// and execute it
//
// This object can hold <size> commands where <size> is defined at compile time, e.g.
// 
// "CommandHandler<10> handler;
//
// This class also contains methods for storing commands in the EEPROM in 
// order to queue a command on device startup
// These can be disabled by adding the line:
// #define EEPROM_DISABLED
// The user must call `executeStartupCommands()` in their code once they are ready 
// for EEPROM commands to be executed

template <size_t array_size>
class CommandHandler
{

	// Forward declare the CommandLookup class
private:
	class CommandLookup;

public:

	// Constuctor
	// Initialise private members and queue any stored command in the EEPROM
	CommandHandler() :
		_lookupList(), // Not needed, but just to be explicit
		_command_too_long(false),
		_bufferFull(false),
		_bufferLength(0)
	{
		CONSOLE_LOG_LN(F("CommandHandler::CommandHandler()"));

		// Start the input buffer empty
		_inputBuffer[0] = '\0';
	}

	// Execute the waiting command
	CommandHandlerReturn executeCommand()
	{

		CommandHandlerReturn error = CommandHandlerReturn::NO_ERROR;

		CONSOLE_LOG_LN(F("Execute command"));

		// Return error code if no command waiting
		if (!_bufferFull) {
			CONSOLE_LOG_LN(F("No command error"));
			error = CommandHandlerReturn::NO_COMMAND_WAITING;
		}

		CONSOLE_LOG(F("Command is: "));
		CONSOLE_LOG_LN(_inputBuffer);

		// Return error code if string is empty
		if (_bufferLength == 0 && error == CommandHandlerReturn::NO_ERROR)
		{
			CONSOLE_LOG_LN(F("Empty command error"));
			error = CommandHandlerReturn::EMPTY_COMMAND_STRING;
		}

		// Constuct a parameter lookup object from the command string
		// This invalidates the string for future use
		CONSOLE_LOG_LN(F("Creating ParameterLookup object..."));
		ParameterLookup lookupObj = ParameterLookup(_inputBuffer);

		CONSOLE_LOG_LN(F("Running callStoredCommand..."));
		CommandHandlerReturn found = _lookupList.callStoredCommand(lookupObj);

		// Mark buffer as ready again
		_bufferFull = false;
		_inputBuffer[0] = '\0';
		_bufferLength = 0;

		return found;
	}

	// Register a command
	CommandHandlerReturn registerCommand(const char* command, int num_of_parameters,
		commandFunction* pointer_to_function) {
		return _lookupList.registerCommand(command, num_of_parameters,
			pointer_to_function);
	}

	// Add a char from the serial connection to be processed and added to the queue
	// Returns BUFFER_FULL if buffer is full and char wasn't added
	CommandHandlerReturn addCommandChar(const char c)
	{

		// Check if the buffer is already full
		if (_bufferFull) {
			return CommandHandlerReturn::BUFFER_FULL;
		}

		// If c is a newline, mark the buffer as full
		if (c == '\n') {

			CONSOLE_LOG(F("Newline received. Command: "));
			CONSOLE_LOG_LN(_inputBuffer);

			// Should we ignore this command?
			if (_command_too_long) {
				// Reset the `_command_too_long` flag
				_command_too_long = false;

				CONSOLE_LOG_LN(F("Ignoring command since too long"));

				return CommandHandlerReturn::COMMAND_TOO_LONG;
			}

			// If not, we are already null terminated so mark the string as ready
			_bufferFull = true;
		}
		// if c is a carridge return, ignore it
		else if (c == '\r') {
			CONSOLE_LOG_LN(F("Ignoring a \r"));
			return CommandHandlerReturn::NO_ERROR;
		}
		// else c is a normal char, so add it to the buffer
		else {
			if (_command_too_long || _bufferLength >= COMMAND_SIZE_MAX)
			{
				// Command was too long! Set the `_command_too_long` flag to chuck away all subsequent chars until next newline
				CONSOLE_LOG_LN(F("ERROR: command too long!"));

				_command_too_long = true;

				return CommandHandlerReturn::COMMAND_TOO_LONG;
			}
			else
			{
				// The normal case. Add the new char to the buffer

				_inputBuffer[_bufferLength] = c;

				_bufferLength++;

				_inputBuffer[_bufferLength] = '\0';

				CONSOLE_LOG(F("Char received: '"));
				CONSOLE_LOG(c);
				CONSOLE_LOG(F("', Buffer length: "));
				CONSOLE_LOG_LN(_bufferLength);

				return CommandHandlerReturn::NO_ERROR;
			}
		}

		return CommandHandlerReturn::UNKNOWN_ERROR; // We should never get here
	}

	// Check to see if the handler is ready for more incoming chars
	inline bool bufferFull() { return _bufferFull; }

	// Is a command waiting?
	inline bool commandWaiting() { return bufferFull(); }

#ifndef EEPROM_DISABLED
	// Store a command to be executed on startup in the EEPROM
	// This command should not include newlines: it will be copied verbatim into the
	// buffer and then executed as a normal command would be
	// Multiple commands can be seperated by ';' chars
	// Max length is COMMAND_LENGTH_MAX - 2 (1 char to append a newline, 1 for the null term)
	// Returns false on fail
	CommandHandlerReturn storeStartupCommand(const String& command)
	{

		// Check that the string is small enough to fit into the buffer, including null char
		if (command.length() > COMMAND_SIZE_MAX - 2) {
			CONSOLE_LOG_LN(F("CommandHandler::Command too long for EEPROM"));
			return CommandHandlerReturn::COMMAND_TOO_LONG;
		}

		// Store it
		return storeStartupCommand(command.c_str());
	}

	// Store a command to be executed on startup in the EEPROM
	// This command should not include newlines: it will be copied verbatim into the
	// buffer and then executed as a normal command would be
	// Multiple commands can be seperated by ';' chars
	// Max length is COMMAND_LENGTH_MAX - 2 (1 char to append a newline, 1 for the null term)
	// Returns false on fail
	CommandHandlerReturn storeStartupCommand(const char* command)
	{

		// Store a flag indicating that a command exists
		const bool trueFlag = true;
		EEPROM.update(EEPROM_STORED_COMMAND_FLAG_LOCATION, trueFlag);

		// Loop through the command and store it
		int commandIdx = 0;
		int eeprom_ptr = 0;

		while (command[commandIdx] != '\0' && eeprom_ptr < COMMAND_SIZE_MAX - 2) {

			char toBeStored;

			// Check if it's a semicolon delimiter
			if (command[commandIdx] != ';') {
				// Nope. Just copy it
				toBeStored = command[commandIdx];
			}
			else {
				// It was a semicolon, so store a newline
				toBeStored = '\n';
			}

			// Store the char in EEPROM
			CONSOLE_LOG(F("CommandHandler::Update EEPROM ("));
			CONSOLE_LOG(EEPROM_STORED_COMMAND_LOCATION + eeprom_ptr);
			CONSOLE_LOG(F("): [0x"));
			CONSOLE_LOG(toBeStored, HEX);
			CONSOLE_LOG_LN(']');

			EEPROM.update(EEPROM_STORED_COMMAND_LOCATION + eeprom_ptr, toBeStored);

			// Increment string pointer and eeprom pointer
			commandIdx++;
			eeprom_ptr += sizeof(char);
		}

		// Terminate with a newline and a null
		CONSOLE_LOG(F("CommandHandler::Terminate EEPROM ("));
		CONSOLE_LOG(EEPROM_STORED_COMMAND_LOCATION + eeprom_ptr);
		CONSOLE_LOG('&');
		CONSOLE_LOG(EEPROM_STORED_COMMAND_LOCATION + eeprom_ptr + 1);
		CONSOLE_LOG_LN(')');

		EEPROM.update(EEPROM_STORED_COMMAND_LOCATION + eeprom_ptr, '\n');
		eeprom_ptr++;
		EEPROM.update(EEPROM_STORED_COMMAND_LOCATION + eeprom_ptr, '\0');

		return CommandHandlerReturn::NO_ERROR;
	}

	// Remove any startup commands from the EEPROM
	bool wipeStartupCommand()
	{
		// There is no need to wipe the command from the EEPROM: we simply set the flag to false

		// Store a flag indicating that no command exists
		const bool falseFlag = false;

		EEPROM.update(EEPROM_STORED_COMMAND_FLAG_LOCATION, falseFlag);

		return true;
	}

	// Return any stored startup command by copying into buf.
	// buf must point to a buffer of at least COMMAND_SIZE_MAX chars
	void getStartupCommand(char * buf)
	{
		CONSOLE_LOG_LN(F("CommandHandler::getStartupCommand(char*)"))

			// Index of location in buffer
			int bufIdx = 0; // Start at start of buffer

							// There should be a bool stored in EEPROM_STORED_COMMAND_FLAG_LOCATION if this program has run before
							// It will tell us if there's a command to be read or not
							// Read it as a byte though, since the memory location will be 0xFF if it has never been written to
							// We only want to use it if it's exactly a bool
		char fromEEPROM;
		EEPROM.get(EEPROM_STORED_COMMAND_FLAG_LOCATION, fromEEPROM);

		CONSOLE_LOG(F("CommandHandler::EEPROM flag contains : [0x"));
		CONSOLE_LOG(fromEEPROM, HEX);
		CONSOLE_LOG_LN(']');

		if (fromEEPROM == (char)true) {

			CONSOLE_LOG_LN(F("CommandHandler::EEPROM flag true"));

			// Index of location in EEPROM
			int EEPROM_idx = EEPROM_STORED_COMMAND_LOCATION;

			// At most, go to the end of the buffer - 1, to leave space for a null terminator
			while (bufIdx < COMMAND_SIZE_MAX - 1)
			{
				char c;
				EEPROM.get(EEPROM_idx, c);

				CONSOLE_LOG(F("CommandHandler::Read from EEPROM ("));
				CONSOLE_LOG(EEPROM_idx);
				CONSOLE_LOG(F("): [0x"));
				CONSOLE_LOG(c, HEX);
				CONSOLE_LOG_LN(']');

				if (c == '\0') {
					// Found the end

					CONSOLE_LOG(F("CommandHandler::Stored command ends at "));
					CONSOLE_LOG_LN(EEPROM_idx);

					break;
				}

				// Store the char
				buf[bufIdx] = c;

				EEPROM_idx++;
				bufIdx++;
			}
		}
		else if (fromEEPROM == (char)false) {
			CONSOLE_LOG_LN(F("CommandHandler::EEPROM flag false"));
		}
		else {
			CONSOLE_LOG_LN(F("CommandHandler::EEPROM flag undefined"));
		}

		// Null terminate
		buf[bufIdx] = '\0';
	}

	// Execute any startup commands stored in the EEPROM
	CommandHandlerReturn executeStartupCommands()
	{

		CONSOLE_LOG_LN(F("CommandHandler::executeStartupCommands"));

		// Is a command stored (see getStoredCommand for details)?
		char fromEEPROM;
		EEPROM.get(EEPROM_STORED_COMMAND_FLAG_LOCATION, fromEEPROM);

		// See if the flag is anything other than "true"
		if (fromEEPROM != (char)true) {
			CONSOLE_LOG_LN(F("CommandHandler::executeStartupCommands: No command stored"));
			return CommandHandlerReturn::NO_COMMAND_WAITING;
		}

		// Command is waiting, so queue it one char at a time
		// If we reach a newline, commandWaiting() will flag "true": execute the command
		// If we reach a NULL, end
		
		// Index of location in EEPROM
		int EEPROM_idx = EEPROM_STORED_COMMAND_LOCATION;
		CommandHandlerReturn result = CommandHandlerReturn::NO_ERROR;
		while (true) {

			char c;
			EEPROM.get(EEPROM_idx, c);

			CONSOLE_LOG(F("CommandHandler::Read from EEPROM ("));
			CONSOLE_LOG(EEPROM_idx);
			CONSOLE_LOG(F("): [0x"));
			CONSOLE_LOG(c, HEX);
			CONSOLE_LOG_LN(']');

			if (c == '\0') {
				// Found the end

				CONSOLE_LOG(F("CommandHandler::Stored command ends at "));
				CONSOLE_LOG_LN(EEPROM_idx);

				break;
			}

			// If any preceding commands have failed, stop executing here
			if (CommandHandlerReturn::NO_ERROR == result) {

				CONSOLE_LOG(F("CommandHandler::executeStartupCommands: Queueing "));
				CONSOLE_LOG_LN(c);

				// Queue the char
				addCommandChar(c);

				// If CommandHandler is ready to execute the queued command, do so
				if (commandWaiting()) {
					result = executeCommand();
				}
			}

			EEPROM_idx++;
		}

		return result;

	}
#endif

private:
	// An object for handling the matching of commands -> functions
	CommandLookup _lookupList;

	// Flag to warn that the command handler cannot handle more incoming chars
	// until the current command is processed
	bool _bufferFull;

	// A buffer for receiving new commands
	char _inputBuffer[COMMAND_SIZE_MAX + 1];
	unsigned int _bufferLength;

	// A flag to report that the command currently being received has overrun
	bool _command_too_long;

	//////////////////////  COMMAND LOOKUP  //////////////////////

	// This class is responsible for matching strings -> commands
	// It maintains a vector of hashes, associated commands and number of
	// parameters required for those commands
	// `callStoredCommand` performs the lookup and calls the appropriate
	// command, passing through a parameter lookup object
	//
	// Its maximum size is determined at compile time by the `size` template argument

private:
	// Structure of the data to be stored for each command
	struct dataStruct {
		unsigned long hash; // Hash of the keyword (case insensitive)
		int n; // Number of params this function takes
		commandFunction* f; // Pointer to this function
	};

	class CommandLookup
	{
	public:

		CommandLookup() :
			_commandsIdx(0)
		{}

		// Add a new command to the list
		CommandHandlerReturn registerCommand(const char* command, int num_of_parameters,
			commandFunction* pointer_to_function)
		{
			if (_commandsIdx >= array_size) {
				CONSOLE_LOG_LN(F("CommandLookup::Out of pre-allocated space"));
				return CommandHandlerReturn::OUT_OF_MEM;
			}

			// Set up a struct containing the number of params and a pointer to the function
			dataStruct d;

			// Get hash of command
			long keyHash = djbHash(command);

			// Save params
			d.hash = keyHash;
			d.n = num_of_parameters;
			d.f = pointer_to_function;

			// Store it in the vector
			_commands[_commandsIdx++] = d;

			return CommandHandlerReturn::NO_ERROR;
		}

		// Search the list of commands for the given command and execute it with the given parameter array
		CommandHandlerReturn callStoredCommand(const ParameterLookup& params)
		{

			CONSOLE_LOG(F("callStoredCommand with n="));
			CONSOLE_LOG_LN(params.size());

			// Get hash of command requested
			const unsigned long reqHash = djbHash(params[0]);

			int foundIdx = -1;

			// Search through vector for this hash
			for (int i = 0; i < _commandsIdx; i++) {
				if (reqHash == _commands[i].hash) {
					foundIdx = i;
					break;
				}
			}

			if (foundIdx == -1) {
				CONSOLE_LOG_LN(F("Command not found"));
				return CommandHandlerReturn::COMMAND_NOT_FOUND;
			}

			const dataStruct& d = _commands[foundIdx];
			const commandFunction* f = d.f;

			CONSOLE_LOG(F("Recalled data: d.n = "));
			CONSOLE_LOG_LN(d.n);

			// Return error if wrong number of parameters
			if (d.n != params.size() - 1 && d.n != -1) {
				CONSOLE_LOG(F("ERROR: Expecting "));
				CONSOLE_LOG(d.n);
				CONSOLE_LOG(F(" parameters but got "));
				CONSOLE_LOG_LN(params.size() - 1);

				return CommandHandlerReturn::WRONG_NUM_OF_PARAMS;
			}

			CONSOLE_LOG_LN(F("Calling function..."));
			f(params);

			return CommandHandlerReturn::NO_ERROR;
		}

	protected:

		dataStruct _commands[array_size];
		unsigned int _commandsIdx;

		// Hash string (case insensitive)
		// Uses	the djb2 algorithm by Dan Bernstein
		// See http://www.cse.yorku.ca/~oz/hash.html
		static unsigned long djbHash(const char *str)
		{
			unsigned long hash = 5381;
			int c;

			while (c = tolower(*str++)) {
				hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
			}

			return hash;
		}
	};
};

