/*

  Walk.pde
  
  Walk Cycle Study for the dogm128 library.
  
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

  
  SCL (SPI Clock)   Pin 13
  SI (MOSI)         Pin 11
  CS (Chip Select)  Pin 10

  Note: MISO (Pin 12) is not used, but can not be reused as generic I/O
  
*/

#include <Dogm.h>

// the bitmap contains 7 pictures each with a size of 10x12 pixel
unsigned char bitmaps[] = {

/*
0000000000000000  
0000111000000000
0001000100000000
0001000100000000
0000111000000000
0000100000000000
0001110000000000
0010101110000000
0100100000000000
0011010000000000
0100001000000000
1110000111000000
*/

0x0, 0x0,
0x0E, 0x00,
0x11, 0x00,
0x11, 0x00,
0xE, 0x00,
0x8, 0x00,
0x1C, 0x00,
0x2B, 0x80,
0x48, 0x00,
0x34, 0x00,
0x42, 0x00,
0xE1, 0xC0,

/*
0000000000000000
0000111000000000
0001000100000000
0001000100000000
0000111000000000
0000100000000000
0001110000000000
0010111100000000
0010100000000000
0001010000000000
1110001000000000
1100001110000000
*/

0x0, 0x0,
0xE, 0x00,
0x11, 0x00,
0x11, 0x00,
0xE, 0x00,
0x8, 0x00,
0x1C, 0x00,
0x2F, 0x00,
0x28, 0x00,
0x14, 0x00,
0xE2, 0x00,
0xC3, 0x80,

/*
0001110000000000
0010001000000000
0010001000000000
0001110000000000
0000100000000000
0001100000000000
0010111000000000
0001100000000000
0000100000000000
0111010000000000
0100010000000000
0000011100000000
*/

0x1C, 0x00,
0x22, 0x00,
0x22, 0x00,
0x1C, 0x00,
0x8, 0x00,
0x18, 0x00,
0x2E, 0x00,
0x18, 0x00,
0x8, 0x00,
0x74, 0x00,
0x44, 0x00,
0x7, 0x00,

/*
0001110000000000
0010001000000000
0010001000000000
0001110000000000
0000100000000000
0001100000000000
0001110000000000
0001100000000000
0000100000000000
0001100000000000
0010100000000000
0001111000000000
*/

0x1C, 0x00,
0x22, 0x00,
0x22, 0x00,
0x1C, 0x00,
0x8, 0x00,
0x18, 0x00,
0x1C, 0x00,
0x18, 0x00,
0x8, 0x00,
0x18, 0x00,
0x28, 0x00,
0x1E, 0x00,

/*
0001110000000000
0010001000000000
0010001000000000
0001110000000000
0000100000000000
0001100000000000
0001110000000000
0000110000000000
0000100000000000
0000110000000000
0001011100000000
0001110000000000
*/

0x1C, 0x00,
0x22, 0x00,
0x22, 0x00,
0x1C, 0x00,
0x8, 0x00,
0x18, 0x00,
0x1C, 0x00,
0xC, 0x00,
0x8, 0x00,
0xC, 0x00,
0x17, 0x00,
0x1C, 0x00,

/*
0000000000000000
0001110000000000
0010001000000000
0010001000000000
0001110000000000
0000100000000000
0001100000000000
0010111000000000
0010100000000000
0000110000000000
0001001110000000
0011100000000000
*/

0x0, 0x00,
0x1C, 0x00,
0x22, 0x00,
0x22, 0x00,
0x1C, 0x00,
0x8, 0x00,
0x18, 0x00,
0x2E, 0x00,
0x28, 0x00,
0xC, 0x00,
0x13, 0x80,
0x38, 0x00,

/*
0000000000000000
0000111000000000
0001000100000000
0001000100000000
0000111000000000
0000100000000000
0001110000000000
0010111100000000
0010100000000000
0001010000000000
0010001000000000
011100111000000
*/
0x0, 0x00,
0xE, 0x00,
0x11, 0x00,
0x11, 0x00,
0xE, 0x00,
0x8, 0x00,
0x1C, 0x00,
0x2F, 0x00,
0x28, 0x00,
0x14, 0x00,
0x22, 0x00,
0x73, 0x80
};

int a0Pin = 9;      // address line a0 for the dogm module
Dogm dogm(a0Pin);

uint8_t walk_cycle = 0;
uint8_t position = 0;

void setup() {
}

void loop() {  
  dogm.start();
  do {
    // a box behind the character
    dogm.setBox(30,22,36,41);
    dogm.clrBox(31,23,35,40);

    // draw the character
    dogm.setBitmap(position,30,bitmaps+walk_cycle*24, 10, 12);

    // a box before the character
    dogm.setBox(40,12,46,31);
    dogm.clrBox(41,13,45,30);

    // draw the cartoon frame
    dogm.clrBox(0,0,15,63);
    dogm.clrBox(90,0,105,63);
    dogm.setVLine(15,0,50);
    dogm.setVLine(90,0,50);
    dogm.setHLine(0,105, 5);
  } while( dogm.next() );
  
  // do some delay so that the LCD can display the character
  delay(120);
  
  // character movement
  walk_cycle++;
  if ( walk_cycle == 7 )
    walk_cycle = 0;
  
  position++;
  if ( position == 90 )
    position = 0;
}
