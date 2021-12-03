# Veridian-Heliograph
## What is it?
A hardy solar powered GPS tracker. Born from a need to always have a GPS device that can run for decades without any lithium batteries.
Using 2 independent solar charging circuits the Heliograph seeks to always provide a GPS Lat/Long position for the most travellers.
The whole device is cast in water clear epoxy, no ports, no holes, no batteries. Just a robust device that will provide a GPS position whenever outside.

## Who wants it?
- Offshore Nearshore Sailors
- Outdoorsmen
- Overlanders

## Expected life
TBD

## Tech Features
- Duel MPPT solar chargers feed from high effiency solar panels
- 10F of supercap energy storage
- TI nano-power Buck-Boost
- UBlox Ultra low power M10 GPS processor
- STM32WB5MM Module for processing and BLE 5.0
- 3 Axis accelerometer
- Sharp Sunreadable Memory Display
- Maxtena Helio-core GPS antenna

## POWER SYSTEM
The power system comprises of 2 high effiency solar panels at 120 degress.  With this approach I hope the product design will always have access to some light to charge the super caps. Incomoing power from the solar panels is feed through individual low drop shotcky diodes. I had considered using the MAX40200 superdiode however the high turn on voltage means no charging until ~1.6V. With the diodes we lose 300mV which is acceptable. In a no cost consideration scenario I would use one of the BQ25505 MPPT chargers for each panel. The BQ25505 is a super awesome MPPT charger that operates between 0 and 5.5V boost. I found it supiror to all other low voltage MPPT chargers and the market is slim! This chargers feeds into 2 series Supercaps at 10F that should hold plenty of charge for severa fixes without sun.

The Supercaps are feed into Ti TPS63802 which provides buck-boost topography from the supercaps. Effectively allowing maximum use of the supercaps linear charge. 1.3 - 5.5V should be supported. Ti does also make an integrated BQ25570 MPPT with buck however I wanted to maximize the super caps voltage range.

VDD was selected at 2.7 volts as this was the minimum for the Sharp Memory LCD and the uBlox M10 GNSS chip. The M10 chip does also have a 1.8V system however we are still limited by the LCD. It might be possible to add a level converter however that quickly increases our BOM. The Buck-Boost seems like a good balence of low BOM and high effiency.