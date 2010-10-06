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
  
  User Interface:
    Analog Potentiometer: 
      Assign pin number to variable "sensorPin". Default is analog pin 0.
    Digital Input:
      Assign left button pin to variable uiKeyLeftPin. Default is DOGS102 Shield
      Assign right button pin to variable uiKeyRightPin. Default is DOGS102 Shield
  
*/

#include <Dogm.h>

#ifdef DOGM132_HW
#error Breakorino can not be used on a DOGM132 display
#endif


int a0Pin = 9;      // address line a0 for the dogm module
int sensorPin = 0;  // analog input

// configuration values
uint8_t uiKeyLeftPin = 4;
uint8_t uiKeyRightPin = 2;
uint8_t uiMinIncrement = 2*8;
uint8_t uiMaxIncrement = 4*8;

// output values
uint8_t playerLocation = 0;

// internal values
uint8_t isKeyPad = 0;
uint8_t uiActualIncrement = 2*8;

// initialize the dogm library
Dogm dogm(a0Pin);

// setup the user interface
void uiSetup(void) {
  // configure input keys 
  pinMode(uiKeyLeftPin, INPUT);           // set pin to input
  digitalWrite(uiKeyLeftPin, HIGH);       // turn on pullup resistors
  pinMode(uiKeyRightPin, INPUT);           // set pin to input
  digitalWrite(uiKeyRightPin, HIGH);       // turn on pullup resistors
  // configure internal variables
  isKeyPad = 0;
  // assign some (more or less) useful values to the output variables
  playerLocation = 127;
}

// calculate new output values
void uiStep(void) {
  uint8_t is_key_pressed = 0;
  uint8_t real_inc;
  if ( isKeyPad == 0 )
  {
    int sensorValue;
    // assume, that no keypad is used and derive the location from analog input
    sensorValue = analogRead(sensorPin);
    sensorValue >>= 3;
    playerLocation = sensorValue;
    // now check if some keys are pressed
    if ( digitalRead(uiKeyLeftPin) == LOW )
      isKeyPad = 1;
    if ( digitalRead(uiKeyRightPin) == LOW )
      isKeyPad = 1;
  }
  if ( isKeyPad != 0 )
  {
    real_inc = uiActualIncrement;
    real_inc >>= 3;
    if ( digitalRead(uiKeyLeftPin) == LOW )
    {
      is_key_pressed = 1;
      if ( playerLocation <= 255 -  real_inc )
	playerLocation += real_inc;
    }
    if ( digitalRead(uiKeyRightPin) == LOW )
    {
      is_key_pressed = 1;
      if ( playerLocation >= 0+  real_inc )
	playerLocation -= real_inc;
    }
    if ( is_key_pressed == 0 )
    {
      uiActualIncrement = uiMinIncrement;
    }
    else
    {
      if ( uiActualIncrement < uiMaxIncrement )
	uiActualIncrement++;
    }
  }
}


void setup() {
  uiSetup();
  bo_Setup(0);
}

void loop() {
  dogm.start();
  do{
    bo_Draw();
  } while( dogm.next() );
  dog_Delay(1);
  uiStep();
  bo_SetX(playerLocation);
  bo_Step();  
}


