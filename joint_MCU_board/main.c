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
#include <p33Exxxx.h>

CircularBuffer uartBuffer;
uint8_t uartBuf[64];
CircularBuffer canBuffer;
uint8_t canBuf[64];
CircularBuffer spiBuffer;
uint16_t spiBuf[64];

uint16_t events;
bool controller;
ADCBuffer ADCBuff;
long int iii = 0;
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
    static uint8_t out[64];
    static uint8_t size;
    CB_Init(&uartBuffer, uartBuf, 32);
    CB_Init(&spiBuffer, (uint8_t *) spiBuf, 128);
    InitBoard(&ADCBuff, &uartBuffer, &spiBuffer, EventChecker);
    putsUART2((unsigned int *) "Init. Complete\r\n");
    controller = 0;
    SetMotors();

    InitDecoder(&commandSet);
    //Pass pointer to com protocol


    long int i;

    for (i = 0; i < 5000; i++) {
        Nop(); //Let other IC'S catch their breath...
    }


    while (1) {
        if (events & EVENT_UPDATE_SPEED) {

            iii++;

            if (iii % 500 == 0) {
                jjj++;
            }

            if (iii % 10 == 1) {
                //size = sprintf((char *) out, "%i,%i,%i,%i,%i,%i,%i,\r\n", (int) (10000 * quaternion[0]), (int) (10000 * quaternion[1]), (int) (10000 * quaternion[2]), (int) (10000 * quaternion[3]), (int) flex1, (int) flex2, (int) ADCBuff.Adc1Data[3]);
                //size = sprintf((char *) out, "%5i  %5i  %5i  %5i  %5i  %5i  %5i  %5i  %5i\r\n", (int) (10 * imuData.accelX), (int) (10 * imuData.accelY), (int) (10 * imuData.accelZ), (int) (imuData.gyroX), (int) (imuData.gyroY), (int) (imuData.gyroZ), (int) (10 * imuData.magX), (int) (10 * imuData.magY), (int) (10 * imuData.magZ));
                //size = sprintf((char *) out, "%5i  %5i  %5i  %5i\r\n", ADCBuff.Adc1Data[0],ADCBuff.Adc1Data[1],ADCBuff.Adc1Data[2],ADCBuff.Adc1Data[3]);
                //DMA0_UART2_Transfer(size, out);
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

            flex1 = ((0.2 * ADCBuff.Adc1Data[1]) + (0.8 * flex1));
            flex2 = ((0.2 * ADCBuff.Adc1Data[2]) + (0.8 * flex2));
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

