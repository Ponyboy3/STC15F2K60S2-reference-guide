/*******************************************************************************
* 文件名称：开机次数存储实验
* 实验目的：1.掌握I2C总线通讯基本特点和工作时序
*           2.掌握51单片机模拟I2C总线时序的程序设计方法
*           3.掌握EEPROM存储器的特性及AT24C02的读写操作方法
* 实验配置：J3跳线配置为IO方式，J5配置为BTN、J2配置为1-3和2-4
* 程序现象：重新上电，数码管显示+1
* 硬件环境：IAP15F2K61S2@11.0592MHz
* 注意事项：无
* 日期版本：2022/02/08
*******************************************************************************/
#include "reg52.h"
#include "i2c.h"
#include "intrins.h"


sbit p27 = P2 ^ 7;
sbit p26 = P2 ^ 6;
sbit p25 = P2 ^ 5;
sfr AUXR = 0x8E;
code unsigned char tab[] = { 0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90, 0xff};
unsigned char dspbuf[8] = {10, 10, 10, 10, 10, 10, 10, 10}; //显示缓冲区
unsigned char dspcom = 0;
void display(void);
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


void delay()		//10ms @11.0592MHz
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
    unsigned char reset_cnt;  //开机次数存储 (最大存储值255)
    cls_buzz();
    cls_led();
    AUXR |= 0x80;
    TMOD &= 0xF0;
    TL0 = 0xCD;
    TH0 = 0xD4;
    TF0 = 0;
    TR0 = 1;
    ET0 = 1;
    reset_cnt = read_eeprom(0x00);  //从AT24C02地址0x00中读取数据
    reset_cnt++;
    delay();	//延时10ms
    write_eeprom(0x00, reset_cnt); //向AT24C02地址0x00中写入数据
    delay();
    EA = 1;	//数据写入完成后，开机中断
    //更新显示数据
    (reset_cnt >= 100) ? (dspbuf[5] = reset_cnt / 100) : (dspbuf[5] = 10);
    (reset_cnt >= 10) ? (dspbuf[6] = reset_cnt % 100 / 10) : (dspbuf[6] = 10);
    dspbuf[7] = reset_cnt % 10;

    while(1)
    {
        ;
    }
}

//定时器中断服务函数
void isr_timer_0(void)  interrupt 1  //默认中断优先级 1
{
    display();
}


void display(void)
{

    p27 = 1;
    p26 = 1;
    p25 = 1;
    P0 = 0xff;
    p27 = 0;
    p26 = 0;
    p25 = 0;

    p27 = 1;
    p26 = 1;
    p25 = 0;
    P0 = (1 << dspcom);
    p27 = 0;
    p26 = 0;
    p25 = 0;

    p27 = 1;
    p26 = 1;
    p25 = 1;
    P0 = tab[dspbuf[dspcom]];
    p27 = 0;
    p26 = 0;
    p25 = 0;
    if(++dspcom == 8)
    {
        dspcom = 0;
    }
}
