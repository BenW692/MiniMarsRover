/*
 * File:   NavFunctions.c
 * Author: benwo
 *
 * Created on March 24, 2025, 5:57 PM
 */


#include "xc.h"
#include "NavFunctions.h"


void DUMPYs_Favorite_Game() {
    
    /* Navigation */
    senseLine();
    fourBit_FSM();
//    poll_GPS(); // also shifty... more issues

    /* Task Check-List */
    switch (roverState) {
        case state1: // In Lander
            pollLander1();
            break;
            
        case state2: // pre-tower
            pollTower();
            break;
            
        case state3: // pre-drop
            pollDrop();
            break;
            
        case state4: // pre-lander and laser transmission
            pollLander2(); // shifty... has issues
            break;
    }
}

void pollLander1() {
    
    /* leave Lander */
    bitWord = STRAIGHT;
    fourBit_FSM();
    while (!isLanderSensed());
    
    /* turn out of lander */
    bitWord = ROTATE_CCW;
    fourBit_FSM();
    while (QRD2 > QRD_MED);

    /* continue line following */
    bitWord = STOP;
    fourBit_FSM();

    /* change roverState */
    roverState = state2;
}

void poll_GPS() {
        while (isCanyonSensed) // this should maybe not be a while loop
        {
            locateTurn(); // change to pollCanyon() ??
        }
}

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

int setTimer3(int ms) {
    stateTimer3 = TRUE;
    TMR3 = 0;
    _T3IE = 1;
    PR3 = ms * 15;
    T3CONbits.TON = 1; // turn timer on
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

BOOL isTowerSensed() {
    if (filterSignal(11, 100) > TOWER_DETECT) { // BALL_TOWER is ADC1BUF11 !!! also filterSignal will slow polling down
        return TRUE;
    } else {
        return FALSE;       
    }
}

void pollLander2() {
    if (isLanderSensed()) 
    {
        /* line follow into lander */
        bitWord = ROTATE_CCW;
        fourBit_FSM();
        delay(500); // to avoid triggering cross white line
        while (QRD2 > QRD_MED);
        bitWord = STOP;
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
        // "gg Dumpy"
    }
}

void pollTower() {
    if (isTowerSensed()) {
        bitWord = DRIVE_EAST;
        fourBit_FSM();
        delay(1500);
        bitWord = STOP;
        fourBit_FSM();
        delay(750);
        bitWord = DRIVE_WEST;
        fourBit_FSM();
        while (QRD2 > QRD_MED);
        bitWord = STOP;
        fourBit_FSM();
        
        setTimer3(3000);
        while (stateTimer3) {
            senseLine();
            fourBit_FSM();
        }
        
        /* change roverState */
        roverState = state3;
    }
}

void pollDrop() {
    if (isDropSensed()) {
        bitWord = STOP;
        fourBit_FSM();
        
        if (BALL_QRD < 100)
        {
            bitWord = DRIVE_WEST; // white decision
            fourBit_FSM();
            delay(600);
            bitWord = STOP;
            fourBit_FSM();
            SERVO_ANGLE = WHITE_ANGLE;
            delay(2000);
            bitWord = DRIVE_EAST;
            fourBit_FSM();
        }
        else 
        {
            bitWord = DRIVE_EAST; // black decision
            fourBit_FSM();
            delay(600);
            bitWord = STOP;
            fourBit_FSM();
            SERVO_ANGLE = BLACK_ANGLE;
            delay(2000);
            bitWord = DRIVE_WEST;
            fourBit_FSM();
        }
        
        SERVO_ANGLE = MIDDLE_ANGLE; // find the line
        while(QRD2 > QRD_HIGH);
        bitWord = STOP;
        fourBit_FSM();
        
        setTimer3(3000); // leave drop zone
        while (stateTimer3) {
            senseLine();
            fourBit_FSM();
        }
        
        roverState = state4; // change roverState
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

void senseLine()
{
    //we can get rid of read QRD. It get auto updated from the ADC  
    qrd1 = read_QRD(QRD1); // west qrd
    qrd2 = read_QRD(QRD2); // middle qrd
    qrd3 = read_QRD(QRD3); // east qrd
    
    switch(bitWord)
    {
        case STOP: // after some sequences the STOP word is sent. This case accelerates to straight motion once senseLine() is finally called
            bitWord = ACCEL_STRAIGHT;
            break;
        case ACCEL_STRAIGHT:
            delay(125); // this delays the signal to ensure that the motor pic receives it
            bitWord = STRAIGHT;
            break;
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
    PR1 = 0b1111111111111110; // max 16 bit
    TMR1 = 0;
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
        case ACCEL_STRAIGHT:
            sendWord(1, 0, 1, 0);
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
            sendWord(0, 1, 1, 1);
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

int read_ADC1BUF(int num) {
    switch (num) {
        case 0:
            return ADC1BUF0;
            break; 
        case 1:
            return ADC1BUF1;
            break; 
        case 2:
            return ADC1BUF2;
            break; 
        case 3:
            return ADC1BUF3;
            break; 
        case 4:
            return ADC1BUF4;
            break; 
        case 5:
            return ADC1BUF5;
            break; 
        case 6:
            return ADC1BUF6;
            break; 
        case 7:
            return ADC1BUF7;
            break; 
        case 8:
            return ADC1BUF8;
            break; 
        case 9:
            return ADC1BUF9;
            break; 
        case 10:
            return ADC1BUF10;
            break; 
        case 11:
            return ADC1BUF11;
            break; 
        case 12:
            return ADC1BUF12;
            break; 
        case 13:
            return ADC1BUF13;
            break; 
        case 14:
            return ADC1BUF14;
            break; 
        case 15:
            return ADC1BUF15;
            break;
        default:
            return -1; // Invalid case
    }
}

int alphaFilter(int input) {
    // !!! this function might need to be changed if needed in more than one place. Or it may not
    static float alpha = 0.1f; // the f treats it as a 4 bit float instead of a double - C language quirk
    static float previousOutput = 0.0f;
    
    previousOutput = alpha * input + (1 - alpha) * previousOutput;

    return (int)previousOutput;
}

int filterSignal(int ADC_num, int numCounts) {
    // this function seems to be behaving as expected but with out serial com its hard to evaluate its effectiveness
    // slight delays at numCounts = 1000, and long perceivable delay around numCounts = 10000
    int value;
    
    for (int i = 0; i < numCounts; i++) {
        value = alphaFilter(read_ADC1BUF(ADC_num));
    }
    
    return value;
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