#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "common.h" 
#include "wifista.h"
#include "malloc.h"
#include "lcd.h"
#include "cc1101.h"
#include "touch.h"	
#include "spi.h"
#include "usart2.h"	
 
volatile u16 Time_1ms = 0;               // 1ms������  ���շ�Ӧ��ʱ���
u16 SendCnt = 0;
u16 RecvCnt = 0;
int RecvWaitTime = 0;                    // ���յȴ���ʱʱ��

                                // ֡ͷ  Դ��ַ  Ŀ���ַ ��Ч����9B                                   ֡β2B
u8 SendBuffer[SEND_LENGTH] = {0x55,   0,    0xff,    '2', '9', '6', '8', '5', '1', '2', '9', '2', 0x0d, 0x0a};       // ����������
                                // ֡ͷ  Դ��ַ  Ŀ���ַ  ֡β2B
u8 AckBuffer[ACK_LENGTH]   = {0x55,  0xff,     0,     0x0d, 0x0a};                                  // ����Ӧ������

volatile u8 Str_Info[20];       // �ӻ����ݰ��е� ��Ч����

u8 RF_SendPacket(uint8_t *Sendbuffer, uint8_t length);     // ���߷������ݺ���  
u8 RF_RecvHandler(void);                                   // �������ݽ��մ��� 

void Load_Drow_Dialog(void);                 
void rtp_test(void);                           // ���败�������Ժ���
	
int main(void)
{ 
	u8 Link_Error = 0;

	delay_init();	    	                         // ��ʱ������ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  // ����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�	  
	uart_init(115200);	 	                         // ��ʼ������1������Ϊ115200	����
	USART2_Init(115200);                             // ��ʼ������2������Ϊ115200   wifi
	
	LCD_Init();				                         // ��ʼ��Һ�� 		
	LCD_ShowString(25,150,200,24,24,(u8*)"wifi scanning..."); 
	tp_dev.init();			                         // ��ʼ��������
	
	SPI1_Init();                                     // CC1101 SPIͨ�ų�ʼ��
	TIM3_Init(99,7199);		                         // CC1101 1ms�ж�
	mem_init();                                      // ��ʼ���ڴ��
	
	printf("Oil Can...\r\n");
	atk_8266_init();                                 // ATK-ESP8266ģ���ʼ�����ú���
	
	while(1);
//	CC1101Init();                                    // ��ʼ��CC1101

//#if (WORK_MODE == TX)     // ִ�з���ģ�����
//	SendBuffer[1] = TX_Address;  // ���ݰ�Դ��ַ
//	printf("Mode:TX\r\n");
//	CC1101SetTRMode(TX_MODE);    // ����ģʽ  
//	while(1)
//	{
////		do
////		{
////			res = RF_SendPacket(SendBuffer, SEND_LENGTH);
////			if(res != 0) printf("\r\nSend ERROR:%d\r\n", (int)res);  // ���ʹ���
////		} while(res != 0);
////		printf("\r\nSend OK\r\n"); 
//		u8 res = RF_SendPacket(SendBuffer, SEND_LENGTH);
//		if(res != 0) printf("\r\nSend ERROR:%d\r\n", (int)res);  // ���ʹ������
//		delay_ms(1000);
//		delay_ms(1000);
//		delay_ms(1000);
//		delay_ms(1000);
//	}
//#else                     // ִ�н���ģ�����
//	printf("Mode:RX\r\n");
//	CC1101SetTRMode(RX_MODE);   // ����ģʽ  
//	while(1)
//	{
//		u8 res = RF_RecvHandler();                              // �������ݽ��մ��� 
//		if(res != 0) printf("Rec ERROR:%d\r\n", (int)res);      // ���մ���
//		else                                                    // ���ճɹ�
//		{
//			while(1)    // �������ͣ�ֱ�����ͳɹ���
//			{
//				if(atk_8266_wifisend_data((u8*)Str_Info) == 0)      // wifi����ʧ��
//				{
//					if(++Link_Error == 3)     // ����3�η���ʧ�ܣ���������TCP
//					{
//						Link_Error = 0;       // ����
//						printf("���ӳ���������������...\r\n\r\n");	
//						atk_8266_init();      // ATK-ESP8266ģ���ʼ�����ú���
//						delay_ms(500);
//					}
//					delay_ms(500);
//				}
//				else                                                // wifi���ͳɹ�
//				{
//					Link_Error = 0;      // ����
//					printf("wifi���ͳɹ�\r\n\r\n");
//					break;
//				}
//			}
//		}
//	}
//#endif
//}

//int main(void)
//{
//	uint8_t res = 0;
//	
//  HAL_Init();
//  SystemClock_Config();

//  MX_GPIO_Init();
//	MX_USART1_UART_Init();
//	MX_TIM3_Init();
//  MX_SPI1_Init();	
//	
//	CC1101Init();                                   // ��ʼ��L01�Ĵ���     

//#if (WORK_MODE == TX)     // ִ�з���ģ�����
//	
//	SendBuffer[0] = TX_Address;  // ���ݰ����ֽڱ��Դ��ַ
//	printf("Mode:TX\r\n");
//	CC1101SetTRMode(TX_MODE);    // ����ģʽ  
//	while(1)
//	{
//		res = RF_SendPacket(SendBuffer, SEND_LENGTH);
//		if(res == 1)         // �������ݰ��ɹ�
//		{
//			SendCnt++;
//			printf("Send OK\r\n\r\n");
//		}
//		else if(res == 2)    // Ӧ��ʱ
//		{
//			SendCnt++;
//			printf("Ack ERROR\r\n\r\n");
//		}
//		else
//		{
//			printf("Send ERROR\r\n\r\n");
//		}
//		HAL_Delay(1000);HAL_Delay(1000);HAL_Delay(1000);HAL_Delay(800);
//	}
//	
//#else                     // ִ�н���ģ�����
//	
//	AckBuffer[0] = RX_Address;  // ���ݰ����ֽڱ��Դ��ַ
//	printf("Mode:RX\r\n");
//	CC1101SetTRMode(RX_MODE);   // ����ģʽ  
//	while(1)
//	{
//		RF_RecvHandler();     // �������ݽ��մ��� 
//	}
//	
//#endif
}

void Load_Drow_Dialog(void)
{
		LCD_Clear(WHITE);//����   
		POINT_COLOR=BLUE;//��������Ϊ��ɫ 
		LCD_ShowString(lcddev.width-24,0,200,16,16,(u8*)"RST");//��ʾ��������
		POINT_COLOR=RED;//���û�����ɫ 
}
////////////////////////////////////////////////////////////////////////////////
//5�����ص����ɫ												 
//���败�������Ժ���
void rtp_test(void)
{
	while(1)
	{
		tp_dev.scan(0); 		 
		if(tp_dev.sta&TP_PRES_DOWN)			//������������
		{	
			if(tp_dev.x[0]<lcddev.width&&tp_dev.y[0]<lcddev.height)
			{	
				if(tp_dev.x[0]>(lcddev.width-24)&&tp_dev.y[0]<16)Load_Drow_Dialog();//���
				else TP_Draw_Big_Point(tp_dev.x[0],tp_dev.y[0],RED);		//��ͼ	  			   
			}
		}
		else delay_ms(10);	//û�а������µ�ʱ�� 	    
	}
}

/*===========================================================================
* ���� : RF_SendPacket() => ���߷������ݺ���                            *
* ���� : Sendbufferָ������͵����ݰ���length���ݰ�����                 *
* ��� : 0�����ͳɹ�
		 1���ȴ�Ӧ��ʱ 
		 2�����ݰ����ȴ���
		 3�����ݰ�֡ͷ����
		 4�����ݰ�Դ��ַ����        
		 5�����ݰ�Ŀ���ַ����
		 6�����ݰ�֡β����

============================================================================*/
uint8_t RF_SendPacket(uint8_t *Sendbuffer, uint8_t length)
{
	volatile uint8_t ack_flag = 1;         // =1,����Ӧ���ź�,=0������
	uint8_t  i = 0, ack_len = 0, ack_buffer[65] = {0};
	
	CC1101SendPacket(SendBuffer, length, ADDRESS_CHECK);    // ��������   
	                       
	CC1101Init();                               // ��ʼ��L01�Ĵ���    
	CC1101SetTRMode(RX_MODE);                   // ����RFоƬΪ����ģʽ,��������

	RecvWaitTime = (int)RECV_TIMEOUT;           // �ȴ�Ӧ��ʱ����1500ms
	TIM3_Set(1);                                // ������ʱ��TIM3

	printf("Send Over, waiting for ack...\r\n");
	while(CC_IRQ_READ() != 0)                   // �ȴ��������ݰ�
	{
		if(RecvWaitTime <= 0)      
		{  
			TIM3_Set(0);                            // �رն�ʱ��TIM3
			return 1;                               // �ȴ�Ӧ��ʱ
		}
	}
	while(CC_IRQ_READ() == 0);               
	
	TIM3_Set(0);                                // �رն�ʱ��TIM3
	ack_len = CC1101RecPacket(ack_buffer);      // ��ȡ�յ�������
	
//	                                 //֡ͷ  Դ��ַ  Ŀ���ַ  ֡β2B
//uint8_t AckBuffer[ACK_LENGTH]   = {0x55,  0xff,     0,     0x0d, 0x0a};    // ����Ӧ������
	
	if(ack_len != ACK_LENGTH) return 2;                                               // ���ݰ����ȴ���
	if(ack_buffer[0] != 0x55) return 3;                                               // ���ݰ�֡ͷ����
	if(ack_buffer[1] != 0xff) return 4;                                               // ���ݰ�Դ��ַ����        
	if(ack_buffer[2] == 0xff) return 5;                                               // ���ݰ�Ŀ���ַ����     �Ƿ���ַ��0xffΪ������ַ������
	if((ack_buffer[ack_len-2] != 0x0d) || (ack_buffer[ack_len-1] != 0x0a)) return 6;  // ���ݰ�֡β����

	// Ӧ����ȷ
	printf("ack_len=%d;ack_buffer:", (int)ack_len);
	for(i = 0; i < ack_len; i++)                     
	{
		printf("%d ", (int)ack_buffer[i]);
	}
	printf("\r\n");

	return 0;  
}

/*===========================================================================
*   ����: 	RF_RecvHandler() => �������ݽ��մ���                            *
* ����ֵ��	0��������ȷ                                                     *
			1�����ݰ����ȴ���
			2�����ݰ�֡ͷ����
			3�����ݰ�Դ��ַ����        
			4�����ݰ�Ŀ���ַ����
			5�����ݰ�֡β����
============================================================================*/
uint8_t RF_RecvHandler(void)
{
	uint8_t i = 0, j = 0, length = 0, recv_buffer[30] = {0};
	
	CC1101SetTRMode(RX_MODE);            // ����RFоƬΪ����ģʽ,��������
	
	printf("waiting for data...\r\n");
	while(CC_IRQ_READ() != 0);           // �ȴ��������ݰ�
	printf("waiting for data1...\r\n");
	while(CC_IRQ_READ() == 0);

	for(i=0; i<SEND_LENGTH; i++) recv_buffer[i] = 0;  // ��������,��ֹ����
	length = CC1101RecPacket(recv_buffer);            // ��ȡ���ܵ������ݳ��Ⱥ���������

//	                              // ֡ͷ  Դ��ַ  Ŀ���ַ  ��Ч����9B                 ֡β2B
//uint8_t SendBuffer[SEND_LENGTH] = {0x55,   0,    0xff,     2, 9, 6, 8, 5, 1, 2, 9, 2, 0x0d, 0x0a};       // ����������
	if(length != SEND_LENGTH)  return 1;                                              // ���ݰ����ȴ���
	if(recv_buffer[0] != 0x55) return 2;                                              // ���ݰ�֡ͷ����
	if(recv_buffer[1] == 0xff) return 3;                                              // ���ݰ�Դ��ַ����        �Ƿ���ַ��0xffΪ������ַ������
	if(recv_buffer[2] != 0xff) return 4;                                              // ���ݰ�Ŀ���ַ����
	if((recv_buffer[length-2] != 0x0d) || (recv_buffer[length-1] != 0x0a)) return 5;  // ���ݰ�֡β����
	
	
	// ���ݽ�����ȷ  ��ȡ���ӻ����ݰ��е���Ч���ݵ�  Str_Info ��    �ӻ���ַ+��Ч����
	Str_Info[j++] = recv_buffer[1];                // �ӻ���ַ
	for(i = 3; i < (length-2); i++, j++)           // ��Ч����             
	{
		Str_Info[j] = recv_buffer[i];
	}
	Str_Info[j] = 0;           // ����ַ���������
	printf("Rec from:%d\r\n", (int)Str_Info[0]);        // ��ʾ�ӻ���ַ
	printf("Str_length=%d; Str_Info:%s; RSSI=%d dB\r\n", (int)strlen((const char*)Str_Info), Str_Info, (int)Get_1101RSSI());
	
	// ������ӻ��ط�Ӧ���ź�

	CC1101SetTRMode(TX_MODE);  
	AckBuffer[2] = recv_buffer[1];        // �ӻ���ַ
	CC1101SendPacket(AckBuffer, ACK_LENGTH, ADDRESS_CHECK);
	 
	printf("%d:Ack to %d OK\r\n", (int)++RecvCnt, (int)AckBuffer[2]);

	return 0;
}
















