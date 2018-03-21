/*********************************************************************
 * FileName:        display.h
 ********************************************************************/
#ifndef INPUT_H
#define INPUT_H

/** P U B L I C  P R O T O T Y P E S ********************************/
void InitVarInput(void);
byte ReadDebug(void);
byte ReadButton(void);

/** D E F I N E *****************************************************/
#define DEBUG 0
#define PUSH 0
//
#define NEXT 1
#define STORAGE 2

// 3 ms x 60 = 180 ms
#define BOUNCE 180/TIME_TICK
// 3 ms x 1000 = 3000 ms
#define PRESS 3000/TIME_TICK

//
#define fDebugInput		FlagInput.b0
#define fDebugDelay		FlagInput.b1
#define fDebugOn		FlagInput.b2
//
#define fButtonInput	FlagInput.b4
#define fButtonDelay	FlagInput.b5
#define fButtonOn		FlagInput.b6
#define fButtonPress	FlagInput.b7

//
#endif //INPUT_H