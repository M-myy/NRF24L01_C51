#include"nrf.h"

const uchar RX_ADDR0[] = {0x43,0x43,0x10,0x10,0x01};    //通道0接收地址
const uchar TX_ADDR0[] = {0x43,0x43,0x10,0x10,0x01};    //从机0发射地址

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
 	  MOSI=(dat&0x80);    //写入第一位数据，高位发送
 	  dat=(dat<<1);    //待写入数据左移一位，产生下一个最高位
 	  SCK=1;        //拉高SCK（时钟信号）引脚
 	  dat|=MISO;     //捕获当前主机读取从机的状态或数据
 	  SCK=0;    //拉低时钟信号
 	}
 	return dat;    //返回读取到的数据
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
	CE = 0;
	SPI_Write_reg(WRITE_REG+CONFIG,0X0D);    //确保为掉电模式，防止干扰（不设置也可以）
	CE = 1;
	delay10us();
	
	CE = 0;         	//写数据前CE必拉低
	SPI_Write_buf(WRITE_REG+RX_ADDR_P0,TX_ADDR0,ADDR_WIDTH); //数据通道0接收地址
	SPI_Write_reg(WRITE_REG+EN_AA,0X01);                     //允许数据通道0自动应答
	SPI_Write_reg(WRITE_REG+EN_RXADDR,0X01);                 //接收数据通道0允许
	SPI_Write_reg(WRITE_REG+RF_CH,CHANNEL);                   //选择射频通道
	SPI_Write_reg(WRITE_REG+RX_PW_P0,DATE_WIDTH);            //接收数据通道0有效宽度设置
	SPI_Write_reg(WRITE_REG+RF_SETUP,0X07);                  //数据传输速率1Mb/S,发射功率0dBm,低噪声放大器增益
	SPI_Write_reg(WRITE_REG+CONFIG,0X0F);                    //CRC使能，16位CRC校验，上电接收模式
	CE = 1; 	//重新拉高CE引脚   
	delay1ms();
}
/*NRF发射模式*/
void NRF_TX_Mode(uchar *txbuf)
{
	CE = 0; 
	SPI_Write_reg(WRITE_REG+CONFIG,0X0D);
	CE = 1;
	delay10us();
	
	CE = 0;
	SPI_Write_buf(WRITE_REG+TX_ADDR,TX_ADDR0,ADDR_WIDTH);   //发射通道地址
	SPI_Write_buf(WRITE_REG+RX_ADDR_P0,TX_ADDR0,ADDR_WIDTH);//数据通道0接收地址,还作为应答通道
	SPI_Write_buf(WR_TX_PLOAD,txbuf,DATE_WIDTH);
	
	SPI_Write_reg(WRITE_REG+EN_AA,0X01);                    //允许数据通道0自动应答
	SPI_Write_reg(WRITE_REG+EN_RXADDR,0X01);                //接收数据通道0允许
	SPI_Write_reg(WRITE_REG+SETUP_RETR,0X1f);               //等待500+86us，自动重发15次
	SPI_Write_reg(WRITE_REG+RF_CH,CHANNEL);                  //选择射频通道
	SPI_Write_reg(WRITE_REG+RF_SETUP,0X07);                 //数据传输速率1Mb/S,发射功率0dBm（最大）,低噪声放大器增益
	SPI_Write_reg(WRITE_REG+CONFIG,0X0E);                   //RC使能，16位CRC校验，上电发射模式
	CE = 1;
	delay1ms();
}

uchar CHECK_ACK(void)
{
	uchar sta;
	while(IRQ);    //等待发送完成
	sta = SPI_Read_byte(READ_REG+STATUS);    //读取状态寄存器状态
	if((sta&TX_OK)||(sta&MAX_TX))            //判断发送完成还是达到最大重发次数
	{
		SPI_Write_reg(WRITE_REG+STATUS,sta);    //清除中断
		CSN = 0;
		SPI_Write_byte(FLUSH_TX);    //清除FIFO寄存器
		CSN = 1;
		IRQ = 1;    //再次确保IRQ中断引脚被拉高
		return 0;
	}
	IRQ = 1;
	return 1;
}
/******************NRF读取数据包*******************/
void NRF_RxPacket(uchar *rxbuf)
{
	uchar sta;
	sta = SPI_Read_byte(STATUS);    //读取状态寄存器
	if(sta&RX_OK)    //是否发送完成
	{
		CE = 0;
		SPI_Read_buf(RD_RX_PLOAD,rxbuf,DATE_WIDTH);    //读取接收到的数据
		SPI_Write_reg(WRITE_REG+STATUS,sta);    //清除中断标志
		CSN = 0;
		SPI_Write_byte(FLUSH_RX);    //清除RX_FIFO寄存器
		CSN = 1;
		IRQ = 1;    //确保IRQ被拉高（也可不写）
	}
  IRQ = 1;
}

























/**************************************************************另一种配置方式**********************************************************************/
/*NRF接收模式初始化*/                   /*从此以下修改*/
/*void NRF_RX_Mode(void)
{
	CE = 0;
	SPI_Write_reg(WRITE_REG+CONFIG,0X0D);
	CE = 1;
	delay_us(20);
	
	CE = 0;         	//写数据前CE必拉低
	delay_ms(1);
	SPI_Write_buf(WRITE_REG+RX_ADDR_P0,RX_ADDR0,ADDR_WIDTH); //数据通道0接收地址
	SPI_Write_reg(WRITE_REG+EN_AA,0X01);                     //允许数据通道0自动应答
	SPI_Write_reg(WRITE_REG+EN_RXADDR,0X01);                 //接收数据通道0允许
	SPI_Write_reg(WRITE_REG+RF_CH,CHANEL);                   //选择射频通道
	SPI_Write_reg(WRITE_REG+RX_PW_P0,DATE_WIDTH);            //接收数据通道0有效宽度设置
	SPI_Write_reg(WRITE_REG+RF_SETUP,0X07);                  //数据传输速率1Mb/S,发射功率0dBm,低噪声放大器增益
	SPI_Write_reg(WRITE_REG+CONFIG,0X0F);                    //CRC使能，16位CRC校验，上电接收模式
	CE = 1;                                                  //重新拉高CE引脚                                            //给予拉高引脚时间
}
/*NRF发射模式*/
/*void NRF_TX_Mode(void)
{
	CE = 0;
	SPI_Write_reg(WRITE_REG+CONFIG,0X0D);
	CE = 1;
	delay_us(20);
	
	CE = 0;
	SPI_Write_buf(WRITE_REG+TX_ADDR,TX_ADDR0,ADDR_WIDTH);   //发射通道地址
	SPI_Write_buf(WRITE_REG+RX_ADDR_P0,TX_ADDR0,ADDR_WIDTH);//数据通道0接收地址,还作为应答通道
	
	SPI_Write_reg(WRITE_REG+EN_AA,0X01);                    //允许数据通道0自动应答
	SPI_Write_reg(WRITE_REG+EN_RXADDR,0X01);                //接收数据通道0允许
	SPI_Write_reg(WRITE_REG+SETUP_RETR,0X1a);               //等待500+86us，自动重发15次
	SPI_Write_reg(WRITE_REG+RF_CH,CHANEL);                  //选择射频通道
	SPI_Write_reg(WRITE_REG+RF_SETUP,0X07);                 //数据传输速率1Mb/S,发射功率0dBm（最大）,低噪声放大器增益
  
	SPI_Write_reg(WRITE_REG+CONFIG,0X0E);                   //RC使能，16位CRC校验，上电发射模式
	CE = 1;
}

/*检验nrf是否正常工作*/
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
	return 0;    //正常
}
/*NRF读取数据包*/
/*uchar NRF_RxPacket(uchar *rxbuf)
{
	uchar sta;
	
	sta = SPI_Read_byte(STATUS);    //读出状态寄存器的值
	SPI_Write_reg(WRITE_REG+STATUS,sta);     //清除TX_DS或MAX_RT中断
	if(sta&0x40)    //收到数据
	{
		SPI_Read_buf(RD_RX_PLOAD,rxbuf,DATE_WIDTH);
		SPI_Write_reg(FLUSH_RX,0XFF);    //清除FIFO寄存器
		return 0;
	}
	return 1;
}
/*NRF发送数据包*/
/*uchar NRF_TxPacket(uchar *txbuf)
{
	uchar sta;
	CE = 0;
	SPI_Write_buf(WR_TX_PLOAD,txbuf,DATE_WIDTH);    //写入有效数据
	CE = 1;    //启动发送
	while(IRQ!=0);    //等待发送完成
//	while(!(0x20&SPI_Read_byte(READ_REG+STATUS)));
	sta = SPI_Read_byte(STATUS);    //读出状态寄存器的值
	SPI_Write_reg(WRITE_REG+STATUS,sta);     //清除TX_DS或MAX_RT中断
	if(sta&0x10)    //达到重发次数最大值
	{ 
		SPI_Write_reg(FLUSH_TX,0XFF);    //清除FIFO寄存器
		return 0x10;
	}
	if(sta&0x20)    //发送完成
		return 0x20;
	return 0xff;    //发送失败
}*/