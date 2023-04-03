#include "reg52.h"

sbit S7 = P3^0;
sbit S6 = P3^1;
sbit S5 = P3^2;
sbit S4 = P3^3;

void Delay(unsigned int t)
{
	while(t--);
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

void Scan_Key()
{
	if(S7 == 0)
	{
		Delay(100);
		if(S7 == 0)
		{
			P2 = (P2 & 0x1f) | 0x80;
			P0 = ~0x01;
			P2 &= 0x1f;
			while(S7 == 0);
			P2 = (P2 & 0x1f) | 0x80;
			P0 = 0xff;
			P2 &= 0x1f;
		}
	}
	if(S6 == 0)
	{
		Delay(100);
		if(S6 == 0)
		{
			P2 = (P2 & 0x1f) | 0x80;
			P0 = ~0x02;
			P2 &= 0x1f;
			while(S6 == 0);
			P2 = (P2 & 0x1f) | 0x80;
			P0 = 0xff;
			P2 &= 0x1f;
		}
	}
	if(S5 == 0)
	{
		if(S5 == 0)
		{
			P2 = (P2 & 0x1f) | 0x80;
			P0 = ~0x04;
			P2 &= 0x1f;
			while(S5 == 0);
			P2 = (P2 & 0x1f) | 0x80;
			P0 = 0xff;
			P2 &= 0x1f;
		}
	}
	if(S4 == 0)
	{
		Delay(100);
		if(S4 == 0)
		{
			P2 = (P2 & 0x1f) | 0x80;
			P0 = ~0x08;
			P2 &= 0x1f;
			while(S4 == 0);
			P2 = (P2 & 0x1f) | 0x80;
			P0 = 0xff;
			P2 &= 0x1f;
		}
	}
}

void main()
{
	Init_Sys();
	while(1)
	{
		Scan_Key();
	}
}