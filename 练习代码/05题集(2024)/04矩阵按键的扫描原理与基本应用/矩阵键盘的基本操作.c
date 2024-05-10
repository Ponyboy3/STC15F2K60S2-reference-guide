#include "reg52.h"

sfr P4    = 0xC0;

sbit R1 = P3^0;
sbit R2 = P3^1;
sbit R3 = P3^2;
sbit R4 = P3^3;

sbit C1 = P4^4;
sbit C2 = P4^2;
sbit C3 = P3^5;
sbit C4 = P3^4;

unsigned char code SMG_duanma[18] = 
		{0xc0,0xf9,0xa4,0xb0,0x99,0x92,
		 0x82,0xf8,0x80,0x90,0x88,0x80,
		 0xc6,0xc0,0x86,0x8e,0xbf,0x7f}; 

void SelectHC573(unsigned char channel);
void DiplayKeyNum(unsigned char value);
void ScanKeysMulti();

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

void DiplayKeyNum(unsigned char value)
{
	SelectHC573(6);
	P0 = 0x01;
	SelectHC573(7);
	P0 = value;
}

unsigned char key_num;
void ScanKeysMulti()
{
	R1 = 0;                            //扫描第一行
	R2 = R3 = R4 = 1;
	C1 = C2 = C3 = C4 = 1;
	if(C1 == 0)
	{
		
		while(C1 == 0);
		key_num = 0;
		DiplayKeyNum(SMG_duanma[key_num]);
	}
	else if(C2 == 0)
	{
		while(C2 ==0);
		key_num = 1;
		DiplayKeyNum(SMG_duanma[key_num]);
	}
	else if(C3 == 0)
	{
		while(C3 ==0);
		key_num = 2;
		DiplayKeyNum(SMG_duanma[key_num]);
	}
	else if(C4 == 0)
	{
		while(C4 ==0);
		key_num = 3;
		DiplayKeyNum(SMG_duanma[key_num]);
	}
	
	R2 = 0;                           //扫描第二行
	R1 = R3 = R4 = 1;
	C1 = C2 = C3 = C4 = 1;
	if(C1 == 0)
	{
		
		while(C1 == 0);
		key_num = 4;
		DiplayKeyNum(SMG_duanma[key_num]);
	}
	else if(C2 == 0)
	{
		while(C2 ==0);
		key_num = 5;
		DiplayKeyNum(SMG_duanma[key_num]);
	}
	else if(C3 == 0)
	{
		while(C3 ==0);
		key_num = 6;
		DiplayKeyNum(SMG_duanma[key_num]);
	}
	else if(C4 == 0)
	{
		while(C4 ==0);
		key_num = 7;
		DiplayKeyNum(SMG_duanma[key_num]);
	}
	
	R3 = 0;                           //扫描第三行
	R1 = R2 = R4 = 1;
	C1 = C2 = C3 = C4 = 1;
	if(C1 == 0)
	{
		
		while(C1 == 0);
		key_num = 8;
		DiplayKeyNum(SMG_duanma[key_num]);
	}
	else if(C2 == 0)
	{
		while(C2 ==0);
		key_num = 9;
		DiplayKeyNum(SMG_duanma[key_num]);
	}
	else if(C3 == 0)
	{
		while(C3 ==0);
		key_num = 10;
		DiplayKeyNum(SMG_duanma[key_num]);
	}
	else if(C4 == 0)
	{
		while(C4 ==0);
		key_num = 11;
		DiplayKeyNum(SMG_duanma[key_num]);
	}
	
	R4 = 0;                           //扫描第四行
	R1 = R2 = R3 = 1;
	C1 = C2 = C3 = C4 = 1;
	if(C1 == 0)
	{
		
		while(C1 == 0);
		key_num = 12;
		DiplayKeyNum(SMG_duanma[key_num]);
	}
	else if(C2 == 0)
	{
		while(C2 ==0);
		key_num = 13;
		DiplayKeyNum(SMG_duanma[key_num]);
	}
	else if(C3 == 0)
	{
		while(C3 ==0);
		key_num = 14;
		DiplayKeyNum(SMG_duanma[key_num]);
	}
	else if(C4 == 0)
	{
		while(C4 ==0);
		key_num = 15;
		DiplayKeyNum(SMG_duanma[key_num]);
	}
}

void Init_System()
{
	SelectHC573(4);
	P0 = 0xff;
	SelectHC573(5);
	P0 = 0x00;
}

void main()
{
	Init_System();
	while(1)
	{
		ScanKeysMulti();
	}
}