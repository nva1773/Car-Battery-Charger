/*********************************************************************
 * FileName:        eeprom.h
 ********************************************************************/
#ifndef EEPROM_H
#define EEPROM_H

/** P U B L I C  P R O T O T Y P E S ********************************/
byte ReadEEPROM(unsigned char addr);
void WriteEEPROM(byte data, unsigned char addr);

/** D E F I N E *****************************************************/
#define ADDR_MIN 0
#define ADDR_MAX 1

//
#endif //EEPROM_H
