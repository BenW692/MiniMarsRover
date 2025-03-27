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
    
//    target_speed_FB = 1500;
//    FB_PERIOD = 3000;
//    FB_DUTY_CYCLE = FB_PERIOD / 2;
//    target_speed_LR = 2500;
//    LR_PERIOD = 5000;
//    LR_DUTY_CYCLE = LR_PERIOD / 2;
//    while(1);

//    setStrafeSpeed(1, 1, -1, -1, STRAFE_SPEED); // drive North test
//    while(1);
    
    bitWord = STRAIGHT;
    disable_OC_interrupt();
    
    while(1)
    {
        bitWord = poll_bitWord();
        fourBit_FSM();
    }
    
    return 0;
}



