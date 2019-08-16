#include<STC12C5A60S2.H>
#include"nrf.h"
#include"lcd1602.h"
#include"delay.h"

unsigned char TEMP_buf[32] = 0;
unsigned char ADC_buf[32] = 0;

main()
{
	SPI_Init();
	LcdInit();
	LcdShow(0,0,"T:");
	LcdShow(9,0,"ADC:");
	while(1)
	{		
		NRF_RX_Mode();
		NRF_RxPacket(TEMP_buf,ADC_buf);
		LcdShow(2,0,TEMP_buf);
		LcdShow(13,0,ADC_buf);
	}
}