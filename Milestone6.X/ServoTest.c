/*
 * File:   ServoTest.c
 * Author: benwo
 *
 * Created on March 25, 2025, 2:26 PM
 */


#include "xc.h"
#pragma config FNOSC = FRCDIV

#define SERVO_PERIOD OC1RS
#define SERVO_ANGLE OC1R



int main(void) {
    
    _RCDIV = 0;
    OC1CON1 = 0;
    OC1CON2 = 0;
    OC1CON1bits.OCTSEL = 0b111; //output compare timer is system clock
    OC1CON1bits.OCM = 0b110; //edge aligned output
    OC1CON2bits.SYNCSEL = 0x1F;
    OC1CON2bits.OCTRIG = 0; //use this OC module
   
    SERVO_PERIOD = 79999;
    SERVO_ANGLE = 2000;
    
    while ((SERVO_ANGLE < 9999))
    {
        while (SERVO_ANGLE < 9999)
        {
            SERVO_ANGLE ++;
//            for (int i =0; i < 100; i++);
        }
        while (SERVO_ANGLE > 2000)
        {
            SERVO_ANGLE --;
//            for (int i =0; i < 100; i++);
        }
    }
    return 0;
    
}
