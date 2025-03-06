/*
 * File:   LineFollowingStateMachine.c
 * Author: benwo
 *
 * Created on February 25, 2025, 9:41 AM
 */


#include "xc.h"
#pragma config FNOSC = FRCDIV
#pragma config ICS = PGx3

//define state machine
static enum {straight, right, left, stop} line_state = straight; 

//Motor Speeds in rev / sec
int straight_speed = 1500;
int corner_speed = 3500;

// Define global sensor thresholds
#define QRDTHRESHOLD 2048

// Define pin names for stepper motor direction control
#define FRONT_DIR     _LATA0
#define BACK_DIR      _LATA1
#define LEFT_DIR      _LATB9
#define RIGHT_DIR     _LATB8

//define global booleans
#define BOOL unsigned char
#define TRUE 1
#define FALSE 0

//define global direction variables (all defined as inverted pairs)
static int dir_front = 0;
static int dir_back = 1;
static int dir_left = 0;
static int dir_right = 1;

static int QRDs[2];

//interrupt global variables
static unsigned int current_speed = 0;
unsigned int target_speed = 0;


//function declarations
void update_motor_directions();
void config_PWM();
void config_AD();
void poll_QRDs();
void get_turn_speed();

//alert variables
BOOL new_line_state;
BOOL dir_ramp_occuring;


//this is ramp up interrupt, but we won't need it much for line turning I think
//void __attribute__((interrupt, no_auto_psv))_OC3Interrupt(void) //left right motors
//{
//    _OC3IF = 0;
//    
//    if (current_speed < target_speed)
//    {
//        current_speed ++;
//    }
//}
//
//void __attribute__((interrupt, no_auto_psv))_OC2Interrupt(void) //front back motors
//{
//    _OC2IF = 0;
//    
//    if (current_speed < target_speed)
//    {
//        current_speed ++;
//    }
//}
    


int main(void) {
    config_pwm();
    config_AD();
    
    //start by going straight
    
    while(1){
        poll_QRDs();
        
        switch (line_state) {
            case straight:
                if (QRDs[0] < QRDTHRESHOLD) //when left QRD goes off line
                {
                    line_state = right;
                    dir_right ^= 1;
                    update_motor_directions();
                } 
                else if (QRDs[1] < QRDTHRESHOLD) //when right QRD goes off line
                {
                    line_state = left;
                    dir_left ^= 1;
                    update_motor_directions();
                }
                else if (QRDs[0] < QRDTHRESHOLD & (QRDs[1] < QRDTHRESHOLD) //both QRDs not on a line
                {
                    line_state = stop;
                    //in future builds this logic will switch global state to canyon
                }
                
                break;
                
            case left: //left means QRDs[1] is off the line
                get_turn_speed();
                if (QRDs[1] > QRDTHRESHOLD) //when right QRD goes off line
                {
                    line_state = straight;
                    dir_left ^= 1;
                    update_motor_directions();
                }
                
                break;
                
            case right:
                get_turn_speed();
                
                break;
                
            
            case stop:
                OC3R = 0;
                OC3RS = 0;
                OC2R = 0;
                OC2RS = 0;
                break; 
                
        }
    }
    
    return 0;
}

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
    update_motion(4000);
    new_line_state = FALSE;
    
    new_line_state = FALSE;
}

void config_AD()
{
    //clear pin registers
    TRISA = 0;
    TRISB = 0;
    ANSA = 0;
    ANSB = 0;
    
    //configure in out
    TRISBbits.TRISB2 = 1; // Configure pin 6 for input
    TRISAbits.TRISA2 = 1; // Configure pin 7 for input
   
    //configure pins for analog
    ANSBbits.ANSB2 = 1; // Configure pin 6 for analog
    ANSAbits.ANSA2 = 1; // Configure pin 7 for analog
    
    //A/D conversion
    _ADON = 0;    // AD1CON1<15> -- Turn off A/D during config
    
    // AD1CON1 register
    _ADSIDL = 0;  // AD1CON1<13> -- A/D continues in idle mode
    _MODE12 = 0;  // AD1CON1<10> -- 12-bit A/D operation
    _FORM = 0;    // AD1CON1<9:8> -- Unsigned integer output
    _SSRC = 7;    // AD1CON1<7:4> -- Auto conversion (internal counter)
    _ASAM = 1;    // AD1CON1<2> -- Auto sampling

    // AD1CON2 register
    _PVCFG = 0;   // AD1CON2<15:14> -- Use VDD as positive
                  // ref voltage
    _NVCFG = 0;   // AD1CON2<13> -- Use VSS as negative
                  // ref voltage
    _BUFREGEN = 1;// AD1CON2<11> -- Result appears in buffer
                  // location corresponding to channel
    _CSCNA = 1;   // AD1CON2<10> -- Scans inputs specified
                  // in AD1CSSx registers
    _SMPI = 9;	  // AD1CON2<6:2> -- Every 9th conversion sent (number of channels sampled -1)
                  // to buffer (if sampling 10 channels)
    _ALTS = 0;    // AD1CON2<0> -- Sample MUXA only

    _CN2PDE = 1; 
    
    // AD1CON3 register
    _ADRC = 0;    // AD1CON3<15> -- Use system clock
    _SAMC = 0;    // AD1CON3<12:8> -- Auto sample every A/D
                  // period TAD
    _ADCS = 0b00000010;    // AD1CON3<7:0> -- TAD needs to be at least 750 ns. Thus, _ADCS = 0b00000010 will give us the fastest AD clock given a 4 MHz system clock.

    // AD1CSS registers
    AD1CSSL = 0b0000000000000011; // choose A2D channels you'd like to scan here.
    
    _ADON = 1;    // AD1CON1<15> -- Turn on A/D

}

void poll_QRDs()
{
    QRDs[0] = (int)ADC1BUF4;  // Pin 6
    QRDs[1] = (int)ADC1BUF13; // Pin 7 
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

void get_turn_speed()
{
    int max_speed = 0; //define max period we are ok with
    //set period by Vread / Vmax * max_speed
}







