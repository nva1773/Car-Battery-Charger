/*********************************************************************
 * FileName:        control.h
 ********************************************************************/
#ifndef CONTROL_H
#define CONTROL_H

/** P U B L I C  P R O T O T Y P E S ********************************/
void InitVarControl(void);
void Control(void);
void ProcessOff(void);
void ProcessRun(void);

/** D E F I N E *****************************************************/
// Voltage battery set = 7.0V => 70, 
// Delta = 2.5V, Start = 12.5V, End = 14.2V
#define BATTERY_SET 70
#define DELTA 25
#define CHARGE_START 125
#define CHARGE_END 142
//
#define MAGIC_NUM 3

// Time = XXX * 3ms
#define DELTA_TIME 1800/TIME_TICK
#define CHARGE_TIME 648/TIME_TICK
#define DISCHARGE_TIME 1296/TIME_TICK
#define ADJUST_TIME 1500/TIME_TICK

// Flag control
#define fStartProcess	FlagControl.b0
#define fFirstConnect	FlagControl.b1
#define fFastChange		FlagControl.b2
#define fUpdateControl	FlagControl.b3
#define fDisplayVoltage FlagControl.b4
#define fDisplayCurrent	FlagControl.b5
#define fDisplayAdjust	FlagControl.b6

//
#endif //CONTROL_H