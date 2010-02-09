/*

  Fonts
  
  Show some fonts of the dog128 library.
  Written by Oliver Kraus.
  
  
  
  SCL (SPI Clock)   Pin 13
  SI (MOSI)         Pin 11
  CS (Chip Select)  Pin 10

  Note: MISO (Pin 12) is not used, but can not be reused as generic I/O
  
*/

#include <Dogm.h>

int a0Pin = 9;      // address line a0 for the dogm module
int sensorPin = 0;  // analog input

Dogm dogm(a0Pin);

void setup() {
}

int font_select = 0;


void loop() {
  char *f;
  PGM_P p;
  unsigned char h;
  
  switch(font_select) {
      default: font_select = 0; /* no break */
      case 0: h = 10; p = font_4x6; f = "4x6"; break;
      case 1: h = 10; p = font_5x7; f = "5x7"; break;
      case 2: h = 10; p = font_5x8; f = "5x8"; break;
      case 3: h = 10; p = font_6x9; f = "6x9"; break;
      case 4: h = 10; p = font_6x10; f = "6x10"; break;
      case 5: h = 10; p = font_6x12; f = "6x12"; break;
      case 6: h = 11; p = font_6x13; f = "6x13"; break;
      case 7: h = 11; p = font_7x13; f = "7x13"; break;
      case 8: h = 12; p = font_7x14; f = "7x14"; break;
      case 9: h = 12; p = font_8x13; f = "8x13"; break;
      case 10: h = 15; p = font_8x16; f = "8x16"; break;
  }
  font_select++;
  
  dogm.start();
  do {
    dogm.setFont(p);
    dogm.setXY(0,h*3+3);
    dogm.print(f);
    dogm.print(": 0123456789.,;");
    dogm.setXY(0,h*2+3);
    dogm.print("abcdefgjijklmnopqrstuvwxyz");
    dogm.setXY(0,h*1+3);
    dogm.print("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    dogm.setXY(0,h*0+3);
    dogm.print("+-*/#-!$%&(){}[]");
  } while( dogm.next() );
  delay(1000);
}
