#include "reg52.h"

unsigned char SMG_NUM[16] =
{0xc0,0xf9,0xa4,0xb0,0x99,0x92,
 0x82,0xf8,0x80,0x90,0x88,0x80,
 0xc6,0xc0,0x86,0x8e};

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

void Display_SMG()
{
	unsigned char i,j;
	for(i = 0;i < 8;i++)
	{
		for(j = 0;j < 10;j++)
		{
			P2 = (P2 & 0x1f) | 0xc0;
			P0 = 0x01 << i;
			P2 &= 0x1f;
		
			P2 = (P2 & 0x1f) | 0xe0;
			P0 = SMG_NUM[j];
			P2 &= 0x1f;
			
			Delay(60000);
			Delay(60000);Delay(60000);
			Delay(60000);
		}
	}
	
	for(j = 0;j < 16;j++)
		{
			P2 = (P2 & 0x1f) | 0xc0;
			P0 = 0xff;
			P2 &= 0x1f;
		
			P2 = (P2 & 0x1f) | 0xe0;
			P0 = SMG_NUM[j];
			P2 &= 0x1f;
			
			Delay(60000);
			Delay(60000);Delay(60000);
			Delay(60000);
		}
}

void main()
{
	Init_Sys();
	while(1)
	{
		Display_SMG();
	}
}