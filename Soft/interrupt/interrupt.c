/*********************************************************************
* interrupt.c
*********************************************************************/

/** I N C L U D E S *************************************************/
#include <p18cxxx.h>
#include "header.h"
#include "system\typedefs.h"
#include "system\cpucfg.h"
#include "interrupt\interrupt.h"
#include "display\display.h"
#include "input\input.h"

/** V A R I A B L E S ***********************************************/
extern BITS FlagADC, FlagMode;
extern byte Angle, DutyPWM;

/** I N T E R R U P T  V E C T O R S ********************************/
#pragma code high_vector=0x08
void interrupt_high_vector(void)
{
    _asm goto isrHigh _endasm
}
#pragma code

#pragma code low_vector=0x18
void interrupt_low_vector(void)
{
    _asm goto isrLow _endasm
}
#pragma code

/** D E C L A R A T I O N S *****************************************/
/*********************************************************************
 * Function:	void isrHigh(void)
 ********************************************************************/
#pragma interrupt isrHigh
void isrHigh(void)
{
	static byte CountImpulse = 0;
	
	// Sync interrupt
	if(mInterruptSync)
	{
		mInitSync();
		if(mSync)
		{
			fEnableVoltADC = true;
			mTriac = TRIAC_OFF;
		}
		else
		{
			if(fEnableCharge)
			{
				CountImpulse = 0;
				mInitImpulse();
				mStartImpulse();
				fEnableCurrADC = true;
			}
			else
			{
				mTriac = TRIAC_OFF;
			}
		}
		mToggleSync();
	}
	
	// Impulse interrupt once every 100mks
	if(mInterruptImpulse)
	{
		mInitImpulse();
		if(mTriac == TRIAC_OFF)
		{
			CountImpulse++;
			if(CountImpulse >= Angle) mTriac = TRIAC_ON;
		}
		else
		{
			mTriac = TRIAC_OFF;
			mStopImpulse();
		}
	}
}
#pragma code

/*********************************************************************
 * Function:	void isrLow(void)
 ********************************************************************/
#pragma interruptlow isrLow
void isrLow(void)
{
	// Temporary variable
	byte dummy;
	
	// Timer Tick interrupt once every 3 ms
	if(mInterrupTick)
	{
		mInitTick();
		// Update Dislpay
		UpdateDisplay();
		// Read input Debug
		dummy = ReadDebug();
		if(dummy == TRUE) fDebugMode = true;
		else fDebugMode = false;
		// Read input Button
		dummy = ReadButton();
		if(dummy == NEXT) fPushNext = true;
		if(dummy == STORAGE) fPushStorage = true;
		// Tick new
		fNewTick = true;
	}
	
	// PWM interrupt
	if(mInterruptPWM)
	{
		mInterruptPWM = false;
		mLoadDutyPWM(DutyPWM);
	}
}
#pragma code

/** EOF interrupt.c *************************************************/
