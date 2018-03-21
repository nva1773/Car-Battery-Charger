/*********************************************************************
 * FileName:        input.c
 ********************************************************************/
 
/** I N C L U D E S *************************************************/
#include <p18cxxx.h>
#include "header.h"
#include "system\typedefs.h"
#include "system\cpucfg.h"             // I/O pin mapping
#include "input\input.h"

/** V A R I A B L E S ***********************************************/
BITS FlagInput;

/*********************************************************************
 * Function:			void InitVarInput(void)
 ********************************************************************/
void InitVarInput(void)
{
    FlagInput.bits = 0;
    //
    return;
}

/*********************************************************************
 * Function:			byte ReadDebug(void)
 ********************************************************************/
byte ReadDebug(void)
{
    static byte TimerDebug = 0;
    byte debug;
    //
    if(!fDebugDelay)
	{
		fDebugInput = mDebug;
		// Select Debug Mode
		if((fDebugInput == DEBUG)&&(!fDebugOn))
		{
			TimerDebug = BOUNCE;
			fDebugDelay = true;
			fDebugOn = true;
		}
		// Release BUTTON
		if((fDebugInput != DEBUG)&&(fDebugOn))
		{
			TimerDebug = BOUNCE;
			fDebugDelay = true;
			fDebugOn = false;
		}
	}
	// Delay for contact bounce
	else
	{
		TimerDebug--;
		if(TimerDebug == 0) fDebugDelay = false;
	}
    // return
    if(fDebugOn) debug = TRUE;
    else debug = FALSE;
    //
    return debug;
}

/*********************************************************************
 * Function:			byte ReadButton(void)
 ********************************************************************/
byte ReadButton(void)
{
	static byte TimerButton = 0;
	static word TimerPress = 0;
	byte button;
	//
	button = FALSE;
	if(!fButtonDelay)
	{
		fButtonInput = mButton;
		// Push BUTTON
		if((fButtonInput == PUSH)&&(!fButtonOn))
		{
			TimerButton = BOUNCE;
			fButtonDelay = true;
			fButtonOn = true;
		}
		// Press BUTTON 
		if((fButtonInput == PUSH)&&(fButtonOn)&&(!fButtonPress))
		{
			TimerPress++;
			if(TimerPress > PRESS)
			{
				fButtonPress = true;
				// return
				button = STORAGE;
			}
		}
		// Release BUTTON
		if((fButtonInput != PUSH)&&(fButtonOn))
		{
			TimerButton = BOUNCE;
			fButtonDelay = true;
			fButtonOn = false;
			// return
			if(!fButtonPress) button = NEXT;
			// reset press
			fButtonPress = false;				
			TimerPress = 0;
		}
	}
	// Delay for contact bounce
	else
	{
		TimerButton--;
		if(TimerButton == 0) fButtonDelay = false;
	}
    //
    return button;
}
