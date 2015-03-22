

# Introduction

The SPI interface is implemented in the file `dogmspi.c`. This file can
be replaced with any other file as long as the six functions below are implemented.
See the template file [below](spiapi#Template.md) or [here](http://code.google.com/p/dogm128/source/browse/tools/spi_interface/dogmspi.c).

# Functions

## `dog_spi_init`
> Setup the SPI interface: Setup I/O lines and directions, prepare SPI subsystem.
  * Prototype
```
void dog_spi_init(void)
```

## `dog_spi_out`
> Output value via SPI.
  * Prototype
```
unsigned char dog_spi_out(unsigned char data)
```
  * Arguments
    * `data`: Value, which should be sent via SPI bus.


## `dog_spi_enable_client`
> Enable the display: Send low on the chip select line.
  * Prototype
```
void dog_spi_enable_client(void)
```
## `dog_spi_disable_client`
> Disable the display: Send high on the chip select line.
  * Prototype
```
void dog_spi_disable_client(void)
```
## `dog_cmd_mode`
> Enter command mode: Send low on A0 line.
  * Prototype
```
void dog_cmd_mode(void)
```
## `dog_data_mode`
> Enter data mode: Send high on A0 line.
  * Prototype
```
void dog_data_mode(void)
```


# Variables

## `dog_spi_pin_a0`
> The value for this variable can be set through the user-API.
> In the Arduino Environment this variable contains the pin
> number which is connected to the A0 line of the display hardware.

  * Declaration
```
extern uint8_t dog_spi_pin_a0;
```

## `dog_spi_pin_cs`
> The value for this variable can be set through the user-API.
> In the Arduino Environment this variable contains the pin
> number which is connected to the chip select line of the display hardware.

  * Declaration
```
extern uint8_t dog_spi_pin_cs;
```

# Template

```
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

```