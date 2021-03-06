EESchema Schematic File Version 2
LIBS:conservify
LIBS:fk-weather-sensors-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "fk-core"
Date "2017-03-05"
Rev "0.1"
Comp "Conservify"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L 3V3 #PWR4
U 1 1 5A4464F2
P 5750 4800
F 0 "#PWR4" H 5750 4650 50  0001 C CNN
F 1 "3V3" H 5750 4940 50  0000 C CNN
F 2 "" H 5750 4800 60  0000 C CNN
F 3 "" H 5750 4800 60  0000 C CNN
	1    5750 4800
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR3
U 1 1 5A4464F3
P 5550 5000
F 0 "#PWR3" H 5550 4750 50  0001 C CNN
F 1 "GND" H 5550 4850 50  0001 C CNN
F 2 "" H 5550 5000 60  0000 C CNN
F 3 "" H 5550 5000 60  0000 C CNN
	1    5550 5000
	-1   0    0    1   
$EndComp
$Comp
L MPL3115A2 U1
U 1 1 5A4464F4
P 2850 1950
F 0 "U1" H 2850 1950 60  0000 C CNN
F 1 "MPL3115A2" H 2850 2050 60  0000 C CNN
F 2 "conservify:DFN-8-1EP_5x2.55mm_Pitch1.25mm_98ASA002260D" H 2850 1950 60  0001 C CNN
F 3 "" H 2850 1950 60  0001 C CNN
F 4 "0" H 2850 2050 50  0001 C CNN "price"
F 5 "841-MPL3115A2" H -7150 100 50  0001 C CNN "spn1"
F 6 "mouser" H -7150 100 50  0001 C CNN "supplier1"
	1    2850 1950
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR2
U 1 1 5A4464F5
P 4800 2800
F 0 "#PWR2" H 4800 2550 50  0001 C CNN
F 1 "GND" H 4800 2650 50  0001 C CNN
F 2 "" H 4800 2800 60  0000 C CNN
F 3 "" H 4800 2800 60  0000 C CNN
	1    4800 2800
	1    0    0    -1  
$EndComp
$Comp
L CAPACITOR-CERAMIC C1
U 1 1 5A4464F6
P 4350 2200
F 0 "C1" V 4200 2150 50  0000 L CNN
F 1 "100nF" V 4500 2100 50  0000 L CNN
F 2 "conservify:CAP-0603" H 4350 2000 30  0001 C CNN
F 3 "" H 4350 2200 60  0000 C CNN
F 4 "0" H 4200 2250 50  0001 C CNN "price"
F 5 "81-GRM39X104K50D" H -7150 100 50  0001 C CNN "spn1"
F 6 "mouser" H -7150 100 50  0001 C CNN "supplier1"
	1    4350 2200
	1    0    0    -1  
$EndComp
$Comp
L CAPACITOR-CERAMIC C2
U 1 1 5A4464F7
P 4800 2200
F 0 "C2" V 4650 2150 50  0000 L CNN
F 1 "100nF" V 4950 2100 50  0000 L CNN
F 2 "conservify:CAP-0603" H 4800 2000 30  0001 C CNN
F 3 "" H 4800 2200 60  0000 C CNN
F 4 "0" H 4650 2250 50  0001 C CNN "price"
F 5 "81-GRM39X104K50D" H -7150 100 50  0001 C CNN "spn1"
F 6 "mouser" H -7150 100 50  0001 C CNN "supplier1"
	1    4800 2200
	1    0    0    -1  
$EndComp
$Comp
L CAPACITOR-CERAMIC C4
U 1 1 5A4464F8
P 5250 2200
F 0 "C4" V 5100 2150 50  0000 L CNN
F 1 "10uF" V 5400 2100 50  0000 L CNN
F 2 "conservify:CAP-0603" H 5250 2000 30  0001 C CNN
F 3 "" H 5250 2200 60  0000 C CNN
F 4 "0" H 5100 2250 50  0001 C CNN "price"
F 5 "81-GRM188R60J106ME47" H -7150 100 50  0001 C CNN "spn1"
F 6 "mouser" H -7150 100 50  0001 C CNN "supplier1"
	1    5250 2200
	1    0    0    -1  
$EndComp
$Comp
L TSL2591 U2
U 1 1 5A4464F9
P 9050 5350
F 0 "U2" H 9050 5350 60  0000 C CNN
F 1 "TSL2591" H 9050 5450 60  0000 C CNN
F 2 "conservify:TSL2591" H 9050 5350 60  0001 C CNN
F 3 "" H 9050 5350 60  0001 C CNN
F 4 "0" H 9050 5450 50  0001 C CNN "price"
F 5 "985-TSL25911FN" H -800 1750 50  0001 C CNN "spn1"
F 6 "mouser" H -800 1750 50  0001 C CNN "supplier1"
	1    9050 5350
	1    0    0    -1  
$EndComp
$Comp
L 3V3 #PWR1
U 1 1 5A4464FA
P 4800 1700
F 0 "#PWR1" H 4800 1550 50  0001 C CNN
F 1 "3V3" H 4800 1840 50  0000 C CNN
F 2 "" H 4800 1700 60  0000 C CNN
F 3 "" H 4800 1700 60  0000 C CNN
	1    4800 1700
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR9
U 1 1 5A4464FB
P 10550 5850
F 0 "#PWR9" H 10550 5600 50  0001 C CNN
F 1 "GND" H 10550 5700 50  0001 C CNN
F 2 "" H 10550 5850 60  0000 C CNN
F 3 "" H 10550 5850 60  0000 C CNN
	1    10550 5850
	1    0    0    -1  
$EndComp
$Comp
L 3V3 #PWR8
U 1 1 5A4464FC
P 10550 5200
F 0 "#PWR8" H 10550 5050 50  0001 C CNN
F 1 "3V3" H 10550 5340 50  0000 C CNN
F 2 "" H 10550 5200 60  0000 C CNN
F 3 "" H 10550 5200 60  0000 C CNN
	1    10550 5200
	1    0    0    -1  
$EndComp
$Comp
L CAPACITOR-CERAMIC C3
U 1 1 5A4464FD
P 10550 5500
F 0 "C3" V 10400 5450 50  0000 L CNN
F 1 "1uF" V 10700 5450 50  0000 L CNN
F 2 "conservify:CAP-0603" H 10550 5300 30  0001 C CNN
F 3 "" H 10550 5500 60  0000 C CNN
F 4 "0" H 10400 5550 50  0001 C CNN "price"
F 5 "963-UMK107BJ105KA-T" H -800 1750 50  0001 C CNN "spn1"
F 6 "mouser" H -800 1750 50  0001 C CNN "supplier1"
	1    10550 5500
	1    0    0    -1  
$EndComp
$Comp
L SHT31 U3
U 1 1 5A4464FE
P 8500 1650
F 0 "U3" H 8500 1650 60  0000 C CNN
F 1 "SHT31" H 8500 1750 60  0000 C CNN
F 2 "conservify:SHT3x" H 8500 1650 60  0001 C CNN
F 3 "" H 8500 1650 60  0001 C CNN
F 4 "0" H 8500 1750 50  0001 C CNN "price"
F 5 "403-SHT31-DIS-B" H 8500 1650 60  0001 C CNN "spn1"
F 6 "mouser" H 8500 1650 60  0001 C CNN "supplier1"
	1    8500 1650
	1    0    0    -1  
$EndComp
$Comp
L CAPACITOR-CERAMIC C5
U 1 1 5A4464FF
P 10050 1650
F 0 "C5" V 9900 1600 50  0000 L CNN
F 1 "100nF" V 10200 1550 50  0000 L CNN
F 2 "conservify:CAP-0603" H 10050 1450 30  0001 C CNN
F 3 "" H 10050 1650 60  0000 C CNN
F 4 "0" H 9900 1700 50  0001 C CNN "price"
F 5 "81-GRM39X104K50D" H -1400 -3250 50  0001 C CNN "spn1"
F 6 "mouser" H -1400 -3250 50  0001 C CNN "supplier1"
	1    10050 1650
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR7
U 1 1 5A446500
P 10050 1950
F 0 "#PWR7" H 10050 1700 50  0001 C CNN
F 1 "GND" H 10050 1800 50  0001 C CNN
F 2 "" H 10050 1950 60  0000 C CNN
F 3 "" H 10050 1950 60  0000 C CNN
	1    10050 1950
	1    0    0    -1  
$EndComp
$Comp
L 3V3 #PWR6
U 1 1 5A446501
P 10050 1400
F 0 "#PWR6" H 10050 1250 50  0001 C CNN
F 1 "3V3" H 10050 1540 50  0000 C CNN
F 2 "" H 10050 1400 60  0000 C CNN
F 3 "" H 10050 1400 60  0000 C CNN
	1    10050 1400
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR5
U 1 1 5A446546
P 7550 1950
F 0 "#PWR5" H 7550 1700 50  0001 C CNN
F 1 "GND" H 7550 1800 50  0001 C CNN
F 2 "" H 7550 1950 60  0000 C CNN
F 3 "" H 7550 1950 60  0000 C CNN
	1    7550 1950
	1    0    0    -1  
$EndComp
Text Label 5750 5250 0    50   ~ 0
SDA
Text Label 5750 5150 0    50   ~ 0
SCL
Text Label 8250 5350 2    50   ~ 0
SDA
Text Label 8250 5250 2    50   ~ 0
SCL
Text Label 7650 1500 2    50   ~ 0
SDA
Text Label 7650 1600 2    50   ~ 0
SCL
Text Label 2000 2000 2    50   ~ 0
SDA
Text Label 2000 2100 2    50   ~ 0
SCL
NoConn ~ 2100 1900
NoConn ~ 2100 1800
NoConn ~ 8400 5450
NoConn ~ 7800 1700
Wire Wire Line
	5750 4950 6000 4950
Wire Wire Line
	1850 2000 2100 2000
Wire Wire Line
	1850 2100 2100 2100
Wire Wire Line
	3600 2000 4000 2000
Wire Wire Line
	4000 2000 4000 2600
Wire Wire Line
	3600 1800 5250 1800
Wire Wire Line
	3600 1900 4350 1900
Wire Wire Line
	4000 2600 5250 2600
Wire Wire Line
	5250 2600 5250 2350
Wire Wire Line
	5250 1800 5250 2050
Wire Wire Line
	4800 1700 4800 2050
Wire Wire Line
	4800 2350 4800 2800
Wire Wire Line
	4350 1900 4350 2050
Wire Wire Line
	4350 2350 4350 2600
Wire Wire Line
	3600 2100 3800 2100
Wire Wire Line
	3800 2100 3800 1800
Wire Wire Line
	9700 5350 10050 5350
Wire Wire Line
	10050 5350 10050 5850
Wire Wire Line
	9700 5250 10550 5250
Wire Wire Line
	10550 5200 10550 5350
Wire Wire Line
	10550 5650 10550 5850
Wire Wire Line
	10550 5850 10050 5850
Wire Wire Line
	8100 5350 8400 5350
Wire Wire Line
	8100 5250 8400 5250
Wire Wire Line
	9200 1500 10050 1500
Wire Wire Line
	9200 1600 9250 1600
Wire Wire Line
	9250 1600 9250 1500
Wire Wire Line
	9200 1700 9250 1700
Wire Wire Line
	9250 1700 9250 1900
Wire Wire Line
	9200 1800 10050 1800
Wire Wire Line
	10050 1800 10050 1950
Wire Wire Line
	10050 1500 10050 1400
Wire Wire Line
	5750 4800 5750 4950
Wire Wire Line
	6000 5050 5550 5050
Wire Wire Line
	5550 5050 5550 5000
Wire Wire Line
	7550 1800 7800 1800
Wire Wire Line
	7550 1800 7550 1950
Wire Wire Line
	7500 1600 7800 1600
Wire Wire Line
	7500 1500 7800 1500
Wire Wire Line
	9250 1900 9200 1900
Connection ~ 4800 1800
Connection ~ 4350 2600
Connection ~ 4800 2600
Connection ~ 3800 1800
Connection ~ 10550 5250
Connection ~ 9250 1500
Connection ~ 9250 1800
Wire Notes Line
	7000 6500 11150 6500
Wire Notes Line
	11150 6500 11150 4050
Wire Notes Line
	11150 4050 7000 4050
Wire Notes Line
	7000 4050 7000 6500
Wire Notes Line
	11150 4000 11150 550 
Wire Notes Line
	11150 550  7000 550 
Wire Notes Line
	7000 550  7000 4000
Wire Notes Line
	7000 4000 11150 4000
Wire Notes Line
	6950 4000 550  4000
Wire Notes Line
	550  4000 550  550 
Wire Notes Line
	550  550  6950 550 
Wire Notes Line
	6950 550  6950 4000
Wire Notes Line
	6950 4050 4350 4050
Wire Notes Line
	4350 4050 4350 6500
Wire Notes Line
	4350 6500 6950 6500
Wire Notes Line
	6950 6500 6950 4050
Text Notes 5150 4300 0    118  ~ 24
CONNECTOR
Text Notes 8600 4300 0    118  ~ 24
TSL (LIGHT)
Text Notes 8600 800  0    118  ~ 24
SHT31 (TEMP)
Text Notes 2150 800  0    118  ~ 24
MPL3115A2 (PRESSURE/HUMIDITY)
Wire Wire Line
	5750 5150 6000 5150
Wire Wire Line
	6000 5250 5750 5250
$Comp
L CONN_01x04 J1
U 1 1 5B584D44
P 6200 5050
F 0 "J1" H 6200 5250 50  0000 C CNN
F 1 "CONN_01x04" H 6200 4750 50  0000 C CNN
F 2 "conservify:Molex_CLIK-Mate_502382-0470_1x04_P1.25mm_Vertical" H 6200 5050 50  0001 C CNN
F 3 "" H 6200 5050 50  0001 C CNN
	1    6200 5050
	1    0    0    -1  
$EndComp
$EndSCHEMATC
