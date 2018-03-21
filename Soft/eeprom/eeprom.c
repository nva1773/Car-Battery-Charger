/*********************************************************************
 * FileName:        eeprom.c
 ********************************************************************/
 
/** I N C L U D E S *************************************************/
#include <p18cxxx.h>
#include "header.h"
#include "system\typedefs.h"
#include "eeprom\eeprom.h"			// EEPROM

/** E E P R O M  I N I T I A L I Z A T I O N *********************************/
// Max and Min angle controll in impulse (100mks)
#pragma romdata eedata=0xF00000			// start address 0õ00
rom byte EEAngleMin = ANGLE_MIN;
rom byte EEAngleMax = ANGLE_MAX;
#pragma romdata

/*********************************************************************
 * Function:		byte ReadEEPROM(unsigned char addr)
 ********************************************************************/
byte ReadEEPROM(unsigned char addr)
{
 byte data;
 EECON1 = 0;
 EEADR = addr;
 EECON1bits.RD = 1;
 data = EEDATA;
 return data;
}

/*********************************************************************
 * Function:		void WriteEEPROM(byte data, unsigned char addr)
 ********************************************************************/
void WriteEEPROM(byte data, unsigned char addr)
{
 EECON1 = 0;
 EEADR = addr;
 EEDATA = data;
 EECON1bits.WREN = 1;
 EECON2 = 0x55;
 EECON2 = 0xAA;
 EECON1bits.WR =1;
 while(EECON1bits.WR);
 EECON1bits.WREN = 0;
 //
 return;
}
