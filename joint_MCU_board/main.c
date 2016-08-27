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

long int  SA_MAX_VEL =  2000;
long int  SF_MAX_VEL =  1000;

CircularBuffer uartBuffer;
uint8_t uartBuf[64];
CircularBuffer canBuffer;
uint8_t canBuf[64];
CircularBuffer spiBuffer;
uint16_t spiBuf[64];

EncoderCts EncCts;
EncoderCtsLong EncCtsLong;

Robot_Encoders robot_encoders;

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
void manageEncoders(void);

int main(void) {
    commandSet.cmd1 = 0;
    commandSet.cmd2 = 0;
    commandSet.cmd3 = 0;
    commandSet.cmd4 = 0;
    commandSet.cmd5 = 0;
    static uint8_t out[500];
    static uint8_t size;
    
    CB_Init(&uartBuffer, uartBuf, 32);
    CB_Init(&spiBuffer, (uint8_t *) spiBuf, 128);
    InitBoard(&ADCBuff, &uartBuffer, &spiBuffer, EventChecker);

    config_spi_slow();


    selectCS(ALL_CS_LOW);
    setQuadX4();
    selectCS(ALL_CS_HIGH);
    
    selectCS(SF_ODD&SF_EVEN&SA_ODD&SA_EVEN);
    set2ByteMode();
    selectCS(ALL_CS_HIGH);
    
    selectCS(RL_ODD&RL_EVEN);
    writeDTRtoZerosLong();
    selectCS(ALL_CS_HIGH);

    selectCS(SF_ODD&SF_EVEN&SA_ODD&SA_EVEN);
    writeDTRtoZeros();
    selectCS(ALL_CS_HIGH);

    selectCS(ALL_CS_LOW);
    setCNTRtoDTR();
    selectCS(ALL_CS_HIGH);





    putsUART2((unsigned int *) "Init. Complete\r\n");
    controller = 0;

    InitDecoder(&commandSet);
    //Pass pointer to com protocol
    while (1) {
        if (events & EVENT_UPDATE_SPEED) {
            RESET_3 = 1;
            RESET_2 = 1;
            RESET_1 = 1;
            iii++;
            int amplitude;
            float freq;
            freq = 200.0;
            amplitude = 100;
            
            MOTOR1_3 = (int) (amplitude * sin(iii / freq) + PTPER / 2);
            MOTOR2_3 = (int) (amplitude * sin(iii / freq) + PTPER / 2);
            MOTOR1_1 = (int) (amplitude * sin(iii / freq) + PTPER / 2);
            MOTOR2_1 = (int) (amplitude * sin(iii / freq) + PTPER / 2);
            MOTOR1_2 = (int) (amplitude * sin(iii / freq) + PTPER / 2);
            MOTOR2_2 = (int) (amplitude * sin(iii / freq) + PTPER / 2);
            
            LED1 = 1;
            manageEncoders();
            LED1 = 0;
            
            if (iii % 100 == 0) {
          
                size = sprintf((char *) out, "RL: %10ld %10ld %10ld %10ld %10ld %10ld  SF: %6d %6d %6d %6d %6d %6d  SA: %6d %6d %6d %6d %6d %6d\r\n",
                        robot_encoders.RL1_ENCDR, robot_encoders.RL2_ENCDR, robot_encoders.RL3_ENCDR, robot_encoders.RL4_ENCDR, robot_encoders.RL5_ENCDR, robot_encoders.RL6_ENCDR,
                        robot_encoders.SF1_ENCDR, robot_encoders.SF2_ENCDR, robot_encoders.SF3_ENCDR, robot_encoders.SF4_ENCDR, robot_encoders.SF5_ENCDR, robot_encoders.SF6_ENCDR,
                        robot_encoders.SA1_ENCDR, robot_encoders.SA2_ENCDR, robot_encoders.SA3_ENCDR, robot_encoders.SA4_ENCDR, robot_encoders.SA5_ENCDR, robot_encoders.SA6_ENCDR);

                //                size = sprintf((char *) out, "1: %5i %5i %5i %5i 2: %5i %5i %5i %5i 3: %5i %5i %5i %5i \r\n",
                //                        SW1_1, SW2_1, SW3_1, SW4_1,
                //                        SW1_2, SW2_2, SW3_2, SW4_2,
                //                        S_SF6, SW2_3, SW3_3, SW4_3);

                DMA0_UART2_Transfer(size, out);
                //LED1 = (jj & 0b1);
                LED1 = SW4_2;
                LED2 = (jj & 0b10) >> 1;
                LED3 = (jj & 0b100) >> 2;
                LED4 = (jj & 0b1000) >> 3;
                jj = ((jj << 1));
                jj = jj == 0b10000 ? 1 : jj;
            }
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

void manageEncoders() {
                uint32_t switchCS = 0;
                
                switchCS = (S_SA1 * ~SA1) | (S_SF1 * ~SF1) | (S_SA2 * ~SA2) |
                           (S_SF2 * ~SF2) | (S_SA3 * ~SA3) | (S_SF3 * ~SF3) |
                           (S_SA4 * ~SA4) | (S_SF4 * ~SF4) | (S_SA5 * ~SA5) |
                           (S_SF5 * ~SF5) | (S_SA6 * ~SA6) | (S_SF6 * ~SF6);

                selectCS(~switchCS);
                setCNTRtoDTR();
                selectCS(ALL_CS_HIGH);

                selectCS(RL_ODD);
                readEncLong(&EncCtsLong);
                selectCS(ALL_CS_HIGH);
                robot_encoders.RL1_ENCDR = EncCtsLong.cts1;
                robot_encoders.RL3_ENCDR = EncCtsLong.cts2;
                robot_encoders.RL5_ENCDR = EncCtsLong.cts3;
                
                selectCS(RL_EVEN);
                readEncLong(&EncCtsLong);
                selectCS(ALL_CS_HIGH);
                robot_encoders.RL2_ENCDR = EncCtsLong.cts3;
                robot_encoders.RL4_ENCDR = EncCtsLong.cts1;
                robot_encoders.RL6_ENCDR = EncCtsLong.cts2;
                
                selectCS(SF_ODD);
                readEnc(&EncCts);
                selectCS(ALL_CS_HIGH);
                robot_encoders.SF1_ENCDR = -EncCts.cts3;
                robot_encoders.SF3_ENCDR = -EncCts.cts1;
                robot_encoders.SF5_ENCDR = -EncCts.cts2;
                
                selectCS(SF_EVEN);
                readEnc(&EncCts);
                selectCS(ALL_CS_HIGH);
                robot_encoders.SF2_ENCDR = EncCts.cts1;
                robot_encoders.SF4_ENCDR = EncCts.cts2;
                robot_encoders.SF6_ENCDR = EncCts.cts3;
                
                selectCS(SA_ODD);
                readEnc(&EncCts);
                selectCS(ALL_CS_HIGH);
                
                robot_encoders.SA1_ENCDR =  EncCts.cts3;
                robot_encoders.SA3_ENCDR =  EncCts.cts1;
                robot_encoders.SA5_ENCDR =  EncCts.cts2;
                
                selectCS(SA_EVEN);
                readEnc(&EncCts);
                selectCS(ALL_CS_HIGH);
                
                robot_encoders.SA2_ENCDR = -EncCts.cts1;
                robot_encoders.SA4_ENCDR = -EncCts.cts2;
                robot_encoders.SA6_ENCDR = -EncCts.cts3;              
}

void __attribute__((__interrupt__, no_auto_psv)) _CNInterrupt(void) {
    IFS1bits.CNIF = 0; // Clear CN interrupt
}

