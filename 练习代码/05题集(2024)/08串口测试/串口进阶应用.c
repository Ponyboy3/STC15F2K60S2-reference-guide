#include "reg52.h"

sfr AUXR = 0x8e;

#define BAUD         9600
#define SYSTEMCLOCK  12000000L

void SelectHC573(unsigned char c)
{
	switch(c)
	{
		case 4:
			P2 = (P2 & 0x1f) | 0x80;
		break;
		case 5:
			P2 = (P2 & 0x1f) | 0xa0;
		break;
		case 6:
			P2 = (P2 & 0x1f) | 0xc0;
		break;
		case 7:
			P2 = (P2 & 0x1f) | 0xe0;
		break;
		case 0:
			P2 = (P2 & 0x1f) | 0x00;
		break;
	}
}

void Init_System()
{
	SelectHC573(4);
	P0 = 0xff;
	SelectHC573(5);
	P0 = 0x00;
}

//==============================
sfr T2H=0xd6;  // 定时器2高8位
sfr T2L=0xd7;  // 定时器2低8位
void Init_UART()
{
//	TMOD = 0x00;
	SCON = 0x50;  // 8位数据,可变波特率
    AUXR |= 0x01; // 串口1选择定时器2为波特率发生器
    AUXR |= 0x04; // 定时器时钟1T模式
//	T2L = 0xC7;
//	T2H = 0xFE;
	T2L = (65535-(SYSTEMCLOCK / 4 / BAUD));
	T2H = (65535-(SYSTEMCLOCK / 4 / BAUD)) >> 8l;
//	TR1 = 1;
//	
//SCON = 0x50;
//AUXR = 0x40;
	AUXR |= 0x10; // 定时器2开始计时
	EA = 1;
	ES = 1;
}

unsigned char command = 0x00;
void ServiceUART() interrupt 4
{
	if(RI == 1)
	{
		command = SBUF;
		RI = 0;
		//SendString();
	}
}

void SendByte(unsigned int dat)
{
	SBUF = dat;
	while(TI == 0);
	TI = 0;
}

void SendString(unsigned char *str)
{
	while(*str != '\0')
	{
		SendByte(*str++);
	}
}
//==============================

void Working()
{
	if(command != 0x00)
	{
		switch(command & 0xf0)
		{
			case 0xa0:
				SelectHC573(4);
				P0 = (P0 | 0x0f) & ((~command) | 0xf0);
			  SelectHC573(0);
				command = 0x00;
			break;
			case 0xb0:
				SelectHC573(4);
				P0 = (P0 | 0xf0) & (((~command) << 4) | 0x0f);
			  SelectHC573(0);
				command = 0x00;
			break;
			case 0xc0:
				SendString("system is running...!\r\n");
				command = 0x00;
			break;
			
		}
	}
}

void main()
{
	Init_System();
	Init_UART();
	SendString("Welcome to XMF system!\r\n");
	while(1)
	{
		Working();
	}
}