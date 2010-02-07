
#ifndef Dogm_h
#define Dogm_h

#include <stdint.h>
#include "utility/dogm128.h"
#include "Print.h"

class Dogm : public Print {
    uint8_t a0Pin;
    uint8_t is_req_init;	// if call to Init() is required
    PGM_P fptr;
    void Init(void);
  public:
    // text cursor position
    uint8_t tx, ty;
  
    Dogm(uint8_t pin_a0); 
      
    void Start(void);
    uint8_t Next(void) { return dog_NextPage(); }
    
    void SetPixel(uint8_t x, uint8_t y) { dog_SetPixel(x, y); }
    void ClrPixel(uint8_t x, uint8_t y) { dog_ClrPixel(x, y); }

    /* y1 must be lower or equal to y2 */
    void SetVLine(uint8_t x, uint8_t y1, uint8_t y2) { dog_SetVLine(x, y1, y2); }
    void ClrVLine(uint8_t x, uint8_t y1, uint8_t y2) { dog_ClrVLine(x, y1, y2); }
    void XorVLine(uint8_t x, uint8_t y1, uint8_t y2) { dog_XorVLine(x, y1, y2); }

    /* x1 must be lower or equal to x2 */
    /* y1 must be lower or equal to y2 */
    void SetBox(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) { dog_SetBox(x1, y1, x2, y2); }
    void ClrBox(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) { dog_ClrBox(x1, y1, x2, y2); }
    void XorBox(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) { dog_XorBox(x1, y1, x2, y2); }

    void SetFont(PGM_P font) { fptr = font; }
    void SetXY(uint8_t x, uint8_t y) { tx = x; ty = y; }
    void Char(uint8_t c) { tx += dog_DrawChar(tx, ty, fptr, c); }
    void write(uint8_t c) { tx += dog_DrawChar(tx, ty, fptr, c); }
    
    
};

#endif 