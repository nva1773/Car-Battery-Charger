// Automatic car charger with the desulphation.
// Processor:       PIC18F252
// Compiler:        C18 3.34 
/** I N C L U D E S *************************************************/
#include <p18f252.h>
#include <delays.h>
#include "header.h"
#include "system\typedefs.h"
#include "system\cpucfg.h"			// IO configuration
#include "interrupt\interrupt.h"	// Interrupts
#include "display\display.h"		// LED display
#include "input\input.h"			// INPUT
#include "adc\adc.h"				// ADC
#include "debuger\debuger.h"		// DEBUGER
#include "control\control.h"		// CONTROL
#include "eeprom\eeprom.h"			// EEPROM

/** C O N F I G U R A T I O N   B I T S *****************************/ 
#pragma config OSC = HS, OSCS = OFF
#pragma config PWRT = ON
#pragma config BOR = ON, BORV = 42
#pragma config WDT = ON
#pragma config WDTPS = 128
#pragma config CCP2MUX = OFF
#pragma config STVR = ON
#pragma config LVP = OFF
#pragma config CP0 = OFF
#pragma config CP1 = OFF

/** P R I V A T E  P R O T O T Y P E S ******************************/
void InitCPU(void);
void InitVar(void);

/** G L O B A L   V A R I A B L E S *********************************/
BITS FlagADC, FlagMode;
byte Angle, DutyPWM;
byte Voltage, Current, Adjust;
byte AngleMin, AngleMax;

/*********************************************************************
 * Function:        void main(void)
 ********************************************************************/
void main(void)
{
	// Temporary variable
	byte dummy;
	
	// Init ports and delay 50ms 
	ClrWdt();
	mInitPorts();
	Delay1KTCYx(250);//50ms
	
	// Init CPU and Variable
	InitCPU();
	InitVar();
	
	//Run and enable interrupt
	mStartTick();
	mStartSync();
	mEnableInterrupt();
	
	// Main cycle - Read ADC, Debuger, Controll
	while(1)
	{
		// Clear Watchdog Timer (Period = 18ms * 128 = 2304 ms)
		ClrWdt();
		
		// Read Voltage and Adjust
		if(fEnableVoltADC && !fEnableCurrADC)
		{
			fEnableVoltADC = false;
			// Voltage
			dummy = GetVoltageADC();
			if(Voltage != dummy)
			{
				Voltage = dummy;
				fNewVoltage = true;
			}
			// Adjust
			dummy = GetAdjustADC();
			if(Adjust != dummy)
			{
				Adjust = dummy;
				fNewAdjust = true;
			}
		}
		
		// Read Current
		if(fEnableCurrADC)
		{
			if(fEnableVoltADC) fEnableCurrADC = false;
			//
			if(fEnableCurrADC) dummy = GetCurrentADC(RUN_CUR_ADC);
			else dummy = GetCurrentADC(STOP_CUR_ADC);
			//
			if(Current != dummy)
			{
				Current = dummy;
				fNewCurrent = true;
			}
		}
		
		// Debug mode
		if(fDebugMode)
		{
			if(!fInitDebuger) InitVarDebuger();
			//
			Debuger();
		}
		// Control mode
		else
		{
			if(!fInitControl) InitVarControl();
			//
			Control();
		}
	}//end while
}

/*********************************************************************
 * Function:		void InitCPU(void)
 ********************************************************************/
void InitCPU(void)
{
 // Interrupt (See interrupt.h)
    mPriorityInterrupt(true);
    // Timer0 -> Sync on T0CKI _/- (See cpucfg.h)
    mInitTimer0();
    mLoadTimer0();
    // Timer1 -> Impmulse (100 mks)
    mInitTimer1();
    mLoadTimer1();
    // Timer3 -> used for timing (3 ms)
    mInitTimer3();
    mLoadTimer3();
    // ADC
    mInitADC();
    // PWM
    mInitPWM();
    mLoadDutyPWM(PWM_MIN);
    //
    return;
}

/*********************************************************************
 * Function:		void InitVariable(void)
 ********************************************************************/
void InitVar(void)
{
	// Bits - See header.h
	FlagADC.bits = 0;
	FlagMode.bits = 0;
	//
	Voltage = 0;
	Current = 0;
	Adjust = 0;
	//
	AngleMin = ReadEEPROM(ADDR_MIN);
	AngleMax = ReadEEPROM(ADDR_MAX);
	Angle = AngleMin;
	//
	DutyPWM = PWM_MIN;
	//
	InitVarDisplay();
	InitVarInput();
	InitVarADC();
	//
	return;
}
