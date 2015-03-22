


# Features

## Top View

![http://wiki.dogm128.googlecode.com/hg/dogs102/LCD_full.jpg](http://wiki.dogm128.googlecode.com/hg/dogs102/LCD_full.jpg)

## Schematic

The schematic is available as PDF:
[LCDShield\_schematics.pdf](http://wiki.dogm128.googlecode.com/hg/dogs102/LCDShield_schematics.pdf)

## BOM

Parts from Mouser Electronics:
| `Pos`  | Part | Qty | Mouser Part Number |
|:-------|:-----|:----|:-------------------|
| `U1`   | LCD Display |  1  | EA DOGS102W-6      |
| `U1a`  | Backlight |  1  | EA LED39x41-A      |
| `R1`   | Resistor SMD 1206 10K | 1   | 290-10K-RC         |
| `R2`   | Resistor SMD 1206 15 Ohm | 1   | 290-15-RC          |
| `C1,C2,C6` | Ceramic Capacitor 0.1uF | 3   | C1206C104K5RACTU   |
| `C3,C4,C5` | Ceramic Capacitor 1.0uF | 3   | C1206C105K4RAC7210 |
| `IC1`  | Level Buffer | 1   | CD4050BDWR         |
| `S1-S4` | Pushbutton 6mm | 4   | TL1105FF160Q       |
| `JP1-4` | Pin Header | 1   | 68001-236HLF       |



# Software Support

## DOGM128 Library

This shield is fully supported by the DOGM128 library.

## Buttons

Buttons are assigned to the following pins of the Arduino Environment:

  * Top: Pin 5
  * Down: Pin 3
  * Left: Pin 2
  * Right: Pin 4

For all buttons the pull-up resistor must be activated. The following code
shows the setup of the "top" button:
```
  pinMode(5, INPUT);           // set pin to input
  digitalWrite(5, HIGH);       // turn on pullup resistors
```

## Backlight with PWM
Optional backlight brightness control can be activated using BKLT\_SRC jumper.
If set for PWM Arduino digital pin D6 will control brightness in 255 steps

Here is a code to activate PWM:
```
#define LCD_backlight  6                // D6 used for PWM
int LCD_bright=127;                     // set initial brightness at 50% 
pinMode(LCD_backlight,OUTPUT);          // Set Pin D6 for output
analogWrite(LCD_backlight,LCD_bright);  // Enable PWM and set initial value
```

## Single Gauge code
![http://www.plctools.com/images/shield_m1a.jpg](http://www.plctools.com/images/shield_m1a.jpg)
```
/*
  Test for the dogm128 library
  Developed by:
  (c) 2010 Alec N. (controlconn@gmail.com)
  
  Based on dogm128 Arduino library developed by:
  (c) 2010 Oliver Kraus (olikraus@gmail.com)
  
 
  Note:
    1) Set correct display hardware in Dogm/utility/dogm128.h
    2) Set top/bottom view (DOG_REVERSE) in Dogm/utility/dogm128.h
    See also: http://code.google.com/p/dogm128/wiki/install 
  
*/
#include <Dogm.h>
#define R 66
#define cX DOG_WIDTH/2-2
#define cY -25

#define MaxScale   90  // scaled value 0 to Max Scale
#define scaleAngle 50   // Scale Angle in relation to the full circle 256 point
#define scalePoints 6   // Scale Marks in addition to "0" mark, ex. 2 will set marks at 0, 50 , 100 

//calculated parameters - do not change!
#define scaleStart      (128-scaleAngle)/2
#define scaleEnd        128-scaleStart
#define scalePerPoint   scaleAngle*0.0245/scalePoints
#define scaleUnit       scaleAngle*0.0245/MaxScale
//=====================================
int a0Pin = 9;      // address line a0 for the dogm module

#define right_btn  4
#define left_btn 2
#define up_btn  5
#define down_btn  3

int udValue=MaxScale*0.2;
int lrValue=MaxScale*0.7;

Dogm dogm(a0Pin);

void setup() {
  pinMode(right_btn,INPUT);
  pinMode(left_btn,INPUT);
  pinMode(up_btn,INPUT);
  pinMode(down_btn,INPUT);
  
  digitalWrite(right_btn, HIGH); //enable pullup resistor
  digitalWrite(left_btn, HIGH);  //enable pullup resistor
  digitalWrite(up_btn, HIGH);    //enable pullup resistor
  digitalWrite(down_btn, HIGH);  //enable pullup resistor
 
}
void loop() {
  dogm.start();
  do {

     dogm.setFont(font_5x8);
     dogm.setRot(0);

     dogm.setBox(2,0,DOG_WIDTH-1,DOG_HEIGHT-3);
     dogm.setBox(0,1,DOG_WIDTH-2,DOG_HEIGHT-1);
     dogm.clrBox(1,2,DOG_WIDTH-3,DOG_HEIGHT-2);
     
     dogm.setSize(1);
     dogm.drawArc(cX, cY, R, scaleStart, scaleEnd); 
     dogm.setSize(0);
     dogm.drawArc(cX, cY, (R+3), scaleStart, scaleEnd);
     for (int i=0; i<=scalePoints; i++)
     {
       float a=scalePerPoint*i+scaleStart*0.0245;
       float ac=cos(a);
       float as=sin(a);
 
       
       dogm.setSize(0);
       dogm.drawLine(R*ac+cX, R*as+cY, (R+5)*ac+cX, (R+5)*as+cY);
       char *s=dog_itoa(MaxScale-i*(MaxScale/scalePoints));
       int sw=dogm.getStrWidth(s)/2;
       dogm.setXY((R+13)*ac+cX-sw, (R+13)*as+cY-4);
       dogm.drawStr(s);
     } 

       float a=scaleUnit*lrValue+scaleStart*0.0245;
       float ac=-cos(a);
       float as=sin(a);
       dogm.drawLine((R-27)*ac+cX, (R-27)*as+cY, (R-4)*ac+cX, (R-4)*as+cY);
  
       a=scaleUnit*udValue+scaleStart*0.0245;
       ac=-cos(a);
       as=sin(a);
       dogm.setSize(1);
       dogm.drawLine((R-27)*ac+cX, (R-27)*as+cY, (R-7)*ac+cX, (R-7)*as+cY);
 
       dogm.setBox(3,3,DOG_WIDTH-4,15);
       //dogm.setBox(4,3,DOG_WIDTH-4,14);
       dogm.clrBox(4,2,DOG_WIDTH-3,14);

       dogm.setXY(8, 5);
       dogm.drawStr("VarA=");
       dogm.setXY(55, 5);
       dogm.drawStr("VarB=");
       dogm.setXY(35, 5);
       dogm.drawStr(dog_itoa(udValue));
       dogm.setXY(82, 5);
       dogm.drawStr(dog_itoa(lrValue));
  
       
  }while( dogm.next() );
  
  if ((digitalRead(left_btn)==LOW) && (lrValue>0)) lrValue--;
  if ((digitalRead(right_btn)==LOW) && (lrValue<MaxScale)) lrValue++;

  if ((digitalRead(up_btn)==LOW) && (udValue<MaxScale)) udValue++;
  if ((digitalRead(down_btn)==LOW) && (udValue>0)) udValue--;

  dog_Delay(5);
  //page++;
  
}

```

## Dual Gauge Code
![http://www.plctools.com/images/shield_m2a.jpg](http://www.plctools.com/images/shield_m2a.jpg)
```
#include <Dogm.h>
#define R 38            // Scale Rradius 
#define cX1 -10           // Left  Scale X Center
#define cX2 111         // Right Scale X Center
#define cY 32           // Y center position
#define MaxScaleA   60  // scaled value 0 to Max Scale Left 
#define MaxScaleB   100  // scaled value 0 to Max Scale Right 
#define scaleAngle 50   // Scale Angle in relation to the full circle 256 point
#define scalePoints 4   // Scale Marks in addition to "0" mark, ex. 2 will set marks at 0, 50 , 100 

//calculated parameters - do not change!
#define scaleOffset     scaleAngle*0.0123  // =PI /128 /2 * scaleAngle
#define scalePerPoint   scaleOffset*2/scalePoints
#define scaleUnitA       scaleOffset*2/MaxScaleA
#define scaleUnitB       scaleOffset*2/MaxScaleB

//=====================================

int a0Pin = 9;      // address line a0 for the dogm module

#define right_btn  4
#define left_btn 2
#define up_btn  5
#define down_btn  3


int udValue= MaxScaleA*0.2;
int lrValue= MaxScaleB*0.8;

Dogm dogm(a0Pin);

void setup() {
  pinMode(right_btn,INPUT);
  pinMode(left_btn,INPUT);
  pinMode(up_btn,INPUT);
  pinMode(down_btn,INPUT);
  
  digitalWrite(right_btn, HIGH); //enable pullup resistor
  digitalWrite(left_btn, HIGH);  //enable pullup resistor
  digitalWrite(up_btn, HIGH);    //enable pullup resistor
  digitalWrite(down_btn, HIGH);  //enable pullup resistor
 
}

void loop() {
  dogm.start();
  do {

     dogm.setFont(font_5x8);
     dogm.setRot(0);
     
     dogm.setBox(0,0,50,63);
     dogm.clrBox(1,1,49,62);
     dogm.setBox(52,0,101,63);
     dogm.clrBox(53,1,100,62);
     
     dogm.setSize(0);
     dogm.drawArc(cX1, cY, R, -scaleAngle/2, scaleAngle/2);
     dogm.drawArc(cX2, cY, R, 128-scaleAngle/2, (scaleAngle/2)-128);
     
     dogm.setSize(0);
     dogm.drawArc(cX1, cY, R+3, -scaleAngle/2, scaleAngle/2);
     dogm.drawArc(cX2, cY, R+3, 128-scaleAngle/2, (scaleAngle/2)-128);
     
     for (int i=0; i<=scalePoints; i++)
     {
       float a=scalePerPoint*i-scaleOffset; 
       float ac=cos(a);
       float as=sin(a);
 
       
       dogm.setSize(0);
       dogm.drawLine(cX1+R*ac, R*as+cY, cX1+(R+6)*ac, (R+6)*as+cY);
       dogm.drawLine(cX2-R*ac, R*as+cY, cX2-(R+6)*ac, (R+6)*as+cY);
       
       char *s=dog_itoa(i*(MaxScaleA/scalePoints));
       int sw=dogm.getStrWidth(s);
       dogm.setXY(47-sw, (R+6)*as+cY-4);
       dogm.drawStr(s);
       
       s=dog_itoa(i*(MaxScaleB/scalePoints));
       dogm.setXY(55, (R+6)*as+cY-4);
       dogm.drawStr(s);
   } 

       float a=scaleUnitA*udValue-scaleOffset;
       float ac=cos(a);
       float as=sin(a);
       dogm.setSize(1);
       dogm.drawLine((R-23)*ac+cX1, (R-23)*as+cY, (R-5)*ac+cX1, (R-5)*as+cY);

       a=scaleUnitB*lrValue-scaleOffset;
       ac=-cos(a);
       as=sin(a);
       dogm.setSize(1);
       dogm.drawLine((R-23)*ac+cX2, (R-23)*as+cY, (R-5)*ac+cX2, (R-5)*as+cY);

       dogm.setXY(2, 55);
       dogm.drawStr("VarA");
       dogm.setXY(80,55);
       dogm.drawStr("VarB");
//       dogm.setFont(font_7x13);
       dogm.setXY(3, 1);
       dogm.drawStr(dog_itoa(udValue));
       dogm.setXY(84, 1);
       dogm.drawStr(dog_itoa(lrValue));
     
  }while( dogm.next() );
  
// button handling - simulation only - replace with real application  
  if ((digitalRead(left_btn)==LOW) && (lrValue>0)) lrValue--;
  if ((digitalRead(right_btn)==LOW) && (lrValue<MaxScaleB)) lrValue++;

  if ((digitalRead(down_btn)==LOW) && (udValue>0)) udValue--;
  if ((digitalRead(up_btn)==LOW) && (udValue<MaxScaleA)) udValue++;
  
  dog_Delay(5);
  //page++;
  
}
```



# Links

  * [Wiki Toplevel Page](dogm128.md)
  * [Wiki Hardware Overview](hardware.md)
  * [Project Home](http://code.google.com/p/dogm128/)
  * [Browse Source](http://code.google.com/p/dogm128/source/browse/)