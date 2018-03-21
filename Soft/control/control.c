/*********************************************************************
 * FileName:        control.c
 ********************************************************************/
 
/** I N C L U D E S *************************************************/
#include <p18cxxx.h>
#include "header.h"
#include "system\typedefs.h"
#include "system\cpucfg.h"			// IO configuration
#include "display\display.h"		// LED display
#include "control\control.h"		// CONTROL


/** V A R I A B L E S ***********************************************/
extern BITS FlagADC, FlagMode;
extern byte Angle, DutyPWM;
extern byte Voltage, Current, Adjust;
extern byte AngleMin, AngleMax;
//
BITS FlagControl;
char AngleOffset;
word TimerControl;

/*********************************************************************
 * Function:			void InitVarControl(void)
 ********************************************************************/
void InitVarControl(void)
{
    fInitDebuger = false;
    FlagControl.bits = 0;
    AngleOffset = 0;
	TimerControl = 0;
    //
    fEnableCharge = false;
    fEnableDischarge = false;
    mStopPWM();
    //
    fNewVoltage = true;
    fNewAdjust = true;
    Current = 0;
    //
    fInitControl = true;
    return;
}

/*********************************************************************
 * Function:			void Control(void)
 ********************************************************************/
void Control(void)
{
	static byte VoltStorage = 0;
	static word TimerAdjust = 0;
	// Temporary variable
	byte dummy;
	
	// Tick once every 3 ms -> Increment timers
	if(fNewTick){ TimerControl++; TimerAdjust++; fNewTick = false;}
	
	// New Adjust -> Update Control and Show Adjust
	if(fNewAdjust)
	{
		// Enable update Angle and Duty
		fUpdateControl = true;
		// Show Adjust value on display
		fDisplayAdjust = true;
		TimerAdjust = 0;
		//
		fNewAdjust = false;
	}
	
	// Update Angle and Duty
	if(fUpdateControl)
	{
		// New Duty
		DutyPWM = PWM_MIN + (Adjust<<1);
		// New Angle
		dummy = AngleMin - Adjust - AngleOffset;
		if(dummy < AngleMax)
			Angle = AngleMax;
		else
			Angle = dummy;
		//
		fUpdateControl = false;
	}

	// Check voltage delta -> detect too fast change
	if(Voltage > VoltStorage)
		dummy = Voltage - VoltStorage;
	else
		dummy = VoltStorage - Voltage;
	if(dummy > DELTA)
	{
		fFastChange = true;
		TimerControl = 0;
	}
	VoltStorage = Voltage;
		
	// Too fast change of voltage -> turn off process
	if(fFastChange)
	{
		if(fStartProcess)
		{
			ProcessOff();
		}
		if(TimerControl >= DELTA_TIME) fFastChange = false;
	}
	// else -> Check the battery is connected and process control
	else
	{
		if(Voltage > BATTERY_SET)
		{
			ProcessRun();
		}
		else if(fStartProcess || fFirstConnect)
		{
			ProcessOff();
		}
	}
		
	// Display Voltage
	if((fNewVoltage && !fStartProcess)||(fStartProcess && fDisplayVoltage))
	{
		SetVoltageDisplay(Voltage);
		fNewVoltage = false;
		fDisplayVoltage = false;
	}
	
	// Display Adjust or Current
	if(fDisplayAdjust)
	{
		if(TimerAdjust == 0)
		{
			dummy = Adjust + 9;
			SetCurrentDisplay(dummy);
		}
		if(TimerAdjust >= ADJUST_TIME)
		{
			fDisplayAdjust = false;
			fDisplayCurrent = true;
		}
	}
	else if(fDisplayCurrent)
	{
		SetCurrentDisplay(Current);
		fNewCurrent = false;
		fDisplayCurrent = false;
	}
		
	//
	return;
}

/*********************************************************************
 * Function:			void ProcessOff(void)
 * Inctruction cycles:	22
 * Time cycles:			4,4 mks
 ********************************************************************/
void ProcessOff(void)
{
	// Stop process
	fStartProcess = false;
	fFirstConnect = false;
	// Charge off
	fEnableCharge = false;
	// Discharge off
	fEnableDischarge = false; 
	mStopPWM();
	// Update current display
	Current = 0;
	fDisplayCurrent = true;
	// Clear Offset
	AngleOffset = 0;
	fUpdateControl = true;
	//
	return;
}

/*********************************************************************
 * Function:			void ProcessRun(void)
 ********************************************************************/
void ProcessRun(void)
{
	static byte CounterEnd = 0;
	
	// Start porcess
	if(!fStartProcess) 
	{
		// Start
		if((fFirstConnect == false)||(Voltage < CHARGE_START))
		{
			fFirstConnect = true;
			fStartProcess = true;
			fEnableCharge = true;
			TimerControl = 0;
			CounterEnd = 0;
		}
	}
			
	// Stop Charge and Start Discharge
	if((fEnableCharge == true)&&(TimerControl >= CHARGE_TIME))
	{
		// Automatic adjust current (Adjust 1...41; Current 10...50)
		if(Current <= Adjust + 8)
		{
			AngleOffset++;
			fUpdateControl = true;
		}
		if(Current >= Adjust + 10)
		{
			AngleOffset--;
			fUpdateControl = true;
		}
		// Stop Charge
		TimerControl = 0;
		fEnableCharge = false;
		// Start Discharge
		fEnableDischarge = true;
		mStartPWM();
		// Update Display
		fDisplayVoltage = true;
		fDisplayCurrent = true;
	}
	
	// Stop Discharge and Start Charge
	if((fEnableDischarge == true)&&(TimerControl >= DISCHARGE_TIME))
	{
		// Stop Discharge
		TimerControl = 0;
		fEnableDischarge = false;
		mStopPWM();
		// Start Charge
		fEnableCharge = true;
	}
	
	// Stop process
	if(fDisplayVoltage)
	{
		// Stop
		if(Voltage >= CHARGE_END)
		{
			CounterEnd++;
			if(CounterEnd >= MAGIC_NUM)
			{
				CounterEnd = 0;
				fStartProcess = false;
				fEnableCharge = false;
				fEnableDischarge = false;
				mStopPWM();
				// Update current display
				Current = 0;
				fDisplayCurrent = true;
			}
		}
		else CounterEnd = 0;
	}
	
	//
	return;
}
