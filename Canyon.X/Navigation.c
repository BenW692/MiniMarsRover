/*
 * File:   Navigation.c
 * Author: jss265
 *
 * Created on March 15, 2025, 3:09 PM
 */


#include "xc.h"

#pragma config FNOSC = FRCDIV // 8 MHz osccilator 
#pragma config OSCIOFNC = OFF
#pragma config SOSCSRC = DIG

#define TRUE 1
#define FALSE 0
#define BOOL int

#define NO_LINE 0b0000 // change this !!!! potentially put into header file with global var
#define STRAIGHT 0b0001 
#define SMALL_RIGHT 0b0010
#define MED_RIGHT 0b0011
#define BIG_RIGHT 0b0100
#define SMALL_LEFT 0b0101
#define MED_LEFT 0b0110
#define BIG_LEFT 0b0111

#define DRIVE_NORTH 0b1000
#define DRIVE_EAST 0b1001
#define DRIVE_SOUTH 0b1010
#define DRIVE_WEST 0b1011

#define QRD1 ADC1BUF0
#define QRD2 ADC1BUF1
#define QRD3 ADC1BUF2

#define QRD_HIGH 682 // 2/3 of 1023
#define QRD_MED 341  // 1/3 of 1023

#define SONAR_N ADC1BUF3 // pin5
#define SONAR_E ADC1BUF4 // pin6
#define SONAR_S ADC1BUF13 // pin7
#define SONAR_W ADC1BUF14 // pin8

#define SONAR_HIGH 220 // Sense Wall 
#define SONAR_LOW 140 // Close enough to wall

#define WORDBIT1 _LATB4
#define WORDBIT2 _LATA4 
#define WORDBIT3 _LATB8
#define WORDBIT4 _LATB7

int bitWord = STRAIGHT;
int lineState = -1;

int qrd1 = 0;
int qrd2 = 2;
int qrd3 = 0;

int main(void) {
    _RCDIV = 0;
    pinSetup();
    config_ADC();
    
//    while(1) {
//        if (SONAR_N < SONAR_HIGH) {
//            WORDBIT1 = 1;
//        } else {
//            WORDBIT1 = 0;
//        }
//        if (SONAR_E < SONAR_HIGH) {
//            WORDBIT2 = 1;
//        } else {
//            WORDBIT2 = 0;
//        }
//        if (SONAR_S < SONAR_HIGH) {
//            WORDBIT3 = 1;
//        } else {
//            WORDBIT3 = 0;
//        }
//        if (SONAR_W < SONAR_HIGH) {
//            WORDBIT4 = 1;
//        } else {
//            WORDBIT4 = 0;
//        }
//    }
    
    while(TRUE) //test canyon start detection
    {
        lineState = senseLine();
        if (SONAR_W < SONAR_HIGH)
        {
            WORDBIT4 = 1;
            if (SONAR_E < SONAR_HIGH)
                WORDBIT2 = 1;
            {
                if (lineState == NO_LINE)
                {
                    WORDBIT1 = 1;
                    WORDBIT3 = 1;
                }
                    
            }
        }
        else
        {
            WORDBIT1 = 0;
            WORDBIT2 = 0;
            WORDBIT3 = 0;
            WORDBIT4 = 0;
        }
    }

    while(TRUE) {
        bitWord = senseLine();
        
        if (isCanyonSensed()) {
            bitWord = senseWall();
            WORDBIT1 = 1;
        } else {
            WORDBIT1 = 0;
        }
        
//        fourBit_FSM();        
    }
    
    return 0;
}

void senseWall() {
    static int sonarN = 1023;
    static int sonarE = 1023;
    static int sonarS = 1023;
    static int sonarW = 1023;

    sonarN = read_Sonar(SONAR_N);
    sonarE = read_Sonar(SONAR_E);
    sonarS = read_Sonar(SONAR_S);
    sonarW = read_Sonar(SONAR_W);
    
    if (1) {} // this is logic to determine where the pic goes
}

BOOL isCanyonSensed() {
    if (!qrd1 && !qrd2 && !qrd3){
        if (SONAR_W < SONAR_HIGH && SONAR_E < SONAR_HIGH) {
            return TRUE;
        }
    } else {
        return FALSE;
    }
}

BOOL read_Sonar(unsigned int sonarVal) {
    if (sonarVal / SONAR_LOW) { //only small dip in qrd readout
        return FALSE; // off the line
    } else {
        return TRUE; // on the line
    }
}

int senseLine()
{
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
    else if (qrd2)
    {
        return STRAIGHT;
    }
    else
    {
        return NO_LINE;
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


void fourBit_FSM() { 
    static int oldState = -1;
    
    if (oldState == bitWord) {
        return;
    }
    
    switch(bitWord) {
        case NO_LINE:
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
    
    oldState = bitWord;

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
    
    /* Enable Inputs */
    _TRISA0 = 1;
    _TRISA1 = 1;
    _TRISB0 = 1;
    
    _TRISB1 = 1;
    _TRISB2 = 1;
    _TRISA2 = 1;
    _TRISA3 = 1;
    
    /* Enable Analog */
    _ANSA0 = 1;
    _ANSA1 = 1;    
    _ANSB0 = 1;   
    
    _ANSB1 = 1;
    _ANSB2 = 1;
    _ANSA2 = 1;
    _ANSA3 = 1;
}

void config_ADC() {
    /* Pins 2, 3, 4 (QRDS) && 5, 6, 7, 8 (Sonars) */  
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
    _SMPI = 6;    // begin new sampling sequence after 7 samples
    _ALTS = 0;    // sample MUXA only

    // AD1CON3
    _ADRC = 0;    // use system clock
    _SAMC = 1;    // sample every A/D period
    _ADCS = 32; // TAD = 64*TCY // SHOULD WE CHANGE THIS??

    // AD1CSS -- Choose which channel/pin to scan
    // Select AN0, AN1, AN2 (pins 2, 3, 4) && AN3, AN4, AN13, AN14 (pins 5, 6, 7, 8)
    AD1CSSL = 0b110000000011111; 
    
    _ADON = 1;    // enable module after configuration
}

