/*********************************************************************
 * FileName:        cpucfg.h
 ********************************************************************/
#ifndef CPUCFG_H
#define CPUCFG_H

/** O S C ***********************************************************/
// OSC = 20 MHz
#define OSCILLATOR	20000000
#define INSTRUCTION_CYCLE (OSCILLATOR)/4

/** T I M E R 0 *****************************************************/
// Timer0 (8 bits mode, T0CKI, not prescale), T0SE = 0 -> _/-
#define TIMER0_PRESCALE 1
#define TMR0_VALUE 0xFF
#define mInitTimer0() T0CON = 0b01101111; INTCONbits.TMR0IF = 0; INTCONbits.TMR0IE = 1
#define mLoadTimer0() TMR0L = TMR0_VALUE; INTCONbits.TMR0IF = 0
#define mStartTimer0() T0CONbits.TMR0ON = 1
#define mStopTimer0() T0CONbits.TMR0ON = 0; INTCONbits.TMR0IF = 0
#define mInterruptTimer0 INTCONbits.TMR0IF
// Used for Sync
#define mInitSync() mLoadTimer0()
#define mStartSync() mStartTimer0()
#define mSync T0CONbits.T0SE == 0
#define mToggleSync() T0CONbits.T0SE ^= 1
#define mInterruptSync mInterruptTimer0

/** T I M E R  1 ****************************************************/
// Timer1 = 10 kHz (100mks) (16 bits mode,Fosc/4, Not prescale)
#define TIMER1_PRESCALE 1
#define TIMER1_FREQUENCY 10000
#define TIMER1_CORRECTION 21
#define TMR1_VALUE 65535-(INSTRUCTION_CYCLE/(TIMER1_FREQUENCY*TIMER1_PRESCALE))+TIMER1_CORRECTION
#define mInitTimer1()T1CON = 0b10000000; IPR1bits.TMR1IP = 1; PIR1bits.TMR1IF = 0; PIE1bits.TMR1IE = 1
#define mLoadTimer1() TMR1H = TMR1_VALUE>>8; TMR1L = TMR1_VALUE&0xFF; PIR1bits.TMR1IF = 0; Nop()
#define mStartTimer1() T1CONbits.TMR1ON = 1
#define mStopTimer1() T1CONbits.TMR1ON = 0; PIR1bits.TMR1IF = 0
#define mInterruptTimer1 PIR1bits.TMR1IF
// Used for Impulse
#define mInitImpulse() mLoadTimer1()
#define mStartImpulse() mStartTimer1()
#define mStopImpulse() mStopTimer1()
#define mInterruptImpulse mInterruptTimer1

/** T I M E R 2 *****************************************************/
// PWM ~ 5 kHz used Timer2, discharge battery.
#define PR2_VALUE 230
// PWM = 99% => Vbat = 12.5 V, R = 10 Om, Imax = 12,5/10 = 1250mA
// Adjust (2...80) + PWM_MIN => 15...95 -> discharge 100...500mA
#define PR2_VALUE 230
#define PWM_MIN 13
// Intialization 5kHz PWM 7% 
#define mInitPWM() {PR2 = PR2_VALUE;		\
					CCPR1L = PWM_MIN;		\
					LATCbits.LATC2 = 0;		\
					TRISCbits.TRISC2 = 0;	\
					T2CON = 0b01111001;		\
					CCP1CON = 0b00000000;	\
					PIR1bits.TMR2IF = 0;	\
					IPR1bits.TMR2IP = 0;    \
					PIE1bits.TMR2IE = 1;}
#define mLoadDutyPWM(a) (CCPR1L = a)
#define mStartPWM() T2CONbits.TMR2ON = true; CCP1CON = 0b00001111
#define mStopPWM() T2CONbits.TMR2ON = false; CCP1CON = 0b00000000; LATCbits.LATC2 = 0; TMR2 = 0
#define mInterruptPWM PIR1bits.TMR2IF

/** T I M E R 3 *****************************************************/
// Timer3 = 3ms (16 bits mode, Fosc/4, Not prescale) low interrupt
#define TIME_TICK 3
#define TIMER3_PRESCALE 1
#define TIMER3_FREQUENCY 333
#define TIMER3_CORRECTION 81
#define TMR3_VALUE 65535-(INSTRUCTION_CYCLE/(TIMER3_FREQUENCY*TIMER3_PRESCALE))+TIMER3_CORRECTION
#define mInitTimer3() T3CON = 0b10000000; IPR2bits.TMR3IP = 0; PIR2bits.TMR3IF = 0; PIE2bits.TMR3IE = 1
#define mLoadTimer3() TMR3H = TMR3_VALUE>>8; TMR3L = TMR3_VALUE&0xFF; PIR2bits.TMR3IF = 0
#define mStartTimer3() T3CONbits.TMR3ON = 1
#define mStopTimer3() T3CONbits.TMR3ON = 0; PIR2bits.TMR3IF = 0
#define mInterruptTimer3 PIR2bits.TMR3IF
#define mDisableTimer3() PIE2bits.TMR3IE = 0
#define mEnableTimer3() PIE2bits.TMR3IE = 1
// Used for timing
#define mInitTick() mLoadTimer3()
#define mStartTick() mStartTimer3()
#define mInterrupTick mInterruptTimer3
#define mDisableTick() mDisableTimer3()
#define mEnableTick() mEnableTimer3()

/** P O R T S ******************************************************/
#define mInitPorts() {ADCON1 = 0x07;	\
					  TRISA = 0xFF;		\
					  TRISB = 0x00;		\
					  LATB	= 0x00;		\
					  TRISC = 0x00;		\
					  LATC = 0x00;}

// Inputs
#define mButton PORTAbits.RA2
#define mDebug PORTAbits.RA5

// Dispaly
// PORTB = ANOD_A...ANOD_H; PORTC = CATOD1...CATOD6;
#define mAnodeLED LATB
#define mCatodeLED LATC
#define mDisplayOff() mCatodeLED &= 0b00000110

// Outputs
#define mTriac LATCbits.LATC1
#define TRIAC_OFF 0
#define TRIAC_ON 1

/** A D C **********************************************************/
// ADCON0 |ADCS1|ADCS0|CHS2|CHC1|CHS0|GO  |--- |ADON|
// ADCON1 |ADMF |ADCS2|--- |--- |PCF3|PCF2|PCF1|PCF0|
// Setting for ADCON0:
// ADSC1 = 1, ADSC0 = 0, AN0,GO = 0, X, ADON = 1
#define ADC_BAT 0b10000001
// ADSC1 = 1, ADSC0 = 0, AN1,GO = 0, X, ADON = 1
#define ADC_CUR 0b10001001
// ADSC1 = 1, ADSC0 = 0, AN3,GO = 0, X, ADON = 1
#define ADC_ADJ 0b10011001
// Setting for ADCON1:
// ADMF = 1, ADCS2 = 0, PCF3...PCF0 = 0100 => AN3|D |AN1|AN0
#define mInitADC() ADCON1 = 0b10000100

//
#endif //CPUCFG_H
