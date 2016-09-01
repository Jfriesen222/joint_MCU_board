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
#define MOTOR1_1R               SDC3                 
#define MOTOR2_1R               SDC4  


#define MOTOR1_2                PDC5                 
#define MOTOR2_2                PDC6
#define MOTOR1_2R               SDC5                 
#define MOTOR2_2R               SDC6  

#define MOTOR1_3                PDC2                 
#define MOTOR2_3                PDC1
#define MOTOR1_3R               SDC2                 
#define MOTOR2_3R               SDC1 

#define MOTOR0                  MOTOR1_1
#define MOTOR0R                 MOTOR1_1R
#define MOTOR1                  MOTOR2_3 
#define MOTOR1R                 MOTOR2_3R
#define MOTOR2                  MOTOR1_2   
#define MOTOR2R                 MOTOR1_2R
#define MOTOR3                  MOTOR2_1  
#define MOTOR3R                 MOTOR2_1R
#define MOTOR4                  MOTOR1_3      
#define MOTOR4R                 MOTOR1_3R
#define MOTOR5                  MOTOR2_2 
#define MOTOR5R                 MOTOR2_2R

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
#define CNPU_SW1_1              CNPUAbits.CNPUA5
#define CNPU_SW2_1              CNPUAbits.CNPUA4
#define CNPU_SW3_1              CNPUAbits.CNPUA3
#define CNPU_SW4_1              CNPUAbits.CNPUA2
#define SW1_1                   PORTAbits.RA5
#define SW2_1                   PORTAbits.RA4
#define SW3_1                   PORTAbits.RA3
#define SW4_1                   PORTAbits.RA2


#define TRIS_SW1_2              TRISBbits.TRISB11
#define TRIS_SW2_2              TRISBbits.TRISB10
#define TRIS_SW3_2              TRISBbits.TRISB9
#define TRIS_SW4_2              TRISBbits.TRISB8
#define CNPU_SW1_2              CNPUBbits.CNPUB11
#define CNPU_SW2_2              CNPUBbits.CNPUB10
#define CNPU_SW3_2              CNPUBbits.CNPUB9
#define CNPU_SW4_2              CNPUBbits.CNPUB8
#define SW1_2                   PORTBbits.RB11
#define SW2_2                   PORTBbits.RB10
#define SW3_2                   PORTBbits.RB9
#define SW4_2                   PORTBbits.RB8

#define TRIS_SW1_3              TRISFbits.TRISF1
#define TRIS_SW2_3              TRISFbits.TRISF0
#define TRIS_SW3_3              TRISAbits.TRISA7
#define TRIS_SW4_3              TRISAbits.TRISA6
#define CNPU_SW1_3              CNPUFbits.CNPUF1
#define CNPU_SW2_3              CNPUFbits.CNPUF0
#define CNPU_SW3_3              CNPUAbits.CNPUA7
#define CNPU_SW4_3              CNPUAbits.CNPUA6
#define SW1_3                   PORTFbits.RF1
#define SW2_3                   PORTFbits.RF0
#define SW3_3                   PORTAbits.RA7
#define SW4_3                   PORTAbits.RA6

#define S_SA1                     PORTAbits.RA6
#define S_SF1                     PORTAbits.RA7
#define S_SA2                     PORTAbits.RA4
#define S_SF2                     PORTAbits.RA5
#define S_SA3                     PORTAbits.RA2
#define S_SF3                     PORTAbits.RA3
#define S_SA4                     PORTBbits.RB10
#define S_SF4                     PORTBbits.RB11
#define S_SA5                     PORTBbits.RB8
#define S_SF5                     PORTBbits.RB9
#define S_SA6                     PORTFbits.RF0
#define S_SF6                     PORTFbits.RF1

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
#define CS6_1                   LATAbits.LATA15
#define ODC_CS1_1               ODCDbits.ODCD0
#define ODC_CS2_1               ODCDbits.ODCD11
#define ODC_CS3_1               ODCDbits.ODCD10
#define ODC_CS4_1               ODCDbits.ODCD9
#define ODC_CS5_1               ODCDbits.ODCD8
#define ODC_CS6_1               ODCAbits.ODCA15

#define TRIS_CS1_2              TRISAbits.TRISA0
#define TRIS_CS2_2              TRISDbits.TRISD15
#define TRIS_CS3_2              TRISDbits.TRISD14
#define TRIS_CS4_2              TRISFbits.TRISF12
#define TRIS_CS5_2              TRISFbits.TRISF13
#define TRIS_CS6_2              TRISAbits.TRISA1
#define CS1_2                   LATAbits.LATA0
#define CS2_2                   LATDbits.LATD15
#define CS3_2                   LATDbits.LATD14
#define CS4_2                   LATFbits.LATF12
#define CS5_2                   LATFbits.LATF13
#define CS6_2                   LATAbits.LATA1
#define ODC_CS1_2               ODCAbits.ODCA0
#define ODC_CS2_2               ODCDbits.ODCD15
#define ODC_CS3_2               ODCDbits.ODCD14
#define ODC_CS4_2               ODCFbits.ODCF12
#define ODC_CS5_2               ODCFbits.ODCF13
#define ODC_CS6_2               ODCAbits.ODCA1

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
#define ODC_CS1_3               ODCGbits.ODCG15
#define ODC_CS2_3               ODCGbits.ODCG13
#define ODC_CS3_3               ODCGbits.ODCG12
#define ODC_CS4_3               ODCGbits.ODCG14
#define ODC_CS5_3               ODCGbits.ODCG0
#define ODC_CS6_3               ODCGbits.ODCG1

#define RL1_1                     0b1111111111111110
#define RL4_1                     0b1111111111111101
#define SA2_1                     0b1111111111111011
#define SF2_1                     0b1111111111110111
#define SA3_1                     0b1111111111101111
#define SF3_1                     0b1111111111011111
#define RL3_1                     0b1111111110111111
#define RL6_1                     0b1111111101111111
#define SA4_1                     0b1111111011111111
#define SF4_1                     0b1111110111111111
#define SA5_1                     0b1111101111111111
#define SF5_1                     0b1111011111111111
#define RL5_1                     0b1110111111111111
#define RL2_1                     0b1101111111111111
#define SA6_1                     0b1011111111111111
#define SF6_1                     0b0111111111111111
#define SA1_2                     0b1111111111111110
#define SF1_2                     0b1111111111111101




#define ALL_CS_LOW              0x0
#define ALL_CS_HIGH             0xFFFF

#define SF_ODD_1                SF3_1&SF5_1
#define SF_ODD_2                SF1_2
#define SF_EVEN_1               SF2_1&SF4_1&SF6_1 
#define SF_EVEN_2               ALL_CS_HIGH
#define SA_ODD_1                SA3_1&SA5_1
#define SA_ODD_2                SA1_2
#define SA_EVEN_1               SA2_1&SA4_1&SA6_1
#define SA_EVEN_2               ALL_CS_HIGH
#define RL_ODD_1                RL1_1&RL3_1&RL5_1
#define RL_ODD_2                ALL_CS_HIGH
#define RL_EVEN_1               RL2_1&RL4_1&RL6_1
#define RL_EVEN_2               ALL_CS_HIGH


/* Reset Pins */
#define TRIS_RESET_1            TRISAbits.TRISA14
#define RESET_1                 LATAbits.LATA14

#define TRIS_RESET_2            TRISAbits.TRISA10
#define RESET_2                 LATAbits.LATA10

#define TRIS_RESET_3            TRISDbits.TRISD7
#define RESET_3                 LATDbits.LATD7




#define STRING_OFFSET_0 356700
#define STRING_OFFSET_1 347800
#define STRING_OFFSET_2 362800
#define STRING_OFFSET_3 352100
#define STRING_OFFSET_4 352800
#define STRING_OFFSET_5 356000




typedef struct{
    long int RL_ENCDR[6][3];
    int SF_ENCDR[6][3];
    int SA_ENCDR[6][3];
    long int RL_VEL[6];
    int SF_VEL[6];
    int SA_VEL[6];
} Robot_Encoders; 

typedef struct{
    uint8_t SF[6];
    uint8_t SA[6];
} Robot_Switches; 


void InitBoard(ADCBuffer *ADBuff, CircularBuffer *cB, CircularBuffer *spi_cB, void *eventCallback);
void selectCS(uint16_t cs_bits1, uint16_t cs_bits2 );
void readSwitches(Robot_Switches *robot_switches);
bool checkSPIbus();
void haltAndCatchFire(unsigned int *message);
void setMotors(int *duty_cycle);