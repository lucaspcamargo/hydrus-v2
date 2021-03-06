EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:switches
LIBS:relays
LIBS:motors
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
LIBS:drone-alt-phmeter-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "SIMPLE PH METER WITH ADJUSTABLE OFFSET VOLTAGE"
Date "2017-11-11"
Rev "1"
Comp "UFSC - CTJ"
Comment1 "based on the TL082 OpAmp as a signal buffer"
Comment2 "Adapted from http://www.66pacific.com/ph/simplest-ph.aspx"
Comment3 ""
Comment4 ""
$EndDescr
Text Notes 750  925  0    60   ~ 0
SIMPLE PH METER WITH ADJUSTABLE OFFSET VOLTAGE\nbased on the TL082 OpAmp as a signal buffer
Text Notes 750  1100 0    60   ~ 0
Adapted from http://www.66pacific.com/ph/simplest-ph.aspx
$Comp
L TL082 U1
U 1 1 5A071C21
P 5675 3600
F 0 "U1" H 5675 3800 50  0000 L CNN
F 1 "TL082" H 5675 3400 50  0000 L CNN
F 2 "" H 5675 3600 50  0001 C CNN
F 3 "" H 5675 3600 50  0001 C CNN
	1    5675 3600
	1    0    0    -1  
$EndComp
$Comp
L Battery BT2
U 1 1 5A071DF1
P 5575 4250
F 0 "BT2" H 5675 4350 50  0000 L CNN
F 1 "9V Battery" H 5675 4250 50  0000 L CNN
F 2 "" V 5575 4310 50  0001 C CNN
F 3 "" V 5575 4310 50  0001 C CNN
	1    5575 4250
	-1   0    0    1   
$EndComp
$Comp
L Battery BT1
U 1 1 5A071E90
P 5575 2950
F 0 "BT1" H 5675 3050 50  0000 L CNN
F 1 "9V Battery" H 5675 2950 50  0000 L CNN
F 2 "" V 5575 3010 50  0001 C CNN
F 3 "" V 5575 3010 50  0001 C CNN
	1    5575 2950
	-1   0    0    1   
$EndComp
Wire Wire Line
	5575 3150 5575 3300
Wire Wire Line
	5575 3900 5575 4050
Wire Wire Line
	5575 2750 5575 2600
Wire Wire Line
	5575 2600 4975 2600
Wire Wire Line
	4975 2600 4975 2750
$Comp
L GND #PWR3
U 1 1 5A072015
P 4975 2750
F 0 "#PWR3" H 4975 2500 50  0001 C CNN
F 1 "GND" H 4975 2600 50  0000 C CNN
F 2 "" H 4975 2750 50  0001 C CNN
F 3 "" H 4975 2750 50  0001 C CNN
	1    4975 2750
	1    0    0    -1  
$EndComp
Wire Wire Line
	5575 4450 5575 4550
$Comp
L GND #PWR4
U 1 1 5A07203A
P 5575 4550
F 0 "#PWR4" H 5575 4300 50  0001 C CNN
F 1 "GND" H 5575 4400 50  0000 C CNN
F 2 "" H 5575 4550 50  0001 C CNN
F 3 "" H 5575 4550 50  0001 C CNN
	1    5575 4550
	1    0    0    -1  
$EndComp
Wire Wire Line
	5375 3700 5300 3700
Wire Wire Line
	5300 3700 5300 3975
Wire Wire Line
	5975 3975 5975 3600
$Comp
L Conn_01x03 J2
U 1 1 5A0720BA
P 6850 3600
F 0 "J2" H 6850 3800 50  0000 C CNN
F 1 "Conn_01x03" H 6850 3400 50  0000 C CNN
F 2 "" H 6850 3600 50  0001 C CNN
F 3 "" H 6850 3600 50  0001 C CNN
	1    6850 3600
	1    0    0    -1  
$EndComp
Wire Wire Line
	5300 3975 5975 3975
Wire Wire Line
	6650 3700 6375 3700
Wire Wire Line
	6375 3700 6375 4025
$Comp
L GND #PWR6
U 1 1 5A072158
P 6375 4025
F 0 "#PWR6" H 6375 3775 50  0001 C CNN
F 1 "GND" H 6375 3875 50  0000 C CNN
F 2 "" H 6375 4025 50  0001 C CNN
F 3 "" H 6375 4025 50  0001 C CNN
	1    6375 4025
	1    0    0    -1  
$EndComp
Wire Wire Line
	5975 3600 6650 3600
Text GLabel 6150 3600 3    60   Input ~ 0
Vo
$Comp
L +3.3V #PWR5
U 1 1 5A07218F
P 6375 3150
F 0 "#PWR5" H 6375 3000 50  0001 C CNN
F 1 "+3.3V" H 6375 3290 50  0000 C CNN
F 2 "" H 6375 3150 50  0001 C CNN
F 3 "" H 6375 3150 50  0001 C CNN
	1    6375 3150
	1    0    0    -1  
$EndComp
Wire Wire Line
	6375 3150 6375 3500
Wire Wire Line
	6375 3500 6650 3500
$Comp
L Conn_Coaxial J1
U 1 1 5A072252
P 4300 3650
F 0 "J1" H 4310 3770 50  0000 C CNN
F 1 "pH Probe" V 4415 3650 50  0000 C CNN
F 2 "" H 4300 3650 50  0001 C CNN
F 3 "" H 4300 3650 50  0001 C CNN
	1    4300 3650
	0    1    1    0   
$EndComp
Wire Wire Line
	4300 3500 5375 3500
$Comp
L POT RV1
U 1 1 5A072319
P 3275 3425
F 0 "RV1" V 3100 3425 50  0000 C CNN
F 1 "POT" V 3175 3425 50  0000 C CNN
F 2 "" H 3275 3425 50  0001 C CNN
F 3 "" H 3275 3425 50  0001 C CNN
	1    3275 3425
	0    1    1    0   
$EndComp
$Comp
L POT RV2
U 1 1 5A07245C
P 3275 3825
F 0 "RV2" V 3100 3825 50  0000 C CNN
F 1 "POT" V 3175 3825 50  0000 C CNN
F 2 "" H 3275 3825 50  0001 C CNN
F 3 "" H 3275 3825 50  0001 C CNN
	1    3275 3825
	1    0    0    -1  
$EndComp
Wire Wire Line
	3275 3675 3275 3650
Wire Wire Line
	3275 3650 3275 3575
Wire Wire Line
	4100 3650 3275 3650
Connection ~ 3275 3650
Text GLabel 3725 3650 3    60   Input ~ 0
Vref
$Comp
L +3.3V #PWR1
U 1 1 5A072605
P 3775 3300
F 0 "#PWR1" H 3775 3150 50  0001 C CNN
F 1 "+3.3V" H 3775 3440 50  0000 C CNN
F 2 "" H 3775 3300 50  0001 C CNN
F 3 "" H 3775 3300 50  0001 C CNN
	1    3775 3300
	1    0    0    -1  
$EndComp
Wire Wire Line
	3775 3300 3775 3425
Wire Wire Line
	3775 3425 3425 3425
$Comp
L GND #PWR2
U 1 1 5A072643
P 3850 4200
F 0 "#PWR2" H 3850 3950 50  0001 C CNN
F 1 "GND" H 3850 4050 50  0000 C CNN
F 2 "" H 3850 4200 50  0001 C CNN
F 3 "" H 3850 4200 50  0001 C CNN
	1    3850 4200
	1    0    0    -1  
$EndComp
Wire Wire Line
	3850 4125 3850 4200
Wire Notes Line
	2925 3150 2925 4425
Wire Notes Line
	2925 4425 3525 4425
Wire Notes Line
	3525 4425 3525 3150
Wire Notes Line
	3525 3150 2925 3150
Text Notes 2925 4700 0    60   ~ 0
Adjustable Reference\n(offset)
Wire Wire Line
	3425 3825 3425 4125
Wire Wire Line
	3425 4125 3850 4125
Text Notes 7500 3600 0    60   ~ 0
Vcc: 3.3V\nDesired Vref: 1.65V\nTrimpots are of 10 kOhm\n
$EndSCHEMATC
