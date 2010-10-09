/*

  dogmpgm.h
  
  Abstraction layer for ROM and PRGMEM concepts and keywords.

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

*/

/*
  dog_pgm_uint8_t
    uint8_t data keyword which can be used in declarations (as replacement for 
    prog_uint8_t from avr lib)
    
  DOG_PGM_P
    A macro, intended to be used as more portable replacement for the PGM_P from avr lib.
    
  dog_pgm_read(adr)
    return a value from flash rom, returns uint8_t
    
  DOG_PSTR("xyz")
    Return pointer to rom with the string "xyz"
    
    
  see also http://code.google.com/p/dogm128/wiki/
*/

#ifndef _DOGMPGM_H
#define _DOGMPGM_H

/*========================================================
  ATMEL AVR 
*/
#if defined(__AVR__)
#include <stdint.h>
#include <avr/pgmspace.h>
typedef uint8_t PROGMEM dog_pgm_uint8_t;
typedef char PROGMEM dog_pgm_char_t;
#define dog_pgm_read(adr) pgm_read_byte_near(adr)
#define DOG_ATTR_FN_INLINE __attribute__ ((noinline))
#define DOG_PROGMEM PROGMEM
#define DOG_ROM
#define DOG_PSTR(s) PSTR(s)

/*========================================================
  MICROCHIP PIC18  
*/
#elif defined(__18CXX)
typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef signed int int16_t;
typedef unsigned int uint16_t;
typedef uint8_t rom dog_pgm_uint8_t;
typedef char rom dog_pgm_char_t;
#define dog_pgm_read(adr) (*(const dog_pgm_uint8_t *)(adr)) 
#define DOG_ATTR_FN_INLINE
#define DOG_PROGMEM
#define DOG_ROM rom
/* Strings in C18 are always in ROM */
#define DOG_PSTR(s) (s)


/*========================================================
  Fallback: Standard C
*/
#else
#include <stdint.h>
typedef uint8_t dog_pgm_uint8_t;
typedef char dog_pgm_char_t;
#define dog_pgm_read(adr) (*(const dog_pgm_uint8_t *)(adr)) 
#define DOG_ATTR_FN_INLINE
#define DOG_PROGMEM
#define DOG_ROM
#define DOG_PSTR(s) (s)
#endif

/*========================================================
  All Systems
*/
#define DOG_PGM_P const dog_pgm_uint8_t *
#define DOG_PSTR_P const dog_pgm_char_t *

#endif /* _DOGMPGM_H */

