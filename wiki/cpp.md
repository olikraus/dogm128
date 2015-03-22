
# Concept
## Naming Convention
All functions start with a verb:
  * **set**: Set the value of an additional parameter or set pixel values to logical one. For the DOGXL160 display, the value assigned by the procedure [setPixelValue](cpp#setPixelValue.md) will be used.
  * **clr**: Clear pixels (set pixel values to logical zero).
  * **xor**: Flip pixel value from logical one to zero or zero to one.
  * **draw**: Set pixel values to logical one, but use additional parameters (e.g. [setSize](cpp#setSize.md)).
## Display Orientation
The origin `(0,0)` is at the lower left corner. The pixel at the upper right corner has
the coordinates `(63,127)`.

## Picture Loop
There is a small difference to other graphics libraries: All graphic output must
be inside a while loop:
```
  Dogm dogm;
  /* ... */
  dogm.start();
  do {
    /* ... */
    dogm.drawLine(0,0,50,50);
    /* ... */
  } while( dogm.next() );
```
In order to minimize the RAM usage, the graphic is splited into smaller parts. The same
picture is created several times, but different parts are transfered to the graphics
module. As soon as all parts are transfered, [next](cpp#next.md) will return false and
the while loop will terminate.

Rules:
  * Use the [start](cpp#start.md) function call at the beginning of the picture loop.
  * Loop while [next](cpp#next.md) returns true.
  * Build exactly the same graphics within the body of the loop.

The following example violates the third point and will not work. `i` is incremented
in the body of the picture loop. As a result, different graphics are drawn with
each repetition of the loop:
```
  Dogm dogm;
  int i = 0;
  /* ... */
  dogm.start();
  i = 0;
  do {
    dogm.drawLine(i,0,50,50);
    i++;
  } while( dogm.next() );
```

Of course `i` can be used and changed inside the loop as long as the same graphics
is produced:
```
  Dogm dogm;
  int i = 0;
  /* ... */
  dogm.start();
  do {
    i = 0;
    do {
      dogm.drawLine(i,0,50,50);
      i++;
    } while( i < 10 );
  } while( dogm.next() );
```

# Macros

  * `DOG_WIDTH`: The width of the display.
  * `DOG_HEIGHT`: The height of the display.

# Global Variables

  * `uint8_t dog_spi_result`: The last value returned from SPI bus after execution of the picture loop (see above).


# Function Reference
## clrBox
> Clear all pixel within and including the provided boundaries. `(x1,y1)` is the
> lower left corner, `(x2,y2)` is the upper right corner. Conditions: `x1<=x2` and
> `y1<=y2`.
  * Prototype
```
void Dogm::clrBox(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
```
  * Arguments
    * `x1,y1`: Lower left edge
    * `x2,y2`: Upper right edge
  * See also: [clrVLine](cpp#clrVLine.md), [setBox](cpp#setBox.md), [xorBox](cpp#xorBox.md)

## clrHLine
> Clear all pixel along the horizontal line from `(x1,y)` to `(x2,y)`. Condition: `x1<=x2`.
  * Available: v1.03
  * Prototype
```
void Dogm::clrHLine(uint8_t x1, uint8_t x2, uint8_t y)
```
  * Arguments
    * `x1`: Left end of the line
    * `x2`: Right end of the line
    * `y`: Row
  * See also: [clrVLine](cpp#clrVLine.md), [setHLine](cpp#setHLine.md), [xorHLine](cpp#xorHLine.md)

## clrPixel
> Clear a single pixel.
  * Prototype
```
void Dogm::clrPixel(uint8_t x, uint8_t y) 
```
  * Arguments
    * `x,y`: Position of the pixel.
  * See also: [setPixel](cpp#setPixel.md)

## clrVLine
> Clear all pixel along the vertical line from `(x,y1)` to `(x,y2)`. Condition: `y1<=y2`.
  * Prototype
```
void Dogm::clrVLine(uint8_t x, uint8_t y1, uint8_t y2)
```
  * Arguments
    * `x`: Column
    * `y1`: Lower end of the vertical line
    * `y2`: Upper end of the vertical line
  * See also: [clrBox](cpp#clrBox.md)

## Dogm
> The C++ constructor of the library. The only argument is the pin number of the
> adress pin (see [hardware](hardware.md) page).
  * Prototype
```
void Dogm::Dogm(uint8_t pin_a0)
void Dogm::Dogm(uint8_t pin_a0, uint8_t pin_cs)
```
  * Arguments
    * `pin_a0`: Pin number for the adress line of the dogm LCD module
    * `pin_cs`: Pin number for the chip select line of the dogm LCD module
  * Note 1: The SS pin (Pin 10 of the Arduino Uno Board) will always be configured as output port. Usually the chips select line of the display should be connected to the SS pin and pin\_cs should have the value 10.
  * Note 2: The pin\_cs argument is available with version 1.07 and above of the lib.

## drawArc
> Draw an arc or circle with radius r. Use [setSize](cpp#setSize.md) to define the size
> of the line. A circle is drawn if `w0` is equal to `w1` else an arc is created.
> `w0` and `w1` can have values from 0 to 255, which has been mapped to degree from 0 to
> 2\*Pi. This means that the value `w0 = 128` corresponds to Pi.

> An arc with `w0 = 0` starts at the right at point `(mx+r,my)` and will continue
> counterclockwise until `w1`.

  * Prototype
```
void Dogm::drawArc(uint8_t mx, uint8_t my, uint8_t r, uint8_t w0, uint8_t w1)
```
  * Arguments
    * `mx`, `my`: The center of the arc or circle.
    * `r`: The radius of the arc or circle.
    * `w0`: Start angle.
    * `w1`: End angle.
  * See also: [setSize](cpp#setSize.md)

## drawChar
> Draw a single character. Currently only codes from 32 to 127 are supported. Set
> a font and the xy coordinates of the character ([setFont](cpp#setFont.md) and
> [setXY](cpp#setXY.md)).

> There is no need to use this function: Dogm is derived from the Arduino Print class
> so the prefered character draw function should be `print`. `print` is much
> more flexible than `drawChar`

  * Prototype
```
void Dogm::drawChar(uint8_t c) 
```
  * Arguments
    * `c`: Character code from 32 to 127.
  * See also: [setFont](cpp#setFont.md), [setXY](cpp#setXY.md), [setRot](cpp#setRot.md)
  * Example: See [setFont](cpp#setFont.md)

## drawLine
> Draw a line. Use [setSize](cpp#setSize.md) to define the size
> of the line. Note: There are no restrictions on the relation between `x1` and `x2`
> or `y1` and `y2`.
  * Prototype
```
void Dogm::drawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
```
  * Arguments
    * `x1,y1`: First point of the line
    * `x2,y2`: Second point of the line
  * See also: [setSize](cpp#setSize.md)

## drawPoint
> Draw a single point. Use [setSize](cpp#setSize.md) to define the size of the point.
  * Prototype
```
void Dogm::drawPoint(uint8_t x, uint8_t y)
```
  * Arguments
    * `x,y`: Position of the point.
  * See also: [setSize](cpp#setSize.md)

## drawStr
> Draw a string. Currently only codes from 32 to 127 are supported. Set
> a font and the xy coordinates of the lower left corner
> of the string ([setFont](cpp#setFont.md) and
> [setXY](cpp#setXY.md)).

> There is no need to use this function: Dogm is derived from the Arduino Print class
> so the prefered character draw function should be `print`. `print` is much
> more flexible than `drawStr`.

  * Prototype
```
void Dogm::drawStr(const char *s) 
```
  * Arguments
    * `s`: Pointer to a string.
  * See also: [setFont](cpp#setFont.md), [setXY](cpp#setXY.md), [setRot](cpp#setRot.md)
  * Example:
```
void loop() {
  dogm.start();
  do{
    dogm.setFont(font_4x6);
    dogm.setXY(10,10);
    dogm.drawStr("Hello World!");
  } while( dogm.next() );
  delay(100);
}
```
  * See also: [print](cpp#print.md)


## getStrWidth
> Returns the width (pixels) of the specified string.
  * Available: v1.01
  * Prototype
```
void uint8_t Dogm::getStrWidth(const char *s) 
```
  * Arguments
    * `s`: Pointer to a string.
  * See also: [setFont](cpp#setFont.md)
  * Example: See Fonts.pde

## next
> The `next` function is part of the [picture loop](cpp#Picture_Loop.md). It will return
> true as long as additional loops are required.
  * Prototype
```
uint8_t Dogm::next(void) 
```
  * See also: [start](cpp#start.md), [picture loop](cpp#Picture_Loop.md)
## print
> > The `Dogm` class is derived from the `Print` class of the Arduino environment.
> > See [Serial Print](http://www.arduino.cc/en/Serial/Print) for a more detailed
> > description.
  * Prototype
```
void Dogm::print(...)
```
  * See also: [write](cpp#write.md)

## setBitmap
## setBitmapP

> Draw a bitmap with its upper left corner at position `(x,y)`. A bitmap is a sequence
> of horizontal
> bitmap lines (see [setHBitmap](cpp#setHBitmap.md)). Each line has `(w+7)/8` bytes.
> `bitmap` must point to  `((w+7)/8)*h` bytes.
  * Available: v1.01
  * Prototype
```
void Dogm::setBitmap(uint8_t x, uint8_t y, const unsigned char *bitmap, uint8_t w, uint8_t h)
```
```
void Dogm::setBitmapP(uint8_t x, uint8_t y, PGM_P bitmap, uint8_t w, uint8_t h)
```
  * Arguments
    * `x,y`: Position of the upper left corner of the bitmap.
    * `bitmap`: Pointer to the bitmap
    * `w`: Width of the bitmap (number of pixels, (w+7)/8 bytes are occupied)
    * `h`: Height of the bitmap

  * See also: [setHBitmap](cpp#setHBitmap.md)
  * Example
```
  Dogm dogm;
  const unsigned char bitmap[2] = { 0xff, 0xff };
  // ...
  dogm.start();
  do {
    // draw a 2x2 box, equivalent to setBox(0,0,1,1)
    dogm.setBitmap(0,1,bitmap,2,2);
  } while( dogm.next() );
```
```
  Dogm dogm;
  const char bitmap[2] PROGMEM = { 0xff, 0xff };
  // ...
  dogm.start();
  do {
    // draw a 2x2 box, equivalent to setBox(0,0,1,1)
    dogm.setBitmapP(0,1,bitmap,2,2);
  } while( dogm.next() );
```

## setBox
> Set all pixel within and including the provided boundaries. `(x1,y1)` is the
> lower left corner, `(x2,y2)` is the upper right corner. Conditions: `x1<=x2` and
> `y1<=y2`.
  * Prototype
```
void Dogm::setBox(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
```
  * Arguments
    * `x1,y1`: Lower left edge
    * `x2,y2`: Upper right edge
  * See also: [clrBox](cpp#clrBox.md), [setVLine](cpp#setVLine.md), [xorBox](cpp#xorBox.md)


## setContrast
> Set the contrast value of the display. This function can be used outside the picture
> loop.
  * Available: v1.06
  * Prototype
```
void Dogm::setContrast(uint8_t val)
```
  * Arguments
    * `val`: Contrast value between 0 and 63.
  * See also: [setInvertPixelMode](cpp#setInvertPixelMode.md)

## setFont
> Select a font for later use by the character drawing functions. Predefined fonts are `font_4x6`,
> `font_5x7`,  `font_5x8`,  `font_6x9`, `font_6x10`, `font_6x12`, `font_6x13`,   `font_7x13`,  `font_7x14`,
> `font_8x13`,  `font_8x16`,  `font_9x15`,  `font_9x18`.
  * Prototype
```
void Dogm::void setFont(PGM_P font)
```
  * Arguments
    * `font`: A pointer to the font data (generated by bdf2dogm)
  * See also: [drawChar](cpp#drawChar.md)
  * Example
```
  Dogm dogm;
  dogm.start();
  do {
    dogm.setFont(font_4x6);
    dogm.setXY(10,10);
    dogm.write("Hello World!");
  } while( dogm.next() );
```

## setHBitmap
## setHBitmapP
> Bytes at position `bitmap` are interpreted as horizontal line of pixels: A logical
> one of a bit in these bytes will set the value 1 in the graphics memory. A logical
> zero will not alter the graphics memory.
> The horizontal bitmap line will start at `(x,y)`. The length of this horizontal
> line has `w` pixels. `bitmap` must point to a sequence of `(w+7)/8` bytes.
  * Available: v1.01
  * Prototype
```
void Dogm::setHBitmap(uint8_t x, uint8_t y, const unsigned char *bitmap, uint8_t w)
```
```
void Dogm::setHBitmapP(uint8_t x, uint8_t y, PGM_P bitmap, uint8_t w)
```
  * Arguments
    * `x,y`: Position of the left end of the horizontal bitmap line.
    * `bitmap`: Pointer to the bitmap line
    * `w`: Number of pixels in the bitmap line
  * See also: [setBitmap](cpp#setBitmap.md)
  * Example
```
  Dogm dogm;
  const unsigned char bitmap[1] = { 0x81 };
  // ...
  dogm.start();
  do {
    // set a pixel at (0,0) and (7,0)
    dogm.setHBitmap(0,0,bitmap,8);
  } while( dogm.next() );
```
```
  // PROGMEM example
  Dogm dogm;
  const char bitmap[1] PROGMEM = { 0x81 };
  // ...
  dogm.start();
  do {
    // set a pixel at (0,0) and (7,0)
    dogm.setHBitmapP(0,0,bitmap,8);
  } while( dogm.next() );
```

## setHLine
> Set all pixel along the horizontal line from `(x1,y)` to `(x2,y)`. Condition: `x1<=x2`.
  * Available: v1.01
  * Prototype
```
void Dogm::setHLine(uint8_t x1, uint8_t x2, uint8_t y)
```
  * Arguments
    * `x1`: Left end of the line
    * `x2`: Right end of the line
    * `y`: Row
  * See also: [setVLine](cpp#setVLine.md), [clrHLine](cpp#clrHLine.md), [xorHLine](cpp#xorHLine.md)

## setInvertPixelMode
> Inverts the display. This function can be used outside the picture
> loop.
  * Available: v1.06
  * Prototype
```
void Dogm::setInvertPixelMode(uint8_t val)
```
  * Arguments
    * `val`: 0 (normal mode) or 1 (inverted display).
  * See also: [setContrast](cpp#setContrast.md)

## setPixel
> Set a single pixel.
  * Prototype
```
void Dogm::setPixel(uint8_t x, uint8_t y) 
```
  * Arguments
    * `x,y`: Position of the pixel.
  * See also: [clrPixel](cpp#clrPixel.md)

## setPixelValue
> Set the pixel value for the DOGXL160 display. Supported values are 0 (no pixel), 1 (light gray), 2 (dark gray) and 3 (black). All `set` procedures are affected by this setting.
  * Available: v1.08
  * Prototype
```
void Dogm::setPixelValue(uint8_t val)
```
  * Arguments
    * `val`: Pixel value.
  * See also: [setPixel](cpp#setPixel.md)



## setRot
> Set the direction of the string and character output. Default is `rotation = 0`.
> Graphic function are optimized for this default value.
  * Available: v1.05
  * Prototype
```
void Dogm::setRot(uint8_t rotation)
```
  * Arguments
    * `rotation`: Values 0, 1, 2 and 3 will rotate the output by 0, 90, 180 and 270 degree.
  * See also: [drawStr](cpp#drawStr.md) [drawChar](cpp#drawChar.md)

## setSize
> Select a size for the drawing functions.
  * Prototype
```
 void setSize(uint8_t s)
```
  * Arguments
    * `s`: 0 (1 pixel size) or 1 (three pixel size)
  * See also: [drawArc](cpp#drawArc.md), [drawLine](cpp#drawLine.md), [drawPoint](cpp#drawPoint.md)

## setVLine
> Set all pixel along the vertical line from `(x,y1)` to `(x,y2)`. Condition: `y1<=y2`.
  * Prototype
```
void Dogm::setVLine(uint8_t x, uint8_t y1, uint8_t y2)
```
  * Arguments
    * `x`: Column
    * `y1`: Lower end of the vertical line
    * `y2`: Upper end of the vertical line
  * See also: [clrVLine](cpp#clrVLine.md), [setBox](cpp#setBox.md)

## setXY
> Set a pixel position. Currently this is only used by the character drawing functions.
> Note, that this position might be changed by the drawing functions.
  * Prototype
```
 void setXY(uint8_t x, uint8_t y)
```
  * Arguments
    * `x,y`: Assigns a pixel position for some graphic functions
  * See also: [drawChar](cpp#drawChar.md)
  * Example: See [setFont](cpp#setFont.md)

## write
> A virtual function from the `Print` base class. Use the function `print` from
> the baseclass to write
> something to the display.

  * See also: [print](cpp#print.md)

## xorBox
> Flip all pixel within and including the provided boundaries. `(x1,y1)` is the
> lower left corner, `(x2,y2)` is the upper right corner. Conditions: `x1<=x2` and
> `y1<=y2`.
  * Prototype
```
void Dogm::xorBox(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
```
  * Arguments
    * `x1,y1`: Lower left edge
    * `x2,y2`: Upper right edge
  * See also: [setBox](cpp#setBox.md), [xorVLine](cpp#xorVLine.md)

## xorHLine
> Flip pixel along the horizontal line from `(x1,y)` to `(x2,y)`. Condition: `x1<=x2`.
  * Available: v1.03
  * Prototype
```
void Dogm::xorHLine(uint8_t x1, uint8_t x2, uint8_t y)
```
  * Arguments
    * `x1`: Left end of the line
    * `x2`: Right end of the line
    * `y`: Row
  * See also: [xorVLine](cpp#xorVLine.md), [setHLine](cpp#setHLine.md), [clrHLine](cpp#clrHLine.md)


## xorPixel
> Flip a single pixel.
  * Prototype
```
void Dogm::xorPixel(uint8_t x, uint8_t y) 
```
  * Arguments
    * `x,y`: Position of the pixel.
  * See also: [setPixel](cpp#setPixel.md) [clrPixel](cpp#clrPixel.md)

## xorVLine
> Flip all pixel along the vertical line from `(x,y1)` to `(x,y2)`. Condition: `y1<=y2`.
  * Prototype
```
void Dogm::xorVLine(uint8_t x, uint8_t y1, uint8_t y2)
```
  * Arguments
    * `x`: Column
    * `y1`: Lower end of the vertical line
    * `y2`: Upper end of the vertical line
  * See also: [setVLine](cpp#setVLine.md), [xorBox](cpp#xorBox.md)