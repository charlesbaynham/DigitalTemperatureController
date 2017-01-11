#include <CommandHandler.h>

// Create a CommandHandler object
CommandHandler<4> h;

///////////////////////////////////////////////////////
// Declare functions to be called by serial commands //
///////////////////////////////////////////////////////

// Store a string in EEPROM
// unlimited params: the string to store
commandFunction storeCommand;

// Echo a string from EEPROM
// 0 params
commandFunction readCommand;

// Clear command in EEPROM
// 0 params
commandFunction clearCommand;

// Echo unlimited number of parameters
// unlimited paramas
commandFunction echoMany;

///////////////////////////////////////////////////////
//             End function declaration              //
///////////////////////////////////////////////////////

void setup() {

	Serial.begin(250000);

	// Register serial commands
	h.registerCommand("put", -1, &storeCommand);
	h.registerCommand("get", 0, &readCommand);
	h.registerCommand("clear", 0, &clearCommand);
	CommandHandlerReturn r = h.registerCommand("echo", -1, &echoMany);

	if (r != CommandHandlerReturn::NO_ERROR) {
		Serial.println(F("Error in command registration"));
	}

	// Execute any stored commands
	h.executeStartupCommands();
}

void loop() {

	// Check for commands
	if (h.commandWaiting()) {

		// Execute first waiting command
		CommandHandlerReturn result = h.executeCommand();

		if (result != CommandHandlerReturn::NO_ERROR) {
			Serial.print("Error code ");
			Serial.println((int)result);
		}
	}

	// Check for serial input
	while (Serial.available()) {
		// Queue input for processing
		h.addCommandChar(Serial.read());
	}
}

///////////////////////////////////////////////////////
// Define functions to be called by serial commands //
///////////////////////////////////////////////////////

// Store a string in EEPROM
// unlimited params: the string to store
void storeCommand(const ParameterLookup& params) {

	char command[COMMAND_SIZE_MAX + 1];
	command[0] = '\0';
	
	// Copy first part of command
	if (params.size() > 0) {
		strncat(command, params[1], COMMAND_SIZE_MAX);
	}

	// Copy the rest of the params, with spaces between them
	for (int i = 2; i < params.size(); i++) {
		strncat(command, " ", COMMAND_SIZE_MAX - strlen(command));
		strncat(command, params[i], COMMAND_SIZE_MAX - strlen(command));
	}

	Serial.print(F("Storing string: \""));
	Serial.print(command);
	Serial.println('"');

	h.storeStartupCommand(command);
}

// Echo a string from EEPROM
// 0 params
void readCommand(const ParameterLookup& params) {

	String str = h.getStartupCommand();

	Serial.print(F("Stored string was: \""));
	Serial.print(str);
	Serial.println('"');
}

// Clear command in EEPROM
// 0 params
void clearCommand(const ParameterLookup& params) {

	h.wipeStartupCommand();

	Serial.println(F("Command cleared."));
}

// Echo unlimited number of parameters
// unlimited paramas
void echoMany(const ParameterLookup& params) {

	Serial.print(F("Number of params to echo is: "));
	Serial.println(params.size());

	// Loop until list is done
	for (int i = 0; i < params.size(); i++) {
		Serial.print('"');
		Serial.print(params[i]);
		Serial.println('"');
	}
}