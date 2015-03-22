

# Supported Hardware

This library has been designed to support the DOGM128 Graphics LCD for the [Arduino](http://www.arduino.cc) Board.
With version 1.03 DOGS102 and DOGM132 LCDs are also supported:

  * [DOGM128 with Arduino Hardware](dogm128_arduino_hardware.md)
  * [DOGS102 with Arduino Hardware](dogs102_arduino_hardware.md)
  * [DOGM132 with Arduino Hardware](dogm132_arduino_hardware.md)
  * [DOGXL160](dogxl160.md)

Additionally this library has examples for using ATTINY and ATMEGA controllers without Arduino environment:

  * [DOGM132 with ATTINY84 Hardware](dogm132_attiny84_hardware.md) (Version 1.04 and above)
  * [DOGM132 with ATMEGA88 Hardware](dogm132_atmega88_hardware.md) (Version 1.07 and above)

# Shields

DOG-S and DOG-M shields are available for the Arduino environment:

  * [DOGS102 Shield](dogs102_arduino_shield.md) (Project Wiki), [DOGS102 Shield](http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1285280487/2) (External Link)
  * [DOGS102 Shield with SPI Keypad](dogs102_spi_keypad.md) (Project Wiki)
  * [DOGM128 Shield (Creative Common)](dogm128_arduino_shield.md)
  * [DOGM132 Shield (Creative Common)](dogm132_arduino_shield.md)


# Hardware and Software Overview

## Hardware Environment and Displays

| Display          | DOGM128 | DOGM132 | DOGS102 | DOGXL160 | DOGXL160 |
|:-----------------|:--------|:--------|:--------|:---------|:---------|
| avr-gcc define   | DOGM128\_HW | DOGM132\_HW | DOGS102\_HW | DOGXL160\_HW\_BW | DOGXL160\_HW\_GR |
| Arduino Duemilanove | p, t     | p, t    |   p, t  |   p      |  p       |
| Arduino Uno      | p, t     | p, t    |   p     |    p, t  |   p, t   |
| ATTINY84 (Universal Serial Interface) | p        | p, t    |   p     |
| ATMEGA88 (SPI)   | p        | p, t    |   p     |   p, t   |    p, t  |

  * p: The library has been ported.
  * t: The library has been successfully tested.
  * ATTINY port is available with version v1.04 of the dogm128 library.
  * ATMEGA port is available with version v1.07 of the dogm128 library.
  * DOGXL160 port is available with version v1.08 of the dogm128 library.

## Example Programs and Displays

| Example       | DOGM128 | DOGM132 | DOGS102 | DOGXL160 |
|:--------------|:--------|:--------|:--------|:---------|
| Breakorino    |   s,t   |         |   s,t   |   s,t    |
| Fonts         |   s,t   |   s,t   |   s,t   |   s,t    |
| Walk          |   s,t   |   s,t   |   s,t   |   s,t    |
| Graphics Test |   s,t   |   s,t   |   s,t   |   s,t    |
| Space Trash   |   s,t   |   s,t   |   s,t   |   s,t    |
| Stars         |   s,t   |   s,t   |   s,t   |   s,t    |

  * s: The example supports the display.
  * t: The example has been successfully tested with the display.

## Example Programs and Memory Usage

  * RAM requirements also depend on the display (e.g. DOGM132 requires 4 bytes more than the DOGM128).
  * Values are provided for ATTINY

| Example       | Flash ROM | RAM |
|:--------------|:----------|:----|
| Fonts         | 8 K       | 0.25 K |
| Walk          | 2 K       | 0.25 K |
| Graphics Test | 5 K       | 0.25 K |
| Space Trash   | 8 K        | 0.5 K |



# Links

  * [Wiki](dogm128.md)
  * [Project Home](http://code.google.com/p/dogm128/)
  * [Browse Source](http://code.google.com/p/dogm128/source/browse/)