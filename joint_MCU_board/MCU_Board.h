/* 
 * File:   PMSMBoard.h
 * Author: John Dean
 *
 * Created on August 8, 2016
 */

#include <xc.h>
#include "DMA_Transfer.h"
#include "CircularBuffer.h"

/* Motor PWM */
#define MOTOR1_1                PDC3                 
#define MOTOR2_1                PDC4                 

#define MOTOR1_2                PDC5                 
#define MOTOR2_2                PDC6   

#define MOTOR1_3                PDC2                 
#define MOTOR2_3                PDC1

/* LEDs */
#define TRIS_LED1               TRISBbits.TRISB15
#define TRIS_LED2               TRISBbits.TRISB14
#define TRIS_LED3               TRISBbits.TRISB13
#define TRIS_LED4               TRISBbits.TRISB12
#define LED1                    LATBbits.LATB15
#define LED2                    LATBbits.LATB14
#define LED3                    LATBbits.LATB13
#define LED4                    LATBbits.LATB12


/* Motor Switches */
#define TRIS_SW1_1              TRISAbits.TRISA5
#define TRIS_SW2_1              TRISAbits.TRISA4
#define TRIS_SW3_1              TRISAbits.TRISA3
#define TRIS_SW4_1              TRISAbits.TRISA2
#define SW1_1                   PORTAbits.RA5
#define SW2_1                   PORTAbits.RA4
#define SW3_1                   PORTAbits.RA3
#define SW4_1                   PORTAbits.RA2

#define TRIS_SW1_2              TRISBbits.TRISB11
#define TRIS_SW2_2              TRISBbits.TRISB10
#define TRIS_SW3_2              TRISBbits.TRISB9
#define TRIS_SW4_2              TRISBbits.TRISB8
#define SW1_2                   PORTBbits.RB11
#define SW2_2                   PORTBbits.RB10
#define SW3_2                   PORTBbits.RB9
#define SW4_2                   PORTBbits.RB8

#define TRIS_SW1_3              TRISFbits.TRISF1
#define TRIS_SW2_3              TRISFbits.TRISF0
#define TRIS_SW3_3              TRISAbits.TRISA7
#define TRIS_SW4_3              TRISAbits.TRISA6
#define SW1_3                   TRISFbits.RF1
#define SW2_3                   TRISFbits.RF0
#define SW3_3                   TRISAbits.RA7
#define SW4_3                   TRISAbits.RA6


/* Chip Select */
#define TRIS_CS1_1              TRISDbits.TRISD0
#define TRIS_CS2_1              TRISDbits.TRISD11
#define TRIS_CS3_1              TRISDbits.TRISD10
#define TRIS_CS4_1              TRISDbits.TRISD9
#define TRIS_CS5_1              TRISDbits.TRISD8
#define TRIS_CS6_1              TRISAbits.TRISA15
#define CS1_1                   LATDbits.LATD0
#define CS2_1                   LATDbits.LATD11
#define CS3_1                   LATDbits.LATD10
#define CS4_1                   LATDbits.LATD9
#define CS5_1                   LATDbits.LATD8
#define CS6_1                   LATAbits.LATAA5

#define TRIS_CS1_2              TRISFbits.TRISF4
#define TRIS_CS2_2              TRISDbits.TRISD15
#define TRIS_CS3_2              TRISDbits.TRISD14
#define TRIS_CS4_2              TRISFbits.TRISF12
#define TRIS_CS5_2              TRISFbits.TRISF13
#define TRIS_CS6_2              TRISAbits.TRISA1
#define CS1_2                   LATFbits.LATF4
#define CS2_2                   LATDbits.LATD15
#define CS3_2                   LATDbits.LATD14
#define CS4_2                   LATFbits.LATF12
#define CS5_2                   LATFbits.LATF13
#define CS6_2                   LATAbits.LATA1

#define TRIS_CS1_3              TRISGbits.TRISG15
#define TRIS_CS2_3              TRISGbits.TRISG13
#define TRIS_CS3_3              TRISGbits.TRISG12
#define TRIS_CS4_3              TRISGbits.TRISG14
#define TRIS_CS5_3              TRISGbits.TRISG0
#define TRIS_CS6_3              TRISGbits.TRISG1
#define CS1_3                   LATGbits.LATG15
#define CS2_3                   LATGbits.LATG13
#define CS3_3                   LATGbits.LATG12
#define CS4_3                   LATGbits.LATG14
#define CS5_3                   LATGbits.LATG0
#define CS6_3                   LATGbits.LATG1


/* Reset Pins */
#define TRIS_RESET_1            TRISCbits.TRISC15
#define RESET_1                 LATCbits.LATC15

#define TRIS_RESET_2            TRISAbits.TRISA10
#define RESET_2                 LATAbits.LATA10

#define TRIS_RESET_3            TRISDbits.TRISD7
#define RESET_3                 LATDbits.LATD7




void InitBoard(ADCBuffer *ADBuff, CircularBuffer *cB, CircularBuffer *spi_cB, void *eventCallback);

