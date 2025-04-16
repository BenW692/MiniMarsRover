#ifndef MASTERGLOBALS_H
#define MASTERGLOBALS_H

#include "xc.h"

#define TRUE 1
#define FALSE 0
#define BOOL int

// turning
#define NO_LINE 0b0000
#define STRAIGHT 0b0001 
#define TURN_RIGHT 0b0010
#define TURN_LEFT 0b0011

// strafing 
#define DRIVE_NORTH 0b0100
#define DRIVE_EAST 0b0101
#define DRIVE_SOUTH 0b0110
#define DRIVE_WEST 0b0111

// rotating
#define ROTATE_CW 0b1000
#define ROTATE_CCW 0b1001

// other
#define ACCEL_STRAIGHT 0b1010
#define SLOW_MOTORS 0b1011
#define QUICKEN_MOTORS 0b1100
#define DECEL_STRAIGHT 0b1101
#define STOP 0b1111

#define ACCEL_VAL 2 // was 5 which was a little too fast
#define SPEED_MULTIPLIER 2

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

// OLD USED TO BE GLOBAL DEFINE. ARE NOW GLOBAL VARS
//#define strafe_speed 1000 //1000 is too fast b/c. the struggles to change directions before hitting the wall
//#define slip_speed 3000    
//
//#define straight_speed 1000 //2500 * .4 when driving straight
//#define non_turn_speed 2400 //6000 * .4 wheel that keeps going straight
//#define fb_turn_speed 1600 //4000 * .4 front back speed
//#define med_turn_speed 600 //1500 * .4 wheel that speeds up to turn

extern int strafe_speed;
extern int slip_speed;

extern int straight_speed;
extern int non_turn_speed;
extern int fb_turn_speed;
extern int med_turn_speed;

extern int target_speed_R;
extern int target_speed_L;
extern int target_speed_FB;

extern int oldWord;
extern int bitWord;

extern BOOL isDecelerate;

#endif