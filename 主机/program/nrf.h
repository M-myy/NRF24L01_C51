#ifndef _NRF_H_
#define _NRF_H_
#include<STC12C5A60S2.H>
#include"delay.h"

/*NRF�������궨��*/
#define READ_REG    0x00    //�����üĴ���
#define WRITE_REG   0x20    //д���üĴ���
#define RD_RX_PLOAD 0x61    //��RX��Ч���ݣ�����RXģʽ
#define WR_TX_PLOAD 0xA0    //дTX��Ч���ݣ�׼�����ͣ�����TXģʽ
#define FLUSH_TX    0xE1    //���TX FIFO�Ĵ�����Ӧ���ڷ���ģʽ��
#define FLUSH_RX    0xE2    //���RX FIFO�Ĵ�����Ӧ���ڽ���ģʽ��
#define REUSE_TX_PL 0xE3    //Ӧ���ڷ���ˣ�����ʹ����һ���������Ч���ݣ���CE=1ʱ���ݲ��ϱ����·���
#define NOP         0xFF    //�ղ�������������״̬�Ĵ���

/*��ؼĴ�����ַ�궨��*/
#define CONFIG      0x00    //���üĴ�����ַ    
#define EN_AA       0x01    //ʹ������ͨ��(0)�Զ�Ӧ��
#define EN_RXADDR   0x02    //���յ�ַ����
#define SETUP_AW    0x03    //������������ͨ����ַ���
#define SETUP_RETR  0x04    //�����Զ��ط�
#define RF_CH       0x05    //��Ƶͨ��
#define RF_SETUP    0x06    //��Ƶ�Ĵ���
#define STATUS      0x07    //״̬�Ĵ���
#define OBSERVE_TX  0x08    //���ͼ��Ĵ���
#define CD          0x09    //�ز����
#define RX_ADDR_P0  0x0A    //����ͨ��0���յ�ַ
#define RX_ADDR_P1  0x0B    //����ͨ��1���յ�ַ
#define RX_ADDR_P2  0x0C    //����ͨ��2���յ�ַ
#define RX_ADDR_P3  0x0D    //����ͨ��3���յ�ַ
#define RX_ADDR_P4  0x0E    //����ͨ��4���յ�ַ
#define RX_ADDR_P5  0x0F    //����ͨ��5���յ�ַ
#define TX_ADDR     0x10    //���͵�ַ����д���ֽ�
#define RX_PW_P0    0x11    //��������ͨ��0��Ч���ݿ������
#define RX_PW_P1    0x12    //��������ͨ��1��Ч���ݿ������
#define RX_PW_P2    0x13    //��������ͨ��2��Ч���ݿ������
#define RX_PW_P3    0x14    //��������ͨ��3��Ч���ݿ������
#define RX_PW_P4    0x15    //��������ͨ��4��Ч���ݿ������
#define RX_PW_P5    0x16    //��������ͨ��5��Ч���ݿ������
#define FIFO_STATUS 0x17    //FIFO״̬�Ĵ���

#define ADDR_WIDTH  5       //ͨ����ַ���ȣ��̶�����ֽ�
#define DATE_WIDTH  32      //���ݰ����ȣ����Ϊ32���ֽڣ�
#define CHANNEL     40      //ѡ��ͨ��ͨ������125����

#define MAX_TX  	0x10  //�ﵽ����ʹ����ж�
#define TX_OK   	0x20  //TX��������ж�
#define RX_OK   	0x40  //���յ������ж�

sbit CSN = P1^4;
sbit MOSI = P1^5;
sbit MISO = P1^6;
sbit SCK = P1^7;
sbit IRQ = P3^3;
sbit CE = P3^4;

typedef unsigned char uchar;
typedef unsigned int uint;

void SPI_Init(void);
uchar SPI_Write_byte(uchar dat);
uchar SPI_Write_reg(uchar reg,uchar dat);
uchar SPI_Read_byte(uchar reg);
uchar SPI_Write_buf(uchar reg,uchar *buf,uchar num);
uchar SPI_Read_buf(uchar reg,uchar *buf,uchar num);
void NRF_RX_Mode(void);
void NRF_TX_Mode(uchar *txbuf);
uchar CHECK_ACK(void);
void NRF_RxPacket(uchar *rxbuf0,uchar *rxbuf1);
bit NRF_CHECK(void);

#endif