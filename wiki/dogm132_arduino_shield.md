German users will find a complete description in their language [here](http://www.schmelle2.de/wp/arduino/dogm132-shield).


# Introduction
To make it easier for you to perform your first steps with the DOGM-Library and the display type we have developed a DOGM132-Shield for you.
**Please notice:** This DOGM132-Shield is only designed to use with the Arduino Duemilanove and Uno. The Deciemila Board hasn't been tested.

# Details
In this version of the DOGM132-Shield the display is using both power supplies provided by the Arduino Duemilanove/Uno board. The 5V power supply is used for the LED background lighting and the 3,3V power supply provided by the FTDI-Chip/Power regulator is used for the display itself.
The value of resistor **`R2`** depends on your LED background lighting. Please consult the datasheet for the correct value.

Additionally the PCB contains 4 momentary switches an 2 potentiometers.

### For german developers
All electronic parts are delivered in Germany by **Reichelt Elektronik**, **Conrad Electronic** and **Elektronikladen Detmold**.
### Bill of material
**Reichelt Elektronik**
| **Quantity** | **Partnumber** | **Description** |
|:-------------|:---------------|:----------------|
| 1            | EA DOGM132?-6  | EA DOGM Display (? = depends on colour) |
| 1            | EA LED55X31-?  | EA DOGM Background Lighting (? = depends on colour) |
| 1            | 1/4W 10K       | Resistor 10K 1/4W |
| 1            | Metall ?       | Metal Resistor 1/4W (? = Value depends on LED background lighting. See datasheet for more information) |
| 2            | Z5U 100N       | Capacitor 100nF 5mm Grid |
| 9            | Z5U 1,0u       | Capacitor 1,0uF 5mm Grid |
| 1            | 74HC 4050      | 6x Driver IC (please use no socket!) |
| 4            | Taster 9303    | Momentary switch 6x6mm, 7mm height |
| 2            | RK09K113-LIN50K | ALPS Potentiometer 50KΩ, vertical mounting, 6mm axis |

**Conrad Electronic**
| **Quantity** | **Partnumber** | **Description** |
|:-------------|:---------------|:----------------|
| 2            | 741401         |  Female Connector, Single Row, 20-Pin, 2,54mm Spacing |

**Elektronikladen Mikrocomputer GmbH, Detmold**
| **Quantity** | **Partnumber** | **Description** |
|:-------------|:---------------|:----------------|
| 1            | SFE10007       | Arduino Stackable Header Kit (Sparkfun: PRT-10007) |

# Download

Eagle schematic and board files are provided under the terms of the
[cc-by-sa-3.0](http://creativecommons.org/licenses/by-sa/3.0/deed.de) license:
  * [Eagle schematic file](http://wiki.dogm128.googlecode.com/hg/ps/DOGM132/dogm132_shield.sch)
  * [Eagle board file](http://wiki.dogm128.googlecode.com/hg/ps/DOGM132/dogm132_shield.brd)

# Future plans

None at the moment

# Pictures
![http://wiki.dogm128.googlecode.com/hg/ps/DOGM132/DOGM132-PCB.jpg](http://wiki.dogm128.googlecode.com/hg/ps/DOGM132/DOGM132-PCB.jpg)
![http://wiki.dogm128.googlecode.com/hg/ps/DOGM132/DOGM132-Natural.jpg](http://wiki.dogm128.googlecode.com/hg/ps/DOGM132/DOGM132-Natural.jpg)
![http://wiki.dogm128.googlecode.com/hg/ps/DOGM132/DOGM132_Action.jpg](http://wiki.dogm128.googlecode.com/hg/ps/DOGM132/DOGM132_Action.jpg)

# Video
[Here](http://schmelle2.de/wp/arduino/shields/dogm132-shield) you'll find two videos to see the DOGM132-Shield in action... Have fun.