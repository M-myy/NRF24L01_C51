#ifndef _ADC_H_
#define _ADC_H_
#include<stc12c5a60s2.h>
#include"delay.h"

void ADC_Init(void);                //初始化ADC
unsigned char Get_ADC_Res(void);    //获取ADC数据
void tran(void);                    //转换ADC数据

#endif