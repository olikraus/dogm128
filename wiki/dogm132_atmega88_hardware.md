


# ATMEGA88 with DOGM132 Graphics Module
Version v1.07 of the library also includes the ATMEGA port of the library.

In this schematic the ATMEGA88 and the DOGM132 (wide range configuration)
are powered by two AA battery cells.
The default init sequence for the DOGM132 matches this setup.
The ATMEGA88 is flashed through the 6-pin ISP connector. `R5` is required
to protect the DOGM132 hardware during the flash procedure. The values of `R1`
and `R2` are valid for the red backlight.

![http://wiki.dogm128.googlecode.com/hg/dogm132_atmega88_sch.jpg](http://wiki.dogm128.googlecode.com/hg/dogm132_atmega88_sch.jpg)

# Links

  * [Install and setup instructions for the ATMEGA88](install.md)
  * [Wiki Toplevel Page](dogm128.md)
  * [Wiki Hardware Overview](hardware.md)
  * [Project Home](http://code.google.com/p/dogm128/)
  * [Browse Source](http://code.google.com/p/dogm128/source/browse/)