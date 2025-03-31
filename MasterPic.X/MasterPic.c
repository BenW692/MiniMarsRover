/*
 * File:   MasterFSMV1.c
 * Author: benwo
 *
 * Created on March 14, 2025, 3:43 PM
 */


#include "xc.h"
#include "MasterInterrupts.h"
#include "MasterFunctions.h"
#include "MasterConfigs.h"

int main(void) {
    setupPins();
    config_OC_interrupt();
    config_PWM();
    
    bitWord = STRAIGHT;
    disable_OC_interrupt();
    
    while(1)
    {
        bitWord = poll_bitWord();
        fourBit_FSM();
    }
    
    return 0;
}



