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
/****************************************************33.1776MHz***************************************************/
void delay2us()		//@33.1776MHz
{
	unsigned char i;

	i = 14;
	while (--i);
}
void delay10us()		//@33.1776MHz 10Œ¢√Î
{
	unsigned char i;

	_nop_();
	_nop_();
	i = 80;
	while (--i);
}
void delay60us()		//@33.1776MHz
{
	unsigned char i, j;

	_nop_();
	_nop_();
	_nop_();
	i = 2;
	j = 236;
	do
	{
		while (--j);
	} while (--i);
}
void delay500us()		//@33.1776MHz
{
	unsigned char i, j;

	_nop_();
	i = 17;
	j = 31;
	do
	{
		while (--j);
	} while (--i);
}
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