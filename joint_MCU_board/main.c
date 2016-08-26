/*
 * File:   Soft_Robot_Board
 * Authors: Pavlo Milo Manovi and Jeffrey Friesen
 *
 *
 */
#include <xc.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <uart.h>
#include <math.h>
#include "MCU_Board.h"
#include "DMA_Transfer.h"
#include "CircularBuffer.h"
#include "NMEAProtocol.h"
#include "I2CdsPIC.h"
#include "LS7366R_SPI.h"
#include <p33Exxxx.h>

CircularBuffer uartBuffer;
uint8_t uartBuf[64];
CircularBuffer canBuffer;
uint8_t canBuf[64];
CircularBuffer spiBuffer;
uint16_t spiBuf[64];

EncoderCts EncCts1;
EncoderCts EncCts2;
EncoderCts EncCts3;
EncoderCts EncCts4;
EncoderCts EncCts5;
EncoderCts EncCts6;

uint16_t events;
bool controller;
ADCBuffer ADCBuff;
long int iii = 0;
int jj = 1;
int jjj = 0;
float flex1 = 0;
float flex2 = 0;
actuatorCommands commandSet;

enum {
    EVENT_UART_DATA_READY = 0x01,
    EVENT_CAN_RX = 0x02,
    EVENT_SPI_RX = 0x04,
    EVENT_REPORT_FAULT = 0x08,
    EVENT_UPDATE_SPEED = 0x10,
    EVENT_ADC_DATA = 0x20,
    EVENT_UPDATE_LENGTHS = 0x40
};

void EventChecker(void);
void SetMotors(void);

int main(void) {
    commandSet.cmd1 = 0;
    commandSet.cmd2 = 0;
    commandSet.cmd3 = 0;
    commandSet.cmd4 = 0;
    commandSet.cmd5 = 0;
    static uint8_t out[300];
    static uint8_t size;
    static uint8_t size2;
    CB_Init(&uartBuffer, uartBuf, 32);
    CB_Init(&spiBuffer, (uint8_t *) spiBuf, 128);
    InitBoard(&ADCBuff, &uartBuffer, &spiBuffer, EventChecker);
    
    config_spi_slow();
    
    
    selectCS(ALL_CS_LOW);
    setQuadX4();
    selectCS(ALL_CS_HIGH);

    selectCS(ALL_CS_LOW);
    writeDTRtoZeros();
    selectCS(ALL_CS_HIGH);
    
    selectCS(0);
    setCNTRtoDTR();
    selectCS(0x3FFFF);
    
    



    putsUART2((unsigned int *) "Init. Complete\r\n");
    controller = 0;
    SetMotors();

    InitDecoder(&commandSet);
    //Pass pointer to com protocol


    while (1) {
        if (events & EVENT_UPDATE_SPEED) {

            /******************************************************************************
             * Function:        void Delay(void))
             *
             * PreCondition:    None   
             *
             * Input:           None
             *                  
             * Output:          None
             *
             * Side Effects:    None
             *
             * Overview:        This function provides sofware Delay
             *****************************************************************************/



            RESET_3 = 1;
            RESET_2 = 1;
            RESET_1 = 1;
            iii++;
            int amplitude;
            float freq;
            freq = 200.0;
            amplitude = 100;
            MOTOR1_3 = (int) (amplitude * sin(iii / freq) + PTPER/2);
            MOTOR2_3 = (int) (amplitude * sin(iii / freq) + PTPER/2);
            MOTOR1_1 = (int) (amplitude * sin(iii / freq) + PTPER/2);
            MOTOR2_1 = (int) (amplitude * sin(iii / freq) + PTPER/2);
            MOTOR1_2 = (int) (amplitude * sin(iii / freq) + PTPER/2);
            MOTOR2_2 = (int) (amplitude * sin(iii / freq) + PTPER/2);
            if (iii % 500 == 0) {
                jjj++;
            }

            if (iii % 100 == 0) {
                //size = sprintf((char *) out, "%i,%i,%i,%i,%i,%i,%i,\r\n", (int) (10000 * quaternion[0]), (int) (10000 * quaternion[1]), (int) (10000 * quaternion[2]), (int) (10000 * quaternion[3]), (int) flex1, (int) flex2, (int) ADCBuff.Adc1Data[3]);
                //size = sprintf((char *) out, "%5i  %5i  %5i  %5i  %5i  %5i  %5i  %5i  %5i\r\n", (int) (10 * imuData.accelX), (int) (10 * imuData.accelY), (int) (10 * imuData.accelZ), (int) (imuData.gyroX), (int) (imuData.gyroY), (int) (imuData.gyroZ), (int) (10 * imuData.magX), (int) (10 * imuData.magY), (int) (10 * imuData.magZ));
//                CS1_1 = CS1_2  = CS1_3 = 0;
//                readEnc(&EncCts1);
//                CS1_1 = CS1_2  = CS1_3 = 1;
//                CS2_1 = CS2_2  = CS2_3 = 0;
//                readEnc(&EncCts2);
//                CS2_1 = CS2_2  = CS2_3 = 1;
//                CS3_1 = CS3_2  = CS3_3 = 0;
//                readEnc(&EncCts3);
//                CS3_1 = CS3_2  = CS3_3 = 1;
//                CS4_1 = CS4_2  = CS4_3 = 0;
//                readEnc(&EncCts4);
//                CS4_1 = CS4_2  = CS4_3 = 1;
//                CS5_1 = CS5_2  = CS5_3 = 0;
//                readEnc(&EncCts5);
//                CS5_1 = CS5_2  = CS5_3 = 1;
//                CS6_1 = CS6_2  = CS6_3 = 0;
//                readEnc(&EncCts6);
//                CS6_1 = CS6_2  = CS6_3 = 1;
//              
                LED1 = 1;
                selectCS(RL_ODD);
                readEnc(&EncCts1);
                selectCS(ALL_CS_HIGH);
                selectCS(RL_EVEN);
                readEnc(&EncCts2);
                selectCS(ALL_CS_HIGH);
                selectCS(SA_EVEN);
                readEnc(&EncCts3);
                selectCS(ALL_CS_HIGH);
                selectCS(SF_EVEN);
                readEnc(&EncCts4);
                selectCS(ALL_CS_HIGH);
                selectCS(SA_ODD);
                readEnc(&EncCts5);
                selectCS(ALL_CS_HIGH);
                selectCS(SF_ODD);
                readEnc(&EncCts6);
                selectCS(ALL_CS_HIGH);
                LED1 = 0;
                
                

                size = sprintf((char *) out, "1: %10ld %10ld %10ld %10ld %10ld %10ld  2: %10ld %10ld %10ld %10ld %10ld %10ld  3: %10ld %10ld %10ld %10ld %10ld %10ld \r\n", 
                        EncCts1.cts1 , EncCts2.cts1 , EncCts3.cts1 , EncCts4.cts1 , EncCts5.cts1 , EncCts6.cts1 ,
                        EncCts1.cts2 , EncCts2.cts2 , EncCts3.cts2 , EncCts4.cts2 , EncCts5.cts2 , EncCts6.cts2 ,
                        EncCts1.cts3 , EncCts2.cts3 , EncCts3.cts3 , EncCts4.cts3 , EncCts5.cts3 , EncCts6.cts3);
                
//                size = sprintf((char *)out, "1: %5i %5i %5i %5i 2: %5i %5i %5i %5i 3: %5i %5i %5i %5i \r\n",
//                        SW1_1 , SW2_1 , SW3_1 , SW4_1, 
//                        SW1_2 , SW2_2 , SW3_2 , SW4_2,
//                        SW1_3 , SW2_3 , SW3_3 , SW4_3);
                
                DMA0_UART2_Transfer(size, out);

                //jj = (jj + (jj&0b10000)>>4)^0b10001;

                //LED1 = (jj & 0b1);
                LED1 = SW4_2;
                LED2 = (jj & 0b10) >> 1;
                LED3 = (jj & 0b100) >> 2;
                LED4 = (jj & 0b1000) >> 3;
                jj = ((jj << 1));
                jj = jj == 0b10000 ? 1 : jj;
            }
            if (iii % 5 == 1) {

                //IMU_GetData(&mpuData, &magData);

            }
            //SetMotors();
            events &= ~EVENT_UPDATE_SPEED;
        }

        if (events & EVENT_UART_DATA_READY) {
            uint8_t data;
            CB_ReadByte(&uartBuffer, &data);
            //size = sprintf(out, "%c", data);
            //DMA0_UART2_Transfer(size, out);
            //LED7 = !LED7;
            //                                                LED8 = 1;
            if (DecodeStream(data)) {
                events |= EVENT_UPDATE_LENGTHS;
            }
            events &= ~EVENT_UART_DATA_READY;
        }

        if (events & EVENT_CAN_RX) {
            events &= ~EVENT_CAN_RX;
        }

        if (events & EVENT_SPI_RX) {

            events &= ~EVENT_SPI_RX;
        }

        if (events & EVENT_REPORT_FAULT) {
            events &= ~EVENT_REPORT_FAULT;
        }

        if (events & EVENT_ADC_DATA) {

            //flex1 = ((0.2 * ADCBuff.Adc1Data[1]) + (0.8 * flex1));
            //flex2 = ((0.2 * ADCBuff.Adc1Data[2]) + (0.8 * flex2));
            events &= ~EVENT_ADC_DATA;
        }

        if (events & EVENT_UPDATE_LENGTHS) {
            events &= ~EVENT_UPDATE_LENGTHS;
        }
    }
}

void EventChecker(void) {
#ifndef CHARACTERIZE
    if (uartBuffer.dataSize) {
        events |= EVENT_UART_DATA_READY;
    }

    if (canBuffer.dataSize) {
        events |= EVENT_CAN_RX;
    }

    if (spiBuffer.dataSize) {
        events |= EVENT_SPI_RX;
    }

    if (ADCBuff.newData) {
        ADCBuff.newData = 0;
        events |= EVENT_ADC_DATA;
    }
#endif
    events |= EVENT_UPDATE_SPEED;
}

void SetMotors() {

}

void __attribute__((__interrupt__, no_auto_psv)) _CNInterrupt(void) {

    SetMotors();
    IFS1bits.CNIF = 0; // Clear CN interrupt
}

