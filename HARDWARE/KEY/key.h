#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"

//������������	   	 

#define BL_SWITCH  PCin(11)	     // LCD ���⿪��

void KEY_Init(void);    // IO��ʼ��
u8 KEY_Scan(u8 mode);  	// ����ɨ�躯��					    
#endif
