#ifndef _LCD1602_H_
#define _LCD1602_H_
#include<stc12c5a60s2.h>
#include"delay.h"

void LcdShow(unsigned char x,unsigned char y,unsigned char *s);     //������λ����ʾ�ַ�
void LcdInit(void);       //LCD��ʼ��

#endif