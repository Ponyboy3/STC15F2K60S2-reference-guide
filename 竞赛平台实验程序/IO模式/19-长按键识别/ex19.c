/*******************************************************************************
* 文件名称：长按键识别实验
* 实验目的：1.掌握单片机IO口操作的基本方法
*           2.掌握按键扫描及软件延时消除抖动的基本原理
* 实验配置：J3跳线配置为IO方式，J5配置为BTN、J2配置为1-3和2-4
* 程序现象：长按S4按键，关闭所有LED指示灯，短按S4按键，指示灯位移
* 硬件环境：IAP15F2K61S2@11.0592MHz
* 注意事项：本程序的目的在于介绍长按键的识别方法，在实际项目中，应考虑更为精准的
			      按键扫描周期和长按键识别间隔，建议使用基于定时器的方法对按键进行扫描
* 日期版本：2022/02/08
*******************************************************************************/

#include "reg52.h"  //定义51单片机特殊功能寄存器

sbit P33 = P3 ^ 3;
unsigned char read_key(void);

//关闭蜂鸣器
void cls_buzz()
{
    P2 = (P2 & 0x1F | 0xA0);
    P0 = 0x00;
    P2 &= 0x1F;
}

//关闭指示灯
void cls_led()
{
    P2 = (P2 & 0x1F | 0x80);
    P0 = 0xFF;
    P2 &= 0x1F;
}

//主函数
void main(void)
{
    unsigned char temp;
    unsigned char mov = 0;
    unsigned int scan_period = 0;

    cls_buzz();
    cls_led();

    while(1)
    {
        if(++scan_period == 1000)
        {
            scan_period = 0;
            temp = read_key();
            if(temp != 0xff)
            {
                if(temp == 1)
                {
                    P0 = ~(1 << mov);
                    if(++mov == 8)
                    {
                        mov = 0;
                    }
                }
                else
                {
                    P0 = 0xFF;
                }
                P2 = (P2 & 0x1F | 0x80);
                P2 &= 0x1F;

            }
        }

    }
}

//按键扫描函数
unsigned char read_key(void)
{
    static unsigned int press_counter = 0;	//长短按键计数器
    static unsigned char status = 0;
    unsigned char key_val = 0xFF;

    switch(status)
    {
    case 0:
        if(P33 == 0)
        {
            status = 1;
        }
        break;
    case 1:
        if(P33 == 0)
        {
            if(press_counter < 400)
            {
                press_counter++;
            }
        }
        else
        {
            if(press_counter > 20)
            {
                status = 2;
            }
            else
            {
                status = 0;	//抖动
                press_counter = 0;
            }
        }
        break;
    case 2:
        if(P33 == 1)
        {
            if(press_counter < 400)
            {
                key_val = 1;	//短按键
            }
            else
            {
                key_val = 2	;	//长按键
            }
            status = 0;
            press_counter = 0;
        }
        break;
    }

    return key_val;
}

