/*

  dogm128.c
  
  (c) 2010 Oliver Kraus (olikraus@gmail.com)
  
  graphic functions for the dogm128 graphics module 
  with ST7565R controller from electronic assembly
  
  optimized c-code for avr-gcc


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


  ST7565R SPI interface
  
    ST7565R reads data with rising edge of the clock signal (SCL)
      --> CPHA = 0 and CPOL = 0
    ST7565R reads  MSB first 
      --> DORD = 0
      
  /usr/lib/avr/include/avr/pgmspace.h

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dogm128.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <wiring.h>	/* arduino pinMode */


unsigned char dog_page_buffer[DOG_PAGE_WIDTH];
uint8_t dog_curr_page = 0;	/* 0...DOG_PAGE_CNT-1 */
uint8_t dog_min_y = 0;
uint8_t dog_max_y = DOG_PAGE_HEIGHT-1;




/*==============================================================*/
/* 
  SPI subsystem  for Arduino
  Pins 15 to 19 (ATMEGA8) or 10 to 13 (Arduino) are used.
*/
/*==============================================================*/



unsigned char dog_spi_pin_a0 = PIN_A0_DEFAULT;

void dog_init_spi(void)
{
  pinMode(PIN_SCK, OUTPUT);
  pinMode(PIN_MOSI, OUTPUT);
  //pinMode(PIN_MISO, INPUT); 
  pinMode(PIN_SS, OUTPUT);
  pinMode(dog_spi_pin_a0, OUTPUT);
  
  /*
    SPR1 SPR0
	0	0		fclk/4
	0	1		fclk/16
	1	0		fclk/64
	1	1		fclk/128
  */
  SPCR = 0;
  SPCR =  (1<<SPE) | (1<<MSTR)|(0<<SPR1)|(0<<SPR0)|(0<<CPOL)|(0<<CPHA);
  /*
  {
  unsigned char x;
    x = SPSR;
    x= SPDR;
  }
  */
}

unsigned char dog_spi_out(unsigned char data)
{
  
  /* unsigned char x = 100; */
  /* send data */
  SPDR = data;
  /* wait for transmission */
  while (!(SPSR & (1<<SPIF))) 
    ;
  /* clear the SPIF flag by reading SPDR */
  return  SPDR;
}

void dog_spi_enable_client(void)
{
        digitalWrite(PIN_SS, LOW);  
}

void dog_spi_disable_client(void)
{
        digitalWrite(PIN_SS, HIGH);
}

void dog_cmd_mode(void)
{
  digitalWrite(dog_spi_pin_a0, LOW);
}

void dog_data_mode(void)
{
  digitalWrite(dog_spi_pin_a0, HIGH);
}

void dog_delay(unsigned short val)
{
  delay(val);
}

void dog_init_display(void)
{
  dog_delay(10);
  dog_spi_disable_client();	/* force reset of the spi subsystem of the ST7565R */
  dog_delay(10);
  dog_spi_enable_client();
  dog_delay(10);
  dog_cmd_mode();
  /* mostly taken from the EA dogm description */
#ifdef DOGM128_HW
  dog_spi_out(0x040);		/* set display start line to 0 */
  dog_spi_out(0x0a1);		/* ADC set to reverse */
  dog_spi_out(0x0c0);		/* common output mode */
  dog_spi_out(0x0a6);		/* display normal, bit val 0: LCD pixel off. */
  dog_spi_out(0x0a2);		/* LCD bias 1/9 */
  dog_spi_out(0x02f);		/* all power  control circuits on */
  dog_spi_out(0x0f8);		/* set booster ratio to */
  dog_spi_out(0x000);		/* 4x */
  dog_spi_out(0x027);		/* set V0 voltage resistor ratio to large */
  dog_spi_out(0x081);		/* set contrast */
  dog_spi_out(0x018);		/* contrast value, EA default: 0x016 */
  dog_spi_out(0x0ac);		/* indicator */
  dog_spi_out(0x000);		/* disable */
  dog_spi_out(0x0af);		/* display on */
#endif

#ifdef DOGM132_HW
  dog_spi_out(0x040);		/* set display start line to 0 */
  dog_spi_out(0x0a1);		/* ADC set to reverse */
  dog_spi_out(0x0c0);		/* common output mode */
  dog_spi_out(0x0a6);		/* display normal, bit val 0: LCD pixel off. */
  dog_spi_out(0x0a2);		/* LCD bias 1/9 */
  dog_spi_out(0x02f);		/* all power  control circuits on */
  dog_spi_out(0x0f8);		/* set booster ratio to */
  dog_spi_out(0x000);		/* 4x */
  dog_spi_out(0x023);		/* set V0 voltage resistor ratio to large */
  dog_spi_out(0x081);		/* set contrast */
  dog_spi_out(0x01f);		/* contrast value, EA default: 0x01f */
  dog_spi_out(0x0ac);		/* indicator */
  dog_spi_out(0x000);		/* disable */
  dog_spi_out(0x0af);		/* display on */
#endif

#ifdef DOGS102_HW
  dog_spi_out(0x040);		/* set display start line to 0 */
  dog_spi_out(0x0a1);		/* ADC set to reverse */
  dog_spi_out(0x0c0);		/* common output mode */
  dog_spi_out(0x0a6);		/* display normal, bit val 0: LCD pixel off. */
  dog_spi_out(0x0a2);		/* LCD bias 1/9 */
  dog_spi_out(0x02f);		/* all power  control circuits on */
  dog_spi_out(0x027);		/* regulator, booster and follower */
  dog_spi_out(0x081);		/* set contrast */
  dog_spi_out(0x00e);		/* contrast value, EA default: 0x010 */
  dog_spi_out(0x0fa);		/* Set Temp compensation */ 
  dog_spi_out(0x090);		/* 0.11 deg/c WP Off WC Off*/
  dog_spi_out(0x0af);		/* display on */
#endif

  dog_spi_out(0x0a5);		/* display all points */
  dog_delay(300);
  dog_spi_out(0x0a4);		/* normal display  */
  dog_delay(300);
  dog_spi_disable_client();
}

void dog_init(unsigned short pin_a0)
{
  dog_delay(50);			/* initial delay */
  dog_spi_pin_a0 = pin_a0;
  dog_delay(10);
  dog_init_spi();
  dog_init_display();
}


void dog_transfer_page(void)
{
  uint8_t idx;

  /* enable and reset com interface of the ST7565R */
  dog_spi_enable_client();
  
  /* set write position */
  dog_cmd_mode();
  
  dog_spi_out(0x0b0 | dog_curr_page );		/* select current page  */
  dog_spi_out(0x010 );		/* set upper 4 bit of the col adr to 0 */
  dog_spi_out(0x000 );		/* set lower 4 bit of the col adr to 0 */
  
  /* send a complete page */
  
  dog_data_mode();
  
  idx = DOG_PAGE_WIDTH;
  while( idx != 0 )
  {
    idx--;
    dog_spi_out(dog_page_buffer[idx]); 
  }

  /* disable com interface of the ST7565R */

  dog_spi_disable_client();
}


/*==============================================================*/
/* page buffer functions */
/*==============================================================*/

void dog_ClearPage(void)
{
  uint8_t i;
  for( i = 0; i < DOG_PAGE_WIDTH; i++ )
    dog_page_buffer[i] = 0;
}

void dog_StartPage(void)
{
  dog_curr_page = 0;
  dog_min_y = 0;
  dog_max_y = DOG_PAGE_HEIGHT-1;
  dog_ClearPage();
}

uint8_t dog_NextPage(void)
{
  dog_transfer_page();
  dog_ClearPage();
  dog_curr_page++;
  if ( dog_curr_page >= DOG_PAGE_CNT )
      return 0;
  
  dog_min_y = DOG_PAGE_HEIGHT;
  dog_min_y *= dog_curr_page;
  dog_max_y = dog_min_y;
  dog_max_y += DOG_PAGE_HEIGHT-1;
  return 1;
}

