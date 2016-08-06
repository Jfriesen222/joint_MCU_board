#include "MCU_Board.h"
#include <timer.h>
#include <uart.h>
#include <pps.h>
#include <p33ep256mu810.h>

_FOSCSEL(FNOSC_FRC & IESO_OFF);
_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF & POSCMD_NONE);
_FPOR(ALTI2C1_ON)
_FWDT(FWDTEN_OFF);
_FICD(ICS_PGD1 & JTAGEN_OFF);


void UART2Init(void);
void ClockInit(void);
void MotorInit(void);
void TimersInit(void);
void PinInit(void);
void EventCheckInit(void *eventCallback);
void ADCInit(void);

void (*eventCallbackFcn)(void);

void InitBoard(ADCBuffer *ADBuff, CircularBuffer *cB, CircularBuffer *spi_cB, void *eventCallback) {

    ClockInit();
    PinInit();
    MotorInit();
    UART2Init();
   // ADCInit();

    DMA1_UART2_Enable_RX(cB);
    DMA6_ADC_Enable(ADBuff);


    EventCheckInit(eventCallback);
    TimersInit();
}

void MotorInit() {
    /* Set PWM Period on Primary Time Base */
    PTPER = 1000;
    /* Set Duty Cycles */

    /* Set Dead Time Values */
    DTR1 = DTR2 = DTR3 = DTR4 = DTR5 = DTR6 = 0;
    ALTDTR1 = ALTDTR2 = ALTDTR3 = ALTDTR4 = ALTDTR5 = ALTDTR6 = 0;
    /* Set PWM Mode to Complementary */
    IOCON1 = IOCON2 = IOCON3 = IOCON4 = IOCON5 = IOCON6 = 0xC000;
    /* Set Primary Time Base, Edge-Aligned Mode and Independent Duty Cycles */
    PWMCON1 =  PWMCON3 = PWMCON4 = PWMCON5 = PWMCON6 = 0x0000;
    /* Configure Faults */
    FCLCON1 =  FCLCON3 = FCLCON4 = FCLCON5 = FCLCON6 = 0x0003;
    /* 1:1 Prescaler */
    /* Enable PWM Module */
    PTCON = 0x8000;

}

void UART2Init(void) {
    U2MODEbits.STSEL = 0; // 1-stop bit
    U2MODEbits.PDSEL = 0; // No parity, 8-data bits
    U2MODEbits.ABAUD = 0; // Auto-baud disabled
    U2MODEbits.BRGH = 0; // High speed UART mode...
    U2BRG = 37; //455 for 9600,227 for 19200, 113 for 38400,  37 for 115200 on BRGH 0, 460800 on BRGH 1, 921600 = 19
    //BRGH = 0, BRG = 18 for 230400
    U2STAbits.UTXISEL0 = 0; // int on last character shifted out tx register
    U2STAbits.UTXISEL1 = 0; // int on last character shifted out tx register
    U2STAbits.URXISEL = 0; // Interrupt after one RX character is received
    U2MODEbits.UARTEN = 1; // Enable UART
    U2STAbits.UTXEN = 1; // Enable UART TX
}

void ClockInit(void) {
    // 140.03 MHz VCO  -- 70 MIPS
    PLLFBD = 74;
    CLKDIVbits.PLLPRE = 0;
    CLKDIVbits.PLLPOST = 0;
    // Initiate Clock Switch to FRC oscillator with PLL (NOSC=0b001)
    __builtin_write_OSCCONH(0x01);

    __builtin_write_OSCCONL(OSCCON | 0x01);

    // Wait for Clock switch to occur
    while (OSCCONbits.COSC != 0b001);

    // Wait for PLL to lock
    while (OSCCONbits.LOCK != 1);
}

void PinInit(void) {
    //Right now no analog peripherals are being used, so we let digital
    //peripherals take over.
    ANSELA = 0;
    ANSELB = 0;
    ANSELC = 0;
    ANSELD = 0;
    ANSELE = 0;
    ANSELG = 0;
    
    /* initialize LED pins as outputs */
    TRIS_LED1 = 0;
    TRIS_LED2 = 0;
    TRIS_LED3 = 0;
    TRIS_LED4 = 0;
    
    /* initialize CS pins as outputs */
    TRIS_CS1_1 = 0;
    TRIS_CS2_1 = 0;
    TRIS_CS3_1 = 0;
    TRIS_CS4_1 = 0;
    TRIS_CS5_1 = 0;
    TRIS_CS6_1 = 0;
    
    TRIS_CS1_2 = 0;
    TRIS_CS2_2 = 0;
    TRIS_CS3_2 = 0;
    TRIS_CS4_2 = 0;
    TRIS_CS5_2 = 0;
    TRIS_CS6_2 = 0;
    
    TRIS_CS1_3 = 0;
    TRIS_CS2_3 = 0;
    TRIS_CS3_3 = 0;
    TRIS_CS4_3 = 0;
    TRIS_CS5_3 = 0;
    TRIS_CS6_3 = 0;
    
    /* initialize switch pins as inputs */
    TRIS_SW1_1 = 1;
    TRIS_SW2_1 = 1;
    TRIS_SW3_1 = 1;
    TRIS_SW4_1 = 1;
    
    TRIS_SW1_2 = 1;
    TRIS_SW2_2 = 1;
    TRIS_SW3_2 = 1;
    TRIS_SW4_2 = 1;
    
    TRIS_SW1_3 = 1;
    TRIS_SW2_3 = 1;
    TRIS_SW3_3 = 1;
    TRIS_SW4_3 = 1;
    
//    //Set up Change Notify Interrupt
//    //These correspond to your change hall effect pins.
//    CNENCbits.CNIEC13 = 1; // Enable RC14 pin for interrupt detection

    IEC1bits.CNIE = 1; // Enable CN interrupts
    IFS1bits.CNIF = 0; // Reset CN interrupt

    //Unlock PPS Registers
    __builtin_write_OSCCONL(OSCCON & ~(1 << 6));

    OUT_PIN_PPS_RP82= OUT_FN_PPS_U2TX; //U2Tx
    IN_FN_PPS_U2RX = IN_PIN_PPS_RP125; //U2Rx

    //Lock PPS Registers
    __builtin_write_OSCCONL(OSCCON | (1 << 6));

}

void TimersInit(void) {
    T7CONbits.TON = 0;
    T7CONbits.TCS = 0;
    T7CONbits.TGATE = 0;
    T7CONbits.TCKPS = 0b11; // Select 1:256 Prescaler
    TMR7 = 0x00;
    PR7 = 0x00112; //Approximately 5kHz... 0x0112 For 1kHz  0x0037 for 5kHz
    IPC12bits.T7IP = 0x01;
    IFS3bits.T7IF = 0;
    IEC3bits.T7IE = 1;
    T7CONbits.TON = 1;

    TMR5 = 0x0000;
    T5CONbits.TCKPS = 3;
    PR5 = 60000; //68; // Trigger ADC1at a rate of 4kHz
    IFS1bits.T5IF = 0; // Clear Timer5 interrupt
    IEC1bits.T5IE = 0; // Disable Timer5 interrupt
    T5CONbits.TON = 1; // Start Timer5

    T3CONbits.TON = 0;
    T3CONbits.TCKPS = 0b11; // Select 1:256 Prescaler
    TMR3 = 0x0000;
    PR3 = 60000; // Trigger ADC1 every 125 ?s @ 40 MIPS
    IFS0bits.T3IF = 0; // Clear Timer3 interrupt
    IEC0bits.T3IE = 0; // Disable Timer3 interrupt
    T3CONbits.TON = 1;
}

void ADCInit(void) {
    //ANSELBbits.ANSB14 = 1; //AN14
    //ANSELBbits.ANSB15 = 1; //AN15
    //ANSELBbits.ANSB13 = 1; //AN13
    //ANSELAbits.ANSA7 = 1; //AN23
    
    //Setup ADC1 for Channel 0-3 sampling
    AD1CON2bits.VCFG = 0;
    AD1CON1bits.FORM = 0; //Data Output Format : Integer Output
    AD1CON1bits.SSRCG = 1; //
    //AD1CON1bits.SSRC = 0; //Sample Clock Source : PWM Generator 1 primary trigger compare ends sampling
    AD1CON1bits.SSRC = 4; //Sample Clock Source : GP Timer5 starts conversion
    AD1CON1bits.ASAM = 1; // Sampling begins immediately after conversion
    AD1CON1bits.AD12B = 1; // 12-bit ADC operation
    AD1CON1bits.SIMSAM = 0; // Samples channel 0;

    AD1CON2bits.BUFM = 0;
    AD1CON2bits.CSCNA = 1; // Scan CH0+ Input Selections during Sample A bit
    AD1CON2bits.CHPS = 0; // Converts CH0  //This got changed

    AD1CON3bits.ADRC = 0; // ADC clock is derived from systems clock

    /*
     * ADCS is the main clock multiplier.  The result should be >= 1.6uS.
     * In a 40MHz processor the base period is 25nS.  64 * 25nS = 1.6uS.
     */
    AD1CON3bits.ADCS = 448; // 112 * 14.3nS = 1.6uS -- Conversion Clock
    AD1CON4bits.ADDMAEN = 1; // DMA Enable

    //AD1CHS0: Analog-to-Digital Input Select Register
    AD1CHS0bits.CH0SA = 0; // MUXA +ve input selection (AIN0) for CH0
    AD1CHS0bits.CH0NA = 0; // MUXA -ve input selection (VREF- if 0 AN1 if 1) for CH0
    //AD1CHS123: Analog-to-Digital Input Select Register
    AD1CHS123bits.CH123SA = 0; // MUXA +ve input selection (AIN0) for CH1
    AD1CHS123bits.CH123NA = 0; // MUXA -ve input selection (VREF- if 0 AN1 if 1) for CH1

    //AD1CSSH/AD1CSSL: Analog-to-Digital Input Scan Selection Register
    AD1CSSL = 0x0000;
    AD1CSSH = 0x0000;
    //AD1CSSLbits.CSS1 = 1;  //PVDD MONITORING
    AD1CSSLbits.CSS15 = 1; 
    AD1CSSLbits.CSS14 = 1;
    AD1CSSLbits.CSS13 = 1;
   //AD1CSSLbits.CSS9 = 1;
    //AD1CSSLbits.CSS11 = 1; 
    AD1CSSHbits.CSS23 = 1; //battery sense
    //AD1CSSLbits.CSS13 = 1;

    AD1CON1bits.ADDMABM = 0; // DMA buffers are built in scatter/gather mode
    AD1CON2bits.SMPI = 4; // 2 ADC buffers
    AD1CON4bits.DMABL = 0b110; // Allocate one word of buffer per input.
    IFS0bits.AD1IF = 0; // Clear Analog-to-Digital Interrupt Flag bit
    IEC0bits.AD1IE = 0; // Do Not Enable Analog-to-Digital interrupt
    AD1CON1bits.ADON = 1; // Turn on the ADC
}

void EventCheckInit(void *eventCallback) {
    eventCallbackFcn = eventCallback;
}

void __attribute__((__interrupt__, no_auto_psv)) _T7Interrupt(void) {
    eventCallbackFcn();
    IFS3bits.T7IF = 0; // Clear Timer1 Interrupt Flag
}