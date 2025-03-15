/*
 * File:   Navigation.c
 * Author: jss265
 *
 * Created on March 15, 2025, 3:09 PM
 */


#include "xc.h"

#pragma config FNOSC = FRCDIV // 8 MHz osccilator 
#pragma config ICS = PGx3 // defines pins 9 and 10 for debug & programming

#define TRUE 1
#define FALSE 0
#define BOOL unsigned char

#define STOP 0b000 // potentially put into header file with global var
#define STRAIGHT 0b001 
#define SMALL_RIGHT 0b010
#define MED_RIGHT 0b011
#define BIG_RIGHT 0b100
#define SMALL_LEFT 0b101
#define MED_LEFT 0b110
#define BIG_LEFT 0b111

#define QRD1 ADC1BUF0
#define QRD2 ADC1BUF1
#define QRD3 ADC1BUF2

#define QRD_HIGH 682 // 2/3 of 1023
#define QRD_MED 341  // 1/3 of 1023

#define WORD1BIT1 _LATB13
#define WORD1BIT2 _LATB12
#define WORD1BIT3 _LATA6

#define WORD2BIT1 _LATB9
#define WORD2BIT2 _LATB8
#define WORD2BIT3 _LATB7

int lineState = STRAIGHT;

int main(void) {
    
    return 0;
}
