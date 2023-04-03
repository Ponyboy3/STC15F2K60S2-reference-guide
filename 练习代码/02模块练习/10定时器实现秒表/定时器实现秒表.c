#include "reg52.h"

unsigned char code SMG_NUM[11] = {0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xbf};

unsigned int T_h = 0;
unsigned int T_m = 0;
unsigned int T_s = 0;
unsigned int count = 0;

void Delay_SMG(unsigned int t)
{
	while(t--);
}

void Display_SMG_Bit(unsigned char pos,unsigned char value)
{
	P2 = (P2 & 0x1f) | 0xc0;
	P0 = 0x01 << pos;
	P2 &= 0x1f;
	
	P2 = (P2 & 0x1f) | 0xe0;
	P0 = value;
	P2 &= 0x1f;
	
	Delay_SMG(500);
	
	P2 = (P2 & 0x1f) | 0xc0;
	P0 = 0x01 << pos;
	P2 &= 0x1f;
	
	P2 = (P2 & 0x1f) | 0xe0;
	P0 = 0xff;
	P2 &= 0x1f;
}

void Display_SMG_All()
{
	Display_SMG_Bit(0,SMG_NUM[T_h / 10]);
	Display_SMG_Bit(1,SMG_NUM[T_h % 10]);
	
	Display_SMG_Bit(2,0xbf);
	
	Display_SMG_Bit(3,SMG_NUM[T_m / 10]);
	Display_SMG_Bit(4,SMG_NUM[T_m % 10]);
	
	Display_SMG_Bit(5,0xbf);
	
	Display_SMG_Bit(6,SMG_NUM[T_s / 10]);
	Display_SMG_Bit(7,SMG_NUM[T_s % 10]);
}

void Init_T0()
{
	TMOD = 0x01;
	TH0 = (0 - 50000) / 256;
	TL0 = (0 - 50000) % 256;
	
	EA = 1;
	ET0 = 1;
	TR0 = 1;
}

void Service_T0() interrupt 1
{
	TH0 = (0 - 50000) / 256;
	TL0 = (0 - 50000) % 256;
	
	count++;
	if(count == 20)
	{
		T_s++;
		count = 0;
	}
	if(T_s == 60)
	{
		T_s = 0;
		T_m++;
		if(T_m == 60)
		{
			T_m = 0;
			T_h++;
			if(T_h == 24) T_h = 0;
		}
	}
}

void Init_Sys()
{
	P2 = (P2 & 0x1f) | 0x80;
	P0 = 0xff;
	P2 &= 0x1f;
	
	P2 = (P2 & 0x1f) | 0xa0;
	P0 = 0x00;
	P2 &= 0x1f;
}

void main()
{
	Init_Sys();
	Init_T0();
	while(1)
	{
		Display_SMG_All();
	}
}