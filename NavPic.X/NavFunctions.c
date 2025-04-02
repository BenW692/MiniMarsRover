/*
 * File:   NavFunctions.c
 * Author: benwo
 *
 * Created on March 24, 2025, 5:57 PM
 */


#include "xc.h"
#include "NavFunctions.h"


BOOL isCanyonSensed() 
{
    if (QRD1 > QRD_HIGH && QRD2 > QRD_HIGH && QRD3 > QRD_HIGH) // should we be calling read_QRD()???
    {
        if (SONAR_W < W_WALL_DETECT || SONAR_E < E_WALL_DETECT || SONAR_N < N_WALL_DETECT || SONAR_S < S_WALL_DETECT) //for entering the canyon
        {
            return TRUE;
        }
    } 
    else 
    {
        return FALSE;
    }
}

BOOL isLanderSensed() {
    return (read_QRD(LANDER_QRD)) ? TRUE : FALSE;
}

BOOL isDropSensed()
{
    if ( (QRD1 < QRD_MED || QRD2 < QRD_MED || QRD3 < QRD_MED) && SONAR_W < W_BALL_DROP_DETECT)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


void pollLander() {
    if (isLanderSensed()) 
    {
        /* line follow into lander */
        bitWord = ROTATE_CCW;
        fourBit_FSM();
        delay(600); // do we need this??
        while (QRD2 > QRD_MED);
        bitWord = STRAIGHT;
        while (SONAR_N > N_LANDER_WALL)
        {
            senseLine();
            fourBit_FSM();
        }
        
        /* stop */
        bitWord = STOP;
        fourBit_FSM();
        
        /* point and shoot laser (CREATE ARRAY or BISECTION) */        
        aimShootLaser(); 
    }
}

void aimShootLaser() {
    int tol = 6;
    int commsArray[2][5] = {
        {0, 0, 0, 0, 0}, // OC1R - Duty Cycle
        {0, 0 ,0, 0, 0}  // Associated Value
    };
    
    int lowerBound = (MIDDLE_ANGLE + BLACK_ANGLE) / 2;
    int upperBound = WHITE_ANGLE;
    int valCompare = 0;
    int maxIndex = 0;
    
    /* AIM */
    for (int j; j < tol; j++) {
        // reset to lower bound
        SERVO_ANGLE = lowerBound;
        delay(600); // shorten this eventually
        int step = (upperBound - lowerBound) / 4; 
        
        // read in sensor array
        for (int i = 0; i < 5; i++) {
            commsArray[0][i] = SERVO_ANGLE;
            commsArray[1][i] = SATELLITE_DIODE;
            if (i != 5) {
                SERVO_ANGLE += step;
                delay(300);
            }
        }
        
        // get index for max array value
        for (int i = 0; i < 5; i++) {
            if (commsArray[1][i] > valCompare) {
                valCompare = commsArray[1][i];
                maxIndex = i;
            } 
        }
        if (maxIndex == 0) maxIndex = 1;
        if (maxIndex == 4) maxIndex = 3;    
        
        // adjusting bounds
        lowerBound = commsArray[0][maxIndex - 1];
        upperBound = commsArray[0][maxIndex + 1];
    }
    SERVO_ANGLE = commsArray[0][maxIndex];
    
    /* shooting laser */
    
    /* END PROGRAM */
    while(TRUE)
    {   
        LASER_OUT = 1;
    }
}

void pollDrop() {
    if (isDropSensed()) {
        delay(500);
        bitWord = STOP;
        fourBit_FSM();
        if (BALL_QRD < 100)
        {
            bitWord = DRIVE_WEST;
            fourBit_FSM();
            delay(375);
            bitWord = STOP;
            fourBit_FSM();
            SERVO_ANGLE = WHITE_ANGLE;
            delay(2000);
            bitWord = DRIVE_EAST;
            fourBit_FSM();
        }
        else 
        {
            bitWord = DRIVE_EAST;
            fourBit_FSM();
            delay(375);
            bitWord = STOP;
            fourBit_FSM();
            SERVO_ANGLE = BLACK_ANGLE;
            delay(2000);
            bitWord = DRIVE_WEST;
            fourBit_FSM();
        }
        SERVO_ANGLE = MIDDLE_ANGLE;
        while(QRD2 > QRD_HIGH);
        bitWord = STRAIGHT;
        fourBit_FSM();
        delay(650);
    }
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

int senseLine()
{
    //we can get rid of read QRD. It get auto updated from the ADC  
    qrd1 = read_QRD(QRD1); //west qrd
    qrd2 = read_QRD(QRD2);
    qrd3 = read_QRD(QRD3);
    
    switch(bitWord)
    {
        case STRAIGHT:
            if (qrd1 == 2)
            {
                bitWord = TURN_LEFT;
            }
            else if (qrd3 == 2)
            {
                bitWord = TURN_RIGHT;
            }
            break;
        case TURN_RIGHT:
            if (qrd2 == 2)
            {
                bitWord = STRAIGHT;
            }
            break;
        case TURN_LEFT:
            if (qrd2 == 2)
            {
                bitWord = STRAIGHT;
            }
            break;        
                
    }
    
}

void delay(int ms) {
    T1CONbits.TON = 1;
    TMR1 = 0;
    PR1 = 0b1111111111111111; // max 16 bit
    while (TMR1 < ms * 15); // ms * 15625 / 1000 is better not sure if it works
    // NOTE: blocking delay
    T1CONbits.TON = 0;
}

int read_QRD(unsigned int QRD_val) {
    if (QRD_val / QRD_HIGH) {
        return 0; // off the line
    } else if (QRD_val / QRD_MED) {
        return 1; // kinda on the line
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
        case TURN_RIGHT:
            sendWord(0, 0, 1, 0);
            break;
        case TURN_LEFT:
            sendWord(0, 0, 1, 1);
            break;
        case DRIVE_NORTH:
            sendWord(0, 1, 0, 0);
            break;
        case DRIVE_EAST:
            sendWord(0, 1, 0, 1);
            break; 
        case DRIVE_SOUTH:
            sendWord(0, 1, 1, 0);
            break;
        case DRIVE_WEST:
            sendWord(1, 1, 1, 1);
            break;
        case ROTATE_CW:
            sendWord(1, 0, 0, 0);
            break;
        case ROTATE_CCW:
            sendWord(1, 0, 0, 1);
            break;
        case STOP:
            sendWord(1, 1, 1, 1);
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
        senseLine();
        if (SONAR_W < SONAR_HIGH)
        {
            WORDBIT4 = 1;
            if (SONAR_E < SONAR_HIGH)
                WORDBIT2 = 1;
            {
                if (bitWord == NO_LINE)
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
        sendWord(qrd3, qrd2, qrd1, 0);
     }
}

//void dropBall()
//{
//    
////    get ball color !!MOVE THIS TO WHEN BALL IS STILL!!
//    if (BALL_QRD < QRD_MED)
//    {
//        ball_color = 0; //ball is white
//    }
//    else
//    {
//        ball_color = 1; //ball is black
//    }
////    strafe to the correct bin
//    while (SONAR_W > SONAR_DROP && SONAR_E > SONAR_DROP)
//    {
//        if (ball_color)
//        {
//            bitWord = DRIVE_EAST;
//        }
//        else
//        {
//            bitWord = DRIVE_WEST;
//        }
//    }
//    bitWord = STOP;
////    drop the ball (use a timer of 2 seconds)
//    T1CONbits.TON = 1; //start timer
//    while (!drop_complete)
//    {
//        SERVO_PERIOD = 79999;
//        if (ball_color) //black
//        {
//            SERVO_ANGLE = BLACK_ANGLE; //tip it right (not sure about this angle)
//        }
//        else
//        {
//            SERVO_ANGLE = WHITE_ANGLE;
//        } 
//    }
//    T1CONbits.TON = 0; //timer turned off
////    go back to line
//    while (QRD2 > QRD_MED)
//    {
//        if (ball_color)
//        {
//            bitWord = DRIVE_WEST;
//        }
//        else
//        {
//            bitWord = DRIVE_EAST;
//        }
//    }
//    
//    T1CONbits.TON = 1; //timer turned off !!Change this for another event checker in main function
//    PR1 = 15000; //subject to change based on how long it takes to drive away from the drop
////    past_drop = FALSE;
//    while (!past_drop)
//    {
//        bitWord = STRAIGHT;
//    }
//    
//}