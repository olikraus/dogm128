
#include <avr/io.h>
#include <Dogm.h>
#include "WProgram.h"

void Dogm::Init(void)
{
  is_req_init = 0;
  dog_init(a0Pin);
}

Dogm::Dogm(uint8_t pin_a0)
{
  tx = 0;
  ty = 0;
  a0Pin = pin_a0;
  is_req_init = 1;
  // why can dog_init() not be called here... arduino will hang if this is done in the constructor
  // should be investigated some day
  // dog_init(pin_a0);
  
}

void Dogm::Start(void)
{
  if ( is_req_init )
    Init();
  dog_StartPage();
}


