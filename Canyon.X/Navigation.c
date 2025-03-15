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

#define WORDBIT1 _LATB4
#define WORDBIT2 _LATA4 
#define WORDBIT3 _LATB8
#define WORDBIT4 _LATB7

int lineState = STRAIGHT;

int main(void) {
    pinSetup();
    config_ADC();
    
    return 0;
}

int senseLine()
{
    static int qrd1 = 0;
    static int qrd2 = 2;
    static int qrd3 = 0;
    
    qrd1 = read_QRD(QRD1);
    qrd2 = read_QRD(QRD2);
    qrd3 = read_QRD(QRD3);
    
    if (qrd1 == 1 && qrd2) // if left little and middle a lot
    {
        return SMALL_LEFT;
    } 
    else if (qrd3 == 1 && qrd2) // if right little and middle a lot
    {
        return SMALL_RIGHT;
    }
    else if (qrd1)
    {
        return MED_RIGHT;
    }
    else if (qrd3)
    {
        return MED_LEFT;
    }
    else
    {
        return STRAIGHT;
    }
    
}

int read_QRD(unsigned int QRD_val) {
    if (QRD_val / QRD_HIGH) { //only small dip in qrd readout
        return 0; // off the line
    } else if (QRD_val / QRD_MED) { //medium dip in qrd readout
        return 1; // kind on the line
    } else {
        return 2; // on the line
    }
}


void lineFSM() { 
    static int oldState = -1;
    
    if (oldState == lineState) {
        return;
    }
    
    switch(lineState) {
        case STOP:
            sendWord1(0, 0, 0);
            break;
        case STRAIGHT:
            sendWord1(0, 0, 1);
            break;
        case SMALL_RIGHT:
            sendWord1(0, 1, 0);
            break;
        case MED_RIGHT:
            sendWord1(0, 1, 1);
            break;
        case BIG_RIGHT:
            sendWord1(1, 0, 0);
            break;
        case SMALL_LEFT:
            sendWord1(1, 0, 1);
            break;
        case MED_LEFT:
            sendWord1(1, 1, 0);
            break;
        case BIG_LEFT:
            sendWord1(1, 1, 1);
            break;
    }
    
    oldState = lineState;

//    corse correction try later OR WITH A TIMER
//    static int lastLineState = STOP;
//
//    if (lineState != lastLineState) {
//        lastLineState = lineState;
//    }
}

void sendWord1(int s1, int s2, int s3) {
    WORDBIT1 = s1;
    WORDBIT2 = s2;
    WORDBIT3 = s3;
    //WORDBIT4 = s4;
}

//void sendWord2(int s1, int s2, int s3) {
//    WORD2BIT1 = s1;
//    WORD2BIT2 = s2;
//    WORD2BIT3 = s3;
//}

void pinSetup() {
    /* Digital output pins: 11, 12, 13, 14, 15, 16 
     * Digital input pins:
     * Analog output:
     * Analog input: 2, 3, 4
     */
    
    /* Clear Pin Registers */
    TRISA = 0;
    TRISB = 0;
    ANSA = 0;
    ANSB = 0;
    LATA = 0;
    LATB = 0;
    
    /* I/O */
    _TRISA0 = 1;
    _TRISA1 = 1;
    _TRISB0 = 1;

    _TRISB7 = 0;
    _TRISB8 = 0;
    _TRISB9 = 0;
    _TRISA6 = 0;
    _TRISB12 = 0;
    _TRISB13 = 0;
    
    /* Enable Analog */
    _ANSA0 = 1;
    _ANSA1 = 1;    
    _ANSB0 = 1;        
}

void config_ADC() {
    /* Pins 2, 3, 4 */  
    _ADON = 0;    // Disable A/D module during configuration
    
    // AD1CON1
    _MODE12 = 0;  // 10-bit resolution
    _FORM = 0;    // unsigned integer output
    _SSRC = 7;    // auto convert
    _ASAM = 1;    // auto sample

    // AD1CON2
    _PVCFG = 0;   // use VDD as positive reference
    _NVCFG = 0;   // use VSS as negative reference
    _BUFREGEN = 1;// store results in buffer corresponding to channel number
    _CSCNA = 1;   // enable scanning mode
    _SMPI = 2;    // begin new sampling sequence after 3 samples
    _ALTS = 0;    // sample MUXA only

    // AD1CON3
    _ADRC = 0;    // use system clock
    _SAMC = 1;    // sample every A/D period
    _ADCS = 0x3F; // TAD = 64*TCY // SHOULD WE CHANGE THIS??

    // AD1CSS -- Choose which channel/pin to scan
    AD1CSSL = 0b111; // Select AN0, AN1, AN2 (pins 2, 3, 4)
    
    _ADON = 1;    // enable module after configuration
}