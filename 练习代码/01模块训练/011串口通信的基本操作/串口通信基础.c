#include "reg52.h"

sfr AUXR = 0x8e;

unsigned char urdat;

void GetByte(unsigned char dat);
void Init_Uart()
{
	TMOD = 0x20;
	TH1 = 0xfd;
	TL1 = 0xfd;
	ET1 = 0;		    //禁止定时器1中断
	TR1 = 1;		    //启动定时器1
	
	SCON = 0x50;
	AUXR = 0x00;
	
	
	
	ES = 1;
	EA = 1;
}

void ServiceUart() interrupt 4
{
	if(RI == 1)
	{
		RI = 0;//人工清零
		urdat = SBUF;
		GetByte(urdat + 1);
		GetByte(0x5a);
	  GetByte(0xa5);
	}
}

void GetByte(unsigned char dat)
{
	SBUF = dat;
	while(TI == 0);
	TI = 0;
}

void main()
{
	Init_Uart();
	GetByte(0x5a);
	GetByte(0xa5);
	while(1)
	{
		
	}
}

