/*******************************************************************************
* 文件名称：LED移位控制实验
* 实验目的：1.掌握单片机IO口操作的基本方法
*           2.掌握移位运算符的使用方法
* 实验配置：J3跳线配置为MM方式，J5配置为KBD、J2配置为1-3和2-4
* 实验现象：LED以0.2S为间隔移位操作
* 硬件环境：IAP15F2K61S2单片机，单片机内部振荡器频率：11.0592M
* 注意事项：要用到_nop_()必须要包含intrins.h头文件
* 日期版本：2022-2-08
*******************************************************************************/
#include "reg52.h"  //定义51单片机特殊功能寄存器
#include "intrins.h"
#include "absacc.h"
sbit LCD_E = P1 ^ 7;

void cls_buzz(void)
{
    XBYTE[0xA000] = 0x00;
}

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

    unsigned char s;
    cls_buzz();

    LCD_E = 0;
    while(1)
    {
        for(s = 0; s < 8; s++)
        {
            XBYTE[0x8000] = 0xfe << s;
            delay();


        }
    }
}