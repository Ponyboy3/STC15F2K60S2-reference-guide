#include "reg52.h"

sbit S4 = P3^3;
sbit S5 = P3^2;

unsigned char t_m = 0;
unsigned char t_s = 0;
unsigned char t_005s = 0;

unsigned char code SMG_NUM[18]=
 {0xc0,0xf9,0xa4,0xb0,0x99,0x92,
	0x82,0xf8,0x80,0x90,0x88,0x80,
	0xc6,0xc0,0x86,0x8e,0xbf,0x7f};

void SelectHC573(unsigned char channel)
{
	switch(channel)
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
	}
}

void DisplaySMG_bit(unsigned char value,unsigned char pos)
{
	SelectHC573(6);
	P0 = 0x01 << pos;
	SelectHC573(7);
	P0 = value;
}

void DelaySMG(unsigned int t)
{
	while(t--);
}

void DisplayTime()
{
	DisplaySMG_bit(SMG_NUM[t_005s % 10],7);
	DelaySMG(500);
	DisplaySMG_bit(SMG_NUM[t_005s / 10],6);
	DelaySMG(500);
	DisplaySMG_bit(SMG_NUM[16],5);
	DelaySMG(500);
	DisplaySMG_bit(SMG_NUM[t_s % 10],4);
	DelaySMG(500);
	DisplaySMG_bit(SMG_NUM[t_s / 10],3);
	DelaySMG(500);
	DisplaySMG_bit(SMG_NUM[16],2);
	DelaySMG(500);
	DisplaySMG_bit(SMG_NUM[t_m % 10],1);
	DelaySMG(500);
	DisplaySMG_bit(SMG_NUM[t_m / 10],0);
	DelaySMG(500);
}

//==============定时器相关函数============
void Init_Timer0()
{
	TMOD = 0X01;
	TH0 = (65535 - 50000) / 256; //   50ms
	TL0 = (65535 - 50000) % 256;
	
	ET0 = 1;
	EA = 1;
	TR0 = 1;
}

void ServiceTimer0() interrupt 1
{
	TH0 = (65535 - 50000) / 256; //   50ms
	TL0 = (65535 - 50000) % 256;
	
	t_005s++;
	if(t_005s == 20)
	{
		t_s++;
		t_005s = 0;
		if(t_s == 60)
		{
			t_m++;
			t_s = 0;
			if(t_m == 99)
				t_m = 0;
		}
	}
}
//========================================

void DeleyKey(unsigned int t)
{
	while(t--);
}

void Scan_Keys()
{
	if(S4 == 0)//秒表启动/暂停
	{
		DeleyKey(100);
		if(S4 == 0)
		{
			TR0 =~TR0;
			while(S4 == 0)
			{
				DisplayTime();
			}
		}
	}
	if(S5 == 0)//秒表清零
	{
		DeleyKey(100);
		if(S5 == 0)
		{
			t_m = 0;
			t_s = 0;
			t_005s = 0;
			while(S5 == 0)
			{
				DisplayTime();
			}
		}
	}
}

//void main()
{
	
	Init_Timer0();
	while(1)
	{
		SelectHC573(4);
		P0 = 0xff;
		
		DisplayTime();
		Scan_Keys();
	}
}