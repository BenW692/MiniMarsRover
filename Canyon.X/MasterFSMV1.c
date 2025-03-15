/*
 * File:   MasterFSMV1.c
 * Author: benwo
 *
 * Created on March 14, 2025, 3:43 PM
 */


#include "xc.h"

//#define NORTH 0b1000 
//#define WEST 0b1001 
//#define EAST 0b1010
//#define SOUTH 0b1011

#define NORTH 0b00 
#define WEST 0b01 
#define EAST 0b10
#define SOUTH 0b11

//this code has not incorporated seperate pwms for the left and right motor
#define LR_PERIOD OC3RS
#define LR_DUTY_CYCLE OC3R
#define FB_PERIOD OC1RS
#define FB_DUTY_CYCLE OC1R

/* Define Pins */
#define FRONT_DIR     _LATA0
#define BACK_DIR      _LATA1
#define LEFT_DIR      _LATB9
#define RIGHT_DIR     _LATB8

#define WORDBIT1 PORTBbits.RB15
#define WORDBIT2 PORTAbits.RB14 //saying this pin doesn't exist
#define WORDBIT3 PORTAbits.RB13 
#define WORDBIT4 PORTAbits.RB12

#define STRAFE_SPEED 2500

static int current_speed = 0;
static int target_speed = 0;

int canyonState = -1;

void __attribute__((interrupt, no_auto_psv))_OC3Interrupt(void)
{
    _OC3IF = 0;
    
    if (current_speed > target_speed)
    {
        current_speed;
    }
}



int main(void) {
    int canyonState = -1;
    
    while(1)
    {
        canyonState = poll_slave_canyon_state();
        canyon_fsm();
    
    }
    
    return 0;
}

void canyon_fsm()
{
    static int oldState = -1;
    
    if (oldState == canyonState) {
        return;
    }
    
    current_speed = 0;
    
    switch (canyonState)
    {
        case NORTH:
            //setStrafeSpeed(STRAIGHT_SPEED);
            RIGHT_DIR = 1;
            LEFT_DIR = 1;
            LR_PERIOD = STRAFE_SPEED;
            LR_DUTY_CYCLE = STRAFE_SPEED / 2;
            FB_PERIOD = 0;
            FB_DUTY_CYCLE = 0;
            break;
        case SOUTH:
            //setStrafeSpeed(STRAIGHT_SPEED);
            RIGHT_DIR = 0;
            LEFT_DIR = 0;
            LR_PERIOD = STRAFE_SPEED;
            LR_DUTY_CYCLE = STRAFE_SPEED / 2;
            FB_PERIOD = 0;
            FB_DUTY_CYCLE = 0;
            break;
        case WEST:
            //setStrafeSpeed(STRAIGHT_SPEED);
            FRONT_DIR = 1;
            BACK_DIR = 1;
            FB_PERIOD = STRAFE_SPEED;
            FB_DUTY_CYCLE = STRAFE_SPEED / 2;
            LR_PERIOD = 0;
            LR_DUTY_CYCLE = 0; 
            break;
        case EAST:
            //setStrafeSpeed(STRAIGHT_SPEED);
            FRONT_DIR = 0;
            BACK_DIR = 0;
            FB_PERIOD = STRAFE_SPEED;
            FB_DUTY_CYCLE = STRAFE_SPEED / 2;
            LR_PERIOD = 0;
            LR_DUTY_CYCLE = 0; 
            break;  
    }
    oldState = canyonState;
}


int poll_slave_canyon_state()
{
    unsigned int canyon_state_word = 0;
    //the weird syntax below forces the int to change at the 0, 1, or 2 bit spot in the digital word
    canyon_state_word |= (WORDBIT1 << 0);  // LSB
    canyon_state_word |= (WORDBIT2 << 1);
    
    //we don't have to check last two bits because they don't affect canyon state
    //line_state_word |= (WORDBIT3 << 2);
    //line_state_word |= (WORDBIT4 << 3); //MSB 
    
    return canyon_state_word;
}

//void setStrafeSpeed(int speed) {
//    PERIOD_1 = speed;
//    DUTY_CYCLE_1 = PERIOD_1 / 2;
//}

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
    /* Left Right Stepper PWM pin 5 */
    OC3CON1 = 0;
    OC3CON2 = 0;
    OC3CON1bits.OCTSEL = 0b111; //output compare timer is system clock
    OC3CON1bits.OCM = 0b110; //edge aligned output
    OC3CON2bits.SYNCSEL = 0x1F;
    OC3CON2bits.OCTRIG = 0; //use this OC module
}

void config_OC_interrupt()
{
    _OC3IP = 4; //sets priority
    _OC3IE = 1; //enables OC3 interrupt
    _OC3IF = 0; //clears interrupt flag
}