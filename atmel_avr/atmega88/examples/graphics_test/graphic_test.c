/*

  graphic_test.c
    
  The attiny84 port of the graphics_test example
    
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


void all_page(void)
{
  
    // draw a cross at the DOGS x-boundary
    dog_SetVLine(101,0, 3);
    dog_SetVLine(102,1, 4);
    dog_SetHLine(100,103, 2);

    // draw a cross at the DOGM132 y-boundary  
    dog_SetHLine(98,103, 31);
    dog_SetHLine(99,104, 32);
    dog_SetVLine(101,30, 34);
}


void page0(void)
{
    all_page();
    //dog_SetFont(font_5x8);
    //dog_SetXY(20,10);
    dog_DrawStrP(20, 10, font_5x8, DOG_PSTR("Set Test"));
    dog_SetBox(5,20,DOG_WIDTH-5,43);
    dog_SetVLine(80,0, DOG_HEIGHT);
    dog_SetHLine(0,DOG_WIDTH-1, 2);
    dog_SetPixel(10, 17);
    dog_SetPixel(12, 17);
    dog_SetPixel(14, 17);
}

void page1(void)
{
    all_page();
    dog_DrawStrP(20, 10, font_5x8, DOG_PSTR("Clr Test"));
    dog_SetBox(5,20,DOG_WIDTH-5,43);
    dog_ClrBox(20,30,30,40);
    dog_ClrVLine(80,0, DOG_HEIGHT);
    dog_ClrPixel(10, 27);
    dog_ClrPixel(12, 27);
    dog_ClrPixel(14, 27);
    dog_ClrPixel(10, 17);
    dog_ClrPixel(12, 17);
    dog_ClrPixel(14, 17);
    dog_ClrHLine(16, 23, 27);
}

void page2(void)
{
    all_page();
    dog_DrawStrP(20, 16, font_5x8, DOG_PSTR("Xor Test"));
    dog_XorBox(5,20,90,43);
    dog_XorVLine(80,0, DOG_HEIGHT);
    dog_XorPixel(10, 27);
    dog_XorPixel(12, 27);
    dog_XorPixel(14, 27);
    dog_XorPixel(10, 17);
    dog_XorPixel(12, 17);
    dog_XorPixel(14, 17);
    dog_XorHLine(16, 127, 27);
}

void page3(void)
{
    all_page();
    dog_DrawStrP(2, 24, font_5x8, DOG_PSTR("Draw Test"));
    dog_DrawLine(0,0,127,20, 1);
    dog_DrawLine(0,1,127,30, 1);
    dog_DrawLine(0,2,127,40, 0);
    dog_DrawLine(0,3,127,50, 0);
    dog_DrawPoint(10,20, 0);
    dog_DrawPoint(16,20, 0);
    dog_DrawArc(53, 40, 10, 192, 64, 0); 
    dog_DrawPoint(13,20, 1);
    dog_DrawArc(53, 40, 14, 192, 64, 1); 
}

void page4(void)
{
    all_page();
    dog_DrawPoint(0,0, 1);
    dog_DrawRStrP(0, 0, 0, font_5x8, DOG_PSTR("Text r=0"));
    dog_DrawPoint(50,5, 1);
    dog_DrawRStrP(50, 5, 1, font_5x8, DOG_PSTR("T r=1"));
    dog_DrawPoint(80,20, 1);
    dog_DrawRStrP(80, 20, 2, font_5x8, DOG_PSTR("T r=2"));
    dog_DrawPoint(90,25, 1);
    dog_DrawRStrP(90, 25, 3, font_5x8, DOG_PSTR("T r=3"));
}

void page5(void)
{
  char s_buf[4];
  char *s_ram = s_buf;
  DOG_PSTR_P s_rom;
  s_buf[0] = 'R';
  s_buf[1] = 'A';
  s_buf[2] = 'M';
  s_buf[3] = '\0';
  s_rom = DOG_PSTR("ROM");

  all_page();
  dog_DrawStrP(20, 10, font_5x8, DOG_PSTR("RAM/ROM Test"));
  
  dog_DrawStr(10, 1, font_5x8, s_ram);
  dog_DrawStrP(50, 1, font_5x8, s_rom);
}

void system_init(void)
{
#if defined(__AVR__)
  /* select minimal prescaler (max system speed) */
  CLKPR = 0x80;
  CLKPR = 0x00;
#endif
}


int main(void)
{
  uint8_t page = 0;
  system_init();
  dog_Delay(2000);  
  dog_Init(0);

  for(;;)
  {
    
    dog_StartPage();
    do
    {
      switch(page) {
	case 0: page0(); break;
	case 1: page1(); break;
	case 2: page2(); break;
	case 3: page3(); break;
	case 4: page4(); break;
	case 5: page5(); break;
      }
    } while( dog_NextPage() );
    dog_Delay(1500);
    page++;
    if ( page == 6 )
      page = 0;
  }  
}

