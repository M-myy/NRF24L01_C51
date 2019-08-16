#include"timer.h"

bit flag = 0;    //��ʱ1S��־λ

void Timer0_Init(void)    //��ʱ��0��ʼ��
{
	AUXR |= 0x80;
	TMOD &= 0xF0;
	TMOD |= 0x01;
	TL0 = 0x66;
	TH0 = 0x7E;
	TF0 = 0;
	ET0 = 1;
	TR0 = 1;        //������ʱ��0
}

void Interrupt_Timer0() interrupt 1
{
	static unsigned int cnt = 0;
	
	TL0 = 0X66;    //����װ�س�ֵ
	TH0 = 0X7E;
	cnt++;
	if(cnt>=1000)    //����Ƿ�ﵽ1��
	{
		cnt = 0;     //����ֵ����
		flag = 1;    //��־λ��λ
	}
}