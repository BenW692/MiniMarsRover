/*
 * File:   MasterInterrupts.c
 * Author: benwo
 *
 * Created on March 25, 2025, 11:30 AM
 */


#include "xc.h"

#include "MotorInterrupts.h"
/* vvv ALL OC interrupts are OLD CODE vvv */

//void __attribute__((interrupt, no_auto_psv))_OC3Interrupt(void)
//{
//    _OC3IF = 0;
//    
//    if (R_PERIOD > target_speed_R) //if period is bigger it needs to get smaller to go faster
//    {
//        R_PERIOD -= ACCEL_VAL;
//        R_DUTY_CYCLE = R_PERIOD / 2;
//    }
//}
//
//void __attribute__((interrupt, no_auto_psv))_OC1Interrupt(void)
//{
//    _OC1IF = 0;
//    
//    if (FB_PERIOD > target_speed_FB) //if period is bigger it needs to get smaller to go faster
//    {
//        FB_PERIOD -= ACCEL_VAL;
//        FB_DUTY_CYCLE = FB_PERIOD / 2;
//    }
//}
//
//void __attribute__((interrupt, no_auto_psv))_OC2Interrupt(void)
//{
//    _OC2IF = 0;
//    
//    if (L_PERIOD > target_speed_L) //if period is bigger it needs to get smaller to go faster
//    {
//        L_PERIOD -= ACCEL_VAL;
//        L_DUTY_CYCLE = L_PERIOD / 2;
//    }
//}

/* ^^^ ALL OC interrupts are OLD CODE ^^^ */

void __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void)
{
	_T1IF = 0;	// Clear interrupt flag
    static int val1 = 3;
    static int val2 = 4;

    /* DECCELERATION */
    if (isDecelerate) {
        if (R_PERIOD < target_speed_R * val1) //if period is bigger it needs to get smaller to go faster
        {
            R_PERIOD += ACCEL_VAL * val2;
            R_DUTY_CYCLE = R_PERIOD / 2;
        } else isDecelerate = FALSE;

        if (L_PERIOD > target_speed_L * val1) //if period is bigger it needs to get smaller to go faster
        {
            L_PERIOD += ACCEL_VAL * val2;
            L_DUTY_CYCLE = L_PERIOD / 2;
        }

        if (FB_PERIOD > target_speed_FB * val1) //if period is bigger it needs to get smaller to go faster
        {
            FB_PERIOD += ACCEL_VAL * val2;
            FB_DUTY_CYCLE = FB_PERIOD / 2;
        }
    }
    
    /* ACCELERATION */
    else {
        if (R_PERIOD > target_speed_R) //if period is bigger it needs to get smaller to go faster
        {
            R_PERIOD -= ACCEL_VAL;
            R_DUTY_CYCLE = R_PERIOD / 2;
        }   

        if (L_PERIOD > target_speed_L) //if period is bigger it needs to get smaller to go faster
        {
            L_PERIOD -= ACCEL_VAL;
            L_DUTY_CYCLE = L_PERIOD / 2;
        }

        if (FB_PERIOD > target_speed_FB) //if period is bigger it needs to get smaller to go faster
        {
            FB_PERIOD -= ACCEL_VAL;
            FB_DUTY_CYCLE = FB_PERIOD / 2;
        }
    }
}
