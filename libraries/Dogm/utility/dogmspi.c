/*

  dogmspi.c
  
  (c) 2010 Oliver Kraus (olikraus@gmail.com)
  
  spi abstraction layer
  
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

#include "dogm128.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

uint8_t dog_spi_pin_a0 = PIN_A0_DEFAULT;

#ifdef DOG_SPI_USI

/*=======================================================================*/
/* USI Interface */
/*=======================================================================*/

/* NOT TESTED, DO NOT USE THIS CODE */

/* dog_spi_pin_a0 is ignored, instead, port and pins are hard coded */
#define DOG_SPI_SCL_DDR DDRA
#define DOG_SPI_SCL_PORT PORTA
#define DOG_SPI_SCL_PIN 4

#define DOG_SPI_MOSI_DDR DDRA
#define DOG_SPI_MOSI_PORT PORTA
#define DOG_SPI_MOSI_PIN 5

#define DOG_SPI_CS_DDR DDRB
#define DOG_SPI_CS_PORT PORTB
#define DOG_SPI_CS_PIN 0

#define DOG_SPI_A0_DDR DDRB
#define DOG_SPI_A0_PORT PORTB
#define DOG_SPI_A0_PIN 1

void dog_spi_init(void)
{
  /* setup port directions */
  DOG_SPI_CS_DDR |= _BV(DOG_SPI_CS_PIN);
  DOG_SPI_MOSI_DDR |= _BV(DOG_SPI_MOSI_PIN);
  DOG_SPI_A0_DDR |= _BV(DOG_SPI_A0_PIN);
  DOG_SPI_SCL_DDR |= _BV(DOG_SPI_SCL_PIN);
}

unsigned char dog_spi_out(unsigned char data)
{
  uint8_t i = 8;
  /* the SCL level is set during dog_spi_enable_client() */
  /* output the data */
  USIDR = data;
  do
  {
    /* generate edge at the clock pin, edge types depends of the current pin state */
    /* slave should now sample the data */
    USICR = _BV(USIWM0) | _BV(USITC);
    /* output next bit and toggle the clock pin again*/
    USICR = _BV(USIWM0) | _BV(USICLK) | _BV(USITC);
    i--;
  } while( i != 0 );  
  return USIDR;
}

void dog_spi_enable_client(void)
{
  /* before the slave is enabled, esure that the clk pin has a logical zero */
  DOG_SPI_SCL_PORT &= ~_BV(DOG_SPI_SCL_PIN);
  //DOG_SPI_SCL_PORT |= _BV(DOG_SPI_SCL_PIN);
  
  /* now enable the SPI slave */
  DOG_SPI_CS_PORT &= ~_BV(DOG_SPI_CS_PIN);
}

void dog_spi_disable_client(void)
{
  /* disable the client (write a logical zero on the CS line) */
  DOG_SPI_CS_PORT |= _BV(DOG_SPI_CS_PIN);
}

void dog_cmd_mode(void)
{
  DOG_SPI_A0_PORT &= ~_BV(DOG_SPI_A0_PIN);
}

void dog_data_mode(void)
{
  DOG_SPI_A0_PORT |= _BV(DOG_SPI_A0_PIN);
}


#else

/*=======================================================================*/
/* Arduino SPI */
/*=======================================================================*/

#include <wiring.h>	/* arduino pinMode */

void dog_spi_init(void)
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

#endif