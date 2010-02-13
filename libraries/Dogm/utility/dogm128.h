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

#ifndef PROGMEM
#define PROGMEM
#endif

#ifndef PGM_P
#define PGM_P const char *
#endif

#define PIN_SCK   13
#define PIN_MISO  12
#define PIN_MOSI  11
#define PIN_SS    10
#define PIN_A0_DEFAULT     6



#ifdef __cplusplus
extern "C" {
#endif


extern unsigned char dog_spi_pin_a0;
  
void dog_init(unsigned short pin_a0);
void dog_set_contrast(uint8_t val);
void dog_set_inverse(uint8_t val);

  
/* --- page functions --- */

void dog_StartPage(void);
uint8_t dog_NextPage(void);

/* --- set/clr functions --- */
  
void dog_SetPixel(uint8_t x, uint8_t y);
void dog_ClrPixel(uint8_t x, uint8_t y);

/* y1 must be lower or equal to y2 */
void dog_SetVLine(uint8_t x, uint8_t y1, uint8_t y2);
void dog_ClrVLine(uint8_t x, uint8_t y1, uint8_t y2);
void dog_XorVLine(uint8_t x, uint8_t y1, uint8_t y2);

/* x1 must be lower or equal to x2 */
/* y1 must be lower or equal to y2 */
void dog_SetBox(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
void dog_ClrBox(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
void dog_XorBox(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);

/* --- draw functions --- */

void dog_DrawPoint(uint8_t x, uint8_t y, uint8_t size);
void dog_DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t size);
void dog_DrawArc(uint8_t mx, uint8_t my, uint8_t r, uint8_t w0, uint8_t w1, uint8_t size);

uint8_t dog_DrawChar(uint8_t x, uint8_t y, PGM_P font, unsigned char code);
uint8_t dog_DrawStr(uint8_t x, uint8_t y, PGM_P font, const char *s);

/* --- font information --- */
uint8_t dog_GetFontBBXHeight(PGM_P buf);
uint8_t dog_GetFontBBXWidth(PGM_P buf);
uint8_t dog_GetFontBBXDescent(PGM_P buf);
  

/* --- util --- */

  /* w: 0..255     64 = PI/2, 128 = PI, 192 = 3*PI/2 */
  /* returns: -64 .. 64 */
signed char dog_sin(uint8_t w);
signed char dog_cos(uint8_t w);

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

