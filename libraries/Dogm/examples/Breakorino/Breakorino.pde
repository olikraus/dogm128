/*

  Breakorino
  
  A breakout clone for dogm128 graphics module
  
  SCL (SPI Clock)   Pin 13
  SI (MOSI)         Pin 11
  CS (Chip Select)  Pin 10

  Note: MISO (Pin 12) is not used, but can not be reused as generic I/O
  
*/

#include <Dogm.h>

int a0Pin = 9;      // address line a0 for the dogm module
int sensorPin = 0;  // analog input

// initialize the dogm library
Dogm dogm(a0Pin);


void setup() {
  bo_Setup(0);
}

void loop() {
  int sensorValue;
  dogm.Start();
  do{
    bo_Draw();
  } while( dogm.Next() );
  delay(1);
  sensorValue = analogRead(sensorPin);
  bo_SetX(sensorValue>>2);
  bo_Step();  
}


