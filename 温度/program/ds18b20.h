#ifndef _DS18B20_H_
#define _DS18B20_H_
#include<STC12C5A60S2.H>
#include"delay.h"

/*ROM��RAM���ָ��*/
#define SKIPROM 0XCC
#define READ 0XBE
#define CONVERT 0X44

sbit DS_IO = P3^5;
bit DS18B20_Init(void);               //��ʼ��
bit Enable_DS(void);                  //�����¶�ת��
void Write_DS(unsigned char dat);      //��DS18B20д������
unsigned char Read_DS(void);          //��ȡDS18B20���ݲ����� 
bit Read_Temp(int *temp);    //��ȡ�¶ȣ�����ֵΪ�Ƿ�ɹ�������������ֵ���浽�β���      
#endif