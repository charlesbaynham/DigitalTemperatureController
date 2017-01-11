/*
 *  pinToggler example
 *    - Charles Baynham
 *
 *    This example shows how to use pinToggler to flash 3 LEDs at different rates.
 *
 *    The onboard LED on pin 13 will flash initially slowly, then quicker and then quicker again.
 *    Pins A4 and A5 will also be toggled, but their speeds do not change.
 *
 */

#include <pinToggler.h>

void setup()
{
	/* Define the three pins to be controlled:
	  0 -> 13
	  1 -> A4
	  2 -> A5   */
	uint8_t pins[3] = { 13, A4, A5 };

	// Start the interrupt timer and set the pins to LOW
	// We use the "pinToggler<3>" instance since we're managing 3 pins
	pinToggler<3>::init(pins);

	// Start toggling
	pinToggler<3>::setFlashRate(0, SLOW);
	pinToggler<3>::setFlashRate(1, FAST);
	pinToggler<3>::setFlashRate(2, MAX);

}

void loop()
{
	delay(5000);

	// Set pin 13's speed to MEDIUM
	pinToggler<3>::setFlashRate(0, MEDIUM);

	delay(5000);

	// Set pin 13's speed to MAXIMUM
	pinToggler<3>::setFlashRate(0, MAX);

}
