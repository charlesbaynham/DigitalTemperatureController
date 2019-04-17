Digital Temperature Controller
==============================

_Copyright Charles Baynham 2018_

This repository holds a design for a digital temperature controller. The controller is designed for taking high precision
temperature measurements via a thermistor and applying low bandwidth PID feedback in response. 

This README file gives a brief overview of the device, but a much more detailed description is available in the manual, located in 
`./Manual`. 

`./Board design` contains schematics and PCB layouts for the controller board. This was produced in the open-source software KiCad. 

`./Code_for_microcontroller` contains the C++ code that must run on the on-board microcontroller. 

`./Code for Labview` contains a sample LabView application for controlling the device. See warning below...

Output
------

Output is by either OPA458 high power opamps. Output can be up to 1A continuous, at voltages up to ±24V depending on the power supply.

Input
-----

Error signal acquisition is done by taking a balanced measurement between a thermistor
and a set resistor. The board produces an excitation voltage and measures
the difference in current between the thermistor and resistor to produce a temperature
measurement. For less demanding applications the set resistor can be on-board. For the
highest stability, the precision resistor should be placed next to its paired thermistor.
This measurement is performed by an INA330 chip, of which there are two present on
the board. The entire analogue section is electrically isolated from the rest of the board to
prevent noise on the electrical ground affecting the measurement.

Control
-------

The board can be controlled using a (virtual) serial connection over either USB or RS-485. Both of these interfaces 
are galvanically isolated to prevent electrical interference. The on-board microcontroller permits headless operation of the
device, and commands may be issued using a LabView interface.

Code
----

The code running on the device is written in c++ and has been well tested. The LabView control interface, designed to be run on a monitoring computer, is less well-tested and should be used at your own peril! For integration into an experimental system, control the device by issuing serial commands detail in the supplied manual. 

Manual
------

You should go read it now. It's in `./Manual`. 

Initial setup
-------------

The microcontroller needs to be flashed using Atmel Studio and an ISP programmer. You should either:

1. Set the fuses as described in `fuse_settings.txt`
2. Flash the bootloader saved as `optiboot_atmega328_custom_NANO.hex`
3. Use the Arduino IDE to compile and upload the latest version of the code

OR

1. Flash the complete production `.elf` file stored in `.\Code_for_microcontroller` using Atmel Studio, making sure to select "Flash", "Fuses" and "Verify Device ID" in Atmel Studio. This will give you whatever version of the code was stored in this `.elf` file, you can then update it using the Arduino IDE if you want to make changes. 