/*******************************************************************************
* 文件名称：DS18B20温度传感器实验-小数点处理
* 实验目的：1.掌握常用的字符串处理函数
*           2.掌握DS18B20温度传感器的操作方法
* 实验配置：J3跳线配置为MM方式，J5配置为BTN、J2配置为1-3和2-4
* 程序现象：板子通过串口将温度检测数值发送到电脑
* 硬件环境：IAP15F2K61S2@11.0592MHz
* 程序说明：1.通过串口输出实时温度数据，使用串口调试工具接收数据
*           2.串口通讯波特率设定为2400
* 日期版本：2022/02/08
*******************************************************************************/
#include "STC15F2K60S2.H"  //定义51单片机特殊功能寄存器
#include "onewire.h"  //单总线函数库
#include <stdio.h>
#include "intrins.h"
#include "absacc.h"

#define BAUD	     2400  //波特率
#define SYSTEMCLOCK  11059200L  //系统时钟频率
//函数声明
/*****************************************/
void delay(void);
void cls_buzz();
void uart_sendstring(unsigned char *str);
/*****************************************/

//主函数
void main(void)
{
    float temperature;
    unsigned char str[40];
    cls_buzz();
    SCON = 0x50;
    AUXR = 0x40;                //1T
    TMOD = 0x00;
    TL1 = (65536 - (SYSTEMCLOCK / 4 / BAUD));
    TH1 = (65536 - (SYSTEMCLOCK / 4 / BAUD)) >> 8;
    TR1 = 1;

    while(1)
    {
        sprintf(str, "%s%6.3f%c%c", "temperature:", temperature, '\r', '\n');

        temperature = rd_temperature_f();  //读温度
        uart_sendstring(str);
        delay();
        delay();
    }
}

void delay()		//@11.0592MHz  300ms
{
    unsigned char i, j, k;
    _nop_();
    _nop_();
    i = 13;
    j = 156;
    k = 83;
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