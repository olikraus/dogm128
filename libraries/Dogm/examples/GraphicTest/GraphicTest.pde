/*

  GraphicTest
  
  Test for the dogm128 library

  Written 2010 by Oliver Kraus.
  
  SCL (SPI Clock)   Pin 13
  SI (MOSI)         Pin 11
  CS (Chip Select)  Pin 10

  Note: MISO (Pin 12) is not used, but can not be reused as generic I/O
  
*/

#include <Dogm.h>

int a0Pin = 9;      // address line a0 for the dogm module

Dogm dogm(a0Pin);

void setup() {
}


void all_page()
{
    dogm.setFont(font_5x8);
  
    // draw a cross at the DOGS x-boundary
    dogm.setVLine(101,0, 3);
    dogm.setVLine(102,1, 4);
    dogm.setHLine(100,103, 2);

    // draw a cross at the DOGM132 y-boundary  
    dogm.setHLine(98,103, 31);
    dogm.setHLine(99,104, 32);
    dogm.setVLine(101,30, 34);
}


void page0()
{
    all_page();
    dogm.setFont(font_5x8);
    dogm.setXY(20,10);
    dogm.drawStr("Set Test");
    dogm.setBox(5,20,DOG_WIDTH-5,43);
    dogm.setVLine(80,0, 63);
    dogm.setHLine(0,DOG_WIDTH-1, 2);
    dogm.setPixel(10, 17);
    dogm.setPixel(12, 17);
    dogm.setPixel(14, 17);
}

void page1()
{
    all_page();
    dogm.setFont(font_5x8);
    dogm.setXY(20,10);
    dogm.drawStr("Clr Test");
    dogm.setBox(5,20,DOG_WIDTH-5,43);
    dogm.clrBox(20,30,30,40);
    dogm.clrVLine(80,0, 63);
    dogm.clrPixel(10, 27);
    dogm.clrPixel(12, 27);
    dogm.clrPixel(14, 27);
    dogm.clrPixel(10, 17);
    dogm.clrPixel(12, 17);
    dogm.clrPixel(14, 17);
    dogm.clrHLine(16, 23, 27);
}

void page2()
{
    all_page();
    dogm.setFont(font_5x8);
    dogm.setXY(20,16);
    dogm.drawStr("Xor Test");
    dogm.xorBox(5,20,90,43);
    dogm.xorVLine(80,0, 63);
    dogm.xorPixel(10, 27);
    dogm.xorPixel(12, 27);
    dogm.xorPixel(14, 27);
    dogm.xorPixel(10, 17);
    dogm.xorPixel(12, 17);
    dogm.xorPixel(14, 17);
    dogm.xorHLine(16, 127, 27);
}

void page3()
{
    all_page();
    dogm.setFont(font_5x8);
    dogm.setXY(2,24);
    dogm.drawStr("Draw Test");
    dogm.setSize(1);
    dogm.drawLine(0,0,127,20);
    dogm.drawLine(0,1,127,30);
    dogm.setSize(0);
    dogm.drawLine(0,2,127,40);
    dogm.drawLine(0,3,127,50);
    dogm.setSize(0);
    dogm.drawPoint(10,20);
    dogm.drawPoint(16,20);
    dogm.drawArc(53, 40, 10, 192, 64); 
    dogm.setSize(1);
    dogm.drawPoint(13,20);
    dogm.drawArc(53, 40, 14, 192, 64); 
}

uint8_t page = 0;
void loop() {
  dogm.start();
  do {
    switch(page) {
      case 0: page0(); break;
      case 1: page1(); break;
      case 2: page2(); break;
      case 3: page3(); break;
    }
  } while( dogm.next() );
  dog_delay(1000);
  page++;
  if ( page == 4 )
    page = 0;
}
