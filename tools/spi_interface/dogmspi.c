/*

  dogmspi.c
  
  spi abstraction layer
  
*/

#include "dogm128.h"

/* setup hardware pins, prepare SPI subsystem */
void dog_spi_init(void)
{
}

/* output a 8 bit value on the SPI bus */
unsigned char dog_spi_out(unsigned char data)
{
}

/* assign low to chip select line */
void dog_spi_enable_client(void)
{
}

/* assign high to chip select line */
void dog_spi_disable_client(void)
{
}

/* assign low to A0 line */
void dog_cmd_mode(void)
{
}

/* assign high to A0 line */
void dog_data_mode(void)
{
}

