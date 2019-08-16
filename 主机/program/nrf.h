#ifndef _NRF_H_
#define _NRF_H_
#include<STC12C5A60S2.H>
#include"delay.h"

/*NRF相关命令宏定义*/
#define READ_REG    0x00    //读配置寄存器
#define WRITE_REG   0x20    //写配置寄存器
#define RD_RX_PLOAD 0x61    //读RX有效数据，用于RX模式
#define WR_TX_PLOAD 0xA0    //写TX有效数据，准备发送，用于TX模式
#define FLUSH_TX    0xE1    //清除TX FIFO寄存器，应用于发射模式下
#define FLUSH_RX    0xE2    //清楚RX FIFO寄存器，应用于接收模式下
#define REUSE_TX_PL 0xE3    //应用于发射端，重新使用上一包发射的有效数据，当CE=1时数据不断被重新发送
#define NOP         0xFF    //空操作，可用来读状态寄存器

/*相关寄存器地址宏定义*/
#define CONFIG      0x00    //配置寄存器地址    
#define EN_AA       0x01    //使能数据通道(0)自动应答
#define EN_RXADDR   0x02    //接收地址允许
#define SETUP_AW    0x03    //设置所有数据通道地址宽度
#define SETUP_RETR  0x04    //建立自动重发
#define RF_CH       0x05    //射频通道
#define RF_SETUP    0x06    //射频寄存器
#define STATUS      0x07    //状态寄存器
#define OBSERVE_TX  0x08    //发送检测寄存器
#define CD          0x09    //载波检测
#define RX_ADDR_P0  0x0A    //数据通道0接收地址
#define RX_ADDR_P1  0x0B    //数据通道1接收地址
#define RX_ADDR_P2  0x0C    //数据通道2接收地址
#define RX_ADDR_P3  0x0D    //数据通道3接收地址
#define RX_ADDR_P4  0x0E    //数据通道4接收地址
#define RX_ADDR_P5  0x0F    //数据通道5接收地址
#define TX_ADDR     0x10    //发送地址，先写低字节
#define RX_PW_P0    0x11    //接收数据通道0有效数据宽度设置
#define RX_PW_P1    0x12    //接收数据通道1有效数据宽度设置
#define RX_PW_P2    0x13    //接收数据通道2有效数据宽度设置
#define RX_PW_P3    0x14    //接收数据通道3有效数据宽度设置
#define RX_PW_P4    0x15    //接收数据通道4有效数据宽度设置
#define RX_PW_P5    0x16    //接收数据通道5有效数据宽度设置
#define FIFO_STATUS 0x17    //FIFO状态寄存器

#define ADDR_WIDTH  5       //通道地址长度，固定五个字节
#define DATE_WIDTH  32      //数据包长度（最大为32个字节）
#define CHANNEL     40      //选择通信通道（共125个）

#define MAX_TX  	0x10  //达到最大发送次数中断
#define TX_OK   	0x20  //TX发送完成中断
#define RX_OK   	0x40  //接收到数据中断

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