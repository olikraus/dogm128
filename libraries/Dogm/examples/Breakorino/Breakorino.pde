/*

  Breakorino
  
  A little game for dogm128 graphics module.

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

#ifdef DOGM132_HW
#error Breakorino can not be used on a DOGM132 display
#endif


int a0Pin = 9;      // address line a0 for the dogm module
int sensorPin = 0;  // analog input

// initialize the dogm library
Dogm dogm(a0Pin);


void setup() {
  bo_Setup(0);
}

void loop() {
  int sensorValue;
  dogm.start();
  do{
    bo_Draw();
  } while( dogm.next() );
  dog_Delay(1);
  sensorValue = analogRead(sensorPin);
  bo_SetX(sensorValue>>2);
  bo_Step();  
}


