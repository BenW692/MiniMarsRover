/*
 * File:   Navigation.c
 * Author: jss265
 *
 * Created on March 15, 2025, 3:09 PM
 */


#include "xc.h"
#include "NavConfigs.h"
#include "NavFunctions.h"
#include "NavInterrupts.h"

#pragma config FNOSC = FRCDIV // 8 MHz osccilator 
#pragma config OSCIOFNC = OFF
#pragma config SOSCSRC = DIG


int main(void) {
    
    _RCDIV = 0;
    pinSetup();
    config_ADC();
    config_Timers();
    config_PWM();
    
    bitWord = STOP; // should be stop to enable acceleration
    fourBit_FSM();

    delay(1000); //we need this delay so the ADC can configure
    //at boot up it returns a random value for the sonar sensors that will trigger ball
    //drop if we don't have this delay
    
    while (TRUE) // Final FSM
    {
        DUMPYs_Favorite_Game();
    }
    
    while (TRUE) { // task spot checker
        senseLine();
        fourBit_FSM();
        pollTower();
        pollDrop();
//        pollLander(); // shifty... has issues
//        poll_GPS();
    }
}
    
