

# Library Support

Version v1.08 and above also support the DOGXL160 display.


<table><tr><td>

<h2>Introduction</h2>

The DOGXL160 has 160x104 pixel. Each pixel can have one of 4 different values:<br>
<ul><li>Pixel off (value 0)<br>
</li><li>Light gray (value 1)<br>
</li><li>Dark gray (value 2)<br>
</li><li>Black (value 3)</li></ul>

<h2>BW and GR Modes</h2>

For the DOGXL160 display, two different modes are available (See also the <a href='install.md'>install</a> wiki page for configuration of the library):<br>
<ul><li><code>DOGXL160_HW_BW</code>: A black and white mode, which does not support gray levels. It is faster than  <code>DOGXL160_HW_GR</code> mode. Only values 0 and 3 can be used.<br>
</li><li><code>DOGXL160_HW_GR</code>: Supports gray levels (all pixel values) for the DOGXL160</li></ul>

</td><td>

<img src='http://wiki.dogm128.googlecode.com/hg/dogxl160_gr_test.jpg' />


</td></tr></table>

## Frames per Second

The following table gives an overview on the frame rates with the DOGXL160 display. Measure condition: End of level 1 in spacetrash (Arduino Uno).

| Mode           | DOG\_DOUBLE\_MEMORY | DOG\_REVERSE | Frames per Second |
|:---------------|:--------------------|:-------------|:------------------|
| DOGXL160\_HW\_GR | no			               | no REV	      | 14 FPS            |
| DOGXL160\_HW\_GR | no			               | with REV	    | 14 FPS            |
| DOGXL160\_HW\_GR | yes		               | no REV	      | 18 FPS            |
| DOGXL160\_HW\_GR | yes		               | with REV	    | 17 FPS            |
| DOGXL160\_HW\_BW | no			               | no REV	      | 19 FPS            |
| DOGXL160\_HW\_BW | no			               | with REV	    | 19 FPS            |
| DOGXL160\_HW\_BW | yes		               | no REV	      | 21 FPS            |
| DOGXL160\_HW\_BW | yes		               | with REV	    | 21 FPS            |


# Hardware

A new Arduino compatible shield with the DOGXL160 display will be documented here:
http://schmelle2.de/wp/arduino/shields/dogxl160-shield



![http://wiki.dogm128.googlecode.com/hg/dogxl160_shield_sch.jpg](http://wiki.dogm128.googlecode.com/hg/dogxl160_shield_sch.jpg)

# Links

  * [Wiki](dogm128.md)
  * [Project Home](http://code.google.com/p/dogm128/)
  * [Browse Source](http://code.google.com/p/dogm128/source/browse/)