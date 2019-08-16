#include"ds18b20.h"

bit DS18B20_Init(void)   //初始化1-Wire总线，并检测DS18B20是否存在，通知其做好准备
{
	bit ACK;
	
	EA = 0;            //关闭总中断，防止初始化过程被打断
	DS_IO = 0;         //给予复位脉冲
	delay500us();
	DS_IO = 1;         
	delay60us(); 	   //上拉电阻把引脚电平拉高
	ACK = DS_IO;       //采集存在脉冲ACK =0
	while(!DS_IO);     //等待直到没有存在脉冲
	EA = 1;            //重新开启总中断
	
	return ~ACK;     //返回ACK,~ACK=1则成功
}

bit Enable_DS(void)    //启动一次温度转换
{
	bit ACK;
	
	ACK = DS18B20_Init();
	if(ACK)
	{
		Write_DS(SKIPROM);    //由于总线上只有一个器件，故跳过检测
		Write_DS(CONVERT);    //启动温度转换指令
	}
	return ACK;
}

void Write_DS(unsigned char dat)    //向DS18B20写入数据
{
	unsigned char mask;
	
	EA = 0;                          //关闭总中断防止干扰 
	for(mask=0x01;mask!=0;mask<<=1)    //从最低位开始读取
	{
		DS_IO = 0;         //拉低通信引脚
		delay2us();       //至少1微秒后才可向控制器写入1，故延时2微秒
		if((mask&dat)==0)    //检测写入数据        
		{DS_IO = 0;}
		else 
		{DS_IO = 1;}
		delay60us();      //延时DS18B20采样时间
		DS_IO = 1;         //重新拉高通信引脚，为下次写入做准备
	}
	EA = 1;                         //重新开启总中断
}

unsigned char Read_DS(void)    //读取DS18B20数据      
{
	unsigned char mask,dat;
	
	EA = 0;
	for(mask=0x01;mask!=0;mask<<=1)    
	{
		DS_IO = 0;                       //拉低通信引脚
		delay2us();
		DS_IO = 1;                       //拉高通信引脚
		delay2us();
		if(!DS_IO)
			dat &= ~mask;                  
		else
			dat |= mask;
		delay60us();
	}
	EA = 1;
	
	return dat;                        //返回读取到的数据
}

bit Read_Temp(int *temp)    //读取温度，返回值为是否成功，并将读到的值储存到形参中
{
	bit ACK;
  unsigned char MSB,LSB;
	
	ACK = DS18B20_Init();           //检测是否转换成功
	if(ACK)
	{
		Write_DS(SKIPROM);         //跳过检测
		Write_DS(READ);            //读命令
		LSB = Read_DS();           //读数据的时候先读到的是低字节后是高字节，直到读完为止
		MSB = Read_DS();
		*temp = ((int)MSB<<8)+LSB;    //高字节左移八位，与低字节组成16位数据
	}
	
	return ACK;                  //读取成功应答位
}