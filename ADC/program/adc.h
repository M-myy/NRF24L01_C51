#ifndef _ADC_H_
#define _ADC_H_
#include<stc12c5a60s2.h>
#include"delay.h"

void ADC_Init(void);                //��ʼ��ADC
unsigned char Get_ADC_Res(void);    //��ȡADC����
void tran(void);                    //ת��ADC����

#endif