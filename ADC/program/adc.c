#include"adc.h"

extern unsigned char TX_buf[3];    //准备发送的数据包

/**
*函数名：ADC初始化函数
*形参：无
*返回值：无
*/
void ADC_Init(void)
{
	P1ASF = 0X01;    //P1^0口作为ADC模拟功能A/D使用
	ADC_RES = 0;    //清空ADC的A/D转换结果寄存器
	ADC_CONTR = 0X88;    //打开A/D转换器电源，启动模数转换器，清0中断标志位，540个时钟周期转换一次
	AUXR1 = 0X00;    //10位ADC转化结果的高八位存放在ADC_RES中，低2位存在ADC_RESL中
	delay_us(1);
}

/**
*函数名：获取ADC数值
*形参：无
*返回值：转换得到的ADC数值
*/
unsigned char Get_ADC_Res(void)
{
	unsigned char res;
	ADC_CONTR = 0X88;    //打开A/D转换器电源，启动模数转换器，清0中断标志位，540个时钟周期转换一次
	delay_us(1);
	while(!(ADC_CONTR&0X10));    //等待转换完成中断标志位置位
	ADC_CONTR &= 0XEF;    //清零中断标志位
	res = ADC_RES;    //获取高八位结果
	return res;    //返回数值
}

/**
*函数名：转换ADC数值
*形参：无
*返回值：将ADC数值转换存储
*/
void tran(void)
{
	unsigned char dat = Get_ADC_Res();  //获取ADC数值
	TX_buf[0] = dat/100 + '0';     //取百位
	TX_buf[1] = (dat%100)/10 + '0';  //取十位
	TX_buf[2] = dat%10 + '0';    //取个位
}