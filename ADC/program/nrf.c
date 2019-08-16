#include"nrf.h"

const uchar RX_ADDR0[] = {0x43,0x43,0x10,0x10,0x01};    //ͨ��0���յ�ַ
const uchar TX_ADDR0[] = {0x43,0x43,0x10,0x10,0x01};    //�ӻ�0�����ַ

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
 	  MOSI=(dat&0x80);    //д���һλ���ݣ���λ����
 	  dat=(dat<<1);    //��д����������һλ��������һ�����λ
 	  SCK=1;        //����SCK��ʱ���źţ�����
 	  dat|=MISO;     //����ǰ������ȡ�ӻ���״̬������
 	  SCK=0;    //����ʱ���ź�
 	}
 	return dat;    //���ض�ȡ��������
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
	CE = 0;
	SPI_Write_reg(WRITE_REG+CONFIG,0X0D);    //ȷ��Ϊ����ģʽ����ֹ���ţ�������Ҳ���ԣ�
	CE = 1;
	delay10us();
	
	CE = 0;         	//д����ǰCE������
	SPI_Write_buf(WRITE_REG+RX_ADDR_P0,TX_ADDR0,ADDR_WIDTH); //����ͨ��0���յ�ַ
	SPI_Write_reg(WRITE_REG+EN_AA,0X01);                     //��������ͨ��0�Զ�Ӧ��
	SPI_Write_reg(WRITE_REG+EN_RXADDR,0X01);                 //��������ͨ��0����
	SPI_Write_reg(WRITE_REG+RF_CH,CHANNEL);                   //ѡ����Ƶͨ��
	SPI_Write_reg(WRITE_REG+RX_PW_P0,DATE_WIDTH);            //��������ͨ��0��Ч�������
	SPI_Write_reg(WRITE_REG+RF_SETUP,0X07);                  //���ݴ�������1Mb/S,���书��0dBm,�������Ŵ�������
	SPI_Write_reg(WRITE_REG+CONFIG,0X0F);                    //CRCʹ�ܣ�16λCRCУ�飬�ϵ����ģʽ
	CE = 1; 	//��������CE����   
	delay1ms();
}
/*NRF����ģʽ*/
void NRF_TX_Mode(uchar *txbuf)
{
	CE = 0; 
	SPI_Write_reg(WRITE_REG+CONFIG,0X0D);
	CE = 1;
	delay10us();
	
	CE = 0;
	SPI_Write_buf(WRITE_REG+TX_ADDR,TX_ADDR0,ADDR_WIDTH);   //����ͨ����ַ
	SPI_Write_buf(WRITE_REG+RX_ADDR_P0,TX_ADDR0,ADDR_WIDTH);//����ͨ��0���յ�ַ,����ΪӦ��ͨ��
	SPI_Write_buf(WR_TX_PLOAD,txbuf,DATE_WIDTH);
	
	SPI_Write_reg(WRITE_REG+EN_AA,0X01);                    //��������ͨ��0�Զ�Ӧ��
	SPI_Write_reg(WRITE_REG+EN_RXADDR,0X01);                //��������ͨ��0����
	SPI_Write_reg(WRITE_REG+SETUP_RETR,0X1f);               //�ȴ�500+86us���Զ��ط�15��
	SPI_Write_reg(WRITE_REG+RF_CH,CHANNEL);                  //ѡ����Ƶͨ��
	SPI_Write_reg(WRITE_REG+RF_SETUP,0X07);                 //���ݴ�������1Mb/S,���书��0dBm�����,�������Ŵ�������
	SPI_Write_reg(WRITE_REG+CONFIG,0X0E);                   //RCʹ�ܣ�16λCRCУ�飬�ϵ緢��ģʽ
	CE = 1;
	delay1ms();
}

uchar CHECK_ACK(void)
{
	uchar sta;
	while(IRQ);    //�ȴ��������
	sta = SPI_Read_byte(READ_REG+STATUS);    //��ȡ״̬�Ĵ���״̬
	if((sta&TX_OK)||(sta&MAX_TX))            //�жϷ�����ɻ��Ǵﵽ����ط�����
	{
		SPI_Write_reg(WRITE_REG+STATUS,sta);    //����ж�
		CSN = 0;
		SPI_Write_byte(FLUSH_TX);    //���FIFO�Ĵ���
		CSN = 1;
		IRQ = 1;    //�ٴ�ȷ��IRQ�ж����ű�����
		return 0;
	}
	IRQ = 1;
	return 1;
}
/******************NRF��ȡ���ݰ�*******************/
void NRF_RxPacket(uchar *rxbuf)
{
	uchar sta;
	sta = SPI_Read_byte(STATUS);    //��ȡ״̬�Ĵ���
	if(sta&RX_OK)    //�Ƿ������
	{
		CE = 0;
		SPI_Read_buf(RD_RX_PLOAD,rxbuf,DATE_WIDTH);    //��ȡ���յ�������
		SPI_Write_reg(WRITE_REG+STATUS,sta);    //����жϱ�־
		CSN = 0;
		SPI_Write_byte(FLUSH_RX);    //���RX_FIFO�Ĵ���
		CSN = 1;
		IRQ = 1;    //ȷ��IRQ�����ߣ�Ҳ�ɲ�д��
	}
  IRQ = 1;
}

























/**************************************************************��һ�����÷�ʽ**********************************************************************/
/*NRF����ģʽ��ʼ��*/                   /*�Ӵ������޸�*/
/*void NRF_RX_Mode(void)
{
	CE = 0;
	SPI_Write_reg(WRITE_REG+CONFIG,0X0D);
	CE = 1;
	delay_us(20);
	
	CE = 0;         	//д����ǰCE������
	delay_ms(1);
	SPI_Write_buf(WRITE_REG+RX_ADDR_P0,RX_ADDR0,ADDR_WIDTH); //����ͨ��0���յ�ַ
	SPI_Write_reg(WRITE_REG+EN_AA,0X01);                     //��������ͨ��0�Զ�Ӧ��
	SPI_Write_reg(WRITE_REG+EN_RXADDR,0X01);                 //��������ͨ��0����
	SPI_Write_reg(WRITE_REG+RF_CH,CHANEL);                   //ѡ����Ƶͨ��
	SPI_Write_reg(WRITE_REG+RX_PW_P0,DATE_WIDTH);            //��������ͨ��0��Ч�������
	SPI_Write_reg(WRITE_REG+RF_SETUP,0X07);                  //���ݴ�������1Mb/S,���书��0dBm,�������Ŵ�������
	SPI_Write_reg(WRITE_REG+CONFIG,0X0F);                    //CRCʹ�ܣ�16λCRCУ�飬�ϵ����ģʽ
	CE = 1;                                                  //��������CE����                                            //������������ʱ��
}
/*NRF����ģʽ*/
/*void NRF_TX_Mode(void)
{
	CE = 0;
	SPI_Write_reg(WRITE_REG+CONFIG,0X0D);
	CE = 1;
	delay_us(20);
	
	CE = 0;
	SPI_Write_buf(WRITE_REG+TX_ADDR,TX_ADDR0,ADDR_WIDTH);   //����ͨ����ַ
	SPI_Write_buf(WRITE_REG+RX_ADDR_P0,TX_ADDR0,ADDR_WIDTH);//����ͨ��0���յ�ַ,����ΪӦ��ͨ��
	
	SPI_Write_reg(WRITE_REG+EN_AA,0X01);                    //��������ͨ��0�Զ�Ӧ��
	SPI_Write_reg(WRITE_REG+EN_RXADDR,0X01);                //��������ͨ��0����
	SPI_Write_reg(WRITE_REG+SETUP_RETR,0X1a);               //�ȴ�500+86us���Զ��ط�15��
	SPI_Write_reg(WRITE_REG+RF_CH,CHANEL);                  //ѡ����Ƶͨ��
	SPI_Write_reg(WRITE_REG+RF_SETUP,0X07);                 //���ݴ�������1Mb/S,���书��0dBm�����,�������Ŵ�������
  
	SPI_Write_reg(WRITE_REG+CONFIG,0X0E);                   //RCʹ�ܣ�16λCRCУ�飬�ϵ緢��ģʽ
	CE = 1;
}

/*����nrf�Ƿ���������*/
/*bit NRF_CHECK(void)
{
	uchar i;
	uchar check_in_buf[5] = {1,2,3,4,5};
	uchar check_out_buf[5];
	
	delay_ms(1);
	SPI_Write_buf(WRITE_REG+TX_ADDR,check_in_buf,ADDR_WIDTH);
	SPI_Read_buf(READ_REG+TX_ADDR,check_out_buf,ADDR_WIDTH);
	
	for(i=0;i<ADDR_WIDTH;i++)
	{
		if(check_in_buf[i] != check_out_buf[i])
			return 1;
	}
	return 0;    //����
}
/*NRF��ȡ���ݰ�*/
/*uchar NRF_RxPacket(uchar *rxbuf)
{
	uchar sta;
	
	sta = SPI_Read_byte(STATUS);    //����״̬�Ĵ�����ֵ
	SPI_Write_reg(WRITE_REG+STATUS,sta);     //���TX_DS��MAX_RT�ж�
	if(sta&0x40)    //�յ�����
	{
		SPI_Read_buf(RD_RX_PLOAD,rxbuf,DATE_WIDTH);
		SPI_Write_reg(FLUSH_RX,0XFF);    //���FIFO�Ĵ���
		return 0;
	}
	return 1;
}
/*NRF�������ݰ�*/
/*uchar NRF_TxPacket(uchar *txbuf)
{
	uchar sta;
	CE = 0;
	SPI_Write_buf(WR_TX_PLOAD,txbuf,DATE_WIDTH);    //д����Ч����
	CE = 1;    //��������
	while(IRQ!=0);    //�ȴ��������
//	while(!(0x20&SPI_Read_byte(READ_REG+STATUS)));
	sta = SPI_Read_byte(STATUS);    //����״̬�Ĵ�����ֵ
	SPI_Write_reg(WRITE_REG+STATUS,sta);     //���TX_DS��MAX_RT�ж�
	if(sta&0x10)    //�ﵽ�ط��������ֵ
	{ 
		SPI_Write_reg(FLUSH_TX,0XFF);    //���FIFO�Ĵ���
		return 0x10;
	}
	if(sta&0x20)    //�������
		return 0x20;
	return 0xff;    //����ʧ��
}*/