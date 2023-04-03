/*******************************************************************************
* 文件名称：串口通讯实验
* 实验目的：1.掌握51单片机串口工作模式及相关寄存器配置方法
*           2.了解51单片机波特率的计算方法
* 实验配置：J3跳线配置为MM方式，J5配置为BTN、J2配置为1-3和2-4
* 程序现象：板子通过串口将数据发送到电脑
* 硬件说明：IAP15F2K61S2@11.0592MHz
* 注意事项：1.通过USB连接线连接PC机，在计算机硬件管理器中查看串口号
*           2.打开串口调试工具软件，并将串口通讯波特率设定为9600
* 日期版本：2022/02/08
*******************************************************************************/

#include "STC15F2K60S2.H"  //定义51单片机特殊功能寄存器
#include "intrins.h"
#include "absacc.h"
#define BAUD	     9600  //波特率
#define SYSTEMCLOCK  11059200L  //系统时钟频率


unsigned char *number = "0123456789";
unsigned char *string = "GXCT_TEST";
unsigned char i = 96;


//函数声明
/*******************************************/
void delay();
void cls_buzz(void);
void uart_sendstring(unsigned char *str);
/*******************************************/

//主函数
void main(void)
{
    cls_buzz();
    SCON = 0x50;
    AUXR = 0x40;
    TMOD = 0x00;
    TL1 = (65536 - (SYSTEMCLOCK / 4 / BAUD));
    TH1 = (65536 - (SYSTEMCLOCK / 4 / BAUD)) >> 8;
    TR1 = 1;
    while(1)
    {
        uart_sendstring("\r\n");
        uart_sendstring(number);
        uart_sendstring("\r\n");
        uart_sendstring(string);
        uart_sendstring("\r\n");
        i++;
        if(i == 123)
            i = 96;
        delay();
    }
}

void delay()
{
    unsigned char i, j, k;

    _nop_();
    _nop_();
    i = 43;
    j = 6;
    k = 203;
    do
    {
        do
        {
            while (--k);
        }
        while (--j);
    }
    while (--i);
}

void cls_buzz(void)
{
    XBYTE[0xA000] = 0;
}

//通过串口发送字符串
void uart_sendstring(unsigned char *str)
{
    unsigned char *p;

    p = str;
    while(*p != '\0')
    {
        SBUF = *p;
        while(TI == 0);  //等待发送标志位置位
        TI = 0;
        p++;
    }
}

