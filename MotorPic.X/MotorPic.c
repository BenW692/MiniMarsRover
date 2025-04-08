/*
 * File:   MasterFSMV1.c
 * Author: benwo
 *
 * Created on March 14, 2025, 3:43 PM
 */


#include "xc.h"
#include "MotorInterrupts.h"
#include "MotorFunctions.h"
#include "MotorConfigs.h"

int main(void) {
    setupPins();
    config_OC_interrupt();
    config_PWM();
    
    bitWord = STOP;
    fourBit_FSM();
    
    while(1)
    {
        bitWord = poll_bitWord();
        fourBit_FSM();
    }
    
    return 0;
}



