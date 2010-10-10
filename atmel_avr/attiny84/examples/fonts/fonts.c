/*

  Fonts.c
    
  The attiny84 port of the fonts example
  Not all fonts are shown, because they do not all fit into 8K flash memory
    
  (c) 2010 Oliver Kraus (olikraus@gmail.com)
  
  This file is part of the dogm128 library.

  The dogm128 library is free software: you can redistribute it and/or modify
  it under the terms of the Lesser GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  The dogm128 library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  Lesser GNU General Public License for more details.

  You should have received a copy of the Lesser GNU General Public License
  along with dogm128.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "dogm128.h"


void system_init(void)
{
#if defined(__AVR__)
  /* select minimal prescaler (max system speed) */
  CLKPR = 0x80;
  CLKPR = 0x00;
#endif
}

uint8_t font_select = 0;

void loop(void) {
  uint8_t x;
  DOG_PSTR_P f;
  DOG_PGM_P p;
  unsigned char h;
  
  switch(font_select) {
      default: font_select = 0; /* no break */
      case 0: h = 7; p = font_4x6; f = DOG_PSTR("4x6"); break;
      case 1: h = 8; p = font_5x7; f = DOG_PSTR("5x7"); break;
      case 2: h = 9; p = font_5x8; f = DOG_PSTR("5x8"); break;
      case 3: h = 9; p = font_6x9; f = DOG_PSTR("6x9"); break;
      case 4: h = 10; p = font_6x10; f = DOG_PSTR("6x10"); break;
      case 5: h = 10; p = font_6x12; f = DOG_PSTR("6x12"); break;
   }
  font_select++;
  
  dog_StartPage();
  do {
    x = 0;
    x+=dog_DrawStrP(x,h*0+3, p, f);
    dog_SetBox(0, h*0+3, dog_GetStrWidthP(p,f), h*0+3 );
    x+=dog_DrawStrP(x,h*0+3, p, DOG_PSTR(": 0123456789.,;"));
    dog_DrawStrP(0,h*2+3, p, DOG_PSTR("abcdefgjijklmnopqrstuvwxyz"));
    dog_DrawStrP(0,h*1+3, p, DOG_PSTR("ABCDEFGHIJKLMNOPQRSTUVWXYZ"));
    dog_DrawStrP(0,h*3+3, p, DOG_PSTR("+-*/#-!$%&(){}[]"));
  } while( dog_NextPage() );
  dog_Delay(1000);
}


int main(void)
{
  system_init();
  dog_Init(0);

  for(;;)
  {
    loop();
  }
}

