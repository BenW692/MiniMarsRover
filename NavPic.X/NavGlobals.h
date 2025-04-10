
#ifndef NAVGLOBALS_H
#define NAVGLOBALS_H

#include <xc.h>

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
#define STOP 0b1111

#define QRD1 ADC1BUF0 // west
#define QRD2 ADC1BUF1 // middle
#define QRD3 ADC1BUF2 // east

#define LANDER_QRD ADC1BUF10 
#define BALL_QRD ADC1BUF9 // pin 18

#define BALL_TOWER ADC1BUF11 // pin 16
#define TOWER_DETECT 185

#define QRD_HIGH 682 // 2/3 of 1023
#define QRD_MED 341  // 1/3 of 1023

#define SERVO_PERIOD OC1RS
#define SERVO_ANGLE OC1R

#define BLACK_ANGLE 470 //???Not sure
#define MIDDLE_ANGLE 685
#define WHITE_ANGLE 920

#define SONAR_N ADC1BUF3 // pin5
#define SONAR_W ADC1BUF4 // pin6
#define SONAR_S ADC1BUF13 // pin7
#define SONAR_E ADC1BUF12 // pin15

#define SONAR_STUPID 400
#define SONAR_HIGH 300 // Sense Wall 
#define SONAR_LOW 200 // Close enough to wall to change direction
#define SONAR_DROP 100 // close enough to wall to drop the ball

//for all of these thresholds I added 30 to the conversion number
#define N_WALL_DETECT 340 //was 300
//#define N_BALL_DROP_DETECT 140 //was 600 WE DONT USE THIS
#define N_LANDER_WALL 425

#define E_WALL_DETECT 320 //was 280
#define E_BALL_DROP_DETECT 170 //was 775

#define S_WALL_DETECT 310 //was 260
//#define S_BALL_DROP_DETECT 500 //WE DONT USE THIS

#define W_WALL_DETECT 420 //was 370
#define W_BALL_DROP_DETECT 200

#define SATELLITE_DIODE ADC1BUF14
#define SATELLITE_DETECT 25 //50mv

#define LASER_OUT _LATB9


#define WORDBIT1 _LATB4
#define WORDBIT2 _LATA4 
#define WORDBIT3 _LATB8
#define WORDBIT4 _LATB7

extern int bitWord;
extern int oldState;

extern int qrd1;
extern int qrd2;
extern int qrd3;

extern int ball_color;
extern int ball_qrd;

extern BOOL drop_complete;
extern BOOL past_drop;
extern BOOL stateTimer3;

typedef enum {
    state1,
    state2,
    state3,
    state4
} State;

extern State roverState;


#endif