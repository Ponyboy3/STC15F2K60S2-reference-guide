/*******************************************************************************
* 文件名称：超声波测距实验
* 实验目的：1.掌握超声波测距原理
*           2.掌握51单片机定时器工作模式和配置方法
* 实验配置：J13跳线配置为MM模式，J5配置为BTN、J2配置为1-3和2-4
* 实验现象：将超声模块对准被测量的物体，可以测量距离
* 硬件说明：IAP15F2K61S2@11.0592MHz
* 程序说明：设计延时函数，需要根据单片机的时钟频率进行调整,本例工作于11.0592MHz
* 日期版本：2022/02/08
*******************************************************************************/

#include "reg52.h"  //定义51单片机特殊功能寄存器
#include "intrins.h"
#include "absacc.h"

#define somenop {_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_(); _nop_();_nop_();_nop_();_nop_();_nop_();_nop_();}

sbit TX = P1 ^ 0; //发射引脚
sbit RX = P1 ^ 1; //接收引脚

code unsigned char tab[] = { 0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90, 0xff};
unsigned char dspbuf[8] = {10, 10, 10, 10, 10, 10, 10, 10}; //显示缓冲区

bit s_flag;//更新标志位


void send_wave(void);
void display(void);



void main(void)
{
    unsigned int distance;
    unsigned int time = 0;
    TMOD |= 0x11;  //配置定时器工作模式
    TH0 = (65536 - 2000) / 256;
    TL0 = (65536 - 2000) % 256;
    TH1 = 0;
    TL1 = 0;

    EA = 1;
    ET0 = 1;  //打开定时器0中断
    TR0 = 1;  //启动定时器


    while(1)
    {
        /** 200毫秒更新一次数据 */
        if(s_flag)
        {
            s_flag = 0;
            /** 关闭定时器0中断:计算超声波发送到返回的时间 */

            send_wave();  //发送方波信号
            TR1 = 1;  //启动计时
            while((RX == 1) && (TF1 == 0));  //等待收到脉冲，没有计满溢出
            TR1 = 0;  //关闭计时

            //发生溢出
            if(TF1 == 1)
            {
                TF1 = 0;
                distance = 9999;  //无返回
            }
            else
            {
                /**  计算时间  */
                time = TH1;
                time <<= 8;
                time |= TL1;
                distance = (unsigned int)(time * 0.017); //计算距离
            }
            TH1 = 0;
            TL1 = 0;
        }
        /** 数据处理                      */

        dspbuf[5] = distance / 100;
        dspbuf[6] = distance % 100 / 10;
        dspbuf[7] = distance % 10;
    }
}

//定时器0中断服务函数
void isr_timer_0(void)  interrupt 1  //默认中断优先级 1
{
    static unsigned int intr = 0;
    TH0 = (65536 - 2000) / 256;
    TL0 = (65536 - 2000) % 256; //定时器重载
    display();  //2ms执行一次

    if(++intr == 200)
    {
        s_flag = 1;
        intr = 0;
    }
}

//显示函数
void display(void)
{
    static unsigned char dspcom = 0;
    XBYTE[0xE000] = 0xff;  //去除鬼影
    XBYTE[0xC000] = (1 << dspcom);
    XBYTE[0xE000] = tab[dspbuf[dspcom]];
    XBYTE[0xa000] = 0x00;

    if(++dspcom == 8)
    {
        dspcom = 0;
    }
}

//TX引脚发送40KHz方波信号驱动超声波发送探头
//使用软件延时注意RC振荡器频率
void send_wave(void)
{
    unsigned char i = 8;  //发送8个脉冲

    do
    {
        TX = 1;
        somenop;somenop;somenop;somenop;
        somenop;somenop;somenop;somenop;
        somenop;somenop;somenop;somenop;
        somenop;somenop;somenop;somenop;
        TX = 0;
        somenop;somenop;somenop;somenop;
        somenop;somenop;somenop;somenop;
        somenop;somenop;somenop;somenop;
        somenop;somenop;somenop;somenop;
    }
    while(i--);
}
