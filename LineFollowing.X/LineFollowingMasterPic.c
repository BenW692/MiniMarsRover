/*
 * File:   LineFollowingMasterPic.c
 * Author: benwo
 *
 * Created on March 8, 2025, 11:46 AM
 */


#include "xc.h"
#pragma config FNOSC = FRCDIV
#pragma config ICS = PGx3

//define pin names
#define FRONT_DIR     _LATA0
#define BACK_DIR      _LATA1
#define LEFT_DIR      _LATB9
#define RIGHT_DIR     _LATB8

#define LINE_STATE_PIN_1    PORTAbits.RA0 //whatever pin we choose
#define LINE_STATE_PIN_2    PORTAbits.RA1 //whatever pin we choose
#define LINE_STATE_PIN_3    PORTAbits.RA2 //whatever pin we choose

//define global variables
static enum {stop, straight, right, fast_right, fast_left, left} line_state = straight;
          //{ 000, 001, 010, 011, 100, 101}

#define STOP 0b000
#define STRAIGHT 0b001
#define RIGHT 0b010
#define FAST_RIGHT 0b011
#define LEFT 0b100
#define FAST_LEFT 0b101

int straight_speed = 1500;
int fast_turn_speed = 3500;
int slow_turn_speed = 5000;

#define BOOL unsigned char
#define TRUE 1
#define FALSE 0

static int dir_front = 0;
static int dir_back = 1;
static int dir_left = 0;
static int dir_right = 1;


void config_pwm()
{
    //clear all registers
    ANSA = 0; 
    ANSB = 0; 
    TRISA = 0; 
    TRISB = 0; 
    LATA = 0; 
    LATB = 0; 

    // Front Stepper Direction DO pin 2
    _ANSA0 = 0;
    _TRISA0 = 0;
    FRONT_DIR = dir_front; //configure as forward NEED TEST
    
    // Back Stepper Direction DO pin 3
    _ANSA1 = 0;
    _TRISA1 = 0;
    BACK_DIR = dir_back; //configure as forward NEED TEST
    
    //Left Stepper Direction DO pin 13
    //_ANSB9 = 0; DONT NEED THIS B/C PIN 13 ONLY DIGITAL
    _TRISB9 = 0;
    LEFT_DIR = dir_left; //configure as forward NEED TEST
    
    //Right Stepper Direction DO pin 12
    //_ANSB8 = 0; DONT NEED THIS B/C PIN 12 ONLY DIGITAL
    _TRISB8 = 0;
    RIGHT_DIR = dir_right; //configure as forward NEED TEST
    
    //Front Back Stepper PWM pin 4
    OC2CON1 = 0;
    OC2CON2 = 0;
    OC2CON1bits.OCTSEL = 0b111; //OC timer is system clock
    OC2CON1bits.OCM = 0b110;
    OC2CON2bits.SYNCSEL = 0x1F;
    OC2CON2bits.OCTRIG = 0;
    
 
    //Left Right Stepper PWM pin 5
    OC3CON1 = 0;
    OC3CON2 = 0;
    OC3CON1bits.OCTSEL = 0b111; //output compare timer is system clock
    OC3CON1bits.OCM = 0b110; //edge aligned output
    OC3CON2bits.SYNCSEL = 0x1F;
    OC3CON2bits.OCTRIG = 0; //use this OC module
    
    //configure OC interrupt
    _OC3IP = 4; //sets priority
    _OC3IE = 1; //enables OC3 interrupt
    _OC3IF = 0; //clears interrupt flag
    
    line_state = straight;//when the switch turns on, the state will go to straight
    go_straight();
}

void go_straight()
{
    line_state = straight;
    //change direction of left wheel to go back straight
    dir_left ^= 1;
}

void update_motor_directions() 
{
    FRONT_DIR = dir_front;
    BACK_DIR = dir_back;
    LEFT_DIR = dir_left;
    RIGHT_DIR = dir_right;
}

unsigned int poll_slave_line_state()
{
    unsigned int line_state_word = 0;
    //the weird syntax below forces the int to change at the 0, 1, or 2 bit spot in the digital word
    line_state_word |= (LINE_STATE_PIN_1 << 0);  // LSB
    line_state_word |= (LINE_STATE_PIN_2 << 1);
    line_state_word |= (LINE_STATE_PIN_3 << 2); //MSB
    return line_state_word;
}

void line_state_fsm()
{
    //poll serial from slave pic
    unsigned int line_state_word = poll_slave_line_state();
    
    
    switch (line_state) {
        case straight:
            if (line_state_word == RIGHT) //when left QRD goes off line a little bit
            {
                line_state = right;
                dir_right ^= 1;
                update_motor_directions();
            } 
            else if (line_state_word == FAST_RIGHT) //when left QRD goes off line a lot
            {
                line_state = fast_right;
                dir_right ^= 1;
                update_motor_directions();
            }
            else if (line_state_word == LEFT) //when right QRD goes off line a little bit
            {
                line_state = left;
                dir_left ^= 1;
                update_motor_directions();
            }
            else if (line_state_word == FAST_LEFT) //when right QRD goes off line a lot
            {
                line_state = fast_left;
                dir_left ^= 1;
                update_motor_directions();
            }
            else if (line_state_word == STOP) //both QRDs not on a line
            {
                line_state = stop;
                //in future builds this logic will switch global state to canyon
            }

            break;

        case left: 
            if (line_state_word == RIGHT) //when left QRD goes off line a little bit
            {
                line_state = right; //we have to switch hard so both motors change direction
                dir_right ^= 1;
                dir_left ^= 1;
                update_motor_directions();
            } 
            else if (line_state_word == FAST_RIGHT) //when left QRD goes off line a lot
            {
                line_state = fast_right; //we have to switch hard so both motors change direction
                dir_right ^= 1;
                dir_right ^= 1;
                update_motor_directions();
            }
            else if (line_state_word == FAST_LEFT) //when right QRD goes off line a lot
            {
                line_state = fast_left;
                update_motor_directions();
            }
            else if (line_state_word == STRAIGHT)
            {
                line_state = straight;
                dir_left ^= 1;
                update_motor_directions();
            }
            else if (line_state_word == STOP) //both QRDs not on a line
            {
                line_state = stop;
                //in future builds this logic will switch global state to canyon
            }
            break;
            
        case fast_left:
            if (line_state_word == RIGHT) //when left QRD goes off line a little bit
            {
                line_state = right; //we have to switch hard so both motors change direction
                dir_right ^= 1;
                dir_left ^= 1;
                update_motor_directions();
            } 
            else if (line_state_word == FAST_RIGHT) //when left QRD goes off line a lot
            {
                line_state = fast_right; //we have to switch hard so both motors change direction
                dir_right ^= 1;
                dir_right ^= 1;
                update_motor_directions();
            }
            else if (line_state_word == LEFT) //when right QRD goes off line a lot
            {
                line_state = left;
                update_motor_directions();
            }
            else if (line_state_word == STRAIGHT)
            {
                line_state = straight;
                dir_left ^= 1;
                update_motor_directions();
            }
            else if (line_state_word == STOP) //both QRDs not on a line
            {
                line_state = stop;
                //in future builds this logic will switch global state to canyon
            }
            break;
            

        case right:
            if (line_state_word == LEFT) //when left QRD goes off line a little bit
            {
                line_state = left; //we have to switch hard so both motors change direction
                dir_right ^= 1;
                dir_left ^= 1;
                update_motor_directions();
            } 
            else if (line_state_word == FAST_LEFT) //when left QRD goes off line a lot
            {
                line_state = fast_left; //we have to switch hard so both motors change direction
                dir_right ^= 1;
                dir_right ^= 1;
                update_motor_directions();
            }
            else if (line_state_word == FAST_RIGHT) //when right QRD goes off line a lot
            {
                line_state = fast_right;
                update_motor_directions();
            }
            else if (line_state_word == STRAIGHT)
            {
                line_state = straight;
                dir_right ^= 1;
                update_motor_directions();
            }
            else if (line_state_word == STOP) //both QRDs not on a line
            {
                line_state = stop;
                //in future builds this logic will switch global state to canyon
            }
            break;

        case fast_right:
            if (line_state_word == LEFT) //when left QRD goes off line a little bit
            {
                line_state = left; //we have to switch hard so both motors change direction
                dir_right ^= 1;
                dir_left ^= 1;
                update_motor_directions();
            } 
            else if (line_state_word == FAST_LEFT) //when left QRD goes off line a lot
            {
                line_state = fast_left; //we have to switch hard so both motors change direction
                dir_right ^= 1;
                dir_right ^= 1;
                update_motor_directions();
            }
            else if (line_state_word == RIGHT) //when right QRD goes off line a lot
            {
                line_state = right;
                update_motor_directions();
            }
            else if (line_state_word == STRAIGHT)
            {
                line_state = straight;
                dir_right ^= 1;
                update_motor_directions();
            }
            else if (line_state_word == STOP) //both QRDs not on a line
            {
                line_state = stop;
                //in future builds this logic will switch global state to canyon
            }
            break;
        case stop:
            OC3R = 0;
            OC3RS = 0;
            OC2R = 0;
            OC2RS = 0;
            break; 
                
        }
}

int main(void) {
    config_pwm();
    
    while(1)
    {
       line_state_fsm(); 
    }
    
    return 0;
}

