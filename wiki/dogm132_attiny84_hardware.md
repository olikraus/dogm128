


# DOGM132 Graphics Module
Version v1.04 of the library also includes the ATTINY port of the library.
The picture shows the **walk** example running on an ATTINY84 with an attched
DOGM132-S graphics module (red backlight).

![http://wiki.dogm128.googlecode.com/hg/attiny_dogm132_on_765x354.jpg](http://wiki.dogm128.googlecode.com/hg/attiny_dogm132_on_765x354.jpg)



# DOGM132 and ATTINY84 Schematic

In this schematic the ATTINY84 and the DOGM132 (wide range configuration)
are powered by two AA battery cells.
The default init sequence for the DOGM132 matches this setup.
The ATTINY84 is flashed through the 6-pin ISP connector. `R5` is required
to protect the DOGM132 hardware during the flash procedure. The values of `R2`
and `R3` are valid for the red backlight.

![http://wiki.dogm128.googlecode.com/hg/dogm132_attiny84_sch.jpg](http://wiki.dogm128.googlecode.com/hg/dogm132_attiny84_sch.jpg)

# DOGM132 and ATTINY84 Board


![http://wiki.dogm128.googlecode.com/hg/attiny_dogm132_exp_765x670.jpg](http://wiki.dogm128.googlecode.com/hg/attiny_dogm132_exp_765x670.jpg)



# Links

  * [Install and setup instructions for the ATTINY84](install.md)
  * [Wiki Toplevel Page](dogm128.md)
  * [Wiki Hardware Overview](hardware.md)
  * [Project Home](http://code.google.com/p/dogm128/)
  * [Browse Source](http://code.google.com/p/dogm128/source/browse/)