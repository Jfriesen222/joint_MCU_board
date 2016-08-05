/* 
 * File:   PMSMBoard.h
 * Author: Pavlo
 *
 * Created on October 29, 2013, 4:55 PM
 */

#include <xc.h>
#include "DMA_Transfer.h"
#include "CircularBuffer.h"


/**
 * @brief Gate Duty Cycle Mapping
 */

#define pwmLED1       PDC5
#define pwmLED2       SDC5


#define pwmMotA1       SDC3
#define pwmMotB2       PDC2




#define SLNDMot_flt       PORTAbits.RA6

#define SLNDMot_flt_tris       TRISAbits.TRISA6

#define SLNDMot_flt_PU         CNPUAbits.CNPUA6



#define SCLpin     LATDbits.LATD10
#define SCLtri     TRISDbits.TRISD10
#define SDApin     LATDbits.LATD9
#define SDAtri     TRISDbits.TRISD9
//#define LED2     LATDbits.LATD5
//#define LED3     LATDbits.LATD6
//#define LED4     LATDbits.LATD7
//#define LED5     LATDbits.LATD2
#define LED6     LATBbits.LATB13
#define LED7     LATBbits.LATB14
#define TRIS_LED6     TRISBbits.TRISB13
#define TRIS_LED7     TRISBbits.TRISB14

void InitBoard(ADCBuffer *ADBuff, CircularBuffer *cB, CircularBuffer *spi_cB, void *eventCallback);

