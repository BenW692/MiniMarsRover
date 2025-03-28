
#ifndef NAVGLOBALS_H
#define NAVGLOBALS_H

#include <xc.h>

#define TRUE 1
#define FALSE 0
#define BOOL int


//line following
#define NO_LINE 0b0000 //potentially put into header file with global var
#define STRAIGHT 0b0001 
#define SMALL_RIGHT 0b0010
#define MED_RIGHT 0b0011
#define BIG_RIGHT 0b0100
#define SMALL_LEFT 0b0101
#define MED_LEFT 0b0110
#define BIG_LEFT 0b0111

//canyon mode
#define DRIVE_NORTH 0b1000
#define DRIVE_EAST 0b1001
#define DRIVE_SOUTH 0b1010
#define DRIVE_WEST 0b1011

//other tasks
#define STOP 0b1111  

#define QRD1 ADC1BUF0 //west
#define QRD2 ADC1BUF1 //middle
#define QRD3 ADC1BUF2 //east

#define BALL_QRD ADC1BUF9 //pin 18

#define SERVO_PERIOD OC1RS
#define SERVO_ANGLE OC1R

#define BLACK_ANGLE 470 //???Not sure
#define MIDDLE_ANGLE
#define WHITE_ANGLE 765

#define QRD_HIGH 682 // 2/3 of 1023
#define QRD_MED 341  // 1/3 of 1023

#define SONAR_N ADC1BUF3 // pin5
#define SONAR_W ADC1BUF4 // pin6
#define SONAR_S ADC1BUF13 // pin7
#define SONAR_E ADC1BUF12 // pin15

#define SONAR_STUPID 400
#define SONAR_HIGH 300 // Sense Wall 
#define SONAR_LOW 200 // Close enough to wall to change direction
#define SONAR_DROP 100 // close enough to wall to drop the ball

#define N_WALL_DETECT 1400
#define N_BALL_DROP_DETECT 600

#define E_WALL_DETECT 1500
#define E_BALL_DROP_DETECT 775

#define S_WALL_DETECT 1050
#define S_BALL_DROP_DETECT 500

#define W_WALL_DETECT 1700
#define W_BALL_DROP_DETECT 700



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
extern BOOL drop_complete;
extern BOOL past_drop;

#endif