/*******************************************************************************
* 文件名称：数码管控制实验
* 实验目的：1.掌握数码管驱动电路的设计方法
*           2.掌握数码管显示的基本原理
* 实验配置：J3跳线配置为MM方式，J5配置为BTN、J2配置为1-3和2-4
* 程序现象：数码管跳动显示0-9
* 硬件环境：IAP15F2K61S2@11.0592MHz
* 注意事项：无
* 日期版本：2022/02/08
*******************************************************************************/

#include "reg52.h"  //定义51单片机特殊功能寄存器
#include "absacc.h"
#include "intrins.h"

code unsigned char tab[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90};

void cls_buzz()
{
    XBYTE[0xA000] = 0x00;
}

void delay()	//delay 200ms @ 11.0592M
{
    unsigned char i, j, k;

    _nop_();
    _nop_();
    i = 9;
    j = 104;
    k = 139;
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

//主函数
void main(void)
{
    unsigned char i;
    cls_buzz();
    while(1)
    {
        (i==9)?(i=0):(i++);
        XBYTE[0xE000] = tab[i];  //显示数值1   段选
        XBYTE[0xC000] = 0x55;    //位选
        delay();
        delay();
        delay();
        XBYTE[0xE000] = 0X00;

        XBYTE[0xE000] = tab[i+1];
        XBYTE[0xC000] = 0xAA;
        delay();
        delay();
        delay();
        XBYTE[0xE000] = 0X00;
    }
}


