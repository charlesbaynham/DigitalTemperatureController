#pragma once

#include "Pins.h"
#include <Arduino.h>
#include <SPI.h>
#include <limits.h>

#include <avr/wdt.h>

#include "ErrorChannel.h"

// ADS1262 op codes
#define RESET 0x06
#define WREG 0x40
#define RREG 0x20

#define PGAD_ALM 0b00000010 // PGAD_ALM bitmask for STATUS reg

#include "ADS1262_reg.h"

#include "Microprocessor_debugging\debugging_disable.h"

namespace YbCtrl {

	/**
	 * @brief      Implementation of an ErrorChannel for the ADS1262
	 *
	 *             Reads two channels from the ADS1261 and return the
	 *             difference. The ADS1262 can read a differential of at most
	 *             2.5V in either direction, so the -1 -> +1 scale used in this
	 *             code corresponds to a -2.5 -> +2.5V scale at the ADC
	 *
	 * @ingroup    ErrorInput
	 */
	class V4_ADC_ChannelPair :
		public ErrorChannel
	{
	public:

		// Resets the ADC
		static void resetADC();

		// Recalls the last read error from this channel. Values from -1 to 1
		virtual inline double recallError() override { return _lastReading; }
		virtual ErrorChannelReturn recallError(double& output) {
			output = _lastReading;
			return ErrorChannelReturn::NO_ERROR;
		}

		// Returns the validity of this object
		virtual explicit operator bool() const override { return _isValid; }

		// Override functions for starting / reading error signal conversions
		// See ErrorChannel documentation
		bool readInProgress() override {
			return _readingChannel == this;
		}
		virtual bool globalReadInProgress() override {
			return 0 != _readingChannel;
		}
		bool readingReady() override;
		bool readingTimeout() override;
		void abortReading() override;
		ErrorChannelReturn startReading() override;
		ErrorChannelReturn getReading(double &readingOutput) override;
		
		/**
		 * @brief      Default constuctor
		 * 
		 * The object created thus will be marked as invalid
		 */
		V4_ADC_ChannelPair() :
			_isValid(false), _highRes(false)
		{
			CONSOLE_LOG_LN(F("V4_ADC_ChannelPair::Default constuctor"));

			setupADCpins();
			
			if (!_ADC_registers_loaded) {
				getRegisters();
			}
		}

		/**
		 * @brief      Constuctor
		 *
		 *             The error signal measured by readError(double&) will be
		 *             channel 1 - channel 2
		 *
		 * @param[in]  channel1  ADC channel 1
		 * @param[in]  channel2  ADC channel 2
		 * @param[in]  highRes   Should this channel be measured quickly or carefully?
		 */
		V4_ADC_ChannelPair(uint8_t channel1, uint8_t channel2, bool highRes = true) :
			_channel1(channel1), _channel2(channel2), _isValid(true), _highRes(highRes),
			_timeout(0),
			_numLowReadings(0), _PGA_gain(0)
		{
			CONSOLE_LOG(F("V4_ADC_ChannelPair::Constuctor at "));
			CONSOLE_LOG_LN((int)this);

			setupADCpins();
			
			if (!_ADC_registers_loaded) {
				getRegisters();
			}
		}

		~V4_ADC_ChannelPair() override {
			CONSOLE_LOG(F("~V4_ADC_ChannelPair::at "));
			CONSOLE_LOG_LN((int)this);
		}

		// Copy constuctor
		// V4_ADC_ChannelPair(const V4_ADC_ChannelPair& rhs) :
		// 	_channel1(rhs._channel1), _channel2(rhs._channel2), _lastReading(rhs._lastReading), _highRes(rhs.highRes)
		// {
		// 	CONSOLE_LOG_LN(F("V4_ADC_ChannelPair::Copy constuctor"));
		// }

#ifdef DEBUG
	public:
#else
	protected:
#endif

		uint8_t _channel1, _channel2;

		double _lastReading;

		uint8_t _numLowReadings, _PGA_gain;

		bool _isValid;
		const bool _highRes;
		
		static V4_ADC_ChannelPair * _readingChannel;
		unsigned long _timeout;

		static constexpr uint8_t __maxLowReadings = 3;		/*!< The max number of readings to take getting less than __lowGainThreshold before boosting the gain */
		static constexpr double __lowGainThreshold = 0.4; 	/*!< The threshold below which a reading is considered to have required a higher gain */
		static constexpr unsigned long __highResTimeout = 8000;
		static constexpr unsigned long __lowResTimeout = 1000;

		static bool _ADC_registers_loaded;

		/************************************************
		  Internal funcitons to interface with the ADC:
		************************************************/

		// Setup pins for communication
		static void setupADCpins();

		// Read ADC, comparing the two given channels
		// Values from LONG_MIN to LONG_MAX
		// static ErrorChannelReturn readChannel(uint8_t channel1, uint8_t channel2, long& output, uint8_t& status, bool highRes = false);
		// static ErrorChannelReturn readChannel(uint8_t channel1, uint8_t channel2, long& output) {
		// 	uint8_t throwAway;
		// 	return readChannel(channel1, channel2, output, throwAway);
		// }

		// Write data to the ADC resister reg and check. Return TRUE for success
		static ErrorChannelReturn writeAndCheckADCReg(uint8_t reg, uint8_t data);

		// Write data to the ADC resister reg. 
		static void writeADCReg(uint8_t reg, uint8_t data);

		// Read data from the ADC register reg
		static uint8_t readADCReg(uint8_t reg);

		// SPI settings for ADC
		// Static, since these are the same for all ChannelPair objects
		static SPISettings _settingsADC;

		// Load registers from the ADC into the registers_data array
		static void getRegisters();

		// Load registers the registers_data array into the ADC
		static ErrorChannelReturn writeRegisters();
	};

}
