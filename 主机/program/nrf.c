#include"nrf.h"

const uchar RX_ADDR0[] = {0x34,0x43,0x10,0x10,0x01};    //ͨ��0���յ�ַ
const uchar TX_ADDR0[] = {0x34,0x43,0x10,0x10,0x01};    //�ӻ�0�����ַ
const uchar RX_ADDR1[] = {0x43,0x43,0x10,0x10,0x01};    //ͨ��1���յ�ַ
const uchar TX_ADDR1[] = {0x43,0x43,0x10,0x10,0x01};    //�ӻ�1���յ�ַ


/*��ʼ��NRF*/
void SPI_Init(void)
{
	CSN = 1;
	CE = 0;
	SCK = 0;
	IRQ = 1;
}
/*�����ݼĴ���д��һ���ֽ�����*/
uchar SPI_Write_byte(uchar dat)
{ 
	uchar i;
 	for(i=0;i<8;i++)
 	{
 		MOSI=(dat&0x80);
 		dat=(dat<<1);
 		SCK=1;
 		dat|=MISO;
 		SCK=0;
 	}
 	return dat;
}
/*��Ĵ�����д��һ���ֽ����ݣ�regλ�Ĵ�����ַ��datΪд������*/
uchar SPI_Write_reg(uchar reg,uchar dat)
{
	uchar status;    //�Ĵ���״̬
	
	CSN = 0;         //����CSN,ʹ��ͨ��
	delay1ms();
	status = SPI_Write_byte(reg);    //���Ĵ���
	SPI_Write_byte(dat);           //д������
	CSN = 1;         //����ʹ��CSN,��ֹͨ��
	
	return status;    //����״̬
}
/*�ӼĴ����ж�һ���ֽ�,regΪ�Ĵ�����ַ*/
uchar SPI_Read_byte(uchar reg)
{
	uchar backdat;
	
	CSN = 0;
	delay_ms(1);
	SPI_Write_byte(reg);                   //д���ַ
	backdat = SPI_Write_byte(READ_REG);    //��ȡ����
	CSN = 1;
	
	return backdat;
}
/*��Ĵ�����д��һ�����ݰ�*/
uchar SPI_Write_buf(uchar reg,uchar *buf,uchar num)
{
	uchar backdat,i;
	
	CSN = 0;                          //ʹ��ͨ��    
	delay1ms();
	backdat = SPI_Write_byte(reg);    //д��Ĵ�����ַ	
	for(i=0;i<num;i++)                //д��num���ֽ�
	{
		SPI_Write_byte(buf[i]);
	}
	CSN = 1;                          //��ֹͨ��
	
	return backdat;                   //����״ֵ̬
}

/*�ӼĴ����ж�ȡһ�����ݰ�,���������ݴ����β�*buf��*/
uchar SPI_Read_buf(uchar reg,uchar *buf,uchar num)
{
	uchar backdat,i;
	
	CSN = 0;                              //ʹ��ͨ��
	delay1ms();
	backdat = SPI_Write_byte(reg);        //д���ַ
	for(i=0;i<num;i++)
	{
		buf[i] = SPI_Write_byte(READ_REG);    //��ȡnum���ֽ�
	}
	CSN = 1;                              //��ֹͨ��
	
	return backdat;
}

/*NRF����ģʽ��ʼ��*/
void NRF_RX_Mode(void)
{	
	CE = 0;         	//д����ǰCE������
	SPI_Write_buf(WRITE_REG+RX_ADDR_P0,TX_ADDR0,ADDR_WIDTH); //����ͨ��0���յ�ַ
	SPI_Write_buf(WRITE_REG+RX_ADDR_P1,TX_ADDR1,ADDR_WIDTH); //����ͨ��1���յ�ַ
	SPI_Write_reg(WRITE_REG+EN_AA,0X03);                     //��������ͨ��0,1�Զ�Ӧ��
	SPI_Write_reg(WRITE_REG+EN_RXADDR,0X03);                 //��������ͨ��0,1����
	SPI_Write_reg(WRITE_REG+RF_CH,CHANNEL);                  //ѡ����Ƶͨ��
	SPI_Write_reg(WRITE_REG+RX_PW_P0,DATE_WIDTH);            //��������ͨ��0��Ч�������
	SPI_Write_reg(WRITE_REG+RX_PW_P1,DATE_WIDTH);            //��������ͨ��0��Ч�������
	SPI_Write_reg(WRITE_REG+RF_SETUP,0X07);                  //���ݴ�������1Mb/S,���书��0dBm,�������Ŵ�������
	SPI_Write_reg(WRITE_REG+CONFIG,0X0F);                    //CRCʹ�ܣ�16λCRCУ�飬�ϵ����ģʽ
	CE = 1; 	//��������CE����   
	delay1ms();
}
/*NRF����ģʽ*/
void NRF_TX_Mode(uchar *txbuf)
{
	CE = 0;
	SPI_Write_buf(WRITE_REG+TX_ADDR,TX_ADDR0,ADDR_WIDTH);   //����ͨ����ַ
	SPI_Write_buf(WRITE_REG+RX_ADDR_P0,TX_ADDR0,ADDR_WIDTH);//����ͨ��0���յ�ַ,����ΪӦ��ͨ��
	SPI_Write_buf(WR_TX_PLOAD,txbuf,DATE_WIDTH);
	
	SPI_Write_reg(WRITE_REG+EN_AA,0X01);                    //��������ͨ��0�Զ�Ӧ��
	SPI_Write_reg(WRITE_REG+EN_RXADDR,0X01);                //��������ͨ��0����
	SPI_Write_reg(WRITE_REG+SETUP_RETR,0X1a);               //�ȴ�500+86us���Զ��ط�15��
	SPI_Write_reg(WRITE_REG+RF_CH,CHANNEL);                  //ѡ����Ƶͨ��
	SPI_Write_reg(WRITE_REG+RF_SETUP,0X07);                 //���ݴ�������1Mb/S,���书��0dBm�����,�������Ŵ�������
	SPI_Write_reg(WRITE_REG+CONFIG,0X0E);                   //RCʹ�ܣ�16λCRCУ�飬�ϵ緢��ģʽ
	CE = 1;
	delay1ms();
}

/*uchar CHECK_ACK(void)    //����Ϊ����ģʽ����Ҫ����Ƿ������
{
	uchar sta;
	while(IRQ);
	sta = SPI_Read_byte(READ_REG+STATUS);
	if((sta&TX_OK)||(sta&MAX_TX))
	{
		SPI_Write_reg(WRITE_REG+STATUS,sta);
		CSN = 0;
		SPI_Write_byte(FLUSH_TX);
		CSN = 1;
		IRQ = 1;
		return 0;
	}
	IRQ = 1;
	return 1;
}*/
/*NRF��ȡ2�����ݰ�*/
void NRF_RxPacket(uchar *rxbuf0,uchar *rxbuf1)
{
	uchar sta;
	while(IRQ);  //��ȥ
	sta = SPI_Read_byte(STATUS);
	if(sta&RX_OK)  //���յ�����
	{
		if(((sta&0x0e)>>1) == 0)    //�ж��Ƿ�Ϊ����ͨ��0���յ�����
		{
		  CE = 0;
		  SPI_Read_buf(RD_RX_PLOAD,rxbuf0,DATE_WIDTH);
		  SPI_Write_reg(WRITE_REG+STATUS,sta);
		  CSN = 0;
		  SPI_Write_byte(FLUSH_RX);
		  CSN = 1;
		  IRQ = 1;
			delay_ms(1);
		}
		else if(((sta&0x0e)>>1) == 1)  //�Ƿ�Ϊ����ͨ��1���յ�����
		{
			CE = 0;
		  SPI_Read_buf(RD_RX_PLOAD,rxbuf1,DATE_WIDTH);
		  SPI_Write_reg(WRITE_REG+STATUS,sta);
		  CSN = 0;
		  SPI_Write_byte(FLUSH_RX);
		  CSN = 1;
		  IRQ = 1;
		}
	}
  IRQ = 1;
}