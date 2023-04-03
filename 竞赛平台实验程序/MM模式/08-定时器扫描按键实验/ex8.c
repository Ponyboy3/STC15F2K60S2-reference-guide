/*******************************************************************************
* 文件名称：定时器扫描按键实验
* 实验目的：1.掌握通过定时器扫描按键键值的基本方法
*           2.掌握51单片机定时器工作模式和配置方法
* 实验配置：J3跳线配置为MM方式，J5配置为BTN、J2配置为1-3和2-4
* 程序现象：按S7、S6按键，数码管显示数字分别加减1
* 硬件环境：IAP15F2K61S2@11.0592MHz
* 注意事项：无
* 日期版本：2022/02/08
*******************************************************************************/

#include "reg52.h"  //定义51单片机特殊功能寄存器
#include "absacc.h"

sfr AUXR = 0x8E;

bit key_re;
unsigned char key_press;
unsigned char key_value;
bit key_flag;
unsigned char intr; 
                           //  0    1     2      3    4      5     6     7     8    9     熄灭
code unsigned char tab[] = { 0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90, 0xff};
unsigned char dspbuf[8] = {10, 10, 10, 10, 10, 10, 10, 10}; //显示缓冲区
unsigned char dspcom = 0;

unsigned char read_key(void);
void display(void);
void cls_buzz()
{
    XBYTE[0xA000] = 0;
}
void cls_led()
{
    XBYTE[0x8000] = 0xFF;
}

//主函数
void main(void)
{
    unsigned char key_temp;
    unsigned char num;

    cls_buzz();
    cls_led();
    P3 = 0xff;  //按键接口初始化
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
            key_temp = read_key();
        }
        //进入按键处理程序部分
        switch(key_temp)
        {
        case 1:
            num++;
            break;
        case 2:
            num--;
            break;
        }
        key_temp = 0;  //完成按键处理程序后，清除键值
        (num >= 100) ? (dspbuf[5] = num / 100) : (dspbuf[5] = 10);
        (num >= 10) ? (dspbuf[6] = num % 100 / 10) : (dspbuf[6] = 10);
        dspbuf[7] = num % 10;
    }
}

//定时器中断服务函数
void isr_timer_0(void)  interrupt 1  //默认中断优先级 1
{
    display();
    if(++intr == 10)  //1ms执行一次
    {
        intr = 0;
        key_flag = 1;  //10ms按键扫描标志位置1
    }
}

//读取键值
unsigned char read_key(void)
{

    unsigned char key_temp;
    key_temp = (P3 & 0x0f);
    /**************多次检测，可以防止抖动*********************/
    if(key_temp != 0x0f) //有按键按下
        key_press++;
    else
        key_press = 0;  //抖动

    if(key_press == 5)
    {
        key_press = 0;
        key_re = 1;
        switch(key_temp)
        {
        case 0x0e:
            key_value = 1;
            break;
        case 0x0d:
            key_value = 2;
            break;
        case 0x0b:
            key_value = 3;
            break;
        case 0x07:
            key_value = 4;
            break;
        }
    }
    //连续5次检测到按键被按下，并且该按键已经释放
    if((key_re == 1) && (key_temp == 0x0f))
    {
        key_re = 0;
        return key_value;
    }

    return 0xff;  //无按键按下或被按下的按键未被释放
}

//显示函数
void display(void)
{
    XBYTE[0xE000] = 0xff;  //消隐
    XBYTE[0xC000] = (1 << dspcom);
    XBYTE[0xE000] = tab[dspbuf[dspcom]]; //段码

    if(++dspcom == 8)
    {
        dspcom = 0;
    }
}
