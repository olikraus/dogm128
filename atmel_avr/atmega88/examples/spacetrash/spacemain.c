
/*
  spacemain.c
  
  Space Trash for the dogm128 library.
  
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

  Note: See actual code for this program is located in the file spacetrash.c of the library directory.
  
*/
#include "dogm128.h"


// generic configuration values
uint8_t uiIncrement = 2;

// output values
uint8_t shipLocation = 0;
uint8_t isAutoFire = 1;
uint8_t isFire = 0;

// internal values
uint8_t isKeyPad = 0;
uint8_t keyStatus = 0;

void sys_setup_keys(void)
{
  /* configure buttons (inputs with pullup) */
  DDRD &= ~(1<<5);
  PORTD |= (1<<5);
  DDRD &= ~(1<<6);
  PORTD |= (1<<6);
  DDRD &= ~(1<<7);
  PORTD |= (1<<7);
  DDRB &= ~(1<<7);
  PORTB |= (1<<7);
}

uint8_t sys_get_key(void)
{
  uint8_t result = 0;
  if ( (PIND & (1<<5)) == 0 ) 
    result |= 1;
  if ( (PIND & (1<<6)) == 0 ) 
    result |= 2;
  if ( (PIND & (1<<7)) == 0 ) 
    result |= 4;
  if ( (PINB & (1<<7)) == 0 ) 
    result |= 8;
  return result;
}

uint16_t sys_adc(void)
{
  unsigned short l, h;
  
  /* use PC0 as input pin for the ADC */
  DDRC &= ~_BV(0);
  /* ADC 0*/
  ADMUX = 0;	
  /* default operation */
  ADCSRB = 0x0;
  /* enable and start conversion, maximum prescalar */
  ADCSRA = 0xc7;
  /* wait for conversion to be finished */
  while ( ADCSRA == 0xc7 )
    ;
  /* return 8 bit result */
  l = ADCL;
  h = ADCH;
  return (h<<8) | l ;
}

void sys_init(void)
{
#if defined(__AVR__)
  /* select minimal prescaler (max system speed) */
  CLKPR = 0x80;
  CLKPR = 0x00;
#endif
}

void ui_setup(void)
{
    // configure internal variables
  isKeyPad = 0;
  // assign some (more or less) useful values to the output variables
  shipLocation = 0;
}


// calculate new output values
void ui_step(void) {
  keyStatus = sys_get_key();
  if ( isKeyPad == 0 )
  {
    uint16_t sensorValue;
    // assume, that no keypad is used and derive the location from analog input
    sensorValue = sys_adc();
    sensorValue >>= 2;
    shipLocation = sensorValue;
    // now check if some keys are pressed
    if ( keyStatus != 0 )
      isKeyPad = 1;
  }
  if ( isKeyPad != 0 )
  {
    isAutoFire = 0;
    if ( keyStatus & 1 )
      if ( shipLocation <= 255 -  uiIncrement )
	shipLocation += uiIncrement;
    if ( keyStatus & 2 )
      if ( shipLocation >= 0+  uiIncrement )
	shipLocation -= uiIncrement;
    isFire = 0;
    if ( keyStatus & 4 )
      isFire = 1;
  }
}


int main(void)
{
  uint16_t adc;
  sys_init();
  sys_setup_keys();
  ui_setup();
  dog_Init(0);
  for(;;)
  {    
    dog_StartPage();
    do
    {
      st_Draw(0);
    } while( dog_NextPage() );
    adc = sys_adc();
    adc >>= 2;
    dog_Delay(10);
    ui_step();
    st_Step(shipLocation, isAutoFire, isFire);
  }  
}

