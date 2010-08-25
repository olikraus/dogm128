/*

  Fonts.c
    
  The attiny84 port of the fonts example
  Not all fonts are shown, because they do not all fit into 8K flash memory
    
  (c) 2010 Oliver Kraus (olikraus@gmail.com)
  
  This file is part of the dogm128 library.

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


void system_init(void)
{
  /* select minimal prescaler (max system speed) */
  CLKPR = 0x80;
  CLKPR = 0x00;
}

uint8_t font_select = 0;

void loop() {
  uint8_t x;
  char *f;
  PGM_P p;
  unsigned char h;
  
  switch(font_select) {
      default: font_select = 0; /* no break */
      case 0: h = 7; p = font_4x6; f = "4x6"; break;
      case 1: h = 8; p = font_5x7; f = "5x7"; break;
      case 2: h = 9; p = font_5x8; f = "5x8"; break;
      case 3: h = 9; p = font_6x9; f = "6x9"; break;
      case 4: h = 10; p = font_6x10; f = "6x10"; break;
      case 5: h = 10; p = font_6x12; f = "6x12"; break;
      /* 
      case 6: h = 11; p = font_6x13; f = "6x13"; break;
      case 7: h = 11; p = font_7x13; f = "7x13"; break;
      case 8: h = 12; p = font_7x14; f = "7x14"; break;
      case 9: h = 12; p = font_8x13; f = "8x13"; break;
      case 10: h = 15; p = font_8x16; f = "8x16"; break; 
      */
   }
  font_select++;
  
  dog_StartPage();
  do {
    x = 0;
    x+=dog_DrawStr(x,h*0+3, p, f);
    dog_SetBox(0, h*0+3, dog_get_str_width(p,f), h*0+3 );
    x+=dog_DrawStr(x,h*0+3, p, ": 0123456789.,;");
    dog_DrawStr(0,h*2+3, p, "abcdefgjijklmnopqrstuvwxyz");
    dog_DrawStr(0,h*1+3, p, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    dog_DrawStr(0,h*3+3, p, "+-*/#-!$%&(){}[]");
  } while( dog_NextPage() );
  dog_delay(1000);
}


int main(void)
{
  system_init();
  dog_init(0);

  for(;;)
  {
    loop();
  }
}

