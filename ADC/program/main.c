#include<STC12C5A60S2.H>
#include"adc.h"
#include"nrf.h"
#include"lcd1602.h"
#include"delay.h"

uchar TX_buf[3] = 0;    //准备发送的数据包

main()
{
	SPI_Init();
	LcdInit();
	ADC_Init();
	LcdShow(0,0,"ADC:");
	while(1)
	{
		tran();
		LcdShow(4,0,TX_buf);    //显示发送数据，与接收机（主机）对比
		NRF_TX_Mode(TX_buf);    //发送模式并发送数据包
		while(CHECK_ACK());     //等待发送完成
	}
}