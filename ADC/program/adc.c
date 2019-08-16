#include"adc.h"

extern unsigned char TX_buf[3];    //׼�����͵����ݰ�

/**
*��������ADC��ʼ������
*�βΣ���
*����ֵ����
*/
void ADC_Init(void)
{
	P1ASF = 0X01;    //P1^0����ΪADCģ�⹦��A/Dʹ��
	ADC_RES = 0;    //���ADC��A/Dת������Ĵ���
	ADC_CONTR = 0X88;    //��A/Dת������Դ������ģ��ת��������0�жϱ�־λ��540��ʱ������ת��һ��
	AUXR1 = 0X00;    //10λADCת������ĸ߰�λ�����ADC_RES�У���2λ����ADC_RESL��
	delay_us(1);
}

/**
*����������ȡADC��ֵ
*�βΣ���
*����ֵ��ת���õ���ADC��ֵ
*/
unsigned char Get_ADC_Res(void)
{
	unsigned char res;
	ADC_CONTR = 0X88;    //��A/Dת������Դ������ģ��ת��������0�жϱ�־λ��540��ʱ������ת��һ��
	delay_us(1);
	while(!(ADC_CONTR&0X10));    //�ȴ�ת������жϱ�־λ��λ
	ADC_CONTR &= 0XEF;    //�����жϱ�־λ
	res = ADC_RES;    //��ȡ�߰�λ���
	return res;    //������ֵ
}

/**
*��������ת��ADC��ֵ
*�βΣ���
*����ֵ����ADC��ֵת���洢
*/
void tran(void)
{
	unsigned char dat = Get_ADC_Res();  //��ȡADC��ֵ
	TX_buf[0] = dat/100 + '0';     //ȡ��λ
	TX_buf[1] = (dat%100)/10 + '0';  //ȡʮλ
	TX_buf[2] = dat%10 + '0';    //ȡ��λ
}