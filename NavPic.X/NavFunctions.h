
#ifndef NAVFUNCTIONS_H
#define NAVFUNCTIONS_H

#include <xc.h>
#include "NavGlobals.h"

BOOL isCanyonSensed(); 

BOOL isLanderSensed();

BOOL isDropSensed();

void pollLander();

void pollDrop();

void locateTurn(); 

int senseLine();

void delay(int ms);

int read_QRD(unsigned int QRD_val);

void fourBit_FSM();

void sendWord(int s1, int s2, int s3, int s4);

void test_canyon_detect();

void qrd_test();

#endif