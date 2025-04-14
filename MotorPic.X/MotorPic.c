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

#pragma config FWDTEN=OFF //watchdog
#pragma config WINDIS=OFF //watchdog

int main(void) {
    setupPins();
    config_ACCEL_interrupt();
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



