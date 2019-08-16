#include<STC12C5A60S2.H>
#include"adc.h"
#include"nrf.h"
#include"lcd1602.h"
#include"delay.h"

uchar TX_buf[3] = 0;    //׼�����͵����ݰ�

main()
{
	SPI_Init();
	LcdInit();
	ADC_Init();
	LcdShow(0,0,"ADC:");
	while(1)
	{
		tran();
		LcdShow(4,0,TX_buf);    //��ʾ�������ݣ�����ջ����������Ա�
		NRF_TX_Mode(TX_buf);    //����ģʽ���������ݰ�
		while(CHECK_ACK());     //�ȴ��������
	}
}