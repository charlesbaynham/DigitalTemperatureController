EESchema Schematic File Version 2
LIBS:TempCtrl-rescue
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:max3440e
LIBS:Custom_Components
LIBS:TempCtrl-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 8 8
Title "NPL Backplane"
Date "2016-04-15"
Rev "0.1"
Comp "NPL"
Comment1 "Charles Baynham"
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L MAX3440E U11
U 1 1 57112551
P 4050 6350
F 0 "U11" H 4050 7100 60  0000 C CNN
F 1 "MAX3440E" H 4300 5600 60  0000 C CNN
F 2 "Housings_SOIC:SOIC-8_3.9x4.9mm_Pitch1.27mm" H 4100 7200 40  0001 C CNN
F 3 "" H 4075 6250 60  0000 C CNN
	1    4050 6350
	-1   0    0    -1  
$EndComp
Text Label 4550 5750 0    60   ~ 0
RS485_+5V
Text Label 8500 5350 0    60   ~ 0
RS485_+5V
Text Label 8500 5450 0    60   ~ 0
RS485_B
Text Label 8500 5550 0    60   ~ 0
RS485_A
Text Label 4550 6250 0    60   ~ 0
RS485_A
Text Label 4550 6450 0    60   ~ 0
RS485_B
Text HLabel 3550 5950 0    60   Input ~ 0
RS485_RO
Text HLabel 3550 6150 0    60   Input ~ 0
RS485_FAULT
Text HLabel 3550 6400 0    60   Input ~ 0
RS485_DE/RE
Text HLabel 3550 6650 0    60   Input ~ 0
RS485_DI
Text Notes 5100 6250 0    60   ~ 0
This chip implements the RS485 -> Serial conversion.\nIf your use case does not require RS485 comms, delete this component
$Comp
L GND #PWR050
U 1 1 5718ABD7
P 4550 6950
F 0 "#PWR050" H 4550 6700 50  0001 C CNN
F 1 "GND" H 4550 6800 50  0000 C CNN
F 2 "" H 4550 6950 60  0000 C CNN
F 3 "" H 4550 6950 60  0000 C CNN
	1    4550 6950
	1    0    0    -1  
$EndComp
$Comp
L CONN_01X04 P?
U 1 1 57F8EEBF
P 8300 5500
F 0 "P?" H 8300 5750 50  0000 C CNN
F 1 "CONN_01X04" V 8400 5500 50  0000 C CNN
F 2 "" H 8300 5500 60  0000 C CNN
F 3 "" H 8300 5500 60  0000 C CNN
	1    8300 5500
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR?
U 1 1 57F8F059
P 8500 5650
F 0 "#PWR?" H 8500 5400 50  0001 C CNN
F 1 "GND" H 8500 5500 50  0000 C CNN
F 2 "" H 8500 5650 60  0000 C CNN
F 3 "" H 8500 5650 60  0000 C CNN
	1    8500 5650
	1    0    0    -1  
$EndComp
$EndSCHEMATC
