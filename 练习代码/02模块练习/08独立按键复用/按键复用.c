#include "reg52.h"

sbit L1 = P0^0;
sbit L2 = P0^1;
sbit L3 = P0^2;
sbit L4 = P0^3;
sbit L5 = P0^4;
sbit L6 = P0^5;
sbit L7 = P0^6;
sbit L8 = P0^7;

sbit S7 = P3^0;
sbit S6 = P3^1;
sbit S5 = P3^2;
sbit S4 = P3^3;

unsigned char key_sate = 0;

void Delay(unsigned int t)
{
	while(t--);
}

void scan_keys()
{
	if(S7 == 0 && L2 == 1)
	{
		Delay(100);
		if(S7 == 0 )
		{
			key_sate = 1;
			while(S7 == 0);
			L1 =~L1;
		}
	}
	else if(S6 == 0 && L1 == 1)
	{
		Delay(100);
		if(S6 == 0)
		{
			key_sate = 2;
			while(S6 == 0);
			L2 = ~L2;
		}
	}
	if(S5 == 0)
	{
		Delay(100);
		if(S5 == 0)
		{
			while(S5 == 0);
			if(key_sate == 1)
			{
				L3 = ~L3;
			}
			else if(key_sate == 2)
			{
				L5 = ~L5;
			}
		}
	}
	if(S4 == 0)
	{
		Delay(100);
		if(S4 == 0)
		{
			while(S4 == 0);
			if(key_sate == 1)
			{
				L4 = ~L4;
			}
			else if(key_sate == 2)
			{
				L6 = ~L6;
			}
		}
	}
}

void Init_Sys()
{
	P2 = (P2 & 0x1f) | 0xa0;
	P0 = 0x00;
	P2 &= 0x1f;
	
	P2 = (P2 & 0x1f) | 0x80;
	P0 = 0xff;
	P2 &= 0x1f;
	
	
}

void main()
{
	Init_Sys();
	P2 = (P2 & 0x1f) | 0x80;
	while(1)
	{
		scan_keys();
	}
}