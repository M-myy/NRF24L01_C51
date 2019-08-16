#ifndef _DS18B20_H_
#define _DS18B20_H_
#include<STC12C5A60S2.H>
#include"delay.h"

/*ROM与RAM相关指令*/
#define SKIPROM 0XCC
#define READ 0XBE
#define CONVERT 0X44

sbit DS_IO = P3^5;
bit DS18B20_Init(void);               //初始化
bit Enable_DS(void);                  //启动温度转换
void Write_DS(unsigned char dat);      //向DS18B20写入数据
unsigned char Read_DS(void);          //读取DS18B20数据并返回 
bit Read_Temp(int *temp);    //读取温度，返回值为是否成功，并将读到的值储存到形参中      
#endif