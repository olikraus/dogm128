

# Introduction

This project includes a small tool `bdf2dogm` to convert the BDF file format into the
internal data structure used by the dogm128 library.

# bdf2dogm

`bdf2dogm` is a simple c-file `bdf2dogm.c`. To generate the executable (unix systems),
simply compile
the c-file:
```
cc bdf2dogm.c -o bdf2dogm
```

# TTF (truetype font) conversion

## Overview

This is the conversion procedure for truetype fonts:
  1. Find out a suitable point size (ftview)
  1. Convert TTF to BDF (otf2bdf)
  1. Convert BDF to a c-file (bdf2dogm)

## TTF point size

A truetype font often does not look very well with every point size. You can use
the unix command `ftview` from the freetype tool collection to check the font with
different point sizes:
```
ftview 8 <fontname>.ttf
```

Different point sizes can be accessed with cursor up and down. Often it is useful to
turn off aliasing by pressing "a".

## TTF to BDF conversion

The tool `otf2bdf` can convert the truetype font into bitmap format (bdf).
For a linux environment, `otf2bdf` should be available as software package.

This conversion is done for a specific point size:
```
otf2bdf -p <pointsize> -r 75 -o <fontname>.bdf <fontname>.ttf
```

The result can be checked with `fontforge'.

## BDF to c-file conversion

Use the tool `bdf2dgom` (part of this project) to create a c-file:
```
bdf2dogm <fontname>.bdf <fontname>.c <fontname>
```

## Add font to a dogm128 project

Copy the c-file into the `Dogm/utility` directory and place the declaration
```
extern const dog_pgm_uint8_t <fontname>[];		/* version 1.6 and above */
```
or
```
extern const char <fontname>[] PROGMEM;		/* version 1.5 and below */
```
into your source code:
```
#include <Dogm.h>
extern const dog_pgm_uint8_t <fontname>[];

int a0Pin = 9;     // address line a0 for the dogm module
Dogm dogm(a0Pin);  // initialize the dogm library

void setup() {
}

void loop() {
  dogm.start();
  do{
    dogm.setFont(my_fontname);
    dogm.setXY(10,10);
    dogm.drawStr("Hello World!");
  } while( dogm.next() );
  delay(100);
}
```

# PCF (Portable Compiled Format) conversion

Fonts distributed in the `.pcf` or `.pcf.gz` file format are part of the X11
distribution. Steps are:

  1. Convert PCF to BDF (pcf2bdf)
  1. Convert BDF to the internal representation (bdf2dogm)

`pcf2bdf` is often available as a software package on a linux system:
```
pcf2bdf -o <fontname>.bdf <fontname>.pcf.gz
```

See the section for truetype conversion for further handling of the BDF
file.

# Create new BDF fonts

There are several tools available for the creation of new fonts:
  * [gbdfed](http://www.math.nmsu.edu/~mleisher/Software/gbdfed/).
  * [fontforge](http://fontforge.sourceforge.net/)
Both tools can export to the BDF file format.

