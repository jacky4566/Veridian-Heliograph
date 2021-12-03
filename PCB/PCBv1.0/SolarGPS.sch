EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr USLegal 14000 8500
encoding utf-8
Sheet 1 1
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
L power:GND #PWR02
U 1 1 5FC2886B
P 4750 7750
F 0 "#PWR02" H 4750 7500 50  0001 C CNN
F 1 "GND" H 4755 7577 50  0000 C CNN
F 2 "" H 4750 7750 50  0001 C CNN
F 3 "" H 4750 7750 50  0001 C CNN
	1    4750 7750
	1    0    0    -1  
$EndComp
Wire Wire Line
	4300 7050 4300 7175
$Comp
L Device:R_Small R15
U 1 1 5FE60743
P 5275 2075
F 0 "R15" H 5400 2025 50  0000 C CNN
F 1 "1M" H 5400 2100 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 5275 2075 50  0001 C CNN
F 3 "~" H 5275 2075 50  0001 C CNN
	1    5275 2075
	1    0    0    1   
$EndComp
$Comp
L Device:C_Small C28
U 1 1 5FE60749
P 5000 2075
F 0 "C28" H 4800 2050 50  0000 C CNN
F 1 "10nF" H 4800 2125 50  0000 C CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 5000 2075 50  0001 C CNN
F 3 "~" H 5000 2075 50  0001 C CNN
	1    5000 2075
	1    0    0    1   
$EndComp
Wire Wire Line
	5000 2175 5275 2175
$Comp
L power:GND #PWR017
U 1 1 5FE60750
P 5275 2175
F 0 "#PWR017" H 5275 1925 50  0001 C CNN
F 1 "GND" H 5280 2002 50  0000 C CNN
F 2 "" H 5275 2175 50  0001 C CNN
F 3 "" H 5275 2175 50  0001 C CNN
	1    5275 2175
	1    0    0    -1  
$EndComp
Connection ~ 5275 2175
$Comp
L Device:R_Small R14
U 1 1 5FE60757
P 5275 1875
F 0 "R14" H 5375 1825 50  0000 C CNN
F 1 "1M" H 5400 1900 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 5275 1875 50  0001 C CNN
F 3 "~" H 5275 1875 50  0001 C CNN
	1    5275 1875
	1    0    0    1   
$EndComp
Wire Wire Line
	5000 1975 5275 1975
Connection ~ 5275 1975
Text GLabel 5275 1775 1    50   Input ~ 0
SOLAR+
Text GLabel 5000 1975 1    50   Input ~ 0
SOLAR_SENSE1
Connection ~ 1900 1425
$Comp
L Device:C_Small C24
U 1 1 5FA4FE7E
P 1900 1525
F 0 "C24" H 2000 1550 50  0000 L CNN
F 1 "1uF" H 1992 1480 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 1900 1525 50  0001 C CNN
F 3 "~" H 1900 1525 50  0001 C CNN
	1    1900 1525
	1    0    0    -1  
$EndComp
Text GLabel 1675 3175 0    50   Input ~ 0
GPS_EXINT
$Comp
L power:GND #PWR015
U 1 1 5FA4E6CF
P 2275 3675
F 0 "#PWR015" H 2275 3425 50  0001 C CNN
F 1 "GND" H 2280 3502 50  0000 C CNN
F 2 "" H 2275 3675 50  0001 C CNN
F 3 "" H 2275 3675 50  0001 C CNN
	1    2275 3675
	1    0    0    -1  
$EndComp
$Comp
L RF_GPS:MAX-M8Q U2
U 1 1 6017075D
P 2275 2975
F 0 "U2" H 2850 3550 50  0000 C CNN
F 1 "MAX-M10Q" H 2975 3450 50  0000 C CNN
F 2 "RF_GPS:ublox_MAX" H 2675 2325 50  0001 C CNN
F 3 "https://www.u-blox.com/sites/default/files/MAX-M8-FW3_DataSheet_%28UBX-15031506%29.pdf" H 2275 2975 50  0001 C CNN
	1    2275 2975
	1    0    0    -1  
$EndComp
NoConn ~ 1675 2875
NoConn ~ 1675 2975
NoConn ~ 1675 3275
NoConn ~ 1675 3375
NoConn ~ 2875 3275
NoConn ~ 2875 2775
NoConn ~ 2475 2275
Text GLabel 1675 2575 0    50   Input ~ 0
GPS_UART_MOSI
Text GLabel 1675 2675 0    50   Input ~ 0
GPS_UART_MISO
Wire Wire Line
	4300 7375 4300 7750
$Comp
L Device:C_Small C12
U 1 1 61957471
P 4425 7650
F 0 "C12" H 4600 7625 50  0000 C CNN
F 1 "100uF" H 4625 7700 50  0000 C CNN
F 2 "Capacitor_SMD:C_1206_3216Metric" H 4425 7650 50  0001 C CNN
F 3 "~" H 4425 7650 50  0001 C CNN
	1    4425 7650
	1    0    0    1   
$EndComp
Wire Wire Line
	4425 7550 4425 7050
Text GLabel 1025 1425 0    50   Input ~ 0
VDD
Wire Wire Line
	1825 1425 1900 1425
$Comp
L Connector:Conn_01x10_Female J1
U 1 1 61A063D0
P 7800 5575
F 0 "J1" H 7500 6200 50  0000 L CNN
F 1 "Conn_01x10_Female" H 7125 6100 50  0000 L CNN
F 2 "Margo_Lib:AYF531035" H 7800 5575 50  0001 C CNN
F 3 "~" H 7800 5575 50  0001 C CNN
	1    7800 5575
	0    -1   1    0   
$EndComp
Text GLabel 7600 5375 1    50   Input ~ 0
DISP_CS
Text GLabel 7700 5375 1    50   Input ~ 0
EXTCOMIN
Text GLabel 7800 5375 1    50   Input ~ 0
DISP_EN
Wire Wire Line
	8300 5375 8200 5375
Wire Wire Line
	8000 5375 7900 5375
Wire Wire Line
	8000 5375 8100 5375
Connection ~ 8000 5375
Text GLabel 7500 5375 1    50   Input ~ 0
SPI_MOSI
Wire Wire Line
	2875 2975 3200 2975
Wire Wire Line
	5675 3150 5950 3150
Connection ~ 5950 3350
$Comp
L power:GND #PWR010
U 1 1 5FE5611E
P 5950 3350
F 0 "#PWR010" H 5950 3100 50  0001 C CNN
F 1 "GND" H 5955 3177 50  0000 C CNN
F 2 "" H 5950 3350 50  0001 C CNN
F 3 "" H 5950 3350 50  0001 C CNN
	1    5950 3350
	1    0    0    -1  
$EndComp
Wire Wire Line
	5675 3350 5950 3350
$Comp
L Device:R_Small R13
U 1 1 5FE4C69D
P 5950 3250
F 0 "R13" H 6050 3200 50  0000 C CNN
F 1 "1M" H 6075 3275 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 5950 3250 50  0001 C CNN
F 3 "~" H 5950 3250 50  0001 C CNN
	1    5950 3250
	1    0    0    1   
$EndComp
$Comp
L Device:C_Small C23
U 1 1 5FE4D012
P 5675 3250
F 0 "C23" H 5525 3200 50  0000 C CNN
F 1 "10nF" H 5525 3300 50  0000 C CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 5675 3250 50  0001 C CNN
F 3 "~" H 5675 3250 50  0001 C CNN
	1    5675 3250
	1    0    0    1   
$EndComp
Text GLabel 5675 3150 1    50   Input ~ 0
VBAT_SENSE
Connection ~ 5950 3150
$Comp
L Device:R_Small R12
U 1 1 5FE565E7
P 5950 3050
F 0 "R12" H 6050 3000 50  0000 C CNN
F 1 "1M" H 6075 3075 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 5950 3050 50  0001 C CNN
F 3 "~" H 5950 3050 50  0001 C CNN
	1    5950 3050
	1    0    0    1   
$EndComp
$Comp
L Analog_Switch:MAX40200AUK U3
U 1 1 61BA8BAE
P 1425 1525
F 0 "U3" H 1625 1900 50  0000 C CNN
F 1 "MAX40200AUK" H 1425 1801 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-23-5" H 1425 2025 50  0001 C CNN
F 3 "https://datasheets.maximintegrated.com/en/ds/MAX40200.pdf" H 1425 2025 50  0001 C CNN
	1    1425 1525
	1    0    0    -1  
$EndComp
Text GLabel 1025 1625 0    50   Input ~ 0
GPSEN
$Comp
L power:GND #PWR06
U 1 1 61BB7ABB
P 1425 1825
F 0 "#PWR06" H 1425 1575 50  0001 C CNN
F 1 "GND" H 1430 1652 50  0000 C CNN
F 2 "" H 1425 1825 50  0001 C CNN
F 3 "" H 1425 1825 50  0001 C CNN
	1    1425 1825
	1    0    0    -1  
$EndComp
$Comp
L Device:CP C14
U 1 1 61BE4AC8
P 5075 7225
F 0 "C14" H 5193 7271 50  0000 L CNN
F 1 "10F" H 5193 7180 50  0000 L CNN
F 2 "Veridian-Library-KiCad:SCCT20E106SRB" H 5113 7075 50  0001 C CNN
F 3 "~" H 5075 7225 50  0001 C CNN
F 4 "SCCT20E106SRB" H 5075 7225 50  0001 C CNN "Part"
	1    5075 7225
	1    0    0    -1  
$EndComp
$Comp
L Device:CP C17
U 1 1 61BF4258
P 5075 7525
F 0 "C17" H 5193 7571 50  0000 L CNN
F 1 "10F" H 5193 7480 50  0000 L CNN
F 2 "Veridian-Library-KiCad:SCCT20E106SRB" H 5113 7375 50  0001 C CNN
F 3 "~" H 5075 7525 50  0001 C CNN
F 4 "SCCT20E106SRB" H 5075 7525 50  0001 C CNN "Part"
	1    5075 7525
	1    0    0    -1  
$EndComp
Wire Wire Line
	5075 7750 5075 7675
$Comp
L Connector:Conn_01x01_Female J2
U 1 1 61C94A66
P 1825 5225
F 0 "J2" V 1825 5150 50  0000 R CNN
F 1 "Solar1+" V 1725 5150 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x01_P2.54mm_Vertical" H 1825 5225 50  0001 C CNN
F 3 "~" H 1825 5225 50  0001 C CNN
	1    1825 5225
	0    1    -1   0   
$EndComp
$Comp
L Connector:Conn_01x01_Female J3
U 1 1 61CA53E7
P 2175 5225
F 0 "J3" V 2113 5137 50  0000 R CNN
F 1 "GND" V 2022 5137 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x01_P2.54mm_Vertical" H 2175 5225 50  0001 C CNN
F 3 "~" H 2175 5225 50  0001 C CNN
	1    2175 5225
	0    -1   -1   0   
$EndComp
$Comp
L Connector:Conn_01x01_Female J6
U 1 1 61CC324C
P 2175 4700
F 0 "J6" V 2113 4612 50  0000 R CNN
F 1 "GND" V 2022 4612 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x01_P2.54mm_Vertical" H 2175 4700 50  0001 C CNN
F 3 "~" H 2175 4700 50  0001 C CNN
	1    2175 4700
	0    -1   -1   0   
$EndComp
$Comp
L Connector:Conn_01x01_Female J5
U 1 1 61CC8536
P 1825 4700
F 0 "J5" V 1825 4600 50  0000 R CNN
F 1 "Solar2+" V 1725 4600 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x01_P2.54mm_Vertical" H 1825 4700 50  0001 C CNN
F 3 "~" H 1825 4700 50  0001 C CNN
	1    1825 4700
	0    1    -1   0   
$EndComp
Text Label 2875 2975 0    50   ~ 0
GPS_RF
$Comp
L Device:Antenna_Shield AE1
U 1 1 61D38EF7
P 3350 2775
F 0 "AE1" H 3494 2814 50  0000 L CNN
F 1 "Antenna_Shield" H 3494 2723 50  0000 L CNN
F 2 "RF_Antenna:Pulse_W3011" H 3350 2875 50  0001 C CNN
F 3 "~" H 3350 2875 50  0001 C CNN
F 4 "W3011A" H 3350 2775 50  0001 C CNN "Part"
	1    3350 2775
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0101
U 1 1 61D3AA3F
P 3450 3175
F 0 "#PWR0101" H 3450 2925 50  0001 C CNN
F 1 "GND" H 3455 3002 50  0000 C CNN
F 2 "" H 3450 3175 50  0001 C CNN
F 3 "" H 3450 3175 50  0001 C CNN
	1    3450 3175
	-1   0    0    -1  
$EndComp
$Comp
L Device:C_Small C20
U 1 1 61E379B6
P 8775 5275
F 0 "C20" H 8867 5321 50  0000 L CNN
F 1 "1uF" H 8867 5230 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 8775 5275 50  0001 C CNN
F 3 "~" H 8775 5275 50  0001 C CNN
	1    8775 5275
	1    0    0    1   
$EndComp
Text GLabel 8000 5375 1    50   Input ~ 0
VDD
Text Label 1925 1425 0    50   ~ 0
GPSVCC
Text GLabel 7400 5375 1    50   Input ~ 0
SPI_CLK
Wire Wire Line
	4300 7750 4425 7750
Wire Wire Line
	4300 7050 4425 7050
Connection ~ 4425 7050
Connection ~ 4425 7750
Wire Wire Line
	4425 7750 4750 7750
$Comp
L SolarGPS-rescue:TPS63802-VeridianDesigns U7
U 1 1 61A98429
P 4500 5625
F 0 "U7" H 4800 5552 50  0000 C CNN
F 1 "TPS63802" H 4800 5461 50  0000 C CNN
F 2 "Margo_Lib:VREG_TPS63802DLAR" H 4500 5625 50  0001 C CNN
F 3 "" H 4500 5625 50  0001 C CNN
	1    4500 5625
	1    0    0    -1  
$EndComp
$Comp
L Device:L_Small L2
U 1 1 61A9953B
P 4750 4775
F 0 "L2" V 4900 4775 50  0000 C CNN
F 1 "0.47uH" V 4825 4775 50  0000 C CNN
F 2 "Inductor_SMD:L_0806_2016Metric" H 4750 4775 50  0001 C CNN
F 3 "~" H 4750 4775 50  0001 C CNN
F 4 "DFE201612E" V 4750 4775 50  0001 C CNN "Part"
	1    4750 4775
	0    -1   -1   0   
$EndComp
Text GLabel 5825 5075 2    50   Input ~ 0
VDD
Wire Wire Line
	5825 5075 5700 5075
$Comp
L power:GND #PWR08
U 1 1 61AA53DF
P 5200 5575
F 0 "#PWR08" H 5200 5325 50  0001 C CNN
F 1 "GND" H 5205 5402 50  0000 C CNN
F 2 "" H 5200 5575 50  0001 C CNN
F 3 "" H 5200 5575 50  0001 C CNN
	1    5200 5575
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR05
U 1 1 61AA5901
P 4325 5575
F 0 "#PWR05" H 4325 5325 50  0001 C CNN
F 1 "GND" H 4330 5402 50  0000 C CNN
F 2 "" H 4325 5575 50  0001 C CNN
F 3 "" H 4325 5575 50  0001 C CNN
	1    4325 5575
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C4
U 1 1 61AA99D6
P 4325 5375
F 0 "C4" H 4450 5350 50  0000 C CNN
F 1 "1uF" H 4425 5425 50  0000 C CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 4325 5375 50  0001 C CNN
F 3 "~" H 4325 5375 50  0001 C CNN
	1    4325 5375
	-1   0    0    1   
$EndComp
Wire Wire Line
	4325 5475 4400 5475
Wire Wire Line
	4325 5475 4325 5575
Wire Wire Line
	4325 5575 4400 5575
Connection ~ 4325 5475
Connection ~ 4325 5575
Wire Wire Line
	4400 5275 4325 5275
$Comp
L Device:R_Small R1
U 1 1 61AB5D9A
P 4325 5175
F 0 "R1" H 4225 5200 50  0000 C CNN
F 1 "1M" H 4225 5125 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 4325 5175 50  0001 C CNN
F 3 "~" H 4325 5175 50  0001 C CNN
	1    4325 5175
	1    0    0    1   
$EndComp
Connection ~ 4325 5275
Wire Wire Line
	4400 5075 4325 5075
Connection ~ 4325 5075
$Comp
L Device:R_Small R4
U 1 1 61AC0081
P 5475 5275
F 0 "R4" H 5625 5300 50  0000 C CNN
F 1 "412k" H 5600 5225 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 5475 5275 50  0001 C CNN
F 3 "~" H 5475 5275 50  0001 C CNN
	1    5475 5275
	-1   0    0    1   
$EndComp
Connection ~ 5475 5075
Wire Wire Line
	5475 5075 5200 5075
NoConn ~ 5200 5175
$Comp
L Device:R_Small R5
U 1 1 61AE367A
P 5475 5475
F 0 "R5" H 5650 5500 50  0000 C CNN
F 1 "91k" H 5625 5425 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 5475 5475 50  0001 C CNN
F 3 "~" H 5475 5475 50  0001 C CNN
	1    5475 5475
	-1   0    0    1   
$EndComp
Wire Wire Line
	5200 5575 5475 5575
Wire Wire Line
	5200 5375 5475 5375
Connection ~ 5475 5375
Wire Wire Line
	5475 5175 5475 5075
$Comp
L Device:C_Small C7
U 1 1 61AF5036
P 5700 5450
F 0 "C7" H 5850 5425 50  0000 C CNN
F 1 "100uF" H 5900 5500 50  0000 C CNN
F 2 "Capacitor_SMD:C_1206_3216Metric" H 5700 5450 50  0001 C CNN
F 3 "~" H 5700 5450 50  0001 C CNN
	1    5700 5450
	1    0    0    1   
$EndComp
$Comp
L Device:C_Small C5
U 1 1 61AF5D74
P 5600 5175
F 0 "C5" H 5750 5150 50  0000 C CNN
F 1 "10nF" H 5800 5250 50  0000 C CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 5600 5175 50  0001 C CNN
F 3 "~" H 5600 5175 50  0001 C CNN
	1    5600 5175
	1    0    0    1   
$EndComp
Connection ~ 5600 5075
Wire Wire Line
	5600 5075 5475 5075
Wire Wire Line
	5475 5575 5600 5575
Wire Wire Line
	5700 5575 5700 5550
Connection ~ 5475 5575
Wire Wire Line
	5600 5275 5600 5575
Connection ~ 5600 5575
Wire Wire Line
	5600 5575 5700 5575
Wire Wire Line
	5700 5350 5700 5075
Connection ~ 5700 5075
Wire Wire Line
	5700 5075 5600 5075
Text GLabel 5950 2950 1    50   Input ~ 0
VBAT
Connection ~ 4750 7750
Wire Wire Line
	5075 7075 5075 7050
Wire Wire Line
	4750 7750 5075 7750
Wire Wire Line
	4325 4875 4325 5075
Wire Wire Line
	4850 4775 4950 4775
$Comp
L Device:C_Small C8
U 1 1 61BA52AB
P 4125 5475
F 0 "C8" H 4250 5450 50  0000 C CNN
F 1 "1uF" H 4225 5525 50  0000 C CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 4125 5475 50  0001 C CNN
F 3 "~" H 4125 5475 50  0001 C CNN
	1    4125 5475
	-1   0    0    1   
$EndComp
Wire Wire Line
	4125 5575 4325 5575
Wire Wire Line
	4125 5375 4125 4875
Wire Wire Line
	4125 4875 4325 4875
Text GLabel 4325 4875 1    50   Input ~ 0
VBAT
$Comp
L Device:C_Small C9
U 1 1 61C326D1
P 8475 5275
F 0 "C9" H 8567 5321 50  0000 L CNN
F 1 "1uF" H 8567 5230 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 8475 5275 50  0001 C CNN
F 3 "~" H 8475 5275 50  0001 C CNN
	1    8475 5275
	1    0    0    1   
$EndComp
$Comp
L Device:D_Zener_Small D4
U 1 1 619FB2B4
P 1100 6050
F 0 "D4" H 1100 6200 50  0000 L CNN
F 1 "5.1V" H 1050 6125 50  0000 L CNN
F 2 "Diode_SMD:D_SOD-523" V 1100 6050 50  0001 C CNN
F 3 "~" V 1100 6050 50  0001 C CNN
F 4 "BZT52C5V1T-TP" H 1100 6050 50  0001 C CNN "Part"
	1    1100 6050
	-1   0    0    -1  
$EndComp
Text GLabel 4350 6450 0    50   Input ~ 0
VSTOR
$Comp
L Device:C_Small C19
U 1 1 61AA952C
P 4350 6550
F 0 "C19" H 4500 6525 50  0000 C CNN
F 1 "10nF" H 4550 6625 50  0000 C CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 4350 6550 50  0001 C CNN
F 3 "~" H 4350 6550 50  0001 C CNN
	1    4350 6550
	1    0    0    1   
$EndComp
$Comp
L power:GND #PWR0105
U 1 1 61AA9FC6
P 4775 6650
F 0 "#PWR0105" H 4775 6400 50  0001 C CNN
F 1 "GND" V 4780 6522 50  0000 R CNN
F 2 "" H 4775 6650 50  0001 C CNN
F 3 "" H 4775 6650 50  0001 C CNN
	1    4775 6650
	-1   0    0    -1  
$EndComp
Wire Wire Line
	4350 6650 4775 6650
Connection ~ 4775 6650
Wire Wire Line
	4350 6450 4775 6450
$Comp
L Device:L_Small L3
U 1 1 61ADDA7B
P 1450 6200
F 0 "L3" V 1600 6200 50  0000 C CNN
F 1 "22uH" V 1525 6200 50  0000 C CNN
F 2 "Inductor_SMD:L_1210_3225Metric" H 1450 6200 50  0001 C CNN
F 3 "~" H 1450 6200 50  0001 C CNN
F 4 "CBC3225T220MR" V 1450 6200 50  0001 C CNN "Part"
	1    1450 6200
	0    -1   -1   0   
$EndComp
$Comp
L Device:C_Small C3
U 1 1 61ADDA81
P 1100 6200
F 0 "C3" V 1150 5900 50  0000 C CNN
F 1 "4.7uF" V 1075 5925 50  0000 C CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 1100 6200 50  0001 C CNN
F 3 "~" H 1100 6200 50  0001 C CNN
	1    1100 6200
	0    1    -1   0   
$EndComp
Wire Wire Line
	1350 6800 1325 6800
Wire Wire Line
	1325 6800 1300 6800
Connection ~ 1325 6800
$Comp
L Device:R_Small ROV3
U 1 1 61ADDA8A
P 1200 6800
F 0 "ROV3" V 1275 6800 50  0000 C CNN
F 1 "4.30M" V 1125 6800 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 1200 6800 50  0001 C CNN
F 3 "~" H 1200 6800 50  0001 C CNN
	1    1200 6800
	0    1    -1   0   
$EndComp
$Comp
L Device:R_Small ROV4
U 1 1 61ADDA90
P 1450 6800
F 0 "ROV4" V 1525 6800 50  0000 C CNN
F 1 "8.66M" V 1375 6800 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 1450 6800 50  0001 C CNN
F 3 "~" H 1450 6800 50  0001 C CNN
	1    1450 6800
	0    1    -1   0   
$EndComp
Text GLabel 2875 6300 2    50   Input ~ 0
VSTOR
Text Notes 1050 7025 0    50   ~ 0
VBATOV = 5.470
$Comp
L MargoGeneral:BQ25505 U5
U 1 1 61ADDAA4
P 1875 7250
F 0 "U5" H 2325 8525 50  0000 C CNN
F 1 "BQ25505" H 2325 8434 50  0000 C CNN
F 2 "Package_DFN_QFN:QFN-20-1EP_3.5x3.5mm_P0.5mm_EP2x2mm_ThermalVias" H 1875 7250 50  0001 C CNN
F 3 "" H 1875 7250 50  0001 C CNN
	1    1875 7250
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C6
U 1 1 61ADDAAA
P 1675 6300
F 0 "C6" V 1700 6100 50  0000 C CNN
F 1 "10nF" V 1625 6150 50  0000 C CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 1675 6300 50  0001 C CNN
F 3 "~" H 1675 6300 50  0001 C CNN
	1    1675 6300
	0    1    -1   0   
$EndComp
Wire Wire Line
	1350 6200 1350 6400
Wire Wire Line
	1350 6400 1775 6400
Wire Wire Line
	1550 6200 1775 6200
Connection ~ 1350 6200
Wire Wire Line
	1775 6800 1550 6800
Wire Wire Line
	1325 6900 1775 6900
Wire Wire Line
	1325 6800 1325 6900
NoConn ~ 1775 6700
NoConn ~ 1775 7000
NoConn ~ 1775 7100
$Comp
L power:GND #PWR0107
U 1 1 61ADDAC0
P 1000 7175
F 0 "#PWR0107" H 1000 6925 50  0001 C CNN
F 1 "GND" H 1005 7002 50  0000 C CNN
F 2 "" H 1000 7175 50  0001 C CNN
F 3 "" H 1000 7175 50  0001 C CNN
	1    1000 7175
	1    0    0    -1  
$EndComp
Wire Wire Line
	1000 7175 1000 6800
Wire Wire Line
	1000 6800 1100 6800
Wire Wire Line
	1000 6800 1000 6500
Wire Wire Line
	1000 6300 1575 6300
Connection ~ 1000 6800
Wire Wire Line
	1775 6500 1000 6500
Connection ~ 1000 6500
Wire Wire Line
	1000 6500 1000 6300
Connection ~ 1000 6300
Wire Wire Line
	1000 6200 1000 6300
Wire Wire Line
	1200 6200 1350 6200
Text GLabel 2875 6200 2    50   Input ~ 0
VSTOR
NoConn ~ 2875 6400
NoConn ~ 2875 6600
NoConn ~ 2875 6700
Text GLabel 2875 6500 2    50   Input ~ 0
VBAT
$Comp
L power:GND #PWR0108
U 1 1 61ADDADC
P 2875 7200
F 0 "#PWR0108" H 2875 6950 50  0001 C CNN
F 1 "GND" V 2880 7072 50  0000 R CNN
F 2 "" H 2875 7200 50  0001 C CNN
F 3 "" H 2875 7200 50  0001 C CNN
	1    2875 7200
	-1   0    0    -1  
$EndComp
Text GLabel 4300 7050 0    50   Input ~ 0
VBAT
$Comp
L Device:C_Small C15
U 1 1 61B114B5
P 4300 7275
F 0 "C15" H 4500 7250 50  0000 C CNN
F 1 "10nF" H 4550 7350 50  0000 C CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 4300 7275 50  0001 C CNN
F 3 "~" H 4300 7275 50  0001 C CNN
	1    4300 7275
	1    0    0    1   
$EndComp
Text Notes 5325 4975 0    50   ~ 0
VOUT = 2.76
$Comp
L Device:C_Small C2
U 1 1 61A389F9
P 4775 6550
F 0 "C2" H 4925 6525 50  0000 C CNN
F 1 "4.7uF" H 4975 6600 50  0000 C CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 4775 6550 50  0001 C CNN
F 3 "~" H 4775 6550 50  0001 C CNN
	1    4775 6550
	1    0    0    1   
$EndComp
Wire Wire Line
	4425 7050 5075 7050
Text GLabel 5025 7375 0    50   Input ~ 0
VBATMID
Wire Wire Line
	5075 7375 5025 7375
Connection ~ 5075 7375
Text GLabel 5250 2950 1    50   Input ~ 0
VBATMID
Wire Wire Line
	4975 3150 5250 3150
Connection ~ 5250 3350
$Comp
L power:GND #PWR0103
U 1 1 61A888C0
P 5250 3350
F 0 "#PWR0103" H 5250 3100 50  0001 C CNN
F 1 "GND" H 5255 3177 50  0000 C CNN
F 2 "" H 5250 3350 50  0001 C CNN
F 3 "" H 5250 3350 50  0001 C CNN
	1    5250 3350
	1    0    0    -1  
$EndComp
Wire Wire Line
	4975 3350 5250 3350
$Comp
L Device:R_Small R6
U 1 1 61A888C7
P 5250 3250
F 0 "R6" H 5350 3200 50  0000 C CNN
F 1 "1M" H 5375 3275 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 5250 3250 50  0001 C CNN
F 3 "~" H 5250 3250 50  0001 C CNN
	1    5250 3250
	1    0    0    1   
$EndComp
$Comp
L Device:C_Small C16
U 1 1 61A888CD
P 4975 3250
F 0 "C16" H 4725 3200 50  0000 C CNN
F 1 "10nF" H 4725 3300 50  0000 C CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 4975 3250 50  0001 C CNN
F 3 "~" H 4975 3250 50  0001 C CNN
	1    4975 3250
	1    0    0    1   
$EndComp
Text GLabel 4975 3150 1    50   Input ~ 0
VBATMID_SENSE
Connection ~ 5250 3150
$Comp
L Device:R_Small R2
U 1 1 61A888D5
P 5250 3050
F 0 "R2" H 5350 3000 50  0000 C CNN
F 1 "1M" H 5375 3075 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 5250 3050 50  0001 C CNN
F 3 "~" H 5250 3050 50  0001 C CNN
	1    5250 3050
	1    0    0    1   
$EndComp
Wire Notes Line
	9275 6750 9275 475 
Wire Notes Line
	475  4075 9275 4075
Text Notes 9475 925  0    197  ~ 39
MCU+BLE
Text Notes 5500 4475 0    197  ~ 39
POWER
Text Notes 7400 975  0    197  ~ 39
ACCEL
Text Notes 5025 975  0    197  ~ 39
ANALOG
Text Notes 1975 900  0    197  ~ 39
GPS
Wire Notes Line
	4200 475  4200 4075
Wire Notes Line
	6900 475  6750 475 
Connection ~ 5200 5575
Wire Wire Line
	2075 2275 2075 1425
Wire Wire Line
	2075 1425 2175 1425
Wire Wire Line
	2175 2275 2175 1425
Text GLabel 2275 2275 1    50   Input ~ 0
VDD
Wire Wire Line
	1425 1825 1900 1825
Wire Wire Line
	1900 1825 1900 1625
Connection ~ 1425 1825
Text GLabel 12350 975  2    50   Input ~ 0
VDD
Connection ~ 12350 1175
Connection ~ 12350 1075
Wire Wire Line
	12350 1275 12350 1175
Wire Wire Line
	12350 1175 12350 1075
Wire Wire Line
	12350 975  12350 1075
$Comp
L power:GND #PWR0124
U 1 1 61AAD4B6
P 12350 3375
F 0 "#PWR0124" H 12350 3125 50  0001 C CNN
F 1 "GND" H 12355 3202 50  0000 C CNN
F 2 "" H 12350 3375 50  0001 C CNN
F 3 "" H 12350 3375 50  0001 C CNN
	1    12350 3375
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR0123
U 1 1 61AAC16D
P 12350 3775
F 0 "#PWR0123" H 12350 3525 50  0001 C CNN
F 1 "GND" H 12355 3602 50  0000 C CNN
F 2 "" H 12350 3775 50  0001 C CNN
F 3 "" H 12350 3775 50  0001 C CNN
	1    12350 3775
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR0122
U 1 1 61AA65B3
P 10750 4775
F 0 "#PWR0122" H 10750 4525 50  0001 C CNN
F 1 "GND" H 10755 4602 50  0000 C CNN
F 2 "" H 10750 4775 50  0001 C CNN
F 3 "" H 10750 4775 50  0001 C CNN
	1    10750 4775
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR0120
U 1 1 61AA5805
P 12350 4275
F 0 "#PWR0120" H 12350 4025 50  0001 C CNN
F 1 "GND" H 12355 4102 50  0000 C CNN
F 2 "" H 12350 4275 50  0001 C CNN
F 3 "" H 12350 4275 50  0001 C CNN
	1    12350 4275
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR0119
U 1 1 61AA55E8
P 12350 4175
F 0 "#PWR0119" H 12350 3925 50  0001 C CNN
F 1 "GND" H 12355 4002 50  0000 C CNN
F 2 "" H 12350 4175 50  0001 C CNN
F 3 "" H 12350 4175 50  0001 C CNN
	1    12350 4175
	0    -1   -1   0   
$EndComp
Text GLabel 10750 3175 0    50   Input ~ 0
VBATMID_SENSE
$Comp
L Device:LED D2
U 1 1 61AE613D
P 9675 3725
F 0 "D2" V 9700 3850 50  0000 C CNN
F 1 "LED" V 9625 3875 50  0000 C CNN
F 2 "Diode_SMD:D_0805_2012Metric" H 9675 3725 50  0001 C CNN
F 3 "~" H 9675 3725 50  0001 C CNN
F 4 "LS R976-NR-1" H 9675 3725 50  0001 C CNN "Part"
	1    9675 3725
	0    1    -1   0   
$EndComp
$Comp
L power:GND #PWR0115
U 1 1 61BCA035
P 12350 4075
F 0 "#PWR0115" H 12350 3825 50  0001 C CNN
F 1 "GND" H 12355 3902 50  0000 C CNN
F 2 "" H 12350 4075 50  0001 C CNN
F 3 "" H 12350 4075 50  0001 C CNN
	1    12350 4075
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR0114
U 1 1 61BC97E0
P 12350 3975
F 0 "#PWR0114" H 12350 3725 50  0001 C CNN
F 1 "GND" H 12355 3802 50  0000 C CNN
F 2 "" H 12350 3975 50  0001 C CNN
F 3 "" H 12350 3975 50  0001 C CNN
	1    12350 3975
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR0113
U 1 1 61B24B1B
P 12350 3575
F 0 "#PWR0113" H 12350 3325 50  0001 C CNN
F 1 "GND" H 12355 3402 50  0000 C CNN
F 2 "" H 12350 3575 50  0001 C CNN
F 3 "" H 12350 3575 50  0001 C CNN
	1    12350 3575
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR0112
U 1 1 61A66994
P 12350 1675
F 0 "#PWR0112" H 12350 1425 50  0001 C CNN
F 1 "GND" H 12355 1502 50  0000 C CNN
F 2 "" H 12350 1675 50  0001 C CNN
F 3 "" H 12350 1675 50  0001 C CNN
	1    12350 1675
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR0111
U 1 1 61A659A2
P 10750 1575
F 0 "#PWR0111" H 10750 1325 50  0001 C CNN
F 1 "GND" H 10755 1402 50  0000 C CNN
F 2 "" H 10750 1575 50  0001 C CNN
F 3 "" H 10750 1575 50  0001 C CNN
	1    10750 1575
	0    1    1    0   
$EndComp
NoConn ~ 10750 2375
Text GLabel 10750 1775 0    50   Input ~ 0
RESET
Text GLabel 10175 2375 2    50   Input ~ 0
RESET
NoConn ~ 12350 5475
NoConn ~ 10750 2175
Wire Wire Line
	10550 2275 10750 2275
Wire Wire Line
	10550 2175 10550 2275
Wire Wire Line
	10175 2175 10550 2175
Wire Wire Line
	10175 2075 10750 2075
NoConn ~ 12350 1475
Wire Wire Line
	12350 6375 12350 6275
Connection ~ 12350 6375
Wire Wire Line
	12350 6275 12350 6175
Connection ~ 12350 6275
Wire Wire Line
	12350 6175 12350 6075
Connection ~ 12350 6175
Wire Wire Line
	12350 6075 12350 5975
Connection ~ 12350 6075
Wire Wire Line
	12350 5975 12350 5875
Connection ~ 12350 5975
Connection ~ 12350 6475
Wire Wire Line
	12350 6475 12350 6375
$Comp
L power:GND #PWR0110
U 1 1 619EE70F
P 12350 6475
F 0 "#PWR0110" H 12350 6225 50  0001 C CNN
F 1 "GND" H 12355 6302 50  0000 C CNN
F 2 "" H 12350 6475 50  0001 C CNN
F 3 "" H 12350 6475 50  0001 C CNN
	1    12350 6475
	1    0    0    -1  
$EndComp
$Comp
L SolarGPS-rescue:STM32WB5MMGH6TR-VeridianDesigns U4
U 1 1 619E1AFB
P 11550 3675
F 0 "U4" H 11550 6642 50  0000 C CNN
F 1 "STM32WB5MMGH6TR" H 11550 6551 50  0000 C CNN
F 2 "Veridian-Library-KiCad:XCVR_STM32WB5MMGH6TR" H 11550 3675 50  0001 L BNN
F 3 "" H 11550 3675 50  0001 L BNN
F 4 "1.428mm" H 11550 3675 50  0001 L BNN "MAXIMUM_PACKAGE_HEIGHT"
F 5 "Rev 1" H 11550 3675 50  0001 L BNN "PARTREV"
F 6 "Manufacturer recommendations" H 11550 3675 50  0001 L BNN "STANDARD"
F 7 "STMicroelectronics" H 11550 3675 50  0001 L BNN "MANUFACTURER"
	1    11550 3675
	1    0    0    -1  
$EndComp
Text GLabel 10750 2875 0    50   Input ~ 0
SOLAR_SENSE2
Text GLabel 12350 2675 2    50   Input ~ 0
GPSEN
Text GLabel 10750 5275 0    50   Input ~ 0
DISP_EN
Text GLabel 12350 1975 2    50   Input ~ 0
DISP_CS
Text GLabel 12350 2475 2    50   Input ~ 0
GPS_UART_MISO
Text Label 10175 2175 0    50   ~ 0
SWO
Text Label 10175 2075 0    50   ~ 0
SWDCLK
Text Label 10175 1975 0    50   ~ 0
SWDIO
NoConn ~ 10175 2275
NoConn ~ 9675 2275
Wire Wire Line
	10175 1975 10750 1975
Text GLabel 10750 4475 0    50   Input ~ 0
LED
Text GLabel 9675 3375 1    50   Input ~ 0
LED
$Comp
L Device:R_Small R3
U 1 1 61AF4E77
P 9675 3475
F 0 "R3" H 9550 3500 50  0000 C CNN
F 1 "390" H 9550 3425 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 9675 3475 50  0001 C CNN
F 3 "~" H 9675 3475 50  0001 C CNN
	1    9675 3475
	-1   0    0    -1  
$EndComp
Wire Wire Line
	9675 3950 9675 3875
$Comp
L power:GND #PWR0102
U 1 1 61AED265
P 9675 3950
F 0 "#PWR0102" H 9675 3700 50  0001 C CNN
F 1 "GND" H 9680 3777 50  0000 C CNN
F 2 "" H 9675 3950 50  0001 C CNN
F 3 "" H 9675 3950 50  0001 C CNN
	1    9675 3950
	-1   0    0    -1  
$EndComp
Connection ~ 9600 2575
Connection ~ 9600 2375
Wire Wire Line
	9600 2575 9600 2375
Wire Wire Line
	10175 2575 9600 2575
$Comp
L power:GND #PWR07
U 1 1 61CDC430
P 9600 2575
F 0 "#PWR07" H 9600 2325 50  0001 C CNN
F 1 "GND" H 9605 2402 50  0000 C CNN
F 2 "" H 9600 2575 50  0001 C CNN
F 3 "" H 9600 2575 50  0001 C CNN
	1    9600 2575
	1    0    0    -1  
$EndComp
$Comp
L Device:C_Small C18
U 1 1 61CDBAE3
P 10175 2475
F 0 "C18" H 10275 2500 50  0000 L CNN
F 1 "10nF" H 10267 2430 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 10175 2475 50  0001 C CNN
F 3 "~" H 10175 2475 50  0001 C CNN
	1    10175 2475
	1    0    0    -1  
$EndComp
Text GLabel 12350 1875 2    50   Input ~ 0
EXTCOMIN
Text GLabel 10750 2975 0    50   Input ~ 0
SOLAR_SENSE1
Text GLabel 10750 3075 0    50   Input ~ 0
VBAT_SENSE
Text GLabel 10750 3575 0    50   Input ~ 0
GPS_EXINT
Text GLabel 12350 2575 2    50   Input ~ 0
GPS_UART_MOSI
Wire Wire Line
	9675 2375 9600 2375
Wire Wire Line
	9600 2175 9600 2375
Connection ~ 9600 2175
Wire Wire Line
	9675 2175 9600 2175
Wire Wire Line
	9600 2075 9600 2175
Wire Wire Line
	9675 2075 9600 2075
$Comp
L Connector_Generic:Conn_02x05_Odd_Even J4
U 1 1 60185530
P 9875 2175
F 0 "J4" H 9925 2650 50  0000 C CNN
F 1 "Conn_02x05_Odd_Even" H 9925 2575 50  0000 C CNN
F 2 "Connector_PinHeader_2.00mm:PinHeader_2x05_P2.00mm_Vertical_SMD" H 9875 2175 50  0001 C CNN
F 3 "~" H 9875 2175 50  0001 C CNN
F 4 "PM20C05VBDN" H 9875 2175 50  0001 C CNN "Part"
	1    9875 2175
	1    0    0    -1  
$EndComp
Text GLabel 9675 1975 0    50   Input ~ 0
VDD
$Comp
L Connector:Screw_Terminal_01x01 J7
U 1 1 61DB9C44
P 7875 7175
F 0 "J7" V 7839 7087 50  0000 R CNN
F 1 "Screw_Terminal_01x01" V 7748 7087 50  0000 R CNN
F 2 "Veridian-Library-KiCad:Right Angle Screw Terminal M3M4" H 7875 7175 50  0001 C CNN
F 3 "~" H 7875 7175 50  0001 C CNN
F 4 "7781" V 7875 7175 50  0001 C CNN "Part"
F 5 "Keystone Electronics" V 7875 7175 50  0001 C CNN "Manufacture"
	1    7875 7175
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR0125
U 1 1 61DC7BBC
P 7875 7375
F 0 "#PWR0125" H 7875 7125 50  0001 C CNN
F 1 "GND" H 7880 7202 50  0000 C CNN
F 2 "" H 7875 7375 50  0001 C CNN
F 3 "" H 7875 7375 50  0001 C CNN
	1    7875 7375
	-1   0    0    -1  
$EndComp
Wire Wire Line
	1900 1425 2075 1425
$Comp
L Device:C_Small C25
U 1 1 61A1AA0E
P 12825 1075
F 0 "C25" H 12925 1100 50  0000 L CNN
F 1 "1uF" H 12917 1030 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 12825 1075 50  0001 C CNN
F 3 "~" H 12825 1075 50  0001 C CNN
	1    12825 1075
	1    0    0    -1  
$EndComp
Text GLabel 12825 975  1    50   Input ~ 0
VDD
$Comp
L power:GND #PWR01
U 1 1 61A1BA62
P 12825 1175
F 0 "#PWR01" H 12825 925 50  0001 C CNN
F 1 "GND" H 12830 1002 50  0000 C CNN
F 2 "" H 12825 1175 50  0001 C CNN
F 3 "" H 12825 1175 50  0001 C CNN
	1    12825 1175
	1    0    0    -1  
$EndComp
Text GLabel 7450 3000 0    50   Input ~ 0
SPI_CLK
Text GLabel 12350 2975 2    50   Input ~ 0
SPI_MOSI
Text GLabel 7450 2800 0    50   Input ~ 0
SPI_MISO
Text GLabel 12350 2875 2    50   Input ~ 0
SPI_MISO
Text GLabel 7450 2900 0    50   Input ~ 0
SPI_MOSI
NoConn ~ 8450 2700
NoConn ~ 8450 2800
NoConn ~ 8450 2900
Text GLabel 7450 3100 0    50   Input ~ 0
LIS_CS
Text GLabel 10750 3675 0    50   Input ~ 0
LIS_CS
Text GLabel 8050 2300 1    50   Input ~ 0
VDD
Wire Wire Line
	8050 2300 7850 2300
$Comp
L Device:C_Small C26
U 1 1 61A8ADD1
P 8650 2400
F 0 "C26" H 8742 2446 50  0000 L CNN
F 1 "1uF" H 8742 2355 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 8650 2400 50  0001 C CNN
F 3 "~" H 8650 2400 50  0001 C CNN
	1    8650 2400
	1    0    0    1   
$EndComp
$Comp
L power:GND #PWR0126
U 1 1 61A8B0D8
P 7950 3400
F 0 "#PWR0126" H 7950 3150 50  0001 C CNN
F 1 "GND" H 7955 3227 50  0000 C CNN
F 2 "" H 7950 3400 50  0001 C CNN
F 3 "" H 7950 3400 50  0001 C CNN
	1    7950 3400
	1    0    0    -1  
$EndComp
Text GLabel 8650 2300 1    50   Input ~ 0
VDD
$Comp
L power:GND #PWR0127
U 1 1 61A8B666
P 8650 2500
F 0 "#PWR0127" H 8650 2250 50  0001 C CNN
F 1 "GND" H 8655 2327 50  0000 C CNN
F 2 "" H 8650 2500 50  0001 C CNN
F 3 "" H 8650 2500 50  0001 C CNN
	1    8650 2500
	1    0    0    -1  
$EndComp
NoConn ~ 7450 2700
Text GLabel 7450 2600 0    50   Input ~ 0
LIS_INT
$Comp
L Sensor_Motion:LIS3DH U6
U 1 1 61A5A316
P 7950 2800
F 0 "U6" H 8125 2175 50  0000 C CNN
F 1 "LIS3DH" H 8225 2250 50  0000 C CNN
F 2 "Package_LGA:LGA-16_3x3mm_P0.5mm_LayoutBorder3x5y" H 8050 1750 50  0001 C CNN
F 3 "https://www.st.com/resource/en/datasheet/cd00274221.pdf" H 7750 2700 50  0001 C CNN
	1    7950 2800
	1    0    0    -1  
$EndComp
Text GLabel 10750 3775 0    50   Input ~ 0
LIS_INT
Text GLabel 12350 3275 2    50   Input ~ 0
SPI_CLK
Text GLabel 8775 5175 1    50   Input ~ 0
DISP_EN
$Comp
L power:GND #PWR0128
U 1 1 61C00045
P 8775 5550
F 0 "#PWR0128" H 8775 5300 50  0001 C CNN
F 1 "GND" H 8780 5377 50  0000 C CNN
F 2 "" H 8775 5550 50  0001 C CNN
F 3 "" H 8775 5550 50  0001 C CNN
	1    8775 5550
	1    0    0    -1  
$EndComp
Text GLabel 8475 5175 1    50   Input ~ 0
VDD
Wire Wire Line
	8775 5375 8775 5550
Connection ~ 8300 5375
Wire Wire Line
	8300 5375 8475 5375
Connection ~ 8775 5375
Connection ~ 8475 5375
Wire Wire Line
	8475 5375 8775 5375
Text Notes 7650 4575 0    197  ~ 39
DISP
Wire Notes Line
	6750 475  6750 8025
Text Notes 7075 6600 0    197  ~ 39
MECHANICAL
Wire Notes Line
	9275 6125 6725 6125
$Comp
L power:GND #PWR0116
U 1 1 61B71588
P 12350 2075
F 0 "#PWR0116" H 12350 1825 50  0001 C CNN
F 1 "GND" H 12355 1902 50  0000 C CNN
F 2 "" H 12350 2075 50  0001 C CNN
F 3 "" H 12350 2075 50  0001 C CNN
	1    12350 2075
	0    -1   -1   0   
$EndComp
Wire Wire Line
	1200 6050 1350 6050
Connection ~ 1350 6050
Wire Wire Line
	1350 6050 1350 6200
Wire Wire Line
	1000 6050 1000 6200
Connection ~ 1000 6200
$Comp
L power:GND #PWR0104
U 1 1 61AB92EC
P 2175 5425
F 0 "#PWR0104" H 2175 5175 50  0001 C CNN
F 1 "GND" H 2180 5252 50  0000 C CNN
F 2 "" H 2175 5425 50  0001 C CNN
F 3 "" H 2175 5425 50  0001 C CNN
	1    2175 5425
	-1   0    0    -1  
$EndComp
$Comp
L power:GND #PWR0106
U 1 1 61AB97F7
P 2175 4900
F 0 "#PWR0106" H 2175 4650 50  0001 C CNN
F 1 "GND" H 2180 4727 50  0000 C CNN
F 2 "" H 2175 4900 50  0001 C CNN
F 3 "" H 2175 4900 50  0001 C CNN
	1    2175 4900
	-1   0    0    -1  
$EndComp
$Comp
L Device:D_Schottky_Small D1
U 1 1 61ABA4BC
P 1725 4900
F 0 "D1" H 1725 5000 50  0000 C CNN
F 1 "Schottky" H 1625 5075 50  0000 C CNN
F 2 "Diode_SMD:D_SOD-323_HandSoldering" V 1725 4900 50  0001 C CNN
F 3 "~" V 1725 4900 50  0001 C CNN
F 4 "B0530WS" H 1725 4900 50  0001 C CNN "Part"
	1    1725 4900
	1    0    0    1   
$EndComp
$Comp
L Device:D_Schottky_Small D3
U 1 1 61AC143B
P 1725 5425
F 0 "D3" H 1725 5525 50  0000 C CNN
F 1 "Schottky" H 1625 5600 50  0000 C CNN
F 2 "Diode_SMD:D_SOD-323_HandSoldering" V 1725 5425 50  0001 C CNN
F 3 "~" V 1725 5425 50  0001 C CNN
F 4 "B0530WS" H 1725 5425 50  0001 C CNN "Part"
	1    1725 5425
	1    0    0    1   
$EndComp
Wire Wire Line
	1625 5425 1350 5425
Wire Wire Line
	1350 5425 1350 6050
Wire Wire Line
	1625 4900 1350 4900
Wire Wire Line
	1350 4900 1350 5425
Connection ~ 1350 5425
Text Label 1350 6050 1    50   ~ 0
SOLAR+
$Comp
L Device:C_Small C1
U 1 1 61AA34D0
P 3200 3075
F 0 "C1" H 3000 3050 50  0000 C CNN
F 1 "3.3pF" H 3000 3125 50  0000 C CNN
F 2 "Capacitor_SMD:C_0402_1005Metric" H 3200 3075 50  0001 C CNN
F 3 "~" H 3200 3075 50  0001 C CNN
	1    3200 3075
	1    0    0    1   
$EndComp
Connection ~ 2075 1425
Connection ~ 3200 2975
Wire Wire Line
	3200 2975 3350 2975
Wire Wire Line
	3200 3175 3450 3175
Wire Wire Line
	3450 3175 3450 2975
Connection ~ 3450 3175
$EndSCHEMATC
