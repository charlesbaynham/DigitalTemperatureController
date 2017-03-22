/*-------------------------------------------------------------------
-																	-
-		PID Temperature Controller									-
-			Charles Baynham - NPL									-
-																	-
-		Version:													-
-*/
#define REV_VERSION "v4.3"									
#define IDENTIFIER "ARDUINO PID"
/*																	-
-		Controller to manage up to two processes, correcting them	-
-		via a PID algorithm.										-
-																	-
-		By default, this controller communicates by USB-serial		-
-		connection. The Serial connection expects:					-
-			Baud rate: 56700										-
-			Data: 8 bits, Parity: NO, Stop bits: 1					-
-		Comms must be terminated by the '\n' char.					-
-																	-
--------------------------------------------------------------------*/

#pragma message ("Implement *TST")

#include "Pins.h"

// Num of s.f. to print floats with
#define FLT_PRINT_PRECISION 9

// Library to measure free memory
#include "MemoryFree.h"

// Library to parse and handle commands, and store startup commands in EEPROM
#include "CommandHandler\CommandHandler.h"

// Library to toggle pins at a defined rate
#include "arduino-pin-toggler\pinToggler.h"

// Objects specific to this code:
#include "Controller.h"
#include "PIDAlgorithm.h"
#include "V4_ADC_ChannelPair.h"
#include "V4_OPA_OutputChannel.h"
#include "V4_OPA_OutputChannelBipolar.h"

using namespace YbCtrl;

// Not used here, but these are needed by class files so must be included here
#include <SPI.h>

// Macros for the watchdog timer
#include <avr/wdt.h>

// Convenience macros for debugging
#include "Microprocessor_debugging\debugging_init.h"
#include "Microprocessor_debugging\debugging_disable.h"

// Command handler object, can hold 30 commands
CommandHandler<30> handler;
// Declare functions for all the SCPI commands
#include "CommandDefinitions.h"

// Default thresholds for the LED flash rate indicators (in volts)
double led_thresholds[2] = { 0.1, 0.01 };

// Max voltage that the OPA will attempt to output
// This is calculated by measuring the supply voltage (using a divided voltage on pin DIVIDED_SUPPLY_VOLTAGE)
// and subtracting 0.3V since the OPAs are not rail-to-rail
double MAX_VOLTAGE = 0;						

// Vector for pointers to the input channels
#ifdef FOUR_CHANNEL_BOARD
	ErrorChannel * errorChannels[5];
#else
	ErrorChannel * errorChannels[7];
#endif

// Labels for each of the error channels
#ifdef FOUR_CHANNEL_BOARD
enum class ErrorChanIdx {
	NOT_FOUND = -1,
	CHAN_1T, // == 0
	CHAN_2T, // == 1
	CHAN_TMP, // etc
	CHAN_POW
};
#else
enum class ErrorChanIdx {
	NOT_FOUND = -1,
	CHAN_1T, // == 0
	CHAN_2T, // == 1
	CHAN_1V, // etc.
	CHAN_2V,
	CHAN_TMP,
	CHAN_POW
};
#endif

// Vector for pointers to the output channels
// These are (carefully) malloced in setup()
#ifdef FOUR_CHANNEL_BOARD
CtrlChannel * ctrlChannels[6];
#else
CtrlChannel * ctrlChannels[3];
#endif

// Labels for each of the output channels
#ifdef FOUR_CHANNEL_BOARD
enum class CtrlChanIdx {
	NOT_FOUND = -1,
	CHAN_1,
	CHAN_2,
	CHAN_3,
	CHAN_4,
	CHAN_BP_12,
	CHAN_BP_34
};
#else
enum class CtrlChanIdx {
	NOT_FOUND = -1,
	CHAN_1,
	CHAN_2,
	CHAN_BP_12
};
#endif

// DFU mode magic variable
// This variable is not initialised to zero. Therefore we can write to it before a watchdog 
// reset and it won't get overridden, so we can send information from instance to instance. 
// I'll use this to send a "magic key" which, if it is set, means that I'm requesting a bootloader reset
const uint16_t magic_bootloader_key = 0x586c;
uint16_t boot_key __attribute__ ((section (".noinit")));
unsigned int num_resets __attribute__ ((section (".noinit")));

// Reserve space for things
//
// 1) Controllers (not pointers, this reserves space on the stack)
//
// This vector will be 2 long, containing Controllers or nullptrs.
//
// This is the most we can need at one time: we can control channel 1 and
// channel 2 independently (2 Controllers), or both simultaneously in a bipolar
// configuration (1 Controller).
//
// The program will assign Controllers onto this buffer in the order that they are created
// (i.e. when the user calls ("LOCK")
//
// 2) PIDAlgorithms
// 
// Algorithms are stored with the same index as their associated Controller
#ifdef FOUR_CHANNEL_BOARD
Controller controllers[4];
PIDAlgorithm pidAlgorithms[4];
#else
Controller controllers[2];
PIDAlgorithm pidAlgorithms[2];
#endif

// The Arduino IDE will automatically define all the functions declared in this file, but it will fail on those that require a namespace
// Since all the YbTempCtrl related objects use the YbCtrl namespace, all functions that take a parameter relating to one of these objects
// must be declared manually. 
// They are all declared "static" using the C meaning of the modifier: i.e. these functions only exist within this file
static void closeChannelsController(CtrlChannel * chan);
static void reportCtrlError(CtrlChannelReturn err);
static void reportErrorError(ErrorChannelReturn err);
static inline bool voltageToBeDoubled(const ErrorChanIdx chan);
static ErrorChanIdx stringToADCChannel(const char str[]);
static CtrlChanIdx stringToOPAChannel(const char str[]);
static inline bool isBipolar(const CtrlChanIdx chan);

const size_t numLEDs = 2;

// This method is the only place where the use of "malloc" and "new" is allowed
// No memory is "free"ed so there is no possiblilty of fragmentation. 
// Checks for success are strictly adhered to to prevent undetected out of memory errors
void setup()
{
	// Check for DFU mode:
	// Is the magic key present?
	if (boot_key == magic_bootloader_key) {

		// boot_key was marked with the magic key:
		// This means that DFU mode is requested
		
		// We will do 10x resets max
		if (num_resets == 10)
		{
			// Remove the magic key if we've already reset 10x
			// This stops us from rebooting again
			boot_key = 0;

		} else {
			
			// More resets requested. Increment the number of resets (this is preserved from reset to reset)
			// and then jump back to the bootloader
			num_resets++;
			
			// Do a watchdog reset to get back to the bootloader
			doWatchdogReset();
		}
	}

	// Start the watchdog timer with a 2s timeout
#ifndef DEBUGGING_ENABLED
	startWatchdog();
#endif

	// Start the serial connection
	Serial.begin(57600);

	// This line prevents the use of Strings which are anathema
#pragma GCC poison String

	// Tell the debugging lines to output from now on
	__serial_is_ready = true;

	CONSOLE_LOG_LN(F("setup::Setup Started"));

	// Init LED output pins
	uint8_t ledPins[numLEDs] = { LED_1 , LED_2 };
	pinToggler<numLEDs>::init(ledPins);
	// They start off, so leave them there

	CONSOLE_LOG_LN(F("setup::LED setup complete"));

	// Load the possible serial commands
	auto CH_Res = registerCommands(&handler);
	if (CommandHandlerReturn::NO_ERROR != CH_Res) {
		Serial.print(F("Error: Command registration failed with code "));
		Serial.println((int)CH_Res);
	}

	CONSOLE_LOG_LN(F("setup::Command setup complete"));

	// Reset the watchdog timeout
	wdt_reset();

	/****** Setup interfaces ******/

	/* ADC
	Channels :
	* Thermistor 1
	* Thermistor 2
	* Voltage 1 (if present)
	* Voltage 2 (if present)

	* Temperature sensor
	* Analog Supply Voltage
	*/

	// Reset the ADC
	// This is needed since the ADC is isolated from the rest of the electronics, 
	// so does not get reset by the RESET line
	V4_ADC_ChannelPair::resetADC();

	// Define ADC channels
	errorChannels[(int)ErrorChanIdx::CHAN_1T] = new V4_ADC_ChannelPair(ADC_THERM1, ADC_REF, ADC_THERM1_HIGHRES);
	errorChannels[(int)ErrorChanIdx::CHAN_2T] = new V4_ADC_ChannelPair(ADC_THERM2, ADC_REF, ADC_THERM2_HIGHRES);
	errorChannels[(int)ErrorChanIdx::CHAN_TMP] = new V4_ADC_ChannelPair(ADC_TMP, ADC_TMP, false);
	errorChannels[(int)ErrorChanIdx::CHAN_POW] = new V4_ADC_ChannelPair(ADC_POW, ADC_POW, false);

#ifndef FOUR_CHANNEL_BOARD
	errorChannels[(int)ErrorChanIdx::CHAN_1V] = new V4_ADC_ChannelPair(ADC_VOLT1_P, ADC_VOLT1_N, ADC_VOLT1_HIGHRES);;
	errorChannels[(int)ErrorChanIdx::CHAN_2V] = new V4_ADC_ChannelPair(ADC_VOLT2_P, ADC_VOLT2_N, ADC_VOLT2_HIGHRES);;
#endif

	// Check for out of memory
	checkOutOfMem();
	
	CONSOLE_LOG_LN(F("setup::ADC setup complete"));

	/* OPAs
	Channel 0 : Unipolar channel 1
	Channel 1 : Unipolar channel 2
	Channel 2 : Bipolar (both channels 1 & 2)

	If four channel board:
	Channel 3 : Unipolar channel 3
	Channel 4 : Unipolar channel 4
	Channel 5 : Bipolar (both channels 3 & 4)
	*/

	// Set the maximum OPA output
	MAX_VOLTAGE = calculateOPAMaxVoltage();

	CONSOLE_LOG(F("MAX_VOLTAGE = "));
	CONSOLE_LOG_LN(MAX_VOLTAGE);

	// Define OPA channels
	ctrlChannels[(int)CtrlChanIdx::CHAN_1] = new V4_OPA_OutputChannel(VPLUS_CHAN_1, VLIM_CHAN_1, OPA_ES1, OPA_1_IS_548, DAC_CS);
	ctrlChannels[(int)CtrlChanIdx::CHAN_2] = new V4_OPA_OutputChannel(VPLUS_CHAN_2, VLIM_CHAN_2, OPA_ES2, OPA_2_IS_548, DAC_CS);
	ctrlChannels[(int)CtrlChanIdx::CHAN_BP_12] = new V4_OPA_OutputChannelBipolar(	VPLUS_CHAN_1, VLIM_CHAN_1, OPA_ES1, OPA_1_IS_548, DAC_CS,
																					VPLUS_CHAN_2, VLIM_CHAN_2, OPA_ES2, OPA_2_IS_548, DAC_CS);
	
	// Same for four channel board's extra channels
#ifdef FOUR_CHANNEL_BOARD
	ctrlChannels[(int)CtrlChanIdx::CHAN_3] = new V4_OPA_OutputChannel(VPLUS_CHAN_3, VLIM_CHAN_3, OPA_ES3, OPA_3_IS_548, DAC_CS_Alt);
	ctrlChannels[(int)CtrlChanIdx::CHAN_4] = new V4_OPA_OutputChannel(VPLUS_CHAN_4, VLIM_CHAN_4, OPA_ES4, OPA_4_IS_548, DAC_CS_Alt);
	ctrlChannels[(int)CtrlChanIdx::CHAN_BP_34] = new V4_OPA_OutputChannelBipolar(VPLUS_CHAN_3, VLIM_CHAN_3, OPA_ES3, OPA_3_IS_548, DAC_CS_Alt, VPLUS_CHAN_4, VLIM_CHAN_4, OPA_ES4, OPA_4_IS_548, DAC_CS_Alt);
#endif

	// Check for out of memory
	checkOutOfMem();

	// Tell channels which other channels conflict with them
	// I.e. The bipolar channels conflict with both the single-sided channels involved in the bipolar output
	ctrlChannels[(int)CtrlChanIdx::CHAN_1]->addConflictingChannel(ctrlChannels[(int)CtrlChanIdx::CHAN_BP_12]);
	ctrlChannels[(int)CtrlChanIdx::CHAN_2]->addConflictingChannel(ctrlChannels[(int)CtrlChanIdx::CHAN_BP_12]);
	ctrlChannels[(int)CtrlChanIdx::CHAN_BP_12]->addConflictingChannel(ctrlChannels[(int)CtrlChanIdx::CHAN_1]);
	ctrlChannels[(int)CtrlChanIdx::CHAN_BP_12]->addConflictingChannel(ctrlChannels[(int)CtrlChanIdx::CHAN_2]);
	
#ifdef FOUR_CHANNEL_BOARD
	ctrlChannels[(int)CtrlChanIdx::CHAN_3]->addConflictingChannel(ctrlChannels[(int)CtrlChanIdx::CHAN_BP_34]);
	ctrlChannels[(int)CtrlChanIdx::CHAN_4]->addConflictingChannel(ctrlChannels[(int)CtrlChanIdx::CHAN_BP_34]);
	ctrlChannels[(int)CtrlChanIdx::CHAN_BP_34]->addConflictingChannel(ctrlChannels[(int)CtrlChanIdx::CHAN_3]);
	ctrlChannels[(int)CtrlChanIdx::CHAN_BP_34]->addConflictingChannel(ctrlChannels[(int)CtrlChanIdx::CHAN_4]);
#endif

	CONSOLE_LOG_LN(F("setup::DAC setup complete"));

	// Flush the serial input
	while (Serial.available()) Serial.read();

	CONSOLE_LOG(F("setup::Setup complete, freemem = "));
	CONSOLE_LOG_LN(freeMemory());
	CONSOLE_LOG_LN(F("Executing startup commands"));

	// Execute any EEPROM stored commands
	CommandHandlerReturn out = handler.executeStartupCommands();

	if (CommandHandlerReturn::NO_ERROR == out) {
		CONSOLE_LOG_LN(F("setup::Startup commands executed"));
	}
	else if (CommandHandlerReturn::NO_COMMAND_WAITING == out) {
		CONSOLE_LOG_LN(F("setup::No startup commands found"));
	}
	else {
		Serial.print(F("Error in startup commands, CommandHandlerReturn code "));
		Serial.println((int)out);
	}

#ifdef DEBUGGING_ENABLED
	delay(1000);
#endif
}

unsigned int count = 0;
const int led_check_cycles = 10; // Check the LEDs' status every led_check_cycles cycles

void loop()
{
	// Reset the watchdog timeout
	wdt_reset();

	// Check for commands and execute them if they're waiting
	if (handler.commandWaiting()) {

		CONSOLE_LOG_LN(F("loop::Command received"));

		CommandHandlerReturn out = handler.executeCommand();

		if (CommandHandlerReturn::NO_ERROR != out) {
			Serial.print(F("Error in command: CommandHandlerReturn code "));
			Serial.println((int)out);
		}

	}

	// Do any PID loops
	// Any of the Controllers could be active, so check all
	for (size_t i = 0; i < (sizeof(controllers) / sizeof(*controllers)); i++) {
		if (controllers[i]) {
			CONSOLE_LOG(F("*** Controller "));
			CONSOLE_LOG(i);
			CONSOLE_LOG_LN(F(" is active. Running"));

			controllers[i].doLoop();

			CONSOLE_LOG_LN(F("*** Loop done"));
		}
		else {
			CONSOLE_LOG(F("*** Controller "));
			CONSOLE_LOG(i);
			CONSOLE_LOG_LN(F(" is inactive"));
		}
	}

	// Every led_check_cycles cycles, check the LEDs
	if (count++ >= led_check_cycles) {

		// Reset the led cycle counter
		count = 0;
		
		// The `controllers` array can contain several Controllers, potentially one for each output channel.
		// Since there are only two LEDs, we can't in general display status LEDs for all of them, although 
		// 99% of the use cases for this device will involve a maximum of two Controllers. 
		// 
		// The Controller 
		// creation code puts the Controllers into the array in a sensible order, prioritising the first 2 slots. 
		// Here I decided to have the LEDs represent the Controllers stored only in the first two slots of the 
		// array. The points at which we create the Controllers
		
		// There are only 2 LEDs, so only output statuses for the first two active Controllers (void voltageLock(...))
		// will make sure that they are put in the right place. 
		// 
		// For reference, this place will be:
		// 
		// Output channel		LED
		// 				1		1
		// 				2		1
		// 				3		2
		// 				4		2
		//				BPA		1
		//				BPB		2
		//				
		// In the event of a clash, e.g. channels 1 and 2 are locked at the same time, the second one to be started will put 
		// assigned to the other LED. If this is not possble (e.g. channels 1 and 3 are locked, then 2 is started) then no
		// LED will be assigned

		// Loop over the two LED channels
		for (size_t i = 0; i < numLEDs; i++) { 

			// If we're not out of bounds of the array...
			// ...and there is a Controller assigned to this slot
			const size_t arrayLen = sizeof(controllers) / sizeof(*controllers);
			if (i < arrayLen && controllers[i]) {

				// Get distance from the set points and active / deactivate the LED accordingly

				CONSOLE_LOG(F("*** Checking LED status for Controller "));
				CONSOLE_LOG(i);
				CONSOLE_LOG(F(", for LED "));
				CONSOLE_LOG(i);
				CONSOLE_LOG(F(", on pin "));
				CONSOLE_LOG_LN(pinToggler<numLEDs>::getPin(i));

				double errorSig = controllers[i].getErrorInterface()->recallError();
				double setPoint = controllers[i].getAlgorithm()->getSetpoint();

				// For the sake of this conversion, let's just assume this is a thermistor measurement
				// if it's not, it just makes these warning levels twice as far out
				double error = levelToErrorVoltage(abs(setPoint - errorSig), false);

				CONSOLE_LOG(F("Diff from setp. = "));
				CONSOLE_LOG(error);
				CONSOLE_LOG_LN('.');

				/*
				 * Let's have three levels, by default:
				 *
				 * LED ON    -  error < 10mV
				 * LED SLOW  -  error < 100mV
				 * LED FAST  -  error > 100mV
				 *
				 * These can be redefined by the command THRE (as in THREshold)
				 */

				if (error > led_thresholds[0]) {
					pinToggler<numLEDs>::setFlashRate(i, FAST);
				}
				else if (error > led_thresholds[1]) {
					pinToggler<numLEDs>::setFlashRate(i, SLOW);
				}
				else {
					pinToggler<numLEDs>::setFlashRate(i, ON);
				}
			}
			else {

				CONSOLE_LOG(F("*** Controller "));
				CONSOLE_LOG(i);
				CONSOLE_LOG_LN(F(" is inactive"));

				pinToggler<numLEDs>::setFlashRate(i, OFF);
			}
		} // end loop over Controllers
	}
}

void serialEvent() {

	while (Serial.available()) {
		char c = Serial.read();

		CONSOLE_LOG(F("serialEvent::Adding: ["));
		CONSOLE_LOG(c);
		CONSOLE_LOG_LN(F("]"));

		handler.addCommandChar(c);
	}

}


// ********************* COMMAND DEFINITIONS *********************
// See CommandDefinitions.h for more documentation about these functions

// Test the interface
// Expects 0 params
void testComms(const ParameterLookup&) {
	Serial.println(F("Loud and clear!"));
}
void testCommsQ(const ParameterLookup&) {
	Serial.println(F("Query received"));
}

// Reset the Arduino
// Expects 0 params
void resetDevice(const ParameterLookup&) {

	if (isSerialControlDisabled()) return;

	Serial.println(F("Resetting..."));
	Serial.flush();
	
	doWatchdogReset();
}

/**
 * @brief      Launch DFU mode
 *
 *             Reset the Arduino to its bootloader
 *
 * @param[in]  params  The parameters
 */
void DFU_Mode(const ParameterLookup& params) {

	if (isSerialControlDisabled()) return;

	Serial.println(F("DFU Mode..."));
	Serial.flush();

	// Mark the boot key with the magic variable
	boot_key = magic_bootloader_key;

	// Restart the reset counter
	num_resets = 0;

	// Initiate a watchdog reset
	doWatchdogReset();
}

// To do a proper reset, we set the watchdog to its shortest timeout and then wait for the reset to happen
void doWatchdogReset() {

	// Clear MCU Status Register. 
	MCUSR = 0;

	// Disable and clear all Watchdog settings
	WDTCSR = (1 << WDCE);
	WDTCSR = 0;

	// Setup Watchdog for instant reset, with a 16ms timeout
	const uint8_t shortest_timeout_WDTCSR = (1 << WDCE) | (1 << WDE);

	CONSOLE_LOG(F("resetDevice::setting WDTCSR to 0b"));
	CONSOLE_LOG_LN(shortest_timeout_WDTCSR, BIN);

	// Reset counter
	wdt_reset();

	// Disable all interrupts
	noInterrupts();

	WDTCSR = (1 << WDCE) | (1 << WDE);
	WDTCSR = shortest_timeout_WDTCSR;

	// Wait for impending doom
	while (true) ;

}

// Return the identity string
// Expects 0 params
void identity(const ParameterLookup&) {
	Serial.println(IDENTIFIER);
}

// Return the version string
// Expects 0 params
void version(const ParameterLookup&) {
	Serial.println(REV_VERSION);
}

// Query the error signal on given channel.
// If this channel is under Controller control, recall the previously
// measured value. Else, measure it now and then return. 
// Expects 1 params : a string, the input channel ("1T", "1V", "2T", "2V", "TMP", "POW")
void measureError(const ParameterLookup& params) {

	ErrorChanIdx channelNum = stringToADCChannel(params[1]);

	if (channelNum == ErrorChanIdx::NOT_FOUND) {
		Serial.print(F("#GetError"));
		channelNotRecognised_ADC(params[1]);
		return;
	}

	ErrorChannel * theChannel = errorChannels[(int)channelNum];

	// Search the Controllers for this channel to see if it's under control already
	bool isControlled = false;
	const size_t arrayLen = sizeof(controllers) / sizeof(*controllers);
	for (int i = 0; i < arrayLen; i++) {
		if (controllers[i].getErrorInterface() == theChannel) {
			isControlled = true;
			break;
		}
	}

	double reading;
	ErrorChannelReturn result;

	// If this channel is controlled, recall the error. 
	// Otherwise, measure it now
	if (isControlled) {
		result = theChannel->recallError(reading);
	}
	else {

		// If there's no reading ready (there probably isn't) then start one
		if (!theChannel->readingReady()) {
			ErrorChannelReturn ret1 = theChannel->startReading();

			if (ErrorChannelReturn::NO_ERROR != ret1) {
				Serial.print(F("startReading failed: "));
				reportErrorError(ret1);
				return;
			}
		}

		// Loop until timeout or a reading becomes available
		while (!theChannel->readingReady()) {

			// Kick the watchdog
			wdt_reset();

			if (theChannel->readingTimeout()) {

				theChannel->abortReading();

				Serial.println(F("Error: ADC Timeout"));
				return;
			}
		}

		// Our reading is now ready, so get it
		result = theChannel->getReading(reading);
	}

	if (result != ErrorChannelReturn::NO_ERROR && result != ErrorChannelReturn::OUT_OF_RANGE) {
		Serial.print(F("getReading / recallError failed: "));
		reportErrorError(result);
		return;
	}

	// Convert -1 -> +1 reading into voltage
	double voltage = levelToErrorVoltage(reading, voltageToBeDoubled(channelNum));

	// If the reading hit the rails, prepend a '>' or a '<' depending
	if (result == ErrorChannelReturn::OUT_OF_RANGE) {
		Serial.print(voltage > 0 ? '>' : '<');
	}

	// Report the voltage
	Serial.println(voltage, FLT_PRINT_PRECISION); // 6 s.f.
}

// Return the current control signal value
// Expects 1 params : a string, the output channel number ("1", "2", "BP")
void getControlSignal(const ParameterLookup& params) {

	CtrlChanIdx channelNum = stringToOPAChannel(params[1]);

	if (channelNum == CtrlChanIdx::NOT_FOUND) {
		Serial.print(F("#GetControl"));
		channelNotRecognised(params[1]);
		return;
	}

	double level = ctrlChannels[(int)channelNum]->recallCtrl();

	if (level == -999) {
		Serial.println(F("Error: channel "));
		Serial.print(params[1]);
		Serial.println(F("does not exist in OPA container"));
		return;
	}

	// Convert to volts
	double voltage = ctrlLevelToVoltage(level, isBipolar(channelNum));

	// Report
	Serial.println(voltage, FLT_PRINT_PRECISION);
}

// Sets the current control signal value
// Expects 2 params : a string, the output channel number ("1", "2", "BP")
//					  a double, the new voltage
void setControlSignal(const ParameterLookup& params) {

	if (isSerialControlDisabled()) return;

	CtrlChanIdx channelNum = stringToOPAChannel(params[1]);

	if (channelNum == CtrlChanIdx::NOT_FOUND) {
		Serial.print(F("#SetControl"));
		channelNotRecognised(params[1]);
		return;
	}

	double newVoltage = atof(params[2]);

	double newLevel = voltageToCtrlLevel(newVoltage, isBipolar(channelNum));

	CONSOLE_LOG(F("setControlSignal level = "));
	CONSOLE_LOG_LN(newLevel, FLT_PRINT_PRECISION);

	// Write the new level to the output stage
	ctrlChannels[(int)channelNum]->setCtrl(newLevel);

	// Report
	// e.g. "#SetControl BP 2.500"
	Serial.print(F("#SetControl "));
	Serial.print(params[1]);
	Serial.print(' ');
	Serial.println(newVoltage, FLT_PRINT_PRECISION);
}

// Return the current OPA thermal state: either "GOOD" or "BAD"
// Expects 1 params : a string, the output channel number ("1", "2", "BP")
void getThermalState(const ParameterLookup& params) {

	CtrlChanIdx channelNum = stringToOPAChannel(params[1]);

	if (channelNum == CtrlChanIdx::NOT_FOUND) {
		Serial.print(F("#GetControl"));
		channelNotRecognised(params[1]);
		return;
	}

	bool state;
	CtrlChannelReturn err = ctrlChannels[(int)channelNum]->isOverheated(state);

	if (err != CtrlChannelReturn::NO_ERROR) {
		reportCtrlError(err);
		return;
	}

	// Report
	if (state) {
		Serial.println(F("BAD"));
	}
	else {
		Serial.println(F("GOOD"));
	}
}

// Gets the current control signal limits
// Expects 1 params : a string, the output channel number ("1", "2", "BP")
void getControlLimits(const ParameterLookup& params) {

	CtrlChanIdx channelNum = stringToOPAChannel(params[1]);

	if (channelNum == CtrlChanIdx::NOT_FOUND) {
		Serial.print(F("#GetLimits"));
		channelNotRecognised(params[1]);
		return;
	}

	double min;
	double max;
	CtrlChannelReturn result = ctrlChannels[(int)channelNum]->getLimits(min, max);

	// Convert to volts
	double minV = ctrlLevelToVoltage(min, isBipolar(channelNum));
	double maxV = ctrlLevelToVoltage(max, isBipolar(channelNum));

	// Report
	// e.g. "0.000 3.000"
	Serial.print(minV, FLT_PRINT_PRECISION);
	Serial.print(", ");
	Serial.println(maxV, FLT_PRINT_PRECISION);
}

// Sets the current control signal limits
// Expects 3 params : a string, the output channel number ("1", "2", "BP")
//                    a double, the new min control signal in volts
//                    a double, the new max control signal in volts
// Sets the current control signal limits
void setControlLimits(const ParameterLookup& params) {

	if (isSerialControlDisabled()) return;

	CtrlChanIdx channelNum = stringToOPAChannel(params[1]);

	if (channelNum == CtrlChanIdx::NOT_FOUND) {
		Serial.print(F("#SetLimits"));
		channelNotRecognised(params[1]);
		return;
	}

	// Read new limits in volts
	double newMin = atof(params[2]);
	double newMax = atof(params[3]);

	// Convert to -1 -> +1 format
	double newMinF = voltageToCtrlLevel(newMin, isBipolar(channelNum));
	double newMaxF = voltageToCtrlLevel(newMax, isBipolar(channelNum));

	// Limit output
	CtrlChannelReturn result = ctrlChannels[(int)channelNum]->setLimits(newMinF, newMaxF);

	// Report errors
	if (CtrlChannelReturn::NO_SUCH_CHANNEL == result) {
		Serial.print(F("Error: channel "));
		Serial.print((int)channelNum);
		Serial.println(F(" does not exist in OPA container"));
		return;
	}
	else if (CtrlChannelReturn::NO_ERROR != result) {
		reportCtrlError(result);
		return;
	}

	// Report
	// e.g. "#SetLimits 1 0.000 3.000"
	Serial.print(F("#SetLimits "));
	Serial.print(params[1]);
	Serial.print(" ");
	Serial.print(newMin, FLT_PRINT_PRECISION);
	Serial.print(" ");
	Serial.println(newMax, FLT_PRINT_PRECISION);
}

// Sets the current control current limit
// Expects 2 params : a string, the output channel number ("1", "2", "BP")
//                    a double, the new max current in amps
void setCurrentLimit(const ParameterLookup& params) {

	if (isSerialControlDisabled()) return;

	CtrlChanIdx channelNum = stringToOPAChannel(params[1]);

	if (channelNum == CtrlChanIdx::NOT_FOUND) {
		Serial.print(F("#setCurrentLimit"));
		channelNotRecognised(params[1]);
		return;
	}

	// Read new limits in amps
	double newLim = atof(params[2]);

	// Limit current
	CtrlChannelReturn result = ctrlChannels[(int)channelNum]->setCurrentLimit(newLim);

	// Report errors
	if (CtrlChannelReturn::NO_ERROR != result) {
		reportCtrlError(result);
		return;
	}

	// Report
	// e.g. "#setCurrentLimit 1 1.500
	Serial.print(F("#setCurrentLimit "));
	Serial.print(params[1]);
	Serial.print(" ");
	Serial.println(newLim, FLT_PRINT_PRECISION);
}

// Gets the current control current limit
// Expects 1 param : a string, the output channel number ("1", "2", "BP")
void getCurrentLimit(const ParameterLookup& params) {

	CtrlChanIdx channelNum = stringToOPAChannel(params[1]);

	if (channelNum == CtrlChanIdx::NOT_FOUND) {
		Serial.print(F("#setCurrentLimit"));
		channelNotRecognised(params[1]);
		return;
	}

	// Get current limit
	double theLim;
	CtrlChannelReturn result = ctrlChannels[(int)channelNum]->getCurrentLimit(theLim);

	// Report errors
	if (CtrlChannelReturn::NO_ERROR != result) {
		reportCtrlError(result);
		return;
	}

	// Report
	// e.g. "1.500"
	Serial.println(theLim, FLT_PRINT_PRECISION);
}

// Gets the current PID setpoint
// Expects 1 params : a string, the output channel number ("1", "2", "BP")
void getSetpoint(const ParameterLookup& params) {

	CtrlChanIdx channelNum = stringToOPAChannel(params[1]);

	if (channelNum == CtrlChanIdx::NOT_FOUND) {
		Serial.print(F("#GetSetpoint"));
		channelNotRecognised(params[1]);
		return;
	}

	// Retrive associated controller from the appropriate output channel object
	Controller * targetController = ctrlChannels[(int)channelNum]->getContainingController();

	if (!targetController) { // If there's no Controller associated

		Serial.print(F("#GetSetpoint error: no lock running on channel "));
		Serial.println(params[1]);

		return;
	}

	double currentSP = targetController->getAlgorithm()->getSetpoint();
	double currentSP_V = levelToErrorVoltage(currentSP, false);

	// Report
	Serial.println(currentSP_V, FLT_PRINT_PRECISION);

}

// Sets the PID setpoint
// Expects 2 params : a string, the output channel number ("1", "2", "BP")
//                    a double, the new setpoint in volts
void setSetpoint(const ParameterLookup& params) {

	if (isSerialControlDisabled()) return;

	CtrlChanIdx channelNum = stringToOPAChannel(params[1]);

	if (channelNum == CtrlChanIdx::NOT_FOUND) {
		Serial.print(F("#SetSetpoint"));
		channelNotRecognised(params[1]);
		return;
	}

	double newSP = atof(params[2]);
	double newSP_F = errorVoltageToLevel(newSP, false); 

	#pragma message ("Fix the hardcoded non-bipolar inputs")

	// Retrive associated controller from the appropriate output channel object
	Controller * targetController = ctrlChannels[(int)channelNum]->getContainingController();

	if (!targetController) { // If there's no Controller associated

		Serial.print(F("#SetSetpoint error: no lock running on channel "));
		Serial.print(params[1]);

		return;
	}

	targetController->setSetpoint(newSP_F);

	// Report
	Serial.print(F("#SetSetpoint "));
	Serial.print(params[1]);
	Serial.print(", ");
	Serial.println(newSP, FLT_PRINT_PRECISION);
}

// Sets the thresholds at which the LEDs start flashing (globally initially)
// Expects 2 params : a double, the higher threshold
//                    a double, the lower threshold
void setThresholds(const ParameterLookup& params) {

	if (isSerialControlDisabled()) return;

	CONSOLE_LOG_LN(F("setThresholds()"));

	double levelHigh = atof(params[1]);
	double levelLow = atof(params[2]);

	// Don't check for sanity, this is the user's problem and shouldn't cause overflows etc.

	// Store	
	led_thresholds[0] = levelHigh;
	led_thresholds[1] = levelLow;

	// Report
	Serial.print(F("#setThresholds "));
	Serial.print(levelHigh, FLT_PRINT_PRECISION);
	Serial.print(", ");
	Serial.println(levelLow, FLT_PRINT_PRECISION);

}

// Sets the thresholds at which the LEDs start flashing (globally initially)
// Expects 0 params.
void getThresholds(const ParameterLookup&) {

	CONSOLE_LOG_LN(F("getThresholds()"));

	// Report
	Serial.print(led_thresholds[0], FLT_PRINT_PRECISION);
	Serial.print(", ");
	Serial.println(led_thresholds[1], FLT_PRINT_PRECISION);

}

// Do a PID lock, varying the output voltage
// Expects 7 params : a string, the output channel number ("1", "2", "BP")
//                    a string, the input channel ("1T", "1V", "2T", "2V", "TMP", "POW")
//                    a double, the error setpoint value in volts
//					  a double, the proportional gain
//                    a double, the integral time
//                    a double, the differential time
//                    a double, N
void voltageLock(const ParameterLookup& params) {

	if (isSerialControlDisabled()) return;

	CONSOLE_LOG(F("voltageLock::start, freemem = "));
	CONSOLE_LOG_LN(freeMemory());

	const CtrlChanIdx outputChannel = stringToOPAChannel(params[1]);

	if (outputChannel == CtrlChanIdx::NOT_FOUND) {
		Serial.print(F("#StartLock"));
		channelNotRecognised(params[1]);
		return;
	}

	const ErrorChanIdx errorChannel = stringToADCChannel(params[2]);

	if (errorChannel == ErrorChanIdx::NOT_FOUND) {
		Serial.print(F("#StartLock"));
		channelNotRecognised_ADC(params[2]);
		return;
	}

	const double setpoint = atof(params[3]);

	const double K = atof(params[4]);
	const double Ti = atof(params[5]);
	const double Td = atof(params[6]);
	const double N = atof(params[7]);

	// Check all parameters for sanity
	if (isnan(K) || isnan(Ti) || isnan(Td) || isnan(N)) {
		Serial.println(F("#StartLock error: invalid parameter values"));

		return;
	}

	if (Ti <= 0 || N <= 0) {
		Serial.println(F("#StartLock error: Ti and N cannot be <= 0"));
		return;
	}

	// Convert setpoint from volts to +-1 format
	const double setpointF = errorVoltageToLevel(setpoint, voltageToBeDoubled(errorChannel));

	// The coefficients Ti and Td are in seconds, N is unitless so these need no scaling
	// However, K is in units of <output volts> / <error volts>
	// To convert it to +-1 format, we therefore need to scale it appropriatly.
	// pmFmt_per_errVlt  is in units of <+- error format>  / <error volts>
	// pmFmt_per_ctrlVlt is in units of <+- output format> / <output volts>
	// => K * pmFmt_per_ctrlVlt / pmFmt_per_errVlt gives unit conversion as shown:
	//   K_scaled = <output volts> / <error volts> * (<+- output format> / <output volts>) / (<+- error format>  / <error volts>)
	//   K_scaled = <+- output format> / <+- error format>
	// ...the correct format for K_scaled
	const double pmFmt_per_errVlt   = errorVoltageToLevel(1, voltageToBeDoubled(errorChannel)) - errorVoltageToLevel(0, voltageToBeDoubled(errorChannel));
	const double pmFmt_per_ctrlVlt  = voltageToCtrlLevel(1, isBipolar(outputChannel)) - voltageToCtrlLevel(0, isBipolar(outputChannel));
	const double scaleFactor = pmFmt_per_ctrlVlt / pmFmt_per_errVlt;
	const double K_scaled = K * scaleFactor;

	CONSOLE_LOG(F("voltageLock::K_scaled = "));
	CONSOLE_LOG_LN(K_scaled, FLT_PRINT_PRECISION);

	CONSOLE_LOG_LN(F("voltageLock::closeConflictingControllers..."));

	// Get a pointer to the CtrlChannel
	CtrlChannel* const ctrlChannelPtr = ctrlChannels[(int)outputChannel];
	
	// Close any Controllers that are managing this channel currently
	const auto res = ctrlChannelPtr->closeConflictingControllers();
	if (res != CtrlChannelReturn::NO_ERROR) {
		reportCtrlError(res);
		return;
	}

	// The requested channel and any conflicting channels are now not under control

	// Find the slot to store this Controller in
	const int controllerIdx = findSlotForController(outputChannel);

	// Check that we found a slot
	if (controllerIdx == -1) {
		Serial.println(F("#StartLock error: no free Controller slot"));
		return;
	}

	CONSOLE_LOG(F("Free controllers slot found @ "));
	CONSOLE_LOG_LN(controllerIdx);

	// Now, we constuct a PIDAlgorithm with the requested parameters and store it in the stack allocated space,
	// same index as we have found to store the new Controller
	const double initialOutput = ctrlChannels[(int)outputChannel]->recallCtrl();
	pidAlgorithms[controllerIdx] = PIDAlgorithm(K, Ti, Td, initialOutput, setpointF, N);

	// Save a pointer to this algorithm
	PIDAlgorithm* const algoPtr = &pidAlgorithms[controllerIdx];

	CONSOLE_LOG(F("*** voltageLock::Algo made - "));
	CONSOLE_LOG_LN((int)algoPtr);

	// Get a pointer to the error channel
	ErrorChannel* const errorChannelPtr = errorChannels[(int)errorChannel];

	CONSOLE_LOG(F("*** Error Retrieved - "));
	CONSOLE_LOG_LN((int)errorChannelPtr);

	// We already have a pointer to the control channel

	CONSOLE_LOG(F("*** Ctrl Retrieved - "));
	CONSOLE_LOG_LN((int)ctrlChannelPtr);

	// Tell the Algorithm about any ctrl limits that are applicable
	double ctrlMin, ctrlMax;
	CtrlChannelReturn err = ctrlChannelPtr->getLimits(ctrlMin, ctrlMax);

	if (err != CtrlChannelReturn::NO_ERROR) {
		reportCtrlError(err);
		return;
	}

	algoPtr->setLimits(ctrlMin, ctrlMax);

	// Store the three parts in the Controller
	controllers[controllerIdx] = Controller(errorChannelPtr, ctrlChannelPtr, algoPtr);

	CONSOLE_LOG(F("*** Controller stored @ "));
	CONSOLE_LOG_LN((int)&controllers[controllerIdx]);

	// Ensure the new controller is valid
	if (!(controllers[controllerIdx])) {
		CONSOLE_LOG_LN(F("*** ERROR Controller invalid"));
		Serial.print(F("#StartLock error: assigning control interface to new controller failed on channel "));
		Serial.println(params[1]);

		controllers[controllerIdx].reset();
		return;
	}

	CONSOLE_LOG_LN(F("*** Controller valid"));
	// Report
	Serial.print(F("#StartLock "));
	Serial.print(params[1]); // output channel
	Serial.print(' ');
	Serial.print(params[2]); // input channel
	Serial.print(' ');
	Serial.print(setpoint, FLT_PRINT_PRECISION);
	Serial.print(' ');
	Serial.print(K, FLT_PRINT_PRECISION);
	Serial.print(' ');
	Serial.print(Ti, FLT_PRINT_PRECISION);
	Serial.print(' ');
	Serial.print(Td, FLT_PRINT_PRECISION);
	Serial.print(' ');
	Serial.println(N, FLT_PRINT_PRECISION);

	CONSOLE_LOG_LN(F("*** Finished"));
}

// Output constant voltage
// Expects 2 params : a string, the output channel number ("1", "2", "BP")
//                    a double, the voltage
void constVoltage(const ParameterLookup& params) {

	if (isSerialControlDisabled()) return;

	CtrlChanIdx outputChannel = stringToOPAChannel(params[1]);

	if (outputChannel == CtrlChanIdx::NOT_FOUND) {
		Serial.print(F("#ConstVoltage"));
		channelNotRecognised(params[1]);
		return;
	}

	double voltage = atof(params[2]);

	// Close any Controllers that are managing this channel currently
	auto r = ctrlChannels[(int)outputChannel]->closeConflictingControllers();
	if (r != CtrlChannelReturn::NO_ERROR) {
		reportCtrlError(r);
		return;
	}

	// Convert voltage given in volts to -1 -> +1 format
	double level = voltageToCtrlLevel(voltage, isBipolar(outputChannel));

	// Request this level
	ctrlChannels[(int)outputChannel]->setCtrl(level);

	// Report
	Serial.print(F("#ConstVoltage "));
	Serial.print(params[1]);
	Serial.print(' ');
	Serial.println(voltage, FLT_PRINT_PRECISION);
}

void availableMemory(const ParameterLookup&) {
	int val = freeMemory();

	Serial.print(val);
	Serial.println(F(" bytes remain"));
}

void storeCommand(const ParameterLookup& params) {

	if (isSerialControlDisabled()) return;

	auto r = handler.storeStartupCommand(params[-2], false);

	if (CommandHandlerReturn::NO_ERROR == r) {
		Serial.println(F("Done"));
	} else {
		Serial.print(F("Error "));
		Serial.println(int(r));
	}
}

void appendCommand(const ParameterLookup& params) {

	if (isSerialControlDisabled()) return;

	auto r = handler.storeStartupCommand(params[-2], true);

	if (CommandHandlerReturn::NO_ERROR == r) {
		Serial.println(F("Done"));
	} else {
		Serial.print(F("Error "));
		Serial.println(int(r));
	}
}

void retreiveCommand(const ParameterLookup&) {

	char * command;

	// Assign space for the command
	command = (char*)malloc( sizeof(char) * EEPROM_SIZE_MAX );

	if (command == 0) {
		Serial.println(F("Error: out of mem"));
		return;
	}

	// Load the command into the newly created buffer
	handler.getStartupCommand(command);

	if (command[0] == '\0') {
		Serial.println(F("None"));
	}
	else {
		// I'm being cheeky here in that I'm not adding my own newline by using Serial.println instead of 
		// Serial.print, because I'm counting on the command already containing a newline. 
		// Hopefully this doesn't come back to bite me. 
		Serial.print(command);
	}

	// Free the buffer
	free(command);
}

void wipeCommand(const ParameterLookup&) {

	if (isSerialControlDisabled()) return;

	bool result = handler.wipeStartupCommand();

	if (result) {
		Serial.println(F("Done"));
	}
	else {
		Serial.println(F("Failed"));
	}
}


//////////////////////////////////////////////////////
//													//
//				HELPER FUNCTIONS					//
//													//
//////////////////////////////////////////////////////

// Parse a string to get a channel num for the ADC
// Options are:
// 1T
// 2T
// 1V
// 2V
// TMP
// POW
// other -> NOT_FOUND
ErrorChanIdx stringToADCChannel(const char str[]) {

	const char options[7][4] = { "1T", "2T", "1V", "2V", "TMP", "POW", "TST" };
	const size_t numOptions = 7;

	// Loop through strings and compare
	for (size_t i = 0; i < numOptions; i++) {
		if (0 == strcasecmp(str, options[i])) {
			return ErrorChanIdx(i);
		}
	}

	return ErrorChanIdx::NOT_FOUND;
}

// Parse a string to get a channel num for the OPA
// Options for two channel board:
// 1
// 2
// BP
// other -> NOT_FOUND
//
// Options for four channel board:
// 1
// 2
// BPA
// 3
// 4
// BPB
// other -> NOT_FOUND
CtrlChanIdx stringToOPAChannel(const char str[]) {

#ifdef FOUR_CHANNEL_BOARD
	const char options[6][4] = { "1", "2", "3", "4", "BPA", "BPB" };
	const size_t numOptions = 6;
#else
	const char options[3][4] = { "1", "2", "BPA" };
	const size_t numOptions = 3;
#endif

	// Loop through strings and compare
	for (size_t i = 0; i < numOptions; i++) {
		if (0 == strcasecmp(str, options[i])) {
			// This cast requires that the enum class CtrlChanIdx is in the same order as the arrays hard-coded here
			return CtrlChanIdx(i);
		}
	}

	return CtrlChanIdx::NOT_FOUND;
}

// Returns the index of the slot for the next Controller, according to the
// rules shown in this comment
// N.B. This function accesses the global `controllers` vector so is not properly encapsulated
// 
// " // The `controllers` array can contain several Controllers, potentially one for each output channel.
		// Since there are only two LEDs, we can't in general display status LEDs for all of them, although 
		// 99% of the use cases for this device will involve a maximum of two Controllers. 
		// 
		// The Controller 
		// creation code puts the Controllers into the array in a sensible order, prioritising the first 2 slots. 
		// Here I decided to have the LEDs represent the Controllers stored only in the first two slots of the 
		// array. The points at which we create the Controllers
		
		// There are only 2 LEDs, so only output statuses for the first two active Controllers (void voltageLock(...))
		// will make sure that they are put in the right place. 
		// 
		// For reference, this place will be:
		// 
		// Output channel		LED
		// 				1		1
		// 				2		1
		// 				3		2
		// 				4		2
		//				BPA		1
		//				BPB		2
		//				
		// In the event of a clash, e.g. channels 1 and 2 are locked at the same time, the second one to be started will put 
		// assigned to the other LED. If this is not possble (e.g. channels 1 and 3 are locked, then 2 is started) then no
		// LED will be assigned
//	"
static int findSlotForController(const CtrlChanIdx outputChannel) {

	int preferredSlot;

	const size_t arrayLen = sizeof(controllers) / sizeof(*controllers);

	CONSOLE_LOG(F("findSlotForController with array size = "));
	CONSOLE_LOG_LN(arrayLen);

	// Figure out which slot we'd like to put this Controller into, if it's free
#ifdef FOUR_CHANNEL_BOARD
	switch (outputChannel) {
		case CtrlChanIdx::CHAN_1 	:
		case CtrlChanIdx::CHAN_2 	:
		case CtrlChanIdx::CHAN_BP_12 	:

			preferredSlot = 0;
			break;

		case CtrlChanIdx::CHAN_3 	:
		case CtrlChanIdx::CHAN_4 	:
		case CtrlChanIdx::CHAN_BP_34 	:

			preferredSlot = 1;
			break;
	}
#else
	switch (outputChannel) {
		case CtrlChanIdx::CHAN_1 	:
		case CtrlChanIdx::CHAN_BP_12 	:

			preferredSlot = 0;
			break;

		case CtrlChanIdx::CHAN_2 	:

			preferredSlot = 1;
			break;

		default:

			// Error!
			return -1;
	}
#endif

	// If the slot is available, use it
	if (preferredSlot < arrayLen && !controllers[preferredSlot])
		return preferredSlot;

	// If not, check the other one
	const int otherSlot = (preferredSlot == 0 ? 1 : 0);
	if (otherSlot < arrayLen && !controllers[otherSlot])
		return otherSlot;	

	// If both the above fail, then just put the Controller in the next free slot
	for (size_t i = 0; i < arrayLen; i++) {
		if (!controllers[i]) { // is controller invalid?
			
			// We found a slot
			return i;
		}
	}

	// Somehow we managed not to find a single slot! 
	// Return an error
	return -1;
}

// Close the controller directly managing a channel if it exists
static void closeChannelsController(CtrlChannel * chan) {

	Controller * contr = chan->getContainingController();

	if (contr) {
		CONSOLE_LOG(F("closeChannelsController::Controller "));
		CONSOLE_LOG((int)contr);
		CONSOLE_LOG(F(" of channel "));
		CONSOLE_LOG((int)chan);
		CONSOLE_LOG_LN(F(" closed"));


		contr->reset();
	}
	else {
		CONSOLE_LOG(F("closeChannelsController::Channel "));
		CONSOLE_LOG((int)chan);
		CONSOLE_LOG_LN(F(" not managed"));
	}
}

static void channelNotRecognised(const char channel[]) {
	Serial.print(F(" Error: output channel "));
	Serial.print(channel);
#ifndef FOUR_CHANNEL_BOARD
	Serial.println(F(" not recognised. Options are 1, 2, BPA"));
#else
	Serial.println(F(" not recognised. Options are 1, 2, BPA, 3, 4, BPB"));
#endif
}

static void channelNotRecognised_ADC(const char channel[]) {
	Serial.print(F(" Error: input channel "));
	Serial.print(channel);
	Serial.println(F(" not recognised. Options are 1T, 1V, 2T, 2V, TMP, POW"));
}


// Convert output voltage given in volts to -1 -> +1 format
// where -1 -> 0, +1 -> MAX_VOLTAGE if single channel, 
// or -MAX_VOLTAGE -> +MAX_VOLTAGE if bipolar
double voltageToCtrlLevel(double voltage, bool isBipolar) {

	double level;
	if (isBipolar) {
		level = voltage / double(MAX_VOLTAGE);
	}
	else {
		level = 2.0 * voltage / double(MAX_VOLTAGE) - 1.0;
	}

	return level;
}

// Convert level given in -1 -> +1 format to output volts
// where voltage range is 0 -> MAX_VOLTAGE if single channel, 
// or -MAX_VOLTAGE -> +MAX_VOLTAGE if bipolar
double ctrlLevelToVoltage(double level, bool isBipolar) {

	double voltage;
	if (isBipolar) {
		voltage = level * double(MAX_VOLTAGE);
	}
	else {
		voltage = (level + 1.0) * double(MAX_VOLTAGE) / 2.0;
	}

	return voltage;
}

// Convert input voltage given in volts to -1 -> +1 format
// where range is +- 2.5 for the thermistors or +- 5 for the generic inputs
double errorVoltageToLevel(double voltage, bool doubledVoltage) {

	double divider = (doubledVoltage ? 5.0 : 2.5);

	return voltage / divider;

}

// Convert -1 -> +1 format to input voltage in volts
// where range is +- 2.5 for the thermistors or +- 5 for the generic inputs
double levelToErrorVoltage(double level, bool doubledVoltage) {

	double multiplier = (doubledVoltage ? 5.0 : 2.5);

	return level * multiplier;

}

// Does this ErrorChannel measure voltages from 0 -> 5V (true) or 0 -> 2.5V (false)
inline bool voltageToBeDoubled(const ErrorChanIdx chan) {

#ifndef	FOUR_CHANNEL_BOARD
	return chan == ErrorChanIdx::CHAN_1V || chan == ErrorChanIdx::CHAN_2V;
#else
	return false;
#endif
}

// Does this CtrlChannel output bipolar voltages (true) or one-sided (false)?
inline bool isBipolar(const CtrlChanIdx chan) {
#ifdef FOUR_CHANNEL_BOARD
	return chan == CtrlChanIdx::CHAN_BP_12 || chan == CtrlChanIdx::CHAN_BP_34;
#else
	return chan == CtrlChanIdx::CHAN_BP_12;
#endif
}

void startWatchdog() {

	// Clear MCU Status Register. 
	MCUSR = 0;

	// Disable and clear all Watchdog settings
	WDTCSR = (1 << WDCE);
	WDTCSR = 0;

	// Setup Watchdog for instant reset, with a 2s timeout
	const uint8_t newWDTCSR = (1 << WDCE) | (1 << WDE) | (1 << WDP2) | (1 << WDP1) | (1 << WDP0);

	CONSOLE_LOG(F("startWatchdog::setting WDTCSR to 0b"));
	CONSOLE_LOG_LN(newWDTCSR, BIN);

	// Reset counter
	wdt_reset();

	// Disable all interrupts
	noInterrupts();

	WDTCSR = (1 << WDCE) | (1 << WDE);
	WDTCSR = newWDTCSR;

	// Reset counter
	wdt_reset();

	// Reenable all interrupts.
	interrupts();

}

void reportCtrlError(CtrlChannelReturn err) {
	Serial.print(F("Error: CtrlChannelReturn code "));
	Serial.println((int)err);
}

void reportErrorError(ErrorChannelReturn err) {

	if (err == ErrorChannelReturn::NO_SUCH_CHANNEL) {
		Serial.println(F("Error: channel does not exist in OPA container"));
		return;
	}

	Serial.print(F("Error:ErrorChannelReturn code "));
	Serial.println((int)err);
}

// Check to see if there's any free memory left. 
// If not, loop forever and report this by serial
inline void checkOutOfMem() {
	char * testChar = (char*)malloc(1);
	if (testChar == 0) {
		while (true) {
			wdt_reset();
			Serial.println(F("OUT OF MEM"));
			delay(1000);
		}
	}
	CONSOLE_LOG(F("testChar assigned at "));
	CONSOLE_LOG_LN((int)testChar);
	free(testChar);
}

// Read the supply voltage and set the OPA's maximum voltage accordingly
double calculateOPAMaxVoltage() {

	double voltage;

	if (DIVIDED_SUPPLY_VOLTAGE == 0xFF) {
		voltage = FALLBACK_SUPPLY_VOLTAGE;
	} else {

		pinMode(INPUT, DIVIDED_SUPPLY_VOLTAGE);

		// Take 20 readings and average
		const int n = 20;
		int level = 0;

		for (int i = 0; i < n; i++) {
			wdt_reset();
			delay(2);
			level += analogRead(DIVIDED_SUPPLY_VOLTAGE);
		}

		voltage = double(level) / double(n) * (5.0 / 1024.0) * DIVIDED_SUPPLY_FACTOR;
	}

	// Make sure it's 0.3V away from the rail
	const double maxOPAOutput = voltage - 0.3;

	// Calculate the maximum possible output based on the DAC's gain
	const double maxDacOutput = 2.5 * OPA_GAIN - 0.3;

	const double output = maxOPAOutput < maxDacOutput ? maxOPAOutput : maxDacOutput;

	return output > 0 ? output : 0;
}

// Check if serial control is disabled
bool isSerialControlDisabled() {

	// Fallback for this pin not being present
	if (DISABLE_SERIAL_CTRL == 0xFF)
		return false;

	pinMode(INPUT, DISABLE_SERIAL_CTRL);

	// Read the voltage on DISABLE_SERIAL_CTRL. This must be analog since this pin
	// on the ATMega does not have the capability for digital readings
	int level = analogRead(DISABLE_SERIAL_CTRL);

	const bool disabled = level > 1024/2;

	if (disabled)
		Serial.println(F("Error: serial control is disabled"));

	return disabled;
}