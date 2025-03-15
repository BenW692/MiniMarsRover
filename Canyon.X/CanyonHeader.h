/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> // include processor files - each processor file is guarded.  

#pragma config FNOSC = FRCDIV
#pragma config OSCIOFNC = OFF
#pragma config SOSCSRC = DIG

#define TRUE 1
#define FALSE 0
#define BOOL unsigned char

#define STOP 0b0000 // potentially put into header file with global var
#define STRAIGHT 0b0001 
#define SMALL_RIGHT 0b0010
#define MED_RIGHT 0b0011
#define BIG_RIGHT 0b0100
#define SMALL_LEFT 0b0101
#define MED_LEFT 0b0110
#define BIG_LEFT 0b0111

#define STRAIGHT_SPEED 2500
#define BIG_TURN_SPEED (STRAIGHT_SPEED * 2)
#define MED_TURN_SPEED (STRAIGHT_SPEED * 3)
#define SMALL_TURN_SPEED (STRAIGHT_SPEED * 4)


//this code has not incorporated seperate pwms for the left and right motor
#define PERIOD_1 OC3RS
#define DUTY_CYCLE_1 OC3R

/* Define Pins */
#define FRONT_DIR     _LATA0
#define BACK_DIR      _LATA1
#define LEFT_DIR      _LATB9
#define RIGHT_DIR     _LATB8


#define WORDBIT1 PORTBbits.RB15
#define WORDBIT2 PORTAbits.RB14 
#define WORDBIT3 PORTAbits.RB13
#define WORDBIT4 PORTAbits.RB12

int lineState = STRAIGHT;

/* END HEADER */

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

