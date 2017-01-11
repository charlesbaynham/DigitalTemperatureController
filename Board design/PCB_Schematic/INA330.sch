EESchema Schematic File Version 2
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
Sheet 2 7
Title ""
Date "11 aug 2015"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Wire Wire Line
	6850 3500 6850 3600
Connection ~ 6850 3550
$Comp
L INA330 U9
U 1 1 55CA0D04
P 7550 3700
AR Path="/5653042F/55CA0D04" Ref="U9"  Part="1" 
AR Path="/56541853/55CA0D04" Ref="U10"  Part="1" 
F 0 "U9" H 7850 4250 60  0000 C CNN
F 1 "INA330" H 7950 4350 60  0000 C CNN
F 2 "Housings_SSOP:MSOP-10_3x3mm_Pitch0.5mm" H 7500 3750 60  0001 C CNN
F 3 "~" H 7500 3750 60  0000 C CNN
	1    7550 3700
	1    0    0    -1  
$EndComp
Wire Wire Line
	7450 2850 7450 2950
Wire Wire Line
	7350 2850 7750 2850
Wire Wire Line
	7350 2850 7350 2950
Connection ~ 7400 2850
Wire Wire Line
	7750 2850 7750 2950
Connection ~ 7450 2850
Wire Wire Line
	6850 3850 6200 3850
Wire Wire Line
	6850 4000 6300 4000
$Comp
L R R_gain1
U 1 1 55CA0E6E
P 8800 4000
AR Path="/5653042F/55CA0E6E" Ref="R_gain1"  Part="1" 
AR Path="/56541853/55CA0E6E" Ref="R_gain2"  Part="1" 
F 0 "R_gain1" V 8880 4000 40  0000 C CNN
F 1 "24K" V 8807 4001 40  0000 C CNN
F 2 "Resistors_SMD:R_0805_HandSoldering" V 8730 4000 30  0000 C CNN
F 3 "~" H 8800 4000 30  0000 C CNN
	1    8800 4000
	0    -1   -1   0   
$EndComp
$Comp
L C C_filter1
U 1 1 55CA0E8C
P 8750 3800
AR Path="/5653042F/55CA0E8C" Ref="C_filter1"  Part="1" 
AR Path="/56541853/55CA0E8C" Ref="C_filter2"  Part="1" 
F 0 "C_filter1" H 8750 3900 40  0000 L CNN
F 1 "1n" H 8756 3715 40  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 8788 3650 30  0000 C CNN
F 3 "~" H 8750 3800 60  0000 C CNN
	1    8750 3800
	0    -1   -1   0   
$EndComp
$Comp
L CP C26
U 1 1 55CA0F0E
P 7650 2700
AR Path="/5653042F/55CA0F0E" Ref="C26"  Part="1" 
AR Path="/56541853/55CA0F0E" Ref="C27"  Part="1" 
F 0 "C26" H 7700 2800 40  0000 L CNN
F 1 "100n" H 7700 2600 40  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 7750 2550 30  0000 C CNN
F 3 "~" H 7650 2700 300 0000 C CNN
	1    7650 2700
	-1   0    0    1   
$EndComp
Connection ~ 7650 2850
Wire Wire Line
	8550 3800 8550 4000
Wire Wire Line
	8250 3900 8550 3900
Connection ~ 8550 3900
Wire Wire Line
	8900 3800 9050 3800
Wire Wire Line
	9050 3800 9050 4000
Wire Wire Line
	9050 3900 9300 3900
Connection ~ 9050 3900
Wire Wire Line
	6750 3550 6850 3550
Wire Wire Line
	8250 3400 9300 3400
Wire Wire Line
	6300 4000 6300 4150
Wire Wire Line
	6200 3850 6200 4150
Text Notes 6150 4700 0    39   ~ 0
Output to 10k thermistor and set resistor
Wire Wire Line
	8600 3800 8550 3800
Wire Wire Line
	8550 4000 8650 4000
Wire Wire Line
	9050 4000 8950 4000
Text HLabel 6200 4150 0    60   Input ~ 0
I_BIAS_T
Text HLabel 6300 4150 2    60   Input ~ 0
I_BIAS_R
Text HLabel 9300 3400 2    60   Input ~ 0
V_ERROR
Text Notes 2600 1700 0    118  ~ 0
Instrumentation amplifier, optimized for temperature control applications
Connection ~ 9300 3900
Wire Wire Line
	9300 3900 9300 4100
Text HLabel 9300 4100 2    60   Input ~ 0
2.5V_OUT
Text HLabel 6750 3550 0    60   Input ~ 0
1.0V_REF
Text HLabel 9300 3900 2    60   Input ~ 0
2.5V_REF
Text HLabel 7650 2550 1    59   Input ~ 0
GND
Text HLabel 7400 2850 1    59   Input ~ 0
+5V
Text HLabel 7550 4400 3    59   Input ~ 0
GND
$EndSCHEMATC
