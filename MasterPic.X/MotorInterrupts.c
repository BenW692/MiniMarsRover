/*
 * File:   MasterInterrupts.c
 * Author: benwo
 *
 * Created on March 25, 2025, 11:30 AM
 */


#include "xc.h"

#include "MotorInterrupts.h"

void __attribute__((interrupt, no_auto_psv))_OC3Interrupt(void)
{
    _OC3IF = 0;
    
    if (R_PERIOD > target_speed_R) //if period is bigger it needs to get smaller to go faster
    {
        R_PERIOD -= 5;
        R_DUTY_CYCLE = R_PERIOD / 2;
    }
}

void __attribute__((interrupt, no_auto_psv))_OC1Interrupt(void)
{
    _OC1IF = 0;
    
    if (FB_PERIOD > target_speed_FB) //if period is bigger it needs to get smaller to go faster
    {
        FB_PERIOD -= 5;
        FB_DUTY_CYCLE = FB_PERIOD / 2;
    }
}

void __attribute__((interrupt, no_auto_psv))_OC2Interrupt(void)
{
    _OC2IF = 0;
    
    if (L_PERIOD > target_speed_L) //if period is bigger it needs to get smaller to go faster
    {
        L_PERIOD -= 5;
        L_DUTY_CYCLE = L_PERIOD / 2;
    }
}
