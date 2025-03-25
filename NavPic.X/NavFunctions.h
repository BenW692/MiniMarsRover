
#ifndef NAVFUNCTIONS_H
#define NAVFUNCTIONS_H

#include <xc.h>
#include "NavGlobals.h"

BOOL isCanyonSensed(); 

BOOL isDropSensed();

void locateTurn(); 

int senseLine();

void dropBall();

int read_QRD(unsigned int QRD_val);

void fourBit_FSM();

void test_canyon_detect();

void qrd_test();

#endif