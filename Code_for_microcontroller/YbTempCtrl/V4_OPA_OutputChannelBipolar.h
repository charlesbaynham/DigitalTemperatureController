#pragma once

#include "V4_OPA_OutputChannel.h"

namespace YbCtrl {

	/**
	 * @brief      Implementation of a CtrlChannel for a bipolar output
	 *
	 *             Using both output channels simultaneously, allow for positive
	 *             / negative voltages from -#MAX_VOLTAGE to +#MAX_VOLTAGE
	 *
	 *             This object manages output via both OPA548 / OPA549 s. Its
	 *             most important method is setCtrl(double)
	 *
	 * @ingroup    CtrlOutput
	 */
	 class V4_OPA_OutputChannelBipolar :
		public V4_OPA_OutputChannel
	{
	public:

		// Limits the current. Value in amps
		virtual CtrlChannelReturn setCurrentLimit(double val) override;

		// Writes to the control signal. Value from -1 -> +1
		virtual void writeCtrl(double val) override;

		// Enable OPA output
		virtual void enableOutput() override;

		// Disable OPA output
		virtual void disableOutput() override;

		// Check for overheat
		// Returns error code
		virtual CtrlChannelReturn isOverheated(bool& state) override;

		V4_OPA_OutputChannelBipolar(uint8_t channelVPlus, uint8_t channelVLim, uint8_t OPA_ES, bool smallerOPA, uint8_t DAC_CS_pin,
									uint8_t channelVPlusAlt, uint8_t channelVLimAlt, uint8_t OPA_ESAlt, bool smallerOPAAlt, uint8_t DAC_CS_pinAlt) :
			_channelVPlusAlt(channelVPlusAlt), _channelVLimAlt(channelVLimAlt), _OPA_ESAlt(OPA_ESAlt), _smallerOPAAlt(smallerOPAAlt), _DAC_CS_Alt(DAC_CS_pinAlt),
			V4_OPA_OutputChannel(channelVPlus, channelVLim, OPA_ES, smallerOPA, DAC_CS_pin)
		{
			CONSOLE_LOG(F("V4_OPA_OutputChannelBipolar::Constuctor of "));
			CONSOLE_LOG_LN((int)this);

			// Output starts at minimum
			_lastCtrl = 0;

			disableOutput();
			setCurrentLimit(MAX_INITIAL_CURRENT);
		}

		V4_OPA_OutputChannelBipolar() :
			V4_OPA_OutputChannel()
		{
			CONSOLE_LOG_LN(F("V4_OPA_OutputChannelBipolar::Default constuctor"));
		}

		~V4_OPA_OutputChannelBipolar() {
			CONSOLE_LOG_LN(F("V4_OPA_OutputChannelBipolar::Destructor of "));
			CONSOLE_LOG_LN((int)this);
		}

		// Copy constuctor
		// Just use the default
		/*V4_OPA_OutputChannelBipolar(const V4_OPA_OutputChannelBipolar& rhs) :
			_channelVPlusAlt(rhs._channelVPlusAlt), _channelVLimAlt(rhs._channelVLimAlt),
			_OPA_ESAlt(rhs._OPA_ESAlt), _smallerOPAAlt(rhs._smallerOPAAlt),
			V4_OPA_OutputChannel(rhs._channelVPlus, rhs._channelVLim, rhs._OPA_ES, rhs._smallerOPA)
		{
			CONSOLE_LOG_LN(F("V4_OPA_OutputChannelBipolar::Copy constuctor"));
		}*/

	private:

		uint8_t _channelVPlusAlt, _channelVLimAlt, _OPA_ESAlt, _smallerOPAAlt, _DAC_CS_Alt;

	};

}