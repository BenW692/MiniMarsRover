/*
 * File:   MasterFunctions.c
 * Author: benwo
 *
 * Created on March 25, 2025, 11:36 AM
 */


#include "xc.h"

#include "MasterFunctions.h"

void fourBit_FSM()
{
    
    if (oldWord == bitWord) 
    {
        return;
    }
    
    mode_check(); //turns OC interrupt on an off based on line mode or strafe mode
    
    switch (bitWord)
    {
    case NO_LINE:
        set_Straight_Speed(STRAIGHT_SPEED);
        RIGHT_DIR = 1;
        LEFT_DIR = 1;
        break;

    case STRAIGHT:
        set_Straight_Speed(STRAIGHT_SPEED);
        RIGHT_DIR = 1;
        LEFT_DIR = 1;
        break;

//    case SMALL_RIGHT:
//        set_Turn_Speed(1, NON_TURN_SPEED, SMALL_TURN_SPEED);
//        RIGHT_DIR = 1;
//        LEFT_DIR = 1;
//        break;

    case MED_RIGHT:
        set_Turn_Speed(1, NON_TURN_SPEED, MED_TURN_SPEED);
        RIGHT_DIR = 1;
        LEFT_DIR = 1;
        break;

//    case BIG_RIGHT:
//        set_Turn_Speed(1, NON_TURN_SPEED, BIG_TURN_SPEED);
//        RIGHT_DIR = 1;
//        LEFT_DIR = 1;
//        break;

//    case SMALL_LEFT:
//        set_Turn_Speed(0, NON_TURN_SPEED, SMALL_TURN_SPEED);
//        RIGHT_DIR = 1;
//        LEFT_DIR = 1;
//        break;

    case MED_LEFT:
        set_Turn_Speed(0, NON_TURN_SPEED, MED_TURN_SPEED);
        RIGHT_DIR = 1;
        LEFT_DIR = 1;
        break;

//    case BIG_LEFT:
//        set_Turn_Speed(0, NON_TURN_SPEED, BIG_TURN_SPEED);
//        RIGHT_DIR = 1;
//        LEFT_DIR = 1;
//        break;    
        
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
        
    case STOP:
        setStrafeSpeed(-1, -1, -1, -1, STRAFE_SPEED);
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
        FB_PERIOD = SLIP_SPEED;
        FB_DUTY_CYCLE = FB_PERIOD / 2;
    } 
    else 
    {
        FB_PERIOD = 0;
        FB_DUTY_CYCLE = 0;
    }

    if (left != -1 || right != -1) 
    { // Control Left-Right Motors
        target_speed_L = speed;
        L_PERIOD = SLIP_SPEED;
        L_DUTY_CYCLE = L_PERIOD / 2;
        
        target_speed_R = speed;
        R_PERIOD = SLIP_SPEED;
        R_DUTY_CYCLE = R_PERIOD / 2;
    } 
    else 
    {
        R_PERIOD = 0;
        R_DUTY_CYCLE = 0;
    }
}


void enable_OC_interrupt()
{
    _OC3IE = 1;
    _OC3IE = 1;
    _OC1IE = 1;
}

void disable_OC_interrupt()
{
    _OC3IE = 0;
    _OC3IE = 0;
    _OC1IE = 0;
}

void set_Straight_Speed(int speed) {
    FB_PERIOD = 0;
    FB_DUTY_CYCLE = 0;
    R_PERIOD = speed;
    R_DUTY_CYCLE = R_PERIOD / 2;
    L_PERIOD = speed;
    L_DUTY_CYCLE = L_PERIOD /2;
}

void set_Turn_Speed(int turn_dir, int straight_speed, int turn_speed)
{
    FB_PERIOD = 0;
    FB_DUTY_CYCLE = 0;
    
    if (turn_dir) // turn right
    {
        L_PERIOD = turn_speed;
        L_DUTY_CYCLE = L_PERIOD / 2;
        R_PERIOD = straight_speed;
        R_DUTY_CYCLE = R_PERIOD / 2;
    }
    else //turn left
    {
        R_PERIOD = turn_speed;
        R_DUTY_CYCLE = L_PERIOD / 2;
        L_PERIOD = straight_speed;
        L_DUTY_CYCLE = R_PERIOD / 2;
    }
            
}

void mode_check()
{
    if (WORDBIT1) //checks if we are strafing  !!!!This might be wrong bit to check!!!
    {
        enable_OC_interrupt();
    }
    else
    {
        disable_OC_interrupt();
    }
}

