#ifndef _DELAY_H_
#define _DELAY_H_
#include"intrins.h"

void delay_us(unsigned int n);      //微秒延时
void delay_ms(unsigned int n);      //毫秒延时
/*33.1776精确延时，由于nrf对时序要求较高*/      
void delay10us();
void delay1ms();

#endif