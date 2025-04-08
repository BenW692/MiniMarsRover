/*
 * File:   NavConfigs.c
 * Author: benwo
 *
 * Created on March 24, 2025, 5:59 PM
 */


#include "xc.h"
#include "NavConfigs.h"

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
    //line following qrds
    _TRISA0 = 1;
    _TRISA1 = 1;
    _TRISB0 = 1;
    
    _TRISB15 = 1; //ball qrd
    _TRISB14 = 1; //lander qrd
    _TRISA3 = 1; //satellite diode
    
    _TRISB1 = 1; //north sonar
    _TRISB2 = 1; //west sensor
    _TRISA2 = 1; //sonar south
    _TRISB12 = 1; //sonar east
    
    /* Enable Analog */
    //line following qrds
    _ANSA0 = 1;
    _ANSA1 = 1;    
    _ANSB0 = 1;   
    
    _ANSB15 = 1; //ball qrd
    _ANSB14 = 1; //lander qrd
    _ANSA3 = 1; //satellite diode
    
    _ANSB1 = 1;
    _ANSB2 = 1;
    _ANSA2 = 1;
    _ANSB12 = 1;
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
    _SMPI = 9;    // begin new sampling sequence after 10 samples
    _ALTS = 0;    // sample MUXA only

    // AD1CON3
    _ADRC = 0;    // use system clock
    _SAMC = 1;    // sample every A/D period
    _ADCS = 32; // TAD = 64*TCY // SHOULD WE CHANGE THIS??

    // AD1CSS -- Choose which channel/pin to scan
    // Select AN0, AN1, AN2 (pins 2, 3, 4) && AN3, AN4, AN13, AN12 (pins 5, 6, 7, 15) and pin AN9 AN10 AN14 (18 17 8))
    AD1CSSL = 0b0111011000011111; 
    
    _ADON = 1;    // enable module after configuration
}

void config_Timers()
{
        // Configure a 16-bit timer using Timer1
    T1CONbits.TCS = 0;
    T1CONbits.TCKPS = 0b11; // 256 presclaler
    PR1 = 31250; // Period of 1 second
    T1CONbits.TON = 0; //bit is off
    
	// Configure Timer1 interrupt
	_T1IP = 4;	// Select interrupt priority
	_T1IF = 0;	// Clear interrupt flag
	_T1IE = 1;	// Enable interrupt
    TMR1 = 0;
    
    T2CONbits.TON = 1;
    T2CONbits.TCKPS = 0b01;
    T2CONbits.TCS = 0;
    PR2 = 9999;
}

void config_PWM() {
    
    /* Servo Set Up */
    OC1CON1 = 0;
    OC1CON2 = 0;
    OC1CON1bits.OCTSEL = 0b000; //output compare timer is Timer 2
    OC1CON1bits.OCM = 0b110; //edge aligned output
    OC1CON2bits.SYNCSEL = 0x1F;
    OC1CON2bits.OCTRIG = 0; //use this OC module
   
    SERVO_PERIOD = 9999;
    SERVO_ANGLE = MIDDLE_ANGLE; //sets it to the straight angle
}

void __attribute__((interrupt, no_auto_psv)) _T3Interrupt(void) {
    _T3IF = 0; // Clear flag
    T3CONbits.TON = 0; //bit is off
    _T3IE = 0; // Disable Interupt
    TMR3 = 0; // Clear Timer
    stateTimer3 = FALSE;
}