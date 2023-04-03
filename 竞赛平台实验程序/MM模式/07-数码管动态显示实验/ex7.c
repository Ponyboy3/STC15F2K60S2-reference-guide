/*******************************************************************************
* 文件名称：数码管控制实验
* 实验目的：1.掌握数码管动态扫描的基本原理,掌握数码管消除“鬼影”的方法
*           2.掌握51单片机定时器工作模式和配置方法
*           3.掌握keil uvision集成开发环境下，中断服务函数设计方法
* 实验配置：J3跳线配置为MM方式，J5配置为BTN、J2配置为1-3和2-4
* 程序现象：数码管动态显示
* 硬件环境：IAP15F2K61S2@11.0592MHz
* 注意事项：理解动态显示和静态显示
* 日期版本：2022/02/08
*******************************************************************************/
#include "reg52.h"  //定义51单片机特殊功能寄存器
#include "absacc.h"

sfr AUXR = 0x8E;    //定义辅助寄存器，因为在51单片机头文件中没有定义
                           //0     1      2      3     4     5     6     7      8    9    熄灭
code unsigned char tab[] = { 0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90, 0xFF};
unsigned char dspbuf[8] = {10, 10, 10, 10, 10, 10, 10, 10}; //显示缓冲区
unsigned char dspcom = 0;  //显示位选

unsigned char ms = 0;
unsigned char counter = 0;	//200ms 加1,最大值255

void display(void);

void cls_buzz(void)
{
    XBYTE[0xA000] = 0x00;
}

void cls_led(void)
{
    XBYTE[0x8000] = 0xFF;
}

//主函数
void main(void)
{
    cls_buzz();
    cls_led();

    AUXR |= 0x80;	//定时模式下，1T模式，IAP15F2K61S2单片机特殊功能寄存器不分频
    TMOD &= 0xF0;      //T1在模式3下被禁止。T0工作在方式1下，16位自动重装定时器
    TL0 = 0xCD;	    	//1ms定时
    TH0 = 0xD4;
    TF0 = 0;
    TR0 = 1;
    ET0 = 1;
    EA = 1;

    while(1)
    {
        if(counter >= 100)
        {
            dspbuf[0] = counter / 100;
        }
        else
        {
            dspbuf[0] = 10;  //10对应熄灭
        }
        if(counter >= 10)
        {
            dspbuf[1] = counter % 100 / 10;
        }
        else
        {
            dspbuf[1] = 10;
        }
        dspbuf[2] = counter % 10;
    }
}

//定时器中断服务函数
void isr_timer_0(void)  interrupt 1  //默认中断优先级 1
{
    if(++ms == 200)
    {
        ms = 0;
        counter++;
    }
    display();  //1ms执行一次
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

