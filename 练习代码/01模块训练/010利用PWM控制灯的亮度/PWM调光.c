#include "reg52.h"

sbit L1 = P0^0;
sbit S7 = P3^0;//独立按键

void SelectHC573()
{
	P2 = (P2 & 0x1f) | 0x80;
}
//=================定时器相关函数=====
unsigned char count = 0;
unsigned char pwm_dut = 0;//占空比
void InitTimer0()
{
	TMOD = 0x01;
	TH0 = (65535 - 100) / 256;//    100us  10ms分成100份
	TL0 = (65535 - 100) % 256;
	
	ET0 = 1;
	EA = 1;
}

void ServiceTimer0() interrupt 1
{
	TH0 = (65535 - 100) / 256;//    100us  10ms分成100份
	TL0 = (65535 - 100) % 256;
	
	count++;
	if(count <= pwm_dut)
	{
		L1 = 0;
	}
	else if(count < 100)
	{
		L1 = 1;
	}
	else if(count == 100)
	{
		L1 = 0;
		count = 0;
	}
}
//===================================

//=================按键相关函数=====
void DelayKey(unsigned int t)
{
	while(t--);
}

unsigned char Stat = 0;
void ScanKeys()
{
	if(S7 == 0)
	{
		DelayKey(100);
		if(S7 == 0)
		{
			switch(Stat)
			{
				case 0:
					L1 = 0;
					TR0 = 1;
					pwm_dut = 10;
					Stat = 1;
				break;
				
				case 1:
					L1 = 0;
					pwm_dut = 50;
					Stat = 2;
				break;
				
				case 2:
					L1 = 0;
					pwm_dut = 90;
					Stat = 3;
				break;
				
				case 3:
					L1 = 1;
					TR0 = 0;
					Stat = 0;
				break;
			}
			while(S7 == 0);
		}
	}
//==================================
	
	
}

void main()
{
	SelectHC573();
	L1 = 1;
	InitTimer0();
	while(1)
	{
		ScanKeys();
	}
}