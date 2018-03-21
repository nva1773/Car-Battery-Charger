/*********************************************************************
 * FileName:        adc.h
 ********************************************************************/
#ifndef ADC_H
#define ADC_H

/** P U B L I C  P R O T O T Y P E S ********************************/
void InitVarADC(void);
word ReadADC(byte channel);
byte GetVoltageADC(void);
byte GetAdjustADC(void);
byte GetCurrentADC(byte stop);

/** D E F I N E *****************************************************/
#define RUN_CUR_ADC 1
#define STOP_CUR_ADC 0
//
#endif //ADC_H