/*
 * File:   LineFollowingMasterPic2.c
 * Author: joshu
 *
 * Created on March 11, 2025, 5:45 PM
 */

#include "xc.h"
#pragma config FNOSC = FRCDIV
#pragma config OSCIOFNC = OFF
#pragma config SOSCSRC = DIG

#define TRUE 1
#define FALSE 0
#define BOOL unsigned char

#define STOP 0b0000 // potentially put into header file with global var
#define STRAIGHT 0b0001 
#define SMALL_RIGHT 0b0010
#define MED_RIGHT 0b0011
#define BIG_RIGHT 0b0100
#define SMALL_LEFT 0b0101
#define MED_LEFT 0b0110
#define BIG_LEFT 0b0111

#define STRAIGHT_SPEED 2500
#define BIG_TURN_SPEED (STRAIGHT_SPEED * 2)
#define MED_TURN_SPEED (STRAIGHT_SPEED * 3)
#define SMALL_TURN_SPEED (STRAIGHT_SPEED * 4)


//this code has not incorporated seperate pwms for the left and right motor
#define PERIOD_1 OC3RS
#define DUTY_CYCLE_1 OC3R

/* Define Pins */
#define FRONT_DIR     _LATA0
#define BACK_DIR      _LATA1
#define LEFT_DIR      _LATB9
#define RIGHT_DIR     _LATB8


#define WORDBIT1 PORTBbits.RB15
#define WORDBIT2 PORTAbits.RB14 
#define WORDBIT3 PORTAbits.RB13
#define WORDBIT4 PORTAbits.RB12

int lineState = STRAIGHT;


void _ISR _CNInterrupt(void)
{
    _CNIF = 0; // Clear interrupt flag (IFS1 register)
// Figure out which pin changed if you?re looking at multiple pins
// Do something here
    if (WORDBIT4)
    {
        // if the bit4 goes high do canyon
    }
    else if (WORDBIT4 && WORDBIT3)
    {
        // if bit4 and bit3 are high we need to check for ball pick up, ball drop, go lander
    }
    else
    {
        // if bit4 goes low default to line state
    }
}



int main(void) {
    setupPins();
    config_PWM();
    config_CN_interrupt(); 
            
    while (TRUE) {
        lineState = poll_slave_line_state();
        lineFSM();
    }
    
    return 0;
}

void lineFSM() {
    static int oldState = -1;
    
    if (oldState == lineState) {
        return;
    }
    
    switch(lineState) {
        case STOP:
            setSpeed1(0);
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
    }
            
    
    oldState = lineState;
}

int poll_slave_line_state()
{
    unsigned int line_state_word = 0;
    //the weird syntax below forces the int to change at the 0, 1, or 2 bit spot in the digital word
    line_state_word |= (WORDBIT1 << 0);  // LSB
    line_state_word |= (WORDBIT2 << 1);
    line_state_word |= (WORDBIT3 << 2);
    
    //we don't have to check bit 4 because it will always be zero in the line following state
    //line_state_word |= (WORD1BIT4 << 3); //MSB 
    return line_state_word;
}

void setSpeed1(int speed) {
    PERIOD_1 = speed;
    DUTY_CYCLE_1 = PERIOD_1 / 2;
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

void config_CN_interrupt() 
{
    _CN14IE = 1; // Enable CN on pin 15 (CNEN1 register)
    _CN5PUE = 0; // Disable pull-up resistor (CNPU1 register)
    _CNIP = 6; // Set CN interrupt priority (IPC4 register)
    _CNIF = 0; // Clear interrupt flag (IFS1 register)
    _CNIE = 1; // Enable CN interrupts (IEC1 register)
}