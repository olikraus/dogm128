

# Arduino and Dogm128-Graphics-LCD

The Dogm-graphics-module should be connected to the Arduino board as shown below.
Pin 12 (MISO) of the Arduino board must not be used. Additionally the Dogm-graphics-module requires an addess line (A0) which can be connected to any output port.

The potentiometer at analog input pin 0 is required for the Breakorino example game.

![http://wiki.dogm128.googlecode.com/hg/dogm_block.jpg](http://wiki.dogm128.googlecode.com/hg/dogm_block.jpg)

# 3.3 Volt Single Supply

The following schematic shows the use of the dogm-graphics-module with a 3.3 volt supply.
The level translation (5 volt to 3.3 volt) is done by the HC4050 integrated circuit.

The schematic uses 47 ohm resitors for the backlight. The actual value depends on the type of the backlight.

![http://wiki.dogm128.googlecode.com/hg/dogm_sch_100.jpg](http://wiki.dogm128.googlecode.com/hg/dogm_sch_100.jpg)

# 3.3 Volt Supply with 5 Volt for the Background LED

In the following schematic all supply current is provided by the Arduino board:

![http://wiki.dogm128.googlecode.com/hg/dogm128_arduino_sch.jpg](http://wiki.dogm128.googlecode.com/hg/dogm128_arduino_sch.jpg)


# Links

  * [Wiki Toplevel Page](dogm128.md)
  * [Wiki Hardware Overview](hardware.md)
  * [Project Home](http://code.google.com/p/dogm128/)
  * [Browse Source](http://code.google.com/p/dogm128/source/browse/)