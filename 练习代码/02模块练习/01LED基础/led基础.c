#include "reg52.h"


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

void Display_Led()
{
	unsigned char i;
	for(i = 0;i < 3;i++)
	{
		P2 = (P2 & 0x1f) | 0x80;
		P0 = 0x00;
		P2 &= 0x1f;
		Delay(60000);
		Delay(60000);
		P2 = (P2 & 0x1f) | 0x80;
		P0 = 0xff;
		P2 &= 0x1f;
		Delay(60000);
		Delay(60000);
	}
	
	for(i = 0;i < 8;i++)
	{
		P2 = (P2 & 0x1f) | 0x80;
		P0 = 0xfe << i;
		P2 &= 0x1f;
		Delay(60000);
		Delay(60000);
	}
	for(i = 0;i < 8;i++)
	{
		P2 = (P2 & 0x1f) | 0x80;
		P0 = ~(0xfe << i);
		P2 &= 0x1f;
		Delay(60000);
		Delay(60000);
	}
	
	
}

void main()
{
	Init_Sys();
	while(1)
	{
		Display_Led();
	}
}