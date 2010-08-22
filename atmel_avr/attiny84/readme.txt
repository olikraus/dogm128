
2010-08-22 olikraus@gmail.com

This directory contains the examples of the dogm128 library for ATTINY84.
These examples do not need the Arduino environment. All examples can
be flashed on the target device without additional operating system.

Requirements and assumptions:
- avr-gcc toolchain
- gnumake
- avrdude
- AVR ISP mkII

Display device:
All examples use the DOG132 graphics display. This can be changed
by assigning a different display in the Makefile.
Supported defines are
  DOGM128_HW
  DOGM132_HW
  DOGS102_HW
  


