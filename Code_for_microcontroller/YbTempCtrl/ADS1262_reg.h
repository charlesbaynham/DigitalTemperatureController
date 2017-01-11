#pragma once

/*
	TYPESAFE REGISTER ACCESS

	This file allows for safe access and modification of a set of registers, typically
	used to control a device. Use of this code has zero overhead compared with manually
	typing out bit-by-bit access, but is safer and easier. 

	This particular customisation has been adapted from the GP22 register settings to control
	instead the ADS1262. All comments referring to the GP22 should be understood to mean the ADS1262. 

	Usage:

		Initialisation - 
			Include this file and define `uint32_t GP22::registers_data[7]` in your code.
			e.g.
				#include "GP22_reg.h"

				uint32_t GP22::registers_data[7] = {0};

		regWrite - Overwrites an entire register.
		e.g.
			regWrite(GP22::REG0, 0x12345678);

		regRead - Reads from an entire register
		e.g.
			uint32_t reg2Contents = regRead(GP22::REG2);

		bitmaskWrite - Overwrites the specified bits in the given register
		e.g.
			bitmaskWrite(GP22::REG0, GP22::REG0_ID0, 0xab);

		Warning: it is possible to give the wrong mask to this function without
		triggering a compiler error. e.g. 
			bitmaskWrite(GP22::REG2, GP22::REG0_ID0, 0xab);
		would compile fine, even though mask REG0_ID0 does not refer to register REG0. 

		bitmaskRead - Reads the specified bits from the given register
		e.g.
			uint8_t IDBits = bitmaskRead(GP22::REG0, GP22::REG0_ID0);
			// Gives IDBits = 0xab after previous call

	Customisation / copying:
		1. Copy this file and rename
		2. Change the namespace to an appropriate value
		3. Define the register size using the `T_register` typedef
		4a. Update the `registers` enum to hold the correct number of register labels
    4b. Update the `extern T_register registers_data[<NUM_REGISTERS>];` definition to match the 
      number of registers defined in 4a.
		5. Update the bitset enums: one for each register, defining the location and names
			of available settings.
		6. Include this file in your code and initialise the data before calling any of the functions.
			e.g.
				T_register MY_NAMESPACE::registers_data[<NUM_REGISTERS>];
		7. Call the functions as described above in order to read / write to the registers. 

*/

// Change the namespace title for other applications
namespace ADS1262 {

	// This defines the size of a register
	// E.g. for a 32 bit register, use 
	// 		typedef uint32_t T_register;
	typedef uint8_t T_register;

	// This is the location of the data. It is declared "extern" here, meaning that it must be defined
	// and initialised elsewhere. 
	//
	// On initialisation, ensure that the size matches the size of `registers` or there will
	// be buffer overflows
	extern T_register registers_data[7];

	// The registers
	// The number of labels here must match the size of `registers_data` and must be consecutive
	enum registers : size_t
	{
		REG_ID  		= 0x00,
		REG_POWER  		= 0x01,
		REG_INTERFACE  	= 0x02,
		REG_MODE0  		= 0x03,
		REG_MODE1  		= 0x04,
		REG_MODE2  		= 0x05,
		REG_INPMUX  	= 0x06
		// The other 14 registers are not used and therefore skipped
	};

	// A macro that defines encodes values for a bitset
	// This encodes the start and stop positions of a bitset for a given register into
	// the first two and last two bytes of a 32 bit int respectively
	// See below for usage
	#define REG_BIT_DEFN(start, end) ((uint32_t(start)<<16)|(end-start+1))

	// The settings
	//
	// Use the `REG_BIT_DEFN` macro to define the start and stop positions of all the settings
	// available on your device
	//
	// It is important that the names of these enum values contain the register to which they
	// relate. This is the only protection against erroneously matching register x's bitmask with
	// register y, as this will not throw a compile error. 
	//
	// Once defined here, the bitset functions can be called with these values. E.g.
	// 		bitmaskWrite(GP22:REG0, GP22:REG0_DIV_FIRE, 0b1011);
	enum bitmaskDefs : uint32_t {
	    REG_ID_DEVID            = REG_BIT_DEFN(5, 7),
	    REG_ID_REVID          	= REG_BIT_DEFN(0, 4),
	
	    REG_POWER_RESET         = REG_BIT_DEFN(4, 4),
	    REG_POWER_VBIAS         = REG_BIT_DEFN(1, 1),
	    REG_POWER_INTREF        = REG_BIT_DEFN(0, 0),
	
	    REG_INTERFACE_TIMEOUT   = REG_BIT_DEFN(3, 3),
	    REG_INTERFACE_STATUS    = REG_BIT_DEFN(2, 2),
	    REG_INTERFACE_CRC       = REG_BIT_DEFN(0, 1),
	
	    REG_MODE0_REFREV        = REG_BIT_DEFN(7, 7),
	    REG_MODE0_RUNMODE       = REG_BIT_DEFN(6, 6),
	    REG_MODE0_CHOP       	= REG_BIT_DEFN(4, 5),
	    REG_MODE0_DELAY         = REG_BIT_DEFN(0, 3),
	
	    REG_MODE1_FILTER        = REG_BIT_DEFN(5, 7),
	    REG_MODE1_SBADC       	= REG_BIT_DEFN(4, 4),
	    REG_MODE1_SBPOL       	= REG_BIT_DEFN(3, 3),
	    REG_MODE1_SBMAG         = REG_BIT_DEFN(0, 2),

	    REG_MODE2_BYPASS        = REG_BIT_DEFN(7, 7),
	    REG_MODE2_GAIN          = REG_BIT_DEFN(4, 6),
	    REG_MODE2_DR          	= REG_BIT_DEFN(3, 0),

	 	REG_INPMUX_MUXP         = REG_BIT_DEFN(4, 7),
	    REG_INPMUX_MUXN         = REG_BIT_DEFN(0, 3)
	};

	inline volatile T_register* regAddress(registers reg)
	{
		return reinterpret_cast<volatile T_register*>( 
			(unsigned int)registers_data
			+ (size_t)reg * sizeof(T_register) );
	}

	inline T_register regRead(registers reg)
	{
		return *regAddress(reg);
	}
	 
	inline void regWrite(registers reg, T_register value)
	{
		*regAddress(reg) = value;
	}

	// Functions to read the requested bits from a register, decoding the information
	// stored by the above macro
	inline T_register bitmaskRead(registers reg, uint32_t bits)
	{
		// Get the current value of the register
		T_register regval = *regAddress(reg);

		// Extract the width of the bits required, stored in the last two bytes
		// of `bits`
		const uint32_t width  = bits & 0xff;

		// Extract the position of the first bit requested, stored in the first
		// two bytes of `bits`
		const uint32_t bitno  = bits >> 16;

		// Shift so that the first interesting bit is in position `1`
		regval >>= bitno;

		// Mask out only the bits below position `width`
		regval  &= (T_register(1<<width)-1);

		// Return
		return regval;
	}
	inline void bitmaskWrite(registers reg, uint32_t bits, T_register value)
	{
		// Get the current value of the register
		T_register regval = *regAddress(reg);

		// Extract the width of the bits required, stored in the last two bytes
		// of `bits`
		const uint32_t width  = bits & 0xff;

		// Extract the position of the first bit requested, stored in the first
		// two bytes of `bits`
		const uint32_t bitno  = bits >> 16;

		// Set all the bits selected by `bits` to zero
		regval &= ~( T_register((1<<width)-1) << bitno);

		// Set all the bits selected by `bits` to their required new value
		regval |=  value << bitno;

		// Store
		*regAddress(reg) = regval;
	}

	// Clean up after ourselves
	#undef REG_BIT_DEFN	
}
