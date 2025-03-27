/*
 * File:   Navigation.c
 * Author: jss265
 *
 * Created on March 15, 2025, 3:09 PM
 */


#include "xc.h"
#include "NavConfigs.h"
#include "NavFunctions.h"
#include "NavInterrupts.h"

#pragma config FNOSC = FRCDIV // 8 MHz osccilator 
#pragma config OSCIOFNC = OFF
#pragma config SOSCSRC = DIG


int main(void) {
    
    _RCDIV = 0;
    pinSetup();
    config_ADC();
    config_Timer();
    config_PWM();
    
    WORDBIT3 = 0;
    WORDBIT4 = 1;
    while(1);
    while (1) {
        if (ADC1BUF9 < 500) {
            WORDBIT3 = 1;
        }
    }
//    bitWord = DRIVE_NORTH;
//    while (TRUE) { //defaults to canyon mode test
//        locateTurn();
//        fourBit_FSM();
//    }
    
    

////    
//    while (TRUE) //defaults to line following test
//    {
////        senseLine();
//        WORDBIT1 = 1;
//        WORDBIT2 = 1;
//        WORDBIT3 = 1;
//        WORDBIT4 = 1;
//        if (SONAR_N < SONAR_HIGH) //&& SONAR_W < SONAR_HIGH QRD2 < QRD_MED
//        {
//            WORDBIT1 = 0;
//            WORDBIT2 = 0;
//            WORDBIT3 = 0;
//            WORDBIT4 = 0;
////            dropBall();
//        }
////        if (SONAR_E < SONAR_HIGH) //&& SONAR_W < SONAR_HIGH QRD2 < QRD_MED
////        {
////            WORDBIT1 = 0;
////            WORDBIT2 = 0;
////            WORDBIT3 = 0;
////            WORDBIT4 = 0;
//////            dropBall();
//        }
////        fourBit_FSM();
////    }
    
    
//    
//    while(TRUE) {
//        
//        if (isCanyonSensed())
//        {
//            locateTurn();
//        }
//        else if (isDropSensed())
//        {
//            dropBall();
//        }
//        else    
//        {
//            senseLine(); //line following
//        }
//        
//        fourBit_FSM();
//}
    
    WORDBIT3 = 0;
    WORDBIT4 = 0;
    
    while (TRUE)
    {
        if (BALL_QRD < QRD_HIGH)
        {
            ball_color = 0; //ball is white
            WORDBIT3 = 1;
        }
        else
        {
            ball_color = 1; //ball is black
            WORDBIT4 = 1;
            
        SERVO_PERIOD = 9999;
        if (ball_color) //black
        {
            SERVO_ANGLE = BLACK_ANGLE; //tip it right (not sure about this angle)
        }
        else
        {
            SERVO_ANGLE = WHITE_ANGLE;
        } 
    }
        while (1);
}
}
    
