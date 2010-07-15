/*

  dogm128.h

  (c) 2010 Oliver Kraus (olikraus@gmail.com)
  
  This file is part of the dogm128 Arduino library.

  The dogm128 Arduino library is free software: you can redistribute it and/or modify
  it under the terms of the Lesser GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  The dogm128 Arduino library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  Lesser GNU General Public License for more details.

  You should have received a copy of the Lesser GNU General Public License
  along with dogm128.  If not, see <http://www.gnu.org/licenses/>.


*/

#ifndef _DOGM128_H
#define _DOGM128_H

#include <stdint.h>
#include <avr/pgmspace.h>
#include "dogmfont.h"

#ifndef PGM_P
#define PGM_P const char *
#endif

#define PIN_SCK   13
#define PIN_MISO  12
#define PIN_MOSI  11
#define PIN_SS    10
#define PIN_A0_DEFAULT     6

#ifdef PROGMEM
#define dog_pgm_read_w(adr) pgm_read_word_near(adr)
#define dog_pgm_read(adr) pgm_read_byte_near(adr)
#else
#define dog_pgm_read_w(adr) (*(const int16_t *)(adr))
#define dog_pgm_read(adr) (*(const char *)(adr)) 
#endif

#ifndef PROGMEM
#define PROGMEM
#endif


#define DOG_WIDTH 128
#define DOG_HEIGHT 64

#define DOG_PAGE_WIDTH 128
#define DOG_PAGE_CNT 8
#define DOG_PAGE_HEIGHT 8


#ifdef __cplusplus
extern "C" {
#endif


extern unsigned char dog_page_buffer[DOG_PAGE_WIDTH];

extern unsigned char dog_spi_pin_a0;
extern uint8_t dog_min_y;
extern uint8_t dog_max_y;

unsigned char dog_spi_out(unsigned char data);
void dog_spi_enable_client(void);
void dog_spi_disable_client(void);
void dog_cmd_mode(void);
void dog_data_mode(void);

  
void dog_init(unsigned short pin_a0);
void dog_set_contrast(uint8_t val);
void dog_set_inverse(uint8_t val);

  
/* --- page functions --- */

void dog_StartPage(void);
uint8_t dog_NextPage(void);

/* --- set/clr functions --- */

extern unsigned char dog_bit_to_mask[8];
  
void dog_SetPixel(uint8_t x, uint8_t y);
void dog_ClrPixel(uint8_t x, uint8_t y);
void dog_XorPixel(uint8_t x, uint8_t y);	/* v1.01 */

/* x1 must be lower or equal to x2 */
void dog_SetHLine(uint8_t x1, uint8_t x2, uint8_t y); /* v1.01 */

uint8_t dog_get_vline_mask(uint8_t y1, uint8_t y2);	/* internal function */
/* y1 must be lower or equal to y2 */
void dog_SetVLine(uint8_t x, uint8_t y1, uint8_t y2);
void dog_ClrVLine(uint8_t x, uint8_t y1, uint8_t y2);
void dog_XorVLine(uint8_t x, uint8_t y1, uint8_t y2);

/* x1 must be lower or equal to x2 */
/* y1 must be lower or equal to y2 */
void dog_SetBox(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
void dog_ClrBox(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
void dog_XorBox(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);

/* --- bitmap functions --- */

/* at position (x,y) set a pixel for each logical one bit in the bitmap pattern */
/* the bitmap must contain (w+7)/8 bytes, each byte is interpreted as bitmap pattern */
/* most significant bit of the byte in the pattern is on the left */
void dog_SetHBitmap(uint8_t x, uint8_t y, const unsigned char *bitmap, uint8_t w); /* v1.01 */
void dog_SetBitmap(uint8_t x, uint8_t y, const unsigned char *bitmap, uint8_t w, uint8_t h); /* v1.01 */

/* --- draw functions --- */
/*
  size = 0:
  +++
  +#+
  +++
  size = 1:
  +#+
  ###
  +#+
*/

void dog_DrawPoint(uint8_t x, uint8_t y, uint8_t size);
void dog_DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t size);
void dog_DrawArc(uint8_t mx, uint8_t my, uint8_t r, uint8_t w0, uint8_t w1, uint8_t size);


/* --- font information --- */
uint8_t dog_get_str_width(PGM_P font, const char *s);

uint8_t dog_GetFontBBXHeight(PGM_P buf);
uint8_t dog_GetFontBBXWidth(PGM_P buf);
uint8_t dog_GetFontBBXDescent(PGM_P buf);
  
uint8_t dog_DrawChar(uint8_t x, uint8_t y, PGM_P font, unsigned char code);
uint8_t dog_DrawStr(uint8_t x, uint8_t y, PGM_P font, const char *s);

/* --- math --- */

  /* w: 0..255     64 = PI/2, 128 = PI, 192 = 3*PI/2 */
  /* returns: -64 .. 64 */
signed char dog_sin(uint8_t w);
signed char dog_cos(uint8_t w);

/* --- other --- */
char *dog_itoa(unsigned long v);


/* --- breakout --- */

void bo_Setup(uint8_t level);
void bo_Draw(void);
void bo_Step(void);
void bo_SetX(uint8_t pos);


#ifdef __cplusplus
}
#endif


#endif 

