#include"lcd1602.h"

#define LCD1602DB P0

sbit LCD1602E = P1^2;
sbit LCD1602RS = P2^0;
sbit LCD1602RW = P2^1;

void LcdWait(void)       //忙位检测
{
	unsigned char sta;
	LCD1602DB = 0XFF;
	LCD1602RS = 0;
	LCD1602RW = 1;
	do
	{
	  LCD1602E = 1;
	  sta = LCD1602DB;
		LCD1602E = 0;
	}while(sta & 0X80);
}

void LcdWriteCmd(unsigned char cmd)   //写指令
{
	LcdWait();
	LCD1602RS = 0;
	LCD1602RW = 0;
	LCD1602DB = cmd;
	delay_ms(4);
	LCD1602E = 1;
	LCD1602E = 0;
}

void LcdWriteDat(unsigned char dat)     //写数据
{
	LcdWait();
	LCD1602RS = 1;
	LCD1602RW = 0;
	LCD1602DB = dat;
	delay_ms(4);
	LCD1602E = 1;
	LCD1602E = 0;
}

void LcdInit(void)       //LCD初始化
{
	LcdWriteCmd(0X38);
	delay_ms(5);
	LcdWriteCmd(0X38);
	delay_ms(5);
	LcdWriteCmd(0X38);
	delay_ms(5);
	LcdWait();
	LcdWriteCmd(0X38);
	LcdWait();
  LcdWriteCmd(0X08);
	LcdWait();
	LcdWriteCmd(0X01);
	LcdWait();
	LcdWriteCmd(0X06);
	LcdWait();
	LcdWriteCmd(0X0C);	
}

void LcdAdd(unsigned char x,unsigned char y)   //设置指针数据地址
{
	unsigned char add;
	if(y == 0)
	{
		add = 0X00 + x;
	}
	else
	{
		add = 0X40 + x;
	}
	LcdWriteCmd(add | 0X80);
}

void LcdShow(unsigned char x,unsigned char y,unsigned char *s)   //在设置位置显示字符
{
	static unsigned char *ss;
  ss = s;
	LcdAdd(x,y);
	while(*ss)
	LcdWriteDat(*ss++);
}