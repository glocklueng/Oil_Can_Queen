#include "common.h"
#include "stdlib.h"
#include "stdio.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////// 
//�û�������

//���Ӷ˿ں�:8086,�������޸�Ϊ�����˿�.
const u8* remote_ip = (u8*)"192.168.191.1";	
//const u8* remote_ip = "222.212.204.66";	

const u8* portnum   = (u8*)"8086";	 

//WIFI STAģʽ,����Ҫȥ���ӵ�·�������߲���,��������Լ���·��������,�����޸�.
const u8* wifista_ssid=(u8*)"Growl";			//·����SSID��
const u8* wifista_encryption=(u8*)"wpawpa2_aes";	//wpa/wpa2 aes���ܷ�ʽ
const u8* wifista_password=(u8*)"147258369"; 	//��������

//WIFI APģʽ,ģ���������߲���,�������޸�.
const u8* wifiap_ssid=(u8*)"ATK-ESP8266";			//����SSID��
const u8* wifiap_encryption=(u8*)"wpawpa2_aes";	//wpa/wpa2 aes���ܷ�ʽ
const u8* wifiap_password=(u8*)"12345678"; 		//�������� 

/////////////////////////////////////////////////////////////////////////////////////////////////////////// 
//4������ģʽ
const u8 *ATK_ESP8266_CWMODE_TBL[3]={(u8*)"STAģʽ ",(u8*)"APģʽ ",(u8*)"AP&STAģʽ "};	//ATK-ESP8266,3������ģʽ,Ĭ��Ϊ·����(ROUTER)ģʽ 
//4�ֹ���ģʽ
const u8 *ATK_ESP8266_WORKMODE_TBL[3]={(u8*)"TCP������",(u8*)"TCP�ͻ���",(u8*)"UDPģʽ"};	//ATK-ESP8266,4�ֹ���ģʽ
//5�ּ��ܷ�ʽ
const u8 *ATK_ESP8266_ECN_TBL[5]={(u8*)"OPEN",(u8*)"WEP",(u8*)"WPA_PSK",(u8*)"WPA2_PSK",(u8*)"WPA_WAP2_PSK"};
/////////////////////////////////////////////////////////////////////////////////////////////////////////// 

//usmart֧�ֲ���
//���յ���ATָ��Ӧ�����ݷ��ظ����Դ���
//mode:0,������USART2_RX_STA;
//     1,����USART2_RX_STA;
void atk_8266_at_response(u8 mode)
{
	if(USART2_RX_STA&0X8000)		//���յ�һ��������
	{ 
		USART2_RX_BUF[USART2_RX_STA&0X7FFF]=0;//��ӽ�����
		printf("%s",USART2_RX_BUF);	//���͵�����
		if(mode)USART2_RX_STA=0;
	} 
}
//ATK-ESP8266���������,�����յ���Ӧ��
//str:�ڴ���Ӧ����
//����ֵ:0,û�еõ��ڴ���Ӧ����
//    ����,�ڴ�Ӧ������λ��(str��λ��)
u8* atk_8266_check_cmd(u8 *str)
{
	
	char *strx=0;
	if(USART2_RX_STA&0X8000)		//���յ�һ��������
	{ 
		USART2_RX_BUF[USART2_RX_STA&0X7FFF]=0;//��ӽ�����
		strx=strstr((const char*)USART2_RX_BUF,(const char*)str);
	} 
	return (u8*)strx;
}
//��ATK-ESP8266��������
//cmd:���͵������ַ���
//ack:�ڴ���Ӧ����,���Ϊ��,���ʾ����Ҫ�ȴ�Ӧ��
//waittime:�ȴ�ʱ��(��λ:10ms)
//����ֵ:0,���ͳɹ�(�õ����ڴ���Ӧ����)
//       1,����ʧ��
u8 atk_8266_send_cmd(u8 *cmd,u8 *ack,u16 waittime)
{
	u8 res=0; 
	USART2_RX_STA=0;
	u2_printf("%s\r\n",cmd);	//��������
	if(ack&&waittime)		//��Ҫ�ȴ�Ӧ��
	{
		while(--waittime)	//�ȴ�����ʱ
		{
			delay_ms(10);
			if(USART2_RX_STA&0X8000)//���յ��ڴ���Ӧ����
			{
				if(atk_8266_check_cmd(ack))
				{
					printf("ack:%s\r\n",(u8*)ack);
					break;//�õ���Ч���� 
				}
					USART2_RX_STA=0;
			} 
		}
		if(waittime==0)res=1; 
	}
	return res;
} 
//��ATK-ESP8266����ָ������
//data:���͵�����(����Ҫ��ӻس���)
//ack:�ڴ���Ӧ����,���Ϊ��,���ʾ����Ҫ�ȴ�Ӧ��
//waittime:�ȴ�ʱ��(��λ:10ms)
//����ֵ:0,���ͳɹ�(�õ����ڴ���Ӧ����)luojian
u8 atk_8266_send_data(u8 *data,u8 *ack,u16 waittime)
{
	u8 res=0; 
	USART2_RX_STA=0;
	u2_printf("%s",data);	//��������
	if(ack&&waittime)		//��Ҫ�ȴ�Ӧ��
	{
		while(--waittime)	//�ȴ�����ʱ
		{
			delay_ms(10);
			if(USART2_RX_STA&0X8000)//���յ��ڴ���Ӧ����
			{
				if(atk_8266_check_cmd(ack))break;//�õ���Ч���� 
				USART2_RX_STA=0;
			} 
		}
		if(waittime==0)res=1; 
	}
	return res;
}
//ATK-ESP8266�˳�͸��ģʽ
//����ֵ:0,�˳��ɹ�;
//       1,�˳�ʧ��
u8 atk_8266_quit_trans(void)
{
	while((USART2->SR&0X40)==0);	//�ȴ����Ϳ�
	USART2->DR='+';      
	delay_ms(15);					//���ڴ�����֡ʱ��(10ms)
	while((USART2->SR&0X40)==0);	//�ȴ����Ϳ�
	USART2->DR='+';      
	delay_ms(15);					//���ڴ�����֡ʱ��(10ms)
	while((USART2->SR&0X40)==0);	//�ȴ����Ϳ�
	USART2->DR='+';      
	delay_ms(500);					//�ȴ�500ms
	return atk_8266_send_cmd((u8*)"AT",(u8*)"OK",20);//�˳�͸���ж�.
}
//��ȡATK-ESP8266ģ���AP+STA����״̬
//����ֵ:0��δ����;1,���ӳɹ�
u8 atk_8266_apsta_check(void)
{
	if(atk_8266_quit_trans())return 0;			//�˳�͸�� 
	atk_8266_send_cmd((u8*)"AT+CIPSTATUS",(u8*)":",50);	//����AT+CIPSTATUSָ��,��ѯ����״̬
	if(atk_8266_check_cmd((u8*)"+CIPSTATUS:0") &&	atk_8266_check_cmd((u8*)"+CIPSTATUS:1") && atk_8266_check_cmd((u8*)"+CIPSTATUS:2") &&	atk_8266_check_cmd((u8*)"+CIPSTATUS:4"))
				return 0;
	else  return 1;
}
//��ȡATK-ESP8266ģ�������״̬
//����ֵ:0,δ����;1,���ӳɹ�.
u8 atk_8266_consta_check(void)
{
	u8 *p;
	u8 res;
	if(atk_8266_quit_trans())return 0;			        // �˳�͸�� 
	atk_8266_send_cmd((u8*)"AT+CIPSTATUS",(u8*)":",50);	// ����AT+CIPSTATUSָ��,��ѯ����״̬
	p=atk_8266_check_cmd((u8*)"+CIPSTATUS:");           // +CIPSTATUS:��Ϊid��
	res=*p;									//�õ�����״̬	
	return res;
}


//��ȡClient ip��ַ
//ipbuf:ip��ַ���������
void atk_8266_get_wanip(u8* ipbuf)
{
	u8 *p,*p1;
	if(atk_8266_send_cmd((u8*)"AT+CIFSR",(u8*)"OK",50))//��ȡWAN IP��ַʧ��
	{
		ipbuf[0]=0;
		return;
	}		
	p=atk_8266_check_cmd((u8*)"\"");
	p1=(u8*)strstr((const char*)(p+1),(const char*)"\"");
	*p1=0;
	sprintf((char*)ipbuf,"%s",p+1);	
}


//ATK-ESP8266ģ���ʼ�����ú���
void atk_8266_init(void)
{
	u8 ipbuf[16]; 	                          // IP����
	u8 *p = mymalloc(32);							        // ����32�ֽ��ڴ�
	
	printf("ATK-ESP8266 WIFIģ��\r\n");
	while(atk_8266_send_cmd((u8*)"AT",(u8*)"OK",20))    //���WIFIģ���Ƿ�����
	{
		atk_8266_quit_trans();                       //�˳�͸��
		atk_8266_send_cmd((u8*)"AT+CIPMODE=0",(u8*)"OK",200);  //�ر�͸��ģʽ	
		printf("δ��⵽wifiģ��!!!\r\n");
		delay_ms(1200);
		printf("��������wifiģ��...\r\n");
	} 
	printf("�ɹ���⵽wifiģ��...\r\n");
	while(atk_8266_send_cmd((u8*)"ATE0",(u8*)"OK",20));      //�رջ���
	
	delay_ms(10); 
	atk_8266_at_response(1);//���ATK-ESP8266ģ�鷢�͹���������,��ʱ�ϴ�������

	printf("��������ATK-ESP8266ģ�飬���Ե�...\r\n");
	
	atk_8266_send_cmd((u8*)"AT+CWMODE=1",(u8*)"OK",50);		     // ����WIFI STAģʽ
	atk_8266_send_cmd((u8*)"AT+RST",(u8*)"OK",20);		         // DHCP�������ر�(��APģʽ��Ч) 
	delay_ms(1000);         //��ʱ3S�ȴ������ɹ�
	delay_ms(1000);
	delay_ms(1000);
	//�������ӵ���WIFI��������/���ܷ�ʽ/����,�⼸��������Ҫ�������Լ���·�������ý����޸�!! 
	sprintf((char*)p,"AT+CWJAP=\"%s\",\"%s\"",wifista_ssid,wifista_password);//�������߲���:ssid,����
	while(atk_8266_send_cmd(p,(u8*)"WIFI GOT IP",300))    //����Ŀ��·����,���һ��IP
	{
		printf("��������Ŀ��wifi��%s����ȷ�ϸ�wifi�ѿ���������\r\n", wifista_ssid);
		delay_ms(300);
	}		
	printf("�ɹ�����Ŀ��wifi��%s\r\n", wifista_ssid);
	delay_ms(650);delay_ms(650);
	//TCP
	atk_8266_send_cmd((u8*)"AT+CIPMUX=0",(u8*)"OK",20);   //0�������ӣ�1��������
	sprintf((char*)p,(const char*)"AT+CIPSTART=\"TCP\",\"%s\",%s",remote_ip,(u8*)portnum);    //����Ŀ��TCP������
	while(atk_8266_send_cmd(p,(u8*)"OK",300))
	{
		printf("TCP����ʧ�ܣ���ȷ�Ϸ������Ƿ��\r\n");
		delay_ms(600);
	}	
	printf("TCP���ӳɹ�\r\n");
	atk_8266_send_cmd((u8*)"AT+CIPMODE=1",(u8*)"OK",200);      // ����ģʽΪ��͸��		
		
		
	atk_8266_get_wanip(ipbuf);                       // ������ģʽ,��ȡWAN IP
	sprintf((char*)p,"IP��ַ:%s �˿�:%s",ipbuf,(u8*)portnum);
	printf("%s\r\n", p);

	//atk_8266_wificonf_show(30,180,"������·�������߲���Ϊ:",(u8*)wifista_ssid,(u8*)wifista_encryption,(u8*)wifista_password);
	printf("����ģʽ��%s\r\n", (u8*)ATK_ESP8266_WORKMODE_TBL[1]);      // TCP�ͻ���
	
	myfree(p);		//�ͷ��ڴ� 
	USART2_RX_STA=0;
}

















































