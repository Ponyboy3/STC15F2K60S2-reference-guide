#include "reg52.h"

sfr P4 = 0xc0;

sbit C1 = P3^0;
sbit C2 = P3^1;
sbit C3 = P3^2;
sbit C4 = P3^3;

sbit R1 = P4^4;
sbit R2 = P4^2;
sbit R3 = P3^5;
sbit R4 = P3^4;

void Init_Sys()
{
	P2 = (P2 & 0x1f) | 0x80;
	P0 = 0xff;
	P2 &= 0x1f;
	
	P2 = (P2 & 0x1f) | 0xa0;
	P0 = 0x00;
	P2 &= 0x1f;
}

void Delay(unsigned int t)
{
	while(t--);
}

void Scan_Keys()
{
	C1 = 0;
	C2 = C3 = C4 = 1;
	if(R1 == 0)
	{
		P2 = (P2 & 0x1f) | 0xe0;
		P0 = 0xc0;
		P2 &= 0x1f;
	}
	else if(R2 == 0)
	{
		P2 = (P2 & 0x1f) | 0xe0;
		P0 = 0xf9;
		P2 &= 0x1f;
	}
	else if(R3 == 0)
	{
		P2 = (P2 & 0x1f) | 0xe0;
		P0 = 0xa4;
		P2 &= 0x1f;
	}
	else if(R4 == 0)
	{
		P2 = (P2 & 0x1f) | 0xe0;
		P0 = 0xb0;
		P2 &= 0x1f;
	}
 	
	
	C2 = 0;
	C1 = C3 = C4 = 1;
	if(R1 == 0)
	{
		P2 = (P2 & 0x1f) | 0xe0;
		P0 = 0xb0;
		P2 &= 0x1f;
	}
	else if(R2 == 0)
	{
		P2 = (P2 & 0x1f) | 0xe0;
		P0 = 0xa4;
		P2 &= 0x1f;
	}
	else if(R3 == 0)
	{
		P2 = (P2 & 0x1f) | 0xe0;
		P0 = 0xf9;
		P2 &= 0x1f;
	}
	else if(R4 == 0)
	{
		P2 = (P2 & 0x1f) | 0xe0;
		P0 = 0xc0;
		P2 &= 0x1f;
	}
 	
	
}

void main()
{
	Init_Sys();
	P2 = (P2 & 0x1f) | 0xc0;
	P0 = 0x01;
	P2 &= 0x1f;
	while(1)
	{
		Scan_Keys();
	}
}