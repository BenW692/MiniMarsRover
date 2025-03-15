/*
 * File:   MasterFSMV1.c
 * Author: benwo
 *
 * Created on March 14, 2025, 3:43 PM
 */


#include "xc.h"

#define NORTH 0b1000 
#define WEST 0b1001 
#define EAST 0b1010
#define SOUTH 0b1011

//#define NORTH 0b00 
//#define WEST 0b01 
//#define EAST 0b10
//#define SOUTH 0b11

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


static int target_speed_LR = 0;
static int target_speed_FB = 0;


int canyonState = -1;

void __attribute__((interrupt, no_auto_psv))_OC3Interrupt(void)
{
    _OC3IF = 0;
    
    if (LR_PERIOD > target_speed_LR) //if period is bigger it needs to get smaller to go faster
    {
        LR_PERIOD--;
        LR_DUTY_CYCLE = LR_PERIOD / 2;
    }
}

void __attribute__((interrupt, no_auto_psv))_OC2Interrupt(void)
{
    _OC2IF = 0;
    
    if (FB_PERIOD > target_speed_FB) //if period is bigger it needs to get smaller to go faster
    {
        FB_PERIOD--;
        FB_DUTY_CYCLE = FB_PERIOD / 2;
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
    
    if (oldState == canyonState) 
    {
        return;
    }
    
    switch (canyonState)
    {
    case NORTH:
        setStrafeSpeed(1, 1, -1, -1, STRAFE_SPEED); // Left & Right motors forward
        break;
    case SOUTH:
        setStrafeSpeed(0, 0, -1, -1, STRAFE_SPEED); // Left & Right motors backward
        break;
    case WEST:
        setStrafeSpeed(-1, -1, 1, 1, STRAFE_SPEED); // Front & Back motors forward
        break;
    case EAST:
        setStrafeSpeed(-1, -1, 0, 0, STRAFE_SPEED); // Front & Back motors backward
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
    canyon_state_word |= (WORDBIT3 << 2);
    canyon_state_word |= (WORDBIT4 << 3); //MSB 
    
    return canyon_state_word;
}

void setStrafeSpeed(int left, int right, int front, int back, int speed)
// 1 is forward, 0 is backward, -1 turns off PWM to those motors and lock those wheels
{
    LEFT_DIR = left;
    RIGHT_DIR = right;
    FRONT_DIR = front;
    BACK_DIR = back;

    if (front != -1 || back != -1) 
    { // Control Front-Back Motors
        target_speed_FB = speed;
        FB_PERIOD = 5000;
        FB_DUTY_CYCLE = 5000;
    } 
    else 
    {
        FB_PERIOD = 0;
        FB_DUTY_CYCLE = 0;
    }

    if (left != -1 || right != -1) 
    { // Control Left-Right Motors
        target_speed_LR = speed;
        LR_PERIOD = 5000;
        LR_DUTY_CYCLE = 5000;
    } 
    else 
    {
        LR_PERIOD = 0;
        LR_DUTY_CYCLE = 0;
    }
}

    

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