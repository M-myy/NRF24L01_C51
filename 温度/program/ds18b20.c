#include"ds18b20.h"

bit DS18B20_Init(void)   //��ʼ��1-Wire���ߣ������DS18B20�Ƿ���ڣ�֪ͨ������׼��
{
	bit ACK;
	
	EA = 0;            //�ر����жϣ���ֹ��ʼ�����̱����
	DS_IO = 0;         //���踴λ����
	delay500us();
	DS_IO = 1;         
	delay60us(); 	   //������������ŵ�ƽ����
	ACK = DS_IO;       //�ɼ���������ACK =0
	while(!DS_IO);     //�ȴ�ֱ��û�д�������
	EA = 1;            //���¿������ж�
	
	return ~ACK;     //����ACK,~ACK=1��ɹ�
}

bit Enable_DS(void)    //����һ���¶�ת��
{
	bit ACK;
	
	ACK = DS18B20_Init();
	if(ACK)
	{
		Write_DS(SKIPROM);    //����������ֻ��һ�����������������
		Write_DS(CONVERT);    //�����¶�ת��ָ��
	}
	return ACK;
}

void Write_DS(unsigned char dat)    //��DS18B20д������
{
	unsigned char mask;
	
	EA = 0;                          //�ر����жϷ�ֹ���� 
	for(mask=0x01;mask!=0;mask<<=1)    //�����λ��ʼ��ȡ
	{
		DS_IO = 0;         //����ͨ������
		delay2us();       //����1΢���ſ��������д��1������ʱ2΢��
		if((mask&dat)==0)    //���д������        
		{DS_IO = 0;}
		else 
		{DS_IO = 1;}
		delay60us();      //��ʱDS18B20����ʱ��
		DS_IO = 1;         //��������ͨ�����ţ�Ϊ�´�д����׼��
	}
	EA = 1;                         //���¿������ж�
}

unsigned char Read_DS(void)    //��ȡDS18B20����      
{
	unsigned char mask,dat;
	
	EA = 0;
	for(mask=0x01;mask!=0;mask<<=1)    
	{
		DS_IO = 0;                       //����ͨ������
		delay2us();
		DS_IO = 1;                       //����ͨ������
		delay2us();
		if(!DS_IO)
			dat &= ~mask;                  
		else
			dat |= mask;
		delay60us();
	}
	EA = 1;
	
	return dat;                        //���ض�ȡ��������
}

bit Read_Temp(int *temp)    //��ȡ�¶ȣ�����ֵΪ�Ƿ�ɹ�������������ֵ���浽�β���
{
	bit ACK;
  unsigned char MSB,LSB;
	
	ACK = DS18B20_Init();           //����Ƿ�ת���ɹ�
	if(ACK)
	{
		Write_DS(SKIPROM);         //�������
		Write_DS(READ);            //������
		LSB = Read_DS();           //�����ݵ�ʱ���ȶ������ǵ��ֽں��Ǹ��ֽڣ�ֱ������Ϊֹ
		MSB = Read_DS();
		*temp = ((int)MSB<<8)+LSB;    //���ֽ����ư�λ������ֽ����16λ����
	}
	
	return ACK;                  //��ȡ�ɹ�Ӧ��λ
}