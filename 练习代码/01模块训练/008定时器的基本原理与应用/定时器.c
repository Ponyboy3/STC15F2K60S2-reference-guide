#include "reg52.h"

sbit L1 = P0^0;
sbit L8 = P0^7;

void SelectHC573(unsigned char channel);
void Init_System();

void SelectHC573(unsigned char channel)
{
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
	}
}

void Init_System()
{
	SelectHC573(4);
	P0 = 0xff;
	SelectHC573(5);
	P0 = 0x00;
}

//============================
void Init_Timer0()
{
	TMOD = 0x01;
	TH0 = (65535 - 50000) / 256;
	TL0 = (65535 - 50000) % 256;
	
	ET0 = 1;
	EA = 1;
	TR0 = 1;
}

unsigned char count = 0;
void ServiceTimer0() interrupt 1
{
	TH0 = (65535 - 50000) / 256;
	TL0 = (65535 - 50000) % 256;
	
	count++;
	if(count % 10 == 0)
	{
		SelectHC573(4);
		L1 = ~L1;
	}
	if(count % 100 == 0)
	{
		SelectHC573(4);
		L8 = ~L8;
		count = 0;
	}
}
//============================
void main()
{
	//Init_System();
	
	Init_Timer0();
	while(1)
	{
		
	}
}