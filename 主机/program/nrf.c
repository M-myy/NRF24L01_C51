#include"nrf.h"

const uchar RX_ADDR0[] = {0x34,0x43,0x10,0x10,0x01};    //通道0接收地址
const uchar TX_ADDR0[] = {0x34,0x43,0x10,0x10,0x01};    //从机0发射地址
const uchar RX_ADDR1[] = {0x43,0x43,0x10,0x10,0x01};    //通道1接收地址
const uchar TX_ADDR1[] = {0x43,0x43,0x10,0x10,0x01};    //从机1接收地址


/*初始化NRF*/
void SPI_Init(void)
{
	CSN = 1;
	CE = 0;
	SCK = 0;
	IRQ = 1;
}
/*向数据寄存器写入一个字节数据*/
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
/*向寄存器中写入一个字节数据，reg位寄存器地址，dat为写入数据*/
uchar SPI_Write_reg(uchar reg,uchar dat)
{
	uchar status;    //寄存器状态
	
	CSN = 0;         //拉低CSN,使能通信
	delay1ms();
	status = SPI_Write_byte(reg);    //检测寄存器
	SPI_Write_byte(dat);           //写入数据
	CSN = 1;         //重新使能CSN,禁止通信
	
	return status;    //返回状态
}
/*从寄存器中读一个字节,reg为寄存器地址*/
uchar SPI_Read_byte(uchar reg)
{
	uchar backdat;
	
	CSN = 0;
	delay_ms(1);
	SPI_Write_byte(reg);                   //写入地址
	backdat = SPI_Write_byte(READ_REG);    //读取数据
	CSN = 1;
	
	return backdat;
}
/*向寄存器中写入一个数据包*/
uchar SPI_Write_buf(uchar reg,uchar *buf,uchar num)
{
	uchar backdat,i;
	
	CSN = 0;                          //使能通信    
	delay1ms();
	backdat = SPI_Write_byte(reg);    //写入寄存器地址	
	for(i=0;i<num;i++)                //写入num个字节
	{
		SPI_Write_byte(buf[i]);
	}
	CSN = 1;                          //禁止通信
	
	return backdat;                   //返回状态值
}

/*从寄存器中读取一个数据包,读到的数据存在形参*buf中*/
uchar SPI_Read_buf(uchar reg,uchar *buf,uchar num)
{
	uchar backdat,i;
	
	CSN = 0;                              //使能通信
	delay1ms();
	backdat = SPI_Write_byte(reg);        //写入地址
	for(i=0;i<num;i++)
	{
		buf[i] = SPI_Write_byte(READ_REG);    //读取num个字节
	}
	CSN = 1;                              //禁止通信
	
	return backdat;
}

/*NRF接收模式初始化*/
void NRF_RX_Mode(void)
{	
	CE = 0;         	//写数据前CE必拉低
	SPI_Write_buf(WRITE_REG+RX_ADDR_P0,TX_ADDR0,ADDR_WIDTH); //数据通道0接收地址
	SPI_Write_buf(WRITE_REG+RX_ADDR_P1,TX_ADDR1,ADDR_WIDTH); //数据通道1接收地址
	SPI_Write_reg(WRITE_REG+EN_AA,0X03);                     //允许数据通道0,1自动应答
	SPI_Write_reg(WRITE_REG+EN_RXADDR,0X03);                 //接收数据通道0,1允许
	SPI_Write_reg(WRITE_REG+RF_CH,CHANNEL);                  //选择射频通道
	SPI_Write_reg(WRITE_REG+RX_PW_P0,DATE_WIDTH);            //接收数据通道0有效宽度设置
	SPI_Write_reg(WRITE_REG+RX_PW_P1,DATE_WIDTH);            //接收数据通道0有效宽度设置
	SPI_Write_reg(WRITE_REG+RF_SETUP,0X07);                  //数据传输速率1Mb/S,发射功率0dBm,低噪声放大器增益
	SPI_Write_reg(WRITE_REG+CONFIG,0X0F);                    //CRC使能，16位CRC校验，上电接收模式
	CE = 1; 	//重新拉高CE引脚   
	delay1ms();
}
/*NRF发射模式*/
void NRF_TX_Mode(uchar *txbuf)
{
	CE = 0;
	SPI_Write_buf(WRITE_REG+TX_ADDR,TX_ADDR0,ADDR_WIDTH);   //发射通道地址
	SPI_Write_buf(WRITE_REG+RX_ADDR_P0,TX_ADDR0,ADDR_WIDTH);//数据通道0接收地址,还作为应答通道
	SPI_Write_buf(WR_TX_PLOAD,txbuf,DATE_WIDTH);
	
	SPI_Write_reg(WRITE_REG+EN_AA,0X01);                    //允许数据通道0自动应答
	SPI_Write_reg(WRITE_REG+EN_RXADDR,0X01);                //接收数据通道0允许
	SPI_Write_reg(WRITE_REG+SETUP_RETR,0X1a);               //等待500+86us，自动重发15次
	SPI_Write_reg(WRITE_REG+RF_CH,CHANNEL);                  //选择射频通道
	SPI_Write_reg(WRITE_REG+RF_SETUP,0X07);                 //数据传输速率1Mb/S,发射功率0dBm（最大）,低噪声放大器增益
	SPI_Write_reg(WRITE_REG+CONFIG,0X0E);                   //RC使能，16位CRC校验，上电发射模式
	CE = 1;
	delay1ms();
}

/*uchar CHECK_ACK(void)    //主机为接收模式不需要检测是否发送完成
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
/*NRF读取2个数据包*/
void NRF_RxPacket(uchar *rxbuf0,uchar *rxbuf1)
{
	uchar sta;
	while(IRQ);  //可去
	sta = SPI_Read_byte(STATUS);
	if(sta&RX_OK)  //接收到数据
	{
		if(((sta&0x0e)>>1) == 0)    //判断是否为数据通道0接收到数据
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
		else if(((sta&0x0e)>>1) == 1)  //是否为数据通道1接收到数据
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