

# Defines

## `F_CPU`

  * F\_CPU must be defined and must expand to the CPU frequency in hertz (e.g. -DF\_CPU=16000000L)

## Identify Environment

  * `__AVR__`: Atmel AVR
  * `__18CXX`: MPLAB C18 (PIC)


# Filenames

| 1st Char |       | | 2nd Char |             | | 3rd Char | |
|:---------|:------|:|:---------|:------------|:|:---------|:|
| dogmo... | other | | dogm.p.. | point/pixel | |  dogm..p. | PROGMEM version |
| dogmd... | draw  | | dogm.h.. | h line      | |          |  |
| dogmx... | xor   | | dogm.v.. | v line      | |          |  |
| dogms... | set   | | dogm.l.. | line        | |          |  |
| dogmc... | clear | | dogm.a.. | arc         | |          |  |
| dogmm... | math  | | dogm.b.. | box         | |          |  |
|          |       | | dogm.s.. | string      | |          |  |
|          |       | | dogm.m.. | bitmap      | |          |  |
|          |       | | dogm.d.. | data        | |          |  |
|          |       | | dogm.i.. | information | |          |  |

Exceptions
  * dogmfont: fonts
  * dogmitoa: int to ascii
  * dogmsysc: system display contrast
  * dogmsysi: system display invert
  * dogmspi: SPI subsystem
  * dogmpgm: PROGMEM and ROM abstraction layer (only include file)

# Flash-ROM (PROGMEM) abstraction layer

## Macros
  * `DOG_ROM`: Will expand to the ROM keyword on PICs
  * `DOG_PROGMEM`: Will expand to the avr-gcc macro PROGMEM on avr systems.
  * `DOG_ATTR_FN_INLINE`: Will expand to the inline attribute on avr-gcc. This has nothing to do with the abstraction layer for flash-ROM, but is placed in the same file.
  * `DOG_PGM_P`: Generic rom pointer
  * `DOG_PSTR_P`: String pointer to a string which is placed in flash rom area
  * `dog_pgm_read(adr)`: Access a single byte. Argument is a pointer to the memory location
  * `DOG_PGM(str)`: Macro to put a constant string into flash rom area

## Types
  * `dog_pgm_uint8_t`: 8 Bit unsigned type, which is placed in flash-ROM/PROGMEM area.

# About `avr-size`

Example:
```
avr-size spacetrash.hex 
   text	   data	    bss	    dec	    hex	filename
      0	   7168	      0	   7168	   1c00	spacetrash.hex
avr-size spacetrash.elf 
   text	   data	    bss	    dec	    hex	filename
   7130	     38	    421	   7589	   1da5	spacetrash.elf
```
  * Flash-ROM size: 38 + 7130 = 7168 bytes
  * None-PROGMEM data (copyied from ROM to RAM): 38 bytes
  * PROGMEM data is placed in the text area and does not appear separately
  * Required RAM: 38 + 421 + stack = 459 bytes + stack

# Arduino IDE Build Files

Applies to UNIX IDE 0022

Intermediate build files of the Arduino IDE are stored here (Linux)
```
/tmp/build*.tmp/
```
or here (Windows)
```
C:\Users\<yourname>\AppData\Local\Temp\build*.tmp
```
hex and elf file are named like the sketch but have an `.cpp` appended:
```
/tmp/build7897268455122589440.tmp$ ls -al Chess*
-rw-r--r-- 1 kraus kraus   3242 2011-02-03 12:51 Chess.cpp
-rw-r--r-- 1 kraus kraus     13 2011-02-03 12:51 Chess.cpp.eep
-rwxr-xr-x 1 kraus kraus 139368 2011-02-03 12:51 Chess.cpp.elf
-rw-r--r-- 1 kraus kraus  33351 2011-02-03 12:51 Chess.cpp.hex
-rw-r--r-- 1 kraus kraus  13296 2011-02-03 12:51 Chess.cpp.o
```

Remove this build directory to force a complete rebuild.


# SW Development Principles
  * Create small and reliable interfaces (Never think about execution speed)
  * Build the program on the interfaces, always use the interface functions
  * Know your programming language