#include"delay.h"

void delay_us(unsigned int n)    //Œ¢√Î—” ±
{
	while(n--)
	{
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
	}
}
void delay_ms(unsigned int n)    //∫¡√Î—” ±
{
	while(n--)
	{
		delay_us(1000);
	}
}
/****************************************************33.1776MHzæ´»∑—” ±***************************************************/
void delay1ms()		//@33.1776MHz  1∫¡√Î
{
	unsigned char i, j;

	_nop_();
	_nop_();
	i = 33;
	j = 66;
	do
	{
		while (--j);
	} while (--i);
}