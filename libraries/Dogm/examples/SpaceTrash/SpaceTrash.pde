/*

  SpaceTrash
  
  A little game for dogm128 library graphics module.
  Optimized for the DOGM132 module.
  Will also run on an ATMEL ATTINY

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


  SCL (SPI Clock)   Pin 13
  SI (MOSI)         Pin 11
  CS (Chip Select)  Pin 10

  Note: MISO (Pin 12) is not used, but can not be reused as generic I/O

  User Interface:
    Analog Potentiometer: 
      Assign pin number to variable "sensorPin". Default is analog pin 0.
    Digital Input:
      Assign up button pin to variable uiKeyUpPin. Default is DOGS102 Shield
      Assign down button pin to variable uiKeyDownPin. Default is DOGS102 Shield
      Assign fire button pin to variable uiKeyFirePin. Default is DOGS102 Shield

*/

#include <Dogm.h>


int a0Pin = 9;      // address line a0 for the dogm module
uint16_t sensorPin = 0;  // analog input

// configuration values
uint8_t uiKeyUpPin = 5;
uint8_t uiKeyDownPin = 3;
uint8_t uiKeyFirePin = 4;
uint8_t uiIncrement = 2;

// output values
uint8_t shipLocation = 0;
uint8_t isAutoFire = 1;
uint8_t isFire = 0;

// internal values
uint8_t isKeyPad = 0;



// initialize the dogm library
Dogm dogm(a0Pin);

// setup the user interface
void uiSetup(void) {
  // configure input keys 
  pinMode(uiKeyUpPin, INPUT);           // set pin to input
  digitalWrite(uiKeyUpPin, HIGH);       // turn on pullup resistors
  pinMode(uiKeyDownPin, INPUT);           // set pin to input
  digitalWrite(uiKeyDownPin, HIGH);       // turn on pullup resistors
  pinMode(uiKeyFirePin, INPUT);           // set pin to input
  digitalWrite(uiKeyFirePin, HIGH);       // turn on pullup resistors
  // configure internal variables
  isKeyPad = 0;
  // assign some (more or less) useful values to the output variables
  shipLocation = 0;
}

// calculate new output values
void uiStep(void) {
  if ( isKeyPad == 0 )
  {
    int sensorValue;
    // assume, that no keypad is used and derive the location from analog input
    sensorValue = analogRead(sensorPin);
    sensorValue >>= 2;
    shipLocation = sensorValue;
    // now check if some keys are pressed
    if ( digitalRead(uiKeyUpPin) == LOW )
      isKeyPad = 1;
    if ( digitalRead(uiKeyDownPin) == LOW )
      isKeyPad = 1;
    if ( digitalRead(uiKeyFirePin) == LOW )
      isKeyPad = 1;
  }
  if ( isKeyPad != 0 )
  {
    isAutoFire = 0;
    if ( digitalRead(uiKeyUpPin) == LOW )
      if ( shipLocation <= 255 -  uiIncrement )
	shipLocation += uiIncrement;
    if ( digitalRead(uiKeyDownPin) == LOW )
      if ( shipLocation >= 0+  uiIncrement )
	shipLocation -= uiIncrement;
    isFire = 0;
    if ( digitalRead(uiKeyFirePin) == LOW )
      isFire = 1;
  }
}

void setup() {
  uiSetup();
}

void loop() {
  dogm.start();
  do{
    st_Draw();
  } while( dogm.next() );
  dog_Delay(10);
  uiStep();
  st_Step(shipLocation, isAutoFire, isFire);
}


