#include "V4_OPA_OutputChannelBipolar.h"

namespace YbCtrl {


	// Limits the current. Value in amps
	CtrlChannelReturn V4_OPA_OutputChannelBipolar::setCurrentLimit(double val) {
		CONSOLE_LOG_LN("V4_OPA_OutputChannelBipolar::limitCurrent");
		initOPAs();

		CtrlChannelReturn res1 = setOPACurrentLimit(val, _channelVLim, _smallerOPA);
		CtrlChannelReturn res2 = setOPACurrentLimit(val, _channelVLimAlt, _smallerOPAAlt);

		if (res1 == CtrlChannelReturn::NO_ERROR && res2 == CtrlChannelReturn::NO_ERROR)
			_currLim = val;

		if (res1 == CtrlChannelReturn::NO_ERROR) {
			return res2;
		}
		else
		{
			return res1;
		}
	}

	// Writes to the control signal. Value from -1 -> +1
	void V4_OPA_OutputChannelBipolar::writeCtrl(double val) {
		CONSOLE_LOG_LN("V4_OPA_OutputChannelBipolar::writeCtrl");
		initOPAs();

		// Coerce between limits
		if (val > _max)
			val = _max;
		else if (val < _min)
			val = _min;

		// Disable output if 0 requested
		if (val == 0) {
			disableOutput();
		}
		else {
			// Enable output and write voltage if other

			// Scale to volts
			double halfOutput = val * MAX_VOLTAGE / 2.0;
			double midscale = MAX_VOLTAGE / 2.0;
			double outputV1 = midscale + halfOutput;
			double outputV2 = midscale - halfOutput;

			setOPAVoltage(outputV1, _channelVPlus);
			setOPAVoltage(outputV2, _channelVPlusAlt);

			enableOutput();

		}

		_lastCtrl = val;
	}

	// Enable OPA output
	void V4_OPA_OutputChannelBipolar::enableOutput() {
		
		if (ES_FLOAT_ON) {
			// Float both E/S pins
			pinMode(_OPA_ES, INPUT);
			pinMode(_OPA_ESAlt, INPUT);
		} else {
			// Write HIGH on both pins
			pinMode(_OPA_ES, OUTPUT);
			pinMode(_OPA_ESAlt, OUTPUT);
			digitalWrite(_OPA_ES, HIGH);
			digitalWrite(_OPA_ESAlt, HIGH);
		}

		CONSOLE_LOG_LN("V4_OPA_OutputChannelBipolar::Enabled output on pins ");
		CONSOLE_LOG(_OPA_ES);
		CONSOLE_LOG(F(" and "));
		CONSOLE_LOG_LN(_OPA_ESAlt);
	}

	// Disable OPA output
	void V4_OPA_OutputChannelBipolar::disableOutput() {
		
		// Set both outputs to zero
		setOPAVoltage(0, _channelVPlus);
		setOPAVoltage(0, _channelVPlusAlt);

		// Ground both E/S pins
		pinMode(_OPA_ES, OUTPUT);
		digitalWrite(_OPA_ES, LOW);

		pinMode(_OPA_ESAlt, OUTPUT);
		digitalWrite(_OPA_ESAlt, LOW);

		CONSOLE_LOG("V4_OPA_OutputChannelBipolar::Disabled output on pins ");
		CONSOLE_LOG(_OPA_ES);
		CONSOLE_LOG(F(" and "));
		CONSOLE_LOG_LN(_OPA_ESAlt);
	}

	// Check for overheat
	// Returns error code
	CtrlChannelReturn V4_OPA_OutputChannelBipolar::isOverheated(bool& state) {
		CONSOLE_LOG_LN(F("V4_OPA_OutputChannelBipolar::isOverheated(ref)"));
		
		if (_lastCtrl == 0) {
			state = false;
		} else {
			state = ! ( digitalRead(_OPA_ES) && digitalRead(_OPA_ESAlt) );
		}

		return CtrlChannelReturn::NO_ERROR;
	}
}