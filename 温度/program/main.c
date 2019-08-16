/*******************************************************************************
*�ӻ���ȡ�¶ȳ��򣬵�Ƭ����ȡ1-Wire������DS18B20����ͨ�ţ��ɼ����¶�����֮��ͨ**
*��nRF24L01����ģ�鴫�ݸ������������������ת����                              *
*******************************************************************************/
#include<STC12C5A60S2.H>
#include<stdio.h>
#include"nrf.h"
#include"ds18b20.h"
#include"lcd1602.h"
#include"timer.h"

bit ACK_R;
int temp,intT,decT;
extern bit flag;
char TX_buf[6];

/*���ڵ���ʹ��*/
/*void UART_Init(void)
{
	SM0 = 0;
	SM1 = 1;

	PCON &= 0x7F;
	SCON = 0x50;
	AUXR |= 0x04;
	BRT = 0xE5;
	AUXR |= 0x01;
	AUXR |= 0x10;
	EA = 1;
	ES = 0;
}

void sendbyte(unsigned char byte)
{
	SBUF = byte;
	while(TI == 0);
	TI = 0;
}*/

main()
{
	EA = 1;
	SPI_Init();
	LcdInit();
	Timer0_Init();
	Enable_DS();
	LcdShow(0,0,"T:");
	while(1)
	{
		if(flag)                        //�ﵽ1������¶�
		{
			flag = 0;                     //���������־λ
		  ACK_R = Read_Temp(&temp);    //����ȡ����16λ�¶�ֵ���浽�β��У�������Ƿ�ɹ�
		  if(ACK_R)                   //��ȡ�ɹ�
		  {
			  intT = (temp >> 4);
				decT = temp & 0xf;
				decT = (decT*1000)/16;
				sprintf(TX_buf,"%d.%d",intT,decT);
				LcdShow(2,0,TX_buf);
		  }
			else
			LcdShow(2,0,"error");			
			LcdShow(2,0,TX_buf);
		  NRF_TX_Mode(TX_buf);
		  while(CHECK_ACK());
		  Enable_DS();
	  }
  }
}