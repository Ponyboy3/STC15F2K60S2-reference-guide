/*******************************************************************************
* 文件名称：DS1302时钟驱动实验
* 实验目的：1.掌握数码管动态扫描的基本原理,掌握数码管消除“鬼影”的方法
*           2.掌握51单片机定时器工作模式和配置方法
*           3.掌握keil uvision集成开发环境下，中断服务函数设计方法
*			      4.掌握DS1302时钟芯片驱动和工作模式配置的方法
* 实验配置：J3跳线配置为MM方式
* 程序现象：数码管动态显示
* 硬件环境：IAP15F2K61S2@11.0592MHz
* 注意事项：1.DS1302实验过程中应避免用手接触时钟外部晶振，外部的干扰可能导致晶振停
			        止振荡，造成走时异常
			      2.不同的晶振启振时间存在差异，建议上电初始化后，延迟小段时间，再对RTC
			        芯片进行操作
* 日期版本：2022/02/08
*******************************************************************************/
#include "reg52.h"  //定义51单片机特殊功能寄存器
#include "absacc.h"
#include "ds1302.h"

sfr AUXR = 0x8E;    //定义辅助寄存器
                           //  0     1    2     3     4     5     6     7     8     9    10     熄灭
code unsigned char tab[] = { 0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90, 0xFF, 0xbf};
unsigned char dspbuf[8] = {10, 10, 10, 10, 10, 10, 10, 10}; //显示缓冲区
unsigned char dspcom = 0;  //显示位选

unsigned char ms = 0;
bit rtc_flag = 0;


void cls_buzz(void)
{
    XBYTE[0xA000] = 0x00;
}

void cls_led(void)
{
    XBYTE[0x8000] = 0xFF;
}


void Delay100ms()		//@11.0592MHz
{
    unsigned char i, j, k;

    _nop_();
    _nop_();
    i = 5;
    j = 52;
    k = 195;
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
    unsigned char *p_rtc;

    cls_buzz();
    cls_led();
    AUXR |= 0x80;
    TMOD &= 0xF0;
    TL0 = 0xCD;
    TH0 = 0xD4;
    TF0 = 0;
    TR0 = 1;
    ET0 = 1;
    EA = 1;

    Delay100ms();
    SetRTC();


    while(1)
    {
        if(rtc_flag)
        {
            rtc_flag = 0;
            p_rtc = ReadRTC();
            dspbuf[0] = p_rtc[0];
            dspbuf[1] = p_rtc[1];
            dspbuf[2] = 11;
            dspbuf[3] = p_rtc[2];
            dspbuf[4] = p_rtc[3];
            dspbuf[5] = 11;
            dspbuf[6] = p_rtc[4];
            dspbuf[7] = p_rtc[5];
        }
    }
}

//显示函数
void display(void)
{
    XBYTE[0xE000] = 0xff;  //消隐
    XBYTE[0xC000] = (1 << dspcom);
    XBYTE[0xE000] = tab[dspbuf[dspcom]]; //段码 挨个显示缓冲区的数字 8ms刷新一次，

    if(++dspcom == 8)
    {
        dspcom = 0;
    }
}

//定时器中断服务函数
void isr_timer_0(void)  interrupt 1  //默认中断优先级 1
{
    if(++ms == 200)
    {
        ms = 0;
        rtc_flag = 1;
    }
    display();  //1ms执行一次
}


