#include "reg52.h"
sbit L1 = P0^0;
sbit L8 = P0^7;

unsigned int count = 0;

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
	if(count % 10 == 0)	L1 = ~L1;
	if(count % 100 == 0)
	{
		count = 0;
		L8 = ~L8;
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
	
	P2 = (P2 & 0x1f) | 0x80;
	P0 = 0xff;
	Init_T0();
}
void main()
{
	Init_Sys();
	while(1)
	{
		
	}
}