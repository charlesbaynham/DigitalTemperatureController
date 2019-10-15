EESchema Schematic File Version 4
LIBS:TempCtrl-cache
EELAYER 29 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 4 8
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L MCU_Microchip_ATmega:ATmega328P-AU IC1
U 1 1 56BA55CF
P 1900 3900
AR Path="/56BA55CF" Ref="IC1"  Part="1" 
AR Path="/57222EA2/56BA55CF" Ref="IC1"  Part="1" 
F 0 "IC1" H 1100 4150 40  0000 L BNN
F 1 "ATMEGA328P-A" H 2300 2500 40  0000 L BNN
F 2 "Housings_QFP:LQFP-32_7x7mm_Pitch0.8mm" H 1900 3900 30  0000 C CIN
F 3 "" H 1900 3900 60  0000 C CNN
F 4 "-" H 0   0   50  0001 C CNN "MFR"
F 5 "-" H 0   0   50  0001 C CNN "MPN"
F 6 "-" H 0   0   50  0001 C CNN "SPR"
F 7 "-" H 0   0   50  0001 C CNN "SPN"
F 8 "-" H 0   0   50  0001 C CNN "SPURL"
	1    1900 3900
	1    0    0    -1  
$EndComp
$Comp
L Device:CP_Small C31
U 1 1 56BA55DD
P 700 2800
F 0 "C31" H 710 2870 50  0000 L CNN
F 1 "100n" H 710 2720 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 700 2800 60  0001 C CNN
F 3 "" H 700 2800 60  0000 C CNN
F 4 "-" H 0   0   50  0001 C CNN "MFR"
F 5 "-" H 0   0   50  0001 C CNN "MPN"
F 6 "-" H 0   0   50  0001 C CNN "SPR"
F 7 "-" H 0   0   50  0001 C CNN "SPN"
F 8 "-" H 0   0   50  0001 C CNN "SPURL"
	1    700  2800
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R20
U 1 1 56BA55EB
P 3200 4100
F 0 "R20" H 3230 4120 50  0000 L CNN
F 1 "1k" H 3230 4060 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 3200 4100 60  0001 C CNN
F 3 "" H 3200 4100 60  0000 C CNN
F 4 "-" H 0   0   50  0001 C CNN "MFR"
F 5 "-" H 0   0   50  0001 C CNN "MPN"
F 6 "-" H 0   0   50  0001 C CNN "SPR"
F 7 "-" H 0   0   50  0001 C CNN "SPN"
F 8 "-" H 0   0   50  0001 C CNN "SPURL"
	1    3200 4100
	1    0    0    -1  
$EndComp
$Comp
L Device:CP_Small C29
U 1 1 56BA55F2
P 1650 2100
F 0 "C29" H 1660 2170 50  0000 L CNN
F 1 "100n" H 1660 2020 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 1650 2100 60  0001 C CNN
F 3 "" H 1650 2100 60  0000 C CNN
F 4 "-" H 0   0   50  0001 C CNN "MFR"
F 5 "-" H 0   0   50  0001 C CNN "MPN"
F 6 "-" H 0   0   50  0001 C CNN "SPR"
F 7 "-" H 0   0   50  0001 C CNN "SPN"
F 8 "-" H 0   0   50  0001 C CNN "SPURL"
	1    1650 2100
	1    0    0    -1  
$EndComp
$Comp
L Device:CP_Small C28
U 1 1 56BA55F9
P 1400 2100
F 0 "C28" H 1410 2170 50  0000 L CNN
F 1 "1u" H 1410 2020 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 1400 2100 60  0001 C CNN
F 3 "" H 1400 2100 60  0000 C CNN
F 4 "-" H 0   0   50  0001 C CNN "MFR"
F 5 "-" H 0   0   50  0001 C CNN "MPN"
F 6 "-" H 0   0   50  0001 C CNN "SPR"
F 7 "-" H 0   0   50  0001 C CNN "SPN"
F 8 "-" H 0   0   50  0001 C CNN "SPURL"
	1    1400 2100
	1    0    0    -1  
$EndComp
$Comp
L Device:R_Small R_Ser1
U 1 1 56BA5600
P 3000 4400
F 0 "R_Ser1" H 3030 4420 50  0000 L CNN
F 1 "1k" H 3030 4360 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 3000 4400 60  0001 C CNN
F 3 "" H 3000 4400 60  0000 C CNN
F 4 "-" H 0   0   50  0001 C CNN "MFR"
F 5 "-" H 0   0   50  0001 C CNN "MPN"
F 6 "-" H 0   0   50  0001 C CNN "SPR"
F 7 "-" H 0   0   50  0001 C CNN "SPN"
F 8 "-" H 0   0   50  0001 C CNN "SPURL"
	1    3000 4400
	0    -1   -1   0   
$EndComp
$Comp
L Device:R_Small R_Ser2
U 1 1 56BA5607
P 3000 4500
F 0 "R_Ser2" H 3030 4520 50  0000 L CNN
F 1 "1k" H 3030 4460 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 3000 4500 60  0001 C CNN
F 3 "" H 3000 4500 60  0000 C CNN
F 4 "-" H 0   0   50  0001 C CNN "MFR"
F 5 "-" H 0   0   50  0001 C CNN "MPN"
F 6 "-" H 0   0   50  0001 C CNN "SPR"
F 7 "-" H 0   0   50  0001 C CNN "SPN"
F 8 "-" H 0   0   50  0001 C CNN "SPURL"
	1    3000 4500
	0    1    1    0   
$EndComp
$Comp
L Device:LED D5
U 1 1 56BA560E
P 4200 5650
F 0 "D5" H 4200 5750 50  0000 C CNN
F 1 "SCLK" H 4200 5550 50  0000 C CNN
F 2 "LEDs:LED_0805" H 4200 5650 60  0001 C CNN
F 3 "" H 4200 5650 60  0000 C CNN
F 4 "-" H 0   0   50  0001 C CNN "MFR"
F 5 "-" H 0   0   50  0001 C CNN "MPN"
F 6 "-" H 0   0   50  0001 C CNN "SPR"
F 7 "-" H 0   0   50  0001 C CNN "SPN"
F 8 "-" H 0   0   50  0001 C CNN "SPURL"
	1    4200 5650
	0    -1   -1   0   
$EndComp
$Comp
L Device:R_Small R21
U 1 1 56BA5615
P 4200 5400
F 0 "R21" H 4230 5420 50  0000 L CNN
F 1 "100R" H 4230 5360 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 4200 5400 60  0001 C CNN
F 3 "" H 4200 5400 60  0000 C CNN
F 4 "-" H 0   0   50  0001 C CNN "MFR"
F 5 "-" H 0   0   50  0001 C CNN "MPN"
F 6 "-" H 0   0   50  0001 C CNN "SPR"
F 7 "-" H 0   0   50  0001 C CNN "SPN"
F 8 "-" H 0   0   50  0001 C CNN "SPURL"
	1    4200 5400
	1    0    0    -1  
$EndComp
$Comp
L Custom_Components:FT232RL U12
U 1 1 56BA5634
P 8350 3350
F 0 "U12" H 7600 4550 60  0000 C CNN
F 1 "FT232RL" H 8350 3350 60  0000 C CNN
F 2 "Housings_SSOP:SSOP-28_5.3x10.2mm_Pitch0.65mm" H 8350 3350 60  0001 C CNN
F 3 "" H 8350 3350 60  0000 C CNN
F 4 "-" H 0   0   50  0001 C CNN "MFR"
F 5 "-" H 0   0   50  0001 C CNN "MPN"
F 6 "-" H 0   0   50  0001 C CNN "SPR"
F 7 "-" H 0   0   50  0001 C CNN "SPN"
F 8 "-" H 0   0   50  0001 C CNN "SPURL"
	1    8350 3350
	-1   0    0    -1  
$EndComp
$Comp
L Device:C C33
U 1 1 56BA563F
P 3900 3850
F 0 "C33" H 3925 3950 50  0000 L CNN
F 1 "100n" H 3925 3750 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 3938 3700 30  0001 C CNN
F 3 "" H 3900 3850 60  0000 C CNN
F 4 "-" H 0   0   50  0001 C CNN "MFR"
F 5 "-" H 0   0   50  0001 C CNN "MPN"
F 6 "-" H 0   0   50  0001 C CNN "SPR"
F 7 "-" H 0   0   50  0001 C CNN "SPN"
F 8 "-" H 0   0   50  0001 C CNN "SPURL"
	1    3900 3850
	0    -1   -1   0   
$EndComp
$Comp
L Device:LED D3
U 1 1 56BA5647
P 6550 5250
F 0 "D3" H 6550 5350 50  0000 C CNN
F 1 "TX" H 6550 5150 50  0000 C CNN
F 2 "LEDs:LED_0805" H 6550 5250 60  0001 C CNN
F 3 "" H 6550 5250 60  0000 C CNN
F 4 "-" H 0   0   50  0001 C CNN "MFR"
F 5 "-" H 0   0   50  0001 C CNN "MPN"
F 6 "-" H 0   0   50  0001 C CNN "SPR"
F 7 "-" H 0   0   50  0001 C CNN "SPN"
F 8 "-" H 0   0   50  0001 C CNN "SPURL"
	1    6550 5250
	0    1    1    0   
$EndComp
$Comp
L Device:R_Small R22
U 1 1 56BA564E
P 6550 5500
F 0 "R22" H 6580 5520 50  0000 L CNN
F 1 "100R" H 6580 5460 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 6550 5500 60  0001 C CNN
F 3 "" H 6550 5500 60  0000 C CNN
F 4 "-" H 0   0   50  0001 C CNN "MFR"
F 5 "-" H 0   0   50  0001 C CNN "MPN"
F 6 "-" H 0   0   50  0001 C CNN "SPR"
F 7 "-" H 0   0   50  0001 C CNN "SPN"
F 8 "-" H 0   0   50  0001 C CNN "SPURL"
	1    6550 5500
	1    0    0    -1  
$EndComp
$Comp
L Device:LED D4
U 1 1 56BA5655
P 6850 5250
F 0 "D4" H 6850 5350 50  0000 C CNN
F 1 "RX" H 6850 5150 50  0000 C CNN
F 2 "LEDs:LED_0805" H 6850 5250 60  0001 C CNN
F 3 "" H 6850 5250 60  0000 C CNN
F 4 "-" H 0   0   50  0001 C CNN "MFR"
F 5 "-" H 0   0   50  0001 C CNN "MPN"
F 6 "-" H 0   0   50  0001 C CNN "SPR"
F 7 "-" H 0   0   50  0001 C CNN "SPN"
F 8 "-" H 0   0   50  0001 C CNN "SPURL"
	1    6850 5250
	0    1    1    0   
$EndComp
$Comp
L Device:R_Small R23
U 1 1 56BA565C
P 6850 5500
F 0 "R23" H 6880 5520 50  0000 L CNN
F 1 "100R" H 6880 5460 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 6850 5500 60  0001 C CNN
F 3 "" H 6850 5500 60  0000 C CNN
F 4 "-" H 0   0   50  0001 C CNN "MFR"
F 5 "-" H 0   0   50  0001 C CNN "MPN"
F 6 "-" H 0   0   50  0001 C CNN "SPR"
F 7 "-" H 0   0   50  0001 C CNN "SPN"
F 8 "-" H 0   0   50  0001 C CNN "SPURL"
	1    6850 5500
	1    0    0    -1  
$EndComp
$Comp
L Device:CP_Small C34
U 1 1 56BA5663
P 9450 4350
F 0 "C34" H 9460 4420 50  0000 L CNN
F 1 "100n" H 9460 4270 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 9450 4350 60  0001 C CNN
F 3 "" H 9450 4350 60  0000 C CNN
F 4 "-" H 0   0   50  0001 C CNN "MFR"
F 5 "-" H 0   0   50  0001 C CNN "MPN"
F 6 "-" H 0   0   50  0001 C CNN "SPR"
F 7 "-" H 0   0   50  0001 C CNN "SPN"
F 8 "-" H 0   0   50  0001 C CNN "SPURL"
	1    9450 4350
	1    0    0    -1  
$EndComp
$Comp
L Connector:USB_OTG P4
U 1 1 56BA5671
P 1350 6950
F 0 "P4" H 1400 7400 50  0000 C CNN
F 1 "USB_OTG" H 1350 7300 50  0000 C CNN
F 2 "Custom:MICRO-B_USB_TOP_MOUNT" V 1300 6850 60  0001 C CNN
F 3 "" V 1300 6850 60  0000 C CNN
F 4 "-" H 0   0   50  0001 C CNN "MFR"
F 5 "-" H 0   0   50  0001 C CNN "MPN"
F 6 "-" H 0   0   50  0001 C CNN "SPR"
F 7 "-" H 0   0   50  0001 C CNN "SPN"
F 8 "-" H 0   0   50  0001 C CNN "SPURL"
	1    1350 6950
	-1   0    0    1   
$EndComp
$Comp
L Connector_Generic:Conn_02x03_Odd_Even P5
U 1 1 56BA568C
P 2700 6750
F 0 "P5" H 2700 6950 50  0000 C CNN
F 1 "ICSP" H 2700 6550 50  0000 C CNN
F 2 "Pin_Headers:Pin_Header_Straight_2x03" H 2700 5550 60  0001 C CNN
F 3 "" H 2700 5550 60  0000 C CNN
F 4 "-" H 0   0   50  0001 C CNN "MFR"
F 5 "-" H 0   0   50  0001 C CNN "MPN"
F 6 "-" H 0   0   50  0001 C CNN "SPR"
F 7 "-" H 0   0   50  0001 C CNN "SPN"
F 8 "-" H 0   0   50  0001 C CNN "SPURL"
	1    2700 6750
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR056
U 1 1 56BA5695
P 3000 7000
F 0 "#PWR056" H 3000 6750 50  0001 C CNN
F 1 "GND" H 3000 6850 50  0000 C CNN
F 2 "" H 3000 7000 60  0000 C CNN
F 3 "" H 3000 7000 60  0000 C CNN
	1    3000 7000
	1    0    0    -1  
$EndComp
$Comp
L Custom_Components:ISO7242 U11
U 1 1 56C0BAFB
P 5100 2200
F 0 "U11" H 5200 2850 60  0000 L CNN
F 1 "ISO7242" H 5300 1350 60  0000 L CNN
F 2 "Housings_SOIC:SOIC-16_7.5x10.3mm_Pitch1.27mm" H 5100 2200 60  0001 C CNN
F 3 "" H 5100 2200 60  0000 C CNN
F 4 "-" H 0   0   50  0001 C CNN "MFR"
F 5 "-" H 0   0   50  0001 C CNN "MPN"
F 6 "-" H 0   0   50  0001 C CNN "SPR"
F 7 "-" H 0   0   50  0001 C CNN "SPN"
F 8 "-" H 0   0   50  0001 C CNN "SPURL"
	1    5100 2200
	-1   0    0    -1  
$EndComp
$Comp
L Device:C_Small C30
U 1 1 572B777C
P 3200 3050
F 0 "C30" H 3210 3120 50  0000 L CNN
F 1 "8p" H 3210 2970 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 3200 3050 60  0001 C CNN
F 3 "" H 3200 3050 60  0000 C CNN
F 4 "-" H 0   0   50  0001 C CNN "MFR"
F 5 "-" H 0   0   50  0001 C CNN "MPN"
F 6 "-" H 0   0   50  0001 C CNN "SPR"
F 7 "-" H 0   0   50  0001 C CNN "SPN"
F 8 "-" H 0   0   50  0001 C CNN "SPURL"
	1    3200 3050
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C32
U 1 1 572B7860
P 3150 3650
F 0 "C32" H 3160 3720 50  0000 L CNN
F 1 "8p" H 3160 3570 50  0000 L CNN
F 2 "Capacitors_SMD:C_0805_HandSoldering" H 3150 3650 60  0001 C CNN
F 3 "" H 3150 3650 60  0000 C CNN
F 4 "-" H 0   0   50  0001 C CNN "MFR"
F 5 "-" H 0   0   50  0001 C CNN "MPN"
F 6 "-" H 0   0   50  0001 C CNN "SPR"
F 7 "-" H 0   0   50  0001 C CNN "SPN"
F 8 "-" H 0   0   50  0001 C CNN "SPURL"
	1    3150 3650
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR057
U 1 1 572B78C7
P 3150 3750
F 0 "#PWR057" H 3150 3500 50  0001 C CNN
F 1 "GND" H 3150 3600 50  0000 C CNN
F 2 "" H 3150 3750 60  0000 C CNN
F 3 "" H 3150 3750 60  0000 C CNN
	1    3150 3750
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR058
U 1 1 572B7917
P 3200 2950
F 0 "#PWR058" H 3200 2700 50  0001 C CNN
F 1 "GND" H 3200 2800 50  0000 C CNN
F 2 "" H 3200 2950 60  0000 C CNN
F 3 "" H 3200 2950 60  0000 C CNN
	1    3200 2950
	-1   0    0    1   
$EndComp
$Comp
L Device:Crystal_GND3 X1
U 1 1 572B9603
P 3350 3350
F 0 "X1" H 3250 3500 50  0000 C CNN
F 1 "TSX-3225  X1E0000210013" V 3450 3450 50  0000 L CNN
F 2 "Crystals:crystal_FA238-TSX3225" H 3350 3350 60  0001 C CNN
F 3 "" H 3350 3350 60  0000 C CNN
F 4 "-" H 0   0   50  0001 C CNN "MFR"
F 5 "-" H 0   0   50  0001 C CNN "MPN"
F 6 "-" H 0   0   50  0001 C CNN "SPR"
F 7 "-" H 0   0   50  0001 C CNN "SPN"
F 8 "-" H 0   0   50  0001 C CNN "SPURL"
	1    3350 3350
	0    1    1    0   
$EndComp
$Comp
L power:PWR_FLAG #FLG060
U 1 1 57AB052A
P 1350 6400
F 0 "#FLG060" H 1350 6495 50  0001 C CNN
F 1 "PWR_FLAG" H 1350 6580 50  0000 C CNN
F 2 "" H 1350 6400 60  0000 C CNN
F 3 "" H 1350 6400 60  0000 C CNN
	1    1350 6400
	1    0    0    -1  
$EndComp
$Comp
L power:PWR_FLAG #FLG061
U 1 1 5B99C40B
P 900 7250
F 0 "#FLG061" H 900 7345 50  0001 C CNN
F 1 "PWR_FLAG" H 900 7430 50  0000 C CNN
F 2 "" H 900 7250 60  0000 C CNN
F 3 "" H 900 7250 60  0000 C CNN
	1    900  7250
	-1   0    0    1   
$EndComp
Text Label 2500 4400 0    60   ~ 0
D0/RX
Text Label 2500 4500 0    60   ~ 0
D1/TX
Text Label 2500 4600 0    60   ~ 0
D2
Text Label 2500 4700 0    60   ~ 0
D3
Text Label 2500 4800 0    60   ~ 0
D4
Text Label 2500 4900 0    60   ~ 0
D5
Text Label 2500 5000 0    60   ~ 0
D6
Text Label 2500 5100 0    60   ~ 0
D7
Text Label 2500 3600 0    60   ~ 0
A0
Text Label 2500 3700 0    60   ~ 0
A1
Text Label 2500 3800 0    60   ~ 0
A2
Text Label 2500 3900 0    60   ~ 0
A3
Text Label 2500 4000 0    60   ~ 0
A4
Text Label 2500 4100 0    60   ~ 0
A5
Text Label 1300 2900 2    60   ~ 0
A6
Text Label 1300 3000 2    60   ~ 0
A7
Text Label 2500 4200 0    60   ~ 0
~RESET
Text Label 2500 2700 0    60   ~ 0
D8
Text Label 2500 2800 0    60   ~ 0
D9
Text Label 2500 2900 0    60   ~ 0
D10
Text Label 2500 3000 0    60   ~ 0
D11/MOSI
Text Label 2500 3100 0    60   ~ 0
D12/MISO
Text Label 2500 3200 0    60   ~ 0
D13/SCLK
Text Label 4200 5300 1    60   ~ 0
D13/SCLK
Text Label 3250 4400 0    60   ~ 0
RX
Text Label 3250 4500 0    60   ~ 0
TX
Text Label 900  7150 2    60   ~ 0
USB_5V
Text Label 1350 2000 2    60   ~ 0
5V
Text Label 3000 6500 0    60   ~ 0
5V
Text Label 3000 6750 0    60   ~ 0
D11/MOSI
Text Label 2500 6650 2    60   ~ 0
D12/MISO
Text Label 2500 6750 2    60   ~ 0
D13/SCLK
Text Label 2500 6850 2    60   ~ 0
~RESET
Text Notes 800  3550 0    60   ~ 0
ATMEGA328P
Text Notes 8000 2000 0    60   ~ 0
USB interface
Text Label 6700 5800 3    60   ~ 0
USB_5V
Text Notes 2150 6100 0    60   ~ 0
Programming connector
Text Label 9450 2900 0    60   ~ 0
USB_D-
Text Label 9450 3050 0    60   ~ 0
USB_D+
Text Label 1050 6950 2    60   ~ 0
USB_D+
Text Label 1050 6850 2    60   ~ 0
USB_D-
Text Notes 800  6100 0    60   ~ 0
USB connector
Text Label 3200 4000 0    60   ~ 0
5V
Text Label 9450 2300 0    60   ~ 0
USB_5V
Text Label 9450 4100 0    60   ~ 0
USB_3V3
Text Label 8350 4850 2    60   ~ 0
USB_GND
Text Label 700  2900 3    60   ~ 0
GND
Text Label 1900 5400 3    60   ~ 0
GND
Text Label 4200 5800 3    60   ~ 0
GND
Text Label 1250 6450 2    60   ~ 0
USB_GND
Text Label 9450 4450 3    60   ~ 0
USB_GND
Text Label 1550 2400 2    60   ~ 0
GND
Text Label 10850 6300 0    60   ~ 0
GND
Text Label 10850 3600 0    60   ~ 0
D0/RX
Text Label 10850 3700 0    60   ~ 0
D1/TX
Text Label 10850 3800 0    60   ~ 0
D2
Text Label 10850 3900 0    60   ~ 0
D3
Text Label 10850 4000 0    60   ~ 0
D4
Text Label 10850 4100 0    60   ~ 0
D5
Text Label 10850 4200 0    60   ~ 0
D6
Text Label 10850 4300 0    60   ~ 0
D7
Text Label 10850 5150 0    60   ~ 0
A0
Text Label 10850 5250 0    60   ~ 0
A1
Text Label 10850 5350 0    60   ~ 0
A2
Text Label 10850 5450 0    60   ~ 0
A3
Text Label 10850 5550 0    60   ~ 0
A4
Text Label 10850 5650 0    60   ~ 0
A5
Text Label 10850 6100 0    60   ~ 0
~RESET
Text Label 10850 4400 0    60   ~ 0
D8
Text Label 10850 4500 0    60   ~ 0
D9
Text Label 10850 4600 0    60   ~ 0
D10
Text Label 10850 4700 0    60   ~ 0
D11/MOSI
Text Label 10850 4800 0    60   ~ 0
D12/MISO
Text Label 10850 4900 0    60   ~ 0
D13/SCLK
Text Label 10850 5750 0    60   ~ 0
A6
Text Label 10850 5850 0    60   ~ 0
A7
Text Label 10850 6200 0    60   ~ 0
5V
Text HLabel 10400 6300 0    60   Input ~ 0
GND
Text HLabel 10400 3600 0    60   Input ~ 0
D0/RX
Text HLabel 10400 3700 0    60   Input ~ 0
D1/TX
Text HLabel 10400 3800 0    60   Input ~ 0
D2
Text HLabel 10400 3900 0    60   Input ~ 0
D3
Text HLabel 10400 4000 0    60   Input ~ 0
D4
Text HLabel 10400 4100 0    60   Input ~ 0
D5
Text HLabel 10400 4200 0    60   Input ~ 0
D6
Text HLabel 10400 4300 0    60   Input ~ 0
D7
Text HLabel 10400 5150 0    60   Input ~ 0
A0
Text HLabel 10400 5250 0    60   Input ~ 0
A1
Text HLabel 10400 5350 0    60   Input ~ 0
A2
Text HLabel 10400 5450 0    60   Input ~ 0
A3
Text HLabel 10400 5550 0    60   Input ~ 0
A4
Text HLabel 10400 5650 0    60   Input ~ 0
A5
Text HLabel 10400 6100 0    60   Input ~ 0
~RESET
Text HLabel 10400 4400 0    60   Input ~ 0
D8
Text HLabel 10400 4500 0    60   Input ~ 0
D9
Text HLabel 10400 4600 0    60   Input ~ 0
D10
Text HLabel 10400 4700 0    60   Input ~ 0
D11/MOSI
Text HLabel 10400 4800 0    60   Input ~ 0
D12/MISO
Text HLabel 10400 4900 0    60   Input ~ 0
D13/SCLK
Text HLabel 10400 5750 0    60   Input ~ 0
A6_inputonly
Text HLabel 10400 5850 0    60   Input ~ 0
A7_inputonly
Text HLabel 10400 6200 0    60   Input ~ 0
5V
Text Notes 4850 1300 0    197  ~ 39
Arduino copycat
Text Notes 10300 3350 0    60   ~ 0
Outputs
Text Label 4350 1650 0    60   ~ 0
5V
Text Label 4150 1950 2    60   ~ 0
GND
Text Label 4200 2600 2    60   ~ 0
TX
Text Label 4200 2200 2    60   ~ 0
RX
Text Label 6100 1700 0    60   ~ 0
USB_5V
Text Label 6100 1950 0    60   ~ 0
USB_GND
Text Label 4200 2300 2    60   ~ 0
USB_RESET_OUT
Text Label 5800 2600 0    60   ~ 0
ISO_OUTPUT
Text Label 10850 3450 0    60   ~ 0
ISO_OUTPUT
Text Label 5950 2300 0    60   ~ 0
USB_RESET_IN
Text Label 4400 3850 0    60   ~ 0
USB_RESET_OUT
Text HLabel 10400 3450 0    60   Input ~ 0
ISO_OUTPUT
Wire Wire Line
	2500 3300 3100 3300
Wire Wire Line
	2500 3400 3100 3400
Wire Wire Line
	700  2700 1300 2700
Wire Wire Line
	2500 4200 3200 4200
Wire Wire Line
	1350 2000 1400 2000
Wire Wire Line
	1400 2250 1550 2250
Wire Wire Line
	1650 2250 1650 2200
Wire Wire Line
	1400 2250 1400 2200
Wire Wire Line
	2500 4400 2900 4400
Wire Wire Line
	2900 4500 2500 4500
Wire Wire Line
	3100 4400 3250 4400
Wire Wire Line
	3250 4500 3100 4500
Wire Wire Line
	3500 3850 3750 3850
Wire Wire Line
	9450 2300 9450 2450
Wire Wire Line
	7250 3800 6850 3800
Wire Wire Line
	8050 4750 8200 4750
Wire Wire Line
	8350 4750 8350 4850
Wire Wire Line
	3000 6850 3000 7000
Wire Wire Line
	3000 6500 3000 6650
Wire Wire Line
	6850 3800 6850 5100
Wire Wire Line
	6550 5100 6550 3650
Wire Wire Line
	6550 3650 7250 3650
Wire Wire Line
	6550 5600 6550 5650
Wire Wire Line
	6550 5650 6700 5650
Wire Wire Line
	6700 5650 6700 5800
Wire Wire Line
	6850 5650 6850 5600
Wire Wire Line
	9450 4250 9450 4100
Wire Wire Line
	1550 2250 1550 2400
Wire Wire Line
	10400 3600 10850 3600
Wire Wire Line
	10850 3700 10400 3700
Wire Wire Line
	10400 3800 10850 3800
Wire Wire Line
	10850 3900 10400 3900
Wire Wire Line
	10400 4000 10850 4000
Wire Wire Line
	10850 4100 10400 4100
Wire Wire Line
	10400 4200 10850 4200
Wire Wire Line
	10850 4300 10400 4300
Wire Wire Line
	10400 4400 10850 4400
Wire Wire Line
	10850 4500 10400 4500
Wire Wire Line
	10400 4600 10850 4600
Wire Wire Line
	10850 4700 10400 4700
Wire Wire Line
	10400 4800 10850 4800
Wire Wire Line
	10850 4900 10400 4900
Wire Wire Line
	10850 5150 10400 5150
Wire Wire Line
	10400 5250 10850 5250
Wire Wire Line
	10850 5350 10400 5350
Wire Wire Line
	10400 5450 10850 5450
Wire Wire Line
	10850 5550 10400 5550
Wire Wire Line
	10400 5650 10850 5650
Wire Wire Line
	10850 5750 10400 5750
Wire Wire Line
	10400 5850 10850 5850
Wire Wire Line
	10400 6100 10850 6100
Wire Wire Line
	10850 6200 10400 6200
Wire Wire Line
	10400 6300 10850 6300
Wire Wire Line
	4350 1650 4350 1800
Wire Wire Line
	4350 1800 4400 1800
Wire Wire Line
	4400 1900 4400 1950
Wire Wire Line
	4150 1950 4400 1950
Wire Wire Line
	7200 2450 7250 2450
Wire Wire Line
	7200 2200 7200 2450
Wire Wire Line
	7250 2600 7050 2600
Wire Wire Line
	7050 2600 7050 2500
Wire Wire Line
	7050 2500 5800 2500
Wire Wire Line
	4200 2300 4400 2300
Wire Wire Line
	4200 2600 4400 2600
Wire Wire Line
	6100 1700 6100 1800
Wire Wire Line
	6100 1800 5800 1800
Wire Wire Line
	5800 1900 5800 1950
Wire Wire Line
	6100 1950 5800 1950
Wire Wire Line
	7250 3050 6850 3050
Wire Wire Line
	3500 3850 3500 4200
Wire Wire Line
	4200 2200 4400 2200
Wire Wire Line
	4050 3850 4400 3850
Wire Wire Line
	5800 2300 6850 2300
Wire Wire Line
	6850 2300 6850 3050
Wire Wire Line
	7200 2200 5800 2200
Wire Wire Line
	10400 3450 10850 3450
Wire Wire Line
	3350 3550 3150 3550
Wire Wire Line
	3100 3550 3100 3400
Wire Wire Line
	3100 3300 3100 3150
Wire Wire Line
	3100 3150 3200 3150
Wire Wire Line
	1350 6400 1350 6450
Wire Wire Line
	1250 6450 1350 6450
Wire Wire Line
	1450 6450 1450 6550
Wire Wire Line
	900  7150 900  7250
Wire Wire Line
	900  7150 1050 7150
Wire Wire Line
	3350 3150 3350 3200
Wire Wire Line
	3350 3500 3350 3550
Wire Wire Line
	3200 4200 3500 4200
Wire Wire Line
	1400 2000 1650 2000
Wire Wire Line
	1650 2000 1900 2000
Wire Wire Line
	1550 2250 1650 2250
Wire Wire Line
	9450 2450 9450 2600
Wire Wire Line
	8350 4750 8500 4750
Wire Wire Line
	8200 4750 8350 4750
Wire Wire Line
	8500 4750 8650 4750
Wire Wire Line
	6700 5650 6850 5650
Wire Wire Line
	4400 1950 4400 2000
Wire Wire Line
	5800 1950 5800 2000
Wire Wire Line
	3200 3150 3350 3150
Wire Wire Line
	3150 3550 3100 3550
Wire Wire Line
	1350 6450 1350 6550
Wire Wire Line
	1350 6450 1450 6450
Wire Wire Line
	1900 2000 1900 2400
Wire Wire Line
	1900 2000 2000 2000
Wire Wire Line
	2000 2000 2000 2400
Connection ~ 1400 2000
Connection ~ 1650 2000
Connection ~ 1550 2250
Connection ~ 9450 2450
Connection ~ 9450 4250
Connection ~ 8350 4750
Connection ~ 8200 4750
Connection ~ 8500 4750
Connection ~ 6700 5650
Connection ~ 4400 1950
Connection ~ 5800 1950
Connection ~ 3150 3550
Connection ~ 1350 6450
Connection ~ 3200 3150
Connection ~ 3200 4200
Connection ~ 1900 2000
NoConn ~ 7250 2750
NoConn ~ 7250 2900
NoConn ~ 7250 3200
NoConn ~ 7250 3350
NoConn ~ 7250 3500
NoConn ~ 7250 3950
NoConn ~ 7250 4100
NoConn ~ 7250 4250
NoConn ~ 9450 3800
NoConn ~ 9450 3650
NoConn ~ 9450 3350
NoConn ~ 4400 2400
NoConn ~ 5800 2700
NoConn ~ 4400 2700
NoConn ~ 1050 6750
NoConn ~ 3150 3350
$EndSCHEMATC
