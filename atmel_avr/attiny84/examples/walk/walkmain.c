
/*
  walkmain.c
  
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

  Note: See actual code for this program is located in the file walk.c of the library directory.
  
*/
#include "dogm128.h"


uint16_t sys_adc(void)
{
  unsigned short l, h;
  
  /* use PA6 as input pin for the ADC */
  DDRA &= ~_BV(7);
  /* ADC 7*/
  ADMUX = 7;	
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
  /* select minimal prescaler (max system speed) */
  CLKPR = 0x80;
  CLKPR = 0x00;
}


int main(void)
{
  uint16_t adc;
  sys_init();
  dog_init(0);
  

  for(;;)
  {    
    dog_StartPage();
    do
    {
      walk_Draw();
    } while( dog_NextPage() );
    adc = sys_adc();
    adc >>= 4;
    if ( adc >= 0x0f && adc < 0x30 )
      dog_set_contrast(adc);
    dog_Delay(120);
    walk_Step();
  }  
}

