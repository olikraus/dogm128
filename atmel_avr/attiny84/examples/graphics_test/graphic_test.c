

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
    dog_DrawStr(20, 10, font_5x8, "Set Test");
    dog_SetBox(5,20,DOG_WIDTH-5,43);
    dog_SetVLine(80,0, 63);
    dog_SetHLine(0,DOG_WIDTH-1, 2);
    dog_SetPixel(10, 17);
    dog_SetPixel(12, 17);
    dog_SetPixel(14, 17);
}

void page1(void)
{
    all_page();
    dog_DrawStr(20, 10, font_5x8, "Clr Test");
    dog_SetBox(5,20,DOG_WIDTH-5,43);
    dog_ClrBox(20,30,30,40);
    dog_ClrVLine(80,0, 63);
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
    dog_DrawStr(20, 16, font_5x8, "Xor Test");
    dog_XorBox(5,20,90,43);
    dog_XorVLine(80,0, 63);
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
    dog_DrawStr(2, 24, font_5x8, "Draw Test");
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

void system_init(void)
{
  /* select minimal prescaler (max system speed) */
  CLKPR = 0x80;
  CLKPR = 0x00;
}


int main(void)
{
  uint8_t page = 0;
  system_init();
  dog_delay(2000);  
  dog_init(0);

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
      }
    } while( dog_NextPage() );
    dog_delay(1000);
    page++;
    if ( page == 4 )
      page = 0;
  }  
}