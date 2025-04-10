/*
 * File:   NavInterrupts.c
 * Author: benwo
 *
 * Created on March 25, 2025, 7:45 AM
 */


#include "xc.h"
#include "NavInterrupts.h"

void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void)
{
    
	_T1IF = 0;	// Clear interrupt flag
    
    drop_complete = TRUE;
    
    past_drop ^= 1;
    
	// Reset the timer
    TMR1 = 0;
}

void __attribute__((interrupt, no_auto_psv)) _T3Interrupt(void) {
    _T3IF = 0; // Clear flag
    T3CONbits.TON = 0; //bit is off
    _T3IE = 0; // Disable Interupt
    TMR3 = 0; // Clear Timer
    stateTimer3 = FALSE;
}