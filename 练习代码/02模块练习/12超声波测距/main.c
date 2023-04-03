#include "reg52.h"
#include "intrins.h"

#define uchar unsigned char
#define uint unsigned int

sbit TX = P1 ^ 0;
sbit RX = P1 ^ 1;

uchar code type1[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90, 0xff, 0xc6};
uchar code type2[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};

uchar num1, num2, num3, num4, num5, num6, num7, num8;
bit f_2ms;
uint distance;
void Display_SMG(uchar n1, uchar n2, uchar n3, uchar n4, uchar n5, uchar n6, uchar n7, uchar n8)
{
	char i;
	P0 = 0xff;
	P2 = (P2 & 0x1f) | 0xc0;
	P0 = type2[i];
	P2 &= 0x1f;
	switch (i)
	{
	case 0:
		P2 = (P2 & 0x1f) | 0xe0;
		P0 = type1[n1];
		P2 &= 0x1f;
		break;
	case 1:
		P2 = (P2 & 0x1f) | 0xe0;
		P0 = type1[n2];
		P2 &= 0x1f;
		break;
	case 2:
		P2 = (P2 & 0x1f) | 0xe0;
		P0 = type1[n3];
		P2 &= 0x1f;
		break;
	case 3:
		P2 = (P2 & 0x1f) | 0xe0;
		P0 = type1[n4];
		P2 &= 0x1f;
		break;
	case 4:
		P2 = (P2 & 0x1f) | 0xe0;
		P0 = type1[n5];
		P2 &= 0x1f;
		break;
	case 5:
		P2 = (P2 & 0x1f) | 0xe0;
		P0 = type1[n6];
		P2 &= 0x1f;
		break;
	case 6:
		P2 = (P2 & 0x1f) | 0xe0;
		P0 = type1[n7];
		P2 &= 0x1f;
		break;
	case 7:
		P2 = (P2 & 0x1f) | 0xe0;
		P0 = type1[n8];
		P2 &= 0x1f;
		break;
	}
	if (++i == 8)
		i = 0;
}
void Init_T0()
{
	TMOD = (TMOD & 0xf0) | 0x01;
	TH0 = (65536 - 12) / 256;
	TL0 = (65536 - 12) % 256;
}
void Init_T1()
{
	TMOD = (TMOD & 0x0f) | 0x10;
	TH1 = (65536 - 2000) / 256;
	TL1 = (65536 - 2000) % 256;

	EA = 1;
	ET1 = 1;
	TR1 = 1;
}
// 超声波模块
uchar Get_Distance()
{
	uchar temp = 16;
	TX = 0;
	TR0 = 1;
	// 引脚发送40Khz方波信号
	while (temp--) // 每隔12us发8个方波
	{
		while (!TF0)
			;
		TX ^= 1; // if(TX==1) { Delay20us();}//可以不用
		TH0 = (65536 - 12) / 256;
		TL0 = (65536 - 12) % 256;
		TF0 = 0;
	}
	//	while(temp--)
	//	{
	//		while(!TF0);
	//		TX ^= 1;//TX = ~TX;
	//		TH0=(65536-12)/256;
	//		TL0=(65536-12)%256;
	//		TF0 = 0;//TF0必须要先赋值(益处中断，自动置1)在清零
	//	}
	TR0 = 0;
	TL0 = 0;
	TH0 = 0;
	TR0 = 1;
	while (RX && !TF0)
		;
	TR0 = 0;

	if (TF0 == 1)
	{
		TF0 = 0;
		return 999;
	}
	else
		return ((TH0 << 8) + TL0) * 0.017;
}
void Init_Sys()
{
	P2 = (P2 & 0x1f) | 0x80;
	P0 = 0xff;
	P2 &= 0x1f;
	P2 = (P2 & 0x1f) | 0xa0;
	P0 = 0x00;
	P2 &= 0x1f;
	P2 = (P2 & 0x1f) | 0xc0;
	P0 = 0xff;
	P2 &= 0x1f;
	P2 = (P2 & 0x1f) | 0xe0;
	P0 = 0xff;
	P2 &= 0x1f;
}
void main()
{
	Init_Sys();
	Init_T1();
	Init_T0();
	while (1)
	{
		if (f_2ms == 1)
		{
			f_2ms = 0;
			distance = Get_Distance();
			num1 = num2 = num3 = num4 = 10;
			num5 = distance / 100;
			num6 = distance / 10 % 10;
			num7 = distance % 10;
			num8 = 11;
		}
	}
}
void Service_T1() interrupt 3
{
	TH1 = (65536 - 2000) / 256;
	TL1 = (65536 - 2000) % 256;

	f_2ms = 1;
	Display_SMG(num1, num2, num3, num4, num5, num6, num7, num8);
}