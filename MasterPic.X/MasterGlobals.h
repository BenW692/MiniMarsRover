#ifndef MASTERGLOBALS_H
#define MASTERGLOBALS_H

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

#define STOP 0b1111


#define L_PERIOD OC2RS
#define L_DUTY_CYCLE OC2R
#define R_PERIOD OC3RS
#define R_DUTY_CYCLE OC3R
#define FB_PERIOD OC1RS
#define FB_DUTY_CYCLE OC1R

/* Define Pins */
#define FRONT_DIR     _LATA0
#define BACK_DIR      _LATA1
#define LEFT_DIR      _LATB9
#define RIGHT_DIR     _LATB8

#define WORDBIT1 PORTBbits.RB15
#define WORDBIT2 PORTBbits.RB14
#define WORDBIT3 PORTBbits.RB13 
#define WORDBIT4 PORTBbits.RB12

#define STRAFE_SPEED 1250 //1000 is too fast b/c. the struggles to change directions before
//hitting the wall
#define SLIP_SPEED 2500

#define STRAIGHT_SPEED 1500 //2500 when driving straight
#define NON_TURN_SPEED 3600 //6000 wheel that keeps going straight
#define FB_TURN_SPEED 2400 //4000 front back speed
//#define BIG_TURN_SPEED 3000
#define MED_TURN_SPEED 900 //1500 wheel that speeds up to turn
//#define SMALL_TURN_SPEED 3750


//I made this not static and it might break stuff we will see
extern int target_speed_R;
extern int target_speed_L;
extern int target_speed_FB;

extern int oldWord;
extern int bitWord;

#endif