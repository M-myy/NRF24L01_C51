/*******************************************************************************
*从机采取温度程序，单片机采取1-Wire总线与DS18B20进行通信，采集到温度数据之后通**
*过nRF24L01无线模块传递给主机，并在主机完成转换。                              *
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

/*串口调试使用*/
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
		if(flag)                        //达到1秒更新温度
		{
			flag = 0;                     //重新清零标志位
		  ACK_R = Read_Temp(&temp);    //将读取到的16位温度值保存到形参中，并检测是否成功
		  if(ACK_R)                   //读取成功
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