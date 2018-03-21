/*********************************************************************
 * FileName:        header.h
 ********************************************************************/
#ifndef _HEADER_H_
#define _HEADER_H_

// WORD
#define LSB(a)      ((a).v[0])
#define MSB(a)      ((a).v[1])

// BOOL
#define false 0
#define true 1
#define FALSE 0
#define TRUE 1

// FLAG ADC
#define fEnableVoltADC		FlagADC.b0
#define fEnableCurrADC		FlagADC.b1
#define fNewVoltage			FlagADC.b2
#define fNewAdjust			FlagADC.b3
#define fNewCurrent			FlagADC.b4

// Flag MODE
#define fDebugMode			FlagMode.b0
#define fPushNext			FlagMode.b1
#define fPushStorage		FlagMode.b2
#define fInitDebuger		FlagMode.b3
#define fInitControl		FlagMode.b4
#define fEnableCharge		FlagMode.b5
#define fEnableDischarge 	FlagMode.b6
#define fNewTick			FlagMode.b7

// Angle MIN and MAX in TICK
#define STEP_MAX 20
#define ANGLE_MIN 75
#define ANGLE_MAX 20

//
#endif