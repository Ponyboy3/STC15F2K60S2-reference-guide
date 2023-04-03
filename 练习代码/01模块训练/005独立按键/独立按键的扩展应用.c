#include "reg52.h"

sbit S7 = P3^0;
sbit S6 = P3^1;
sbit S5 = P3^2;
sbit S4 = P3^3;

sbit L1 = P0^0;
sbit L2 = P0^1;
sbit L3 = P0^2;
sbit L4 = P0^3;
sbit L5 = P0^4;
sbit L6 = P0^5;

void SelectHC573(unsigned char channel);
void ScanKeys_Alone();
void DelayKey(unsigned int t);

void DelayKey(unsigned int t)
{
	while(t--);
}

unsigned char stat_k = 0;
void ScanKeys_Alone()
{
	if(S7 == 0)
	{
		DelayKey(100);
		if(S7 == 0)
		{
			if(stat_k == 0)
			{
				L1 = 0;
				stat_k = 1;
			}
			else if(stat_k == 1)
			{
				L1 = 1;
				stat_k = 0;
			}
			while(S7 == 0);
		}
	}
	if(S6 == 0)
	{
		DelayKey(100);
		if(S6 == 0)
		{
			if(stat_k == 0)
			{
				L2 = 0;
				stat_k = 2;
			}
			else if(stat_k == 2)
			{
				L2 = 1;
				stat_k = 0;
			}
			while(S6 == 0);
		}
	}
	if(S5 == 0)
	{
		DelayKey(100);
		if(S5 == 0)
		{
			if(stat_k == 1)
			{
				L3 = 0;
				while(S5 == 0);
			  L3 = 1;
			}
			if(stat_k == 2)
			{
				L5 = 0;
				while(S5 == 0);
			  L5 = 1;
			}
		}
	}
	if(S4 == 0)
	{
		DelayKey(100);
		if(S4 == 0)
		{
			if(stat_k == 1)
			{
				L4 = 0;
				while(S4 == 0);
			  L4 = 1;
			}
			if(stat_k == 2)
			{
				L6 = 0;
				while(S4 == 0);
			  L6 = 1;
			}
		}
	}
}

void SelectHC573(unsigned char channel)
{
	switch(channel)
	{
		case 4:
			P2 = (P2 & 0x0f) | 0x80;
		break;
		case 5:
			P2 = (P2 & 0x0f) | 0xa0;
		break;
		case 6:
			P2 = (P2 & 0x0f) | 0xc0;
		break;
		case 7:
			P2 = (P2 & 0x0f) | 0xe0;
		break;
	}
}

void main()
{
	SelectHC573(4);
	while(1)
	{
		ScanKeys_Alone();
	}
}