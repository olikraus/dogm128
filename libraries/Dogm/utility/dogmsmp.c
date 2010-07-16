/*

  dogmsmp.c


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

#include "dogm128.h"

/* at position (x,y) set a pixel for each logical one bit in the bitmap pattern */
/* the bitmap must contain (w+7)/8 bytes, each byte is interpreted as bitmap pattern */
/* most significant bit of the byte in the pattern is on the left */
void dog_SetHBitmapP(uint8_t x, uint8_t y, PGM_P bitmap, uint8_t w)
{
  uint8_t i, tmp, b;
  if ( x < 128 )
    if ( y >= dog_min_y && y <=dog_max_y )
    {
      tmp = y;
      tmp &= (uint8_t)7;
      tmp = dog_bit_to_mask[tmp];
      
      if ( x+w > 128 )
      {
	w = 128;
	w -= x;
      }
      b = 0;
      for( i = 0; i < w; i++ )
      {
	if ( (i & 7) == 0 )
	{
	  b = dog_pgm_read(bitmap);
	  bitmap++;
	}
	if ( b & 128 )
	  dog_page_buffer[x] |= tmp;
	b<<=1;
	x++;
      }
    }  
}

/* NOTE: (x,y) is the upper left corner of the bitmap !!! */
void dog_SetBitmapP(uint8_t x, uint8_t y, PGM_P bitmap, uint8_t w, uint8_t h)
{
  uint8_t j;
  for( j = 0; j < h; j++)
  {
    dog_SetHBitmapP(x, y, bitmap, w);
    bitmap += (w+7)  / 8;
    if ( y == 0 )
      break;
    y--;
  }
}



