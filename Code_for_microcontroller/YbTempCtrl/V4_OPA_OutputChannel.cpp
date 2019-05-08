#include "V4_OPA_OutputChannel.h"

namespace YbCtrl {

	SPISettings V4_OPA_OutputChannel::_settingsDAC = SPISettings(50000000, MSBFIRST, SPI_MODE1);

	// ********** PRIVATE SECTION ********** //

	void V4_OPA_OutputChannel::initOPAs() {
		
		// Deselect DAC
		pinMode(_DAC_CS, OUTPUT);
		digitalWrite(_DAC_CS, HIGH);

		// Setup SPI
		SPI.begin();
	}

	CtrlChannelReturn V4_OPA_OutputChannel::setOPAVoltage(double voltage, uint8_t V_Plus_Channel) {

		// OPAs have gain of OPA_GAIN
		double out = voltage / OPA_GAIN;

		CONSOLE_LOG(F("setOPAVoltage channel = "));
		CONSOLE_LOG(V_Plus_Channel);
		CONSOLE_LOG(F(", voltage = "));
		CONSOLE_LOG(voltage);
		CONSOLE_LOG(F(", OPA_GAIN = "));
		CONSOLE_LOG(OPA_GAIN);
		CONSOLE_LOG(F(", out = "));
		CONSOLE_LOG_LN(out);

		// Write the DAC's new level
		return sendDACOut(out, V_Plus_Channel, _DAC_CS);
	}

	// Limit the current by writing to the V_LIM pin
	// Val in amps
	// smallerOPA selects between OPA548 and OPA549
	CtrlChannelReturn V4_OPA_OutputChannel::setOPACurrentLimit(double val, uint8_t V_LIM_Channel, bool smallerOPA) {

		// Sanity check
		if (!(val >= 0)) {
			CONSOLE_LOG_LN(F("setCurrentLimit: ERROR: negative current requested"));
			return CtrlChannelReturn::INVALID_PARAMETER;
		}

		if (smallerOPA) {
			CONSOLE_LOG_LN(F("setCurrentLimit: Smaller OPA is true"));
		}
		else {
			CONSOLE_LOG_LN(F("setCurrentLimit: Smaller OPA is false"));
		}

		// Calculate voltage for v_set to achieve this current

		double v_set;
		if (smallerOPA)
			v_set = 4.75 - 13750.0 * abs(val) / 15000.0; // OPA548's values
		else
			v_set = 4.75 - 7500.0 * abs(val) / 15800.0; // OPA549's values

		// Divide by two since the DAC's output will be doubled:
		v_set /= 2.0;

		CONSOLE_LOG(F("setCurrentLimit: For limit = "));
		CONSOLE_LOG(val);
		CONSOLE_LOG(F(", outputting V_set = "));
		CONSOLE_LOG_LN(v_set);

		// Write to DAC, vlim channel
		return sendDACOut(v_set, V_LIM_Channel, _DAC_CS);
	}

	// Channel here can be 00, 01, 10 or 11
	// Val is in volts, 0 -> +2.5
	CtrlChannelReturn V4_OPA_OutputChannel::sendDACOut(double val, uint8_t channel, uint8_t CS) {

		// Check limits
		if (val < 0)
			val = 0;
		else if (val > 2.5)
			val = 2.5;

		// Scale voltage to 16 bits
		uint16_t outputLevel = val / 2.5 * 0xFFFF;

		CONSOLE_LOG(F("SendDACOut: "));
		CONSOLE_LOG(outputLevel, HEX);
		CONSOLE_LOG(F(" on channel "));
		CONSOLE_LOG_LN(channel);

		SPI.beginTransaction(_settingsDAC);

		digitalWrite(CS, LOW);

		SPI.transfer(0b00010000 | (0b0110 & channel << 1));

		SPI.transfer16(outputLevel);

		digitalWrite(CS, HIGH);
		SPI.endTransaction();

		return CtrlChannelReturn::NO_ERROR;

	}

}