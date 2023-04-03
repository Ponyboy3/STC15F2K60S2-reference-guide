#include "reg52.h"

unsigned char SMG_NUM[16] = 
{0xc0,0xf9,0xa4,0xb0,0x99,0x92,
 0x82,0xf8,0x80,0x90};
unsigned char yue = 0;
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

void Display_SMG()
{
	
		P2 = (P2 & 0x1f) | 0xc0;
	  P0 = 0x01 << 0;
 	  P2 &= 0x1f;
	  P2 = (P2 & 0x1f) | 0xe0;
	  P0 = SMG_NUM[2] ;
	  P2 &= 0x1f;
		Delay(500);
		P2 = (P2 & 0x1f) | 0xc0;
		P0 = 0x01 << 1;
		P2 &= 0x1f;
		P2 = (P2 & 0x1f) | 0xe0;
		P0 = SMG_NUM[0] ;
		P2 &= 0x1f;
		Delay(500);
		P2 = (P2 & 0x1f) | 0xc0;
		P0 = 0x01 << 2;
		P2 &= 0x1f;
		P2 = (P2 & 0x1f) | 0xe0;
		P0 = SMG_NUM[2] ;
		P2 &= 0x1f;
		Delay(500);
		P2 = (P2 & 0x1f) | 0xc0;
		P0 = 0x01 << 3;
		P2 &= 0x1f;
		P2 = (P2 & 0x1f) | 0xe0;
		P0 = SMG_NUM[3] ;
		P2 &= 0x1f;
		Delay(500);
		P2 = (P2 & 0x1f) | 0xc0;
		P0 = 0x01 << 4;
		P2 &= 0x1f;
		P2 = (P2 & 0x1f) | 0xe0;
		P0 = 0xbf;
		P2 &= 0x1f;
		Delay(500);
		P2 = (P2 & 0x1f) | 0xc0;
		P0 = 0x01 << 5;
		P2 &= 0x1f;
		P2 = (P2 & 0x1f) | 0xe0;
		P0 = 0xbf ;
		P2 &= 0x1f;
		Delay(500);
		P2 = (P2 & 0x1f) | 0xc0;
		P0 = 0x01 << 6;
		P2 &= 0x1f;
		P2 = (P2 & 0x1f) | 0xe0;
		P0 = SMG_NUM[yue/10] ;
		P2 &= 0x1f;
		Delay(500);
		P2 = (P2 & 0x1f) | 0xc0;
		P0 = 0x01 << 7;
		P2 &= 0x1f;
		P2 = (P2 & 0x1f) | 0xe0;
		P0 = SMG_NUM[yue%10] ;
		P2 &= 0x1f;
		Delay(500);
}
void Delay1(unsigned int t)
{
	while(t--)
	{
		Display_SMG();
	}
}
void main()
{
	Init_Sys();
	while(1)
	{
		
		Display_SMG();
		yue++;
		if(yue > 12)
		{
			yue = 1;
		}
		Delay1(200);
	}
}