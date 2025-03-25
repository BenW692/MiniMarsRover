#ifndef MASTERFUNCTIONS_H
#define MASTERFUNCTIONS_H

#include <xc.h>
#include "MasterGlobals.h"

void fourBit_FSM();

int poll_bitWord();

void setStrafeSpeed(int left, int right, int front, int back, int speed);

void enable_OC_interrupt();

void disable_OC_interrupt();

void set_Straight_Speed(int speed);

void set_Turn_Speed(int turn_dir, int straight_speed, int turn_speed);

void mode_check();


#endif