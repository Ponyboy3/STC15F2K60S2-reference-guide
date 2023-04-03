/*******************************************************************************
* 文件名称：DS18B20温度传感器实验
* 实验目的：1.掌握单总线通讯基本特点和工作时序
*           2.掌握51单片机模拟单总线时序的程序设计方法
*           3.掌握DS18B20温度传感器的操作方法
* 实验配置：J3跳线配置为IO方式，J5配置为BTN、J2配置为1-3和2-4
* 程序现象：用手触摸温度传感器，数码管显示有变化
* 硬件说明：IAP15F2K61S2@11.0592MHz
* 注意事项：测量精度为1摄氏度
* 日期版本：2022/02/08
*******************************************************************************/

#include "reg52.h"  //定义51单片机特殊功能寄存器
#include "onewire.h"  //单总线函数库
#include "absacc.h"

sfr AUXR = 0x8E;

code unsigned char tab[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90, 0xff};
unsigned char dspbuf[8] = {10, 10, 10, 10, 10, 10, 10, 10}; //显示缓冲区

bit temper_flag = 0;//温度读取标志
//函数声明
/**********************************/
void display(void);
void cls_buzz();
void cls_led();
/**********************************/

//主函数
void main(void)
{
    unsigned char temperature;
    cls_buzz();
    cls_led();
    AUXR |= 0x80;
    TMOD &= 0xF0;
    TL0 = 0xCD;	//1ms,1Tmoshi
    TH0 = 0xD4;
    TF0 = 0;
    TR0 = 1;
    ET0 = 1;
    EA = 1;

    while(1)
    {
        if(temper_flag)
        {
            temper_flag = 0;
            temperature = rd_temperature();  //读温度
        }

        //显示数据更新
        (temperature >= 10) ? (dspbuf[6] = temperature / 10) : (dspbuf[6] = 10);
        dspbuf[7] = temperature % 10;
    }
}

//定时器中断服务函数
void isr_timer_0(void)  interrupt 1  //默认中断优先级 1
{
    static unsigned char intr;
    if(++intr == 50)  //1ms执行一次
    {
        intr = 0;
        temper_flag = 1;  //50ms温度读取标志位置1
    }
    display();
}

void cls_buzz()
{
    P2 = ((P2 & 0x1f) | 0xA0);
    P0 = 0x00;
    P2 &= 0x1f;
}
void cls_led()
{
    P2 = ((P2 & 0x1f) | 0x80);
    P0 = 0xFF;
    P2 &= 0x1f;
}

//显示函数
void display(void)
{
    static unsigned char dspcom = 0;
    P2 = ((P2 & 0x1f) | 0xE0); //段选573
    P0 = 0xff;               //消隐段码
    P2 &= 0x1f;

    P0 = 1 << dspcom;	       //循环位选
    P2 = ((P2 & 0x1f) | 0xC0); //位选573
    P2 &= 0x1f;

    P0 = tab[dspbuf[dspcom]];//显示段码
    P2 = ((P2 & 0x1f) | 0xE0); //段选573
    P2 &= 0x1f;

    if(++dspcom == 8)
    {
        dspcom = 0;
    }
}

