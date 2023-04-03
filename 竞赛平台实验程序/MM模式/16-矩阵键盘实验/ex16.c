/*******************************************************************************
* 文件名称：矩阵键盘实验
* 实验目的：1.掌握矩阵键盘扫描的基本原理
*           2.掌握按键扫描及消除抖动的基本原理
* 实验配置：J3跳线配置为MM方式，J5配置为KBD、J2配置为1-3和2-4
* 实验现象：按键S7、S15、S19数码管显示00-02
            按键S6、S14、S18数码管显示03-05
            按键S5、S13、S17数码管显示06-08
            按键S4、S12、S16数码管显示09-11
* 硬件说明：IAP15F2K61S2@11.0592MHz，当J13选为MM模式时需要p42口传输信号，
*           而使用按键s11、s10、s9、s8时需要p42确定键盘列值，冲突，所以在此不使用这些按键
* 程序说明：采用状态法进行按键扫描，系统实时性好，且每10ms扫描可以防止抖动。
* 日期版本：2022/02/08
*******************************************************************************/
#include "STC15F2K60S2.H"  //用此头文件直接代替REG52.H，请进行两个头文件对比
#include "absacc.h"

                          //   0    1     2     3     4     5     6     7     8     9    熄灭
code unsigned char tab[] = { 0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90, 0xFF};
unsigned char dspbuf[8] = {10, 10, 10, 10, 10, 10, 10, 10}; //显示缓冲区

bit key_flag = 0;//按键扫描标志位
unsigned char key_value = 0xFF; //按键值
/******函数声明***********************/
void read_keyboard(void);
void display();
/************************************/

void cls_buzz(void)	//关闭蜂鸣器
{
    XBYTE[0xA000] = 0;
}

void cls_led(void)//关闭所有LED
{
    XBYTE[0x8000] = 0xFF;
}

//主函数
void main(void)
{
    cls_buzz();
    cls_led();
    AUXR |= 0x80;	//1T模式，IAP15F2K61S2单片机特殊功能寄存器
    TMOD &= 0xF0;
    TL0 = 0xCD;
    TH0 = 0xD4;
    TF0 = 0;
    TR0 = 1;
    ET0 = 1;
    EA = 1;
    while(1)
    {
        if(key_flag)
        {
            key_flag = 0;
            read_keyboard();
            if(key_value != 0xFF)
            {
                dspbuf[7] = key_value % 10;
                dspbuf[6] = key_value / 10;
            }
        }

    }
}

//定时器中断服务函数
void isr_timer_0(void)  interrupt 1  //默认中断优先级 1
{
    static unsigned char intr = 0;
    display(); //1ms执行一次
    if(++intr == 10)
    {
        intr = 0;
        key_flag = 1;  //10ms按键扫描标志位置1
    }
}

//读取矩阵键盘键值:转接板中使用P42和P44代替8051引脚
//顺序中的P36和P37引脚
void read_keyboard(void)
{
    static unsigned char hang;
    static unsigned char key_state = 0;
    switch(key_state)
    {
    case 0:
    {
        P3 = 0x0f;
        P44 = 0;
        if(P3 != 0x0f) //有按键按下
            key_state = 1;
    }
    break;
    case 1:
    {
        P3 = 0x0f;
        P44 = 0;
        if(P3 != 0x0f) //有按键按下
        {
            if(P30 == 0)hang = 1;
            if(P31 == 0)hang = 2;
            if(P32 == 0)hang = 3;
            if(P33 == 0)hang = 4;//确定行
            switch(hang)
            {
            case 1:
            {
                P3 = 0xf0;
                P44 = 1;
                if(P44 == 0)
                {
                    key_value = 0;
                    key_state = 2;
                }
                else if(P35 == 0)
                {
                    key_value = 1;
                    key_state = 2;
                }
                else if(P34 == 0)
                {
                    key_value = 2;
                    key_state = 2;
                }
            }
            break;
            case 2:
            {
                P3 = 0xf0;
                P44 = 1;
                if(P44 == 0)
                {
                  key_value = 3;
                  key_state = 2;
                }
                else if(P35 == 0)
                {
                    key_value = 4;
                    key_state = 2;
                }
                else if(P34 == 0)
                {
                    key_value = 5;
                    key_state = 2;
                }
            }
            break;
            case 3:
            {
                P3 = 0xf0;
                P44 = 1;
                if(P44 == 0)
                {
                    key_value = 6;
                    key_state = 2;
                }
                else if(P35 == 0)
                {
                    key_value = 7;
                    key_state = 2;
                }
                else if(P34 == 0)
                {
                    key_value = 8;
                    key_state = 2;
                }
            }
            break;
            case 4:
            {
                P3 = 0xf0;
                P44 = 1;
                if(P44 == 0)
                {
                    key_value = 9;
                    key_state = 2;
                }
                else if(P35 == 0)
                {
                    key_value = 10;
                    key_state = 2;
                }
                else if(P34 == 0)
                {
                    key_value = 11;
                    key_state = 2;
                }
            }
            break;
            }
        }
        else
        {
            key_state = 0;
        }
    }
    break;
    case 2:
    {
        P3 = 0x0f;
        P44 = 0;
        if(P3 == 0x0f) //按键放开
            key_state = 0;
    }
    break;

    }

}


void display(void)
{
    static unsigned char dspcom = 0;
    XBYTE[0xE000] = 0xff;
    XBYTE[0xC000] = 1 << dspcom; 
    XBYTE[0xE000] = tab[dspbuf[dspcom]]; 
    if(++dspcom == 8)
    {
        dspcom = 0;
    }
}



