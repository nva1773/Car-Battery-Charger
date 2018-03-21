/*********************************************************************
 * FileName:        debuger.c
 ********************************************************************/
 
/** I N C L U D E S *************************************************/
#include <p18cxxx.h>
#include "header.h"
#include "system\typedefs.h"
#include "system\cpucfg.h"			// IO configuration
#include "display\display.h"		// LED display
#include "eeprom\eeprom.h"			// EEPROM
#include "debuger\debuger.h"		// DEBUG


/** V A R I A B L E S ***********************************************/
extern BITS FlagADC, FlagMode;
extern byte Angle, DutyPWM;
extern byte Voltage, Current, Adjust;
extern byte AngleMin, AngleMax;
//
byte StateDEBUG;
enum stDEBUG{stVoltage=1,stAdjust,stCurrMin,stCurrMax,stCurrent,stPWM};

/*********************************************************************
 * Function:			void InitVarDebuger(void)
 ********************************************************************/
void InitVarDebuger(void)
{
    fInitControl = false;
    //
    fPushNext = false;
    fPushStorage = false;
    //
    fEnableCharge = false;
    fEnableDischarge = false;
    mStopPWM();
    //
    StateDEBUG = stVoltage;
    fNewVoltage = true;
    //
    fInitDebuger = true;
    //
    return;
}

/*********************************************************************
 * Function:			void Debuger(void)
 ********************************************************************/
void Debuger(void)
{
	// Temporary variable
	byte dummy;
	
	// State machine for Debuger
	switch(StateDEBUG)
	{
		// Display Voltage
		case stVoltage:
			if(fNewVoltage)
			{
				SetVoltageDisplay(Voltage);
				SetCurrentDisplay(stVoltage);
				fEnableCharge = false;
				fNewVoltage = false;
			}
			if(fPushNext)
			{
				fPushNext = false;
				fPushStorage = false;
				fNewAdjust = true;
				StateDEBUG++;
			}
			break;
		// Display Adjust
		case stAdjust:
			if(fNewAdjust)
			{
				SetVoltageDisplay(Adjust);
				SetCurrentDisplay(stAdjust);
				fNewAdjust = false;
			}
			if(fPushNext)
			{
				fPushNext = false;
				fPushStorage = false;
				fNewAdjust = true;
				fNewCurrent = true;
				StateDEBUG++;
			}
			break;
		// Display and Set MIN current
		case stCurrMin:
			if(fNewAdjust)
			{
				Angle = ANGLE_MIN - (Adjust>>1);
				fNewAdjust = false;
			}
			if(fNewCurrent)
			{
				SetVoltageDisplay(Current);
				SetCurrentDisplay(stCurrMin);
				fEnableCharge = true;
				fNewCurrent = false;
			}
			if(fPushStorage)
			{
				fPushStorage = false;
				// Storage angle MIN in to EEPROM
				AngleMin = Angle;
				WriteEEPROM(AngleMin, ADDR_MIN);
			}
			if(fPushNext)
			{
				fPushNext = false;
				fNewAdjust = true;
				fNewCurrent = true;
				fEnableCharge = false;
				StateDEBUG++;
			}
			break;
		// Display and Set MAX current
		case stCurrMax:
			if(fNewAdjust)
			{
				Angle = ANGLE_MAX + STEP_MAX - (Adjust>>1);
				fNewAdjust = false;
			}
			if(fNewCurrent)
			{
				SetVoltageDisplay(Current);
				SetCurrentDisplay(stCurrMax);
				fEnableCharge = true;
				fNewCurrent = false;
			}
			if(fPushStorage)
			{
				fPushStorage = false;
				// Storage angle MAX in to EEPROM
				AngleMax = Angle;
				WriteEEPROM(AngleMax, ADDR_MAX);
				
			}
			if(fPushNext)
			{
				fPushNext = false;
				fNewCurrent = true;
				fNewAdjust = true;
				fEnableCharge = false;
				StateDEBUG++;
			}
			break;
		// Display Current and Triac control 
		case stCurrent:
			if(fNewAdjust)
			{
				dummy = AngleMin - Adjust;
				if(dummy < AngleMax)
					Angle = AngleMax;
				else
					Angle = dummy;
				//
				fNewAdjust = false;
			}
			if(fNewCurrent)
			{
				SetVoltageDisplay(Current);
				SetCurrentDisplay(stCurrent);
				fEnableCharge = true;
				fNewCurrent = false;
			}
			if(fPushNext)
			{
				fPushNext = false;
				fPushStorage = false;
				fEnableCharge = false;
				fNewAdjust = true;
				StateDEBUG++;
			}
			break;
		// Display Adjust and PWM control
		case stPWM:
			if(fNewAdjust)
			{
				SetVoltageDisplay(Adjust);
				SetCurrentDisplay(stPWM);
				fNewAdjust = false;
				DutyPWM = PWM_MIN + (Adjust<<1);
				if(!fEnableDischarge)
				{
					fEnableDischarge = true;
					mStartPWM();
				}
			}
			if(fPushNext)
			{
				fPushNext = false;
				fPushStorage = false;
				fNewVoltage = true;
				fEnableCharge = false;
				fEnableDischarge = false;
				mStopPWM();
				StateDEBUG = stVoltage;
			}
			break;		
	}
	//
	return;
}