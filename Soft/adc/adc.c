/*********************************************************************
 * FileName:        adc.c
 ********************************************************************/

/** I N C L U D E S *************************************************/
#include <p18cxxx.h>
#include <delays.h>
#include "header.h"
#include "system\typedefs.h"
#include "system\cpucfg.h"			// I/O pin mapping
#include "adc\adc.h"				// ADC 

/** V A R I A B L E S ***********************************************/
byte VoltageValue, CurrentValue, AdjustValue;

/*********************************************************************
 * Function:			void InitVarADC(void)
 ********************************************************************/
void InitVarADC(void)
{
	VoltageValue = 0;
	CurrentValue = 0;
	AdjustValue = 0;
	//
	return;
}

/*********************************************************************
 * Function:        word ReadADC(byte channel)
 * Inctruction cycles:	230
 * Time cycles:			46 mks
 ********************************************************************/
word ReadADC(byte channel)
{
 WORD value;
 // select channel
 ADCON0 = channel;
 // delay
 Delay10TCYx(10); // delay 20mks
 // go ADC
 ADCON0bits.GO = true;
 while(ADCON0bits.GO) continue;
 LSB(value) = ADRESL;
 MSB(value) = ADRESH;
 return value.words; 
}

/*********************************************************************
 * Function:        word GetVoltageADC(void)
 * Input divisor of the voltage: 12V => 2,3V (ADC)=> 480 (>> 2)=> 120 
 * The function is calling in 10 ms once. 
 * To improve the measurement accuracy of 10 bits to 12 bits:
 * 1. Add 16 times measured 10 bit value
 *	  and shift to the right >> 2 (12-bit).
 * 2. The resulting 12-bit values are added 4 times
 *    and shift to the right >> 2, thus it averaging
 *    (i.e., filter the interference). As a result, accurate 12-bit.
 * Measure cycle time = 10mc x 16 x 2 = 0.32 seconds.
 ********************************************************************/
byte GetVoltageADC(void)
{
	static byte VoltBits = 0, VoltCount = 0;
	static word VoltADC = 0, VoltSum = 0;
	
	// measuer and 10 bits to 12 bits
	VoltADC += ReadADC(ADC_BAT);
	VoltBits++;
	if(VoltBits == 16)
	{
		// 12bit and average
		VoltSum += VoltADC >> 2;
		VoltCount++;
		if(VoltCount == 2)
		{
			VoltageValue = VoltSum >> 5; // Volt -> Display 
			if(VoltageValue < 7) VoltageValue = 0;
			//
			VoltSum = 0;
			VoltCount = 0;
		}
		VoltADC = 0;
		VoltBits = 0;
	}
	//
	return VoltageValue;
}

/*********************************************************************
 * Function:        word GetAdjustADC(void)
 * Input divisor of the voltage: 3,3V (ADC)=> 640 (>> 4)=> 40 steps
 * The function is calling in 10 ms once. 
 * To improve the measurement accuracy of 10 bits to 12 bits:
 * 1. Add 16 times measured 10 bit value
 *	  and shift to the right >> 2 (12-bit).
 * Measure cycle time = 10mc x 16 = 160 ms.
 ********************************************************************/
byte GetAdjustADC(void)
{
	static byte AdjBits = 0;
	static word AdjADC = 0;

	// measure and avarage
	AdjADC += ReadADC(ADC_ADJ);
	AdjBits++;
	if(AdjBits == 16)
	{
		AdjustValue = AdjADC >> 8; // Adjust -> Display 1...41
		if(AdjustValue == 0) AdjustValue = 1;
		AdjADC = 0;
		AdjBits = 0;
	}
	//
	return AdjustValue;
}

/*********************************************************************
 * Function:        byte GetCurrentADC(byte stop)
 * Input amplifier current: 10AV => 3,9V (ADC)=> 800 (>> 3)=> 100
 * Beginning of the period of the mains voltage - start measure.
 * Summing up all non-zero values and calculate RMS.
 * Measure cycle time = 10mc x 16 = 160 ms.
 ********************************************************************/
byte GetCurrentADC(byte flag)
{
	static byte CurrCount = 0, CurrBits = 0;
	static word CurrSum = 0, CurrADC = 0;
	word current;
	
	// measure current
	Delay10TCYx(25); // delay 50mks
	current = ReadADC(ADC_CUR);
	current = current >> 3;
	CurrCount++;
	if(current > 4)	CurrSum += current;
	// stop measure and calculate RMS
	if(flag == STOP_CUR_ADC)
	{
		current = CurrSum / CurrCount; // RMS
		// average
		CurrADC += current;
		CurrBits++;
		if(CurrBits == 16)
		{
			CurrentValue = CurrADC >> 4; // Current -> Display
			if(CurrentValue < 4) CurrentValue = 0;
			else if(CurrentValue < 14) CurrentValue -= 1;
			//
			CurrADC = 0;
			CurrBits = 0;
		}
		//
		CurrSum = 0;		
		CurrCount = 0;
	}
	//
	return CurrentValue;
}
