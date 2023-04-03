/*******************************************************************************
* 文件名称：LED闪烁实验
* 实验目的：1.掌握单片机IO口操作的基本方法
*           2.掌握软件延时函数的设计方法
* 实验配置：J3跳线配置为MM方式
* 实验现象：LED以0.2S为间隔亮灭
* 硬件环境：IAP15F2K61S2单片机，单片机内部振荡器频率：11.0592M
*注意事项：包含STC15F2K60S2.H头文件，里面包含了所有的STC15系列的寄存器定义，
            此头文件可在STC-ISP软件中生成，要用到_nop_()必须要包含intrins.h头文件
* 日期版本：2022-2-08
*******************************************************************************/

#include "reg52.h"
#include "intrins.h"
#include "absacc.h"

sbit LCD_E = P1 ^ 7;
void delay(void) //@11.0592MHz
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


void main(void)
{
    LCD_E = 0;
    while(1)
    {
        XBYTE[0x8000] = 0x00;
        delay();

        XBYTE[0x8000] = 0xff;
        delay();


    }
}