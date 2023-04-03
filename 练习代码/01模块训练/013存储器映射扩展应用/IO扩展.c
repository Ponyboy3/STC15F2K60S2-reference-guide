#include "reg52.h"


void Delay(unsigned int t)
{
	while(t--);
	while(t--);
}

void SelectHC573(unsigned char channel,unsigned int value)
{
	P0 = value;
	switch(channel)
	{
		case 4:
			P2 = (P2 & 0x1f) | 0x80;
		break;
		case 5:
			P2 = (P2 & 0x1f) | 0xa0;
		break;
		case 6:
			P2 = (P2 & 0x1f) | 0xc0;
		break;
		case 7:
			P2 = (P2 & 0x1f) | 0xe0;
		break;
		case 0:
			P2 = (P2 & 0x1f) | 0x00;
		break;
	}
	P2 = (P2 & 0x1f) | 0x00;//¹Ø±Õ¼Ä´æÆ÷
}

void LEDRunning()
{
	SelectHC573(4,0xf0);
	Delay(60000);
	Delay(60000);
	SelectHC573(4,0x0f);
	Delay(60000);
	Delay(60000);
	SelectHC573(4,0xff);
	Delay(60000);
	Delay(60000);
}

void SMGRunning()
{
	unsigned char i;
	for(i = 0;i < 8 ;i++)
	{
		SelectHC573(6,0x01 << i);
		SelectHC573(7,0x00);
		Delay(60000);
		Delay(60000);
	}
	SelectHC573(7,0xff);
	Delay(60000);
	Delay(60000);
}

void Init_Sys()
{
	SelectHC573(0,0x00);
	SelectHC573(4,0xff);
	SelectHC573(5,0x00);
	SelectHC573(6,0xff);
	SelectHC573(7,0xff);
}

void main()
{
	Init_Sys();
	while(1)
	{
		LEDRunning();
		SMGRunning();
	}
	
}