#include "reg52.h"

unsigned char code SMG_duanma[18] = 
		{0xc0,0xf9,0xa4,0xb0,0x99,0x92,
		 0x82,0xf8,0x80,0x90,0x88,0x80,
		 0xc6,0xc0,0x86,0x8e,0xbf,0x7f};
		
unsigned int count_f = 0;
unsigned int dat_f = 0;	

void SelectHC573(unsigned char channel,unsigned char dat)
{
	P2 = (P2 & 0x1f) | 0x00;
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
	P2 = (P2 & 0x1f) | 0x00;
}

void Delay(unsigned int t)
{
	while(t--);
	
}

void DispalySMG_Bit(unsigned char pos,unsigned char value)
{
	SelectHC573(6,0x01 << pos);
	SelectHC573(7,value);
	Delay(500);
	SelectHC573(6,0x01 << pos);
	SelectHC573(7,0xff);
}
	
void Init_Timer()
{
	TMOD = 0x16;//T1定时，T0计数
	TL0 = 0xff;
	TH0 = 0xff;
	
	TL1 = (65536 - 50000) / 256;
	TH1 = (65536 - 50000) % 256;
	
	EA = 1;
	ET0 = 1;
	ET1 = 1;
	TR0 = 1;
	TR1 = 1;
}

void Service_T0() interrupt 1
{
	count_f++;
}	
unsigned char count_t = 0;
void Service_T1() interrupt 3
{
	TL1 = (65536 - 50000) / 256;
	TH1 = (65536 - 50000) % 256;
	
	count_t++;
	if(count_t == 20)
	{
		dat_f = count_f;
		count_f = 0;
		count_t = 0;
	}
	
}	


void Display_SMG()
{
	P2 = ((P2 & 0x1f) | 0xc0);
	P0 = 0x01 << 0;
	P2 &= 0x1f;
	P2 = ((P2 & 0x1f) | 0xe0);
	P0 = 0x8e;
	P2 &= 0x1f;
	Delay(500);
	
	P2 = ((P2 & 0x1f) | 0xc0);
	P0 = 0x01 << 1;
	P2 &= 0x1f;
	P2 = ((P2 & 0x1f) | 0xe0);
	P0 = 0xff;
	P2 &= 0x1f;
	Delay(500);
	
	P2 = ((P2 & 0x1f) | 0xc0);
	P0 = 0x01 << 2;
	P2 &= 0x1f;
	P2 = ((P2 & 0x1f) | 0xe0);
	P0 = 0xff;
	P2 &= 0x1f;
	Delay(500);
	
		P2 = ((P2 & 0x1f) | 0xc0);
		P0 = 0x01 << 3;
		P2 &= 0x1f;
		P2 = ((P2 & 0x1f) | 0xe0);
		P0 = SMG_duanma[dat_f / 10000];
		P2 &= 0x1f;
		Delay(500);
		
		P2 = ((P2 & 0x1f) | 0xc0);
		P0 = 0x01 << 4;
		P2 &= 0x1f;
		P2 = ((P2 & 0x1f) | 0xe0);
		P0 = SMG_duanma[(dat_f / 1000) % 10];
		P2 &= 0x1f;
		Delay(500);
		
		P2 = ((P2 & 0x1f) | 0xc0);
		P0 = 0x01 << 5;
		P2 &= 0x1f;
		P2 = ((P2 & 0x1f) | 0xe0);
		P0 = SMG_duanma[(dat_f / 100) % 10];
		P2 &= 0x1f;
		Delay(500);
	
		P2 = ((P2 & 0x1f) | 0xc0);
		P0 = 0x01 << 6;
		P2 &= 0x1f;
		P2 = ((P2 & 0x1f) | 0xe0);
		P0 = SMG_duanma[(dat_f / 10) % 10];
		P2 &= 0x1f;
		Delay(500);
	
		P2 = ((P2 & 0x1f) | 0xc0);
		P0 = 0x01 << 7;
		P2 &= 0x1f;
		P2 = ((P2 & 0x1f) | 0xe0);
		P0 = SMG_duanma[dat_f % 10];
		P2 &= 0x1f;
		Delay(500);
}

void main()
{
	P2 = ((P2 & 0x1f) | 0x80);
	P0 = 0xff;
	P2 &= 0x1f;
	
	P2 = ((P2 & 0x1f) | 0xa0);
	P0 = 0x00;
	P2 &= 0x1f;
	
	Init_Timer();
	while(1)
	{
		Display_SMG();
	}
}