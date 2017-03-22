/** @file
 *
 * Hardware limits and pins
 *
 * This file contains pin numbers and hard-coded limits specific to the 2
 * channel board
 *
 * OPA_GAIN is the gain of the OPA compared to the DAC's output voltage of 2.5V.
 * This is set by resistors on the board, calculated according to the values of
 * OPA_R1 and OPA_R2. It is currently assumed that both OPAs have the same gain.
 *
 * @todo Add option for user to configure different hardware gains on the two
 * OPAs.
 */

#pragma once

#include <Arduino.h>

// ADC pins
const uint8_t ADC_CS = A3; 					/*!< Chip select pin of the ADC on the ATMega */
const uint8_t ADC_START = 2;				/*!< ~START pin of the ADC on the ATMega */
const uint8_t ADC_DRDY = A2;				/*!< DRDY pin of the ADC on the ATMega */

// ADC channels
const uint8_t ADC_THERM1    = 9;    /*!< ADC channel for thermistor 1 input */
const bool    ADC_THERM1_HIGHRES    = false;    /*!< Should thermistor 1 be a high-res measurement? */
const uint8_t ADC_THERM2		= 8;		/*!< ADC channel for thermistor 2 input */
const bool    ADC_THERM2_HIGHRES    = false;    /*!< Should thermistor 2 be a high-res measurement? */
const uint8_t ADC_REF			= 10;		/*!< ADC channel for common thermistor reference input */
const uint8_t ADC_VOLT1_P		= 6;		/*!< ADC channel for arbitary voltage positive input 1 */
const uint8_t ADC_VOLT1_N		= 7;		/*!< ADC channel for arbitary voltage negative input 1 */
const bool    ADC_VOLT1_HIGHRES    = false;    /*!< Should voltage 1 be a high-res measurement? */
const uint8_t ADC_VOLT2_P		= 4;		/*!< ADC channel for arbitary voltage positive input 2 */
const uint8_t ADC_VOLT2_N		= 5;		/*!< ADC channel for arbitary voltage negative input 2 */
const bool    ADC_VOLT2_HIGHRES    = false;    /*!< Should voltage 2 be a high-res measurement? */
const uint8_t ADC_TMP			= 11;		/*!< ADC channel for chip temperature measurement */
const uint8_t ADC_POW			= 12;		/*!< ADC channel for chip power level measurement */

// DAC pins
const uint8_t DAC_CS 			= 9;		/*!< Chip select pin of the DAC on the ATMega */
const uint8_t OPA_ES1 			= A0;		/*!< E/S pin for first OPA */
const uint8_t OPA_ES2 			= A1;		/*!< E/S pin for second OPA */
const bool OPA_1_IS_548			= false;	/*!< Is OPA 1 an OPA548 (instead of an OPA549)? */
const bool OPA_2_IS_548			= false;	/*!< Is OPA 2 an OPA548 (instead of an OPA549)? */

// DAC channels
const uint8_t VPLUS_CHAN_1 	= 1;			/*!< DAC channel for first OPA's V+ input */
const uint8_t VPLUS_CHAN_2 	= 0;			/*!< DAC channel for second OPA's V+ input */
const uint8_t VLIM_CHAN_1 	= 3;			/*!< DAC channel for first OPA's Vlim input */
const uint8_t VLIM_CHAN_2 	= 2;			/*!< DAC channel for second OPA's Vlim input */

// OPA settings
extern double MAX_VOLTAGE;					/*!< Maximum voltage for the OPAs to output. Calculated in setup() */
const double MAX_INITIAL_CURRENT = 2.0;		/*!< Maximum current that the OPAs will be limited to on startup */
const double OPA_R1	= 40.2;					/*!< OPA gain resistor 1 */
const double OPA_R2	= 200.2;					/*!< OPA gain resistor 2 */
const double OPA_GAIN = 
			(OPA_R1 + OPA_R2) / OPA_R1;		/*!< DAC -> OPA gain, set by on-board resistors. */

// LED output pins
const uint8_t LED_1 = A5; 					/*!< ATMega pin for LED 1, Arduino labelling */
const uint8_t LED_2 = A4;					/*!< ATMega pin for LED 2, Arduino labelling */

// Other Microprocessor pins
const uint8_t DISABLE_SERIAL_CTRL = 0xFF; 	/*!< ATMega pin for disabling serial control: 0 or 5V (set to 0xFF if not present) */
const uint8_t DIVIDED_SUPPLY_VOLTAGE = 0xFF;/*!< ATMega pin for reading supply voltage: analog voltage (set to 0xFF if not present) */
const double FALLBACK_SUPPLY_VOLTAGE = 15;	/*!< Assumed supply voltage if DIVIDED_SUPPLY_VOLTAGE pin is not present */
const double DIVIDED_SUPPLY_FACTOR = 0;		/*!< Factor by which the supply voltage has been divided  */