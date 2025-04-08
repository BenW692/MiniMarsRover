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
    config_Timers();
    config_PWM();
    
    delay(1000); //we need this delay so the ADC can configure
    //at boot up it returns a random value for the sonar sensors that will trigger ball
    //drop if we don't have this delay
    
    
    bitWord = STRAIGHT;
    fourBit_FSM();
    
    
    while (TRUE)
    {
        senseLine();
        fourBit_FSM();
        pollDrop(); // we should have state machine that controls which checkpoints we poll
        pollLander();
//        if(isCanyonSensed)
//        {
//            bitWord = DRIVE_NORTH;
//            while (TRUE)
//            {
//                locateTurn();
//                fourBit_FSM();
//            }
//        }
        
    }
//    while (1)
//    {
//        if (BALL_QRD < 100)
//        {
//            SERVO_ANGLE = WHITE_ANGLE;
////            WORDBIT3 = 1;
////            WORDBIT4 = 0;
//            for (double i = 0; i<100000; i++);
//        }
//        else 
//        {
//            SERVO_ANGLE = BLACK_ANGLE;
////            WORDBIT4 = 1;
////            WORDBIT3 = 0;
//            for (double i = 0; i<100000; i++);
//        }
//        SERVO_ANGLE = MIDDLE_ANGLE;
//        for (double i = 0; i<100000; i++);
//        for (double i = 0; i<100000; i++);
//        for (double i = 0; i<100000; i++);
//    }
//    WORDBIT1 = 1;
//    WORDBIT2 = 1;
//    WORDBIT3 = 1;
//    WORDBIT4 = 1;
//    
//    while (1) {
//        if (SONAR_N < N_BALL_DROP_DETECT && SONAR_N > 0) 
//        {
//            WORDBIT1 = 0;
//        }
//        else
//        {
//            WORDBIT1 = 1;
//        }
//        if (SONAR_E < E_BALL_DROP_DETECT && SONAR_E > 0) 
//        {
//            WORDBIT2 = 0;
//        }
//        else
//        {
//            WORDBIT2 = 1;
//        }
//        if (SONAR_S < S_BALL_DROP_DETECT && SONAR_S > 0) 
//        {
//            WORDBIT3 = 0;
//        }
//        else
//        {
//            WORDBIT3 = 1;
//        }
//        if (SONAR_W < W_BALL_DROP_DETECT && SONAR_W > 0) 
//        {
//            WORDBIT4 = 0;
//        }
//        else
//        {
//            WORDBIT4 = 1;
//        }
//    }
//        
//    }
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
    
//    WORDBIT3 = 0;
//    WORDBIT4 = 0;
//    
//    while (TRUE)
//    {
//        if (BALL_QRD < QRD_HIGH)
//        {
//            ball_color = 0; //ball is white
//            WORDBIT3 = 1;
//        }
//        else
//        {
//            ball_color = 1; //ball is black
//            WORDBIT4 = 1;
//            
//        SERVO_PERIOD = 9999;
//        if (ball_color) //black
//        {
//            SERVO_ANGLE = BLACK_ANGLE; //tip it right (not sure about this angle)
//        }
//        else
//        {
//            SERVO_ANGLE = WHITE_ANGLE;
//        } 
//    }
//        while (1);
//}
}
    
