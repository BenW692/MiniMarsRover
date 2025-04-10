/*
 * File:   NavGlobals.c
 * Author: benwo
 *
 * Created on March 25, 2025, 7:58 AM
 */


#include "xc.h"
#include "NavGlobals.h"

int bitWord = STRAIGHT;
int oldState = -1;

int qrd1 = 0;
int qrd2 = 2;
int qrd3 = 0;

int ball_color = -1;
int ball_qrd = 0;


BOOL drop_complete = FALSE;
BOOL past_drop = TRUE;
BOOL stateTimer3 = TRUE;

State roverState = state1;


