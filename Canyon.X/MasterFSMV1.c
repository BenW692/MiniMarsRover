/*
 * File:   MasterFSMV1.c
 * Author: benwo
 *
 * Created on March 14, 2025, 3:43 PM
 */


#include "xc.h"

#define NO_LINE 0b0000 //potentially put into header file with global var
#define STRAIGHT 0b0001 
#define SMALL_RIGHT 0b0010
#define MED_RIGHT 0b0011
#define BIG_RIGHT 0b0100
#define SMALL_LEFT 0b0101
#define MED_LEFT 0b0110
#define BIG_LEFT 0b0111

#define DRIVE_NORTH 0b1000
#define DRIVE_EAST 0b1001
#define DRIVE_SOUTH 0b1010
#define DRIVE_WEST 0b1011

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
#define WORDBIT2 PORTBbits.RB14 //saying this pin doesn't exist
#define WORDBIT3 PORTBbits.RB13 
#define WORDBIT4 PORTBbits.RB12

#define STRAFE_SPEED 2500

#define STRAIGHT_SPEED 2500
#define BIG_TURN_SPEED (STRAIGHT_SPEED * 2)
#define MED_TURN_SPEED (STRAIGHT_SPEED * 3)
#define SMALL_TURN_SPEED (STRAIGHT_SPEED * 4)

static int target_speed_LR = 0;
static int target_speed_FB = 0;

static int oldWord = -1;
int bitWord = -1;



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
    setupPins();
    config_OC_interrupt();
    config_PWM();
    
    target_speed_FB = 1000;
    FB_PERIOD = 2000;
    FB_DUTY_CYCLE = FB_PERIOD / 2;
    target_speed_LR = 2500;
    LR_PERIOD = 5000;
    LR_DUTY_CYCLE = LR_PERIOD / 2;
    while(1);
    
    while(1)
    {
        bitWord = poll_bitWord();
        fourBit_FSM();
    }
    
    return 0;
}

void fourBit_FSM()
{
    
    if (oldWord == bitWord) 
    {
        return;
    }
    
    switch (bitWord)
    {
    case NO_LINE:
        setSpeed1(STRAIGHT_SPEED);
        RIGHT_DIR = 1;
        LEFT_DIR = 1;
        break;

    case STRAIGHT:
        setSpeed1(STRAIGHT_SPEED);
        RIGHT_DIR = 1;
        LEFT_DIR = 1;
        break;

    case SMALL_RIGHT:
        setSpeed1(SMALL_TURN_SPEED);
        RIGHT_DIR = 1;
        LEFT_DIR = 0;
        break;

    case MED_RIGHT:
        setSpeed1(MED_TURN_SPEED);
        RIGHT_DIR = 1;
        LEFT_DIR = 0;
        break;

    case BIG_RIGHT:
        setSpeed1(BIG_TURN_SPEED);
        RIGHT_DIR = 1;
        LEFT_DIR = 0;
        break;

    case SMALL_LEFT:
        setSpeed1(SMALL_TURN_SPEED);
        RIGHT_DIR = 0;
        LEFT_DIR = 1;
        break;

    case MED_LEFT:
        setSpeed1(MED_TURN_SPEED);
        RIGHT_DIR = 0;
        LEFT_DIR = 1;
        break;

    case BIG_LEFT:
        setSpeed1(BIG_TURN_SPEED);
        RIGHT_DIR = 0;
        LEFT_DIR = 1;
        break;    
        
    case DRIVE_NORTH:
        setStrafeSpeed(1, 1, -1, -1, STRAFE_SPEED); // Left & Right motors forward
        break;
        
    case DRIVE_SOUTH:
        setStrafeSpeed(0, 0, -1, -1, STRAFE_SPEED); // Left & Right motors backward
        break;
        
    case DRIVE_WEST:
        setStrafeSpeed(-1, -1, 1, 1, STRAFE_SPEED); // Front & Back motors forward
        break;
        
    case DRIVE_EAST:
        setStrafeSpeed(-1, -1, 0, 0, STRAFE_SPEED); // Front & Back motors backward
        break;
    }
    oldWord = bitWord;
}


int poll_bitWord()
{
    unsigned int word = 0;
    //the weird syntax below forces the int to change at the 0, 1, or 2 bit spot in the digital word
    word |= (WORDBIT1 << 0);  // LSB
    word |= (WORDBIT2 << 1);
    word |= (WORDBIT3 << 2);
    word |= (WORDBIT4 << 3); //MSB 
    
    return word;
}

void setStrafeSpeed(int left, int right, int front, int back, int speed)
// 1 is forward, 0 is backward, -1 turns off PWM to those motors and lock those wheels
{
    LEFT_DIR = (left == -1) ? 0 : left;
    RIGHT_DIR = (right == -1) ? 0 : right;
    FRONT_DIR = (front == -1) ? 0 : front;
    BACK_DIR = (back == -1) ? 0 : back;

    if (front != -1 || back != -1) 
    { // Control Front-Back Motors
        target_speed_FB = speed;
        FB_PERIOD = speed * 2;
        FB_DUTY_CYCLE = FB_PERIOD / 2;
    } 
    else 
    {
        FB_PERIOD = 0;
        FB_DUTY_CYCLE = 0;
    }

    if (left != -1 || right != -1) 
    { // Control Left-Right Motors
        target_speed_LR = speed;
        LR_PERIOD = speed * 2;
        LR_DUTY_CYCLE = LR_PERIOD / 2;
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
    /* Front Back Stepper PWM pin 4 */
    OC2CON1 = 0;
    OC2CON2 = 0;
    OC2CON1bits.OCTSEL = 0b111; //output compare timer is system clock
    OC2CON1bits.OCM = 0b110; //edge aligned output
    OC2CON2bits.SYNCSEL = 0x1F;
    OC2CON2bits.OCTRIG = 0; //use this OC module
    
    /* Left Right Stepper PWM pin 5 */
    OC3CON1 = 0;
    OC3CON2 = 0;
    OC3CON1bits.OCTSEL = 0b111; //output compare timer is system clock
    OC3CON1bits.OCM = 0b110; //edge aligned output
    OC3CON2bits.SYNCSEL = 0x1F;
    OC3CON2bits.OCTRIG = 0; //use this OC module
    
    /* Clear period and duty cycles*/
    FB_PERIOD = 0;
    FB_DUTY_CYCLE = 0;
    LR_PERIOD = 0;
    LR_DUTY_CYCLE = 0;
}

void config_OC_interrupt()
{
    _OC3IP = 4; //sets priority
    _OC3IE = 1; //enables OC3 interrupt
    _OC3IF = 0; //clears interrupt flag
}

void setSpeed1(int speed) {
    LR_PERIOD = speed;
    LR_DUTY_CYCLE = LR_PERIOD / 2;
}