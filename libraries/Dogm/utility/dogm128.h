

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


void dog_StartPage(void);
uint8_t dog_NextPage(void);

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

uint8_t dog_DrawChar(uint8_t x, uint8_t y, PGM_P font, unsigned char code);
uint8_t dog_DrawStr(uint8_t x, uint8_t y, PGM_P font, const char *s);

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

