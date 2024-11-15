# Daikin AVR Remote System

Planning on using this repo to make a system to turn on Daikin ACs in my home remotely.

I will have a web interface that interacts with a system of 3 ATMega1284 microcontrollers that are

placed in front of the ACs.

### Reference help

- Someone reverse engineered the [Daikin remote protocol](https://github.com/blafois/Daikin-IR-Reverse). 
This helped me figure how to form the payload I need to send to the Daikin AC.
- [ATMega1284 Datasheet](http://ww1.microchip.com/downloads/en/devicedoc/atmel-42718-atmega1284_datasheet.pdf)
