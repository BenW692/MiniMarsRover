/*
 * File:   MasterGlobals.c
 * Author: benwo
 *
 * Created on March 25, 2025, 11:28 AM
 */


#include "xc.h"

#include "MotorGlobals.h"

int strafe_speed = 800; //1000 is pretty safe
int slip_speed = 2250; //I increased this so slipping didn't occur after ball drop from 2500 I think?

int straight_speed = 800; //1000
int non_turn_speed = 1920;
int fb_turn_speed = 1280;
int med_turn_speed = 480;

int target_speed_R = 2250;
int target_speed_L = 2250;
int target_speed_FB = 2250;

int oldWord = -1;
int bitWord = -1;
