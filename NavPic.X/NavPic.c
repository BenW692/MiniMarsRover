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
    config_Timer();
    

//    bitWord = DRIVE_NORTH;
//    while (TRUE) { //defaults to canyon mode test
//        locateTurn();
//        fourBit_FSM();
//    }
    
//    while (TRUE) //defaults to line following test
//    {
//        bitWord = senseLine();
//        fourBit_FSM();
//    }
    
    
    while(TRUE) {
        
        if (isCanyonSensed())
        {
            locateTurn();
        }
        else if (isDropSensed())
        {
            dropBall();
        }
        else    
        {
            senseLine();
        }
        
        fourBit_FSM();
}
}
    
