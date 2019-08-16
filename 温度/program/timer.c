#include"timer.h"

bit flag = 0;    //定时1S标志位

void Timer0_Init(void)    //定时器0初始化
{
	AUXR |= 0x80;
	TMOD &= 0xF0;
	TMOD |= 0x01;
	TL0 = 0x66;
	TH0 = 0x7E;
	TF0 = 0;
	ET0 = 1;
	TR0 = 1;        //启动定时器0
}

void Interrupt_Timer0() interrupt 1
{
	static unsigned int cnt = 0;
	
	TL0 = 0X66;    //重新装载初值
	TH0 = 0X7E;
	cnt++;
	if(cnt>=1000)    //检测是否达到1秒
	{
		cnt = 0;     //计数值清零
		flag = 1;    //标志位置位
	}
}