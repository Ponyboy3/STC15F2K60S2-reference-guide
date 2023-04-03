/*******************************************************************************
* 文件名称：外部中断实验
*  实验目的：1.掌握51单片机外部中断相关寄存器的配置方法和触发方式
*            2.掌握keil uVision 环境下外部中断服务函数的设计方法
* 实验配置：J3跳线配置为MM方式，J5配置为BTN、J2配置为1-3和2-4
* 程序现象：按键S4、S5改变数码管显示数值
* 硬件环境：IAP15F2K61S2@11.0592MHz
* 注意事项：s4代表外部中断1、s5代表外部中断0
* 日期版本：2022/02/08
*******************************************************************************/

#include "reg52.h"  //定义51单片机特殊功能寄存器
#include "absacc.h"
#include "intrins.h"

unsigned char x  = 0;
code unsigned char tab[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90};

void cls_buzz()
{
    XBYTE[0xA000] = 0x00;
}


//主函数
void main(void)
{

    cls_buzz();
    EX0 = 1;
    IT0 = 1;  //边沿触发方式(下降沿)

    EX1 = 1;
    IT1 = 1;  //边沿触发方式(下降沿)

    EA = 1;  //开启总中断
    while(1)
    {
        XBYTE[0xE000] = tab[x];  //显示数值   段选
        XBYTE[0xC000] = 0x55;    //位选
    }
}
void isr_intr_0(void) interrupt 0

{
    x = x + 1;
    if(x == 10)
        x = 0;

}
void isr_intr_1(void) interrupt 2
{
    x = x - 1;
    if(x >= 10)
        x = 0;
}

