#ifndef _DELAY_H_
#define _DELAY_H_
#include"intrins.h"

void delay_us(unsigned int n);      //΢����ʱ
void delay_ms(unsigned int n);      //������ʱ
/*33.1776��ȷ��ʱ������nrf��ʱ��Ҫ��ϸ�*/      
void delay10us();
void delay1ms();

#endif