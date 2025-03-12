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

#define STOP 0b000 // potentially put into header file with global var
#define STRAIGHT 0b001 
#define SMALL_RIGHT 0b010
#define MED_RIGHT 0b011
#define BIG_RIGHT 0b100
#define SMALL_LEFT 0b101
#define MED_LEFT 0b110
#define BIG_LEFT 0b111

#define STRAIGHT_SPEED 1500
#define BIG_TURN_SPEED (STRAIGHT_SPEED * 2)
#define MED_TURN_SPEED (STRAIGHT_SPEED * 3)
#define SMALL_TURN_SPEED (STRAIGHT_SPEED * 4)

#define PERIOD_1 OC3RS
#define DUTY_CYCLE_1 OC3R

/* Define Pins */
#define FRONT_DIR     _LATA0
#define BACK_DIR      _LATA1
#define LEFT_DIR      _LATB9
#define RIGHT_DIR     _LATB8

#define WORD1BIT1 PORTAbits.RA2 
#define WORD1BIT2 PORTAbits.RA3 
#define WORD1BIT3 PORTBbits.RB4

int lineState = STRAIGHT;

int main(void) {
    setupPins();
    config_PWM();
    
    
//    _LATB14 = 1;
//    setSpeed1(STRAIGHT_SPEED);
//    while (TRUE);
    
    while (TRUE) {
        lineState = poll_slave_line_state();
        lineFSM();
    }
    
    return 0;
}

void lineFSM() {
    static int oldState = STOP;
    
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
            LEFT_DIR = 0;
            break;
            
        case SMALL_RIGHT:
            setSpeed1(SMALL_TURN_SPEED);
            RIGHT_DIR = 0;
            LEFT_DIR = 0;
            break;
            
        case MED_RIGHT:
            setSpeed1(MED_TURN_SPEED);
            RIGHT_DIR = 0;
            LEFT_DIR = 0;
            break;
            
        case BIG_RIGHT:
            setSpeed1(BIG_TURN_SPEED);
            RIGHT_DIR = 0;
            LEFT_DIR = 0;
            break;
            
        case SMALL_LEFT:
            setSpeed1(SMALL_TURN_SPEED);
            RIGHT_DIR = 1;
            LEFT_DIR = 1;
            break;
            
        case MED_LEFT:
            setSpeed1(MED_TURN_SPEED);
            RIGHT_DIR = 1;
            LEFT_DIR = 1;
            break;
            
        case BIG_LEFT:
            setSpeed1(BIG_TURN_SPEED);
            RIGHT_DIR = 1;
            LEFT_DIR = 1;
            break;
    }
    
    oldState = lineState;
}

int poll_slave_line_state()
{
    unsigned int line_state_word = 0;
    //the weird syntax below forces the int to change at the 0, 1, or 2 bit spot in the digital word
    line_state_word |= (WORD1BIT1 << 0);  // LSB
    line_state_word |= (WORD1BIT2 << 1);
    line_state_word |= (WORD1BIT3 << 2); //MSB
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
    _TRISA2 = 1;
    _TRISA3 = 1;
    _TRISB4 = 1;
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
