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

#define NO_LINE 0b0000 //potentially put into header file with global var
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

#define SONAR_HIGH 300 // Sense Wall 
#define SONAR_LOW 200 // Close enough to wall

#define WORDBIT1 _LATB4
#define WORDBIT2 _LATA4 
#define WORDBIT3 _LATB8
#define WORDBIT4 _LATB7

int bitWord = STRAIGHT;
int oldState = -1;

int qrd1 = 0;
int qrd2 = 2;
int qrd3 = 0;

int main(void) {
    _RCDIV = 0;
    pinSetup();
    config_ADC();


    

    bitWord = DRIVE_NORTH;
    while (TRUE) { //defaults to canyon mode test
        locateTurn();
        fourBit_FSM();
    }
    

    
//    while (TRUE) //defaults to line following test
//    {
//        bitWord = senseLine();
//        fourBit_FSM();
//    }
    
//    while(TRUE) {
//        
//        if (isCanyonSensed())
//        {
//            bitWord = senseWall();
//        }
//        else
//        {
//            locate_turn();
//        }
//        
//        fourBit_FSM();
//}
}

void locateTurn() {
    switch(bitWord) {
        case DRIVE_NORTH:
            if (SONAR_N < SONAR_LOW)
            {
                if (SONAR_E < SONAR_HIGH)
                {
                    bitWord = DRIVE_WEST;
                }
                else if (SONAR_W < SONAR_HIGH) 
                {
                    bitWord = DRIVE_EAST;
                }
                
            }
            break;
        case DRIVE_EAST:
            if (SONAR_E < SONAR_LOW)
            {
                if (SONAR_N < SONAR_HIGH) 
                {
                    bitWord = DRIVE_SOUTH;
                }
                else if (SONAR_S < SONAR_HIGH)
                {
                    bitWord = DRIVE_NORTH;
                }
            }
            break;
        case DRIVE_SOUTH:
            if (SONAR_S < SONAR_LOW)
            {
                if (SONAR_W < SONAR_HIGH) 
                {
                    bitWord = DRIVE_EAST;
                }
                else if (SONAR_E < SONAR_HIGH)
                {
                    bitWord = DRIVE_WEST;
                }
            }
            break;
        case DRIVE_WEST:
            if (SONAR_W < SONAR_LOW)
            {
                if (SONAR_N < SONAR_HIGH) 
                {
                    bitWord = DRIVE_SOUTH;
                }
                else if (SONAR_S < SONAR_HIGH)
                {
                    bitWord = DRIVE_NORTH;
                }
            }
            break;
    }
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

int senseLine()
{
    qrd1 = read_QRD(QRD1); //west qrd
    qrd2 = read_QRD(QRD2);
    qrd3 = read_QRD(QRD3);
    
    switch(bitWord)
    {
        case STRAIGHT:
            if (qrd1 == 2)
            {
                bitWord = MED_LEFT;
            }
            else if (qrd3 == 2)
            {
                bitWord = MED_RIGHT;
            }
            break;
        case MED_RIGHT:
            if (qrd2 == 2)
            {
                bitWord = STRAIGHT;
            }
            break;
        case MED_LEFT:
            if (qrd2 == 2)
            {
                bitWord = STRAIGHT;
            }
            break;        
                
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
    
    if (oldState == bitWord) {
        return;
    }
    
    switch(bitWord) {
        case NO_LINE:
            sendWord(0, 0, 0, 0);
            break;
        case STRAIGHT:
            sendWord(0, 0, 0, 1);
            break;
        case SMALL_RIGHT:
            sendWord(0, 0, 1, 0);
            break;
        case MED_RIGHT:
            sendWord(0, 0, 1, 1);
            break;
        case BIG_RIGHT:
            sendWord(0, 1, 0, 0);
            break;
        case SMALL_LEFT:
            sendWord(0, 1, 0, 1);
            break;
        case MED_LEFT:
            sendWord(0, 1, 1, 0);
            break;
        case BIG_LEFT:
            sendWord(0, 1, 1, 1);
            break;
        case DRIVE_NORTH:
            sendWord(1, 0, 0, 0);
            break;
        case DRIVE_EAST:
            sendWord(1, 0, 0, 1);
            break; 
        case DRIVE_SOUTH:
            sendWord(1, 0, 1, 0);
            break;
        case DRIVE_WEST:
            sendWord(1, 0, 1, 1);
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

void sendWord(int s1, int s2, int s3, int s4) {
    WORDBIT1 = s4;
    WORDBIT2 = s3;
    WORDBIT3 = s2;
    WORDBIT4 = s1;
}


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

void test_sonars()
{
        while(1) {
        if (SONAR_N < SONAR_HIGH) {
            WORDBIT1 = 1;
        } else {
            WORDBIT1 = 0;
        }
        if (SONAR_E < SONAR_HIGH) {
            WORDBIT2 = 1;
        } else {
            WORDBIT2 = 0;
        }
        if (SONAR_S < SONAR_HIGH) {
            WORDBIT3 = 1;
        } else {
            WORDBIT3 = 0;
        }
        if (SONAR_W < SONAR_HIGH) {
            WORDBIT4 = 1;
        } else {
            WORDBIT4 = 0;
        }
    }
}

void test_canyon_detect()
{
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
}

void qrd_test()
{
    while (TRUE)
    {
        qrd1 = read_QRD(QRD1);
        qrd2 = read_QRD(QRD2);
        qrd3 = read_QRD(QRD3); 
//      endWord1(qrd1, qrd2, qrd3, 0);
        sendWord1(0, 0, qrd2, 0);
    }
}
