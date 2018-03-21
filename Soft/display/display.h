/*********************************************************************
 * FileName:        display.h
 ********************************************************************/
#ifndef DISPLAY_H
#define DISPLAY_H

/** P U B L I C  P R O T O T Y P E S ********************************/
void InitVarDisplay(void);
void SetVoltageDisplay(word data);
void SetCurrentDisplay(word data);
void itoBCD(auto int value,auto char *s);
void BCD(word data, byte *out);
void UpdateDisplay(void);

/** D E F I N E *****************************************************/
#define Dig1	0
#define Dig2	1
#define Dig3	2
#define POINT	0b10000000
#define DASH	10

//
#endif //DISPLAY_H
