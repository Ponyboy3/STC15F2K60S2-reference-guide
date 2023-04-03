#include "reg52.h"

void Init_Sys()
{
	P2 = (P2 & 0x1f) | 0x80;
	P0 = 0xff;
	P2 &= 0x1f;
	
	P2 = (P2 & 0x1f) | 0xa0;
	P0 = 0x00;
	P2 &= 0x1f;
}

void Delay()
{
	unsigned char i = 9,j = 104,k = 139;
	do
	{
		do
		{
			while(--k);
		}while(--j);
	}while(--i);
		
}

void Display_led()
{
	unsigned char i;
	for(i = 0;i < 3;i++)
	{
		P2 = (P2 & 0x1f) | 0x80;
		P0 = 0x00;
		P2 &= 0x1f;
		Delay();
		Delay();
		P2 = (P2 & 0x1f) | 0x80;
		P0 = 0xff;
		P2 &= 0x1f;
		Delay();
		Delay();
	}
	
	for(i = 0;i < 8;i++)
	{
		P2 = (P2 & 0x1f) | 0x80;
		P0 = 0xfe << i;
		P2 &= 0x1f;
		Delay();
		Delay();
	}
	
	P2 = (P2 & 0x1f) | 0xa0;
	P0 = 0x10;
	P2 &= 0x1f;
	Delay();
	Delay();
	P2 = (P2 & 0x1f) | 0xa0;
	P0 = 0x00;
	P2 &= 0x1f;
	
	for(i = 0;i < 8;i++)
	{
		P2 = (P2 & 0x1f) | 0x80;
		P0 = ~(0xfe << i);
		P2 &= 0x1f;
		Delay();
		Delay();
	}
	
	P2 = (P2 & 0x1f) | 0xa0;
	P0 = 0x40;
	P2 &= 0x1f;
	Delay();
	Delay();
	P2 = (P2 & 0x1f) | 0xa0;
	P0 = 0x00;
	P2 &= 0x1f;
}

void main()
{
	Init_Sys();
	while(1)
	{
		Display_led();
	}
}