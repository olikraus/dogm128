/*

  dogm128.c
  
  (c) 2010 Oliver Kraus (olikraus@gmail.com)
  
  graphic functions for the dogm128 graphics module 
  with ST7565R controller from electronic assembly
  
  optimized c-code for avr-gcc


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


  size = 0:
  +++
  +#+
  +++
  size = 1:
  +#+
  ###
  +#+

  ST7565R SPI interface
  
    ST7565R reads data with rising edge of the clock signal (SCL)
      --> CPHA = 0 and CPOL = 0
    ST7565R reads  MSB first 
      --> DORD = 0
      
  /usr/lib/avr/include/avr/pgmspace.h

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dogm128.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <wiring.h>	/* arduino pinMode */

#ifdef PROGMEM
#define dog_pgm_read_w(adr) pgm_read_word_near(adr)
#define dog_pgm_read(adr) pgm_read_byte_near(adr)
#else
#define dog_pgm_read_w(adr) (*(const int16_t *)(adr))
#define dog_pgm_read(adr) (*(const char *)(adr)) 
#endif

#define DOG_WIDTH 128
#define DOG_HEIGHT 64

#define DOG_PAGE_WIDTH 128
#define DOG_PAGE_CNT 8
#define DOG_PAGE_HEIGHT 8

unsigned char dog_page_buffer[DOG_PAGE_WIDTH];
uint8_t dog_curr_page = 0;	/* 0...DOG_PAGE_CNT-1 */
uint8_t dog_min_y = 0;
uint8_t dog_max_y = DOG_PAGE_HEIGHT-1;




/*==============================================================*/
/* 
  SPI subsystem  for Arduino
  Pins 15 to 19 (ATMEGA8) or 10 to 13 (Arduino) are used.
*/
/*==============================================================*/



unsigned char dog_spi_pin_a0 = PIN_A0_DEFAULT;

void dog_init_spi(void)
{
  pinMode(PIN_SCK, OUTPUT);
  pinMode(PIN_MOSI, OUTPUT);
  //pinMode(PIN_MISO, INPUT); 
  pinMode(PIN_SS, OUTPUT);
  pinMode(dog_spi_pin_a0, OUTPUT);
  
  /*
    SPR1 SPR0
	0	0		fclk/4
	0	1		fclk/16
	1	0		fclk/64
	1	1		fclk/128
  */
  SPCR = 0;
  SPCR =  (1<<SPE) | (1<<MSTR)|(0<<SPR1)|(0<<SPR0)|(0<<CPOL)|(0<<CPHA);
  /*
  {
  unsigned char x;
    x = SPSR;
    x= SPDR;
  }
  */
}

unsigned char dog_spi_out(unsigned char data)
{
  
  /* unsigned char x = 100; */
  /* send data */
  SPDR = data;
  /* wait for transmission */
  while (!(SPSR & (1<<SPIF))) 
    ;
  /* clear the SPIF flag by reading SPDR */
  return  SPDR;
}

void dog_spi_enable_client(void)
{
        digitalWrite(PIN_SS, LOW);  
}

void dog_spi_disable_client(void)
{
        digitalWrite(PIN_SS, HIGH);
}

void dog_cmd_mode(void)
{
  digitalWrite(dog_spi_pin_a0, LOW);
}

void dog_data_mode(void)
{
  digitalWrite(dog_spi_pin_a0, HIGH);
}

void dog_delay(unsigned short val)
{
  delay(val);
}

void dog_init_display(void)
{
  dog_delay(10);
  dog_spi_disable_client();	/* force reset of the spi subsystem of the ST7565R */
  dog_delay(10);
  dog_spi_enable_client();
  dog_delay(10);
  dog_cmd_mode();
  /* mostly taken from the EA dogm description */
  dog_spi_out(0x040);		/* set display start line to 0 */
  dog_spi_out(0x0a1);		/* ADC set to reverse */
  dog_spi_out(0x0c0);		/* common output mode */
  dog_spi_out(0x0a6);		/* display normal, bit val 0: LCD pixel off. */
  dog_spi_out(0x0a2);		/* LCD bias 1/9 */
  dog_spi_out(0x02f);		/* all power  control circuits on */
  dog_spi_out(0x0f8);		/* set booster ratio to */
  dog_spi_out(0x000);		/* 4x */
  dog_spi_out(0x027);		/* set V0 voltage resistor ratio to large */
  dog_spi_out(0x081);		/* set contrast */
  dog_spi_out(0x018);		/* contrast value, EA default: 0x016 */
  dog_spi_out(0x0ac);		/* indicator */
  dog_spi_out(0x000);		/* disable */
  dog_spi_out(0x0af);		/* display on */
  dog_spi_out(0x0a5);		/* display all points */
  dog_delay(300);
  dog_spi_out(0x0a4);		/* normal display  */
  dog_delay(300);
  dog_spi_disable_client();
}

void dog_init(unsigned short pin_a0)
{
  dog_delay(50);			/* initial delay */
  dog_spi_pin_a0 = pin_a0;
  dog_delay(10);
  dog_init_spi();
  dog_init_display();
}

void dog_set_contrast(uint8_t val)
{
  val &= 63;
  dog_spi_enable_client();  
  dog_cmd_mode();
  dog_spi_out(0x081);		/* set contrast */
  dog_spi_out(val);		/* contrast value */
  dog_data_mode();
  dog_spi_disable_client();
}

void dog_set_inverse(uint8_t val)
{
  val &= 1;
  val |= 0x0a6;			/* invert pixel mode */
  dog_spi_enable_client();  
  dog_cmd_mode();
  dog_spi_out(val);		
  dog_data_mode();
  dog_spi_disable_client();
}

void dog_transfer_page(void)
{
  signed char idx;

  /* enable and reset com interface of the ST7565R */
  dog_spi_enable_client();
  
  /* set write position */
  dog_cmd_mode();
  
  dog_spi_out(0x0b0 | dog_curr_page );		/* select current page  */
  dog_spi_out(0x010 );		/* set upper 4 bit of the col adr to 0 */
  dog_spi_out(0x000 );		/* set lower 4 bit of the col adr to 0 */
  
  /* send a complete page */
  
  dog_data_mode();
  
  idx = 127;
  while( idx >= 0 )
  {
    dog_spi_out(dog_page_buffer[idx]); 
    idx--;
  }

  /* disable com interface of the ST7565R */

  dog_spi_disable_client();
}


/*==============================================================*/
/* page buffer functions */
/*==============================================================*/

void dog_ClearPage(void)
{
  uint8_t i;
  for( i = 0; i < DOG_PAGE_WIDTH; i++ )
    dog_page_buffer[i] = 0;
}

void dog_StartPage(void)
{
  dog_curr_page = 0;
  dog_min_y = 0;
  dog_max_y = DOG_PAGE_HEIGHT-1;
  dog_ClearPage();
}

uint8_t dog_NextPage(void)
{
  dog_transfer_page();
  dog_ClearPage();
  dog_curr_page++;
  if ( dog_curr_page >= DOG_PAGE_CNT )
      return 0;
  
  dog_min_y = DOG_PAGE_HEIGHT;
  dog_min_y *= dog_curr_page;
  dog_max_y = dog_min_y;
  dog_max_y += DOG_PAGE_HEIGHT-1;
  return 1;
}

/*==============================================================*/
/* basic graphic functions */
/*==============================================================*/

unsigned char dog_bit_to_mask[8] = { 1, 2, 4, 8, 16, 32, 64, 128 };

void dog_SetPixel(uint8_t x, uint8_t y)
{
  unsigned char tmp;
  if ( x < 128 )
    if ( y >= dog_min_y && y <=dog_max_y )
    {
      tmp = y;
      tmp &= (unsigned char)7;
      tmp = dog_bit_to_mask[tmp];
      dog_page_buffer[x] |= tmp;
    }
}

void dog_ClrPixel(uint8_t x, uint8_t y)
{
  unsigned char tmp;
  if ( x < 128 )
    if ( y >= dog_min_y && y <=dog_max_y )
    {
      tmp = y;
      tmp &= (unsigned char)7;
      tmp = dog_bit_to_mask[tmp];
      tmp ^= 0x0ff;
      dog_page_buffer[x] &= tmp;
    }
}

unsigned char dog_mask0[8] = { 0x01, 0x03, 0x07, 0x0f, 0x1f, 0x3f, 0x7f, 0xff };
unsigned char dog_mask7[8] = { 0xff, 0xfe, 0xfc, 0xf8, 0xf0, 0xe0, 0xc0, 0x80 };

static uint8_t dog_get_vline_mask(uint8_t y1, uint8_t y2)
{
  uint8_t tmp;
  if ( y1 < dog_min_y )
  {
    if ( y2 >= dog_max_y )
    {
      /* all bits */
      tmp  = 0xff;
    }
    else
    {
      tmp = y2;
      tmp &= (unsigned char)7;
      /* bits, starting at bit-position 0 */
      tmp = dog_mask0[tmp];
    }
  }
  else
  {
    if ( y2 >= dog_max_y )
    {
      /* bits, ending at bit-position 7 */
      tmp = y1;
      tmp &= (unsigned char)7;
      tmp = dog_mask7[tmp];
    }
    else
    {
      /* bits, somewhere in the middle */
      uint8_t t2;
      tmp = y1;
      tmp &= (unsigned char)7;
      tmp = dog_mask7[tmp];
      t2 = y2;
      t2 &= (unsigned char)7;
      t2= dog_mask0[t2];
      tmp &= t2;
    }
  }
  return tmp;
}

/* y1 must be lower or equal to y2 */
void dog_SetVLine(uint8_t x, uint8_t y1, uint8_t y2)
{
  unsigned char *ptr;
  if ( x >= 128 )
    return;
  if ( y2 < dog_min_y )
    return;
  if ( y1 > dog_max_y )
    return;
  ptr = dog_page_buffer+x; 
  *ptr |= dog_get_vline_mask(y1, y2);
}

/* y1 must be lower or equal to y2 */
void dog_ClrVLine(uint8_t x, uint8_t y1, uint8_t y2)
{
  uint8_t tmp;
  unsigned char *ptr;
  if ( y2 < dog_min_y )
    return;
  if ( y1 > dog_max_y )
    return;
  ptr = dog_page_buffer+x; 
  tmp = dog_get_vline_mask(y1, y2);
  tmp ^= 0x0ff;
  *ptr &= tmp;
}

/* y1 must be lower or equal to y2 */
void dog_XorVLine(uint8_t x, uint8_t y1, uint8_t y2)
{
  uint8_t tmp, data;
  unsigned char *ptr;
  if ( y2 < dog_min_y )
    return;
  if ( y1 > dog_max_y )
    return;
  tmp = dog_get_vline_mask(y1, y2);
  ptr = dog_page_buffer+x; 
  data = *ptr;
  data ^= tmp;
  *ptr = data;
}

/*==============================================================*/
/* math functions */
/*==============================================================*/

const signed char dog_sin_tab[65] = {
  0,2,3,5,6,8,9,11,12,14,16,17,19,20,22,23,24,26,27,29,
  30,32,33,34,36,37,38,39,41,42,43,44,45,46,47,48,49,50,51,52,
  53,54,55,56,56,57,58,59,59,60,60,61,61,62,62,62,63,63,63,64,
  64,64,64,64,64};
  
  /* w: 0..255     64 = PI/2, 128 = PI, 192 = 3*PI/2 */
  /* returns: -64 .. 64 */
signed char dog_sin(uint8_t w)
{
  uint8_t case_w = (w>>6)&3;
  uint8_t idx_w = w & 63;
  signed char result = 0;
  switch(case_w)
  {
    case 0:
      //result = dog_pgm_read(dog_sin_tab+idx_w);
      result = dog_sin_tab[idx_w];
      break;
    case 1:
      //result = dog_pgm_read(dog_sin_tab+64-idx_w);
      result = dog_sin_tab[64-idx_w];
      break;
    case 2:
      //result = -dog_pgm_read(dog_sin_tab+idx_w);
      result = -dog_sin_tab[idx_w];
      break;
    case 3:
      //result = -dog_pgm_read(dog_sin_tab+64-idx_w);
      result = -dog_sin_tab[64-idx_w];
      break;
  }
  return result;
}

signed char dog_cos(uint8_t w)
{
  return dog_sin( (w+64U) );
}

/*==============================================================*/
/* box graphic functions */
/*==============================================================*/

void dog_SetBox(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
  uint8_t x;
  for ( x = x1; x <= x2; x++ )
    dog_SetVLine(x,y1,y2);
}

void dog_ClrBox(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
  uint8_t x;
  for ( x = x1; x <= x2; x++ )
    dog_ClrVLine(x,y1,y2);
}

void dog_XorBox(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
  uint8_t x;
  for ( x = x1; x <= x2; x++ )
    dog_XorVLine(x,y1,y2);
}

/*==============================================================*/
/* draw graphic functions */
/*==============================================================*/

void dog_DrawPoint(uint8_t x, uint8_t y, uint8_t size)
{
  if ( size == 0 ) 
  {
    dog_SetPixel(x,y);
  }
  else if ( size == 1 )
  {
    dog_SetPixel(x,y);
    dog_SetPixel(x+1,y);
    dog_SetPixel(x,y+1);
    if ( x > 0 ) dog_SetPixel(x-1,y);
    if ( y > 0 ) dog_SetPixel(x,y-1);
  }
  else
    dog_SetPixel(x,y);
}

void dog_DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t size)
{
  uint8_t tmp;
  uint8_t x,y;
  uint8_t dx, dy;
  signed char err;
  uint8_t swapxy = 0;
  signed char ystep;
  if ( x1 > x2 ) dx = x1-x2; else dx = x2-x1;
  if ( y1 > y2 ) dy = y1-y2; else dy = y2-y1;
  if ( dy > dx ) 
  {
    swapxy = 1;
    tmp = dx; dx =dy; dy = tmp;
    tmp = x1; x1 =y1; y1 = tmp;
    tmp = x2; x2 =y2; y2 = tmp;
  }
  if ( x1 > x2 ) 
  {
    tmp = x1; x1 =x2; x2 = tmp;
    tmp = y1; y1 =y2; y2 = tmp;
  }
  err = dx >> 1;
  if ( y2 > y1 ) ystep = 1; else ystep = -1;
  y = y1;
  for( x = x1; x <= x2; x++ )
 {
   if ( swapxy == 0 ) 
     dog_DrawPoint(x,y,size); 
   else 
     dog_DrawPoint(y,x,size);
   err -= (uint8_t)dy;
   if ( err < 0 ) 
   {
     y += (uint8_t)ystep;
     err += (uint8_t)dx;
   }
 }  
}

/*
  mx, my		center of the arc/circle
  r			radius in pixel
  w0			start angle (counter clock direction, zero at the left)
  w1			end angle
  size		line thickness
  
  Notes:
  1) A circle will be drawn, if w0 == w1
  2) angle: 128 = Pi
*/
void dog_DrawArc(uint8_t mx, uint8_t my, uint8_t r, uint8_t w0, uint8_t w1, uint8_t size)
{
  uint8_t l,i,w,x1,y1,x2,y2;
  unsigned short dw;
  dw = w1-w0;
  if ( dw == 0 )
    dw = 256;
  
  /* laenge des bogens berechnen l = 2*r*pi*(w1-w0)/256 */
  /* pi wird mit 201/64 = 3.140625 approximiert */ 
  /* l = (2 * r * 201 * (w1-w0)) >> ( 6+8 ) = (r * 201 * (w1-w0)) >> 13 */
  /* das passt nicht in einen unsigned 16 datentyp */
  /* annahme: r < 41 (dann ist der umfang 251 und passt noch in einen u8*/
  /* l = (((r * (w1-w0)) >> 6) * 201)>>7 */
  /* allerdings zeichnen wir dann doch lieber sekanten */
  /* derzeit alle 4 pixel, also muss l abermals um 2 geshiftet werden */
  /* die beiden shifts werden auf die beiden existierenden verteilt, damit sind radien bis 80 zugelassen */
  
  
  l = (uint8_t)(((((unsigned short)r * dw) >> 7) * (unsigned short)201)>>8) ;
  
  
  x1 = mx+((r*dog_cos(w0)) >> 6);
  y1 = my+((r*dog_sin(w0)) >> 6);
  for ( i = 1; i <= l; i++ )
  {
    w = ((unsigned short)dw*(unsigned short)i )/(unsigned short)l + w0;
    x2 = mx+((r*dog_cos(w)) >> 6);
    y2 = my+((r*dog_sin(w)) >> 6);
    dog_DrawLine(x1,y1,x2,y2,size);
    x1 = x2;
    y1 = y2;
  }
}

/*==============================================================*/
/* font functions */

/*
  FONT
  offset	size
  0		word	capital A index
  2		word	small a index
  4		byte		bbox width
  5		byte		bbox height
  6		byte		bbox descent
  7-		variable (characters with code 0x20 - 0x7f)
  
  
  CHARACTER (Format 1)
  offset	size
  0		byte			total size of the structure, including this byte
  1		bits 0..4		width
  1		bits 5..7		descent
  2		byte			height
  3-		varable (bitmap, topmost line first)
  
  +====+  ====							====
  |		|								upper space (ascent area)
  |		|						====	====
  |		|
  |		|  bbox					height
  |		|  height
  |=====|		====	====							====baseline===
  |		|		bbox d	descent
  |		|				====	====	====
  |		|								lower space (descent area)
  +====+  ====	====					====		
  
  upper space + lower space = bbox height - height
  lower space = bbox descent - descent
  upper space = bbox height + descent - height - bbox descent 

*/

#define fnt_get_bbox_capital_a(buf) (dog_pgm_read_w((buf)+0))
#define fnt_get_bbox_small_a(buf) (dog_pgm_read_w((buf)+2))
#define fnt_get_bbox_width(buf) (dog_pgm_read((buf)+4))
#define fnt_get_bbox_height(buf) (dog_pgm_read((buf)+5))
#define fnt_get_bbox_descent(buf) (dog_pgm_read((buf)+6))
#define FNT_DATA 7


#define chr_get_len_P(buf) ((unsigned char)(dog_pgm_read((buf)+0)))
#define chr_get_descent_P(buf) (((unsigned char)(dog_pgm_read((buf)+1)))  >>5)
#define chr_get_width_P(buf) ((unsigned char)(dog_pgm_read((buf)+1) & (unsigned char)0x1f))
#define chr_get_height_P(buf) ((unsigned char)(dog_pgm_read((buf)+2)))
#define CHR_DATA 3

uint8_t dog_GetFontBBXHeight(PGM_P buf)
{
  return fnt_get_bbox_height(buf);
}

uint8_t dog_GetFontBBXWidth(PGM_P buf)
{
  return fnt_get_bbox_width(buf);
}

uint8_t dog_GetFontBBXDescent(PGM_P buf)
{
  return fnt_get_bbox_descent(buf);
}

static unsigned char chr_get_pixel(PGM_P buf, unsigned char x, unsigned char y )
{
  unsigned char bytes_per_line, index;
  /* bytes_per_line = ((unsigned char)chr_get_width(buf)+(unsigned char)7) >> 3; */
  bytes_per_line = chr_get_width_P(buf);
  bytes_per_line += (unsigned char)7;
  bytes_per_line >>= (unsigned char)3;
  index = y;
  index *= bytes_per_line;
  index += (unsigned char)(x>>3);
  index += CHR_DATA;
    
  if ( ((unsigned char)dog_pgm_read(buf+index)) & ( (unsigned char)1 << (x&(unsigned char)7)) ) 
    return 1;
  return 0;
}


unsigned short chr_get_skip_delta(PGM_P cbuf, unsigned char skip_cnt)
{
  unsigned short pos = 0;
  unsigned char len;
  while( skip_cnt > 0 )
  {
    len = chr_get_len_P(cbuf + pos);
    if ( len == 0 )				
      return pos;			/* should never happen */
    pos += (short)len;
    skip_cnt--;
  }
  return pos;
}

PGM_P fnt_get_chr(PGM_P  fbuf, unsigned char code)
{
  PGM_P adr;

  if ( code >= 'a' )
  {
    adr = fbuf + fnt_get_bbox_small_a(fbuf);
    adr = adr + chr_get_skip_delta(adr, code - 'a');
  }
  else if ( code >= 'A' )
  {
    adr = fbuf + fnt_get_bbox_capital_a(fbuf);
    adr = adr + chr_get_skip_delta(adr, code - 'A');
  }
  else
  {
    adr = fbuf + FNT_DATA + chr_get_skip_delta(fbuf+FNT_DATA, code - ' ');
  }
  return adr;
}

uint8_t dog_get_char_width(PGM_P font, unsigned char code)
{
  PGM_P adr;
  /* adr = font + FNT_DATA + chr_get_skip_delta(font+FNT_DATA, code - ' '); */
  adr = fnt_get_chr(font, code);
  return chr_get_width_P(adr);
}

uint8_t dog_char(uint8_t x, uint8_t y, PGM_P font, uint8_t mode, unsigned char code)
{
  PGM_P cbuf = fnt_get_chr(font, code);
  unsigned char ascent_area, descent_area;
  unsigned char  i, j;
  uint8_t tmp_y;
  uint8_t tmp_x;
  uint8_t height,width;

  if ( chr_get_len_P(cbuf) == 0 )
    return 0;
  height = chr_get_height_P(cbuf);
  width= chr_get_width_P(cbuf);
  
  descent_area = fnt_get_bbox_descent(font);
  descent_area  -= chr_get_descent_P(cbuf);
  ascent_area = fnt_get_bbox_height(font);
  ascent_area -= height; 
  ascent_area -= descent_area;

  /*
  printf("code %c, bbox height %d,  aa %d, da %d, char descent %d\n", code, fnt_get_bbox_height(font), ascent_area, descent_area, chr_get_descent(cbuf) );
  */
  
  /*
    ascent_area starts at 		y + chr_get_height(cbuf) - chr_get_descent(cbuf) + ascent_area
    char code starts at		y + chr_get_height(cbuf) - chr_get_descent(cbuf)
    descent area starts at		y - chr_get_descent(cbuf)
    descent area ends at		y - chr_get_descent(cbuf) - descent_area + 1
  */
  
  
  /*tmp_y = y + chr_get_height(cbuf) - chr_get_descent(cbuf) ;*/
  tmp_y = y;
  tmp_y += height;
  tmp_y -= chr_get_descent_P(cbuf);

  /* char goes from tmp_y to tmp_y-height-1 and x .. x + width -1*/
  /* y test is done in dog_str */
  /*
  if ( dog_max_y < tmp_y - height + 1 )
    return width;
  if ( dog_min_y > tmp_y )
    return width;
 */

  for( i = height; i > 0; i-- )
  {
    tmp_x = x;
    for( j = 0; j < width; j++ )
    {
      if ( chr_get_pixel(cbuf, j, i-1 ) > 0 )
	dog_SetPixel(tmp_x, tmp_y);
      /*
      else
	dog_ClrPixel(tmp_x, tmp_y);
      */
      tmp_x++;
    }
    tmp_y--;
  }
    
  return width;
}

uint8_t dog_DrawChar(uint8_t x, uint8_t y, PGM_P font, unsigned char code)
{
  signed char y0, y1;
  y0 = y;
  y0 -= fnt_get_bbox_descent(font);  
  y1 = y0;
  y1 += fnt_get_bbox_height(font);  
  
  if ( (signed char)dog_max_y < y0 )
    return dog_get_char_width(font, code);
  if ( (signed char)dog_min_y > y1 )
    return dog_get_char_width(font, code);
  
  return dog_char(x,y,font,0,code);
}



uint8_t dog_get_str_width(PGM_P font, const char *s)
{
  uint8_t w = 0;
  while( *s != '\0' )
  {
    w	+= dog_get_char_width(font, (unsigned char)*s);
    s++;
  }
  return w;
}

uint8_t dog_str(uint8_t x, uint8_t y, PGM_P font, uint8_t mode, const char *s)
{
  uint8_t d = 0;
  signed char y0, y1;
  y0 = y;
  y0 -= fnt_get_bbox_descent(font);  
  y1 = y0;
  y1 += fnt_get_bbox_height(font);
  
  
  if ( (signed char)dog_max_y < y0 )
    return dog_get_str_width(font, s);
  if ( (signed char)dog_min_y > y1 )
    return dog_get_str_width(font, s);
  
  while( *s != '\0' )
  {
    d += dog_char(x+d,y,font, mode, (unsigned char)*s);
    s++;
  }
  return d;
}

uint8_t dog_DrawStr(uint8_t x, uint8_t y, PGM_P font, const char *s)
{
  return dog_str(x,y,font,0,s);
}

char dog_itoa_buf[12];

char *dog_itoa(unsigned long v)
{
  volatile unsigned char i = 11;
  dog_itoa_buf[11] = '\0';
  while( i > 0)
  {
      i--;
      dog_itoa_buf[i] = (v % 10)+'0';
      v /= 10;
      if ( v == 0 )
	break;
  }
  return dog_itoa_buf+i;
}


