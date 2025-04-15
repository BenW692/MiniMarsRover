/*
 * File:   MasterGlobals.c
 * Author: benwo
 *
 * Created on March 25, 2025, 11:28 AM
 */


#include "xc.h"

#include "MotorGlobals.h"

int strafe_speed = 1355; //800 is pretty safe
int slip_speed = 3812; //I increased this so slipping didn't occur after ball drop from 2500 I think?

int straight_speed = 1355; //800
int non_turn_speed = 3253;
int fb_turn_speed = 2168;
int med_turn_speed = 813;

int target_speed_R = 3812;
int target_speed_L = 3812;
int target_speed_FB = 3812;

int oldWord = -1;
int bitWord = -1;
