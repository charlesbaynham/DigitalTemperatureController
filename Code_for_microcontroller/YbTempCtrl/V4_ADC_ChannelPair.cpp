#include "V4_ADC_ChannelPair.h"

#include "MemoryFree.h"

// Initialise space for storing the ADC's registers' values
uint8_t ADS1262::registers_data[7] = {0};

namespace YbCtrl {

	// Initialize the SPISettings for the ADC
	SPISettings V4_ADC_ChannelPair::_settingsADC = SPISettings(7E6, MSBFIRST, SPI_MODE1);

	// Initialize the _readingChannel pointer
	V4_ADC_ChannelPair * V4_ADC_ChannelPair::_readingChannel = 0;

	// Mark the registers as unloaded
	bool V4_ADC_ChannelPair::_ADC_registers_loaded = false;

	/************************************************
	Internal functions to interface with the ADC:
	************************************************/

	void V4_ADC_ChannelPair::resetADC() {
		CONSOLE_LOG_LN(F("V4_ADC_ChannelPair::resetADC"));

		setupADCpins();

		SPI.beginTransaction(_settingsADC);

		digitalWrite(ADC_CS, LOW);

		SPI.transfer(RESET); // Write RESET opcode

		// End SPI transaction
		digitalWrite(ADC_CS, HIGH);

		SPI.endTransaction();

		delay(100);

		// Reload the registers
		getRegisters();

		CONSOLE_LOG_LN(F("V4_ADC_ChannelPair::resetADC done"));
	}

	void V4_ADC_ChannelPair::setupADCpins() {

		CONSOLE_LOG_LN(F("V4_ADC_ChannelPair::setupADCpins"));

		// Setup pins
		pinMode(ADC_CS, OUTPUT);
		pinMode(ADC_START, OUTPUT);
		pinMode(ADC_DRDY, INPUT);

		// disable ADC SPI by setting ~CS high
		digitalWrite(ADC_CS, HIGH);

		SPI.begin();

	}

	ErrorChannelReturn V4_ADC_ChannelPair::writeAndCheckADCReg(uint8_t reg, uint8_t data) {

		writeADCReg(reg, data);

		if (data == readADCReg(reg))
			return ErrorChannelReturn::NO_ERROR;

		return ErrorChannelReturn::WRITING_TO_REG_FAILED;

	}

	void V4_ADC_ChannelPair::writeADCReg(uint8_t reg, uint8_t data) {

		CONSOLE_LOG_LN(F("V4_ADC_ChannelPair::writeADCReg"));

		SPI.beginTransaction(_settingsADC);

		digitalWrite(ADC_CS, LOW);

		SPI.transfer(WREG | (reg & 0b00011111)); // Write WREG opcode

		SPI.transfer(0x00); // Write to only the one register

		SPI.transfer(data); // Write the data

		// End SPI transaction
		digitalWrite(ADC_CS, HIGH);

		SPI.endTransaction();

		CONSOLE_LOG_LN(F("V4_ADC_ChannelPair::writeADCReg done"));
	}

	uint8_t V4_ADC_ChannelPair::readADCReg(uint8_t reg) {

		CONSOLE_LOG(F("V4_ADC_ChannelPair::readADCReg 0b"));
		CONSOLE_LOG_LN(RREG | (reg & 0b00011111), BIN)

		SPI.beginTransaction(_settingsADC);
		digitalWrite(ADC_CS, LOW);

		SPI.transfer(RREG | (reg & 0b00011111)); // Write WREG opcode
		SPI.transfer(0x00); // Read to only the one register
		uint8_t output = SPI.transfer(0); // Read the data

										  // End SPI transaction
		digitalWrite(ADC_CS, HIGH);
		SPI.endTransaction();

		CONSOLE_LOG_LN(F("V4_ADC_ChannelPair::readADCReg done"));

		return output;
	}

	bool V4_ADC_ChannelPair::readingReady() {

		CONSOLE_LOG_LN(F("V4_ADC_ChannelPair::readingReady() ?"));

		// Check if this object controls the ADC
		if (_readingChannel == this) {

			// If so, check if the reading in process is complete
			return !digitalRead(ADC_DRDY);
		}

		// If not, there's no reading to be ready! Return false
		return false;
	}

	bool V4_ADC_ChannelPair::readingTimeout() {
		CONSOLE_LOG_LN(F("V4_ADC_ChannelPair::readingTimeout()"));
		return millis() > _timeout;
	}

	void V4_ADC_ChannelPair::abortReading() {

		CONSOLE_LOG_LN(F("V4_ADC_ChannelPair::abortReading()"));

		// Remove control of the ADC from whichever channel had it, if any
		_readingChannel = 0;

	}

	ErrorChannelReturn V4_ADC_ChannelPair::startReading() {

		CONSOLE_LOG(F("V4_ADC_ChannelPair::startReading highRes = "));
		CONSOLE_LOG_LN(_highRes);

		// Setup pin modes
		setupADCpins();

		// Check if the ADC is already busy
		if (globalReadInProgress())
			abortReading();

		// Stop conversions
		digitalWrite(ADC_START, LOW);

		// *** Setup the stored registers for the settings we want ***
		
		// Set the channels
		bitmaskWrite(ADS1262::REG_INPMUX, ADS1262::REG_INPMUX_MUXP, _channel1);
		bitmaskWrite(ADS1262::REG_INPMUX, ADS1262::REG_INPMUX_MUXN, _channel2);

		// Set chop mode, data rate and filter mode
		if (_highRes) {
			bitmaskWrite(ADS1262::REG_MODE0, ADS1262::REG_MODE0_CHOP, 	1); // Chop mode on
			bitmaskWrite(ADS1262::REG_MODE1, ADS1262::REG_MODE1_FILTER, 0b011); // Filter = sinc4
			bitmaskWrite(ADS1262::REG_MODE2, ADS1262::REG_MODE2_DR, 	0b0000); // Data rate = 2.5 SPS

			// Should we increase the gain?
			if (_numLowReadings >= __maxLowReadings && _PGA_gain < 0b101) { // 0b101 is the highest gain setting allowed, see datasheet
				_PGA_gain++;
				_numLowReadings = 0;

				CONSOLE_LOG(F("Boosting gain to : 0x"));
				CONSOLE_LOG_LN(_PGA_gain);
			}

			// Set the gain
			bitmaskWrite(ADS1262::REG_MODE2, ADS1262::REG_MODE2_GAIN, 	_PGA_gain);
		} else {
			bitmaskWrite(ADS1262::REG_MODE0, ADS1262::REG_MODE0_CHOP, 	0); // Chop mode off
			bitmaskWrite(ADS1262::REG_MODE1, ADS1262::REG_MODE1_FILTER, 0b100); // Filter = FIR
			bitmaskWrite(ADS1262::REG_MODE2, ADS1262::REG_MODE2_DR, 	0b0100); // Data rate = 20 SPS (Don't go higher for FIR filter)
		}

		// Send settings
		ErrorChannelReturn result = writeRegisters();
		if (ErrorChannelReturn::NO_ERROR != result) 
		{
			return result;
		}

		CONSOLE_LOG_LN(("V4_ADC_ChannelPair::Requesting data..."));

		// Save the timeout value
		_timeout = millis() + (_highRes ? __highResTimeout : __lowResTimeout);

		// Start conversions
		digitalWrite(ADC_START, HIGH);

		// Mark the reading as started by taking ownership of the ADC
		_readingChannel = this;

		return ErrorChannelReturn::NO_ERROR;
	}


	ErrorChannelReturn V4_ADC_ChannelPair::getReading(double &readingOutput)
	{
		CONSOLE_LOG_LN(F("V4_ADC_ChannelPair::getReading()"))

		// A reading is ready!
		
		// Stop conversions
		digitalWrite(ADC_START, LOW);

		// Relinquish control of the ADC
		_readingChannel = 0;

		CONSOLE_LOG_LN(F("V4_ADC_ChannelPair::Reading response..."));

		// Read the data
		union {
			long ll;
			uint8_t ss[4];
		} dataADC;
		uint8_t statusADC;

		SPI.beginTransaction(_settingsADC);
		digitalWrite(ADC_CS, LOW);

		SPI.transfer(0x12); // Write ADC OPCODE for "read ADC 1"

		statusADC = SPI.transfer(0); // Read status
		dataADC.ss[3] = SPI.transfer(0); // Read data
		dataADC.ss[2] = SPI.transfer(0); // Read data
		dataADC.ss[1] = SPI.transfer(0); // Read data
		dataADC.ss[0] = SPI.transfer(0); // Read data

		// End SPI transaction
		digitalWrite(ADC_CS, HIGH);
		SPI.endTransaction();

		CONSOLE_LOG(F("V4_ADC_ChannelPair::Status read: 0b"));
		CONSOLE_LOG_LN(statusADC, BIN);

		CONSOLE_LOG(F("V4_ADC_ChannelPair::Response read: 0b"));
		CONSOLE_LOG_LN(dataADC.ll, BIN);

		// Does the status byte indicate that hit the PGA limits?
		if (statusADC & PGAD_ALM) {
			// Output 0 arbitarily: we will mark this as invalid
			readingOutput = 0;

			// Do not alter the _lastReading
			
			// Decrease the PGA gain to fix this
			if (_PGA_gain > 0)
				_PGA_gain--;

			CONSOLE_LOG(F("V4_ADC_ChannelPair::PGA out of range. Reducing to 0x"));
			CONSOLE_LOG_LN(_PGA_gain, HEX);

			// Mark this result as invalid
			return ErrorChannelReturn::OUT_OF_RANGE;
		}
		
		// Scale to correct -1 -> +1
		readingOutput = (double)dataADC.ll / (double)0x7FFFFFFF;

		// Check if we could have used a higher gain
		// I.e. was the reading less than __lowGainThreshold % of our 2.5V ref. 
		if (abs(readingOutput) < __lowGainThreshold) {
			// Mark this as a reading that could have been better at a higher gain
			_numLowReadings++;

			CONSOLE_LOG(F("Low reading. _numLowReadings = "));
			CONSOLE_LOG_LN(_numLowReadings);
		}
		else
		{
			_numLowReadings = 0;

			CONSOLE_LOG_LN(F("In range reading. _numLowReadings = 0"));
		}

		// Apply gain correction
		CONSOLE_LOG(F("Gain was : 0x"));
		CONSOLE_LOG_LN(bitmaskRead(ADS1262::REG_MODE2, ADS1262::REG_MODE2_GAIN), HEX);

		switch (bitmaskRead(ADS1262::REG_MODE2, ADS1262::REG_MODE2_GAIN)) {

			case 0b000: // 1 V/V
			            
			        	readingOutput /= 1;
			        	break;

			case 0b001: // 2 V/V
			            
			        	readingOutput /= 2;
			        	break;

			case 0b010: // 4 V/V
			            
			        	readingOutput /= 4;
			        	break;

			case 0b011: // 8 V/V
			            
			        	readingOutput /= 8;
			        	break;

			case 0b100: // 16 V/V
			            
			        	readingOutput /= 16;
			        	break;

			case 0b101: // 32 V/V
			            
			        	readingOutput /= 32;
			        	break;

			default:

						// Something went wrong. Reset PGA gain to default 1V/V and output an error
						CONSOLE_LOG(F("Messed up PGA gain! It was: 0x"));
						CONSOLE_LOG_LN(_PGA_gain, HEX);

						_PGA_gain = 0;
						readingOutput = 0;

						return ErrorChannelReturn::PGA_ERROR;

		}

		// Store for future recall
		_lastReading = readingOutput;

		CONSOLE_LOG(F("V4_ADC_ChannelPair::Output: "));
		CONSOLE_LOG_LN(readingOutput);

		return ErrorChannelReturn::NO_ERROR;
	}

	void V4_ADC_ChannelPair::getRegisters() {

		CONSOLE_LOG_LN(F("getRegisters() :"));

		for (size_t i = 0; i < 7; i++) {
			uint8_t regval = readADCReg(i);
			ADS1262::regWrite((ADS1262::registers)i, regval);

			CONSOLE_LOG(F("reg "));
			CONSOLE_LOG(i);
			CONSOLE_LOG(F(" = 0x"));
			CONSOLE_LOG_LN(regval, HEX);
		}

		_ADC_registers_loaded = true;

	}

	ErrorChannelReturn V4_ADC_ChannelPair::writeRegisters() {

		CONSOLE_LOG_LN(F("writeRegisters() :"));

		for (size_t i = 0; i < 7; i++) {

			CONSOLE_LOG("0x");
			CONSOLE_LOG_LN(ADS1262::regRead((ADS1262::registers)i), HEX);

			ErrorChannelReturn r = writeAndCheckADCReg(i, ADS1262::regRead((ADS1262::registers)i));
			if (ErrorChannelReturn::NO_ERROR != r) {
				CONSOLE_LOG_LN(F("Writing to reg failed"))
				return r;
			}
		}

		CONSOLE_LOG_LN(F("writeRegisters done"));

		return ErrorChannelReturn::NO_ERROR;
	}
}


