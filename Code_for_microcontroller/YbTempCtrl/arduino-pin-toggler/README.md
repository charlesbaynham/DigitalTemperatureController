
arduino-pin-toggler
===================

Introduction						{#sec-Introduction}
=============

This Arduino library manages the toggling of arbitary pins at controllable rates.
For example, it can be used to flash an LED at different speeds according
to the state of your device.

It works using interrupts and is designed to be lightweight when running,
so will not interfere with existing code.

This library requires exclusive usage of TIMER1 so is incompatible with
libraries that also use this timer.

Usage						{#sec-Usage}
=============

Include the class by adding `#include <pinToggler.h>` to your sketch.

Init the class by passing it an array of pins that will be toggled. E.g.

	int pins[3] = {13, A4, A5};
	pinToggler<3>::init(pins);

The template parameter (`<3>` above) defines the total number of pins being controlled.

These pins will start `LOW`, not toggling.

To start the toggling, set their speed to `OFF`, `SLOW`, `MEDIUM`, `FAST` or `MAX`. E.g.

	pinToggler<3>::setFlashRate(0, SLOW);

N.B. The template parameter (here `<3>`) must match the one used in pinToggler::init() or this will
throw an error. Also the LED parameter in pinToggler::setFlashRate refers to the pins passed to pinToggler::init(), zero-indexed
in the order that they appeared in in the array.

The speeds refer to fractions of the max speed, defined by #FLASH_FREQ_HZ.

@warning Note that all the function calls here are static members of the
class. Although a class object is created, this happens internally. For memory
management purposes, be aware that this class allocates `3 * numPins` bytes on
the heap. Thus, to avoid memory framentation, pinToggler::init() should be
called as early in your code as possible.

@copyright Charles Baynham 2016