/*******************************************************************************
* 文件名称：串口通讯实验
* 实验目的：1.掌握51单片机串口工作模式及相关寄存器配置方法
*           2.了解51单片机波特率的计算方法
*           3.掌握单片机串口接收中断服务函数的设计方法
* 实验配置：J3跳线配置为	IO方式，J5配置为BTN、J2配置为1-3和2-4
* 程序现象：通过串口调试助手发送数字1~8，板子上面的对应指示灯亮
* 硬件说明：IAP15F2K61S2@11.0592MHz
* 程序说明：1.通过USB连接线连接PC机，在计算机硬件管理器中查看串口号
*           2.打开串口调试工具软件，并将串口通讯波特率设定为9600
*			      3.输入1-8字符，控制8个LED指示灯状态
* 日期版本：2022/02/08
*******************************************************************************/

#include "STC15F2K60S2.h " //定义51单片机特殊功能寄存器

#define BAUD	       9600  		//波特率
#define SYSTEMCLOCK  11059200L  //系统时钟频率

bit rxc = 0; //数据接收标志位
char rx = '1';//接收到的数据
void uart_sendstring(unsigned char *str); //数据发送


//主函数
void main(void)
{
    SCON = 0x50;
    AUXR = 0x40;                //1T
    TMOD = 0x00;
    TL1 = (65536 - (SYSTEMCLOCK / 4 / BAUD));
    TH1 = (65536 - (SYSTEMCLOCK / 4 / BAUD)) >> 8;
    TR1 = 1;
    ES = 1;
    EA = 1;

    while(1)
    {
        if(rxc == 1)
        {
            rxc = 0;
            switch(rx)
            {
            case '1':
            {
                P2 = ((P2 & 0x1f) | 0x80);
                P0 = 0xFE;
                P2 &= 0x1f;
                uart_sendstring("1");
            }
            break;
            case '2':
            {
                P2 = ((P2 & 0x1f) | 0x80);
                P0 = 0xFD;
                P2 &= 0x1f;
                uart_sendstring("2");
            }
            break;
            case '3':
            {
                P2 = ((P2 & 0x1f) | 0x80);
                P0 = 0xFB;
                uart_sendstring("3");
            }
            break;
            case '4':
            {
                P2 = ((P2 & 0x1f) | 0x80);
                P0 = 0xF7;
                P2 &= 0x1f;
                uart_sendstring("4");
            }
            break;
            case '5':
            {
                P2 = ((P2 & 0x1f) | 0x80);
                P0 = 0xEF;
                P2 &= 0x1f;
                uart_sendstring("5");
            }
            break;
            case '6':
            {
                P2 = ((P2 & 0x1f) | 0x80);
                P0 = 0xDF;
                P2 &= 0x1f;
                uart_sendstring("6");
            }
            break;
            case '7':
            {
                P2 = ((P2 & 0x1f) | 0x80);
                P0 = 0xBF;
                P2 &= 0x1f;
                uart_sendstring("7");
            }
            break;
            case '8':
            {
                P2 = ((P2 & 0x1f) | 0x80);
                P0 = 0x7F;
                P2 &= 0x1f;
                uart_sendstring("8");
            }
            break;
            default:
                uart_sendstring("error\r\n");	//输入其他数据返回error
                break;
            }
            ES = 1;
        }
    }
}

//串口中断服务函数
void isr_uart(void) interrupt 4
{
    if(RI)
    {
        RI = 0;  //清除接收标志位
        rx = SBUF;
        ES = 0;
        rxc = 1;
    }
}

//通过串口发送字符串
void uart_sendstring(unsigned char *str)
{
    unsigned char *p;
    p = str;
    while(*p != '\0')
    {
        SBUF = *p;
        while(TI == 0);  //等待发送标志位置位
        TI = 0;
        p++;
    }
}

