/*
 * File:   MasterGlobals.c
 * Author: benwo
 *
 * Created on March 25, 2025, 11:28 AM
 */


#include "xc.h"

#include "MotorGlobals.h"

int strafe_speed = 1300; //I increased this so canyon detected better from 1000
int slip_speed = 3500; //I increased this so slipping didn't occur after ball drop from 2500 I think?

int straight_speed = 1000;
int non_turn_speed = 2400;
int fb_turn_speed = 1600;
int med_turn_speed = 600;

int target_speed_R = 2500;
int target_speed_L = 2500;
int target_speed_FB = 2500;

int oldWord = -1;
int bitWord = -1;
