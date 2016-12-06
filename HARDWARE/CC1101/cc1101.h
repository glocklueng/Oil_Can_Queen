#ifndef __CC1101_H
#define __CC1101_H

#include "sys.h"
#include "cc1101_REG.h"

#define RX_Address 0xff         // ���ն� �豸��ַ

#define TX_Address 0x01         // ���Ͷ�1 �豸��ַ
//#define TX_Address 0x02         // ���Ͷ�2 �豸��ַ
//#define TX_Address 0x03         // ���Ͷ�3 �豸��ַ
//#define TX_Address 0x04         // ���Ͷ�4 �豸��ַ
//#define TX_Address 0x05         // ���Ͷ�5 �豸��ַ
//#define TX_Address 0x06         // ���Ͷ�6 �豸��ַ
//#define TX_Address 0x07         // ���Ͷ�7 �豸��ַ

#define TX 1                    // ����
#define RX 0                    // ����
#define ACK_LENGTH      5       // Ӧ���źų���       
#define SEND_LENGTH     14      // �������ݰ�����
#define SEND_GAP        1000    // ���1s����һ������
#define RECV_TIMEOUT    1500    // ���ճ�ʱ

//#define WORK_MODE TX       // ����
#define WORK_MODE RX       // ����
/*===========================================================================
------------------------------Internal IMPORT functions----------------------
you must offer the following functions for this module
1. uint8_t SPI_ExchangeByte(uint8_t input); // SPI Send and Receive function
2. CC_CSN_LOW();                            // Pull down the CSN line
3. CC_CSN_HIGH();                           // Pull up the CSN Line
===========================================================================*/
// CC1101,  GDO2(PA3),  CSN(PA4),  GDO0/IRQ(PA8),
#define PORT_CC_GDO2    GPIOA
#define PIN_CC_GDO2     GPIO_Pin_3  

#define PORT_CC_CSN     GPIOA
#define PIN_CC_CSN      GPIO_Pin_4        

#define PORT_CC_IRQ     GPIOA                   // ͨ��ADC�ɼ��ýŵ�ѹ�ɲ��¶�
#define PIN_CC_IRQ      GPIO_Pin_8
	                                                             //��CSN��ͣ�MCU����ȴ�MISO�ű�ͣ�������ѹ�������Ѿ��ȶ����������������У�								
#define CC_CSN_LOW()    GPIO_ResetBits(PORT_CC_CSN, PIN_CC_CSN); while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6) != 0)   // SPI_MISO
#define CC_CSN_HIGH()   GPIO_SetBits(PORT_CC_CSN, PIN_CC_CSN)

#define CC_IRQ_READ()   GPIO_ReadInputDataBit(PORT_CC_IRQ, PIN_CC_IRQ)

/*===========================================================================
----------------------------------macro definitions--------------------------
============================================================================*/
typedef enum {TX_MODE, RX_MODE} TRMODE;
typedef enum {BROAD_ALL, BROAD_NO, BROAD_0, BROAD_0AND255} ADDR_MODE;
typedef enum {BROADCAST, ADDRESS_CHECK} TX_DATA_MODE;

/*===========================================================================
-------------------------------------exported APIs---------------------------
============================================================================*/

uint8_t CC1101ReadReg(uint8_t addr);    /*read a byte from the specified register*/
uint8_t CC1101ReadStatus(uint8_t addr);/*Read a status register*/
void CC1101WriteCmd(uint8_t command);/*Write a command byte to the device*/
void CC1101SetIdle(void);  /*Set the CC1101 into IDLE mode*/
void CC1101SendPacket(uint8_t *txbuffer, uint8_t size, TX_DATA_MODE mode); /*Send a packet*/
void CC1101SetAddress(uint8_t address, ADDR_MODE AddressMode); /*Set the address and address mode of the CC1101*/
void CC1101SetSYNC(uint16_t sync); /*Set the SYNC bytes of the CC1101*/
uint8_t CC1101RecPacket(uint8_t *rxBuffer);/*Receive a packet*/
void  CC1101WORInit(void);/*Initialize the WOR function of CC1101*/
void CC1101Init(void);    /*Initialize the CC1101, User can modify it*/

uint8_t SPI_ExchangeByte(uint8_t Data);
void  CC1101SetTRMode(TRMODE mode);      // Set the device as TX mode or RX mode*/
uint8_t Get_1101RSSI(void);              // ��ȡRSSIֵ

void TIM3_Set(u8 sta);
void TIM3_Init(u16 arr,u16 psc);
	
#endif // _CC1101_H_
