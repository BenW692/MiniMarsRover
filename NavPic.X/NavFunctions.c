/*
 * File:   NavFunctions.c
 * Author: benwo
 *
 * Created on March 24, 2025, 5:57 PM
 */


#include "xc.h"
#include "NavFunctions.h"
#include <stdlib.h>


void DUMPYs_Favorite_Game() {
    
    /* Navigation */
    senseLine();
    poll_GPS();
    fourBit_FSM();
    
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
            pollLander2();
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
    canyonDone = FALSE;
}

void enableTimer2Interrupt() {
    _T3IE = 1;
}

void disableTimer2Interrupt() {
    _T3IE = 0;
}

void poll_GPS() {
    if (!isCanyonSensed()) return;
    
    if (!nestedCounter(0, 1, 2, 3)) return; // reps used to be 10

    bitWord = DRIVE_NORTH; //initialize canyon
    fourBit_FSM();
    
    while (!counter(1, 2, 3)) //do canyon mode until middle qrd reads line
    // while (QRD2 > QRD_HIGH)
    {
        locateTurn();
        fourBit_FSM();
    }
    
    canyonDone = TRUE;
    bitWord = STOP;
    fourBit_FSM();
    
    if (SONAR_N < N_WALL_DETECT) // SONAR_N < SONAR_S
    {
        bitWord = ROTATE_CW;
        fourBit_FSM();
        delay(500); // to avoid triggering cross white line
        while (QRD2 > QRD_MED);     
    }
    bitWord = STOP;
    fourBit_FSM();
    delay(100);
}

BOOL isCanyonSensed() 
{
    if (canyonDone)
    {
        return FALSE;
    }
    
    if ( QRD1 > QRD_HIGH && QRD2 > QRD_HIGH && QRD3 > QRD_HIGH) // should we be calling read_QRD()???
    {
        if (SONAR_W < W_CANYON_DETECT || SONAR_N < N_WALL_DETECT) //for entering the canyon
//        if (SONAR_W < W_WALL_DETECT || SONAR_N < N_WALL_DETECT || SONAR_S < S_WALL_DETECT) //for entering the canyon
        {
            return TRUE;
        }
    } 
    return FALSE;
}

BOOL isCanyonExitSensed()
{
    if (QRD2 < QRD_HIGH)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

BOOL isLanderSensed() {
    return (LANDER_QRD < LANDER_DETECT) ? TRUE : FALSE;
//    if (counter(2, 4, 5))
//    {
//        return TRUE;
//    }
//    else
//    {
//        return FALSE;
//    }
}

//BOOL isFinalLanderSensed()
//{
//    
//}
        

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
        bitWord = SLOW_MOTORS;
        fourBit_FSM();
        delay(50);
        bitWord = ROTATE_CCW;
        fourBit_FSM();
        delay(500); // to avoid triggering cross white line
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
        aimShootLaserNEW(); 
    }
}

void aimShootLaserNEW() {
    int commsArray[2][30] = {0};

    int lowerBound = (MIDDLE_ANGLE + BLACK_ANGLE) / 2;
    int upperBound = WHITE_ANGLE;
    int step = (upperBound - lowerBound) / 30;
    int valCompare = 0;
    int finalAngle = 0;
    
    SERVO_ANGLE = lowerBound;
    delay(150);
    
    for (int i = 0; i < 30; i++) {
        commsArray[0][i] = SERVO_ANGLE;
        commsArray[1][i] = SATELLITE_DIODE;
        if (i >= 29) break;
        SERVO_ANGLE += step;
        delay(50);
    }
    
    for (int i = 0; i < 30; i++) {
        if (valCompare < commsArray[1][i]) {
            valCompare = commsArray[1][i];
            finalAngle = commsArray[0][i];
        }
    }
    int tuningFactor = ((finalAngle - lowerBound)/(upperBound - lowerBound)) * 130;
    SERVO_ANGLE = finalAngle + tuningFactor; //it missed high just barely twice in a row so we took off the extra 20
    
    /* shooting laser */
    /* END PROGRAM */
    while(TRUE)
    {   
        LASER_OUT = 1;
        // "gg Dumpy"
    }
}

void aimShootLaserOLD() {
    int tol = 5; // was 6
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
        delay(300); // was 600
        int step = (upperBound - lowerBound) / 4; 
        
        // read in sensor array
        for (int i = 0; i < 5; i++) {
            commsArray[0][i] = SERVO_ANGLE;
            commsArray[1][i] = SATELLITE_DIODE;
            if (i != 5) {
                SERVO_ANGLE += step;
                delay(200); // was 100 I slowed down hoping it could better detect laser
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
    SERVO_ANGLE = commsArray[0][maxIndex]; //we need a tuning number here

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
        delay(900); //was 1200
        bitWord = STOP;
        fourBit_FSM();
        delay(750);
        
        bitWord = SLOW_MOTORS;
        fourBit_FSM();
        delay(25);
        bitWord = DRIVE_WEST;
        fourBit_FSM();
        while (QRD2 > QRD_MED);
//        delay(750);
        bitWord = STOP;
        fourBit_FSM();
        delay(25);
        bitWord = QUICKEN_MOTORS;
        fourBit_FSM();
        delay(25);
        bitWord = STOP;
        
        setTimer3(3000); //I increased this from 3000 so it would work at slower speeds for vid
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
        bitWord = DECEL_STRAIGHT;
        fourBit_FSM();
        delay(500);
        bitWord = DRIVE_SOUTH;
        fourBit_FSM();
        delay(350);
        bitWord = STOP;
        fourBit_FSM();
        
        if (BALL_QRD < 100)
        {
            bitWord = DRIVE_WEST; // white decision
            fourBit_FSM();
            delay(400); //was 600
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

void adjustProximity(int orig_dir, int dir1, int sensor1_buf, int detect1, int dir2, int sensor2_buf, int detect2) 
{
    int sensor1 = read_ADC1BUF(sensor1_buf);
    int sensor2 = read_ADC1BUF(sensor2_buf);
    
    if (sensor1 > detect1 && sensor2 > detect2)
    {
        return;
    }
    else if (sensor1 < detect1) //i will leave in the if and else if in case we can figure out turning rather than rotating
    {
        //turn toward dir1
        bitWord = dir2;
        fourBit_FSM();
        delay(140);
//        while (read_ADC1BUF(sensor1_buf) < detect1);
    }
    else if (sensor2 < detect2)
    {
        //turn toward dir2
        bitWord = dir1;
        fourBit_FSM();
        delay(140);
//        while (read_ADC1BUF(sensor2_buf) < detect2);
    }
    bitWord = orig_dir;
    fourBit_FSM();
}

void adjustCourse(int bitDir, int bufVal1, int bufVal2, int bufVal3, int thresh1 , int thresh2, int thresh3) {
    static int sensor1;
    static int sensor3;
    static int delayVal;
    
    sensor1 = read_ADC1BUF(bufVal1);
    sensor3 = read_ADC1BUF(bufVal3);
    delayVal = 100;
    
    
    if (sensor1 < thresh1) //i will leave in the if and else if in case we can figure out turning rather than rotating
    {
        bitWord = ROTATE_CW;
        fourBit_FSM();
        delay(delayVal);
    }
    else if (sensor3 < thresh3)
    {
        bitWord = ROTATE_CCW;
        fourBit_FSM();
        delay(delayVal);
    }
    else 
    {
        return;
    }

    bitWord = bitDir;
    fourBit_FSM(); 
    setTimer3(500);
    while(stateTimer3) {
        if (read_ADC1BUF(bufVal2) < thresh2 || read_ADC1BUF(1) < QRD_LOW) break;
    }
}

void locateTurn() {
     switch(bitWord) {
        case DRIVE_NORTH:
//            adjustProximity(DRIVE_NORTH, DRIVE_EAST, 12, E_WALL_COLLISION, DRIVE_WEST, 4, W_WALL_COLLISION);
//            adjustCourse(DRIVE_NORTH, 4, 3, 12, W_WALL_COLLISION , N_WALL_DETECT, E_WALL_COLLISION);
            if (SONAR_N < N_WALL_DETECT)
            {
                if (SONAR_E < E_WALL_DETECT)
                {
                    bitWord = DRIVE_WEST;
                }
                else if (SONAR_W < W_WALL_DETECT) 
                {
                    bitWord = DRIVE_EAST;
                }
                
            }
            break;
        case DRIVE_EAST:
//            adjustProximity(DRIVE_EAST, DRIVE_NORTH, 3, N_WALL_COLLISION, DRIVE_SOUTH, 13, S_WALL_COLLISION);
//            adjustCourse(DRIVE_EAST, 3, 12, 13, N_WALL_COLLISION , E_WALL_DETECT, S_WALL_COLLISION);
            if (SONAR_E < E_WALL_DETECT)
            {
                if (SONAR_N < N_WALL_DETECT) 
                {
                    bitWord = DRIVE_SOUTH;
                }
                else if (SONAR_S < S_WALL_DETECT)
                {
                    bitWord = DRIVE_NORTH;
                }
            }
            break;
        case DRIVE_SOUTH:
//            adjustProximity(DRIVE_SOUTH, DRIVE_EAST, 12, E_WALL_COLLISION, DRIVE_WEST, 4, W_WALL_COLLISION);
//            adjustCourse(DRIVE_SOUTH, 12, 13, 4, E_WALL_COLLISION , S_WALL_DETECT, W_WALL_COLLISION);
            if (SONAR_S < S_WALL_DETECT)
            {
                if (SONAR_W < W_WALL_DETECT) 
                {
                    bitWord = DRIVE_EAST;
                }
                else if (SONAR_E < E_WALL_DETECT)
                {
                    bitWord = DRIVE_WEST;
                }
            }
            break;
        case DRIVE_WEST:
//            adjustProximity(DRIVE_WEST, DRIVE_NORTH, 3, N_WALL_COLLISION, DRIVE_SOUTH, 13, S_WALL_COLLISION);
//            adjustCourse(DRIVE_WEST, 13, 4, 3, S_WALL_COLLISION , W_WALL_DETECT, N_WALL_COLLISION);
            if (SONAR_W < W_WALL_DETECT)
            {
                if (SONAR_N < N_WALL_DETECT) 
                {
                    bitWord = DRIVE_SOUTH;
                }
                else if (SONAR_S < S_WALL_DETECT)
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
            delay(50); // this delays the signal to ensure that the motor pic receives it
            bitWord = STRAIGHT;
            break;
        /* NOTE: ALL if statements used to be (qrd# == 2)... */
        case DECEL_STRAIGHT:
            bitWord = STRAIGHT;
            break;
        case STRAIGHT:
            if (qrd1)
            {
                if (qrd2) 
                {
                    bitWord = STRAIGHT;
                }
                else 
                {
                    bitWord = TURN_LEFT;   
                }
            }
            else if (qrd3)
            {
                if (qrd2) 
                {
                    bitWord = STRAIGHT;
                }
                else
                {
                    bitWord = TURN_RIGHT;
                }
            }
            
            break;
        case TURN_RIGHT:
            if (qrd2)
            {
                bitWord = STRAIGHT;
            }
            break;
            if (qrd1)
            {
                bitWord = TURN_LEFT;
            }
        case TURN_LEFT:
            if (qrd2)
            {
                bitWord = STRAIGHT;
            }
            if (qrd3)
            {
//                bitWord = STRAIGHT;
                bitWord = TURN_RIGHT;
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
    return (QRD_val < QRD_HIGH);
    
//    if (QRD_val / QRD_HIGH) {
//        return 0; // off the line
//    } else if (QRD_val / QRD_LOW) { // USED TO BE MEDIUM
//        return 1; // kinda on the line
//    } else {
//        return 2; // on the line
//    }
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
            sendWord(0, 1, 1, 1);
            break;
        case ROTATE_CW:
            sendWord(1, 0, 0, 0);
            break;
        case ROTATE_CCW:
            sendWord(1, 0, 0, 1);
            break;
        case ACCEL_STRAIGHT:
            sendWord(1, 0, 1, 0);
            break;
        case SLOW_MOTORS:
            sendWord(1, 0, 1, 1);
            break;
        case QUICKEN_MOTORS:
            sendWord(1, 1, 0, 0);
            break;
        case DECEL_STRAIGHT:
            sendWord(1, 1, 0, 1);
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

BOOL nestedCounter(int func, int threshhold, int numCounts, int numReps) {
    for (int i = 0; i < numReps; i++) {
        if (!counter(func, threshhold, numCounts)) return FALSE;
    }
    
    return TRUE;
}

BOOL counter(int func, int threshhold, int numCounts) { // add TRUE/FALSE functions as needed 
    long count = 0;
    
    for (int i = 0; i < 1000; i++) {
        for (int j = 0; j < numCounts; j++) {
            switch (func) {
                case 0:
                    count += isCanyonSensed();
                    break;
                case 1:
                    count += isCanyonExitSensed();
                    break;
                case 2:
                    count += isLanderSensed();
                    break;
            }            
        }
    }
    
    return (count >= 1000 * threshhold);
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