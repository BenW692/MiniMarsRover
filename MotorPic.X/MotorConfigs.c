/*
 * File:   MasterConfigs.c
 * Author: benwo
 *
 * Created on March 25, 2025, 11:41 AM
 */


#include "xc.h"
#include "MotorConfigs.h"

void setupPins() {
    /* Clear registers */
    TRISA = 0;
    TRISB = 0;
    ANSA = 0;
    ANSB = 0;
    LATA = 0;
    LATB = 0;
    
    /* Enable input pins */
    _TRISB15 = 1; //wordbit1
    _TRISB14 = 1; //wordbit2
    _TRISB13 = 1; //wordbit3
    _TRISB12 = 1; //wordbit4
}

void config_PWM() {
    
    /* FB Stepper PWM pin 4 */
    OC1CON1 = 0;
    OC1CON2 = 0;
    OC1CON1bits.OCTSEL = 0b111; //output compare timer is system clock
    OC1CON1bits.OCM = 0b110; //edge aligned output
    OC1CON2bits.SYNCSEL = 0x1F;
    OC1CON2bits.OCTRIG = 0; //use this OC module
    
    /* Left Stepper PWM pin 4 */
    OC2CON1 = 0;
    OC2CON2 = 0;
    OC2CON1bits.OCTSEL = 0b111; //output compare timer is system clock
    OC2CON1bits.OCM = 0b110; //edge aligned output
    OC2CON2bits.SYNCSEL = 0x1F;
    OC2CON2bits.OCTRIG = 0; //use this OC module
    
    /* Right Stepper PWM pin 5 */
    OC3CON1 = 0;
    OC3CON2 = 0;
    OC3CON1bits.OCTSEL = 0b111; //output compare timer is system clock
    OC3CON1bits.OCM = 0b110; //edge aligned output
    OC3CON2bits.SYNCSEL = 0x1F;
    OC3CON2bits.OCTRIG = 0; //use this OC module
    
    /* Clear period and duty cycles*/
    FB_PERIOD = 0;
    FB_DUTY_CYCLE = 0;
    R_PERIOD = 0;
    R_DUTY_CYCLE = 0;
    L_PERIOD = 0;
    L_DUTY_CYCLE = 0;
}

void config_OC_interrupt()
{
    _OC3IP = 4; //sets priority
    _OC3IF = 0; //clears interrupt flag
    
    _OC2IP = 3; //sets priority
    _OC2IF = 0; //clears interrupt flag
    
    _OC1IP = 5; //sets priority
    _OC1IF = 0; //clears interrupt flag
}
