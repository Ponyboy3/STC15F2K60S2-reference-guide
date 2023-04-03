/*******************************************************************************
* 文件名称：按键控制LED位移
* 实验目的：1.掌握单片机IO口操作的基本方法
*           2.掌握按键扫描及软件延时消除抖动的基本原理
* 实验配置：J3跳线配置为IO方式，J5配置为BTN、J2配置为1-3和2-4
* 程序现象：按相应按键led显示发生变化
* 硬件环境：IAP15F2K61S2@11.0592MHz
* 注意事项：1.本程序中使用的软件延时消抖方法会降低单片机系统的实时性，其目的在于
*             理解按键抖动产生的过程和消除方法，在后续的实验例程中会逐步介绍基于
*             定时器扫描的消抖方法。
*           2.相关按键功能可以参考按键处理函数void key_proc(unsigned char key)
* 日期版本：2022/02/08
*******************************************************************************/

#include "reg52.h"  //定义51单片机特殊功能寄存器
#include "absacc.h"

unsigned char temp1;
unsigned char read_key(void);
void key_proc(unsigned char key);

void cls_buzz()
{
    P2 = (P2 & 0x1F | 0xA0);
    P0 = 0x00;
    P2 &= 0x1F;
}

void cls_led()
{
    P2 = (P2 & 0x1F | 0x80);
    P0 = 0xFF;
    P2 &= 0x1F;
}

//延时函数: 10ms@11.0592MHz
void delay(void)
{
    unsigned char i, j;

    i = 108;
    j = 145;
    do
    {
        while (--j);
    }
    while (--i);
}

//主函数
void main(void)
{
    unsigned char temp;
    cls_buzz();
    cls_led();

    while(1)
    {
        temp = read_key();
        if(temp != 0xff)
        {
            delay();  //按键消抖
            temp = read_key();
            if(temp != 0xff)
            {
                //进入按键处理函数
                key_proc(temp);
                while(P3 != 0xff);  //等待按键释放
            }
        }
    }
}

//按键扫描函数
unsigned char read_key(void)
{
    unsigned char temp;
    unsigned char key_value = 0xff;
    temp = P3 & 0x0f;
    switch(temp)
    {
    case 0x0e:
        key_value = 1;  //S7
        break;
    case 0x0d:
        key_value = 2;  //S6
        break;
    case 0x0b:
        key_value = 3;  //S5
        break;
    case 0x07:
        key_value = 4;  //S4
        break;
    }
    return key_value;
}

//按键处理函数
void key_proc(unsigned char key)
{
    switch(key)
    {
    case 1:
        P2 = (P2 & 0x1f | 0x80);
        temp1++;
        P0 = temp1;
        P2 &= 0x1f;
        break;
    case 2:
        P2 = (P2 & 0x1f | 0x80);
        temp1--;
        P0 = temp1;
        P2 &= 0x1f;
        break;
    case 3:
        P2 = (P2 & 0x1f | 0x80);
        temp1 = temp1 + 2;
        P0 = temp1;
        P2 &= 0x1f;
        break;
    case 4:
        P2 = (P2 & 0x1f | 0x80);
        temp1 = temp1 - 2;
        P0 = temp1;
        P2 &= 0x1f;
        break;
    }
}

