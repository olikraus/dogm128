
#include <avr/io.h>
#include <Dogm.h>
#include "WProgram.h"


void Dogm::Init(void)
{
  is_req_init = 0;
  dog_InitA0CS(a0Pin, csPin);
}

Dogm::Dogm(uint8_t pin_a0)
{
  rot = 0;
  size = 0;
  tx = 0;
  ty = 0;
  a0Pin = pin_a0;
  csPin = 255;
  is_req_init= 1;
  libinfo_done = 0;
  // why can dog_Init() not be called here... arduino will hang if this is done in the constructor
  // should be investigated some day
  // dog_Init(pin_a0);  
}

Dogm::Dogm(uint8_t pin_a0, uint8_t pin_cs)
{
  rot = 0;
  size = 0;
  tx = 0;
  ty = 0;
  a0Pin = pin_a0;
  csPin = pin_cs;
  is_req_init= 1;
  // why can dog_Init() not be called here... arduino will hang if this is done in the constructor
  // should be investigated some day
  // dog_Init(pin_a0);
}


void Dogm::start(void)
{
  if ( is_req_init )
    Init();
  dog_StartPage();
}

// values between 0 and 63 allowed
void Dogm::setContrast(uint8_t val)
{
  if ( is_req_init )
    Init();
  dog_SetContrast(val);
}

void Dogm::setInvertPixelMode(uint8_t val)
{
  if ( is_req_init )
    Init();
  dog_SetInvertPixelMode(val);
}



void Dogm::xy_char_correction(uint8_t len)
{
  switch( rot )
  {
    default:
      tx += len;
      break;
    case 1:
      ty += len;
      break;
    case 2:
      tx -= len;
      break;
    case 3:
      ty -= len;
      break;
  }
}

void Dogm::drawChar(uint8_t c) 
{
  xy_char_correction(dog_DrawRChar(tx, ty, rot, fptr, c));  
}
void Dogm::drawStr(const char *s) 
{ 
  xy_char_correction(dog_DrawRStr(tx, ty, rot, fptr, s));
}

void Dogm::showLibInfo(void)
{
  if ( libinfo_done == 0 )
  {
    start();
    do
    {
      libinfo_draw();
    } while( next() );
    libinfo_done = 1;
    dog_Delay(2000);
  }
}

