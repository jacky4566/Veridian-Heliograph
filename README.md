# Veridian-Heliograph
## What is it?
A hardy solar powered giver of GPS XYZ. Born from a need to always have a GPS device that can run for decades without any lithium batteries.
The Heliograph seeks to always provide a GPS Lat/Long position for the most severe travelers.
The whole device is cast in water clear epoxy, no ports, no holes, no batteries. Just a robust device that will provide a GPS position whenever outside.
A single M4 thread is provided for keychain or mounting on your favorite motorbike.

## Who wants it?
- Offshore Sailors
- Motorbikers
- Outdoorsmen
- Overlanders
- GNSS nerds

## Where to get it?
Tindy, Link TBD

## Expected life
TBD

## Tech Features
- nA BQ25505 MPPT solar charger fed from 2x high efficiency solar panels 
- TI nA Buck-Boost TPS63802
- 5.5V 10F of supercap energy storage
- UBlox Ultra low power M10 GPS processor
- STM32WB5MM Module for processing and BLE 5.0
- 3 Axis accelerometer with tap input, LIS3DH
- Sharp Sun readable Memory Display
- M4 screw thread for keychain or mounting

# Development and Details

## POWER SYSTEM
The power system comprises of 2 high efficiency solar panels at 120 degrees.  With this approach I hope the product design will always have access to some light to charge the super caps. Incoming power from the solar panels is feed through individual low drop shotcky diodes. With the diodes we loose ~200mV which is acceptable. In a no cost consideration scenario I would use one of the BQ25505 MPPT chargers for each panel. The BQ25505 is a super awesome MPPT charger that operates between 0 and 5.5V boost. I found it superior to all other low voltage MPPT chargers and the market is slim! This chargers feeds into 2 series Supercaps at 10F that should hold plenty of charge for many fixes without sun.

The Supercaps are feed into Ti TPS63802 which provides buck-boost topography from the supercaps. Effectively allowing maximum use of the supercaps linear charge. 1.3 - 5.5V should be supported. Ti does also make an integrated BQ25570 MPPT with buck however I wanted to maximize the super caps voltage range.

VDD was selected at 2.8 volts as this was the minimum for the Sharp Memory LCD and the uBlox M10 GNSS chip. The M10 chip does also have a 1.8V version however we are still limited by the LCD. It might be possible to add a level converter however that quickly increases our BOM. The Buck-Boost seems like a good balance of low BOM and high efficiency.

## FIRMWARE
The firmware was written with the help of STM32CUBEIDE. The CubeIDE is pretty great for pre-generated code however some aspects can be hard to figure out for a novice. Since the BLE code could already be generated by the IDE most of my work was focused on the board level integration. Reading the sensors and putting that data into a nice GUI.
Firmware Features:
-	32MHz CPU speed coupled with 32.768kHz real time clock. The STM32WB5MM integrates crystals for both these features.
-	BLE for OTA updates, Battery status, and waypoint setting. Further sensors could be integrated as a weather station perhaps?
-	ADC 1Hz with DMA functionality to gather voltages. Supercap 1 & 2, solar voltage.
-	Display updating
-	GNSS processing using the Ublox PVT message. Less bytes to process compared to traditional NEMA messages. Super-e ultra low power tracking is set along side a software standby.
-	Accelerometer processing and Tap inputs

Some programming challenges I faced with my fight against CubeIDE:
-	Integrating OTA functionality. It would be nice if STM tells you they already have a pre-compiled OTA application. Some struggle with linking the proper startup addresses were also found. 
-	Setting up the ADC DMA with proper callbacks. Some of the HAL calls need to be just right for the desired callbacks.
-	Learning the STM32WB sequencer and Low Power Manager. Little documentation is available on these systems. However they are quite powerful and basically act as a dumb RTOS for scheduling tasks with low power in mind. 

## Housing
The Heliograph is essential without a housing. A basic model was 3D printed, then a reverse mold was made in rubber so the PCB and solar panels could be placed in a cavity and cast. Further development to come here. 

