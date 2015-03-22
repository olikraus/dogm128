

# Install Instructions for Arduino Hardware

**NOTE: It is now required to edit dogm128.h for ALL display types**

This library has been tested with the Arduino-0022 IDE.

  1. Download latest version of dogm128\_arduino\_vX.XX.zip
  1. cd <arduino-0021 path>/libraries
  1. unzip dogm128\_vX.XX.zip
  1. cd <arduino-0021 path>/libraries/Dogm/utility/
  1. edit file dogm128.h and uncomment your display hardware (DOGM128, DOGS102, DOGM132 or DOGXL160)
  1. Optional: Edit file dogm128.h and uncomment DOG\_REVERSE (change top/bottom view) and/or DOG\_DOUBLE\_MEMORY (see [Configuration](install#Configuration.md))
  1. (re)start the Arduino IDE
  1. Dogm128 examples are available under `File > Examples > Dogm`

Note: Breakorino has not been ported to DOGM132

# Install Instructions for Atmel AVR ATTINY and ATMEGA Microcontroller

> The ATTINY port of the library is available with Release v1.04.

> The ATMEGA port of the library is available with Release v1.07.

  1. Setup Environment
    * GNU C Compiler for Atmel AVR (`sudo apt-get install gcc-avr`)
    * Atmel AVR Programmer avrdude (`sudo apt-get install avrdude`)
    * gnu make
    * Programmer Hardware for Atmel ISP interface (See http://www.atmel.com/products/avr/ or http://en.wikipedia.org/wiki/Atmel_AVR)
  1. Download latest version of dogm128\_avr\_vX.XX.zip
  1. Extract zip file and change directory to graphics test example
  1. Change the `Makefile` according to your hardware (See also the [ATTINY schematic](dogm132_attiny84_hardware.md) or [ATMEGA schematic](dogm132_atmega88_hardware.md) and the information according configuration below)
    * `MCU:=attiny84`
    * `MCU:=atmega88`
> > > Add the name of the target device. Valid names are reported with the avr-gcc -dumpspec  command. MCU names (devices) are also listed here: http://www.nongnu.org/avr-libc/user-manual/index.html
    * `F_CPU:=8000000`
> > > Set the frequency of the target environment. The examples of this library will try to assign maximum speed.
    * `DOGDEFS = -DDOGM132_HW`
> > > Select display (DOGM128\_HW, DOGM132\_HW or DOGS102\_HW)
    * `DOGDEFS += -DDOG_SPI_USI`
> > > Select controller interface. Possible values are: DOG\_SPI\_ATMEGA, DOG\_SPI\_USI or DOG\_SPI\_ARDUINO (which is also used, if nothing is defined).
    * `DOGDEFS += -DDOG_SPI_CS_PORT=B -DDOG_SPI_CS_PIN=0`
> > > I/O port and bit number which is connected to the chip select line of the display.
    * `DOGDEFS += -DDOG_SPI_A0_PORT=B -DDOG_SPI_A0_PIN=1`
> > > I/O port and bit number which is connected to the A0 line of the display.
    * `DOGDEFS += -DDOG_SPI_SCL_PORT=A -DDOG_SPI_SCL_PIN=4`
> > > Port and pin of the SCL output line of the Atmel AVR USI interface
    * `DOGDEFS += -DDOG_SPI_DO_PORT=A -DDOG_SPI_DO_PIN=5`
> > > Port and pin of the DO output line of the Atmel AVR USI interface
    * Optional: Add `DOGDEFS += -DDOG_REVERSE`
> > > Exchange top/bottom view
  1. Create hex-file and upload the hexfile to the controller: `make upload`
  1. Setup a new project:
    * Create a new directory, copy the modified Makefile into the new directory.
    * `TARGETNAME = project_name`
> > > Assign the project name.
    * `DOGDIR = ../../lib/`
> > > Set the location of the dogm128 library if required.
    * `SRC = file1.c file2.c file3.c`
> > > Add list of the c-files of the project.

# Configuration

The following tables provide the name of C-Macros. These C-Macros can be used to configure the target environment. These C-Macros can be defined on the command line or in the file `dogm128.h`.


## Display Type
Use one of the following Macros to select the display type.
The display can be selected in `dogm128.h`.
|    **C-Macro** | **Description** |
|:---------------|:----------------|
|    DOGM128\_HW	|	DOGM128 Display |
|    DOGS102\_HW	|	DOGS102 Display |
|    DOGM132\_HW	|	DOGM132 Display |
|    DOGXL160\_HW\_BW	|	DOGXL160 Display, black and white mode (v1.08) |
|    DOGXL160\_HW\_GR	|	DOGXL160 Display, graylevel support (v1.08) |
|    nothing defined	|	Compile Error   |

For the DOGXL160 this library supports two different modes. BW mode is faster, but only allows two values per pixel (on and off). GR mode supports four values per pixel.

## Display Orientation (DOG\_REVERSE)
Use the following Macro to rotate the display by 180 degree.
This macro can be enabled in `dogm128.h`.
|    **C-Macro** | **Description** |
|:---------------|:----------------|
|    DOG\_REVERSE	|	180 degree rotation |
|    nothing defined	|	no rotation     |

## Page Memory Size (DOG\_DOUBLE\_MEMORY)
Use the following Macro to tune speed vs. memory usage (available: v1.08 and above).
This macro can be enabled in `dogm128.h`.
|    **C-Macro** | **Description** |
|:---------------|:----------------|
|    DOG\_DOUBLE\_MEMORY	| Faster drawing function, but will require more RAM |
|    nothing defined	| Slower drawing function, but minimal RAM usage |

The following shows the page buffer size for the selected display.
|   **Display** | **without DOG\_DOUBLE\_MEMORY** | **enabled DOG\_DOUBLE\_MEMORY** |
|:--------------|:--------------------------------|:--------------------------------|
|    DOGM128\_HW	|	128 Bytes                       | 256 Bytes                       |
|    DOGS102\_HW	|	104 Bytes                       | 208 Bytes                       |
|    DOGM132\_HW	|	132 Bytes                       | 264 Bytes                       |
|    DOGXL160\_HW\_BW	|	160 Bytes                       | 320 Bytes                       |
|    DOGXL160\_HW\_GR	|	160 Bytes                       | 320 Bytes                       |

## SPI Interface Selection

All macro definitions in this section can be done either at the commandline or
directly in the C-file `dogmspi.c`.

Use one of the following Macros to select the SPI interface.
|    **C-Macro** | **Description** |
|:---------------|:----------------|
|    DOG\_SPI\_USI	|	Universal Serial Interface of ATTINY controller|
|    DOG\_SPI\_ATMEGA	| ATMEGA SPI Interface (v1.07 and above) |
|    DOG\_SPI\_ARDUINO	| SPI Interface for ATMEGA within Arduino Environment (v1.07 and above)|
|    nothing defined	|	defaults to DOG\_SPI\_ARDUINO |

Some selections require additional definitions for the port pins.
The macro definitions could have the following form:
```
 avr-gcc -DDOG_SPI_SCL_PORT=A -DDOG_SPI_SCL_PIN=0
```

| **Macro Name**     |    **Valid Range** |
|:-------------------|:-------------------|
| DOG\_SPI\_x\_PORT  | Singe Char Port Name |
| DOG\_SPI\_x\_PIN   | Number 0 to 7      |

In many cases the functionality of the pin requires, that the pin is bound to a specific hardware pin.


### Port Definition DOG\_SPI\_USI
| **Macro Names**                       | **Description** | **Restrictions** |
|:--------------------------------------|:----------------|:-----------------|
| DOG\_SPI\_SCL\_PORT, DOG\_SPI\_SCL\_PIN | SPI Clock Line  | The corresponding HW pin must be used |
| DOG\_SPI\_DO\_PORT, DOG\_SPI\_DO\_PIN | SPI Data Out Line  | The corresponding HW pin must be used |
| DOG\_SPI\_CS\_PORT, DOG\_SPI\_CS\_PIN | Chip Select Line | Any i/o pin may be used |
| DOG\_SPI\_A0\_PORT DOG\_SPI\_A0\_PIN  | Adress Select Line | Any i/o pin may be used |


### Port Definition DOG\_SPI\_ATMEGA (v1.07 and above)
| **Macro Names**                       | **Description** | **Restrictions** |
|:--------------------------------------|:----------------|:-----------------|
| DOG\_SPI\_SCL\_PORT, DOG\_SPI\_SCL\_PIN | SPI Clock Line  | The corresponding HW pin must be used |
| DOG\_SPI\_MOSI\_PORT, DOG\_SPI\_MOSI\_PIN | SPI Master Out Line  | The corresponding HW pin must be used |
| DOG\_SPI\_SS\_PORT, DOG\_SPI\_SS\_PIN | Slave (Chip) Select Line | The corresponding HW pin must be used |
| DOG\_SPI\_A0\_PORT DOG\_SPI\_A0\_PIN  | Adress Select Line | Any i/o pin may be used |



# Links

  * [Wiki Hardware Overview](hardware.md)
  * [Dogm128 Wiki](dogm128.md)
  * [Arduino Library Tutorial](http://arduino.cc/en/Hacking/LibraryTutorial)