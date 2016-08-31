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


    selectCS(ALL_CS_LOW,ALL_CS_LOW);
    setQuadX4();
    selectCS(ALL_CS_HIGH,ALL_CS_HIGH);
    
    selectCS(SF_ODD_1&SF_EVEN_1&SA_ODD_1&SA_EVEN_1,SF_ODD_2&SF_EVEN_2&SA_ODD_2&SA_EVEN_2 );
    set2ByteMode();
    selectCS(ALL_CS_HIGH,ALL_CS_HIGH);
    
    selectCS(RL_ODD_1&RL_EVEN_1,RL_ODD_2&RL_EVEN_2);
    writeDTRtoZerosLong();
    selectCS(ALL_CS_HIGH,ALL_CS_HIGH);

    selectCS(SF_ODD_1&SF_EVEN_1&SA_ODD_1&SA_EVEN_1,SF_ODD_2&SF_EVEN_2&SA_ODD_2&SA_EVEN_2);
    writeDTRtoZeros();
    selectCS(ALL_CS_HIGH,ALL_CS_HIGH);

    selectCS(ALL_CS_LOW,ALL_CS_LOW);
    setCNTRtoDTR();
    selectCS(ALL_CS_HIGH,ALL_CS_HIGH);





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
            
            if (iii % 50 == 0) {
                int pos = 0;
//                size = sprintf((char *) out, "RL: %10ld %10ld %10ld %10ld %10ld %10ld  SF: %6d %6d %6d %6d %6d %6d  SA: %6d %6d %6d %6d %6d %6d\r\n",
//                        robot_encoders.RL_ENCDR[0][pos], robot_encoders.RL_ENCDR[1][pos], robot_encoders.RL_ENCDR[2][pos], robot_encoders.RL_ENCDR[3][pos], robot_encoders.RL_ENCDR[4][pos], robot_encoders.RL_ENCDR[5][pos],
//                        robot_encoders.SF_ENCDR[0][pos], robot_encoders.SF_ENCDR[1][pos], robot_encoders.SF_ENCDR[2][pos], robot_encoders.SF_ENCDR[3][pos], robot_encoders.SF_ENCDR[4][pos], robot_encoders.SF_ENCDR[5][pos],
//                        robot_encoders.SA_ENCDR[0][pos], robot_encoders.SA_ENCDR[1][pos], robot_encoders.SA_ENCDR[2][pos], robot_encoders.SA_ENCDR[3][pos], robot_encoders.SA_ENCDR[4][pos], robot_encoders.SA_ENCDR[5][pos]);
//                
                 size = sprintf((char *) out, "RL: %10ld %10ld %10ld %10ld %10ld %10ld  SF: %6d %6d %6d %6d %6d %6d  SA: %6d %6d %6d %6d %6d %6d\r\n",
                        robot_encoders.RL_VEL[0], robot_encoders.RL_VEL[1], robot_encoders.RL_VEL[2], robot_encoders.RL_VEL[3], robot_encoders.RL_VEL[4], robot_encoders.RL_VEL[5],
                        robot_encoders.SF_VEL[0], robot_encoders.SF_VEL[1], robot_encoders.SF_VEL[2], robot_encoders.SF_VEL[3], robot_encoders.SF_VEL[4], robot_encoders.SF_VEL[5],
                        robot_encoders.SA_VEL[0], robot_encoders.SA_VEL[1], robot_encoders.SA_VEL[2], robot_encoders.SA_VEL[3], robot_encoders.SA_VEL[4], robot_encoders.SA_VEL[5]);

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
                uint32_t switchCS_1 = 0,switchCS_2 = 0 ;
                
                switchCS_2 = (S_SA1 * ~SA1_2) | (S_SF1 * ~SF1_2); 
                switchCS_1 = (S_SA2 * ~SA2_1) | (S_SF2 * ~SF2_1) 
                           | (S_SA3 * ~SA3_1) | (S_SF3 * ~SF3_1) 
                           | (S_SA4 * ~SA4_1) | (S_SF4 * ~SF4_1) 
                           | (S_SA5 * ~SA5_1) | (S_SF5 * ~SF5_1) 
                           | (S_SA6 * ~SA6_1) | (S_SF6 * ~SF6_1);

                selectCS(~switchCS_1,~switchCS_2);
                setCNTRtoDTR();
                selectCS(ALL_CS_HIGH,ALL_CS_HIGH);
                selectCS(RL_ODD_1,RL_ODD_2);
                readEncLong(&EncCtsLong);
                int i, j;
                for(i =0; i<6; i++) {
                    for(j = 1; j>-1; j--) {
                        robot_encoders.RL_ENCDR[i][j+1] = robot_encoders.RL_ENCDR[i][j];
                        robot_encoders.SF_ENCDR[i][j+1] = robot_encoders.SF_ENCDR[i][j];
                        robot_encoders.SA_ENCDR[i][j+1] = robot_encoders.SA_ENCDR[i][j];
                    }
                }
                selectCS(ALL_CS_HIGH,ALL_CS_HIGH);
                robot_encoders.RL_ENCDR[0][0] = EncCtsLong.cts1;
                robot_encoders.RL_ENCDR[2][0] = EncCtsLong.cts2;
                robot_encoders.RL_ENCDR[4][0] = EncCtsLong.cts3;
                
                selectCS(RL_EVEN_1, RL_EVEN_2);
                readEncLong(&EncCtsLong);
                selectCS(ALL_CS_HIGH,ALL_CS_HIGH);
                robot_encoders.RL_ENCDR[1][0] = EncCtsLong.cts3;
                robot_encoders.RL_ENCDR[3][0] = EncCtsLong.cts1;
                robot_encoders.RL_ENCDR[5][0] = EncCtsLong.cts2;
                
                selectCS(SF_ODD_1, SF_ODD_2);
                readEnc(&EncCts);
                selectCS(ALL_CS_HIGH,ALL_CS_HIGH);
                robot_encoders.SF_ENCDR[0][0] = -EncCts.cts3;
                robot_encoders.SF_ENCDR[2][0] = -EncCts.cts1;
                robot_encoders.SF_ENCDR[4][0] = -EncCts.cts2;
                
                selectCS(SF_EVEN_1, SF_EVEN_2);
                readEnc(&EncCts);
                selectCS(ALL_CS_HIGH,ALL_CS_HIGH);
                robot_encoders.SF_ENCDR[1][0] = EncCts.cts1;
                robot_encoders.SF_ENCDR[3][0] = EncCts.cts2;
                robot_encoders.SF_ENCDR[5][0] = EncCts.cts3;
                
                selectCS(SA_ODD_1,SA_ODD_2);
                readEnc(&EncCts);
                selectCS(ALL_CS_HIGH,ALL_CS_HIGH);
                robot_encoders.SA_ENCDR[0][0] =  EncCts.cts3;
                robot_encoders.SA_ENCDR[2][0] =  EncCts.cts1;
                robot_encoders.SA_ENCDR[4][0] =  EncCts.cts2;
                
                selectCS(SA_EVEN_1,SA_EVEN_2);
                readEnc(&EncCts);
                selectCS(ALL_CS_HIGH,ALL_CS_HIGH);
                robot_encoders.SA_ENCDR[1][0] = -EncCts.cts1;
                robot_encoders.SA_ENCDR[3][0] = -EncCts.cts2;
                robot_encoders.SA_ENCDR[5][0] = -EncCts.cts3;
                int h = 50; // data freq/2
                for(i =0; i<6; i++) {
                    robot_encoders.RL_VEL[i] = (9*robot_encoders.RL_VEL[i])/10 + (3*robot_encoders.RL_ENCDR[i][0] - 4*robot_encoders.RL_ENCDR[i][1] + robot_encoders.RL_ENCDR[i][2])*h;
                    robot_encoders.SF_VEL[i] = (9*robot_encoders.SF_VEL[i])/10 + (3*robot_encoders.SF_ENCDR[i][0] - 4*robot_encoders.SF_ENCDR[i][1] + robot_encoders.SF_ENCDR[i][2])*h;
                    robot_encoders.SA_VEL[i] = (9*robot_encoders.SA_VEL[i])/10 + (3*robot_encoders.SA_ENCDR[i][0] - 4*robot_encoders.SA_ENCDR[i][1] + robot_encoders.SA_ENCDR[i][2])*h;
                }
}

void __attribute__((__interrupt__, no_auto_psv)) _CNInterrupt(void) {
    IFS1bits.CNIF = 0; // Clear CN interrupt
}

