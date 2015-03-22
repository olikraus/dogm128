
# Concept
## Naming Convention

All functions are prefixed with `dog` and continue with a verb:
  * `dog_Set`: Set the value of an additional parameter or set pixel values to logical one.
For the DOGXL160 display, the value assigned by the procedure [dog\_SetPixelValue](cref#dog_SetPixelValue.md) will be used.
  * `dog_Clr`: Clear pixels (set pixel values to logical zero).
  * `dog_Xor`: Flip pixel value from logical one to zero or zero to one.
  * `dog_Draw`: Set pixel values to logical one, but use additional parameters.

## Display Orientation
The origin `(0,0)` is at the lower left corner. The pixel at the upper right corner has
the coordinates `( DOG_WIDTH - 1, DOG_HEIGHT - 1)`.

## Picture Loop
There is a small difference to other graphics libraries: All graphic output must
be inside a while loop:
```
  /* ... */
  dog_StartPage();
  do
  {
    /* ... */
    dog_DrawLine(0,0,50,50,0);
  } while( dog_NextPage() );
```
In order to minimize the RAM usage, the graphic is splited into smaller parts. The same
picture is created several times, but different parts are transfered to the graphics
module. As soon as all parts are transfered, [dog\_NextPage](cref#dog_NextPage.md) will return false and
the while loop will terminate.

Rules:
  * Use the [dog\_StartPage](cref#dog_StartPage.md) function call at the beginning of the picture loop.
  * Loop while [dog\_NextPage](cref#dog_NextPage.md) returns true.
  * Build exactly the same graphics within the body of the loop.

The following example violates the third point and will not work. `i` is incremented
in the body of the picture loop. As a result, different graphics are drawn with
each repetition of the loop:
```
  /* ... */
  dog_StartPage();
  i = 0;
  do
  {
    /* ... */
    dog_DrawLine(i,0,50,50,0);
    i++;
  } while( dog_NextPage() );
```

Of course `i` can be used and changed inside the loop as long as the same graphics
is produced:
```
  int i = 0;
  /* ... */
  dog_StartPage();
  do {
    i = 0;
    do {
      dog_DrawLine(i,0,50,50,0);
      i++;
    } while( i < 10 );
  } while( dog_NextPage() );
```


# Macros

  * `DOG_WIDTH`: The width of the display.
  * `DOG_HEIGHT`: The height of the display.

# Global Variables

  * `uint8_t dog_spi_result`: The last value returned from SPI bus after execution of the picture loop (see above).

# Function Reference
## `dog_ClrBox`
> Clear all pixel within and including the provided boundaries. `(x1,y1)` is the
> lower left corner, `(x2,y2)` is the upper right corner. Conditions: `x1<=x2` and
> `y1<=y2`.
  * Prototype
```
void dog_ClrBox(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
```
  * Arguments
    * `x1,y1`: Lower left edge
    * `x2,y2`: Upper right edge
  * See also: [dog\_ClrVLine](cref#dog_ClrVLine.md), [dog\_SetBox](cref#dog_SetBox.md), [dog\_XorBox](cref#dog_XorBox.md)

## `dog_ClrHLine`
> Clear all pixel along the horizontal line from `(x1,y)` to `(x2,y)`. Condition: `x1<=x2`.
  * Available: v1.03
  * Prototype
```
void dog_ClrHLine(uint8_t x1, uint8_t x2, uint8_t y)
```
  * Arguments
    * `x1`: Left end of the line
    * `x2`: Right end of the line
    * `y`: Row
  * See also: [dog\_ClrVLine](cref#dog_ClrVLine.md), [dog\_SetHLine](cref#dog_SetHLine.md), [dog\_XorHLine](cref#dog_XorHLine.md)

## `dog_ClrPixel`
> Clear a single pixel.
  * Prototype
```
void dog_ClrPixel(uint8_t x, uint8_t y) 
```
  * Arguments
    * `x,y`: Position of the pixel.
  * See also: [dog\_SetPixel](cref#dog_SetPixel.md)

## `dog_ClrVLine`
> Clear all pixel along the vertical line from `(x,y1)` to `(x,y2)`. Condition: `y1<=y2`.
  * Prototype
```
void dog_ClrVLine(uint8_t x, uint8_t y1, uint8_t y2)
```
  * Arguments
    * `x`: Column
    * `y1`: Lower end of the vertical line
    * `y2`: Upper end of the vertical line
  * See also: [dog\_ClrHLine](cref#dog_ClrHLine.md), [dog\_ClrBox](cref#dog_ClrBox.md)

## `dog_Delay`
> Wait for the specified number of milliseconds. Interrupts might extend this period.
> This procedure will wait for up to 65 seconds.

  * Prototype
```
void dog_Delay(uint16_t val)
```
  * Arguments
    * `val`: Milliseconds


## `dog_DrawArc`
> Draw an arc or circle with radius r. A circle is drawn if `w0` is equal to `w1` else an arc is created.
> `w0` and `w1` can have values from 0 to 255, which has been mapped to degree from 0 to
> 2\*Pi. This means that the value `w0 = 128` corresponds to Pi.

> An arc with `w0 = 0` starts at the right at point `(mx+r,my)` and will continue
> counterclockwise until `w1`.

  * Prototype
```
void dog_DrawArc(uint8_t mx, uint8_t my, uint8_t r, uint8_t w0, uint8_t w1, uint8_t size)
```
  * Arguments
    * `mx`, `my`: The center of the arc or circle.
    * `r`: The radius of the arc or circle.
    * `w0`: Start angle.
    * `w1`: End angle.
    * `size`: 0 (1 pixel size) or 1 (three pixel size)
  * See also: [dog\_DrawLine](cref#dog_DrawLine.md)

## `dog_DrawChar` `dog_DrawRChar`
> Draw a single character. Currently only codes from 32 to 127 are supported.
> Graphic function are optimized for `r = 0`.
> (`dog_DrawRChar` is available in v1.05.)
  * Prototype
```
uint8_t dog_DrawChar(uint8_t x, uint8_t y, DOG_PGM_P font, unsigned char code)
uint8_t dog_DrawRChar(uint8_t x, uint8_t y, uint8_t r, DOG_PGM_P font, unsigned char code)
```
  * Arguments
    * `x,y`: Position of the point.
    * `r`: Values 0, 1, 2 and 3 will rotate the output by 0, 90, 180 and 270 degree.
    * `font`: A predefined font: font\_4x6, font\_5x7, font\_5x8, font\_6x9, font\_6x10, font\_6x12, font\_6x13, font\_7x13, font\_7x14, font\_8x13, font\_8x16, font\_9x15, font\_9x18
    * `code`: Character code from 32 to 127.
  * Result: Width (pixels) of the character.
  * See also: [dog\_DrawStr](cref#dog_DrawStr.md)

## `dog_DrawLine`
> Draw a line. Note: There are no restrictions on the relation between `x1` and `x2`
> or `y1` and `y2`.
  * Prototype
```
void dog_DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t size)
```
  * Arguments
    * `x1,y1`: First point of the line
    * `x2,y2`: Second point of the line
    * `size`: 0 (1 pixel size) or 1 (three pixel size)
  * See also: [dog\_DrawPoint](cref#dog_DrawPoint.md)

## `dog_DrawPoint`
> Draw a single point.
  * Prototype
```
void dog_DrawPoint(uint8_t x, uint8_t y, uint8_t size)
```
  * Arguments
    * `x,y`: Position of the point.
    * `size`: 0 (1 pixel size) or 1 (three pixel size)
  * See also: [dog\_DrawLine](cref#dog_DrawLine.md)

## `dog_DrawStr` `dog_DrawRStr`
> Draw a string. Currently only codes from 32 to 127 are supported.
> Graphic function are optimized for `r = 0`.
> (`dog_DrawRStr` is available in v1.05.)
  * Prototype
```
uint8_t dog_DrawStr(uint8_t x, uint8_t y, DOG_PGM_P font, const char *s) 
uint8_t dog_DrawRStr(uint8_t x, uint8_t y, uint8_t r, DOG_PGM_P font, const char *s) 
```
  * Arguments
    * `x,y`: Position of the lower left corner (`r = 0`) of the string.
    * `r`: Values 0, 1, 2 and 3 will rotate the output by 0, 90, 180 and 270 degree.
    * `font`: A predefined font: font\_4x6, font\_5x7, font\_5x8, font\_6x9, font\_6x10, font\_6x12, font\_6x13, font\_7x13, font\_7x14, font\_8x13, font\_8x16, font\_9x15, font\_9x18
    * `s`: Pointer to a string.
  * Result: Width (pixels) of the string.
  * See also: [dog\_DrawChar](cref#dog_DrawChar.md)
  * Example:
```
void main(voidd) {
  /* ... */
  dog_StartPage();
  do{
    dog_DrawStr(10, 10, font_4x6, "Hello World!");
  } while( dog_NextPage() );
  dog_delay(100);
}
```

## `dog_DrawStrP` `dog_DrawRStrP`
> Draw a string (which is located in flash rom area). Currently only codes from 32 to 127 are supported.
> Graphic function are optimized for `r = 0`.
  * Available: v1.06
  * Prototype
```
uint8_t dog_DrawStrP(uint8_t x, uint8_t y, DOG_PGM_P font, const char *s) 
uint8_t dog_DrawRStrP(uint8_t x, uint8_t y, uint8_t r, DOG_PGM_P font, const char *s) 
```
  * Arguments
    * `x,y`: Position of the lower left corner (`r = 0`) of the string.
    * `r`: Values 0, 1, 2 and 3 will rotate the output by 0, 90, 180 and 270 degree.
    * `font`: A predefined font: font\_4x6, font\_5x7, font\_5x8, font\_6x9, font\_6x10, font\_6x12, font\_6x13, font\_7x13, font\_7x14, font\_8x13, font\_8x16, font\_9x15, font\_9x18
    * `s`: Pointer to a string.
  * Result: Width (pixels) of the string.
  * See also: [dog\_DrawChar](cref#dog_DrawChar.md)
  * Example:
```
void main(voidd) {
  DOG_PSTR_P str = DOG_PSTR("World!");
  /* ... */
  dog_StartPage();
  do{
    dog_DrawStr(10, 20, font_4x6, DOG_PSTR("Hello"));
    dog_DrawStr(10, 10, font_4x6, str);
  } while( dog_NextPage() );
  dog_delay(100);
}
```
  * Notes
    * A string is placed in the flash rom area if it is passed to the macro `DOG_PSTR()`. The return value of `DOG_PSTR()` is of type `DOG_PSTR_P` and is an address within flash rom area.
    * `DOG_PSTR_P` can be used to define variables which can hold the output of `DOG_PSTR()` (see example).
    * It is a good pratice to apply `DOG_PSTR()` to all string constants (less ram usage, better portability).

## `dog_GetStrWidth`, `dog_GetStrWidthP`
> Returns the width (pixels) of the specified string.
  * Available: v1.01
  * Prototype
```
uint8_t dog_GetStrWidth(DOG_PGM_P font, const char *s)
uint8_t dog_GetStrWidth(DOG_PGM_P font, DOG_PSTR_P s)
```
  * Arguments
    * `font`: A predefined font: font\_4x6, font\_5x7, font\_5x8, font\_6x9, font\_6x10, font\_6x12, font\_6x13, font\_7x13, font\_7x14, font\_8x13, font\_8x16, font\_9x15, font\_9x18
    * `s`: Pointer to a string.
  * See also: [dog\_DrawStr](cref#dog_DrawStr.md), [dog\_DrawStrP](cref#dog_DrawStrP.md)

## `dog_Init`
> Setup the library. All other function of this library can only be used after
> a calling this function.
> For the Arduino Environment the only argument is the pin number of the
> adress pin (see [hardware](hardware.md) page). For other controllers this argument is ignored.
  * Prototype
```
void dog_Init(unsigned short pin_a0)
```
  * Arguments
    * `pin_a0`: Pin number for the adress line of the dogm LCD module (Arduino Environment)


## `dog_NextPage`
> The `dog_NextPage` function is part of the [picture loop](cref#Picture_Loop.md). It will return
> true as long as additional loops are required.
  * Prototype
```
uint8_t dog_next(void) 
```
  * See also: [dog\_StartPage](cref#dog_StartPage.md), [picture loop](cref#Picture_Loop.md)

## `dog_SetBitmap` `dog_SetBitmapP` `dog_ClrBitmapP`
> Draw a bitmap with its upper left corner at position `(x,y)`. A bitmap is a sequence
> of horizontal
> bitmap lines (see [dog\_SetHBitmap](cref#dog_SetHBitmap.md)). Each line has `(w+7)/8` bytes.
> `bitmap` must point to  `((w+7)/8)*h` bytes.
  * Available: v1.01
  * Prototype
```
void dog_SetBitmap(uint8_t x, uint8_t y, const unsigned char *bitmap, uint8_t w, uint8_t h)
```
```
void dog_SetBitmapP(uint8_t x, uint8_t y, DOG_PGM_P bitmap, uint8_t w, uint8_t h)
```
```
void dog_ClrBitmapP(uint8_t x, uint8_t y, DOG_PGM_P bitmap, uint8_t w, uint8_t h)
```
  * Arguments
    * `x,y`: Position of the upper left corner of the bitmap.
    * `bitmap`: Pointer to the bitmap
    * `w`: Width of the bitmap (number of pixels, (w+7)/8 bytes are occupied)
    * `h`: Height of the bitmap

  * See also: [dog\_SetHBitmap](cref#dog_SetHBitmap.md)
  * Example
```
  const unsigned char bitmap[2] = { 0xff, 0xff };
  /* ... */
  dog_StartPage();
  do {
    /* draw a 2x2 box, equivalent to setBox(0,0,1,1) */
    dog_SetBitmap(0,1,bitmap,2,2);
  } while( dog_NextPage() );
```
```
  dog_pgm_uint8_t bitmap[2] = { 0xff, 0xff };
  /* ... */
  dog_StartPage();
  do {
    /* draw a 2x2 box, equivalent to setBox(0,0,1,1) */
    dog_SetBitmapP(0,1,bitmap,2,2);
  } while( dog_NextPage() );
```

## `dog_SetBox`
> Set all pixel within and including the provided boundaries. `(x1,y1)` is the
> lower left corner, `(x2,y2)` is the upper right corner. Conditions: `x1<=x2` and
> `y1<=y2`.
  * Prototype
```
void dog_SetBox(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
```
  * Arguments
    * `x1,y1`: Lower left edge
    * `x2,y2`: Upper right edge
  * See also: [dog\_ClrBox](cref#dog_ClrBox.md), [dog\_SetVLine](cref#dog_SetVLine.md), [dog\_XorBox](cref#dog_XorBox.md)

## `dog_SetContrast`
> Set the display contrast value (0..63).
  * Prototype
```
void dog_SetContrast(uint8_t val) 
```
  * Arguments
    * `val`: Contrast value from 0 to 63.


## `dog_SetHBitmap` `dog_SetHBitmapP` `dog_ClrHBitmapP`
> Bytes at position `bitmap` are interpreted as horizontal line of pixels: A logical
> one of a bit in these bytes will set the value 1 (or value 0) in the graphics memory. A logical
> zero will not alter the graphics memory.
> The horizontal bitmap line will start at `(x,y)`. The length of this horizontal
> line has `w` pixels. `bitmap` must point to a sequence of `(w+7)/8` bytes.
  * Available: v1.01
  * Prototype
```
void dog_SetHBitmap(uint8_t x, uint8_t y, const unsigned char *bitmap, uint8_t w)
```
```
void dog_SetHBitmapP(uint8_t x, uint8_t y, DOG_PGM_P bitmap, uint8_t w)
```
```
void dog_ClrHBitmapP(uint8_t x, uint8_t y, DOG_PGM_P bitmap, uint8_t w)
```
  * Arguments
    * `x,y`: Position of the left end of the horizontal bitmap line.
    * `bitmap`: Pointer to the bitmap line
    * `w`: Number of pixels in the bitmap line
  * See also: [dog\_SetBitmap](cref#dog_SetBitmap.md)
  * Example
```
  const unsigned char bitmap[1] = { 0x81 };
  /* ... */
  dog_StartPage();
  do {
    /* set a pixel at (0,0) and (7,0) */
    dog_SetHBitmap(0,0,bitmap,8);
  } while( dog_NextPage() );
```
```
  /* PROGMEM example */
  dog_pgm_uint8_t bitmap[1] = { 0x81 };
  /* ... */
  dog_StartPage();
  do {
    /* set a pixel at (0,0) and (7,0) */
    dog_SetHBitmapP(0,0,bitmap,8);
  } while( dog_NextPage() );
```

## `dog_SetHLine`
> Set all pixel along the horizontal line from `(x1,y)` to `(x2,y)`. Condition: `x1<=x2`.
  * Available: v1.01
  * Prototype
```
void dog_SetHLine(uint8_t x1, uint8_t x2, uint8_t y)
```
  * Arguments
    * `x1`: Left end of the line
    * `x2`: Right end of the line
    * `y`: Row
  * See also: [dog\_SetVLine](cref#dog_SetVLine.md), [dog\_ClrHLine](cref#dog_ClrHLine.md), [dog\_XorHLine](cref#dog_XorHLine.md)

## `dog_SetInvertPixelMode`
> Enable or disable the invert pixel mode of the display controller.
  * Prototype
```
void dog_SetInvertPixelMode(uint8_t val) 
```
  * Arguments
    * `val`: 0 (normal mode) or 1 (invert pixel mode).

## `dog_SetPixel`
> Set a single pixel.
  * Prototype
```
void dog_SetPixel(uint8_t x, uint8_t y) 
```
  * Arguments
    * `x,y`: Position of the pixel.
  * See also: [dog\_ClrPixel](cref#dog_ClrPixel.md)

## `dog_SetPixelValue`
> Set the pixel value for the DOGXL160 display. Supported values are 0 (no pixel), 1 (light gray), 2 (dark gray) and 3 (black). All `set` procedures are affected by this setting.
  * Available: v1.08
  * Prototype
```
void dog_SetPixelValue(uint8_t value)
```
  * Arguments
    * `val`: Pixel value.
  * See also: [dog\_SetPixel](cref#dog_SetPixel.md)


## `dog_SetVLine`
> Set all pixel along the vertical line from `(x,y1)` to `(x,y2)`. Condition: `y1<=y2`.
  * Prototype
```
void dog_SetVLine(uint8_t x, uint8_t y1, uint8_t y2)
```
  * Arguments
    * `x`: Column
    * `y1`: Lower end of the vertical line
    * `y2`: Upper end of the vertical line
  * See also: [dog\_ClrVLine](cref#dog_ClrVLine.md), [dog\_SetBox](cref#dog_SetBox.md)

## `dog_XorBox`
> Flip all pixel within and including the provided boundaries. `(x1,y1)` is the
> lower left corner, `(x2,y2)` is the upper right corner. Conditions: `x1<=x2` and
> `y1<=y2`.
  * Prototype
```
void dog_XorBox(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
```
  * Arguments
    * `x1,y1`: Lower left edge
    * `x2,y2`: Upper right edge
  * See also: [dog\_SetBox](cref#dog_setBox.md), [dog\_XorVLine](cref#dog_XorVLine.md)

## `dog_XorHLine`
> Flip pixel along the horizontal line from `(x1,y)` to `(x2,y)`. Condition: `x1<=x2`.
  * Available: v1.03
  * Prototype
```
void dog_XorHLine(uint8_t x1, uint8_t x2, uint8_t y)
```
  * Arguments
    * `x1`: Left end of the line
    * `x2`: Right end of the line
    * `y`: Row
  * See also: [dog\_XorVLine](cref#dog_XorVLine.md), [dog\_SetHLine](cref#dog_SetHLine.md), [dog\_ClrHLine](cref#dog_ClrHLine.md)


## `dog_XorPixel`
> Flip a single pixel.
  * Prototype
```
void dog_XorPixel(uint8_t x, uint8_t y) 
```
  * Arguments
    * `x,y`: Position of the pixel.
  * See also: [dog\_SetPixel](cref#dog_SetPixel.md) [dog\_ClrPixel](cref#dog_ClrPixel.md)

## `dog_XorVLine`
> Flip all pixel along the vertical line from `(x,y1)` to `(x,y2)`. Condition: `y1<=y2`.
  * Prototype
```
void dog_XorVLine(uint8_t x, uint8_t y1, uint8_t y2)
```
  * Arguments
    * `x`: Column
    * `y1`: Lower end of the vertical line
    * `y2`: Upper end of the vertical line
  * See also: [dog\_SetVLine](cref#dog_setVLine.md), [dog\_XorBox](cref#dog_XorBox.md)
