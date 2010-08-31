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

  Note: 
    1) MISO (Pin 12) is not used, but can not be reused as generic I/O
    2) See sourcecode for this program in utility/walk.c
*/

#include <Dogm.h>

int a0Pin = 9;      // address line a0 for the dogm module
Dogm dogm(a0Pin);

void setup() {
}

void loop() {  
  dogm.start();
  do {
    walk_Draw();
  } while( dogm.next() );
  
  // do some delay so that the LCD can display the character
  dog_Delay(120);
  
  // character movement
  walk_Step();
}
