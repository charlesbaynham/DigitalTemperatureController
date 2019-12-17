#pragma once

#include <SPI.h>

#include "Pins.h"
#include "CtrlChannel.h"

namespace YbCtrl {

	/**
	 * @brief      Implementation of a CtrlChannel for a single-ended output
	 *
	 *             This object manages output via a single OPA548 / OPA549. It
	 *             allows for control of the output voltage between 0 and
	 *             #MAX_VOLTAGE.
	 *
	 *             Its most important method is setCtrl(double)
	 *             
	 * @ingroup    CtrlOutput
	 */
	class V4_OPA_OutputChannel :
		public CtrlChannel
	{
	public:

		// Returns the validity of this object
		virtual explicit operator bool() const override { return _isValid; }

		// Limits the current. Value in amps
		// Returns NOT_IMPLEMENTED if this is not implemented
		virtual CtrlChannelReturn setCurrentLimit(double val) override {
			CONSOLE_LOG_LN(F("V4_OPA_OutputChannel::limitCurrent"));

			initOPAs();

			CtrlChannelReturn res = setOPACurrentLimit(val, _channelVLim, _smallerOPA);

			if (res == CtrlChannelReturn::NO_ERROR)
				_currLim = val;

			return res;
		}

		// Get current limit. Value in amps
		// Returns -999 if this is not implemented
		virtual double getCurrentLimit() override {
			CONSOLE_LOG_LN(F("V4_OPA_OutputChannel::getCurrentLimit"));
			return _currLim;
		}

		// Get current limit. Value in amps
		// Returns error code
		virtual CtrlChannelReturn getCurrentLimit(double& out) override {
			CONSOLE_LOG_LN(F("V4_OPA_OutputChannel::getCurrentLimit(ref)"));
			out = _currLim;
			return CtrlChannelReturn::NO_ERROR;
		}

		// Check for overheat
		// Returns error code
		virtual CtrlChannelReturn isOverheated(bool& state);

		// Writes to the control signal. Values from -1 to 1
		virtual void writeCtrl(double val) override {
			CONSOLE_LOG(F("V4_OPA_OutputChannel::writeCtrl with val = "));
			CONSOLE_LOG_LN(val);
			initOPAs();

			// Coerce between limits
			if (val > _max)
				val = _max;
			else if (val < _min)
				val = _min;

			CONSOLE_LOG(F("V4_OPA_OutputChannel::writeCtrl after coersion val = "));
			CONSOLE_LOG_LN(val);

			// disable output if zero requested
			if (val == -1) {
				disableOutput();
			}
			else {
				double voltage = (val + 1.0) * MAX_VOLTAGE / 2.0;

				setOPAVoltage(voltage, _channelVPlus);

				enableOutput();
			}

			_lastCtrl = val;
		}

		
		/**
		 * @brief      Enable OPA output
		 */
		virtual void enableOutput() {
			if (ES_FLOAT_ON) {
				// Float the E/S pin
				pinMode(_OPA_ES, INPUT);
			} else {
				// Write HIGH on the E/S pin
				pinMode(_OPA_ES, OUTPUT);
				digitalWrite(_OPA_ES, HIGH);
			}
			
			CONSOLE_LOG_LN(F("V4_OPA_OutputChannel::Enabled output"));
		}

		
		/**
		 * @brief      Disable OPA output
		 */
		virtual void disableOutput() {
			// Ground the E/S pin
			pinMode(_OPA_ES, OUTPUT);
			digitalWrite(_OPA_ES, LOW);

			CONSOLE_LOG(F("V4_OPA_OutputChannel::Disabled output with pin "));
			CONSOLE_LOG_LN(_OPA_ES);
		}

		// Recall current control value. Value from -1 -> +1
		virtual double recallCtrl() override {
			return _lastCtrl;
		}

		// Recall current control value. Value from -1 -> +1
		// Return error codes
		virtual CtrlChannelReturn recallCtrl(double& out) override {
			out = _lastCtrl;
			return CtrlChannelReturn::NO_ERROR;
		}

		/**
		 * @brief      Constructor
		 *
		 * @param[in]  channelVPlus  DAC channel corresponding to V+
		 * @param[in]  channelVLim   DAC channel corresponsing to Vlim
		 * @param[in]  OPA_ES        ATMega pin corresponding to the OPA's E/S
		 *                           pin
		 * @param[in]  smallerOPA    Is this OPA an OPA548? If so, true. Else if
		 *                           it's an OPA549, false
		 */
		V4_OPA_OutputChannel(uint8_t channelVPlus, uint8_t channelVLim, uint8_t OPA_ES, bool smallerOPA, uint8_t DAC_CS_pin) :
			_channelVPlus(channelVPlus), _channelVLim(channelVLim), _OPA_ES(OPA_ES), _smallerOPA(smallerOPA), _DAC_CS(DAC_CS_pin),
			_lastCtrl(-1), // Output starts at minimum
			_min(-1), _max(1),
			_isValid(true)
		{
			CONSOLE_LOG(F("V4_OPA_OutputChannel::Constuctor of "));
			CONSOLE_LOG_LN((int)this);

			disableOutput();
			setCurrentLimit(MAX_INITIAL_CURRENT);
		}

		V4_OPA_OutputChannel() :
			_isValid(false)
		{
			CONSOLE_LOG_LN(F("V4_OPA_OutputChannel::Default constuctor"));
		}

		virtual ~V4_OPA_OutputChannel() {
			CONSOLE_LOG(F("V4_OPA_OutputChannel::destructor of "));
			CONSOLE_LOG_LN((int)this);
		}

		// Copy constuctor
		// Just use the default
		/*V4_OPA_OutputChannel(const V4_OPA_OutputChannel& rhs) :
			_channelVPlus(rhs._channelVPlus), _channelVLim(rhs._channelVLim),
			_OPA_ES(rhs._OPA_ES), _smallerOPA(rhs._smallerOPA)
		{
			CONSOLE_LOG_LN(F("V4_OPA_OutputChannel::Copy constuctor"));
		}*/
		
		// Get current limits on output voltage
		// Return error code
		CtrlChannelReturn getLimits(double& min, double& max) override {

			min = _min;
			max = _max;

			return CtrlChannelReturn::NO_ERROR;
		}

	protected:

		// Limit output voltage
		CtrlChannelReturn writeLimits(double min, double max) override {

			_min = (min <= -1 ? -1 : min);
			_max = (max >= 1 ? 1 : max);
			_min = (_min <= _max ? _min : _max);

			CONSOLE_LOG(F("V4_OPA_OutputChannel::setLimits : "));
			CONSOLE_LOG(_min);
			CONSOLE_LOG(F(", "));
			CONSOLE_LOG_LN(_max);

			return CtrlChannelReturn::NO_ERROR;
		}

		/************************************************
		Internal methods to interface with the OPAs:
		************************************************/

		uint8_t _channelVPlus, _channelVLim, _OPA_ES, _smallerOPA, _DAC_CS;

		double _min, _max;

		double _currLim;

		double _lastCtrl;

		bool _isValid;

		// Setup pins for IO on DAC and OPAs
		void initOPAs();

		// Limit the current by writing to the V_LIM pin
		// Val in amps
		// smallerOPA selects between OPA548 and OPA549
		CtrlChannelReturn setOPACurrentLimit(double val, uint8_t V_LIM_Channel, bool smallerOPA);

		// Write to a DAC + opamp amplifiers
		// channel is 0,1,2 or 3
		// CS is the DAC's chip select pin
		// val is the voltage, (0 -> +2.5V)
		CtrlChannelReturn sendDACOut(double val, uint8_t channel, uint8_t CS);

		// Set an OPA's output voltage
		// Val is in volts
		CtrlChannelReturn setOPAVoltage(double voltage, uint8_t V_Plus_Channel);

		// SPI settings for DAC
		static SPISettings _settingsDAC;

	};

}