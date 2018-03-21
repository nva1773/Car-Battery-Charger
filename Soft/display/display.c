/*********************************************************************
 * FileName:        display.c
 ********************************************************************/

/** I N C L U D E S *************************************************/
#include <p18cxxx.h>
#include "header.h"
#include "system\typedefs.h"
#include "system\cpucfg.h"             // I/O pin mapping
#include "display\display.h"

/** C O N S T A N T S ***********************************************/
const rom char DIGITAL[]={//HGFEDCBA
			 			   0b00111111,//0
			 			   0b00000110,//1
			 			   0b01011011,//2
			 			   0b01001111,//3
			 			   0b01100110,//4
			 			   0b01101101,//5
			 			   0b01111101,//6
			 			   0b00000111,//7
			 			   0b01111111,//8
			 			   0b01101111,//9
			 			   0b01000000};//10 -

const rom char NUMBER[]={0b00000001,//0
						 0b00001000,//1
						 0b00010000,//2
						 0b00100000,//3
						 0b01000000,//4
						 0b10000000};//5

/** V A R I A B L E S ***********************************************/
byte StateLED;
byte CurrentLED[3]= {0x00,0x00,0x00};
byte VoltageLED[3]= {0x00,0x00,0x00};
enum stLED{stCat1,stCat2,stCat3,stCat4,stCat5,stCat6};

/*********************************************************************
 * Function:			void InitVarLED(void)
 ********************************************************************/
void InitVarDisplay(void)
{
    StateLED = stCat1;
    BCD(0, VoltageLED);
    BCD(0, CurrentLED);
    //
    return;
}

/*********************************************************************
 * Function:			void SetVoltageLED(word data)
 ********************************************************************/
void SetVoltageDisplay(word data)
{
	BCD(data, VoltageLED);
}

/*********************************************************************
 * Function:			void SetCurrentLED(word data)
 ********************************************************************/
void SetCurrentDisplay(word data)
{
	BCD(data, CurrentLED);
}

/*********************************************************************
 * Function:			void BCD(unsigned int data)
 * Inctruction cycles:	71 min ... 877 max
 * Time cycles:			14 mks ... 175 mks
 ********************************************************************/
void BCD(word data, byte *bcd)
{
 	static char buffer[] = {0x00,0x00,0x00,'\0'};
	if(data == 0)
	{
		// ---
		mDisableTick();
		*bcd = DASH;
		bcd++;
		*bcd = DASH;
		bcd++;
		*bcd = DASH;
		mEnableTick();
	}
	else
	{
		itoBCD(data, buffer);
		//
		mDisableTick();
		*bcd = buffer[1]&0x0f;
		bcd++;
		*bcd = (buffer[2]&0xf0)>>4;
		bcd++;
		*bcd = buffer[2]&0x0f;
		mEnableTick();
	}
	//
 	return;
}
 
/********************************************************************
 * Function:      void BCD(unsigned int data)
 * 6 cycle * 3 ms = 18 ms
 * Inctruction cycles:	45
 * Time cycles:			9 mks
 *******************************************************************/
void UpdateDisplay(void)
{
	switch(StateLED)
	{
		case stCat1:
			mDisplayOff();
			mAnodeLED = DIGITAL[CurrentLED[Dig1]];
			mCatodeLED += NUMBER[stCat1];
			StateLED++;
			break;
		case stCat2:
			mDisplayOff();
			mAnodeLED = DIGITAL[CurrentLED[Dig2]] + POINT;// DP
			mCatodeLED += NUMBER[stCat2];
			StateLED++;
			break;
		case stCat3:
			mDisplayOff();
			mAnodeLED = DIGITAL[CurrentLED[Dig3]];
			mCatodeLED += NUMBER[stCat3];
			StateLED++;
			break;
		case stCat4:
			mDisplayOff();
			mAnodeLED = DIGITAL[VoltageLED[Dig1]];
			mCatodeLED += NUMBER[stCat4];
			StateLED++;
			break;
		case stCat5:
			mDisplayOff();
			mAnodeLED = DIGITAL[VoltageLED[Dig2]] + POINT;// DP
			mCatodeLED += NUMBER[stCat5];
			StateLED++;
			break;
		case stCat6:
			mDisplayOff();
			mAnodeLED = DIGITAL[VoltageLED[Dig3]];
			mCatodeLED += NUMBER[stCat6];
			StateLED = stCat1;
			break;
	}
	//
	return;
}
